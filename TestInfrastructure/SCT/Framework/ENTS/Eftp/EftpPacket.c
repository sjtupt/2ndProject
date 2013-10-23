/*++
  The material contained herein is not a license, either        
  expressly or impliedly, to any intellectual property owned    
  or controlled by any of the authors or developers of this     
  material or to any contribution thereto. The material         
  contained herein is provided on an "AS IS" basis and, to the  
  maximum extent permitted by applicable law, this information  
  is provided AS IS AND WITH ALL FAULTS, and the authors and    
  developers of this material hereby disclaim all other         
  warranties and conditions, either express, implied or         
  statutory, including, but not limited to, any (if any)        
  implied warranties, duties or conditions of merchantability,  
  of fitness for a particular purpose, of accuracy or           
  completeness of responses, of results, of workmanlike         
  effort, of lack of viruses and of lack of negligence, all     
  with regard to this material and any contribution thereto.    
  Designers must not rely on the absence or characteristics of  
  any features or instructions marked "reserved" or             
  "undefined." The Unified EFI Forum, Inc. reserves any         
  features or instructions so marked for future definition and  
  shall have no responsibility whatsoever for conflicts or      
  incompatibilities arising from future changes to them. ALSO,  
  THERE IS NO WARRANTY OR CONDITION OF TITLE, QUIET ENJOYMENT,  
  QUIET POSSESSION, CORRESPONDENCE TO DESCRIPTION OR            
  NON-INFRINGEMENT WITH REGARD TO THE TEST SUITE AND ANY        
  CONTRIBUTION THERETO.                                         
                                                                
  IN NO EVENT WILL ANY AUTHOR OR DEVELOPER OF THIS MATERIAL OR  
  ANY CONTRIBUTION THERETO BE LIABLE TO ANY OTHER PARTY FOR     
  THE COST OF PROCURING SUBSTITUTE GOODS OR SERVICES, LOST      
  PROFITS, LOSS OF USE, LOSS OF DATA, OR ANY INCIDENTAL,        
  CONSEQUENTIAL, DIRECT, INDIRECT, OR SPECIAL DAMAGES WHETHER   
  UNDER CONTRACT, TORT, WARRANTY, OR OTHERWISE, ARISING IN ANY  
  WAY OUT OF THIS OR ANY OTHER AGREEMENT RELATING TO THIS       
  DOCUMENT, WHETHER OR NOT SUCH PARTY HAD ADVANCE NOTICE OF     
  THE POSSIBILITY OF SUCH DAMAGES.                              
                                                                
  Copyright 2006 - 2013 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010 - 2013, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:
 
  EftpPacket.c
 
Abstract:
  Eftp Packet buffer
 
--*/
#include "Efi.h"
#include "EftpMain.h"

STATIC NET_LIST_ENTRY mEftpFreePacketList[2];

STATIC UINT32         mEftpPacketBufferSize;
STATIC UINT32         mEftpPacketAllocateNum;
STATIC UINT32         mEftpPacketBufferUse;

UINT16
EftpCkSum (
  UINT16 *buf,
  UINT32 nwords
  )
/*++

Routine Description:

  Caculate checksum for Eftp packet.

Arguments:

  buf    - Buffer to be caculated.
  nwords - Length of the buffer in 2 octets.

Returns:

  Checksum result.

--*/
{
  UINT32  sum;
  UINT32  k;

  sum = 0;
  if (nwords < 0) {
    return 0xffff;
  }

  for (k = 0; k < nwords; k++) {
    sum += (UINT16) (*buf++);
    if (sum & 0xFFFF0000) {
      sum = (sum >> 16) + (sum & 0x0000ffff);
    }
  }

  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);

  return HTONS ((UINT16) (~sum));
}

STATIC
EFTP_PACKET_BUFFER *
EftpAllocNewPacket (
  IN UINT32 DataSize
  )
/*++

Routine Description:

  Allocate a new packet buffer, then initialize it

Arguments:

  DataSize - The size of the buffer to hold packet, not including the structure size

Returns:

  NULL                       - Failed to allocate the data
  Point to new packet buffer - Successfully allocated and initialized the buffer

--*/
{
  EFTP_PACKET_BUFFER                    *Buf;
  EFI_EFTP_PACKET                       *Packet;
  EFI_MANAGED_NETWORK_COMPLETION_TOKEN  *Mnp;

  //
  // limit the minium packet buffer size to improve the reuse of small packet buffer
  //
  if ((DataSize > 0) && (DataSize < EFTP_MINI_PACKET_BUFFER_SIZE)) {
    DataSize = EFTP_MINI_PACKET_BUFFER_SIZE;
  }

  Buf = (EFTP_PACKET_BUFFER *) NetAllocateZeroPool (sizeof (EFTP_PACKET_BUFFER));

  if (Buf == NULL) {
    EFTP_DEBUG_ERROR ((L"EftpAllocPacket: failed to allocate a token for packet buffer\n"));
    return NULL;

  }
  //
  // Init EftpToken, EftpToken has {Signature, MnpToken, *Private}
  //
  Buf->EftpToken.Signature = EFTP_TOKEN_SIGNATURE;

  //
  // Init EftpToken's MnpToken, MnpToken has: {Event, Status, *TxData}. Event isn't created here
  //
  Mnp                 = &Buf->EftpToken.MnpToken;
  Mnp->Packet.TxData  = &Buf->EftpMnpTxData;
  Mnp->Status         = EFI_SUCCESS;

  Buf->DataSize       = DataSize;

  if (DataSize != 0) {
    Packet = (EFI_EFTP_PACKET *) NetAllocatePool (DataSize);
    if (Packet == NULL) {
      EFTP_DEBUG_ERROR ((L"EftpAllocPacket: failed to allocate data memory for packet buffer\n"));
      NetFreePool (Buf);
      return NULL;

    }

    Buf->Packet = Packet;
    Buf->PacketSize = DataSize;
    Buf->EftpMnpTxData.FragmentCount = 1;
    Buf->EftpMnpTxData.FragmentTable[0].FragmentLength = 0;
    Buf->EftpMnpTxData.FragmentTable[0].FragmentBuffer = Packet;
  }

  Buf->RefCnt = 1;

  mEftpPacketBufferSize += DataSize;
  return Buf;

}

EFTP_PACKET_BUFFER *
EftpAllocPacket (
  IN UINT32 DataSize
  )
/*++

Routine Description:

  Allocate a packet buffer from the pool first. If not available, from the system memory.

Arguments:

  DataSize - The size of the buffer to hold packet, not including the structure size

Returns:

  NULL                       - Failed to allocate the data
  Point to new packet buffer - Successfully allocated and initialized the buffer

--*/
{
  NET_LIST_ENTRY      *Entry;
  EFTP_PACKET_BUFFER  *Buf;
  NET_LIST_ENTRY      *Head;

  mEftpPacketAllocateNum++;

  Head = (DataSize < EFTP_BUFFER_DIV ? &mEftpFreePacketList[0] : &mEftpFreePacketList[1]);

  //
  // LIST_FOR_EACH is NOT delete safe, escape once found
  //
  LIST_FOR_EACH (Entry, Head) {
    Buf = LIST_ENTRY (Entry, EFTP_PACKET_BUFFER, List);

    if (Buf->DataSize >= DataSize) {
      EFTP_DEBUG_VERBOSE ((L"EftpAllocPacket: allocate packet from buffer\n"));

      LIST_REMOVE_ENTRY (Entry);

      Buf->EftpToken.Signature        = EFTP_TOKEN_SIGNATURE;
      Buf->EftpToken.MnpToken.Status  = EFI_SUCCESS;

      mEftpPacketBufferUse++;
      return Buf;
    }
  }

  EFTP_DEBUG_VERBOSE ((L"EftpAllocPacket: allocate a new packet buffer\n"));
  return EftpAllocNewPacket (DataSize);
}

STATIC
VOID
EftpFreePacket (
  IN EFTP_PACKET_BUFFER*Buf
  )
/*++

Routine Description:

  Release the packet buffer to system memory

Arguments:

  Buf - Point to the packet buffer

Returns:

  None
  
--*/
{
  ASSERT (Buf != NULL);
  ASSERT (Buf->RefCnt == 1);

  mEftpPacketBufferSize -= Buf->DataSize;

  if (Buf->DataSize) {
    NetFreePool (Buf->Packet);
  }

  NetFreePool (Buf);
}

VOID
EftpReleasePacket (
  IN EFTP_PACKET_BUFFER*Buf
  )
/*++

Routine Description:

  Release the packet to the eftp buffer, if the buffer occupies too 
  much resource, release it to the system memory

Arguments:

  Buf  - Point to the packet buffer

Returns:

  None

--*/
{
  NET_LIST_ENTRY  *Head;
  ASSERT (Buf && (Buf->RefCnt >= 1));

  if (Buf->RefCnt == 1) {

    if (Buf->EftpToken.MnpToken.Event) {
      gBS->CloseEvent (Buf->EftpToken.MnpToken.Event);
      Buf->EftpToken.MnpToken.Event = NULL;

    }

    if (mEftpPacketBufferSize < EFTP_MAX_BUFFER_SIZE) {
      //
      // Insert it at head to improve performance
      //
      Head = (Buf->DataSize < EFTP_BUFFER_DIV ? &mEftpFreePacketList[0] : &mEftpFreePacketList[1]);

      LIST_INSERT_HEAD (Head, &Buf->List);

    } else {
      EftpFreePacket (Buf);

    }

    return ;
  }

  Buf->RefCnt--;
}

VOID
EftpHoldPacket (
  IN EFTP_PACKET_BUFFER*Buf
  )
/*++

Routine Description:

  Increase the packet buffer's reference count

Arguments:

  Buf  - Point to the packet buffer
  
Returns:

  None

--*/
{
  ASSERT (Buf);
  Buf->RefCnt++;
}

EFTP_PACKET_BUFFER *
EftpCopyPacket (
  IN EFTP_PACKET_BUFFER*OldBuf
  )
/*++

Routine Description:

  duplicate a packet buffer

Arguments:

  OldBuf   - packet buffer to be duplicated

Returns:

  NULL                       - Failed to allocate a new packet  buffer
  Point to new packet buffer - Successfully allocated and initialized the buffer

--*/
{
  EFTP_PACKET_BUFFER  *NewBuf;

  ASSERT (OldBuf);

  NewBuf = EftpAllocPacket (OldBuf->DataSize);

  if (NewBuf == NULL) {
    EFTP_DEBUG_ERROR ((L"Mftp4CopyPacket: failed to allocate a new packet buffer\n"));
    return NULL;
  }

  NewBuf->PacketSize = OldBuf->PacketSize;

  NewBuf->EftpToken.Private = OldBuf->EftpToken.Private;
  NewBuf->EftpToken.MnpToken.Status = EFI_SUCCESS;

  NewBuf->EftpMnpTxData.DataLength = OldBuf->EftpMnpTxData.DataLength;
  NewBuf->EftpMnpTxData.FragmentCount = OldBuf->EftpMnpTxData.FragmentCount;

  NewBuf->EftpMnpTxData.FragmentTable[0].FragmentLength = OldBuf->EftpMnpTxData.FragmentTable[0].FragmentLength;

  NetCopyMem (NewBuf->Packet, OldBuf->Packet, OldBuf->PacketSize);

  return NewBuf;
}

VOID
EftpInitPacketBuffer (
  VOID
  )
/*++

Routine Description:

  Initialize the packet buffer pool

Arguments:

  None

Returns:

  None

--*/
{
  UINTN Index;

  EFTP_DEBUG_VERBOSE ((L"EftpInitPacketBuffer: init packet buffer\n"));
  for (Index = 0; Index < 2; Index++) {
    LIST_INIT (&mEftpFreePacketList[Index]);
  }

  mEftpPacketBufferUse    = 0;
  mEftpPacketAllocateNum  = 0;
  mEftpPacketBufferSize   = 0;
}

VOID
EftpClosePacketBuffer (
  VOID
  )
/*++

Routine Description:

  Release packet buffer pools and all the associated packet buffers

Arguments:

  None

Returns:

  None

--*/
{
  NET_LIST_ENTRY      *Head;
  EFTP_PACKET_BUFFER  *Buf;
  UINTN               Index;

  EFTP_DEBUG_VERBOSE (
    (L"EftpClosePacketBuffer: packet allocation: %d out of %d are from buffer\n",
    mEftpPacketBufferUse,
    mEftpPacketAllocateNum)
    );

  EFTP_DEBUG_VERBOSE (
    (L"EftpClosePacketBuffer: current %d bytes in buffer\n",
    mEftpPacketBufferSize)
    );

  for (Index = 0; Index < 2; Index++) {
    Head = &mEftpFreePacketList[Index];

    while (!LIST_IS_EMPTY (Head)) {

      Buf = LIST_HEAD (Head, EFTP_PACKET_BUFFER, List);

      LIST_REMOVE_ENTRY (&Buf->List);
      EftpFreePacket (Buf);
    }
  }

  ASSERT (mEftpPacketBufferSize == 0);

  mEftpPacketAllocateNum  = 0;
  mEftpPacketBufferUse    = 0;
}

EFI_STATUS
EftpSendPacket (
  IN EFTP_IO_PRIVATE    *Private,
  IN EFTP_PACKET_BUFFER *Buf
  )
/*++

Routine Description:

  Send out a packet to the session's UDP port.
  Since Eftp client doesn't send packet to multicast address,
  the UDP port isn't a explict parameter.

Arguments:

  Private  - The eftp session's private data
  Buf      - Packet to send
  
Returns:

  EFI_SUCCESS - Packet is successfully sent.
  Others      - Failed to send the packet.

--*/
{
  EFI_STATUS  Status;

  ASSERT ((Private != NULL) && (Buf != NULL));

  //
  // The callback function held a point to it. The callback function and Pending packet
  // list should be synchronized. When packet successfully sent, add it to pending packet
  // list. When recycled, also remove it from the pending list.
  //
  EftpHoldPacket (Buf);

  ASSERT (!EntryInList (&Private->PendingPacketList, &Buf->List));

  Private->NPendingPacket++;
  LIST_INSERT_TAIL (&Private->PendingPacketList, &Buf->List);

  Status = Private->ManagedNetwork->Transmit (Private->ManagedNetwork, &Buf->EftpToken.MnpToken);

  if (EFI_ERROR (Status)) {
    //
    // If MnpIo->Transmit returns EFI_ACCESS_DENIED, the same event is already in the transmit
    // queue, according to the design, it isn't possible. Check it because it is easy to do it wrong
    //
    ASSERT (Status != EFI_ACCESS_DENIED);

    EFTP_DEBUG_ERROR ((L"EftpSendPacket: UdpIo->Transmit returned with %r.\n", Status));

    Private->NPendingPacket--;
    LIST_REMOVE_ENTRY (&Buf->List);

    EftpReleasePacket (Buf);
    Private->Result = EFI_DEVICE_ERROR;

  }

  return Status;
}

EFI_STATUS
EftpStartRcv (
  IN EFTP_IO_PRIVATE*Private
  )
/*++

Routine Description:

  Start receiving packet on the session's unicast port.

Arguments:

  Private  - Eftp session's private data

Returns:

  EFI_SUCCESS - Successfully started receiving on the udp port
  Others      - Failed to start receiving on the UDP port.
  
--*/
{
  EFI_STATUS  Status;

  ASSERT (Private);

  Status = Private->ManagedNetwork->Receive (Private->ManagedNetwork, &Private->UniToken.MnpToken);

  //
  // If MnpIo->Receive returns EFI_ACCESS_DENIED, the same transmit token
  // with the same event has already been in the receive queue. Silently ignore it
  //
  if (EFI_ERROR (Status) && (Status != EFI_ACCESS_DENIED)) {
    EFTP_DEBUG_ERROR ((L"EftpStartRcv: MnpIo->Receive returned with %r.\n", Status));
    Private->Result = EFI_DEVICE_ERROR;

    return Status;
  }

  return EFI_SUCCESS;

}

EFTP_PACKET_BUFFER *
EftpBuildErrorPacket (
  IN EFTP_IO_PRIVATE  *Private,
  IN EFTP_ERRINFO     *Err,
  IN EFI_EVENT_NOTIFY Func
  )
/*++

Routine Description:

  Build an error packet.

Arguments:

  Private   - Eftp session's private data
  Err       - The error information to add to the packet.
  Func      - Event function to call when recycled.

Returns:

  NULL                - Failed to allocate the packet.
  Point to new packet - Successfully allocated a new packet buffer and initialized it.
  
--*/
{
  EFTP_PACKET_BUFFER  *Buf;
  EFI_EFTP_PACKET     *Packet;
  EFI_STATUS          Status;
  UINT32              Len;
  CHAR8               *P;

  ASSERT (Private && Err);

  Len = EFTP_HEADER_LEN + (UINT32) NetAsciiStrLen (Err->Desc) + 1;
  Buf = EftpAllocPacket (Len);

  if (Buf == NULL) {
    EFTP_DEBUG_ERROR ((L"EftpBuildErrorPacket: Failed to allocate a packet buffer.\n"));

    Private->Result = EFI_OUT_OF_RESOURCES;
    return Buf;

  }

  Packet  = Buf->Packet;
  P       = NetAsciiStrCpy (Packet->Error.ErrorMessage, Err->Desc);

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  NET_TPL_EVENT,
                  Func,
                  Buf,
                  &Buf->EftpToken.MnpToken.Event
                  );

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpBuildErrorPacket: failed to create an event for the send token\n"));

    Private->Result = EFI_OUT_OF_RESOURCES;
    EftpReleasePacket (Buf);
    return NULL;

  }

  Packet->Hdr.OpCode = HTONS (EFI_EFTP_OPCODE_ERROR);
  Packet->Hdr.CodeVar = HTONS (Err->Code);
  Packet->Hdr.Length = HTONS ((UINT16) Len);
  Packet->Hdr.CheckSum = 0;
  Packet->Hdr.CheckSum = HTONS (EftpCkSum ((UINT16 *) Packet, Len / 2));
  Buf->PacketSize = Len;
  Buf->EftpMnpTxData.DataLength = Len;
  Buf->EftpMnpTxData.FragmentCount = 1;
  Buf->EftpMnpTxData.FragmentTable[0].FragmentLength = Len;
  NetCopyMem (
    (CHAR8 *) Buf->EftpMnpTxData.FragmentTable[0].FragmentBuffer,
    Packet,
    Len
    );
  Buf->EftpMnpTxData.ProtocolType       = ENTS_FILE_TRANSER_PROTOCOL_MNP_TYPE;
  Buf->EftpMnpTxData.HeaderLength       = 0;
  Buf->EftpMnpTxData.DestinationAddress = NetAllocatePool (sizeof (EFI_MAC_ADDRESS));
  Buf->EftpMnpTxData.SourceAddress      = NetAllocatePool (sizeof (EFI_MAC_ADDRESS));
  NetCopyMem (
    Buf->EftpMnpTxData.DestinationAddress,
    &Private->ConfigData.DstMac,
    NET_ETHER_ADDR_LEN
    );
  NetCopyMem (
    Buf->EftpMnpTxData.SourceAddress,
    &Private->ConfigData.SrcMac,
    NET_ETHER_ADDR_LEN
    );

  Buf->EftpToken.Private = Private;

  return Buf;
}

EFTP_PACKET_BUFFER *
EftpBuildRequest (
  IN EFTP_IO_PRIVATE              *Private,
  IN UINT16                       OpCode,
  IN CHAR8                        *Filename,
  IN CHAR8                        *Mode,
  IN UINT32                       NOption,
  IN EFI_EFTP_OPTION              *OptionList,
  IN EFI_EVENT_NOTIFY             Func
  )
/*++

Routine Description:

  Build a request packet, be it a RRQ/WRQ.

Arguments:

  Private    - Eftp session's private data
  OpCode     - OpCode to use, for example : RRQ/WRQ/READ_DIRECTORY
  Filename   - File name to read or write
  Mode       - Transfer mode, default to octet
  NOption    - The Number of options in the option list
  OptionList - Option list, it is a group of name/value pairs
  Func       - Event function to call when recycled

Returns:

  NULL                - Failed to allocate new packet buffer for the request packet.
  Point to new packet - Successfully allocated and initialized the packet.

--*/
{
  EFTP_PACKET_BUFFER  *Buf;
  EFI_EFTP_PACKET     *Packet;
  EFI_STATUS          Status;
  UINT32              Len;
  UINTN               Index;
  CHAR8               *P;

  ASSERT (Filename);

  if (Mode == NULL) {
    Mode = "octet";
  }

  Len = EFTP_HEADER_LEN + (UINT32) NetAsciiStrLen (Filename) + (UINT32) NetAsciiStrLen (Mode) + 2;

  if ((NOption != 0) && (OptionList != NULL)) {

    for (Index = 0; Index < NOption; Index++) {
      if (OptionList[Index].OptionStr == NULL || OptionList[Index].ValueStr == NULL) {

        EFTP_DEBUG_ERROR ((L"EftpBuildRequest: Invalid option lists.\n"));

        Private->Result = EFI_INVALID_PARAMETER;
        return NULL;
      }

      Len += (UINT32) NetAsciiStrLen (OptionList[Index].OptionStr) + (UINT32) NetAsciiStrLen (OptionList[Index].ValueStr) + 2;
    }

  }

  Buf = EftpAllocPacket (Len);

  if (Buf == NULL) {
    EFTP_DEBUG_ERROR ((L"EftpBuildRequest: Failed to allocate packet buffer.\n"));

    Private->Result = EFI_OUT_OF_RESOURCES;
    return NULL;

  }

  Packet = Buf->Packet;
  //
  // WRQ and RRQ packet have the same format, do NOT be confused by the variable name
  //
  P = Packet->Rrq.Filename;

  P = NetAsciiStrCpy (P, Filename);
  P = NetAsciiStrCpy (P, Mode);

  if (NOption != 0 && OptionList != NULL) {
    for (Index = 0; Index < NOption; ++Index) {
      P = NetAsciiStrCpy (P, OptionList[Index].OptionStr);
      P = NetAsciiStrCpy (P, OptionList[Index].ValueStr);
    }
  }

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  NET_TPL_EVENT,
                  Func,
                  Buf,
                  &Buf->EftpToken.MnpToken.Event
                  );

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpBuildRequest: failed to create event for the packet buffer\n"));

    Private->Result = EFI_OUT_OF_RESOURCES;
    EftpReleasePacket (Buf);
    return NULL;

  }

  Packet->Hdr.OpCode    = HTONS (OpCode);
  Packet->Hdr.CodeVar   = 0;
  Packet->Hdr.Length    = HTONS ((UINT16) Len);
  Packet->Hdr.CheckSum  = 0;
  Packet->Hdr.CheckSum  = HTONS (EftpCkSum ((UINT16 *) Packet, Len / 2));
  Buf->PacketSize       = Len;

  Buf->EftpMnpTxData.DataLength                       = Len;
  Buf->EftpMnpTxData.HeaderLength                     = 0;
  Buf->EftpMnpTxData.FragmentCount                    = 1;
  Buf->EftpMnpTxData.FragmentTable[0].FragmentLength  = Len;
  NetCopyMem (
    (CHAR8 *) Buf->EftpMnpTxData.FragmentTable[0].FragmentBuffer,
    Packet,
    Len
    );
  Buf->EftpMnpTxData.FragmentTable[0].FragmentBuffer  = (VOID *) Packet;
  Buf->EftpMnpTxData.ProtocolType                     = ENTS_FILE_TRANSER_PROTOCOL_MNP_TYPE;
  Buf->EftpMnpTxData.HeaderLength                     = 0;
  Buf->EftpMnpTxData.DestinationAddress               = NetAllocatePool (sizeof (EFI_MAC_ADDRESS));
  Buf->EftpMnpTxData.SourceAddress                    = NetAllocatePool (sizeof (EFI_MAC_ADDRESS));
  NetCopyMem (
    Buf->EftpMnpTxData.DestinationAddress,
    &Private->ConfigData.DstMac,
    NET_ETHER_ADDR_LEN
    );
  NetCopyMem (
    Buf->EftpMnpTxData.SourceAddress,
    &Private->ConfigData.SrcMac,
    NET_ETHER_ADDR_LEN
    );

  Buf->EftpToken.Private = Private;

  return Buf;

}

EFI_STATUS
EftpSendError (
  IN EFTP_IO_PRIVATE  *Private,
  IN EFTP_ERRINFO     *Err,
  IN EFI_EVENT_NOTIFY Func
  )
/*++

Routine Description:

  Build an error packet, then send it

Arguments:

  Private    - The Eftp session's private data
  Err        - Error information to be included in the packet
  Func       - Event function to call when recycled.

Returns:

  EFI_OUT_OF_RESOURCES - Failed to allocate memory for new packet.
  EFI_SUCCESS          - Successfully built and sent the packet 
  Others               - Failed to send the packet
  
--*/
{
  EFI_STATUS          Status;
  EFTP_PACKET_BUFFER  *Buf;

  ASSERT (Private && Err);

  Buf = EftpBuildErrorPacket (Private, Err, Func);
  if (Buf == NULL) {
    EFTP_DEBUG_ERROR ((L"EftpSendError: failed to build an error packet, double error\n"));

    return EFI_OUT_OF_RESOURCES;

  }

  Status = EftpSendPacket (Private, Buf);
  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpSendError: failed to send an error packet, double error\n"));

    EftpReleasePacket (Buf);
    return Status;

  }

  if (Private->LastCtrl) {
    EftpReleasePacket (Private->LastCtrl);

  }

  Private->LastCtrl = Buf;

  return EFI_SUCCESS;

}

EFI_STATUS
EftpResendPacket (
  IN EFTP_IO_PRIVATE     *Private,
  IN EFTP_PACKET_BUFFER  *Buf,
  IN BOOLEAN             Copy,
  IN EFI_EVENT_NOTIFY    Func
  )
/*++

Routine Description:

  Retransmit the packet, if necessary copy it.

Arguments:

  Private   - The Eftp session's private data
  Buf       - The packet to transmit.
  Copy      - Whether it is necessary to copy it.
  Func      - Event function to call when recycled.

Returns:

  EFI_SUCCESS             - Packet is successfully retransmitted.
  EFI_OUT_OF_RESOURCES    - Failed to allocate memory to copy the packet
  Others                  - Failed to build or send the packet.
  
--*/
{
  EFI_STATUS          Status;
  EFTP_PACKET_BUFFER  *Tmp;

  ASSERT (Private && Buf);

  if (!Copy) {
    Status = EftpSendPacket (Private, Buf);
    if (EFI_ERROR (Status)) {
      EFTP_DEBUG_ERROR ((L"EftpResendPacket: failed to retransmit the packet\n"));

      return Status;
    }

    return EFI_SUCCESS;
  }
  //
  // Copy the buffer and send it
  //
  Tmp = EftpCopyPacket (Buf);
  if (Tmp == NULL) {
    EFTP_DEBUG_ERROR ((L"EftpResendPacket: faile to copy the buffer\n"));

    Private->Result = EFI_OUT_OF_RESOURCES;
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  NET_TPL_EVENT,
                  Func,
                  Tmp,
                  &Tmp->EftpToken.MnpToken.Event
                  );

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpBuildRequest: failed to create event for the packet buffer\n"));

    Private->Result = EFI_OUT_OF_RESOURCES;
    EftpReleasePacket (Tmp);
    return EFI_OUT_OF_RESOURCES;

  }

  Status = EftpSendPacket (Private, Tmp);

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpResendPacket: faile to send the copy of  the buffer\n"));

    EftpReleasePacket (Tmp);
    return Status;
  }

  EftpReleasePacket (Tmp);
  return EFI_SUCCESS;
}

BOOLEAN
EntryInList (
  IN NET_LIST_ENTRY*Head,
  IN NET_LIST_ENTRY*Entry
  )
/*++

Routine Description:

  Check whether the Entry is in the list of Head.

Arguments:

  Head   - The list's head to be checked against.
  Entry  - The entry.

Returns:

  TRUE   - Entry is in the list
  FALSE  - Entry not in the list

--*/
{
  NET_LIST_ENTRY  *Cur;

  ASSERT (Head && Entry);

  LIST_FOR_EACH (Cur, Head) {
    if (Cur == Entry) {
      return TRUE;
    }
  }

  return FALSE;
}
