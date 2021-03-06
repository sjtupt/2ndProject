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

  ScsiIoBBTestMain.c

Abstract:

  Test Driver of Scsi Io Protocol

--*/

#include "ScsiIoBBTestMain.h"

UINT8 EnterEvent;

//
// Build Data structure here
//

EFI_BB_TEST_PROTOCOL_FIELD gBBTestProtocolField = {
  SCSI_IO_PROTOCOL_TEST_REVISION,
  IHV_SCSI_IO_PROTOCOL_GUID,
  L"Scsi Io Protocol Test",
  L"UEFI Scsi Io Protocol Test"
};


EFI_GUID gSupportProtocolGuid1[] = {
  EFI_STANDARD_TEST_LIBRARY_GUID,
  EFI_NULL_GUID
};

EFI_BB_TEST_ENTRY_FIELD gBBTestEntryField[] = {
  {
    SCSI_IO_PROTOCOL_GET_DEVICE_TYPE_FUNCTION_AUTO_GUID,
    L"GetDeviceType_Func",
    L"Invoke GetDeviceType() and verify interface correctness within test case",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    BBTestGetDeviceTypeFunctionAutoTest
  },

  {
    SCSI_IO_PROTOCOL_GET_DEVICE_LOCATION_FUNCTION_AUTO_GUID,
    L"GetDeviceLocation_Func",
    L"Invoke GetDeviceLocation() and verify interface correctness within test case",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    BBTestGetDeviceLocationFunctionAutoTest
  },

  {
    SCSI_IO_PROTOCOL_RESET_BUS_FUNCTION_AUTO_GUID,
    L"ResetBus_Func",
    L"Invoke ResetBus() and verify interface correctness within test case",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    BBTestResetBusFunctionAutoTest
  },

  {
    SCSI_IO_PROTOCOL_RESET_DEVICE_FUNCTION_AUTO_GUID,
    L"ResetDevice_Func",
    L"Invoke ResetDevice() and verify interface correctness within test case",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    BBTestResetDeviceFunctionAutoTest
  },
  
  {
    SCSI_IO_PROTOCOL_EXECUTE_SCSI_COMMAND_FUNCTION_AUTO_GUID,
    L"ExecuteScsiCommand_Func",
    L"Invoke ExecuteScsiCommand() and verify interface correctness within test case",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    BBTestExecuteScsiCommandFunctionAutoTest
  },
  
  {
    SCSI_IO_PROTOCOL_GET_DEVICE_TYPE_CONFORMANCE_AUTO_GUID,
    L"GetDeviceType_Conf",
    L"Test whether GetDeviceType() performs enough parameter checking",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    BBTestGetDeviceTypeConformanceAutoTest
  },

  {
    SCSI_IO_PROTOCOL_GET_DEVICE_LOCATION_CONFORMANCE_AUTO_GUID,
    L"GetDeviceLocation_Conf",
    L"Test whether GetDeviceLocation() performs enough parameter checking",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    BBTestGetDeviceLocationConformanceAutoTest
  },
  
  {
    SCSI_IO_PROTOCOL_EXECUTE_SCSI_COMMAND_CONFORMANCE_AUTO_GUID,
    L"ExecuteScsiCommand_Conf",
    L"Test whether ExecuteScsiCommand() performs enough parameter checking",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    BBTestExecuteScsiCommandConformanceAutoTest
  },
  
  0
};


EFI_BB_TEST_PROTOCOL *gBBTestProtocolInterface;

EFI_STATUS
BBTestScsiIoProtocolUnload (
  IN EFI_HANDLE       ImageHandle
  );

EFI_DRIVER_ENTRY_POINT(InitializeBBTestScsiIo)

/**
 *  Scsi Io Protocol Test Driver Entry point.
 *  @param ImageHandle the driver image handle.
 *  @param SystemTable the system table.
 *  @return EFI_SUCCESS the driver is loaded successfully.
 */
EFI_STATUS
InitializeBBTestScsiIo (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EfiInitializeTestLib (ImageHandle, SystemTable);
  InitializeLib (ImageHandle, SystemTable);


  return EfiInitAndInstallIHVBBTestInterface (
           &ImageHandle,
           &gBBTestProtocolField,
           gBBTestEntryField,
           BBTestScsiIoProtocolUnload,
           &gBBTestProtocolInterface
           );
}

/**
 *  The driver's Unload function.
 *  @param ImageHandle the test driver image handle.
 *  @return EFI_SUCCESS unload successfully.
 */
EFI_STATUS
BBTestScsiIoProtocolUnload (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS Status;

  Status = EfiUninstallAndFreeIHVBBTestInterface (
             ImageHandle,
             gBBTestProtocolInterface
             );

  return Status;
}

VOID
NotifyFunction (
  EFI_EVENT Event,
  VOID      *Context
  )
{
  EnterEvent = 1;
  return;
}
