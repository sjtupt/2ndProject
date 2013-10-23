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
    EftpMain.c
 
Abstract:

	  Interface routine for Eftp
	  
--*/
#include "Efi.h"
#include "EftpMain.h"

CHAR8 *mSupportedOptions[EFTP_SUPPORTED_OPTIONS_COUNT] = {
  "timeout",
  "blksize",
  "tsize",
};

CHAR8 *mUnsupportedOptions[EFTP_UNSUPPORTED_OPTIONS_COUNT] = {
  "restart",
  "session",
  "pktdelay",
  "stream""multicast""bigblk#",
};

STATIC
EFI_STATUS
EftpInitReadHandler (
  IN EFTP_IO_PRIVATE   *Private,
  IN UINT16            OpCode
  )
/*++

Routine Description:

  Initialize the eftp session for downloading, It also sends out the request packet

Arguments:

  Private  - The Eftp session's private data
  OpCode   - OpCode to use for this downloading, RRQ/READ_DIRECTORY

Returns:

  EFI_OUT_OF_RESOURCES  - No enough resources to initialize a read handler
  EFI_SUCCESS           - Successfully initialized a read handler
  Others                - Failed to initialize the read handler
--*/
{
  EFI_STATUS                      Status;
  EFI_TPL                         OldTpl;

  OldTpl = NET_RAISE_TPL(NET_TPL_LOCK);

  //
  // Construct the container of handler of eftp read process
  //
  Private->RrqState = EftpInitRrqState (Private);
  if (Private->RrqState == NULL) {
    EFTP_DEBUG_ERROR ((L"Eftp.ReadFile out of resource.\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Construct the rx callback and timer callback for eftp read process
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  NET_TPL_EVENT,
                  EftpRrqRxCallback,
                  &Private->UniToken,
                  &Private->UniToken.MnpToken.Event
                  );
  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpReadFile: Failed to create  returned %r\n", Status));
    return Status;
  }

  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  NET_TPL_EVENT,
                  EftpRrqTimer,
                  Private,
                  &Private->TimeoutEvent
                  );

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpReadFile: Create timeout event faile with return %r\n", Status));
    return Status;
  }
  //
  // Change the state machine of eftp
  //
  Private->State = EFTP_RRQ_SENT;

  //
  // Assemble a new read request packet and send it out.
  //
  Status = EftpBuildRrq (Private, OpCode);

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"ReadFile: failed to build a request packet%r.\n", Status));
    return Status;
  }

  NET_RESTORE_TPL(OldTpl);

  Status = EftpSendPacket (Private, Private->Request);

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EFTP.ReadFile send packet failed with %r.\n", Status));
    return Status;
  }

  return EFI_SUCCESS;

}

STATIC
EFI_STATUS
EftpInitWriteHandler (
  IN EFTP_IO_PRIVATE *Private
  )
/*++

Routine Description:

  Initialize the eftp session for write, it also send out a WRQ packet

Arguments:

  Private    - The Eftp session's private data

Returns:

  EFI_OUT_OF_RESOURCES  - No enough resources to initialize a read handler
  EFI_SUCCESS           - Successfully initialized a read handler
  Others                - Failed to initialize the read handler

--*/
{
  EFI_STATUS                      Status;
  EFI_TPL                         OldTpl;

  OldTpl = NET_RAISE_TPL(NET_TPL_LOCK);

  //
  // Construct the container of handler of eftp write process
  //
  Private->WrqState = EftpInitWrqState (Private);
  if (Private->WrqState == NULL) {
    EFTP_DEBUG_ERROR ((L"Eftp.WriteFile out of resource.\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Construct the rx callback and timer callback for eftp  write process
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  NET_TPL_EVENT,
                  EftpWrqRxCallback,
                  &Private->UniToken,
                  &Private->UniToken.MnpToken.Event
                  );

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpWriteFile: Failed to create  returned %r\n", Status));
    return Status;
  }

  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  NET_TPL_EVENT,
                  EftpWrqTimer,
                  Private,
                  &Private->TimeoutEvent
                  );

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpWriteFile: Create timeout event faile with return %r\n", Status));
    return Status;
  }
  //
  // Change the state machine of eftp
  //
  Private->State = EFTP_WRQ_SENT;

  //
  // Assemble a new write request packet and send it out.
  //
  Status = EftpBuildWrq (Private);

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"ReadFile: failed to build a request packet%r.\n", Status));
    return Status;
  }

  NET_RESTORE_TPL(OldTpl);

  Status = EftpSendPacket (Private, Private->Request);

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EFTP.ReadFile send packet failed with %r.\n", Status));
    return Status;
  }

  return EFI_SUCCESS;

}

STATIC
EFI_STATUS
EftpInitPrivate (
  IN EFI_EFTP_PROTOCOL   *This,
  IN EFI_EFTP_TOKEN      *Token,
  IN UINT16              OpCode
  )
/*++

Routine Description:

  Check the parameter, then initialize the session accroding to user data

Arguments:

  This      - Eftp protocol
  Token     - User provided toke to specify the action.
  OpCode    - OpCode to use. 

Returns:

  EFI_INVALID_PARAMETER   - Invalid parameter provided.
  EFI_NOT_STARTED         - Session isn't configured
  EFI_ACCESS_DENIED       - Session is busy
  EFI_DEVICE_ERROR        - Failed to config the underlying network stack

--*/
{
  EFTP_IO_PRIVATE                 *Private;
  EFI_MANAGED_NETWORK_CONFIG_DATA MnpConfigData;
  EFI_TPL                         OldTpl;
  EFI_STATUS                      Status;

  if (!This || !Token) {
    EFTP_DEBUG_ERROR ((L"Eftp.Read or Eftp.Write function called with invalid paramter.\n"));
    return EFI_INVALID_PARAMETER;
  }

  if ((Token->OptionCount && !Token->OptionList) || (!Token->Filename)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EftpCheckOption (Token->OptionList, Token->OptionCount, OpCode);
  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpInitPrivate: failed to check option\n"));
    return Status;
  }

  if ((OpCode == EFI_EFTP_OPCODE_RRQ) && !Token->CheckPacket && (!Token->Buffer || (Token->BufferSize == 0))) {
    return EFI_INVALID_PARAMETER;
  }

  if ((OpCode == EFI_EFTP_OPCODE_WRQ) && !Token->PacketNeeded && (!Token->Buffer || (Token->BufferSize == 0))) {
    return EFI_INVALID_PARAMETER;
  }
  Private = EFTP_IO_PRIVATE_FROM_THIS (This);

  ASSERT (Private->Signature == EFTP_IO_PRIVATE_SIGNATURE);

  if (Private->Configured != TRUE) {
    return EFI_NOT_STARTED;

  }

  if (Private->State != EFTP_INIT) {
    return EFI_ACCESS_DENIED;

  }
  EftpInitOption (&Private->Option);

  Private->UserEvtFired = FALSE;
  Private->SynFinished  = FALSE;

  //
  // Initialize the communication parameter used in the eftp configdata
  //
  gBS->CopyMem (
        &Private->InitialServer.DstMac,
        &Private->ConfigData.DstMac,
        NET_ETHER_ADDR_LEN
        );
  Private->MaxNRetry                = Private->ConfigData.TryCount;
  Private->Timeout                  = Private->ConfigData.TimeoutValue;

  Private->Result                   = EFI_SUCCESS;
  Private->NRetry                   = 0;
  Private->Request                  = NULL;
  Private->LastCtrl                 = NULL;
  Private->NextBlkNo                = 1;
  Private->Eof                      = FALSE;
  Private->LastBlkNo                = 0;
  Private->RrqState                 = NULL;
  Private->WrqState                 = NULL;

  Private->TimeoutEvent             = NULL;
  Private->UniToken.MnpToken.Event  = NULL;
  Private->NPendingPacket           = 0;
  LIST_INIT (&Private->PendingPacketList);

  Private->Token = Token;

  //
  // In the light of the value in token->ovveride data to change some value of private structure
  //
  if (Token->OverrideData != NULL) {
    Private->MaxNRetry  = Token->OverrideData->TryCount;
    Private->Timeout    = Token->OverrideData->TimeoutValue;
  }

  if (Private->MaxNRetry == 0) {
    Private->MaxNRetry = EFTP_DEFAULT_RETRY;
  }

  if (Private->Timeout == 0) {
    Private->Timeout = Private->Option.Timeout;
  }
  //
  // BUGBUG:
  //
  Private->Server = Private->InitialServer;

  //
  // Configure the MNP handle
  //
  NetZeroMem (&MnpConfigData, sizeof (EFI_MANAGED_NETWORK_CONFIG_DATA));
  MnpConfigData.EnableBroadcastReceive    = FALSE;
  MnpConfigData.EnableMulticastReceive    = FALSE;
  MnpConfigData.EnablePromiscuousReceive  = FALSE;
  MnpConfigData.EnableReceiveTimestamps   = FALSE;
  MnpConfigData.EnableUnicastReceive      = TRUE;
  MnpConfigData.ProtocolTypeFilter        = ENTS_FILE_TRANSER_PROTOCOL_MNP_TYPE;
  MnpConfigData.ReceivedQueueTimeoutValue = 0;
  MnpConfigData.TransmitQueueTimeoutValue = 0;

  //
  // Configure the MNP IO instance used by eftp
  //
  Status = Private->ManagedNetwork->Configure (Private->ManagedNetwork, NULL);
  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpReadFile: failed to clean the MnpIo's configuration\n"));
    return EFI_DEVICE_ERROR;
  }

  Status = Private->ManagedNetwork->Configure (Private->ManagedNetwork, &MnpConfigData);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // BUGBUG: Call RaiseTPL for now. I don't want to use NET_TRY_LOCK, this isn't a lock
  // but similar to disable interrupt because the same TPL task can preempt each other.
  // define a macro later.
  //
//  OldTpl = gBS->RaiseTPL (NET_TPL_LOCK);

  //
  // Begin the eftp process.
  //
  if (EFI_EFTP_OPCODE_RRQ == OpCode) {
    Status = EftpInitReadHandler (Private, OpCode);
  }

  if (EFI_EFTP_OPCODE_WRQ == OpCode) {
    Status = EftpInitWriteHandler (Private);
  }

  if (EFI_ERROR (Status)) {
    goto CleanUp;
  }

  OldTpl = NET_RAISE_TPL(NET_TPL_LOCK);

  //
  // Initialize a Receive token and send it to MNP layer
  //
  Private->UniToken.Signature       = EFTP_TOKEN_SIGNATURE;
  Private->UniToken.MnpToken.Status = EFI_SUCCESS;
  Private->UniToken.Private         = Private;

  NET_RESTORE_TPL(OldTpl);

  Status = EftpStartRcv (Private);

  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpInitPrivate: failed to start receive %r.\n", Status));
    goto CleanUp;

  }
  EftpSetTimer (Private, Private->Timeout, 0);

//  gBS->RestoreTPL (OldTpl);

  if (Private->Token->Event) {
    return Status;
  }
  //
  // Poll to finishe this eftp process if user do not provide any event
  //
  while (!Private->SynFinished) {
    This->Poll (This);
  }

  EFTP_DEBUG_VERBOSE ((L"Token->Status = %r\n", Private->Token->Status));

  //
  // Configure NULL the MNP IO instance used by eftp
  //
  Status = Private->ManagedNetwork->Configure (Private->ManagedNetwork, NULL);
  if (EFI_ERROR (Status)) {
    EFTP_DEBUG_ERROR ((L"EftpReadFile: failed to clean the MnpIo's configuration\n"));
    return EFI_DEVICE_ERROR;
  }
  
  if (Private->UniToken.MnpToken.Event) {
    gBS->CloseEvent (Private->UniToken.MnpToken.Event);
    Private->UniToken.MnpToken.Event = NULL;
  }

  return Private->Token->Status;

CleanUp:
  Private->ManagedNetwork->Configure (Private->ManagedNetwork, NULL);

  OldTpl = NET_RAISE_TPL(NET_TPL_LOCK);

  if (Private->UniToken.MnpToken.Event) {
    gBS->CloseEvent (Private->UniToken.MnpToken.Event);
    Private->UniToken.MnpToken.Event = NULL;
  }

  if (Private->Request) {
    EftpReleasePacket (Private->Request);
    Private->Request = NULL;
  }

  if (Private->TimeoutEvent) {
    gBS->CloseEvent (Private->TimeoutEvent);
    Private->TimeoutEvent = NULL;
  }

  Private->State = EFTP_INIT;

  NET_RESTORE_TPL(OldTpl);

  return Status;
}

EFI_STATUS
EftpReadFile (
  IN EFI_EFTP_PROTOCOL   *This,
  IN EFI_EFTP_TOKEN      *Token
  )
/*++

Routine Description:

  EftpReadFile interface routine

Arguments:

  This    - Eftp Protocol
  Token   - User-provided token to specify the session's action

Returns:

  EFI_INVALID_PARAMETER   - Invalid parameter provided.
  EFI_NOT_STARTED         - Session isn't configured
  EFI_ACCESS_DENIED       - Session is busy
  EFI_DEVICE_ERROR        - Failed to config the underlying network stack

--*/
{
  return EftpInitPrivate (This, Token, EFI_EFTP_OPCODE_RRQ);
}

EFI_STATUS
EFIAPI
EftpWriteFile (
  IN EFI_EFTP_PROTOCOL       *This,
  IN EFI_EFTP_TOKEN          *Token
  )
/*++

Routine Description:

  EftpWriteFile interface routine

Arguments:

  This    - Eftp Protocol
  Token   - User-provided token to specify the session's action

Returns:

  EFI_INVALID_PARAMETER   - Invalid parameter provided.
  EFI_NOT_STARTED         - Session isn't configured
  EFI_ACCESS_DENIED       - Session is busy
  EFI_DEVICE_ERROR        - Failed to config the underlying network stack

--*/
{
  return EftpInitPrivate (This, Token, EFI_EFTP_OPCODE_WRQ);
}

EFI_STATUS
EFIAPI
EftpReadDirectory (
  IN EFI_EFTP_PROTOCOL   *This,
  IN EFI_EFTP_TOKEN      *Token
  )
/*++

Routine Description:

  EftpReadDirectory interface routine

Arguments:

  This    - Eftp Protocol
  Token   - User-provided token to specify the session's action

Returns:

  EFI_UNSUPPORTED  - This feature is not supported.

--*/
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
EftpGetInfo (
  IN EFI_EFTP_PROTOCOL         * This,
  IN EFI_EFTP_OVERRIDE_DATA    * OverrideData,
  IN CHAR8                     *Filename,
  IN CHAR8                     *ModeStr,
  IN UINT8                     OptionCount,
  IN EFI_EFTP_OPTION           * OptionList,
  OUT UINT32                   *PacketLength,
  OUT EFI_EFTP_PACKET          **Packet OPTIONAL
  )
/*++

Routine Description:

  EftpGetInfo interface routine. GetInfo is impletemented with EftpReadFile.
  It provides the ReadFile with a callback, that is EftpGetInfoCheckPacket, which will 
  terminate the EftpReadFile after enough information is retrieved.

Arguments:

  This           - Eftp protocol
  OverrideData   - Data used to override UDP configuration
  Filename       - File to retrieve
  ModeStr        - Operation mode
  OptionCount    - The number of options in the option list
  OptionList     - The name/value pairs of the options
  PacketLength   - The length of the packet buffer
  Packet         - Packet buffer to return information

Returns:

  EFI_UNSUPPORTED  - This feature is not supported.

--*/
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EftpConfigure (
  IN EFI_EFTP_PROTOCOL       *This,
  IN EFI_EFTP_CONFIG_DATA    *EftpConfigData  // OPTIONAL
  )
/*++

Routine Description:

  Config the eftp session. A eftp session must be configed first to read 
  or write file. If no EftpConfigData is provided, the session is cleaned up.

Arguments:

  This           - The Eftp protocol to configure
  EftpConfigData - Eftp configuration data

Returns:

  EFI_INVALID_PARAMETER  - Invalid parameters provided.
  EFI_ACCESS_DENIED      - The eftp session is busy.
  EFI_SUCCESS            - Successfully configured the session.

--*/
{
  EFTP_IO_PRIVATE *Private;
  EFI_TPL         OldTpl;

  if (This == NULL) {
    EFTP_DEBUG_ERROR ((L"EftpConfig: Called with invalid parameter.\n"));
    return EFI_INVALID_PARAMETER;
  }

  Private = EFTP_IO_PRIVATE_FROM_THIS (This);
  ASSERT (Private->Signature == EFTP_IO_PRIVATE_SIGNATURE);

  if (EftpConfigData == NULL) {
    //
    // ConfigureData is NULL means that stopping any pending operation
    //
    OldTpl              = gBS->RaiseTPL (NET_TPL_EVENT + 1);
    Private->Configured = FALSE;

    if (Private->State != EFTP_INIT) {

      Private->Result = EFI_ABORTED;

      if (Private->RrqState != NULL) {
        EftpRrqCleanUp (Private);
      } else if (Private->WrqState != NULL) {
        EftpWrqCleanUp (Private);
      }
    }

    gBS->RestoreTPL (OldTpl);
  } else {
    if (Private->State != EFTP_INIT) {
      return EFI_ACCESS_DENIED;
    }
    //
    // Save the config data in the privated structure of eftp instance
    //
    NetCopyMem (
      (VOID *) &Private->ConfigData,
      (VOID *) EftpConfigData,
      sizeof (EFI_EFTP_CONFIG_DATA)
      );

    Private->Configured = TRUE;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
EftpGetModeData (
  IN EFI_EFTP_PROTOCOL      *This,
  OUT EFI_EFTP_MODE_DATA    *ModeData
  )
/*++

Routine Description:

  Get the session's configuration data.

Arguments:

  This       - The eftp protocol
  ModeData   - Data structure to save configuration information

Returns:

  EFI_INVALID_PARAMETER   - Invalid parameters provided.
  EFI_NO_MAPPING          - Session not configured
  EFI_SUCCESS             - Information retrieved

--*/
{
  EFTP_IO_PRIVATE *Private;

  if (This == NULL || ModeData == NULL) {
    EFTP_DEBUG_ERROR ((L"EftpGetModeData: Called with invalid parameter.\n"));
    return EFI_INVALID_PARAMETER;
  }

  Private = EFTP_IO_PRIVATE_FROM_THIS (This);
  ASSERT (Private->Signature == EFTP_IO_PRIVATE_SIGNATURE);

  if (Private->Configured == FALSE) {
    return EFI_NO_MAPPING;
  }

  ModeData->SupportedOptionCount    = EFTP_SUPPORTED_OPTIONS_COUNT;
  ModeData->UnsupportedOptionCount  = EFTP_UNSUPPORTED_OPTIONS_COUNT;
  ModeData->SupportedOptoins        = mSupportedOptions;
  ModeData->UnsupportedOptoins      = mUnsupportedOptions;

  NetCopyMem ((VOID *) &ModeData->ConfigData, (VOID *) &Private->ConfigData, sizeof (EFI_EFTP_CONFIG_DATA));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
EftpParseOptions (
  IN EFI_EFTP_PROTOCOL     * This,
  IN UINT32                PacketLen,
  IN EFI_EFTP_PACKET       * Packet,
  IN OUT UINT32            *OptionCount,
  OUT EFI_EFTP_OPTION      **OptionList OPTIONAL
  )
/*++

Routine Description:

  Interface function to parse a packet for options.

Arguments:

  This          - The eftp protocol
  PacketLen     - The length of the packet
  Packet        - The packet
  OptionCount   - Point to variable to save option count
  OptionList    - Variable to store the allocated option list

Returns:

  EFI_INVALID_PARAMETER - Invalid parameters provided.
  EFI_PROTOCOL_ERROR    - Mal-form packet provided
  EFI_SUCCESS           - Options are successfully parsed
  Others                - Failed to allocate memory for the buffer.

--*/
{
  //
  // Check for invalid parameters.
  //
  if (PacketLen <= EFTP_HEADER_LEN || Packet == NULL || OptionCount == NULL || OptionList == NULL) {
    EFTP_DEBUG_WARN ((L"EftpParseOptions: Parameter invalid.\n"));
    EFTP_DEBUG_WARN (
      (L"EftpParseOptions: PacketLen %d, Packet %x,OptionCount %x, OptionList %x\n",
      PacketLen,
      Packet,
      OptionCount,
      OptionList)
      );
    return EFI_INVALID_PARAMETER;
  }

  return EftpGetOptions (This, PacketLen, Packet, OptionCount, OptionList);

}

EFI_STATUS
EFIAPI
EftpPoll (
  IN EFI_EFTP_PROTOCOL *This
  )
/*++

Routine Description:

  Interface function to poll the network stack

Arguments:

  This   - The eftp protocol

Returns:

  EFI_INVALID_PARAMETER   - Invalid parameters provided. 
  EFI_NOT_STARTED         - The session isn't configured

--*/
{
  EFTP_IO_PRIVATE *Private;

  if (This == NULL) {
    EFTP_DEBUG_ERROR ((L"Eftp.EftpPoll function called with invalid paramter.\n"));
    return EFI_INVALID_PARAMETER;
  }

  Private = EFTP_IO_PRIVATE_FROM_THIS (This);
  if (Private->Signature != EFTP_IO_PRIVATE_SIGNATURE) {
    EFTP_DEBUG_ERROR ((L"Eftp.ReadFile called with invalid protocol pointer.\n"));
    return EFI_INVALID_PARAMETER;
  }
  //
  // To allow user to actively poll the network stack, call Udp4Io->Poll() even when eftp instance is not started.
  //
  if (Private->ManagedNetwork == NULL) {
    return EFI_NOT_STARTED;
  }

  ASSERT (Private->ManagedNetwork);
  return Private->ManagedNetwork->Poll (Private->ManagedNetwork);
}

EFI_STATUS
EftpSetTimer (
  IN EFTP_IO_PRIVATE   *Private,
  UINTN                Sec,
  UINTN                MSec
  )
/*++

Routine Description:

  Set the session's timer. It first cancels the old setting, then set it again with 
  new timeout value.

Arguments:

  Private  - The eftp session's private data
  Sec      - The "second" of timeout
  MSec     - The milliseconf of timeout

Returns:

  EFI_SUCCESS - Timer is successfully set

--*/
{
  gBS->SetTimer (Private->TimeoutEvent, TimerCancel, 0);

  gBS->SetTimer (
        Private->TimeoutEvent,
        TimerRelative,
        Sec * TICKS_PER_SECOND + MSec * TICKS_PER_MILLISECOND
        );
  return EFI_SUCCESS;
}
