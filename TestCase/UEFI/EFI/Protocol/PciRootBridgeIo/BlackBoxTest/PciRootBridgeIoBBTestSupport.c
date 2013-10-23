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

  PciRootBridgeIoBBTestSupport.c

Abstract:

  support source file for for Pci Root Bridge Io Protocol BB Test

--*/


#include "PciRootBridgeIoBBTestMain.h"
#include "PciRootBridgeIoBBTestSupport.h"
#include "Acpi.h"

//
//global varibles.
//

EFI_PCI_ROOT_BRIDGE_IO_DEVICE   *gRootBridgeIoDevices   = NULL;
UINTN                           gRootBridgeIoDevNumber  = 0;

EFI_DEVICE_PATH_PROTOCOL        *gDevicePath          = NULL;
CHAR16                          *gFilePath            = NULL;
static BOOLEAN                  Initialized           = FALSE;
//
//WidthCode String
//

WIDTHCODE  WidthCode[]  = {
  L"EfiPciWidthUint8",
  L"EfiPciWidthUint16",
  L"EfiPciWidthUint32",
  L"EfiPciWidthUint64",
  L"EfiPciWidthFifoUint8",
  L"EfiPciWidthFifoUint16",
  L"EfiPciWidthFifoUint32",
  L"EfiPciWidthFifoUint64",
  L"EfiPciWidthFillUint8",
  L"EfiPciWidthFillUint16",
  L"EfiPciWidthFillUint32",
  L"EfiPciWidthFillUint64",
  L"EfiPciWidthMaximum"
};

//
//MemoryTypeCode String
//
WIDTHCODE MemoryTypeCode[] = {
  L"EfiReservedMemoryType",
  L"EfiLoaderCode",
  L"EfiLoaderData",
  L"EfiBootServicesCode",
  L"EfiBootServicesData",
  L"EfiRuntimeServicesCode",
  L"EfiRuntimeServicesData",
  L"EfiConventionalMemory",
  L"EfiUnusableMemory",
  L"EfiACPIReclaimMemory",
  L"EfiACPIMemoryNVS",
  L"EfiMemoryMappedIO",
  L"EfiMemoryMappedIOPortSpace",
  L"EfiPalCode",
  L"EfiPreDxeAllocation",
  L"EfiMaxMemoryType"
};

//
//internal support functions
//

/**
 *  Print the Pci root brdige Device Path
 *  @param DevicePath the Device path of Root Bridge Device
 *  @return EFI_SUCCESS the device path string was printed successfully
 */
EFI_STATUS
PrintRootBridgeDevPath (
  IN  EFI_DEVICE_PATH_PROTOCOL     *DevicePath
  )
{
  CHAR16                      *DevicePathStr;

  if (DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DevicePathStr = DevicePathToStr (DevicePath);
  if (DevicePathStr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  gtST->ConOut->OutputString (gtST->ConOut, L"This Root Bridge Device Path:\r\n");
  gtST->ConOut->OutputString (gtST->ConOut, DevicePathStr);
  gtST->ConOut->OutputString (gtST->ConOut, L"\r\n");

  gtBS->FreePool (DevicePathStr);

  return EFI_SUCCESS;
}

/**
 *  get the if user want to go on testing.
 *  @return FALSE user don't want to go on  testing.
 *  @return TRUE  user wants to go on testing.
 */
BOOLEAN
QueryGoOnTesting (
  VOID
  )
{
  CHAR16        *InputBuffer;
  CHAR16        *TempBuffer;
  EFI_STATUS    Status;

  //
  //query user if go on testing.
  //
  InputBuffer = NULL;
  TempBuffer  = NULL;

  TempBuffer = PoolPrint (
                 L"go on testing (Y/N)? please reply in %ds\r\n",
                 WAIT_TIME
                 );

  if (TempBuffer == NULL) {
    return FALSE;
  }
  gtST->ConOut->OutputString (gtST->ConOut, TempBuffer);
  gtBS->FreePool (TempBuffer);

  //
  //get user response.
  //
  Status = GetUserInputOrTimeOut (
             &InputBuffer,
             WAIT_TIME
             );
  if (EFI_ERROR(Status) || InputBuffer == NULL) {
    return FALSE;
  }

  if (StrLen (InputBuffer) == 0) {
    gtBS->FreePool (InputBuffer);
    return FALSE;
  }

  if (MetaiMatch (InputBuffer, L"y")) {
    gtBS->FreePool (InputBuffer);
    return TRUE;
  } else {
    gtBS->FreePool (InputBuffer);
    return FALSE;
  }

}

/**
 *  get the address offset inputed by user.
 *  @param Offset the Address Offset returned by this function.
 *  @return EFI_SUCCESS.
 */
EFI_STATUS
GetAddress (
  IN UINT64    *Address
  )
{
  EFI_STATUS    Status;
  UINT64        AddressOffset;
  CHAR16        *InputBuffer;
  CHAR16        *TempBuffer;

  AddressOffset   = 0;
  InputBuffer     = NULL;
  TempBuffer      = NULL;

  TempBuffer = PoolPrint (
                 L"Please input the Address (Hex) in %ds.\r\n",
                 LONG_WAIT_TIME
                 );

  if (TempBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  gtST->ConOut->OutputString (gtST->ConOut, TempBuffer);
  gtBS->FreePool (TempBuffer);

  Status = GetUserInputOrTimeOut (
             &InputBuffer,
             LONG_WAIT_TIME
             );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  AddressOffset = (UINT64)XToUint64 (InputBuffer);

  gtBS->FreePool (InputBuffer);

  *Address = AddressOffset;

  return EFI_SUCCESS;
}

/**
 *  get the Target Value inputed by user.
 *  @param TargetValue the Target Value returned by this function.
 *  @return EFI_SUCCESS.
 */
EFI_STATUS
GetTargetValue (
  IN UINT64    *TargetValue
  )
{
  EFI_STATUS    Status;
  CHAR16        *InputBuffer;
  CHAR16        *TempBuffer;

  InputBuffer     = NULL;
  TempBuffer      = NULL;

  TempBuffer = PoolPrint (
                 L"Please input the Target Value(Hex) in %ds.\r\n",
                 LONG_WAIT_TIME
                 );

  if (TempBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  gtST->ConOut->OutputString (gtST->ConOut, TempBuffer);

  gtBS->FreePool (TempBuffer);

  Status = GetUserInputOrTimeOut (
             &InputBuffer,
             LONG_WAIT_TIME
             );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  *TargetValue = (UINT64)XToUint64 (InputBuffer);

  gtBS->FreePool (InputBuffer);

  return EFI_SUCCESS;
}


/**
 *  get the Alternate Value inputed by user.
 *  @param TargetValue the Target Value returned by this function.
 *  @return EFI_SUCCESS.
 */
EFI_STATUS
GetAlternateValue (
  IN UINT64    *AlternateValue
  )
{
  EFI_STATUS    Status;
  CHAR16        *InputBuffer;
  CHAR16        *TempBuffer;

  InputBuffer     = NULL;
  TempBuffer      = NULL;

  TempBuffer = PoolPrint (
                 L"Please input the Alternate Value(Hex) in %ds.\r\n",
                 LONG_WAIT_TIME
                 );

  if (TempBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  gtST->ConOut->OutputString (gtST->ConOut, TempBuffer);

  gtBS->FreePool (TempBuffer);

  Status = GetUserInputOrTimeOut (
             &InputBuffer,
             LONG_WAIT_TIME
             );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  *AlternateValue = (UINT64)XToUint64 (InputBuffer);

  gtBS->FreePool (InputBuffer);

  return EFI_SUCCESS;
}

/**
 *  get the Data uints inputed by user.
 *  @param Length the Data length.
 *  @param DataUnits the data Units returned by this function.
 *  @return EFI_SUCCESS.
 */
EFI_STATUS
GetDataUnits (
  IN UINTN         Length,
  IN UINT8         **DataUnits
  )
{
  EFI_STATUS    Status;
  CHAR16        *InputBuffer;
  CHAR16        *TempBuffer;
  UINT8         *ReturnBuffer;

  InputBuffer = NULL;
  TempBuffer  = NULL;

  TempBuffer = PoolPrint (
                 L"Please input the DataUnits(Hex) in %ds\r\n",
                 LONG_WAIT_TIME
                 );

  if (TempBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  gtST->ConOut->OutputString (gtST->ConOut, TempBuffer);

  gtBS->FreePool (TempBuffer);

  Status = GetUserInputOrTimeOut (
             &InputBuffer,
             LONG_WAIT_TIME
             );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (StrLen (InputBuffer) == 0) {
    return EFI_NOT_FOUND;
  }

  Status = ConvertStringToHex (
             InputBuffer,
             (UINT32)Length,
             &ReturnBuffer
             );

  gtBS->FreePool (InputBuffer);

  if (EFI_ERROR(Status)) {
    return Status;
  }

  *DataUnits = ReturnBuffer;

  return EFI_SUCCESS;
}

/**
 *  get the Address Length inputed by user.
 *  @param AddressLength the address length returned by this function.
 *  @return EFI_SUCCESS.
 */
EFI_STATUS
GetAddressLength (
  IN UINT32    *AddressLength
  )
{
  EFI_STATUS    Status;
  CHAR16        *InputBuffer;
  CHAR16        *TempBuffer;

  InputBuffer     = NULL;
  TempBuffer      = NULL;

  TempBuffer = PoolPrint (
                 L"Please input the Address Length (Hex) in %ds.\r\n",
                 WAIT_TIME
                 );

  if (TempBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  gtST->ConOut->OutputString (gtST->ConOut, TempBuffer);
  gtBS->FreePool (TempBuffer);

  Status = GetUserInputOrTimeOut (
             &InputBuffer,
             WAIT_TIME
             );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  *AddressLength = (UINT32)XToUint64 (InputBuffer);

  gtBS->FreePool (InputBuffer);

  return EFI_SUCCESS;
}

/**
 *  get thePci Io Width inputed by user.
 *  @param PcIoWidth the Pci Io Width returned by this function.
 *  @return EFI_SUCCESS.
 */
EFI_STATUS
GetRootBridgeIoWidth (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH             *RootBridgeIoWidth
  )
{
  EFI_STATUS    Status;
  UINT32        Width;
  CHAR16        *InputBuffer;
  CHAR16        *TempBuffer;

  InputBuffer = NULL;
  TempBuffer  = NULL;

  TempBuffer = PoolPrint (
                 L"Please input the Width (8/16/32/64)bits in %ds\r\n",
                 WAIT_TIME
                 );

  if (TempBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  gtST->ConOut->OutputString (gtST->ConOut, TempBuffer);
  gtBS->FreePool (TempBuffer);

  Status = GetUserInputOrTimeOut (
             &InputBuffer,
             WAIT_TIME
             );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  Width = (UINT32)AToUint64 (InputBuffer);

  gtBS->FreePool (InputBuffer);

  switch (Width) {
    case 8:
      *RootBridgeIoWidth = EfiPciWidthUint8;
      break;
    case 16:
      *RootBridgeIoWidth = EfiPciWidthUint16;
      break;
    case 32:
      *RootBridgeIoWidth = EfiPciWidthUint32;
      break;
    case 64:
      *RootBridgeIoWidth = EfiPciWidthUint64;
      break;
    default:
      return EFI_ABORTED;
      break;
  }

  return EFI_SUCCESS;
}

/**
 *  get the device path from the profile.
 *  @param FileHandle the EFI_INI_FILE_HANDLE instance.
 *  @param SectionName the Section Name string pointer.
 *  @param Order the order of the section.
 *  @param DevicePath the returned devicepath string.
 *  @return EFI_SUCCESS the devicepath was get successfully.
 */
EFI_STATUS
GetSystemDevicePathByFile (
  IN EFI_INI_FILE_HANDLE  FileHandle,
  IN CHAR16               *SectionName,
  IN UINTN                Order,
  OUT CHAR16              **DevicePath
  )
{
  CHAR16          Buffer[MAX_STRING_LEN];
  EFI_STATUS      Status;
  UINT32          MaxLen;
  CHAR16          *TempStr;

  if (SectionName == NULL || DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MaxLen    = MAX_STRING_LEN;
  Buffer[0] = '\0';

  Status = FileHandle->GetStringByOrder (
                         FileHandle,
                         (UINT32)Order,
                         SectionName,
                         L"DevicePath",
                         Buffer,
                         (UINT32 *)&MaxLen
                         );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (StrLen (Buffer) == 0) {
    return EFI_NOT_FOUND;
  }

  TempStr = NULL;
  TempStr = StrDuplicate (Buffer);
  if (TempStr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *DevicePath = TempStr;

  return EFI_SUCCESS;
}

/**
 *  get the Address from the profile.
 *  @param FileHandle the EFI_INI_FILE_HANDLE instance.
 *  @param SectionName the Section Name string pointer.
 *  @param Order the order of the section.
 *  @param Address the returned Address Offset.
 *  @return EFI_SUCCESS the Address Offset was get successfully.
 */
EFI_STATUS
GetAddressByFile (
  IN EFI_INI_FILE_HANDLE  FileHandle,
  IN CHAR16               *SectionName,
  IN UINTN                Order,
  OUT UINT64              *Address
  )
{
  CHAR16          Buffer[MAX_STRING_LEN];
  EFI_STATUS      Status;
  UINT32          MaxLen;


  if (SectionName == NULL || Address == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MaxLen    = MAX_STRING_LEN;
  Buffer[0] = '\0';

  Status = FileHandle->GetStringByOrder (
                         FileHandle,
                         (UINT32)Order,
                         SectionName,
                         L"Address",
                         Buffer,
                         (UINT32 *)&MaxLen
                         );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (StrLen (Buffer) == 0) {
    return EFI_NOT_FOUND;
  }

  *Address = (UINT64)XToUint64 (Buffer);

  return EFI_SUCCESS;
}

/**
 *  get the Data Units from the profile.
 *  @param FileHandle the EFI_INI_FILE_HANDLE instance.
 *  @param SectionName the Section Name string pointer.
 *  @param Order the order of the section.
 *  @param Length the Data Units Length.
 *  @param DataUnits returned Data Units pointer.
 *  @return EFI_SUCCESS the DataUnits was get successfully.
 */
EFI_STATUS
GetDataUnitsByFile (
  IN EFI_INI_FILE_HANDLE  FileHandle,
  IN CHAR16               *SectionName,
  IN UINTN                Order,
  UINTN                   Length,
  OUT UINT8               **DataUnits
  )
{
  CHAR16          Buffer[1024];
  EFI_STATUS      Status;
  UINT32          MaxLen;
  UINT8           *ReturnBuffer;

  if (SectionName == NULL || DataUnits == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  MaxLen    = 1024;
  Buffer[0] = '\0';

  Status = FileHandle->GetStringByOrder (
                         FileHandle,
                         (UINT32)Order,
                         SectionName,
                         L"DataUnits",
                         Buffer,
                         (UINT32 *)&MaxLen
                         );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (StrLen (Buffer) == 0) {
    return EFI_NOT_FOUND;
  }
  ReturnBuffer = NULL;

  Status = ConvertStringToHex (
             Buffer,
             (UINT32)Length,
             &ReturnBuffer
             );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  *DataUnits = ReturnBuffer;

  return EFI_SUCCESS;
}

/**
 *  get the Address Length from the profile.
 *  @param FileHandle the EFI_INI_FILE_HANDLE instance.
 *  @param SectionName the Section Name string pointer.
 *  @param Order the order of the section.
 *  @param AddressLength the returned Address Length.
 *  @return EFI_SUCCESS the Address Length was get successfully.
 */
EFI_STATUS
GetAddressLengthByFile (
  IN EFI_INI_FILE_HANDLE  FileHandle,
  IN CHAR16               *SectionName,
  IN UINTN                Order,
  OUT UINT32              *AddressLength
  )
{
  CHAR16          Buffer[MAX_STRING_LEN];
  EFI_STATUS      Status;
  UINT32          MaxLen;


  if (SectionName == NULL || AddressLength == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MaxLen    = MAX_STRING_LEN;
  Buffer[0] = '\0';

  Status = FileHandle->GetStringByOrder (
                         FileHandle,
                         (UINT32)Order,
                         SectionName,
                         L"Length",
                         Buffer,
                         (UINT32 *)&MaxLen
                         );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (StrLen (Buffer) == 0) {
    return EFI_NOT_FOUND;
  }

  *AddressLength = (UINT32)XToUint64 (Buffer);

  return EFI_SUCCESS;
}

/**
 *  get the Pci Io Width from the profile.
 *  @param FileHandle the EFI_INI_FILE_HANDLE instance.
 *  @param SectionName the Section Name string pointer.
 *  @param Order the order of the section.
 *  @param PciIoWidth the returned Pci Io Width.
 *  @return EFI_SUCCESS the  Pci Io Width was gotten successfully.
 */
EFI_STATUS
GetRootBridgeIoWidthByFile (
  IN EFI_INI_FILE_HANDLE                      FileHandle,
  IN CHAR16                                   *SectionName,
  IN UINTN                                    Order,
  OUT EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH   *RootBridgeIoWidth
  )
{
  CHAR16          Buffer[MAX_STRING_LEN];
  EFI_STATUS      Status;
  UINT32          MaxLen;
  UINTN           Index;

  if (SectionName == NULL || RootBridgeIoWidth == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MaxLen    = MAX_STRING_LEN;
  Buffer[0] = '\0';

  Status = FileHandle->GetStringByOrder (
                         FileHandle,
                         (UINT32)Order,
                         SectionName,
                         L"RootBridgeIoWidth",
                         Buffer,
                         (UINT32 *)&MaxLen
                         );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (StrLen (Buffer) == 0) {
    return EFI_NOT_FOUND;
  }

  for (Index = 0; Index < EfiPciWidthMaximum; Index++) {
    if (StriCmp (WidthCode[Index], Buffer) == 0) {
      *RootBridgeIoWidth = (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH)Index;
      return EFI_SUCCESS;
    }
  }

  //
  //not found
  //

  return EFI_NOT_FOUND;
}

/**
 *  get the Target Value from the profile.
 *  @param FileHandle the EFI_INI_FILE_HANDLE instance.
 *  @param SectionName the Section Name string pointer.
 *  @param Order the order of the section.
 *  @param TargetValue the returned Target Value.
 *  @return EFI_SUCCESS the Target Value was get successfully.
 */
EFI_STATUS
GetTargetValueByFile (
  IN EFI_INI_FILE_HANDLE  FileHandle,
  IN CHAR16               *SectionName,
  IN UINTN                Order,
  OUT UINT64              *TargetValue
  )
{
  CHAR16          Buffer[MAX_STRING_LEN];
  EFI_STATUS      Status;
  UINT32          MaxLen;


  if (SectionName == NULL || TargetValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MaxLen    = MAX_STRING_LEN;
  Buffer[0] = '\0';

  Status = FileHandle->GetStringByOrder (
                         FileHandle,
                         (UINT32)Order,
                         SectionName,
                         L"TargetValue",
                         Buffer,
                         (UINT32 *)&MaxLen
                         );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (StrLen (Buffer) == 0) {
    return EFI_NOT_FOUND;
  }

  *TargetValue = (UINT64)XToUint64 (Buffer);

  return EFI_SUCCESS;
}

/**
 *  get the Alternate Value from the profile.
 *  @param FileHandle the EFI_INI_FILE_HANDLE instance.
 *  @param SectionName the Section Name string pointer.
 *  @param Order the order of the section.
 *  @param AlternateValue the returned Alternate Value.
 *  @return EFI_SUCCESS the Alternate Value was get successfully.
 */
EFI_STATUS
GetAlternateValueByFile (
  IN EFI_INI_FILE_HANDLE  FileHandle,
  IN CHAR16               *SectionName,
  IN UINTN                Order,
  OUT UINT64              *AlternateValue
  )
{
  CHAR16          Buffer[MAX_STRING_LEN];
  EFI_STATUS      Status;
  UINT32          MaxLen;


  if (SectionName == NULL || AlternateValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MaxLen    = MAX_STRING_LEN;
  Buffer[0] = '\0';

  Status = FileHandle->GetStringByOrder (
                         FileHandle,
                         (UINT32)Order,
                         SectionName,
                         L"AlternateValue",
                         Buffer,
                         (UINT32 *)&MaxLen
                         );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (StrLen (Buffer) == 0) {
    return EFI_NOT_FOUND;
  }

  *AlternateValue = (UINT64)XToUint64 (Buffer);

  return EFI_SUCCESS;
}
/**
 *  convert the sring value into hex value.
 *  @param SrcBuffer the input string buffer.
 *  @param Length return buffer length
 *  @param RetBuffer the return Buffer pointer.
 *  @return EFI_SUCCESS the string was converted into hex value successfully.
 */
EFI_STATUS
ConvertStringToHex (
  IN CHAR16             *SrcBuffer,
  IN UINT32             Length,
  OUT UINT8             **RetBuffer
  )
{
  UINTN               Index;
  UINTN               ConvertBytes;
  CHAR16              Buffer[4];
  UINT8               *TempBuffer;
  CHAR16              *StrPtr;

  if (SrcBuffer == NULL || RetBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  StrPtr = SrcBuffer;

  //
  //skip the space if it really has.
  //

  while (*StrPtr == L' ') {
    StrPtr = StrPtr + 1;
  }

  //
  //skip the "0x" prefix if it really has.
  //

  if (StrLen (StrPtr) >= 2) {
    if (StrPtr[0] == L'0' && (StrPtr[1] == L'x' || StrPtr[1] == L'X')) {
      StrPtr = StrPtr + 2;
    }
  }

  ConvertBytes = StrLen (StrPtr) / 2;
  if (ConvertBytes > Length) {
    ConvertBytes = Length;
  }

  TempBuffer = NULL;
  TempBuffer = AllocatePool (Length);
  if (TempBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Buffer[2] =0x00;

  //
  //convert string to hex value.
  //

  for (Index = 0; Index < ConvertBytes; Index++) {
    Buffer[0] = StrPtr[Index * 2];
    Buffer[1] = StrPtr[Index * 2 + 1];
    TempBuffer[Index] = (UINT8) XToUint64 (Buffer);
  }

  if (ConvertBytes < Length) {
    for (Index = ConvertBytes; Index < Length; Index++) {
      TempBuffer[Index] = (UINT8) (Index & 0xFF);
    }
  }

  *RetBuffer = TempBuffer;

  return EFI_SUCCESS;
}

/**
 *  get Pci Io Device by PciIo protocol interface.
 *  @param PciIo the PCI_IO_PROTOCOL interface.
 *  @return PCI_IO_PROTOCOL_DEVICE instance.
 */
EFI_PCI_ROOT_BRIDGE_IO_DEVICE *
GetRootBridgeIoDevice (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *RootBridgeIo
  )
{
  UINTN            Index;

  for (Index = 0; Index < gRootBridgeIoDevNumber; Index++) {
    if (gRootBridgeIoDevices[Index].RootBridgeIo == RootBridgeIo) {
      return &gRootBridgeIoDevices[Index];
    }
  }
  return NULL;
}

/**
 *  create all the Pci Root Bridge Io Devices in this system.
 *  @return EFI_SUCCESS the all the devices were gotten successfully.
 */
EFI_STATUS
InitializeCaseEnvironment (
  VOID
)
{
  EFI_STATUS                          Status;
  UINTN                               HandleNum;
  EFI_HANDLE                          *HandleBuffer;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *RootBridgeIo;
  EFI_DEVICE_PATH_PROTOCOL            *DevicePath;
  UINTN                               Index;


  //
  //get PciRootBrigeIo Protocol Interface.
  //

  HandleNum     = 0;
  HandleBuffer = NULL;
  if (Initialized) {
    return EFI_SUCCESS;
  }

  Status      = LibLocateHandle (
                  ByProtocol,
                  &gEfiPciRootBridgeIoProtocolGuid,
                  NULL,
                  &HandleNum,
                  &HandleBuffer
                  );

  if (EFI_ERROR(Status) || HandleNum == 0) {
    return EFI_ABORTED;
  }

  gRootBridgeIoDevices = (EFI_PCI_ROOT_BRIDGE_IO_DEVICE *)AllocateZeroPool (
                           sizeof (EFI_PCI_ROOT_BRIDGE_IO_DEVICE) * HandleNum);

  if (gRootBridgeIoDevices == NULL) {
    FreePool (HandleBuffer);
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < HandleNum; Index++) {

    Status = gtBS->HandleProtocol (
                     HandleBuffer[Index],
                     &gEfiPciRootBridgeIoProtocolGuid,
                     &RootBridgeIo
                     );

    if (EFI_ERROR(Status)) {
      FreePool (HandleBuffer);
      FreePool (gRootBridgeIoDevices);
      return Status;
    }

    gRootBridgeIoDevices[Index].RootBridgeIo = RootBridgeIo;

    Status = gtBS->HandleProtocol (
                     HandleBuffer[Index],
                     &gEfiDevicePathProtocolGuid,
                     &DevicePath
                     );

    if (EFI_ERROR(Status)) {
      FreePool (HandleBuffer);
      FreePool (gRootBridgeIoDevices);
      return Status;
    }

    gRootBridgeIoDevices[Index].DevicePath = DevicePath;

    //
    //get the root bridge resource information.
    //
    Status = GetRootBridgeIoDeviceInfo (&gRootBridgeIoDevices[Index]);

    if (EFI_ERROR(Status)) {
      FreePool (HandleBuffer);
      FreePool (gRootBridgeIoDevices);
      return Status;
    }

  }

  gRootBridgeIoDevNumber = HandleNum;

  FreePool (HandleBuffer);
  Initialized = TRUE;
  return EFI_SUCCESS;
}

/**
 *  Get a valid Memory Range from root bridge.
 *  @param RBDev the root bridge Io device pointer.
 *  @param Address the returned Valid Memory begin address.
 *  @return EFI_SUCCESS the Valid Memory address was got successfully.
 */
EFI_STATUS
GetValidMemoryAddress (
  IN EFI_PCI_ROOT_BRIDGE_IO_DEVICE *RBDev,
  IN UINT64                        *Address
  )
{

  if (RBDev->Mem32Length > 0) {

    //
    //found a valid memory range.
    //

    *Address = RBDev->Mem32Base;
    return EFI_SUCCESS;
  }


  if (RBDev->PreMem32Length > 0) {

    //
    //found a valid memory range.
    //

    *Address = RBDev->PreMem32Base;
    return EFI_SUCCESS;
  }


  if (RBDev->Mem64Length > 0) {

    //
    //found a valid memory range.
    //

    *Address = RBDev->Mem64Base;
    return EFI_SUCCESS;
  }

  if (RBDev->PreMem64Length > 0) {

    //
    //found a valid memory range.
    //

    *Address = RBDev->PreMem64Base;
    return EFI_SUCCESS;
  }

  //
  //if come here then no valid memory range found.
  //

  return EFI_NOT_FOUND;
}

/**
 *  Get some Memory Range from root bridge.
 *  @param RBDev the root bridge Io device pointer.
 *  @param Address the returned Valid Memory begin address.
 *  @param Length  the returned Valid Memory length.
 *  @return EFI_SUCCESS the Valid Memory address was got successfully.
 */
EFI_STATUS
GetSomeMemeryResource (
  IN EFI_PCI_ROOT_BRIDGE_IO_DEVICE *RBDev,
  IN UINT64                        *Address,
  IN UINT64                        *Length
  )
{
  if (RBDev->PreMem32Length > 0) {

    //
    //found a valid memory range.
    //

    *Address = RBDev->PreMem32Base;
    *Length  = RBDev->PreMem32Length;
    return EFI_SUCCESS;
  }

  if (RBDev->PreMem64Length > 0) {

    //
    //found a valid memory range.
    //

    *Address = RBDev->PreMem64Base;
    *Length  = RBDev->PreMem64Length;
    return EFI_SUCCESS;
  }

  if (RBDev->Mem32Length > 0) {

    //
    //found a valid memory range.
    //

    *Address = RBDev->Mem32Base;
    *Length  = RBDev->Mem32Length;
    return EFI_SUCCESS;
  }

  if (RBDev->Mem64Length > 0) {

    //
    //found a valid memory range.
    //

    *Address = RBDev->Mem64Base;
    *Length  = RBDev->Mem64Length;
    return EFI_SUCCESS;
  }

  //
  //if come here then no valid memory range found.
  //

  return EFI_NOT_FOUND;
}

/**
 *  Get a Invalid Memory Range from root bridge.
 *  @param RBDev the root bridge Io device pointer.
 *  @param Address the returned InValid Memory address.
 *  @return EFI_SUCCESS the InValid Memory address was got successfully.
 */
EFI_STATUS
GetInValidMemoryAddress (
  IN EFI_PCI_ROOT_BRIDGE_IO_DEVICE *RBDev,
  IN UINT64                        *Address
  )
{
  UINT64    FourGBAddress;
  BOOLEAN   CanBeZero;

  CanBeZero = FALSE;

  FourGBAddress = ((UINT64)((UINT32)(-1)) + 1);

  //
  //First we found below 4GB.
  //

  if (RBDev->Mem32Length == 0 && RBDev->PreMem32Length == 0) {
     *Address = 0;
     return EFI_SUCCESS;
  } else if (RBDev->Mem32Length > 0 && RBDev->PreMem32Length > 0) {
    if (RBDev->Mem32Base > RBDev->PreMem32Base) {

      if (RBDev->PreMem32Base > 0) {
        CanBeZero = TRUE;
      }

      if ((RBDev->PreMem32Base + RBDev->PreMem32Length) != RBDev->Mem32Base) {
        *Address = RBDev->PreMem32Base + RBDev->PreMem32Length;
        return EFI_SUCCESS;
      }

      if ((RBDev->Mem32Length + RBDev->Mem32Base) <= (UINT32)(-1)) {
        *Address = RBDev->Mem32Base + RBDev->Mem32Length;
        return EFI_SUCCESS;
      }
    } else {

      if (RBDev->Mem32Base > 0) {
        CanBeZero = TRUE;
      }

      if ((RBDev->Mem32Base + RBDev->Mem32Length) != RBDev->PreMem32Base) {
        *Address = RBDev->Mem32Base + RBDev->Mem32Length;
        return EFI_SUCCESS;
      }

      if ((RBDev->PreMem32Length + RBDev->PreMem32Base) <= (UINT32)(-1)) {
        *Address = RBDev->PreMem32Base + RBDev->PreMem32Length;
        return EFI_SUCCESS;
      }

    }
  } else if (RBDev->Mem32Length > 0) {

    if (RBDev->Mem32Base > 0) {
      CanBeZero = TRUE;
    }

    if ((RBDev->Mem32Base + RBDev->Mem32Length) <= (UINT32)(-1)) {
      *Address = RBDev->Mem32Base + RBDev->Mem32Length;
      return EFI_SUCCESS;
    }
  } else if (RBDev->PreMem32Length > 0) {

    if (RBDev->PreMem32Base > 0) {
      *Address = 0;
      return EFI_SUCCESS;
    }

    if ((RBDev->PreMem32Base + RBDev->PreMem32Length) <= (UINT32)(-1)) {
      *Address = RBDev->PreMem32Base + RBDev->PreMem32Length;
      return EFI_SUCCESS;
    }
  }

  if (CanBeZero) {
    *Address = 0;
    return EFI_SUCCESS;
  }

  //
  //then found an invalid address above 4GB.
  //

  if (RBDev->Mem64Length == 0 && RBDev->PreMem64Length == 0) {
    *Address = FourGBAddress;
    return EFI_SUCCESS;
  } else if (RBDev->Mem64Length > 0 && RBDev->PreMem64Length > 0) {
    if (RBDev->Mem64Base > RBDev->PreMem64Base) {

      if (RBDev->PreMem64Base > FourGBAddress) {
        *Address = FourGBAddress;
        return EFI_SUCCESS;
      }

      if ((RBDev->PreMem64Base + RBDev->PreMem64Length) != RBDev->Mem64Base) {
        *Address = RBDev->PreMem64Base + RBDev->PreMem64Length;
        return EFI_SUCCESS;
      }

      if (((UINT64)(-1)- RBDev->Mem64Base + 1) > RBDev->Mem64Length) {
        *Address = RBDev->Mem64Base + RBDev->Mem64Length;
        return EFI_SUCCESS;
      }

    } else {

      if (RBDev->Mem64Base > FourGBAddress) {
        *Address = FourGBAddress;
        return EFI_SUCCESS;
      }

      if ((RBDev->Mem64Base + RBDev->Mem64Length) != RBDev->PreMem64Base) {
        *Address = RBDev->Mem64Base + RBDev->Mem64Length;
        return EFI_SUCCESS;
      }

      if (((UINT64)(-1)- RBDev->PreMem64Base + 1) > RBDev->PreMem64Length) {
        *Address = RBDev->PreMem64Base + RBDev->PreMem64Length;
        return EFI_SUCCESS;
      }

    }
  } else if (RBDev->Mem64Length > 0) {

    if (RBDev->Mem64Base > FourGBAddress) {
      *Address = FourGBAddress;
      return EFI_SUCCESS;
    }

    if (((UINT64)(-1)- RBDev->Mem64Base + 1) > RBDev->Mem64Length) {
      *Address = RBDev->Mem64Base + RBDev->Mem64Length;
      return EFI_SUCCESS;
    }
  } else if (RBDev->PreMem64Length > 0) {

    if (RBDev->PreMem64Base > FourGBAddress) {
      *Address = FourGBAddress;
      return EFI_SUCCESS;
    }

    if (((UINT64)(-1)- RBDev->PreMem64Base + 1) > RBDev->PreMem64Length) {
      *Address = RBDev->PreMem64Base + RBDev->PreMem64Length;
      return EFI_SUCCESS;
    }
  }


  //
  //if come here then no valid memory range found.
  //

  return EFI_NOT_FOUND;
}


/**
 *  Get the Root Bridge Resource info.
 *  @param RBDev the root bridge Io device pointer.
 *  @return EFI_SUCCESS the resource was got successfully.
 */
EFI_STATUS
GetRootBridgeIoDeviceInfo (
  IN EFI_PCI_ROOT_BRIDGE_IO_DEVICE *RBDev
  )
{
  EFI_STATUS                          Status;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR   *Ptr;
  VOID                                *Resources;

  Resources = NULL;

  Status = RBDev->RootBridgeIo->Configuration (
                                  RBDev->RootBridgeIo,
                                  &Resources
                                  );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  //Travel all the Resource list.
  //

  Ptr = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)Resources;

  while (Ptr->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR) {
    switch (Ptr->ResType) {

      case ACPI_ADDRESS_SPACE_TYPE_BUS:
        if (Ptr->AddrLen > 0) {
          RBDev->PriBus = (UINT8)Ptr->AddrRangeMin;
          RBDev->SubBus = (UINT8)(Ptr->AddrRangeMin  + (UINT8)Ptr->AddrLen - 1);
        }
        break;

      case ACPI_ADDRESS_SPACE_TYPE_IO:
        RBDev->IOBase   = (UINT32)Ptr->AddrRangeMin;
        RBDev->IOLength = (UINT32)Ptr->AddrLen;
        break;

      case ACPI_ADDRESS_SPACE_TYPE_MEM:
        if (Ptr->AddrSpaceGranularity == 32) {
          if (Ptr->SpecificFlag == 0x6) {
            RBDev->PreMem32Base   = (UINT32)Ptr->AddrRangeMin;
            RBDev->PreMem32Length = (UINT32)Ptr->AddrLen;
          } else {
            RBDev->Mem32Base   = (UINT32)Ptr->AddrRangeMin;
            RBDev->Mem32Length = (UINT32)Ptr->AddrLen;
          }
        } else if (Ptr->AddrSpaceGranularity == 64) {
          if (Ptr->SpecificFlag == 0x6) {
            RBDev->PreMem64Base   = Ptr->AddrRangeMin;
            RBDev->PreMem64Length = Ptr->AddrLen;
          } else {
            RBDev->Mem64Base   = Ptr->AddrRangeMin;
            RBDev->Mem64Length = Ptr->AddrLen;
          }
        }

        break;

      default:
        break;
    }

    Ptr++;
  }

  return EFI_SUCCESS;
}

/*
 *  get the system device path and file path.
 *  @param ProfileLib the Profile Library Protocol instance.
 *  @return EFI_SUCCESS the system device path and file path were gotten successfully.
 */
EFI_STATUS
GetSystemData (
  IN EFI_TEST_PROFILE_LIBRARY_PROTOCOL  *ProfileLib
  )
{
  EFI_DEVICE_PATH_PROTOCOL    *TempDevicePath;
  CHAR16                      *TempFilePath;
  EFI_STATUS                  Status;

  //
  // If gFilePath and gDevicePath has been assigned, return directly.
  //
  if ((gFilePath != NULL) && (gDevicePath != NULL)) {
    return EFI_SUCCESS;
  }

  //
  // Free gFilePath or gDevicePath
  //
  if (gFilePath != NULL) {
    gtBS->FreePool (gFilePath);
    gFilePath = NULL;
  }
  if (gDevicePath != NULL) {
    gtBS->FreePool (gDevicePath);
    gDevicePath = NULL;
  }

  //
  //Get system device path and file path
  //
  Status = ProfileLib->EfiGetSystemDevicePath (
                          ProfileLib,
                          &TempDevicePath,
                          &TempFilePath
                          );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  //make up the file path.
  //
  gFilePath = NULL;
  gFilePath = PoolPrint (L"%s\\%s", TempFilePath, DEPENDECY_DIR_NAME);

  gtBS->FreePool (TempFilePath);

  if (gFilePath == NULL) {
    gtBS->FreePool (TempDevicePath);
    return EFI_OUT_OF_RESOURCES;
  }

  gDevicePath = TempDevicePath;

  //
  // Done, return status code EFI_SUCCESS
  //
  return EFI_SUCCESS;
}

/*
 *  get the device handle.
 *  @param ImageHandle the Image Handle instance.
 *  @return EFI_SUCCESS the device handle was gotten successfully.
 */
EFI_STATUS
GetSystemDevicePathAndFilePath (
  IN EFI_HANDLE           ImageHandle
  )
{

  //
  // Done, return status code EFI_SUCCESS
  //
  return EFI_SUCCESS;
}

/**
 *  the Event Notify function it will write the assigned value
 *  into the destinaiton address.
 *  @param Event the Event to be processed.
 *  @param Contex the Event context.
 */
VOID
EventNotifyWriteMem (
  IN EFI_EVENT      Event,
  IN  VOID          *Context
  )
{
  TIMER_EVENT_CONTEXT                 *EventContext;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *RootBridgeIo;
  EFI_TEST_ASSERTION                  AssertionType;
  UINT64                              ReadValue;
  EFI_STATUS                          Status;

  EventContext = (TIMER_EVENT_CONTEXT *)Context;
  RootBridgeIo = EventContext->RootBridgeIo;

  Status = RootBridgeIo->Mem.Write (
                               RootBridgeIo,
                               EventContext->RootBridgeIoWidth,
                               EventContext->Address,
                               1,
                               &EventContext->DestValue
                               );

  if (!EFI_ERROR(Status)) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  EventContext->StandardLib->RecordAssertion (
                               EventContext->StandardLib,
                               AssertionType,
                               gTestGenericFailureGuid,
                               L"EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.Mem.Write - time out event to write target value",
                               L"%a:%d:status - %r,write value - %08Xh",
                               __FILE__,
                               (UINTN)__LINE__,
                               Status,
                               EventContext->DestValue
                               );

 ReadValue = 0x00;

  Status = RootBridgeIo->Mem.Read (
                               RootBridgeIo,
                               EventContext->RootBridgeIoWidth,
                               EventContext->Address,
                               1,
                               &ReadValue
                               );

  if (!EFI_ERROR(Status)) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }

  EventContext->StandardLib->RecordAssertion (
                               EventContext->StandardLib,
                               AssertionType,
                               gTestGenericFailureGuid,
                               L"EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.Mem.Read - return status should be EFI_SUCCESS",
                               L"%a:%d:status - %r,write value - %LXh, ReadValue - %LXh",
                               __FILE__,
                               (UINTN)__LINE__,
                               Status,
                               EventContext->DestValue,
                               ReadValue
                               );

  return;

}


/**
 *  the Event Notify function it will write the assigned value
 *  into the destinaiton address.
 *  @param Event the Event to be processed.
 *  @param Contex the Event context.
 */
VOID
EventNotifyWriteIo (
  IN EFI_EVENT      Event,
  IN  VOID          *Context
  )
{
  TIMER_EVENT_CONTEXT                 *EventContext;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *RootBridgeIo;
  EFI_TEST_ASSERTION                  AssertionType;
  UINT64                              ReadValue;
  EFI_STATUS                          Status;

  EventContext = (TIMER_EVENT_CONTEXT *)Context;
  RootBridgeIo = EventContext->RootBridgeIo;

  Status = RootBridgeIo->Io.Write (
                              RootBridgeIo,
                              EventContext->RootBridgeIoWidth,
                              EventContext->Address,
                              1,
                              &EventContext->DestValue
                              );

  if (!EFI_ERROR(Status)) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  EventContext->StandardLib->RecordAssertion (
                               EventContext->StandardLib,
                               AssertionType,
                               gTestGenericFailureGuid,
                               L"EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.Io.Write - time out event to write target value",
                               L"%a:%d:status - %r,write value - %08Xh",
                               __FILE__,
                               (UINTN)__LINE__,
                               Status,
                               EventContext->DestValue
                               );

  ReadValue = 0x00;

  Status = RootBridgeIo->Io.Read (
                              RootBridgeIo,
                              EventContext->RootBridgeIoWidth,
                              EventContext->Address,
                              1,
                              &ReadValue
                              );

  if (!EFI_ERROR(Status)) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }

  EventContext->StandardLib->RecordAssertion (
                               EventContext->StandardLib,
                               AssertionType,
                               gTestGenericFailureGuid,
                               L"EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.Io.Read - return status should be EFI_SUCCESS",
                               L"%a:%d:status - %r,write value - %08Xh, ReadValue - %08Xh",
                               __FILE__,
                               (UINTN)__LINE__,
                               Status,
                               EventContext->DestValue,
                               ReadValue
                               );

  return;
}

/**
 *  check if the resource list is valid ACPI 2.0 QWORD address space descriptor.
 *  @param Resources the descriptor list pointer.
 *  @return TRUE the list is valid ASD.
 */
BOOLEAN
IsValidResourceDescrptor (
  VOID          *Resources
  )
{
  BOOLEAN                            IsValid;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  *Ptr;
  EFI_ACPI_END_TAG_DESCRIPTOR        *PtrEnd;
  UINTN                              MaxResourceNum;

  if (Resources == NULL) {
    return FALSE;
  }
  MaxResourceNum = 0;
  IsValid        = FALSE;

  Ptr = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)Resources;

  while (TRUE) {
    if (MaxResourceNum  == 100) {
      break;
    }
    switch (Ptr->Desc) {
      case ACPI_ADDRESS_SPACE_DESCRIPTOR:
        switch (Ptr->ResType) {
          case ACPI_ADDRESS_SPACE_TYPE_MEM :
          case ACPI_ADDRESS_SPACE_TYPE_IO  :
          case ACPI_ADDRESS_SPACE_TYPE_BUS :
            break;
          default:
            return IsValid;
            break;
        }
        Ptr = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) ((UINT8*)Ptr + sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR));
        break;
      case ACPI_END_TAG_DESCRIPTOR:
        PtrEnd = (EFI_ACPI_END_TAG_DESCRIPTOR *)Ptr;
        if (PtrEnd->Checksum == 0) {
          IsValid = TRUE;
        }
        return IsValid;
        break;
      default :
        return IsValid;

    }
    MaxResourceNum++;

  }

  return IsValid;
}

/**
 *  compare the two Acpi resource descriptor descript the same resources.
 *  @param Resource1 the first resource list pointer.
 *  @param Resource2 the second resource list pointer.
 *  @return TRUE the two descriptor descript the same resources.
 */
BOOLEAN
CompareAcpiResourceDescrptor (
  IN  VOID              *Resource1,
  IN  VOID              *Resource2
  )
{
  BOOLEAN                            IsEqual;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  *Ptr1;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  *Ptr2;
  UINTN                              MaxResourceNum;

  if (Resource1 == NULL || Resource2 == NULL) {
    return FALSE;
  }

  MaxResourceNum = 0;
  IsEqual        = FALSE;

  Ptr1 = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)Resource1;
  Ptr2 = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *)Resource2;

  while (TRUE) {

    if (MaxResourceNum  == 100) {
      break;
    }

    switch (Ptr1->Desc) {

      case ACPI_ADDRESS_SPACE_DESCRIPTOR:
        if (CompareMem (Ptr1, Ptr2, sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR)) != 0) {
          return FALSE;
        }
        Ptr1 = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) ((UINT8*)Ptr1 + sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR));
        Ptr2 = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) ((UINT8*)Ptr2 + sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR));
        break;

      case ACPI_END_TAG_DESCRIPTOR:
        if (CompareMem (Ptr1, Ptr2, sizeof (EFI_ACPI_END_TAG_DESCRIPTOR)) != 0) {
          return FALSE;
        } else {
          return TRUE;
        }
        break;

      default :
        return IsEqual;
    }

    MaxResourceNum++;
  }

 return IsEqual;

}

/**
 *  wait for user input and the timer if user didnot input any char during seconds
 *  period then the Buffer will point to NULL.
 *  @param Buffer the returned user input buffer.
 *  @param Seconds the timer wait time
 *  @return EFI_SUCCESS seccessfully done
 */
EFI_STATUS
GetUserInputOrTimeOut (
  OUT CHAR16                       **Buffer,
  IN  UINTN                        Seconds
  )
{
  EFI_EVENT             TimeOutEvent;
  EFI_EVENT             OneSecondTimer;
  EFI_EVENT             WaitList[2];
  BOOLEAN               Wait;
  EFI_STATUS            Status;
  CHAR16                *InputBuffer;
  UINTN                 InputLength;
  UINTN                 WaitIndex;
  EFI_INPUT_KEY         Key;
  CHAR16                PrintBuffer[2];

  InputLength = 0;
  InputBuffer = NULL;

  //
  //allocate string buffer.
  //
  Status = gtBS->AllocatePool (
                   EfiBootServicesData,
                   MAX_STRING_LEN * sizeof (CHAR16),
                   (VOID **)&InputBuffer
                   );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  Wait   = TRUE;

  //
  //create time event
  //

  Status = gtBS->CreateEvent (
                   EFI_EVENT_TIMER ,
                   0,
                   NULL,
                   NULL,
                   &TimeOutEvent
                   );

  if (EFI_ERROR(Status)) {
    gtBS->FreePool (InputBuffer);
    return Status;
  }

  Status = gtBS->CreateEvent (
                   EFI_EVENT_TIMER,
                   0,
                   NULL,
                   NULL,
                   &OneSecondTimer
                   );

  if (EFI_ERROR(Status)) {
    gtBS->CloseEvent (TimeOutEvent);
    gtBS->FreePool (InputBuffer);
    return Status;
  }

  //
  //set the Timer
  //

  Status = gtBS->SetTimer (
                   TimeOutEvent,
                   TimerRelative,
                   10000000 * Seconds
                   );

  if (EFI_ERROR(Status)) {
    gtBS->CloseEvent (TimeOutEvent);
    gtBS->CloseEvent (OneSecondTimer);
    gtBS->FreePool (InputBuffer);
    return Status;
  }

  //
  // Set up a wait list for a key and the timer
  //

  Wait = TRUE;

  WaitList[0] = gtST->ConIn->WaitForKey;
  WaitList[1] = OneSecondTimer;
  PrintBuffer[1] = '\0';
  //
  // Wait for timeout or string input.
  //
  while (Wait) {
    //
    // Set one second timer
    //

    Status = gtBS->SetTimer (
                     OneSecondTimer,
                     TimerRelative,
                     10000000
                     );

    if (EFI_ERROR(Status)) {
      gtBS->CloseEvent (TimeOutEvent);
      gtBS->CloseEvent (OneSecondTimer);
      gtBS->FreePool (InputBuffer);
      return Status;
    }

    Status = gtBS->WaitForEvent (
                     2,
                     WaitList,
                     &WaitIndex
                     );

    if (EFI_ERROR(Status)) {
      gtBS->CloseEvent (TimeOutEvent);
      gtBS->CloseEvent (OneSecondTimer);
      gtBS->FreePool (InputBuffer);
      return Status;
    }

    switch (WaitIndex) {
      case 1:
        //
        //the OneSecondTimer is triggered
        //
        break;

      case 0:
        //
        //key press event is triggered.
        //
        Key.ScanCode = SCAN_NULL;
        Status = gtST->ConIn->ReadKeyStroke (gtST->ConIn, &Key);
        if (!EFI_ERROR(Status)) {
          if (Key.ScanCode == SCAN_NULL) {
            if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
                Wait = FALSE;
                break;
            }
            if (Key.UnicodeChar >= 0x20 && Key.UnicodeChar <= 0x7f) {
              InputBuffer[InputLength] = Key.UnicodeChar;
              InputLength++;
              //
              //print this char in screen.
              //
              PrintBuffer[0] = Key.UnicodeChar;
              gtST->ConOut->OutputString (gtST->ConOut, PrintBuffer);

              if ((InputLength + 1) == MAX_STRING_LEN) {
                Wait = FALSE;
              }
            }
          }
        }
        break;

      default:
        break;
    }

    if (Wait) {
      //
      //check if the Timer has been signaled.
      //
      Status = gtBS->CheckEvent (TimeOutEvent);
      if (!EFI_ERROR(Status)) {
        Wait = FALSE;
      }
    }
  }

  //
  //close the timer.
  //

  gtBS->CloseEvent (TimeOutEvent);
  gtBS->CloseEvent (OneSecondTimer);

  if (InputLength > 0) {
    InputBuffer[InputLength] = '\0';
    *Buffer = InputBuffer;
    return EFI_SUCCESS;
  } else {
    gtBS->FreePool (InputBuffer);
    return EFI_TIMEOUT;
  }
}
/**
 *  caculate UINT64 from a string.
 *  @param Str the String to be parsed.
 *  @return Caculate UINT64 Value.
 */
UINT64
XToUint64 (
  CHAR16  *Str
  )
{
    UINT64      U64;
    CHAR16      Char;

    //
    //skip preceeding white space
    //
    while (*Str && *Str == ' ') {
        Str += 1;
    }

    //
    // skip preceeding zeros
    //
    while (*Str && *Str == '0') {
      Str += 1;
    }

    //
    // skip preceeding white space
    //
    if (*Str && (*Str == 'x' || *Str == 'X')) {
      Str += 1;
    }

    //
    // convert hex digits
    //
    U64 = 0;
    Char = *(Str++);
    while (Char) {
      if (Char >= 'a'  &&  Char <= 'f') {
        Char -= 'a' - 'A';
      }

      if ((Char >= '0'  &&  Char <= '9')  ||  (Char >= 'A'  &&  Char <= 'F')) {
        U64 = LShiftU64 (U64, 4)  |  (Char - (Char >= 'A' ? 'A'-10 : '0'));
      } else {
        break;
      }
      Char = *(Str++);
    }

    return U64;
}

/**
 *  caculate UINT64 from a string.
 *  @param Str the String to be parsed.
 *  @return Caculate UINT64 Value.
 */
UINT64
AToUint64 (
  CHAR16  *Str
  )
{
    UINT64      U64;
    CHAR16      Char;

    //
    //skip preceeding white space
    //
    while (*Str && *Str == ' ') {
        Str += 1;
    }

    //
    // convert digits
    //
    U64 = 0;
    Char = *(Str++);
    while (Char) {
      if (Char >= '0' && Char <= '9') {
        U64 = MultU64x32 (U64 , 10) + Char - '0';
      } else {
        break;
      }
      Char = *(Str++);
    }
    return U64;
}

 