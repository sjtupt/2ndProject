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
                                                                
  Copyright 2006, 2007, 2008, 2009, 2010, 2011 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:

  Mtftp6ENTSTestCase.c

Abstract:

--*/

#include "Mtftp6ENTSTestCase.h"

//
// Mtftp6.GetModeData
//
static EFI_MTFTP6_MODE_DATA *GetModeData_ModeData;
static EFI_STATUS           *GetModeData_Status;

ENTS_ARG_FIELD  gMtftp6GetModeDataArgField[] = {
  {
    POINTER,
    &GetModeData_ModeData
  },
  {
    POINTER,
    &GetModeData_Status
  },
  0
};

EFI_STATUS
Mtftp6GetModeData_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Mtftp6 GetModeData Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_MTFTP6_PROTOCOL *Mtftp6;

  Mtftp6 = (EFI_MTFTP6_PROTOCOL *) ClientInterface;

  *GetModeData_Status = Mtftp6->GetModeData (
                                  Mtftp6,
                                  GetModeData_ModeData
                                  );
  RecordMessage (
    &gMtftp6EntsRuntimeInfo,
    &gMtftp6EntsRuntimeInfoSize,
    L"Status - %r",
    *GetModeData_Status
    );
  return EFI_SUCCESS;
}

//
// Mtftp6.Configure
//
static EFI_MTFTP6_CONFIG_DATA *Configure_Mtftp6ConfigData;
static EFI_STATUS             *Configure_Status;

ENTS_ARG_FIELD                gMtftp6ConfigureArgField[] = {
  {
    POINTER,
    &Configure_Mtftp6ConfigData
  },
  {
    POINTER,
    &Configure_Status
  },
  0
};

EFI_STATUS
Mtftp6Configure_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Mtftp6 Configure Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_MTFTP6_PROTOCOL *Mtftp6;

  Mtftp6 = (EFI_MTFTP6_PROTOCOL *) ClientInterface;

  *Configure_Status = Mtftp6->Configure (
                                Mtftp6,
                                Configure_Mtftp6ConfigData
                                );
  RecordMessage (
    &gMtftp6EntsRuntimeInfo,
    &gMtftp6EntsRuntimeInfoSize,
    L"Status - %r",
    *Configure_Status
    );
  return EFI_SUCCESS;
}

//
// Mtftp6.GetInfo
//
static EFI_MTFTP6_OVERRIDE_DATA *GetInfo_OverrideData;
static UINT8                    *GetInfo_Filename;
static UINT8                    *GetInfoModeStr;
static UINT8                    GetInfo_OptionCount;
static EFI_MTFTP6_OPTION        *GetInfo_OptionList;
static UINT32                   *GetInfo_PacketLength;
static EFI_MTFTP6_PACKET        **GetInfo_Packet;
static EFI_STATUS               *GetInfo_Status;

ENTS_ARG_FIELD                  gMtftp6GetInfoArgField[] = {
  {
    POINTER,
    &GetInfo_OverrideData
  },
  {
    POINTER,
    &GetInfo_Filename
  },
  {
    POINTER,
    &GetInfoModeStr
  },
  {
    OCTET8,
    &GetInfo_OptionCount
  },
  {
    POINTER,
    &GetInfo_OptionList
  },
  {
    POINTER,
    &GetInfo_PacketLength
  },
  {
    POINTER,
    &GetInfo_Packet
  },
  {
    POINTER,
    &GetInfo_Status
  },
  0
};

EFI_STATUS
Mtftp6GetInfo_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Mtftp6 GetInfo Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_MTFTP6_PROTOCOL *Mtftp6;

  Mtftp6 = (EFI_MTFTP6_PROTOCOL *) ClientInterface;

  *GetInfo_Status = Mtftp6->GetInfo (
                              Mtftp6,
                              GetInfo_OverrideData,
                              GetInfo_Filename,
                              GetInfoModeStr,
                              GetInfo_OptionCount,
                              GetInfo_OptionList,
                              GetInfo_PacketLength,
                              GetInfo_Packet
                              );
  RecordMessage (
    &gMtftp6EntsRuntimeInfo,
    &gMtftp6EntsRuntimeInfoSize,
    L"Status - %r",
    *GetInfo_Status
    );
  return EFI_SUCCESS;
}

//
// Mtftp6.ParseOptions
//
static UINT32             ParseOptions_PacketLen;
static EFI_MTFTP6_PACKET  *ParseOptions_Packet;
static UINT32             *ParseOptions_OptionCount;
static EFI_MTFTP6_OPTION  **ParseOptions_OptionList;
static EFI_STATUS         *ParseOptions_Status;

ENTS_ARG_FIELD            gMtftp6ParseOptionsArgField[] = {
  {
    OCTETN,
    &ParseOptions_PacketLen
  },
  {
    POINTER,
    &ParseOptions_Packet
  },
  {
    POINTER,
    &ParseOptions_OptionCount
  },
  {
    POINTER,
    &ParseOptions_OptionList
  },
  {
    POINTER,
    &ParseOptions_Status
  },
  0
};

EFI_STATUS
Mtftp6ParseOptions_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Mtftp6 ParseOptions Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_MTFTP6_PROTOCOL *Mtftp6;

  Mtftp6 = (EFI_MTFTP6_PROTOCOL *) ClientInterface;

  *ParseOptions_Status = Mtftp6->ParseOptions (
                                  Mtftp6,
                                  ParseOptions_PacketLen,
                                  ParseOptions_Packet,
                                  ParseOptions_OptionCount,
                                  ParseOptions_OptionList
                                  );
  RecordMessage (
    &gMtftp6EntsRuntimeInfo,
    &gMtftp6EntsRuntimeInfoSize,
    L"Status - %r",
    *ParseOptions_Status
    );
  return EFI_SUCCESS;
}

//
// Mtftp6.ReadFile
//
EFI_STATUS
EFIAPI
ReadFileCheckPacket0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  );

EFI_STATUS
EFIAPI
ReadFileCheckPacket1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  );

EFI_STATUS
EFIAPI
ReadFileCheckPacket2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  );

EFI_STATUS
EFIAPI
ReadFileCheckPacket3 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  );

static EFI_MTFTP6_CHECK_PACKET      ReadFileCheckPacketList[] = {
  ReadFileCheckPacket0,
  ReadFileCheckPacket1,
  ReadFileCheckPacket2,
  ReadFileCheckPacket3
};

EFI_STATUS
EFIAPI
ReadFileTimeoutCallback0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  );

EFI_STATUS
EFIAPI
ReadFileTimeoutCallback1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  );

EFI_STATUS
EFIAPI
ReadFileTimeoutCallback2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  );

static EFI_MTFTP6_TIMEOUT_CALLBACK  ReadFileTimeoutCallbackList[] = {
  ReadFileTimeoutCallback0,
  ReadFileTimeoutCallback1,
  ReadFileTimeoutCallback2
};

EFI_STATUS
EFIAPI
ReadFilePacketNeeded0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  );

EFI_STATUS
EFIAPI
ReadFilePacketNeeded1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  );

EFI_STATUS
EFIAPI
ReadFilePacketNeeded2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  );

static EFI_MTFTP6_PACKET_NEEDED     ReadFilePacketNeededList[] = {
  ReadFilePacketNeeded0,
  ReadFilePacketNeeded1,
  ReadFilePacketNeeded2
};

static EFI_MTFTP6_TOKEN             *ReadFile_Token;
static UINTN                        ReadFileCheckPacket;
static UINTN                        ReadFileTimeoutCallback;
static UINTN                        ReadFilePacketNeeded;
static EFI_STATUS                   *ReadFile_Status;

ENTS_ARG_FIELD                      gMtftp6ReadFileArgField[] = {
  {
    POINTER,
    &ReadFile_Token
  },
  {
    OCTETN,
    &ReadFileCheckPacket
  },
  {
    OCTETN,
    &ReadFileTimeoutCallback
  },
  {
    OCTETN,
    &ReadFilePacketNeeded
  },
  {
    POINTER,
    &ReadFile_Status
  },
  0
};

EFI_STATUS
Mtftp6ReadFile_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Mtftp6 ReadFile Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_MTFTP6_PROTOCOL *Mtftp6;

  Mtftp6 = (EFI_MTFTP6_PROTOCOL *) ClientInterface;

  if (ReadFile_Token != NULL) {
    if (ReadFileCheckPacket < 4) {
      ReadFile_Token->CheckPacket = ReadFileCheckPacketList[ReadFileCheckPacket];
    } else {
      EntsPrint (L"ReadFile_Token->CheckPacket     = NULL\n");
      ReadFile_Token->CheckPacket = NULL;
    }

    if (ReadFileTimeoutCallback < 3) {
      ReadFile_Token->TimeoutCallback = ReadFileTimeoutCallbackList[ReadFileTimeoutCallback];
    } else {
      ReadFile_Token->TimeoutCallback = NULL;
    }

    if (ReadFilePacketNeeded < 3) {
      ReadFile_Token->PacketNeeded = ReadFilePacketNeededList[ReadFilePacketNeeded];
    } else {
      ReadFile_Token->PacketNeeded = NULL;
    }
  }

  *ReadFile_Status = Mtftp6->ReadFile (
                              Mtftp6,
                              ReadFile_Token
                              );
  RecordMessage (
    &gMtftp6EntsRuntimeInfo,
    &gMtftp6EntsRuntimeInfoSize,
    L"Status - %r",
    *ReadFile_Status
    );
  return EFI_SUCCESS;
}

//
// Mtftp6.WriteFile
//
EFI_STATUS
EFIAPI
WriteFileCheckPacket0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  );

EFI_STATUS
EFIAPI
WriteFileCheckPacket1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  );

EFI_STATUS
EFIAPI
WriteFileCheckPacket2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  );

static EFI_MTFTP6_CHECK_PACKET      WriteFileCheckPacketList[] = {
  WriteFileCheckPacket0,
  WriteFileCheckPacket1,
  WriteFileCheckPacket2
};

EFI_STATUS
EFIAPI
WriteFileTimeoutCallback0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  );

EFI_STATUS
EFIAPI
WriteFileTimeoutCallback1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  );

EFI_STATUS
EFIAPI
WriteFileTimeoutCallback2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  );

static EFI_MTFTP6_TIMEOUT_CALLBACK  WriteFileTimeoutCallbackList[] = {
  WriteFileTimeoutCallback0,
  WriteFileTimeoutCallback1,
  WriteFileTimeoutCallback2
};

EFI_STATUS
EFIAPI
WriteFilePacketNeeded0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  );

EFI_STATUS
EFIAPI
WriteFilePacketNeeded1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  );

EFI_STATUS
EFIAPI
WriteFilePacketNeeded2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  );

static EFI_MTFTP6_PACKET_NEEDED     WriteFilePacketNeededList[] = {
  WriteFilePacketNeeded0,
  WriteFilePacketNeeded1,
  WriteFilePacketNeeded2
};

static EFI_MTFTP6_TOKEN             *WriteFile_Token;
static UINTN                        WriteFileCheckPacket;
static UINTN                        WriteFileTimeoutCallback;
static UINTN                        WriteFilePacketNeeded;
static EFI_STATUS                   *WriteFile_Status;

ENTS_ARG_FIELD                      gMtftp6WriteFileArgField[] = {
  {
    POINTER,
    &WriteFile_Token
  },
  {
    OCTETN,
    &WriteFileCheckPacket
  },
  {
    OCTETN,
    &WriteFileTimeoutCallback
  },
  {
    OCTETN,
    &WriteFilePacketNeeded
  },
  {
    POINTER,
    &WriteFile_Status
  },
  0
};

EFI_STATUS
Mtftp6WriteFile_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Mtftp6 WriteFile Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_MTFTP6_PROTOCOL *Mtftp6;

  Mtftp6 = (EFI_MTFTP6_PROTOCOL *) ClientInterface;

  if (WriteFile_Token != NULL) {
    if (WriteFileCheckPacket < 3) {
      WriteFile_Token->CheckPacket = WriteFileCheckPacketList[WriteFileCheckPacket];
    } else {
      WriteFile_Token->CheckPacket = NULL;
    }

    if (WriteFileTimeoutCallback < 3) {
      WriteFile_Token->TimeoutCallback = WriteFileTimeoutCallbackList[WriteFileTimeoutCallback];
    } else {
      WriteFile_Token->TimeoutCallback = NULL;
    }

    if (WriteFilePacketNeeded < 3) {
      WriteFile_Token->PacketNeeded = WriteFilePacketNeededList[WriteFilePacketNeeded];
    } else {
      WriteFile_Token->PacketNeeded = NULL;
    }
  }

  *WriteFile_Status = Mtftp6->WriteFile (
                                Mtftp6,
                                WriteFile_Token
                                );
  RecordMessage (
    &gMtftp6EntsRuntimeInfo,
    &gMtftp6EntsRuntimeInfoSize,
    L"Status - %r",
    *WriteFile_Status
    );
  return EFI_SUCCESS;
}

//
// Mtftp6.ReadDirectory
//
EFI_STATUS
EFIAPI
ReadDirectoryCheckPacket0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  );

EFI_STATUS
EFIAPI
ReadDirectoryCheckPacket1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  );

EFI_STATUS
EFIAPI
ReadDirectoryCheckPacket2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  );

EFI_STATUS
EFIAPI
ReadDirectoryCheckPacket3 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  );

static EFI_MTFTP6_CHECK_PACKET      ReadDirectoryCheckPacketList[] = {
  ReadDirectoryCheckPacket0,
  ReadDirectoryCheckPacket1,
  ReadDirectoryCheckPacket2,
  ReadDirectoryCheckPacket3
};

EFI_STATUS
EFIAPI
ReadDirectoryTimeoutCallback0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  );

EFI_STATUS
EFIAPI
ReadDirectoryTimeoutCallback1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  );

EFI_STATUS
EFIAPI
ReadDirectoryTimeoutCallback2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  );

static EFI_MTFTP6_TIMEOUT_CALLBACK  ReadDirectoryTimeoutCallbackList[] = {
  ReadDirectoryTimeoutCallback0,
  ReadDirectoryTimeoutCallback1,
  ReadDirectoryTimeoutCallback2
};

EFI_STATUS
EFIAPI
ReadDirectoryPacketNeeded0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  );

EFI_STATUS
EFIAPI
ReadDirectoryPacketNeeded1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  );

EFI_STATUS
EFIAPI
ReadDirectoryPacketNeeded2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  );

static EFI_MTFTP6_PACKET_NEEDED     ReadDirectoryPacketNeededList[] = {
  ReadDirectoryPacketNeeded0,
  ReadDirectoryPacketNeeded1,
  ReadDirectoryPacketNeeded2
};

static EFI_MTFTP6_TOKEN             *ReadDirectory_Token;
static UINTN                        ReadDirectoryCheckPacket;
static UINTN                        ReadDirectoryTimeoutCallback;
static UINTN                        ReadDirectoryPacketNeeded;
static EFI_STATUS                   *ReadDirectory_Status;

ENTS_ARG_FIELD                      gMtftp6ReadDirectoryArgField[] = {
  {
    POINTER,
    &ReadDirectory_Token
  },
  {
    OCTETN,
    &ReadDirectoryCheckPacket
  },
  {
    OCTETN,
    &ReadDirectoryTimeoutCallback
  },
  {
    OCTETN,
    &ReadDirectoryPacketNeeded
  },
  {
    POINTER,
    &ReadDirectory_Status
  },
  0
};

EFI_STATUS
Mtftp6ReadDirectory_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Mtftp6 ReadDirectory Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_MTFTP6_PROTOCOL *Mtftp6;

  Mtftp6 = (EFI_MTFTP6_PROTOCOL *) ClientInterface;

  if (ReadDirectory_Token != NULL) {
    if (ReadDirectoryCheckPacket < 4) {
      ReadDirectory_Token->CheckPacket = ReadDirectoryCheckPacketList[ReadDirectoryCheckPacket];
    } else {
      ReadDirectory_Token->CheckPacket = NULL;
    }

    if (ReadDirectoryTimeoutCallback < 3) {
      ReadDirectory_Token->TimeoutCallback = ReadDirectoryTimeoutCallbackList[ReadDirectoryTimeoutCallback];
    } else {
      ReadDirectory_Token->TimeoutCallback = NULL;
    }

    if (ReadDirectoryPacketNeeded < 3) {
      ReadDirectory_Token->PacketNeeded = ReadDirectoryPacketNeededList[ReadDirectoryPacketNeeded];
    } else {
      ReadDirectory_Token->PacketNeeded = NULL;
    }
  }

  *ReadDirectory_Status = Mtftp6->ReadDirectory (
                                    Mtftp6,
                                    ReadDirectory_Token
                                    );
  RecordMessage (
    &gMtftp6EntsRuntimeInfo,
    &gMtftp6EntsRuntimeInfoSize,
    L"Status - %r",
    *ReadDirectory_Status
    );
  return EFI_SUCCESS;
}

//
// Mtftp6.Poll
//
static EFI_STATUS *Poll_Status;

ENTS_ARG_FIELD    gMtftp6PollArgField[] = {
  {
    POINTER,
    &Poll_Status
  },
  0
};

EFI_STATUS
Mtftp6Poll_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Mtftp6 Poll Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EFI_MTFTP6_PROTOCOL *Mtftp6;

  Mtftp6        = (EFI_MTFTP6_PROTOCOL *) ClientInterface;

  *Poll_Status  = Mtftp6->Poll (Mtftp6);
  RecordMessage (
    &gMtftp6EntsRuntimeInfo,
    &gMtftp6EntsRuntimeInfoSize,
    L"Status - %r",
    *Poll_Status
    );
  return EFI_SUCCESS;
}

//
// ReadFile Callback Routines.
//
EFI_STATUS
EFIAPI
ReadFileCheckPacket0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  )
/*++

Routine Description:

  Mtftp6 readfile check packet callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  PacketLen - Packet length.
  Packet    - Pointer to a Mtftpv6 packet.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadFileCheckPacket1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  )
/*++

Routine Description:

  Mtftp6 readfile check packet callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  PacketLen - Packet length.
  Packet    - Pointer to a Mtftpv6 packet.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback ReadFileCheckPacket1() is called.\n");

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadFileCheckPacket2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  )
/*++

Routine Description:

  Mtftp6 readfile check packet callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  PacketLen - Packet length.
  Packet    - Pointer to a Mtftpv6 packet.

Returns:

  EFI_SUCCESS - Operation succeeded.
  Others      - Something failed.

--*/
{
  switch (NTOHS (Packet->OpCode)) {
  // Data8 field should be ignored and treated as reserved
  case EFI_MTFTP6_OPCODE_DATA8:    
    EntsPrint (L"Callback ReadFileCheckPacket2() Data8 field should be ignored.\n");
    break;
    
  case EFI_MTFTP6_OPCODE_DATA:
    EntsPrint (L"Callback ReadFileCheckPacket2() abort the process.\n");
    break;

  case EFI_MTFTP6_OPCODE_OACK:
  case EFI_MTFTP6_OPCODE_ERROR:
  default:
    return EFI_SUCCESS;
  }

  return EFI_ABORTED;
}

EFI_STATUS
EFIAPI
ReadFileCheckPacket3 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  )
/*++

Routine Description:

  Mtftp6 readfile check packet callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  PacketLen - Packet length.
  Packet    - Pointer to a Mtftpv6 packet.

Returns:

  EFI_SUCCESS - Operation succeeded.
  Others      - Something failed.

--*/
{
  switch (NTOHS (Packet->OpCode)) {
  case EFI_MTFTP6_OPCODE_DATA:
    return EFI_SUCCESS;
    
  // Data8 field should be ignored and treated as reserved
  case EFI_MTFTP6_OPCODE_DATA8:
    EntsPrint (L"Callback ReadFileCheckPacket3() Data8 field should be ignored.\n");
    break;
    
  case EFI_MTFTP6_OPCODE_OACK:
  case EFI_MTFTP6_OPCODE_ERROR:
  default:
    EntsPrint (L"Callback ReadFileCheckPacket3() abort the process.\n");
    break;
  }

  return EFI_ABORTED;
}

EFI_STATUS
EFIAPI
ReadFileTimeoutCallback0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  )
/*++

Routine Description:

  Mtftp6 readfile timeout callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadFileTimeoutCallback1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  )
/*++

Routine Description:

  Mtftp6 readfile timeout callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback ReadFileTimeoutCallback1() is called.\n");

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadFileTimeoutCallback2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  )
/*++

Routine Description:

  Mtftp6 readfile timeout callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback ReadFileTimeoutCallback2() abort the process.\n");

  return EFI_ABORTED;
}

EFI_STATUS
EFIAPI
ReadFilePacketNeeded0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  )
/*++

Routine Description:

  Mtftp6 readfile packet needed callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  Length    - Length of raw data wanted on input. 
  Buffer    - Pointer to buffer where the data is stored.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadFilePacketNeeded1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  )
/*++

Routine Description:

  Mtftp6 readfile packet needed callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  Length    - Length of raw data wanted on input. 
  Buffer    - Pointer to buffer where the data is stored.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback ReadFilePacketNeeded1() is called.\n");

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadFilePacketNeeded2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  )
/*++

Routine Description:

  Mtftp6 readfile packet needed callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  Length    - Length of raw data wanted on input. 
  Buffer    - Pointer to buffer where the data is stored.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback ReadFilePacketNeeded2() abort the process.\n");

  return EFI_ABORTED;
}

//
// WriteFile Callback Routines.
//
EFI_STATUS
EFIAPI
WriteFileCheckPacket0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  )
/*++

Routine Description:

  Mtftp6 writefile check packet callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  PacketLen - Packet length.
  Packet    - Pointer to a Mtftpv6 packet.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
WriteFileCheckPacket1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  )
/*++

Routine Description:

  Mtftp6 writefile check packet callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  PacketLen - Packet length.
  Packet    - Pointer to a Mtftpv6 packet.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback WriteFileCheckPacket1() is called.\n");

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
WriteFileCheckPacket2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  )
/*++

Routine Description:

  Mtftp6 writefile check packet callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  PacketLen - Packet length.
  Packet    - Pointer to a Mtftpv6 packet.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback WriteFileCheckPacket2() abort the process.\n");

  return EFI_ABORTED;
}

EFI_STATUS
EFIAPI
WriteFileTimeoutCallback0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  )
/*++

Routine Description:

  Mtftp6 writefile timeout callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
WriteFileTimeoutCallback1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  )
/*++

Routine Description:

  Mtftp6 writefile timeout callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback WriteFileTimeoutCallback1() is called.\n");

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
WriteFileTimeoutCallback2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  )
/*++

Routine Description:

  Mtftp6 writefile timeout callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback WriteFileTimeoutCallback2() abort the process.\n");

  return EFI_ABORTED;
}

EFI_STATUS
EFIAPI
WriteFilePacketNeeded0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  )
/*++

Routine Description:

  Mtftp6 writefile packet needed callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  Length    - Length of raw data wanted on input. 
  Buffer    - Pointer to buffer where the data is stored.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
WriteFilePacketNeeded1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  )
/*++

Routine Description:

  Mtftp6 writefile packet needed callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  Length    - Length of raw data wanted on input. 
  Buffer    - Pointer to buffer where the data is stored.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{ 
  char *S;
  *(Length) = 10;
  (* Buffer) = EfiLibAllocatePool( 10 );
  if ((* Buffer) == NULL)
	  return EFI_ABORTED;

  S = "needed1234";
  gBS->CopyMem ((* Buffer), (void *)S, 10);
  
  EntsPrint (L"Callback WriteFilePacketNeeded1() is called.\n");

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
WriteFilePacketNeeded2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  )
/*++

Routine Description:

  Mtftp6 writefile packet needed callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  Length    - Length of raw data wanted on input. 
  Buffer    - Pointer to buffer where the data is stored.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback WriteFilePacketNeeded2() abort the process.\n");

  return EFI_ABORTED;
}

//
// ReadDirectory Callback Routines.
//
EFI_STATUS
EFIAPI
ReadDirectoryCheckPacket0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  )
/*++

Routine Description:

  Mtftp6 readDirectory check packet callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  PacketLen - Packet length.
  Packet    - Pointer to a Mtftpv6 packet.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadDirectoryCheckPacket1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  )
/*++

Routine Description:

  Mtftp6 readDirectory check packet callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  PacketLen - Packet length.
  Packet    - Pointer to a Mtftpv6 packet.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback ReadDirectoryCheckPacket1() is called.\n");

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadDirectoryCheckPacket2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  )
/*++

Routine Description:

  Mtftp6 readDirectory check packet callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  PacketLen - Packet length.
  Packet    - Pointer to a Mtftpv6 packet.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  switch (NTOHS (Packet->OpCode)) {
  // Data8 field should be ignored and treated as reserved
  case EFI_MTFTP6_OPCODE_DATA8:
    EntsPrint (L"Callback ReadDirectoryCheckPacket2() Data8 field should be ignored.\n");
    break;

  case EFI_MTFTP6_OPCODE_DATA:
    EntsPrint (L"Callback ReadDirectoryCheckPacket2() abort the process.\n");
    break;

  case EFI_MTFTP6_OPCODE_OACK:
  case EFI_MTFTP6_OPCODE_ERROR:
  default:
    return EFI_SUCCESS;
  }

  return EFI_ABORTED;
}

EFI_STATUS
EFIAPI
ReadDirectoryCheckPacket3 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN UINT16              PacketLen,
  IN EFI_MTFTP6_PACKET   *Packet
  )
/*++

Routine Description:

  Mtftp6 readDirectory check packet callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  PacketLen - Packet length.
  Packet    - Pointer to a Mtftpv6 packet.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  switch (NTOHS (Packet->OpCode)) {
  case EFI_MTFTP6_OPCODE_DATA:
    return EFI_SUCCESS;
    
  // Data8 field should be ignored and treated as reserved
  case EFI_MTFTP6_OPCODE_DATA8:
    EntsPrint (L"Callback ReadDirectoryCheckPacket3() Data8 field should be ignored.\n");
    break;
    
  case EFI_MTFTP6_OPCODE_OACK:
  case EFI_MTFTP6_OPCODE_ERROR:
  default:
    EntsPrint (L"Callback ReadDirectoryCheckPacket3() abort the process.\n");
    break;
  }

  return EFI_ABORTED;
}

EFI_STATUS
EFIAPI
ReadDirectoryTimeoutCallback0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  )
/*++

Routine Description:

  Mtftp6 readDirectory timeout callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadDirectoryTimeoutCallback1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  )
/*++

Routine Description:

  Mtftp6 readDirectory timeout callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback ReadDirectoryTimeoutCallback1() is called.\n");

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadDirectoryTimeoutCallback2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token
  )
/*++

Routine Description:

  Mtftp6 readDirectory timeout callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback ReadDirectoryTimeoutCallback2() abort the process.\n");

  return EFI_ABORTED;
}

EFI_STATUS
EFIAPI
ReadDirectoryPacketNeeded0 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  )
/*++

Routine Description:

  Mtftp6 readDirectory packet needed callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  Length    - Length of raw data wanted on input. 
  Buffer    - Pointer to buffer where the data is stored.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadDirectoryPacketNeeded1 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  )
/*++

Routine Description:

  Mtftp6 readDirectory packet needed callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  Length    - Length of raw data wanted on input. 
  Buffer    - Pointer to buffer where the data is stored.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback ReadDirectoryPacketNeeded1() is called.\n");

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReadDirectoryPacketNeeded2 (
  IN EFI_MTFTP6_PROTOCOL *This,
  IN EFI_MTFTP6_TOKEN    *Token,
  IN OUT UINT16          *Length,
  OUT void               **Buffer
  )
/*++

Routine Description:

  Mtftp6 readDirectory packet needed callback routine.

Arguments:

  This      - Pointer to EFI_MTFTP6_PROTOCOL instance.
  Token     - Token that caller provided.
  Length    - Length of raw data wanted on input. 
  Buffer    - Pointer to buffer where the data is stored.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
{
  EntsPrint (L"Callback ReadDirectoryPacketNeeded2() abort the process.\n");

  return EFI_ABORTED;
}
