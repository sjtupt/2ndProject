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
        EftpOption.c
 
Abstract:
	Eftp Option Process routine
--*/
#include "Efi.h"
#include "EftpMain.h"

STATIC
EFI_STATUS
ExtractTsize (
  OUT EFTP_OPTION  *Opt,
  IN CHAR8         *Str
  );

STATIC
EFI_STATUS
ExtractTimeout (
  OUT EFTP_OPTION  *Opt,
  IN CHAR8         *Str
  );

STATIC
EFI_STATUS
ExtractBlksize (
  IN EFTP_OPTION  *Opt,
  CHAR8           *Str
  );

STATIC EFTP_OPTIONOBJ mOptionObjs[] = {
  {
    EFTP_OPTION_TSIZE,
    "tsize",
    ExtractTsize
  },
  {
    EFTP_OPTION_TIMEOUT,
    "timeout",
    ExtractTimeout
  },
  {
    EFTP_OPTION_BLKSIZE,
    "blksize",
    ExtractBlksize
  },
  {
    0,
    NULL,
    NULL
  }
};

VOID
EftpInitOption (
  IN EFTP_OPTION*Opt
  )
/*++

Routine Description:

  Initalize the EFTP_OPTION structure

Arguments:

  Opt - Option structure to initialize

Returns:

  NONE

--*/
{
  ASSERT ((Opt != NULL));
  Opt->Timeout      = EFTP_DEFAULT_TIMEOUT;
  Opt->BlkSize      = EFTP_DEFAULT_BLKSIZE;
  Opt->BlknoLen     = 2;
  Opt->TransferType = EFTP_UNICAST;
  Opt->Tsize        = 0;
  Opt->Pktdelay     = 0;
  Opt->NBlkInStream = 1;
  Opt->Exist        = 0;
}

EFI_STATUS
EftpGetOptions (
  IN EFI_EFTP_PROTOCOL     *This,
  IN UINT32                PacketLen,
  IN EFI_EFTP_PACKET       *Packet,
  IN OUT UINT32            *OptionCount,
  OUT EFI_EFTP_OPTION      **OptionList
  )
/*++

Routine Description:

  Parse the options in the packet(OACK/RRQ/WRQ) to name/value pairs.

Arguments:

  This         - The Eftp protocol
  PacketLen    - The length of the packet
  Packet       - The packet
  OptionCount  - The interger variable to store number of options.
  OptionList   - The memory buffer to store the name/value pairs. The caller should release it.

Returns:

  EFI_INVALID_PARAMETER  - Not correct parameters or packet head
  EFI_PROTOCOL_ERROR     - Mal-form packet provided
  EFI_SUCCESS            - Options are successfully parsed
  Others                 - Failed to allocate memory for the buffer.
  
--*/
{
  CHAR8       *P;
  CHAR8       *Start;
  UINT8       NOption;
  CHAR8       *Last;
  CHAR8       *Name;
  CHAR8       *Value;
  UINT16      OpCode;
  EFI_STATUS  Status;

  ASSERT (This && Packet && OptionCount && (PacketLen >= EFTP_HEADER_LEN));

  if (OptionList) {
    *OptionList = NULL;
  }

  P       = (CHAR8 *) Packet + EFTP_HEADER_LEN;
  Last    = (CHAR8 *) Packet + PacketLen;

  OpCode  = NTOHS (Packet->Hdr.OpCode);
  switch (OpCode) {
  case EFI_EFTP_OPCODE_RRQ:
  case EFI_EFTP_OPCODE_WRQ:
    //
    // Skip the filename, and mode string
    //
    Name  = "filename";
    Value = P;

    while ((P < Last) && *P) {
      P++;
    }

    if (P == Last) {
      EFTP_DEBUG_VERBOSE ((L"EftpGetOptions: invalid file name\n"));
      return EFI_INVALID_PARAMETER;
    }
    //
    // Skip the trailing '\0'
    //
    P++;

    Name  = "mode";
    Value = P;

    while ((P < Last) && *P) {
      P++;
    }

    if (P == Last) {
      EFTP_DEBUG_VERBOSE ((L"EftpGetOptions: invalid mode string"));
      return EFI_INVALID_PARAMETER;
    }

    P++;

    break;

  case EFI_EFTP_OPCODE_OACK:
    break;

  default:
    return EFI_INVALID_PARAMETER;
    break;

  }

  Start = P;

  //
  // process option name and value pairs
  //
  while (1) {
    P       = Start;
    NOption = 0;
    Status  = EFI_NOT_FOUND;

    while (P <= Last) {
      Name = P;

      //
      // OK, this is the last option/value pair
      //
      if (P == Last) {
        Status = EFI_SUCCESS;
        break;
      }

      while ((P < Last) && *P) {
        P++;

      }

      if (P == Last) {
        EFTP_DEBUG_ERROR ((L"EftpGetOptions: parse option name error\n"));
        Status = EFI_PROTOCOL_ERROR;
        break;
      }

      Value = ++P;

      while ((P < Last) && *P) {
        P++;

      }

      if (P == Last) {
        EFTP_DEBUG_ERROR ((L"EftpGetOptions: parse option value error\n"));

        Status = EFI_PROTOCOL_ERROR;
        break;
      }

      P++;

      //
      // OK, get a valid name/value pair
      //
      NOption++;

      if (OptionList && *OptionList) {
        (*OptionList)[NOption - 1].OptionStr  = Name;
        (*OptionList)[NOption - 1].ValueStr   = Value;
      }
    }

    *OptionCount = NOption;

    //
    // If there are valid options, a buffer is located to save the points to the options
    // and traverse the packet again. Otherwise return the status.
    //
    if (NOption && OptionList && (*OptionList == NULL)) {
      Status = gBS->AllocatePool (
                      EfiBootServicesData,
                      NOption * sizeof (EFI_EFTP_OPTION),
                      (VOID **) OptionList
                      );

      if (EFI_ERROR (Status)) {
        *OptionList = NULL;
        return Status;
      }

      continue;
    }

    return Status;
  }

  return EFI_SUCCESS;

}

STATIC
EFI_STATUS
StrToUint64 (
  IN CHAR8  *Str,
  OUT UINT64*Value
  )
/*++

Routine Description:

  Convert a string to UINT64. It checkes for various errors.

Arguments:

  Str     - String to convert
  Value   - Point to integer varable to stor the value

Returns:

  EFI_INVALID_PARAMETER  - The string isn't well-formed.
  EFI_SUCCESS            - Successfully converted.

--*/
{
  CHAR8 *P;

  ASSERT (Str && Value);

  if (*Str == '\0') {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Make sure that all the characters in the string are digitals
  //
  for (P = Str; *P; P++) {
    if (!IS_DIGIT (*P)) {
      return EFI_INVALID_PARAMETER;
    }
  }

  *Value = NetAsciiStrToU64 (Str);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
ExtractTsize (
  OUT EFTP_OPTION  *Opt,
  IN CHAR8         *Str
  )
/*++

Routine Description:

  Extract the Tsize option.

Arguments:

  Opt  - Eftp option structure.
  Str  - The tsize's option value

Returns:

  EFI_INVALID_PARAMETER  - The string isn't well-formed.
  EFI_SUCCESS            - Successfully extracted.

--*/
{
  ASSERT (Opt && Str);

  return StrToUint64 (Str, &(Opt->Tsize));
}

STATIC
EFI_STATUS
ExtractTimeout (
  OUT EFTP_OPTION  *Opt,
  IN CHAR8         *Str
  )
/*++

Routine Description:

  Extract the timeout option.

Arguments:

  Opt  - Eftp option structure.
  Str  - The timeout's option value

Returns:

  EFI_INVALID_PARAMETER  - The string isn't well-formed.
  EFI_SUCCESS            - Successfully extracted.

--*/
{
  EFI_STATUS  Status;
  UINT64      I;

  ASSERT (Opt && Str);

  Status = StrToUint64 (Str, &I);

  if (EFI_ERROR (Status) || (I < 1) || (I > 255)) {
    return EFI_INVALID_PARAMETER;

  }

  Opt->Timeout = (UINT16) I;
  return EFI_SUCCESS;

}

STATIC
EFI_STATUS
ExtractBlksize (
  IN EFTP_OPTION  *Opt,
  CHAR8           *Str
  )
/*++

Routine Description:

  Extract the blksize option.

Arguments:

  Opt  - Eftp option structure.
  Str  - The blksize's option value

Returns:

  EFI_INVALID_PARAMETER  - The string isn't well-formed.
  EFI_SUCCESS            - Successfully extracted.

--*/
{
  EFI_STATUS  Status;
  UINT64      I;

  ASSERT (Opt && Str);

  Status = StrToUint64 (Str, &I);

  if (EFI_ERROR (Status) || (I > 65464) || (I < 8)) {
    return EFI_INVALID_PARAMETER;

  }

  Opt->BlkSize = (UINT16) I;
  return EFI_SUCCESS;

}

EFI_STATUS
EftpCheckOption (
  IN EFI_EFTP_OPTION             *OptionList,
  IN UINT32                      OptionCount,
  IN UINT16                      OpCode
  )
/*++

Routine Description:

  Check whether user provided options are well-formed

Arguments:

  OptionList   - The list of option to check
  OptionCount  - The number of options in the option list
  OpCode       - OpCode the options are to be used with

Returns:

  EFI_INVALID_PARAMETER - Invalid paramter, for example, null option point, 
                          multicast option in WRQ.
  EFI_UNSUPPORTED       - Unsupported options in the option list.
  EFI_SUCCESS           - Options are OK.

--*/
{
  UINT32      Index1;
  UINT32      Index2;
  EFTP_OPTION Option;
  EFI_STATUS  Status;

  for (Index1 = 0; Index1 < OptionCount; Index1++) {
    if ((OptionList[Index1].OptionStr == NULL) || (OptionList[Index1].ValueStr == NULL)) {
      return EFI_INVALID_PARAMETER;
    }

    for (Index2 = 0; Index2 < EFTP_UNSUPPORTED_OPTIONS_COUNT; Index2++) {
      if (0 == NetAsciiStrCaseCmp (OptionList[Index1].OptionStr, mUnsupportedOptions[Index2])) {
        return EFI_UNSUPPORTED;
      }

    }

    for (Index2 = 0; Index2 < EFTP_OPTION_MAX; Index2++) {
      if (NetAsciiStrCaseCmp (mOptionObjs[Index2].Name, OptionList[Index1].OptionStr) != 0) {
        continue;
      }
      //
      // in RRQ multicast option is zero, so it is invalid. Skip this extract
      //
      if (Index2 == EFTP_OPTION_MCAST) {
        continue;
      }

      Status = mOptionObjs[Index2].Extract (&Option, OptionList[Index1].ValueStr);

      if (EFI_ERROR (Status)) {
        return EFI_INVALID_PARAMETER;
      }
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EftpExtractOptions (
  IN EFI_EFTP_PROTOCOL     *This,
  IN EFTP_OPTION           *Option,
  IN EFI_EFTP_PACKET       *Packet,
  IN UINT32                PacketLen
  )
/*++

Routine Description:

  Extract the options from the packet, and check whether they are OK

Arguments:

  This            - The eftp protocol
  Option          - Option structure to fill extracted options.
  Packet          - Packet to extract options from
  PacketLen       - Packet length

Returns:

  EFI_INVALID_PARAMETER - Packet is mal-formed
  EFI_SUCCESS           - Successfully extracted the option from packet

--*/
{
  UINT32          NOption;
  EFI_EFTP_OPTION *OptionList;
  EFI_STATUS      Status;
  UINTN           Index;
  UINTN           Loop;

  ASSERT (Option && Packet);

  OptionList  = NULL;

  Status      = EftpGetOptions (This, PacketLen, Packet, &NOption, &OptionList);

  if (EFI_ERROR (Status)) {
    //
    // When EftpGetOptions returns EFI_PROTOCOL_ERROR, there maybe some
    // memory allocated to accomoderate partial options
    //
    if (OptionList) {
      gBS->FreePool (OptionList);

    }

    return EFI_INVALID_PARAMETER;

  }

  EftpInitOption (Option);

  if (NOption == 0) {
    return EFI_SUCCESS;
  }

  Status = EFI_SUCCESS;

  for (Loop = 0; Loop < NOption; Loop++) {
    for (Index = 0; Index < EFTP_OPTION_MAX; Index++) {

      if (NetAsciiStrCaseCmp (mOptionObjs[Index].Name, OptionList[Loop].OptionStr) != 0) {
        continue;

      }

      Status = mOptionObjs[Index].Extract (Option, OptionList[Loop].ValueStr);

      if (EFI_ERROR (Status)) {
        goto OnExit;
      }

      Option->Exist |= (UINT64) (1 << Index);
      break;
    }
  }

  //
  // Verify that all the options coexist happy
  //
  for (Index = 0; Index < EFTP_OPTION_MAX; Index++) {
    //
    // Only verify the options that DOES exists in the packet
    //
    if (!(Option->Exist & (UINT64) (1 << Index)) || (mOptionObjs[Index].Verify == NULL)) {
      continue;
    }

    Status = mOptionObjs[Index].Verify (Option);

    if (EFI_ERROR (Status)) {
      goto OnExit;
    }

  }

OnExit:
  gBS->FreePool (OptionList);
  return Status;
}
