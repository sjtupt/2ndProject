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
                                                                
  Copyright 2006, 2007, 2008, 2009, 2010 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:

  LoadFileBBTestMain.c

Abstract:

  Test Driver of LoadFile Protocol

--*/

#include "LoadFileBBTestMain.h"

EFI_EVENT              TimerEvent;

EFI_BB_TEST_PROTOCOL_FIELD gEfiLoafFileTestProtField = {
  LOAD_FILE_TEST_REVISION,
  LOAD_FILE_PROTOCOL_GUID,
  L"Load File Protocol Test",
  L"Test Load File Protocol"
};

EFI_GUID gSupportProtocolGuid1[] = {
  EFI_STANDARD_TEST_LIBRARY_GUID,
  EFI_NULL_GUID
};

EFI_BB_TEST_ENTRY_FIELD gLoadFileTestEntryField[] = {

#ifdef EFI_TEST_EXHAUSTIVE
  {
    LOAD_FILE_FUNCTION_TEST_GUID,
    L"LoadFile_Func_Manual",
    L"Manual Function Test of LoadFile",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_MANUAL,
    BBTestLoadFileFunctionTest
  },
#endif

  0
};


EFI_BB_TEST_PROTOCOL *gLoadFileTestProt;


EFI_DRIVER_ENTRY_POINT(InitializeLoadFileTest)
/**
 *  Creates/installs the BlackBox Interface and eminating Entry Point node list.
 *  @param  ImageHandle The test driver image handle
 *  @param  SystemTable Pointer to System Table
 *  @return EFI_SUCCESS Indicates the interface was installed
 *  @return EFI_OUT_OF_RESOURCES Indicates space for the new handle could not be allocated
 *  @return EFI_INVALID_PARAMETER: One of the parameters has an invalid value.
 */
EFI_STATUS
InitializeLoadFileTest (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS Status;

  EfiInitializeTestLib (ImageHandle, SystemTable);
  InitializeLib (ImageHandle, SystemTable);
  EfiInitializeDriverLib (ImageHandle, SystemTable);

  Status = gtBS->CreateEvent (EFI_EVENT_TIMER, 0, NULL, NULL, &TimerEvent);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return EfiInitAndInstallBBTestInterface (
           &ImageHandle,
           &gEfiLoafFileTestProtField,
           gLoadFileTestEntryField,
           UnloadLoadFileTest,
           &gLoadFileTestProt
           );
}

EFI_STATUS
UnloadLoadFileTest (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS Status;

  Status = gtBS->CloseEvent (TimerEvent);
  if (EFI_ERROR(Status)) {

  }
  return EfiUninstallAndFreeBBTestInterface (
           ImageHandle,
           gLoadFileTestProt
           );
}

/**
 *  Entrypoint for EFI_LOAD_FILE.LoadFile() Function and Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
EFI_STATUS
BBTestLoadFileFunctionTest (
  IN EFI_BB_TEST_PROTOCOL             *This,
  IN VOID                             *ClientInterface,
  IN EFI_TEST_LEVEL                   TestLevel,
  IN EFI_HANDLE                       SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib;
  EFI_STATUS                            Status;
  EFI_LOAD_FILE_PROTOCOL               *LoadFileInterface;
  EFI_TEST_ASSERTION                    AssertionType;
  UINTN                                 BufferSize;
  UINTN                                 LocalBufferSize;
  VOID                                 *Buffer;
  CHAR8                                 SampleBuffer[SAMPLE_BOOT_FILE_SIZE+1];

  EfiCopyMem (SampleBuffer, "This is a Sample Boot File.", SAMPLE_BOOT_FILE_SIZE);

  //
  // Get support library (Standard Lib, Profile Lib, Logging Lib)
  //
  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );
  if (EFI_ERROR(Status))
  {
    return Status;
  }

  //
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  LoadFileInterface = (EFI_LOAD_FILE_PROTOCOL *) ClientInterface;

  LocalBufferSize = (SAMPLE_BOOT_FILE_SIZE+1)*2;
  BufferSize = LocalBufferSize;

  //
  // Allocate Buffer for received file
  //
  Buffer = NULL;
  Status= gtBS->AllocatePool (EfiBootServicesData, BufferSize, &Buffer);
  if (EFI_ERROR(Status))
  {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_LOAD_FILE.LoadFile - Allocate Receive Buffer",
                   L"%a:%d:Status - %r, BufSize - %d",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status,
                   BufferSize
                   );
    return Status;
  }

  //
  // 4.1.2.3 Call LoadFile with Invalid Parameter
  //    a) Buffer = NULL
  //
  Status = LoadFileInterface->LoadFile (LoadFileInterface, NULL, TRUE, NULL, NULL);
  //
  // Check Return Code
  //
  if (Status == EFI_INVALID_PARAMETER){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gLoadFileBBTestMainAssertionGuid001,
                 L"EFI_LOAD_FILE.LoadFile - Invalid Parameter - Check return code",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  //    b) DevicePath is invalid device path
  //
  //DevicePath
  //Status = LoadFileInterface->LoadFile(LoadFileInterface, DevicePath, TRUE, &BufferSize, Buffer);

  //
  // 4.1.2.2 Call LoadFile while No Response
  //
  Print (L"\r\nShutdown DHCP Server and TFTP Server\n");
  LoadFileTestWaitForAnyInput ();

  LocalBufferSize = SAMPLE_BOOT_FILE_SIZE+1;
  BufferSize = LocalBufferSize;

  Status = LoadFileInterface->LoadFile (
                                LoadFileInterface,
                                NULL,
                                TRUE,
                                &BufferSize,
                                Buffer
                                );
  if (Status == EFI_NO_RESPONSE){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gLoadFileBBTestMainAssertionGuid002,
                 L"EFI_LOAD_FILE.LoadFile - No Response - Check return code",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // 4.2.1.5 Test EFI_BUFFER_TOO_SMALL
  //
  LocalBufferSize = SAMPLE_BOOT_FILE_SIZE+1;
  BufferSize = 16;

  // Call LoadFile() to get boot file
  Status = LoadFileInterface->LoadFile (LoadFileInterface, NULL, TRUE, &BufferSize, Buffer);
  if (Status == EFI_BUFFER_TOO_SMALL){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gLoadFileBBTestMainAssertionGuid003,
                 L"EFI_LOAD_FILE.LoadFile - Buffer Too Small - Check return code",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  // Log Returned File Size
  if (BufferSize == LocalBufferSize){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gLoadFileBBTestMainAssertionGuid004,
                 L"EFI_LOAD_FILE.LoadFile - Buffer Too Small - Check return buffer size",
                 L"%a:%d:Status - %r, RetLen - %d, ExpectLen - %d",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 BufferSize,
                 LocalBufferSize
                 );

  //
  // 3.1.2.1 Basic Function Test
  //

  //
  // Wait for user configure network environment
  // The sample boot file is located in the denpendency directory
  // The Content of sample boot file is
  //      'This is a Sample Boot File.'
  // So, the File Length is 27Bytes.
  //
  Print (L"\r\nConfigure and Start DHCP Server and TFTP Server");
  Print (L"\r\nCopy Sample Boot File (Sample.efi) in Dependency Directory to TFTP Server");
  LoadFileTestWaitForAnyInput ();

  LocalBufferSize = (SAMPLE_BOOT_FILE_SIZE+1)*2;
  BufferSize = LocalBufferSize;
  //
  // Call LoadFile() to get boot file
  //
  Status = LoadFileInterface->LoadFile (
                                LoadFileInterface,
                                NULL,
                                TRUE,
                                &BufferSize,
                                Buffer
                                );

  //
  // Check Return Code
  //
  if (EFI_ERROR(Status)){
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gLoadFileBBTestMainAssertionGuid005,
                 L"EFI_LOAD_FILE.LoadFile - Load File - Check return code",
                 L"%a:%d:Status - %r, LocalBufferSize - %d, BufferSize - %d",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 LocalBufferSize,
                 BufferSize
                 );

  //
  // Check Received File (Name and Content)
  //
  if ((SAMPLE_BOOT_FILE_SIZE+1 == BufferSize) &&
      (0 == EfiCompareMem (Buffer, SampleBuffer, SAMPLE_BOOT_FILE_SIZE))){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gLoadFileBBTestMainAssertionGuid006,
                 L"EFI_LOAD_FILE.LoadFile - Load File - Check rcvd file",
                 L"%a:%d:Status - %r, RetLen - %d, ExpectLen - %d",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 BufferSize,
                 SAMPLE_BOOT_FILE_SIZE+1
                 );

  gtBS->FreePool (Buffer);
  return Status;
}

/**
 *  Wait for any input from user
 */
VOID
LoadFileTestWaitForAnyInput ()
{
  UINTN                   NoWait = 1, WaitIndex;
  EFI_EVENT               WaitList[1];
  EFI_STATUS              Status;
  EFI_INPUT_KEY           Key;

  Print (L"Press any key to continue...");

  //
  // Set 1 second periodic timer
  //
  Status = gtBS->SetTimer (TimerEvent, TimerPeriodic, 10000000);

  //
  // Set up a wait list for a key and the timer
  //
  NoWait = 0;
  WaitList[NoWait++] = gtST->ConIn->WaitForKey;

  //
  // Wait for either
  //
  while (NoWait) {
    Status = gtBS->WaitForEvent (NoWait, WaitList, &WaitIndex);

    switch (WaitIndex) {
      case 0:
        //
        // Read the key
        //
        Status = gtST->ConIn->ReadKeyStroke (gtST->ConIn, &Key);
        if (EFI_SUCCESS == Status)
          NoWait = 0;
        break;

      default:
        break;
    }
  }
  Print (L"\r\n");

  //
  // Done, cancle periodic timer
  //
  Status = gtBS->SetTimer (TimerEvent, TimerCancel, 0);
}

