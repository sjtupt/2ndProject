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

  HIIConfigAccessBBTestMain.c

Abstract:

  Test Driver of HII Configuration Access Protocol

--*/

#include "HIIConfigAccessBBTestMain.h"

//
// Global variables
//
EFI_HANDLE mImageHandle;

EFI_BB_TEST_PROTOCOL *gBBTestProtocolInterface;

EFI_BB_TEST_PROTOCOL_FIELD gBBTestProtocolField = {
  EFI_HII_CONFIG_ACCESS_PROTOCOL_TEST_REVISION,
  EFI_HII_CONFIG_ACCESS_PROTOCOL_GUID,
  L"HII Configuration Access Protocol Test",
  L"UEFI HII Configuration Access  Protocol Black-Box Test"
};

EFI_GUID gSupportProtocolGuid[] = {
  EFI_STANDARD_TEST_LIBRARY_GUID,
  EFI_NULL_GUID
};

EFI_BB_TEST_ENTRY_FIELD gBBTestEntryField[] = {
  /*
  //Use for unit test only
  {
    { 0x8cdda816, 0x3f97, 0x4155, { 0xa6, 0x73, 0x86, 0x93, 0x79, 0x8f, 0xb0, 0x6d } },
	L"Unit Test Only",
	L"Verify the Assistant Function",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestUnitTest
  },
  */
  {
    EFI_HII_CONFIG_ACCESS_PROTOCOL_TEST_ENTRY_GUID0101,
    L"ExtractConfigFunction",
    L"Function auto test for HII Configuration Access Protocol ExtractConfig().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestExtractConfigFunctionTest
  },
  
  {
    EFI_HII_CONFIG_ACCESS_PROTOCOL_TEST_ENTRY_GUID0102,
    L"RouteConfigFunction",
    L"Function auto test for HII Configuration Access Protocol RouteConfig().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestRouteConfigFunctionTest
  },
  /*
  {
    EFI_HII_CONFIG_ACCESS_PROTOCOL_TEST_ENTRY_GUID0103,
    L"CallBackFunction",
    L"Function auto test for HII Configuration Access Protocol CallBack().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestCallBackFunctionTest
  },
  */
  {
    EFI_HII_CONFIG_ACCESS_PROTOCOL_TEST_ENTRY_GUID0201,
    L"ExtractConfigConformance",
    L"Conformance auto test for HII Configuration Access Protocol ExtractConfig().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestExtractConfigConformanceTest
  },
  {
    EFI_HII_CONFIG_ACCESS_PROTOCOL_TEST_ENTRY_GUID0202,
    L"RouteConfigConformance",
    L"Conformance auto test for HII Configuration Access Protocol RouteConfig().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestRouteConfigConformanceTest
  },
  /*
  {
    EFI_HII_CONFIG_ACCESS_PROTOCOL_TEST_ENTRY_GUID0203,
    L"CallBackConformance",
    L"Conformance auto test for HII Configuration Access Protocol CallBack().",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    BBTestCallBackConformanceTest
  },
  */
  0
};



EFI_DRIVER_ENTRY_POINT(InitializeHIIConfigAccessBBTest)

EFI_STATUS
InitializeHIIConfigAccessBBTest (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EfiInitializeTestLib (ImageHandle, SystemTable);

  //
  // initialize test utility lib
  //

  InitializeLib (ImageHandle, SystemTable);

  mImageHandle = ImageHandle;

  return EfiInitAndInstallBBTestInterface (
           &ImageHandle,
           &gBBTestProtocolField,
           gBBTestEntryField,
           UnloadHIIConfigAccessBBTest,
           &gBBTestProtocolInterface
           );
}


EFI_STATUS
UnloadHIIConfigAccessBBTest (
  IN EFI_HANDLE                   ImageHandle
  )
{
  return EfiUninstallAndFreeBBTestInterface (
           ImageHandle,
           gBBTestProtocolInterface
           );
}


// Assistance Function
UINTN
EfiDevicePathSize (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
/*++

Routine Description:

  Calculate the space size of a device path.

Arguments:

  DevicePath  - A specified device path

Returns:

  The size.

--*/
{
  EFI_DEVICE_PATH_PROTOCOL  *Start;

  if (DevicePath == NULL) {
    return 0;
  }

  //
  // Search for the end of the device path structure
  //
  Start = DevicePath;
  while (!EfiIsDevicePathEnd (DevicePath)) {
    DevicePath = EfiNextDevicePathNode (DevicePath);
  }

  //
  // Compute the size and add back in the size of the end device path structure
  //
  return ((UINTN) DevicePath - (UINTN) Start) + sizeof (EFI_DEVICE_PATH_PROTOCOL);
}


STATIC
CHAR16
NibbleToHexCharPrivate (
  IN UINT8                         Nibble
  )
/*++

  Routine Description:
    Converts the low nibble of a byte to hex unicode character.

  Arguments:
    Nibble - lower nibble of a byte.

  Returns:
    Hex unicode character between L'0' to L'f'.

--*/
{
  Nibble &= 0x0F;

  if (Nibble <= 0x9) {
    return (CHAR16)(Nibble + L'0');
  }

  return (CHAR16)(Nibble - 0xA + L'a');
}


STATIC
EFI_STATUS
BufToHexStringPrivate (
  IN OUT CHAR16                    *Str,
  IN OUT UINTN                     *HexStringBufferLength,
  IN     UINT8                     *Buf,
  IN     UINTN                     Len,
  IN     BOOLEAN                   Flag
  )
/*++

  Routine Description:
    Converts binary buffer to Unicode string.
    At a minimum, any blob of data could be represented as a hex string.

  Arguments:
    Str                   - Pointer to the string.
    HexStringBufferLength - Length in bytes of buffer to hold the hex string. Includes tailing '\0' character.
                            If routine return with EFI_SUCCESS, containing length of hex string buffer.
                            If routine return with EFI_BUFFER_TOO_SMALL, containg length of hex string buffer desired.
    Buf                   - Buffer to be converted from.
    Len                   - Length in bytes of the buffer to be converted.
    Flag                  - If TRUE, encode the data in the same order as the it 
                            resides in the Buf. Else encode it in the reverse direction.

  Returns:
    EFI_SUCCESS           - Routine  success.
    EFI_BUFFER_TOO_SMALL  - The hex string buffer is too small.

--*/
{
  UINTN       Idx;
  UINT8       Byte;
  UINTN       StrLen;

  //
  // Make sure string is either passed or allocate enough.
  // It takes 2 Unicode characters (4 bytes) to represent 1 byte of the binary buffer.
  // Plus the Unicode termination character.
  //
  StrLen = Len * 2;
  if ((*HexStringBufferLength) < (StrLen + 1) * sizeof (CHAR16)) {
    *HexStringBufferLength = (StrLen + 1) * sizeof (CHAR16);
    return EFI_BUFFER_TOO_SMALL;
  }

  *HexStringBufferLength = (StrLen + 1) * sizeof (CHAR16);
  
  //
  // Ends the string.
  //
  Str[StrLen] = 0;

  for (Idx = 0; Idx < Len; Idx++) {

    Byte = Buf[Idx];
    if (Flag) {
      Str[Idx * 2]     = NibbleToHexCharPrivate ((UINT8)(Byte >> 4));
      Str[Idx * 2 + 1] = NibbleToHexCharPrivate (Byte);
    } else {
      Str[StrLen - 1 - Idx * 2] = NibbleToHexCharPrivate (Byte);
      Str[StrLen - 2 - Idx * 2] = NibbleToHexCharPrivate ((UINT8)(Byte >> 4));
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GetDevicePath (
  IN  EFI_HII_CONFIG_ACCESS_PROTOCOL    *ConfigAccess,
  OUT EFI_STRING                        *DevicePathStr
  )
{
  EFI_STATUS                        Status;
  UINTN                             Index;
  UINTN                             NoHandles;
  EFI_HANDLE                        *HandleBuffer;
  EFI_HANDLE                        ConfigAccessHandle = NULL;
  EFI_HII_CONFIG_ACCESS_PROTOCOL    *TestedConfigAccess;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath;
  UINTN                             Length;
  UINTN                             PathHdrSize;

  //
  // locate all Hii Configuration Access Protocol Instances
  //
  Status = gtBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiHIIConfigAccessProtocolGuid,
                   NULL,
                   &NoHandles,
                   &HandleBuffer
                   );
  if (EFI_ERROR(Status) || (NoHandles == 0)) {
    return EFI_NOT_FOUND;
  }

  //
  // scan for the handle that matched with the Hii Configuration Access Protocol that
  // passed in by the test framework
  //
  for (Index = 0; Index < NoHandles; Index++) {
    Status = gtBS->HandleProtocol (
                     HandleBuffer[Index],
                     &gEfiHIIConfigAccessProtocolGuid,
                     &TestedConfigAccess
                     );
    if (EFI_ERROR(Status)) {
      continue;
    }

    if (TestedConfigAccess == ConfigAccess) {
      ConfigAccessHandle = HandleBuffer[Index];
      break;
    }
  }

  gtBS->FreePool (HandleBuffer);

  if (ConfigAccessHandle == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // find controller handles managed by the component name handle.
  //
  Status = gtBS->HandleProtocol (
                   ConfigAccessHandle,
                   &gEfiDevicePathProtocolGuid,
                   (void **)&DevicePath
                   );

  //
  // Convert the device path binary to hex UNICODE %02x bytes in the same order
  // as the device path resides in RAM memory.
  //
  Length = EfiDevicePathSize (DevicePath);
  PathHdrSize = (Length * 2 + 1) * sizeof (CHAR16);
  *DevicePathStr = (EFI_STRING) AllocateZeroPool (PathHdrSize);
  if (*DevicePathStr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = BufToHexStringPrivate (*DevicePathStr, &PathHdrSize, (UINT8 *) DevicePath, Length, TRUE);

  return Status;
}


EFI_STATUS
GetCorrespondingRequest (
  IN     EFI_STRING    MultiConfigAltResp,
  IN     EFI_STRING    DevicePath,
  IN OUT EFI_STRING    Request
  )
{
  EFI_STRING    RespPtr;
  EFI_STRING    ReqPtr;
  EFI_STRING    SingleResp;
  EFI_STRING    SingleRespPtr;
  UINTN         Len;
  
  ReqPtr = Request;
  RespPtr = MultiConfigAltResp;

  Len = StrLen(MultiConfigAltResp);
  
  if (EfiStrnCmp (MultiConfigAltResp, L"GUID=", 5) != 0) {
    return EFI_INVALID_PARAMETER;
  }
  
  if (EfiStrStr(MultiConfigAltResp, DevicePath) == NULL) {
    return EFI_NOT_FOUND;
  }

  SingleResp = (EFI_STRING) AllocateZeroPool( 2 * StrLen(MultiConfigAltResp) + 2 );
  if (SingleResp == NULL) {
	return EFI_OUT_OF_RESOURCES;
  }
  SingleRespPtr = SingleResp;

  while (*MultiConfigAltResp != 0) {
    while ((*MultiConfigAltResp != 0) && (EfiStrnCmp (MultiConfigAltResp, L"&GUID=", 6) != 0)) {
      *(SingleResp++) = *(MultiConfigAltResp++);
    }
	SingleResp = SingleRespPtr;
	if (EfiStrStr(SingleResp, DevicePath) != NULL)
      break;
	ZeroMem(SingleRespPtr, 2 * Len + 2);
	if (*MultiConfigAltResp != 0)
      MultiConfigAltResp++;
  }

  if (EfiStrStr(SingleResp, DevicePath) == NULL)
  	return EFI_NOT_FOUND;

  if (EfiStrStr(SingleResp, L"VALUE=") == NULL){
    while (*SingleResp != 0) {
      while ((*SingleResp != 0) && (EfiStrnCmp (SingleResp, L"&PATH=", 6) != 0)) {
        *(Request++) = *(SingleResp++);
	  }
	  *(Request++) = *(SingleResp++);
	  while ((*SingleResp != 0) && (*SingleResp != L'&')) {
        *(Request++) = *(SingleResp++);
      }
	  while (*SingleResp != 0){
        if (*SingleResp != L'=') {
          *(Request++) = *(SingleResp++);
		} else {
          while ((*SingleResp != 0) && (*SingleResp != L'&')) {
            SingleResp++;
          }
        }
      }
    }
  } else {
    while (*SingleResp != 0) {
      while ((*SingleResp != 0) && (EfiStrnCmp (SingleResp, L"&VALUE=", 7)) != 0) {
        *(Request++) = *(SingleResp++);
      }
	  SingleResp++;
	  while ((*SingleResp != 0) && (*SingleResp != L'&'))
        SingleResp++;
    }

  }

  *Request = 0;

  gtBS->FreePool(SingleRespPtr);
  return EFI_SUCCESS;
}


EFI_STATUS
GetCorrespondingResp (
  IN     EFI_STRING    MultiConfigAltResp,
  IN     EFI_STRING    DevicePath,
  IN OUT EFI_STRING    Resp
  )
{
  EFI_STRING    RespPtr;
  EFI_STRING    ReqPtr;
  UINTN         Len;
  
  ReqPtr = Resp;
  RespPtr = MultiConfigAltResp;

  Len = StrLen(MultiConfigAltResp);
  
  if (EfiStrnCmp (MultiConfigAltResp, L"GUID=", 5) != 0) {
    return EFI_INVALID_PARAMETER;
  }
  
  if (EfiStrStr(MultiConfigAltResp, DevicePath) == NULL) {
    return EFI_NOT_FOUND;
  }

  while (*MultiConfigAltResp != 0) {
    while ((*MultiConfigAltResp != 0) && (EfiStrnCmp (MultiConfigAltResp, L"&GUID=", 6) != 0)) {
      *(Resp++) = *(MultiConfigAltResp++);
    }
	Resp = ReqPtr;
	if (EfiStrStr(Resp, DevicePath) != NULL)
      break;
	ZeroMem(ReqPtr, 2 * Len + 2);
	if (*MultiConfigAltResp != 0)
      MultiConfigAltResp++;
  }

  if (EfiStrStr(Resp, DevicePath) == NULL)
  	return EFI_NOT_FOUND;
  
  return EFI_SUCCESS;
}



EFI_STATUS
GetHIIConfigRoutingInterface (
  OUT EFI_HII_CONFIG_ROUTING_PROTOCOL    **HIIConfigRouting
  )
{
  UINTN                                 NoHandles;
  EFI_HANDLE                            *HandleBuffer;
  EFI_STATUS                            Status;
  
  //
  // Get the HII Database Protocol interface
  //
  Status = gtBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiHIIConfigRoutingProtocolGuid,
                   NULL,
                   &NoHandles,
                   &HandleBuffer
                   );
  if ( EFI_ERROR(Status) ) {
    return Status;
  }
  
  if ( NoHandles <= 0 ) {
    return EFI_DEVICE_ERROR;
  }
  
  Status = gtBS->HandleProtocol (
                   HandleBuffer[0],
                   &gEfiHIIConfigRoutingProtocolGuid,
                   HIIConfigRouting
                   );
  if ( EFI_ERROR(Status) ) {
    gtBS->FreePool( HandleBuffer );
    return Status;
  }
  
  gtBS->FreePool( HandleBuffer );
  
  return EFI_SUCCESS;
}

EFI_STATUS
MultiAltRespToMultiReq (
  IN       EFI_STRING                           Resp,
  IN OUT   EFI_STRING                           Req
  )
{
  EFI_STRING Pointer1 = Resp;      
  EFI_STRING Pointer2 = NULL;
  EFI_STRING Pointer3 = Req;
  EFI_STRING CfgHdr = NULL;
  EFI_STRING FreePtr = NULL;
  CHAR8      Flag = 0;

  if (EfiStrnCmp (Pointer1, L"GUID=", 5) != 0) {    
    return EFI_INVALID_PARAMETER;
  }

  Pointer2 = (EFI_STRING) AllocateZeroPool (2 * StrLen(Resp) + 2);
  if (Pointer2 == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  FreePtr = Pointer2;
  
  while (*Pointer1) {
    if (EfiStrnCmp (Pointer1, L"GUID=", 5) == 0) {
      CfgHdr = Pointer2;
      *(Pointer2++) = *(Pointer1++);
	  while (*Pointer1 != L'&') {
        *(Pointer2++) = *(Pointer1++);
      }
    }
	if (EfiStrnCmp (Pointer1, L"&GUID=", 6) == 0) {
      *(Pointer2++) = *(Pointer1++);
      CfgHdr = Pointer2;
	  while (*Pointer1 != L'&') {
        *(Pointer2++) = *(Pointer1++);
      }
	}
	if (EfiStrnCmp (Pointer1, L"&NAME=", 6) == 0) {
      *(Pointer2++) = *(Pointer1++);
	  while (*Pointer1 != L'&') {
        *(Pointer2++) = *(Pointer1++);
      }
	}
	if (EfiStrnCmp (Pointer1, L"&PATH=", 6) == 0) {
      *(Pointer2++) = *(Pointer1++);
	  while (*Pointer1 != L'&') {
        *(Pointer2++) = *(Pointer1++);
      }
	  if (NULL == EfiStrStr(Req, CfgHdr)){
		if (*Req == L'G')
          *(Pointer3++) = L'&';
        EfiStrCat(Req, CfgHdr);
        Pointer3 += StrLen(CfgHdr);
		Flag = 1;
	  } else {
        Flag = 0;
	  }  
    }
    while ((Flag == 1) && (EfiStrnCmp (Pointer1, L"&GUID=", 6) != 0) && *Pointer1) {
      if (EfiStrnCmp (Pointer1, L"&OFFSET=", 8) == 0) {
        *(Pointer3++) = *(Pointer1++);
    	while (*Pointer1 != L'&') {
          *(Pointer3++) = *(Pointer1++);
        }
      }
      if (EfiStrnCmp (Pointer1, L"&WIDTH=", 7) == 0) {
        *(Pointer3++) = *(Pointer1++);
	    while (*Pointer1 != L'&') {
          *(Pointer3++) = *(Pointer1++);
        }
	  }
	  if (EfiStrnCmp (Pointer1, L"&VALUE=", 7) == 0) {
        Pointer1 += 7;
	    while (*Pointer1 != L'&' && *Pointer1) {
	      Pointer1++;
	    }
	  }
	  if (EfiStrnCmp (Pointer1, L"&ALTCFG=", 8) == 0) {
        Pointer1 += 8;
  	    while (*Pointer1 != L'&' && *Pointer1) {
	      Pointer1++;
	    }
	  }
	  if ((*Pointer1 == L'&') && (EfiStrnCmp (Pointer1, L"&GUID=", 6) != 0) && 
		  (EfiStrnCmp (Pointer1, L"&OFFSET=", 8) != 0) && (EfiStrnCmp (Pointer1, L"&WIDTH=", 7) != 0)) {
        *(Pointer3++) = *(Pointer1++);
	    while (*Pointer1 != L'=') {
          *(Pointer3++) = *(Pointer1++);
	    }
	    while (*Pointer1 != L'&' && *Pointer1) {
          Pointer1++;
	    }
	  }
	}
	Pointer1++;
  }

  FreePool(FreePtr);
  
  return EFI_SUCCESS;
}