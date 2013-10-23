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
    SimpleFileSystemBBTestStress_Manual.c

Abstract:
    for EFI Simple File System Protocol and EFI_FILE Protocol's Manual Extensive Test

--*/

#include "SimpleFileSystemBBTest.h"

#define MAX_DEVICE_PATH_AMOUNT    100
#define SECONDS_TO_100NS  10000000
#define WAIT_100_SECONDS   100
#define WAIT_50_SECONDS   50

VOID
ComposeDevicePathRepository (
  EFI_DEVICE_PATH_PROTOCOL              **WholeDevicePathArray,
  UINT32                                *DevicePathAmount
  );

BOOLEAN
AllFileSystemTested (
  EFI_DEVICE_PATH_PROTOCOL    **WholeDevicePathArray,
  UINT32                      DevicePathAmount,
  EFI_DEVICE_PATH_PROTOCOL    **TestedDevicePathArray
  );

BOOLEAN
IsDevicePathInRepository (
  EFI_DEVICE_PATH_PROTOCOL    *DevicePath,
  EFI_DEVICE_PATH_PROTOCOL    **DevicePathArray,
  UINT32                      DevicePathAmount
  );

BOOLEAN
IsRemovableDevice (
  EFI_HANDLE    DeviceHandle
  );

EFI_STATUS
CreateSampleDirFileTree (
  EFI_FILE                              *Root,
  EFI_FILE                              **DirHandle,
  EFI_FILE                              **FileHandle
  );

VOID
CloseSampleDirFileTree (
  EFI_FILE                              *Root,
  EFI_FILE                              **DirHandle,
  EFI_FILE                              **FileHandle
  );

VOID
DeleteSampleDirFileTree (
  EFI_FILE                              *Root,
  EFI_FILE                              **DirHandle,
  EFI_FILE                              **FileHandle
  );

VOID
PrintStartTestMessage (
  CHAR16                      *PrintMessage,
  EFI_EVENT                   TimerEvent,
  UINT64                      TriggerTimeInSeconds,
  BOOLEAN                     *PressYes
  );

VOID
PrintDevicePathAndAskForInput (
  CHAR16                      *PrintMessage,
  EFI_DEVICE_PATH_PROTOCOL    *DevicePath,      OPTIONAL
  EFI_EVENT                   TimerEvent,
  UINT64                      TriggerTimeInSeconds,
  BOOLEAN                     *PrintYes
  );

VOID
ForceFileSystemReinstall ();

VOID
EFIAPI
TestNotifyFunction0 (
  EFI_EVENT Event,
  VOID      *Context
  );

BOOLEAN
IsMediaReadOnly (
  EFI_HANDLE    DeviceHandle
  );

/**
 *  @brief Entrypoint for Extensive Test 1.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL
 *  @param ClientInterface A pointer to the interface array under test
 *  @param TestLevel Test "thoroughness" control
 *  @param SupportHandle A handle containing protocols required
 *  @return EFI_SUCCESS
 *  @return EFI_NOT_FOUND
 */
//
// TDS 5.3.1
//
EFI_STATUS
BBTestSimpleFileSytemExtensiveTest1 (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
)
/*
  step 1: Find all simple file system handles, record their device paths
          in a repository. When all these device paths have been tested,
          this test case is over.

  step 2: Pick a file system, check its device path with the tested device path
          repository. If it is in the tested repository, pick another file system.

  step 3: Create 2 directories and 5 files. The created directory structure shows
          like below:

                        Root
                        ======
                        | | |
                       /  |  \
                      /   |   \
                     /    |    \
                  ===    ===    ===
                   D1    D2      F1
                  ===    ===    ===
                  / \    / \
                 =   =  =   =
                 F2  F3 F4  F5

  step 4: Open these files and directories.

  step 5: Print the device path of the file system to the tester, and ask
          the tester to CHANGE the corresponding medium and input a key.
          A timer is used in case when there is no tester performing key input.
          If so, timer will expire and the test case will deem a No
          answer is input and will skip the following operation and repeat from
          step 2.

  step 6: Call OpenVolume() with the original file system protocol instance.

  step 7: Call Open() with those original created/opened file handles.

  step 8: Call Read()/Write()/SetPosition()/GetPosition()/SetInfo()/GetInfo()/Delete()
          with those original opened file handles.

  step 9: Call Close() to close all original opened file handles.

  step 10: Add the device path to the tested device path repository and repeat
           step 2.
*/
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_STATUS                            Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *SimpleFileSystem;
  EFI_HANDLE                            *HandleBuffer;
  UINTN                                 NoHandles;
  UINTN                                 Index;
  EFI_DEVICE_PATH_PROTOCOL              *WholeDevicePathArray[MAX_DEVICE_PATH_AMOUNT];
  EFI_DEVICE_PATH_PROTOCOL              *TestedDevicePathArray[MAX_DEVICE_PATH_AMOUNT];
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL              *NewDevicePath;
  UINT32                                DevicePathAmount;
  UINT32                                TestedDevicePathAmount;
  CHAR16                                *PrintMessage;
  CHAR16                                NewFileName[50];
  CHAR16                                FileName[5][100];
  EFI_FILE                              *Root;
  EFI_FILE                              *AnotherRoot;
  EFI_FILE                              *FileHandle[5];
  EFI_FILE                              *DirHandle[2];
  EFI_FILE                              *OpenFileHandle[5];
  EFI_FILE                              *OpenDirHandle[2];
  EFI_FILE                              *NewFileHandle;
  UINTN                                 TempSize;
  UINT8                                 Temp;
  UINT64                                Position;
  UINTN                                 InfoBufferSize;
  EFI_FILE_INFO                         *FileInfo;
  EFI_EVENT                             TimerEvent;
  BOOLEAN                               MediaChanged;
  UINTN                                 FileIndex;
  UINTN                                 Count;

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // init
  //
  NoHandles = 0;
  HandleBuffer = NULL;

  gtBS->SetMem (WholeDevicePathArray, MAX_DEVICE_PATH_AMOUNT * sizeof (EFI_DEVICE_PATH_PROTOCOL*), 0);
  gtBS->SetMem (TestedDevicePathArray, MAX_DEVICE_PATH_AMOUNT * sizeof (EFI_DEVICE_PATH_PROTOCOL*), 0);

  gtBS->SetMem (DirHandle, 2 * sizeof (EFI_FILE*), 0);
  gtBS->SetMem (FileHandle, 5 * sizeof (EFI_FILE*), 0);
  gtBS->SetMem (OpenDirHandle, 2 * sizeof (EFI_FILE*), 0);
  gtBS->SetMem (OpenFileHandle, 5 * sizeof (EFI_FILE*), 0);

  Root = NULL;
  NewFileHandle = NULL;
  FileInfo = NULL;

  TestedDevicePathAmount = 0;
  DevicePathAmount = 0;

  EfiStrCpy (FileName[0], L"File1");
  EfiStrCpy (FileName[1], L"File2");
  EfiStrCpy (FileName[2], L"File3");
  EfiStrCpy (FileName[3], L"File4");
  EfiStrCpy (FileName[4], L"File5");

  EfiStrCpy (NewFileName, L"newfile");

  //
  // create a timer event
  //
  TimerEvent = NULL;
  Status = gtBS->CreateEvent (EFI_EVENT_TIMER, 0, NULL, NULL, &TimerEvent);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Create timer fail",
                   L"%a,%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  //
  // Compose device path repository
  //
  ComposeDevicePathRepository (WholeDevicePathArray, &DevicePathAmount);
  if (DevicePathAmount == 0) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"No device path found",
                   L"%a,%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    goto Done;
  }

  Count = 0;
  while (!AllFileSystemTested (WholeDevicePathArray, DevicePathAmount, TestedDevicePathArray)) {

    Count++;
    //
    // avoid dead loop
    //
    if (Count > MAX_DEVICE_PATH_AMOUNT) {
      break;
    }

    Status = gtBS->LocateHandleBuffer (
                     ByProtocol,
                     &gEfiSimpleFileSystemProtocolGuid,
                     NULL,
                     &NoHandles,
                     &HandleBuffer
                     );
    if (EFI_ERROR (Status)) {
      break;
    }

    for (Index = 0; Index < NoHandles; Index++) {

      Status = gtBS->HandleProtocol (
                       HandleBuffer[Index],
                       &gEfiDevicePathProtocolGuid,
                       &DevicePath
                       );
      if (EFI_ERROR (Status)) {
        continue;
      }

      //
      // do not perform tests on those file systems added after the test case
      // is run.
      //
      if (!IsDevicePathInRepository (DevicePath, WholeDevicePathArray, DevicePathAmount)) {
        continue;
      }

      //
      // if already tested, then skip for the next
      //
      if (IsDevicePathInRepository (DevicePath, TestedDevicePathArray, DevicePathAmount)) {
        continue;
      } else {

        //
        // found a never tested one. add it into Tested Device Path repository.
        //

        NewDevicePath = DuplicateDevicePath (DevicePath);
        if (NewDevicePath == NULL) {
          continue;
        }
        TestedDevicePathArray[TestedDevicePathAmount] = NewDevicePath;
        TestedDevicePathAmount++;

        Status = gtBS->HandleProtocol (
                         HandleBuffer[Index],
                         &gEfiSimpleFileSystemProtocolGuid,
                         &SimpleFileSystem
                         );
        if (EFI_ERROR(Status)) {
          continue;
        }

        //
        // check whether the device is a removable one
        // if it is not, just add its device path into
        // the TestedDevicePathArray
        //
        if (!IsRemovableDevice (HandleBuffer[Index])) {
          continue;
        }

        //
        // perform the test
        //
        Status = SimpleFileSystem->OpenVolume (SimpleFileSystem, &Root);
        if (EFI_ERROR (Status)) {
          StandardLib->RecordAssertion (
                         StandardLib,
                         EFI_TEST_ASSERTION_FAILED,
                         gTestGenericFailureGuid,
                         L"OpenVolume fail",
                         L"%a,%d: Status - %r",
                         __FILE__,
                         (UINTN)__LINE__,
                         Status
                         );
          continue;
        }

        //
        // create sample directory-file tree
        //
        Status = CreateSampleDirFileTree (Root, DirHandle, FileHandle);
        if (EFI_ERROR (Status)) {
          continue;
        }

        //
        // print device path message and ask for input by tester
        //
        PrintMessage = L"\nITS MEDIA SHOULD BE CHANGED\n";
        PrintDevicePathAndAskForInput (
          PrintMessage,
          DevicePath,
          TimerEvent,
          WAIT_100_SECONDS,
          &MediaChanged
          );

        if (!MediaChanged) {
          CloseSampleDirFileTree (Root, DirHandle, FileHandle);
          continue;
        }

        //
        // OpenVolume()
        //
        Status = SimpleFileSystem->OpenVolume (SimpleFileSystem, &AnotherRoot);
        if ((EFI_SUCCESS == Status) || (EFI_UNSUPPORTED == Status)) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid001,
                       L"Extensive Test 1 - OpenVolume()",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // Create new file under Root
        //
        Status = Root->Open (
                         Root,
                         &NewFileHandle,
                         NewFileName,
                         CREATE_FILE_MODE,
                         0
                         );
        if (EFI_MEDIA_CHANGED == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid002,
                       L"Extensive Test 1 - Open(), Create new file",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        if (NewFileHandle != NULL) {
          NewFileHandle->Delete (NewFileHandle);
          NewFileHandle = NULL;
        }
        //
        // Create new file under D1
        //
        Status = DirHandle[0]->Open (
                                 DirHandle[0],
                                 &NewFileHandle,
                                 NewFileName,
                                 CREATE_FILE_MODE,
                                 0
                                 );
        if (EFI_MEDIA_CHANGED == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid003,
                       L"Extensive Test 1 - Open(), Create new file",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        if (NewFileHandle != NULL) {
          NewFileHandle->Delete (NewFileHandle);
          NewFileHandle = NULL;
        }

        //
        // Open F1 under Root
        //
        Status = Root->Open (
                         Root,
                         &FileHandle[0],
                         FileName[0],
                         EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                         0
                         );
        if (EFI_MEDIA_CHANGED == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid004,
                       L"Extensive Test 1 - Open(), Open existing file",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // Open F4 under D2
        //
        Status = DirHandle[1]->Open (
                                 DirHandle[1],
                                 &FileHandle[3],
                                 FileName[3],
                                 EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                                 0
                                 );
        if (EFI_MEDIA_CHANGED == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid005,
                       L"Extensive Test 1 - Open(), Open existing file",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // Read from F1
        //
        TempSize = 1;
        Status = FileHandle[0]->Read (FileHandle[0], &TempSize, &Temp);
        AssertionType = EFI_TEST_ASSERTION_PASSED;
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid006,
                       L"Extensive Test 1 - Read(), Read file after media changed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // Write to F2
        //
        TempSize = 1;
        Status = FileHandle[1]->Write (FileHandle[1], &TempSize, &Temp);
        AssertionType = EFI_TEST_ASSERTION_PASSED;
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid007,
                       L"Extensive Test 1 - Write(), Write file after media changed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // SetPosition to F3
        //
        Status = FileHandle[2]->SetPosition (FileHandle[2], 0);
        AssertionType = EFI_TEST_ASSERTION_PASSED;
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid008,
                       L"Extensive Test 1 - SetPosition(), SetPosition after media changed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // GetPosition from F4
        //
        Status = FileHandle[3]->GetPosition (FileHandle[3], &Position);
        AssertionType = EFI_TEST_ASSERTION_PASSED;
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid009,
                       L"Extensive Test 1 - GetPosition(), GetPosition after media changed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // GetInfo from F5
        //
        Status = InternalGetInfo (FileHandle[4], &FileInfo, &InfoBufferSize, &gEfiFileInfoGuid);
        AssertionType = EFI_TEST_ASSERTION_PASSED;
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid010,
                       L"Extensive Test 1 - GetInfo(), GetInfo after media changed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // SetInfo for F5
        //
        Status = FileHandle[4]->SetInfo (FileHandle[4], &gEfiFileInfoGuid, InfoBufferSize, FileInfo);
        AssertionType = EFI_TEST_ASSERTION_PASSED;
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid011,
                       L"Extensive Test 1 - SetInfo(), SetInfo after media changed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );
        if (FileInfo != NULL) {
          gtBS->FreePool (FileInfo);
          FileInfo = NULL;
        }

  //
        // Delete F1
        //
        Status = FileHandle[0]->Delete (FileHandle[0]);
        FileHandle[0] = NULL;
        AssertionType = EFI_TEST_ASSERTION_PASSED;
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid011,
                       L"Extensive Test 1 - Delete(), Delete file after media changed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // Close All
        //
        for (FileIndex = 0; FileIndex < 2; FileIndex++) {
          Status = DirHandle[FileIndex]->Close (DirHandle[FileIndex]);
          if (EFI_SUCCESS == Status) {
            AssertionType = EFI_TEST_ASSERTION_PASSED;
          } else {
            AssertionType = EFI_TEST_ASSERTION_FAILED;
          }
          StandardLib->RecordAssertion (
                         StandardLib,
                         AssertionType,
                         gSimpleFileSystemExtensiveTest_ManualAssertionGuid012,
                         L"Extensive Test 1 - Close()",
                         L"%a:%d: Status - %r, DirIndex - %d",
                         __FILE__,
                         (UINTN)__LINE__,
                         Status,
                         FileIndex
                         );
          DirHandle[FileIndex] = NULL;
        }

        for (FileIndex = 1; FileIndex < 5; FileIndex++) {
          Status = FileHandle[FileIndex]->Close (FileHandle[FileIndex]);
          if (EFI_SUCCESS == Status) {
            AssertionType = EFI_TEST_ASSERTION_PASSED;
          } else {
            AssertionType = EFI_TEST_ASSERTION_FAILED;
          }
          StandardLib->RecordAssertion (
                         StandardLib,
                         AssertionType,
                         gSimpleFileSystemExtensiveTest_ManualAssertionGuid013,
                         L"Extensive Test 1 - Close()",
                         L"%a:%d: Status - %r, FileIndex - %d",
                         __FILE__,
                         (UINTN)__LINE__,
                         Status,
                         FileIndex
                         );
          FileHandle[FileIndex] = NULL;
        }
      }
    }

    if (HandleBuffer != NULL) {
      gtBS->FreePool (HandleBuffer);
      HandleBuffer = NULL;
    }
  }

  //
  // clean all sample file trees
  //
  Status = gtBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiSimpleFileSystemProtocolGuid,
                   NULL,
                   &NoHandles,
                   &HandleBuffer
                   );
  if (!EFI_ERROR (Status)) {

    for (Index = 0; Index < NoHandles; Index++) {

      gtBS->SetMem (OpenDirHandle, 2 * sizeof (EFI_FILE*), 0);
      gtBS->SetMem (OpenFileHandle, 5 * sizeof (EFI_FILE*), 0);

      Status = gtBS->HandleProtocol (
                       HandleBuffer[Index],
                       &gEfiSimpleFileSystemProtocolGuid,
                       &SimpleFileSystem
                       );
      if (EFI_ERROR (Status)) {
        continue;
      }

      Status = SimpleFileSystem->OpenVolume (SimpleFileSystem, &Root);
      if (EFI_ERROR (Status)) {
        continue;
      }

      DeleteSampleDirFileTree (Root, OpenDirHandle, OpenFileHandle);
    }
  }

Done:

  if (HandleBuffer != NULL) {
    gtBS->FreePool (HandleBuffer);
  }

  for (Index = 0; Index < DevicePathAmount; Index++) {
    gtBS->FreePool (WholeDevicePathArray[Index]);
  }

  for (Index = 0; Index < TestedDevicePathAmount; Index++) {
    gtBS->FreePool (TestedDevicePathArray[Index]);
  }

  if (TimerEvent != NULL) {
    gtBS->SetTimer (TimerEvent, TimerCancel, 0);
    gtBS->CloseEvent (TimerEvent);
  }

  return EFI_SUCCESS;
}

/**
 *  @brief Entrypoint for Extensive Test 2.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL
 *  @param ClientInterface A pointer to the interface array under test
 *  @param TestLevel Test "thoroughness" control
 *  @param SupportHandle A handle containing protocols required
 *  @return EFI_SUCCESS
 *  @return EFI_NOT_FOUND
 */
//
// TDS 5.3.2
//
EFI_STATUS
BBTestSimpleFileSytemExtensiveTest2 (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
)
/*
  step 1: Find all simple file system handles, record their device paths
          in a repository. When all these device paths have been tested,
          this test case is over.

  step 2: Pick a file system, check its device path with the tested device path
          repository. If it is in the tested repository, pick another file system.

  step 3: Create 2 directories and 5 files. The created directory structure shows
          like below:

                        Root
                        ======
                        | | |
                       /  |  \
                      /   |   \
                     /    |    \
                  ===    ===    ===
                   D1    D2      F1
                  ===    ===    ===
                  / \    / \
                 =   =  =   =
                 F2  F3 F4  F5

  step 4: Open these files and directories.

  step 5: Print the device path of the file system to the tester, and ask
          the tester to REMOVE the corresponding medium and input a key.
          A timer is used in case when there is no tester performing key input.
          If so, timer will expire and the test case will deem a No
          answer is input and will skip the following operation and repeat from
          step 2.

  step 6: Call Open() with those original created/opened file handles.

  step 7: Call Read()/Write()/Flush() on those original opened file handles.

  step 8: Call SetIno()/GetInfo() on those original opened file handles.

  step 9: Call Close() to close all original opened file handles.

  step 10: Add the device path to the tested device path repository and repeat
           step 2.
*/
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_STATUS                            Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *SimpleFileSystem;
  EFI_HANDLE                            *HandleBuffer;
  UINTN                                 NoHandles;
  UINTN                                 Index;
  EFI_DEVICE_PATH_PROTOCOL              *WholeDevicePathArray[MAX_DEVICE_PATH_AMOUNT];
  EFI_DEVICE_PATH_PROTOCOL              *TestedDevicePathArray[MAX_DEVICE_PATH_AMOUNT];
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL              *NewDevicePath;
  UINT32                                DevicePathAmount;
  UINT32                                TestedDevicePathAmount;
  CHAR16                                *PrintMessage;
  CHAR16                                FileName[5][100];
  EFI_FILE                              *Root;
  EFI_FILE                              *FileHandle[5];
  EFI_FILE                              *DirHandle[2];
  EFI_FILE                              *OpenFileHandle[5];
  EFI_FILE                              *OpenDirHandle[2];
  CHAR16                                NewFileName[20];
  EFI_FILE                              *NewFileHandle;
  UINTN                                 TempSize;
  UINT8                                 Temp;
  UINTN                                 InfoBufferSize;
  EFI_FILE_INFO                         *FileInfo;
  EFI_EVENT                             TimerEvent;
  BOOLEAN                               MediaRemoved;
  UINTN                                 FileIndex;
  UINTN                                 Count;

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // init
  //
  NoHandles = 0;
  HandleBuffer = NULL;

  gtBS->SetMem (WholeDevicePathArray, MAX_DEVICE_PATH_AMOUNT * sizeof (EFI_DEVICE_PATH_PROTOCOL*), 0);
  gtBS->SetMem (TestedDevicePathArray, MAX_DEVICE_PATH_AMOUNT * sizeof (EFI_DEVICE_PATH_PROTOCOL*), 0);

  gtBS->SetMem (DirHandle, 2 * sizeof (EFI_FILE*), 0);
  gtBS->SetMem (FileHandle, 5 * sizeof (EFI_FILE*), 0);
  gtBS->SetMem (OpenDirHandle, 2 * sizeof (EFI_FILE*), 0);
  gtBS->SetMem (OpenFileHandle, 5 * sizeof (EFI_FILE*), 0);

  Root = NULL;
  NewFileHandle = NULL;
  FileInfo = NULL;

  DevicePathAmount = 0;
  TestedDevicePathAmount = 0;

  EfiStrCpy (FileName[0], L"File1");
  EfiStrCpy (FileName[1], L"File2");
  EfiStrCpy (FileName[2], L"File3");
  EfiStrCpy (FileName[3], L"File4");
  EfiStrCpy (FileName[4], L"File5");

  EfiStrCpy (NewFileName, L"newfile");

  //
  // create a timer event
  //
  TimerEvent = NULL;
  Status = gtBS->CreateEvent (EFI_EVENT_TIMER, 0, NULL, NULL, &TimerEvent);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Create timer fail",
                   L"%a,%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  //
  // Compose device path repository
  //
  ComposeDevicePathRepository (WholeDevicePathArray, &DevicePathAmount);
  if (DevicePathAmount == 0) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"No device path found",
                   L"%a,%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    goto Done;
  }

  Count = 0;
  while (!AllFileSystemTested (WholeDevicePathArray, DevicePathAmount, TestedDevicePathArray)) {

    Count++;
    //
    // avoid dead loop
    //
    if (Count > MAX_DEVICE_PATH_AMOUNT) {
      break;
    }

    Status = gtBS->LocateHandleBuffer (
                     ByProtocol,
                     &gEfiSimpleFileSystemProtocolGuid,
                     NULL,
                     &NoHandles,
                     &HandleBuffer
                     );
    if (EFI_ERROR (Status)) {
      break;
    }

    for (Index = 0; Index < NoHandles; Index++) {

      Status = gtBS->HandleProtocol (
                       HandleBuffer[Index],
                       &gEfiDevicePathProtocolGuid,
                       &DevicePath
                       );
      if (EFI_ERROR (Status)) {
        continue;
      }

      //
      // do not perform tests on those file systems added after the test case
      // is run.
      //
      if (!IsDevicePathInRepository (DevicePath, WholeDevicePathArray, DevicePathAmount)) {
        continue;
      }

      //
      // if already tested, then skip for the next
      //
      if (IsDevicePathInRepository (DevicePath, TestedDevicePathArray, DevicePathAmount)) {
        continue;
      } else {

        //
        // found a never tested one. add it into Tested Device Path repository.
        //

        NewDevicePath = DuplicateDevicePath (DevicePath);
        if (NewDevicePath == NULL) {
          continue;
        }
        TestedDevicePathArray[TestedDevicePathAmount] = NewDevicePath;
        TestedDevicePathAmount++;

        Status = gtBS->HandleProtocol (
                         HandleBuffer[Index],
                         &gEfiSimpleFileSystemProtocolGuid,
                         &SimpleFileSystem
                         );
        if (EFI_ERROR(Status)) {
          continue;
        }

        //
        // check whether the device is a removable one
        // if it is not, just add its device path into
        // the TestedDevicePathArray
        //
        if (!IsRemovableDevice (HandleBuffer[Index])) {
          continue;
        }

        //
        // perform the test
        //
        Status = SimpleFileSystem->OpenVolume (SimpleFileSystem, &Root);
        if (EFI_ERROR (Status)) {
          StandardLib->RecordAssertion (
                         StandardLib,
                         EFI_TEST_ASSERTION_FAILED,
                         gTestGenericFailureGuid,
                         L"OpenVolume fail",
                         L"%a,%d: Status - %r",
                         __FILE__,
                         (UINTN)__LINE__,
                         Status
                         );
          continue;
        }

        //
        // create sample directory-file tree
        //
        Status = CreateSampleDirFileTree (Root, DirHandle, FileHandle);
        if (EFI_ERROR (Status)) {
          continue;
        }

        //
        // print device path message and ask for input by tester
        //
        PrintMessage = L"\nITS MEDIA SHOULD BE REMOVED\n";
        PrintDevicePathAndAskForInput (
          PrintMessage,
          DevicePath,
          TimerEvent,
          WAIT_100_SECONDS,
          &MediaRemoved
          );

        if (!MediaRemoved) {
          CloseSampleDirFileTree (Root, DirHandle, FileHandle);
          continue;
        }

        //
        // Force file system be reinstalled after media is changed.
        //
        ForceFileSystemReinstall ();

        //
        // Create new file under Root
        //
        Status = Root->Open (
                         Root,
                         &NewFileHandle,
                         NewFileName,
                         CREATE_FILE_MODE,
                         0
                         );
        if (EFI_NO_MEDIA == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid014,
                       L"Extensive Test 2 - Open(), Create new file",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        if (NewFileHandle != NULL) {
          NewFileHandle->Delete (NewFileHandle);
          NewFileHandle = NULL;
        }
        //
        // Create new file under D1
        //
        Status = DirHandle[0]->Open (
                                 DirHandle[0],
                                 &NewFileHandle,
                                 NewFileName,
                                 CREATE_FILE_MODE,
                                 0
                                 );
        if (EFI_NO_MEDIA == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid015,
                       L"Extensive Test 2 - Open(), Create new file",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        if (NewFileHandle != NULL) {
          NewFileHandle->Delete (NewFileHandle);
          NewFileHandle = NULL;
        }

        //
        // Open F1 under Root
        //
        Status = Root->Open (
                         Root,
                         &FileHandle[0],
                         FileName[0],
                         EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                         0
                         );
        if (EFI_NO_MEDIA == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid016,
                       L"Extensive Test 2 - Open(), Open existing file",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // Open F4 under D2
        //
        Status = DirHandle[1]->Open (
                                 DirHandle[1],
                                 &FileHandle[3],
                                 FileName[3],
                                 EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                                 0
                                 );
        if (EFI_NO_MEDIA == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid017,
                       L"Extensive Test 2 - Open(), Open existing file",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // Read from F1
        //
        TempSize = 1;
        Status = FileHandle[0]->Read (FileHandle[0], &TempSize, &Temp);
        if (EFI_NO_MEDIA == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid018,
                       L"Extensive Test 2 - Read(), Read after media removed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // Write to F2
        //
        TempSize = 1;
        Status = FileHandle[1]->Write (FileHandle[1], &TempSize, &Temp);
        if (EFI_NO_MEDIA == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid019,
                       L"Extensive Test 2 - Write(), Write after media removed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        // Flush to F3
        //
        Status = FileHandle[2]->Flush (FileHandle[2]);
        if (EFI_NO_MEDIA == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid020,
                       L"Extensive Test 2 - Flush(), Flush after media removed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // GetInfo from F4
        //
        Status = InternalGetInfo (FileHandle[3], &FileInfo, &InfoBufferSize, &gEfiFileInfoGuid);
        if (EFI_NO_MEDIA == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid021,
                       L"Extensive Test 2 - GetInfo(), GetInfo after media removed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        //
        // SetInfo for F5
        //
        Status = FileHandle[4]->SetInfo (FileHandle[4], &gEfiFileInfoGuid, InfoBufferSize, FileInfo);
        if (EFI_NO_MEDIA == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid022,
                       L"Extensive Test 2 - SetInfo(), SetInfo after media removed",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        if (FileInfo != NULL) {
          gtBS->FreePool (FileInfo);
          FileInfo = NULL;
        }

        //
        // Close All
        //
        for (FileIndex = 0; FileIndex < 2; FileIndex++) {
          Status = DirHandle[FileIndex]->Close (DirHandle[FileIndex]);
          if (EFI_SUCCESS == Status) {
            AssertionType = EFI_TEST_ASSERTION_PASSED;
          } else {
            AssertionType = EFI_TEST_ASSERTION_FAILED;
          }
          StandardLib->RecordAssertion (
                         StandardLib,
                         AssertionType,
                         gSimpleFileSystemExtensiveTest_ManualAssertionGuid023,
                         L"Extensive Test 2 - Close()",
                         L"%a:%d: Status - %r, DirIndex - %d",
                         __FILE__,
                         (UINTN)__LINE__,
                         Status,
                         FileIndex
                         );
          DirHandle[FileIndex] = NULL;
        }

        for (FileIndex = 0; FileIndex < 5; FileIndex++) {
          Status = FileHandle[FileIndex]->Close (FileHandle[FileIndex]);
          if (EFI_SUCCESS == Status) {
            AssertionType = EFI_TEST_ASSERTION_PASSED;
          } else {
            AssertionType = EFI_TEST_ASSERTION_FAILED;
          }
          StandardLib->RecordAssertion (
                         StandardLib,
                         AssertionType,
                         gSimpleFileSystemExtensiveTest_ManualAssertionGuid024,
                         L"Extensive Test 2 - Close()",
                         L"%a:%d: Status - %r, FileIndex - %d",
                         __FILE__,
                         (UINTN)__LINE__,
                         Status,
                         FileIndex
                         );
          FileHandle[FileIndex] = NULL;
        }
      }
    }

    if (HandleBuffer != NULL) {
      gtBS->FreePool (HandleBuffer);
      HandleBuffer = NULL;
    }
  }

  //
  // delete all sample file trees
  //
  Status = gtBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiSimpleFileSystemProtocolGuid,
                   NULL,
                   &NoHandles,
                   &HandleBuffer
                   );
  if (!EFI_ERROR (Status)) {

    for (Index = 0; Index < NoHandles; Index++) {

      gtBS->SetMem (OpenDirHandle, 2 * sizeof (EFI_FILE*), 0);
      gtBS->SetMem (OpenFileHandle, 5 * sizeof (EFI_FILE*), 0);

      Status = gtBS->HandleProtocol (
                       HandleBuffer[Index],
                       &gEfiSimpleFileSystemProtocolGuid,
                       &SimpleFileSystem
                       );
      if (EFI_ERROR (Status)) {
        continue;
      }

      Status = SimpleFileSystem->OpenVolume (SimpleFileSystem, &Root);
      if (EFI_ERROR (Status)) {
        continue;
      }

      DeleteSampleDirFileTree (Root, OpenDirHandle, OpenFileHandle);
    }
  }

Done:
  if (HandleBuffer != NULL) {
    gtBS->FreePool (HandleBuffer);
  }

  for (Index = 0; Index < DevicePathAmount; Index++) {
    gtBS->FreePool (WholeDevicePathArray[Index]);
  }

  for (Index = 0; Index < TestedDevicePathAmount; Index++) {
    gtBS->FreePool (TestedDevicePathArray[Index]);
  }

  if (TimerEvent != NULL) {
    gtBS->SetTimer (TimerEvent, TimerCancel, 0);
    gtBS->CloseEvent (TimerEvent);
  }

  return EFI_SUCCESS;
}

/**
 *  @brief Entrypoint for Extensive Test 3.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL
 *  @param ClientInterface A pointer to the interface array under test
 *  @param TestLevel Test "thoroughness" control
 *  @param SupportHandle A handle containing protocols required
 *  @return EFI_SUCCESS
 *  @return EFI_NOT_FOUND
 */
//
// TDS 5.3.3
//
EFI_STATUS
BBTestSimpleFileSytemExtensiveTest3 (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
)
/*
  The write protected media must contain a file named
  BBTestSimpleFileSytemExtensiveTest3_ExistingFile.
*/
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *SimpleFileSystem;
  EFI_FILE                              *Root;
  EFI_STATUS                            Status;
  EFI_HANDLE                            *HandleBuffer;
  UINTN                                 NoHandles;
  UINTN                                 Index;
  EFI_EVENT                             TimerEvent;
  BOOLEAN                               MediaPlaced;
  EFI_FILE                              *FileHandle;
  CHAR16                                *PrintMessage;
  CHAR16                                NewFileName[100];
  CHAR16                                FileName[100];

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Init
  //
  HandleBuffer = NULL;
  TimerEvent = NULL;

  EfiStrCpy (NewFileName, L"BBTestSimpleFileSytemExtensiveTest3_NewFile");
  EfiStrCpy (FileName, L"BBTestSimpleFileSytemExtensiveTest3_ExistingFile");

  //
  // create a timer event
  //
  TimerEvent = NULL;
  Status = gtBS->CreateEvent (EFI_EVENT_TIMER, 0, NULL, NULL, &TimerEvent);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Create timer fail",
                   L"%a,%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  PrintMessage = L"\nPLEASE REPLACE/INSERT A MEDIA WITH WRITE-PROTECTED ONE\n";
  PrintDevicePathAndAskForInput (
    PrintMessage,
    NULL,
    TimerEvent,
    WAIT_100_SECONDS,
    &MediaPlaced
    );

  if (!MediaPlaced) {
    goto Done;
  }

  //
  // Force file system be reinstalled after media is changed.
  //
  ForceFileSystemReinstall ();

  Status = gtBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiSimpleFileSystemProtocolGuid,
                   NULL,
                   &NoHandles,
                   &HandleBuffer
                   );
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  for (Index = 0; Index < NoHandles; Index++) {

    FileHandle = NULL;

    Status = gtBS->HandleProtocol (
                     HandleBuffer[Index],
                     &gEfiSimpleFileSystemProtocolGuid,
                     &SimpleFileSystem
                     );
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (!IsMediaReadOnly (HandleBuffer[Index])) {
      continue;
    }

    //
    // perform the test
    //

    Status = SimpleFileSystem->OpenVolume (SimpleFileSystem, &Root);
    if (EFI_SUCCESS == Status) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_ManualAssertionGuid025,
                   L"Extensive Test 3 - OpenVolume()",
                   L"%a:%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    //
    // Create a file
    //
    Status = Root->Open (
                     Root,
                     &FileHandle,
                     NewFileName,
                     CREATE_FILE_MODE,
                     0
                     );
    if (EFI_WRITE_PROTECTED == Status) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_ManualAssertionGuid026,
                   L"Extensive Test 3 - Open(),Create file on write-protected media",
                   L"%a:%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    if (FileHandle != NULL) {
      FileHandle->Delete (FileHandle);
      FileHandle = NULL;
    }

    //
    // Open an existing file for write
    //
    Status = Root->Open (
                     Root,
                     &FileHandle,
                     FileName,
                     EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                     0
                     );
    if (Status != EFI_NOT_FOUND) {
      if (EFI_WRITE_PROTECTED == Status) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gSimpleFileSystemExtensiveTest_ManualAssertionGuid027,
                     L"Extensive Test 3 - Open(),Open file for write on write-protected media",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
    }

    if (FileHandle != NULL) {
      FileHandle->Close (FileHandle);
      FileHandle = NULL;
    }

    //
    // Delete the existing file
    //
    Status = Root->Open (
                     Root,
                     &FileHandle,
                     FileName,
                     EFI_FILE_MODE_READ,
                     0
                     );
    if (!EFI_ERROR (Status)) {

      Status = FileHandle->Delete (FileHandle);
      if (EFI_WARN_DELETE_FAILURE == Status) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gSimpleFileSystemExtensiveTest_ManualAssertionGuid028,
                     L"Extensive Test 3 - Delete(),Delete file on write-protected media",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
      FileHandle = NULL;
    }

    //
    // SetPosition for the existing file
    //
    Status = Root->Open (
                     Root,
                     &FileHandle,
                     FileName,
                     EFI_FILE_MODE_READ,
                     0
                     );
    if (!EFI_ERROR (Status)) {

      Status = FileHandle->SetPosition (FileHandle, 0);
      if (EFI_SUCCESS == Status) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gSimpleFileSystemExtensiveTest_ManualAssertionGuid029,
                     L"Extensive Test 3 - SetPosition(),SetPosition file on write-protected media",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );

      Status = FileHandle->Close (FileHandle);
      if (EFI_SUCCESS == Status) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gSimpleFileSystemExtensiveTest_ManualAssertionGuid030,
                     L"Extensive Test 3 - Close(),Close file on write-protected media",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
      FileHandle = NULL;
    }
  }

Done:

  if (HandleBuffer != NULL) {
    gtBS->FreePool (HandleBuffer);
  }

  if (TimerEvent != NULL) {
    gtBS->SetTimer (TimerEvent, TimerCancel, 0);
    gtBS->CloseEvent (TimerEvent);
  }

  return EFI_SUCCESS;

}

/**
 *  @brief Entrypoint for Extensive Test 4.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL
 *  @param ClientInterface A pointer to the interface array under test
 *  @param TestLevel Test "thoroughness" control
 *  @param SupportHandle A handle containing protocols required
 *  @return EFI_SUCCESS
 *  @return EFI_NOT_FOUND
 */
//
// TDS 5.3.4
//
EFI_STATUS
BBTestSimpleFileSytemExtensiveTest4 (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
)
/*
  The volume full media should contain a file named
  BBTestSimpleFileSytemExtensiveTest4_ExistingFile.
*/
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *SimpleFileSystem;
  EFI_FILE                              *Root;
  EFI_STATUS                            Status;
  EFI_HANDLE                            *HandleBuffer;
  UINTN                                 NoHandles;
  UINTN                                 Index;
  EFI_EVENT                             TimerEvent;
  EFI_EVENT                             NotifyEvent;
  VOID                                  *Registration;
  BOOLEAN                               MediaPlaced;
  UINTN                                 FileIndex;
  EFI_FILE                              *DirHandle;
  EFI_FILE                              *FileHandle;
  EFI_STATUS                            LocateStatus;
  UINTN                                 TempSize;
  UINT8                                 Temp;
  CHAR16                                *PrintMessage;
  CHAR16                                DirName[4][100];
  CHAR16                                ExistingFileName[100];

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Init
  //
  HandleBuffer = NULL;
  TimerEvent = NULL;
  NotifyEvent = NULL;

  EfiStrCpy (DirName[0], L"BBTestSimpleFileSytemExtensiveTest4_Dir1");
  EfiStrCpy (DirName[1], L"BBTestSimpleFileSytemExtensiveTest4_Dir2");
  EfiStrCpy (DirName[2], L"BBTestSimpleFileSytemExtensiveTest4_Dir3");
  EfiStrCpy (DirName[3], L"BBTestSimpleFileSytemExtensiveTest4_Dir4");

  EfiStrCpy (ExistingFileName, L"BBTestSimpleFileSytemExtensiveTest4_ExistingFile");

  //
  // create a timer event
  //
  TimerEvent = NULL;
  Status = gtBS->CreateEvent (EFI_EVENT_TIMER, 0, NULL, NULL, &TimerEvent);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Create timer fail",
                   L"%a,%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  PrintMessage = L"\nPLEASE REPLACE/INSERT A MEDIA WITH VOLUME FULL ONE\n";
  PrintDevicePathAndAskForInput (
    PrintMessage,
    NULL,
    TimerEvent,
    WAIT_100_SECONDS,
    &MediaPlaced
    );

  if (!MediaPlaced) {
    goto Done;
  }

  //
  // register a event for the changed system file system protocol
  //
  Status = gtBS->CreateEvent (
                   EFI_EVENT_NOTIFY_WAIT,
                   EFI_TPL_CALLBACK,
                   TestNotifyFunction0,
                   NULL,
                   &NotifyEvent
                   );
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Create event fail",
                   L"%a,%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    goto Done;
  }

  Status = gtBS->RegisterProtocolNotify (
                   &gEfiSimpleFileSystemProtocolGuid,
                   NotifyEvent,
                   &Registration
                   );
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"RegisterProtocolNotify fail",
                   L"%a,%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    goto Done;
  }

  //
  // Force file system be reinstalled after media is changed.
  //
  ForceFileSystemReinstall ();

  LocateStatus = EFI_SUCCESS;
  while (!EFI_ERROR (LocateStatus)) {

    LocateStatus = gtBS->LocateHandleBuffer (
                           ByRegisterNotify,
                           &gEfiSimpleFileSystemProtocolGuid,
                           Registration,
                           &NoHandles,
                           &HandleBuffer
                           );
    if (EFI_ERROR (Status)) {
      break;
    }

    for (Index = 0; Index < NoHandles; Index++) {

      FileHandle = NULL;

      Status = gtBS->HandleProtocol (
                       HandleBuffer[Index],
                       &gEfiSimpleFileSystemProtocolGuid,
                       &SimpleFileSystem
                       );
      if (EFI_ERROR (Status)) {
        continue;
      }

      //
      // perform the test
      //

      Status = SimpleFileSystem->OpenVolume (SimpleFileSystem, &Root);
      if (EFI_SUCCESS == Status) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gSimpleFileSystemExtensiveTest_ManualAssertionGuid031,
                     L"Extensive Test 4 - OpenVolume()",
                     L"%a:%d: Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );

      //
      // Create some files on the disk
      //
      for (FileIndex = 0; FileIndex < sizeof (DirName) / sizeof (*DirName); FileIndex++) {
        DirHandle = NULL;
        Status = Root->Open (
                         Root,
                         &DirHandle,
                         DirName[FileIndex],
                         CREATE_FILE_MODE,
                         EFI_FILE_DIRECTORY
                         );
        if (EFI_VOLUME_FULL == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid032,
                       L"Extensive Test 4 - Open(),Create file on Full media",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );
        if (DirHandle != NULL) {
          DirHandle->Delete (DirHandle);
          DirHandle = NULL;
        }
      }

      //
      // Open an existing file for write
      //
      FileHandle = NULL;
      Status = Root->Open (
                       Root,
                       &FileHandle,
                       ExistingFileName,
                       EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                       0
                       );
      if (Status == EFI_SUCCESS) {

        TempSize = 1;
        Status = FileHandle->Write (FileHandle, &TempSize, &Temp);
        if (EFI_VOLUME_FULL == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid033,
                       L"Extensive Test 4 - Write(),Write to Full media",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        Status = FileHandle->Flush (FileHandle);
        if (EFI_VOLUME_FULL == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid034,
                       L"Extensive Test 4 - Flush(),Flush to Full media",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        Status = InternalSetFileSize (FileHandle, 100000);
        if (EFI_VOLUME_FULL == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid035,
                       L"Extensive Test 4 - SetInfo(),SetInfo to Full media",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );

        Status = FileHandle->Close (FileHandle);
        if (EFI_SUCCESS == Status) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gSimpleFileSystemExtensiveTest_ManualAssertionGuid036,
                       L"Extensive Test 4 - Close(),Close file on Full media",
                       L"%a:%d: Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );
      }
    }

    if (HandleBuffer != NULL) {
      gtBS->FreePool (HandleBuffer);
      HandleBuffer = NULL;
    }
  }

Done:

  if (TimerEvent != NULL) {
    gtBS->SetTimer (TimerEvent, TimerCancel, 0);
    gtBS->CloseEvent (TimerEvent);
  }

  if (NotifyEvent != NULL) {
    gtBS->CloseEvent (NotifyEvent);
  }

  return EFI_SUCCESS;

}

VOID
ComposeDevicePathRepository (
  EFI_DEVICE_PATH_PROTOCOL              **WholeDevicePathArray,
  UINT32                                *DevicePathAmount
  )
{
  EFI_STATUS                            Status;
  EFI_HANDLE                            *HandleBuffer;
  UINTN                                 NoHandles;
  UINTN                                 Index;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL              *NewDevicePath;

  //
  // Retrieve all Simple File System protocols
  //
  Status = gtBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiSimpleFileSystemProtocolGuid,
                   NULL,
                   &NoHandles,
                   &HandleBuffer
                   );
  if (EFI_ERROR (Status) || (NoHandles == 0)) {
    Print (
      L"%a,%d: Generic Error: Locate Simple File System Protocol fail, Status - %r",
      __FILE__,
      (UINTN)__LINE__,
      Status
      );
    return;
  }

  //
  // record the device paths into the device path repository
  //
  for (Index = 0; Index < NoHandles; Index++) {

    Status = gtBS->HandleProtocol (
                     HandleBuffer[Index],
                     &gEfiDevicePathProtocolGuid,
                     &DevicePath
                     );
    if (EFI_ERROR (Status)) {
      Print (
        L"%a,%d: Retrieve device path fail, Status - %r",
        __FILE__,
        (UINTN)__LINE__,
        Status
        );
      continue;
    }

    //
    // duplicate the device path content.
    //
    NewDevicePath = NULL;
    NewDevicePath = DuplicateDevicePath (DevicePath);
    if (NewDevicePath == NULL) {
      Print (
        L"%a,%d: Generic Error: DuplicateDevicePath fail",
        __FILE__,
        (UINTN)__LINE__
        );
      continue;
    }

    WholeDevicePathArray[*DevicePathAmount] = NewDevicePath;

    (*DevicePathAmount)++;

    if ((*DevicePathAmount) == MAX_DEVICE_PATH_AMOUNT) {
      break;
    }
  }

  return;
}

BOOLEAN
AllFileSystemTested (
  EFI_DEVICE_PATH_PROTOCOL    **WholeDevicePathArray,
  UINT32                      DevicePathAmount,
  EFI_DEVICE_PATH_PROTOCOL    **TestedDevicePathArray
  )
/*
  if every device path of WholeDevicePathArray also appear in
  the TestedDevicePathArray, it means All File Systems are Tested
*/
{
  UINT32      Indexi, Indexj, MatchCount;

  MatchCount = 0;

  for (Indexi = 0; Indexi < DevicePathAmount; Indexi++) {
    for (Indexj = 0; Indexj < MAX_DEVICE_PATH_AMOUNT; Indexj++) {

      if (TestedDevicePathArray[Indexj] == NULL) {
        break;
      }

      //
      // match
      //
      if (!EfiCompareMem (WholeDevicePathArray[Indexi],
                         TestedDevicePathArray[Indexj],
                         DevicePathSize (WholeDevicePathArray[Indexi]))) {
        MatchCount++;
      }
    }
  }

  if (MatchCount == DevicePathAmount) {
    return TRUE;
  }

  return FALSE;
}

BOOLEAN
IsDevicePathInRepository (
  EFI_DEVICE_PATH_PROTOCOL    *DevicePath,
  EFI_DEVICE_PATH_PROTOCOL    **DevicePathArray,
  UINT32                      DevicePathAmount
  )
{
  UINTN                       Index;

  for (Index = 0; Index < DevicePathAmount; Index++) {

    if (DevicePath == NULL) {
      break;
    }

    if (DevicePathArray[Index] == NULL) {
      break;
    }

    if (!EfiCompareMem (DevicePathArray[Index],
                        DevicePath,
                        DevicePathSize (DevicePathArray[Index]))) {
      return TRUE;
    }
  }

  return FALSE;
}

BOOLEAN
IsRemovableDevice (
  EFI_HANDLE    DeviceHandle
  )
/*
  if no Block I/O attached on the DeviceHandle, the default is Removable device
*/
{
  EFI_STATUS              Status;
  EFI_BLOCK_IO_PROTOCOL   *BlockIo;

  Status = gtBS->HandleProtocol (
                   DeviceHandle,
                   &gEfiBlockIoProtocolGuid,
                   &BlockIo
                   );
  if (EFI_ERROR (Status)) {
    return TRUE;
  }

  return BlockIo->Media->RemovableMedia;
}

EFI_STATUS
CreateSampleDirFileTree (
  EFI_FILE                              *Root,
  EFI_FILE                              **DirHandle,
  EFI_FILE                              **FileHandle
  )
{
  EFI_STATUS                Status;
  UINT32                    FileIndex;
  CHAR16                    FileName[5][100];
  CHAR16                    DirName[2][100];

  EfiStrCpy (FileName[0], L"File1");
  EfiStrCpy (FileName[1], L"File2");
  EfiStrCpy (FileName[2], L"File3");
  EfiStrCpy (FileName[3], L"File4");
  EfiStrCpy (FileName[4], L"File5");

  EfiStrCpy (DirName[0], L"Dir1");
  EfiStrCpy (DirName[1], L"Dir2");

  //
  // create D1 and D2.
  //
  for (FileIndex = 0; FileIndex < 2; FileIndex++) {

    Status = Root->Open (
                     Root,
                     &DirHandle[FileIndex],
                     DirName[FileIndex],
                     CREATE_FILE_MODE,
                     EFI_FILE_DIRECTORY
                     );
    if (EFI_ERROR (Status)) {
      Print (
        L"%a,%d: Generic Error: Create Directory %s fail, Status - %r",
        __FILE__,
        (UINTN)__LINE__,
        DirName[FileIndex],
        Status
        );
      goto ErrorExit;
    }
  }
  //
  // create F2 and F3 (under D1)
  // set file size to 10
  //
  for (FileIndex = 1; FileIndex < 3; FileIndex++) {

    Status = DirHandle[0]->Open (
                             DirHandle[0],
                             &FileHandle[FileIndex],
                             FileName[FileIndex],
                             CREATE_FILE_MODE,
                             0
                             );
    if (EFI_ERROR (Status)) {
      Print (
        L"%a,%d: Generic Error: Create File %s fail, Status - %r",
        __FILE__,
        (UINTN)__LINE__,
        FileName[FileIndex],
        Status
        );
      goto ErrorExit;
    }

    InternalSetFileSize (FileHandle[FileIndex], 10);
  }

  //
  // create F4 and F5 (under D2)
  // set file size to 10
  //
  for (FileIndex = 3; FileIndex < 5; FileIndex++) {

    Status = DirHandle[1]->Open (
                             DirHandle[1],
                             &FileHandle[FileIndex],
                             FileName[FileIndex],
                             CREATE_FILE_MODE,
                             0
                             );
    if (EFI_ERROR (Status)) {
      Print (
        L"%a,%d: Generic Error: Create File %s fail, Status - %r",
        __FILE__,
        (UINTN)__LINE__,
        FileName[FileIndex],
        Status
        );
      goto ErrorExit;
    }

    InternalSetFileSize (FileHandle[FileIndex], 10);
  }

  //
  // create F1
  // set file size to 10
  //
  Status = Root->Open (
                   Root,
                   &FileHandle[0],
                   FileName[0],
                   CREATE_FILE_MODE,
                   0
                   );
  if (EFI_ERROR (Status)) {
    Print (
      L"%a,%d: Generic Error: Create File %s fail, Status - %r",
      __FILE__,
      (UINTN)__LINE__,
      FileName[0],
      Status
      );
    goto ErrorExit;
  }

  InternalSetFileSize (FileHandle[0], 10);

  return EFI_SUCCESS;

ErrorExit:

  for (FileIndex = 0; FileIndex < 5; FileIndex++) {
    if (FileHandle[FileIndex] != NULL) {
      FileHandle[FileIndex]->Delete (FileHandle[FileIndex]);
    }
  }

  for (FileIndex = 0; FileIndex < 2; FileIndex++) {
    if (DirHandle[FileIndex] != NULL) {
      DirHandle[FileIndex]->Delete (DirHandle[FileIndex]);
    }
  }

  return Status;

}

VOID
CloseSampleDirFileTree (
  EFI_FILE                              *Root,
  EFI_FILE                              **DirHandle,
  EFI_FILE                              **FileHandle
  )
{
  UINT32                    FileIndex;

  for (FileIndex = 0; FileIndex < 5; FileIndex++) {
    if (FileHandle[FileIndex] != NULL) {
      FileHandle[FileIndex]->Close (FileHandle[FileIndex]);
    }
  }

  for (FileIndex = 0; FileIndex < 2; FileIndex++) {
    if (DirHandle[FileIndex] != NULL) {
      DirHandle[FileIndex]->Close (DirHandle[FileIndex]);
    }
  }

  return;
}


VOID
DeleteSampleDirFileTree (
  EFI_FILE                              *Root,
  EFI_FILE                              **DirHandle,
  EFI_FILE                              **FileHandle
  )
{
  EFI_STATUS                Status;
  UINT32                    FileIndex;
  CHAR16                    FileName[5][100];
  CHAR16                    DirName[2][100];

  EfiStrCpy (FileName[0], L"File1");
  EfiStrCpy (FileName[1], L"File2");
  EfiStrCpy (FileName[2], L"File3");
  EfiStrCpy (FileName[3], L"File4");
  EfiStrCpy (FileName[4], L"File5");

  EfiStrCpy (DirName[0], L"Dir1");
  EfiStrCpy (DirName[1], L"Dir2");

  //
  // Open D1 and D2.
  //
  for (FileIndex = 0; FileIndex < 2; FileIndex++) {

    Status = Root->Open (
                     Root,
                     &DirHandle[FileIndex],
                     DirName[FileIndex],
                     OPEN_R_W_MODE,
                     0
                     );
    if (EFI_ERROR (Status)) {
      continue;
    }
  }
  //
  // Open F2 and F3 (under D1)
  //
  if (DirHandle[0] != NULL) {
    for (FileIndex = 1; FileIndex < 3; FileIndex++) {

      Status = DirHandle[0]->Open (
                               DirHandle[0],
                               &FileHandle[FileIndex],
                               FileName[FileIndex],
                               OPEN_R_W_MODE,
                               0
                               );
      if (EFI_ERROR (Status)) {
        continue;
      }
    }
  }
  //
  // Open F4 and F5 (under D2)
  //
  if (DirHandle[1] != NULL) {
    for (FileIndex = 3; FileIndex < 5; FileIndex++) {

      Status = DirHandle[1]->Open (
                               DirHandle[1],
                               &FileHandle[FileIndex],
                               FileName[FileIndex],
                               OPEN_R_W_MODE,
                               0
                               );
      if (EFI_ERROR (Status)) {
        continue;
      }
    }
  }

  //
  // Open F1
  //
  Root->Open (Root, &FileHandle[0], FileName[0], OPEN_R_W_MODE, 0);

  for (FileIndex = 0; FileIndex < 5; FileIndex++) {
    if (FileHandle[FileIndex] != NULL) {
      FileHandle[FileIndex]->Delete (FileHandle[FileIndex]);
    }
  }

  for (FileIndex = 0; FileIndex < 2; FileIndex++) {
    if (DirHandle[FileIndex] != NULL) {
      DirHandle[FileIndex]->Delete (DirHandle[FileIndex]);
    }
  }

  return;
}

VOID
PrintStartTestMessage (
  CHAR16                      *PrintMessage,
  EFI_EVENT                   TimerEvent,
  UINT64                      TriggerTimeInSeconds,
  BOOLEAN                     *PressYes
  )
{
  EFI_STATUS                  Status;
  EFI_EVENT                   WaitList[2];
  UINTN                       NoWait, WaitIndex;
  EFI_INPUT_KEY               Key;
  CHAR16                      PrintString1[100];
  CHAR16                      PrintString2[200];

  EfiStrCpy (PrintString1, L"\nTestCase will wait");
  EfiStrCpy (PrintString2, L"seconds.\nplease enter Y or y for continue. \nplease enter N or n,or wait for the timer expire for skip.\n.");

  *PressYes = FALSE;
  Status = gtBS->SetTimer (TimerEvent, TimerPeriodic, 10000000);
  if (EFI_ERROR (Status)) {
    return;
  }

  Print (PrintMessage);

  Print (L"%s %d %s", PrintString1, TriggerTimeInSeconds, PrintString2);

  //
  // Set up a wait list for a key and the timer
  //
  NoWait = 0;
  WaitList[NoWait++] = TimerEvent;
  WaitList[NoWait++] = gtST->ConIn->WaitForKey;

  while (NoWait) {

    Print (L"\rYes/No?(Auto judge as No in %d seconds)", TriggerTimeInSeconds);

    Status = gtBS->WaitForEvent (NoWait, WaitList, &WaitIndex);

    switch (WaitIndex) {
      case 0:
        TriggerTimeInSeconds -= 1;
        if (TriggerTimeInSeconds <= 0) {
          //
          // To stop loop
          //
          NoWait = 0;
        }
        break;

      case 1:
        //
        // Read the key
        //
        Status = gtST->ConIn->ReadKeyStroke (gtST->ConIn, &Key);
        APrint ((char*)(&Key.UnicodeChar));
        switch (Key.UnicodeChar) {
          case 'Y':
          case 'y':
            NoWait = 0;
            *PressYes = TRUE;
            break;
          case 'N':
          case 'n':
            NoWait = 0;
            *PressYes = FALSE;
            break;
          default :
            Print (L"\n");
            break;
        }
        break;
      default:
        break;
        //
        // bad WaitIndex response
        //
    }
  }
  Print (L"\r\n");

  //
  // Done, cancle periodic timer
  //
  gtBS->SetTimer (TimerEvent, TimerCancel, 0);

  if (PressYes) {
    ForceFileSystemReinstall ();
  }

  return;
}

VOID
PrintDevicePathAndAskForInput (
  CHAR16                      *PrintMessage,
  EFI_DEVICE_PATH_PROTOCOL    *DevicePath,      OPTIONAL
  EFI_EVENT                   TimerEvent,
  UINT64                      TriggerTimeInSeconds,
  BOOLEAN                     *PressYes
  )
{
  CHAR16                      *DevicePathString;
  EFI_STATUS                  Status;
  EFI_EVENT                   WaitList[2];
  UINTN                       NoWait, WaitIndex;
  EFI_INPUT_KEY               Key;
  CHAR16                      PrintString1[100];
  CHAR16                      PrintString2[200];

  EfiStrCpy (PrintString1, L"\nTestCase will wait");
  EfiStrCpy (PrintString2, L"seconds.\nIf done, please enter Y or y. \nIf not,please enter N or n, or wait for the timer expire.\n.");

  //
  // default is FALSE
  //
  *PressYes = FALSE;

  DevicePathString = NULL;
  if (DevicePath != NULL) {
    DevicePathString = DevicePathToStr (DevicePath);
  }

  Status = gtBS->SetTimer (TimerEvent, TimerPeriodic, 10000000);
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  if (DevicePath != NULL) {
    Print (L"Device of %s\n", DevicePathString);
  }
  Print (PrintMessage);
  Print (L"%s %d %s", PrintString1, TriggerTimeInSeconds, PrintString2);

  //
  // Set up a wait list for a key and the timer
  //
  NoWait = 0;
  WaitList[NoWait++] = TimerEvent;
  WaitList[NoWait++] = gtST->ConIn->WaitForKey;

  while (NoWait) {

    Print (L"\rYes/No?(Auto judge as No in %d seconds)", TriggerTimeInSeconds);

    Status = gtBS->WaitForEvent (NoWait, WaitList, &WaitIndex);

    switch (WaitIndex) {
      case 0:
        TriggerTimeInSeconds -= 1;
        if (TriggerTimeInSeconds <= 0) {
          //
          // To stop loop
          //
          NoWait = 0;
        }
        break;

      case 1:
        //
        // Read the key
        //
        Status = gtST->ConIn->ReadKeyStroke (gtST->ConIn, &Key);
        APrint ((char*)(&Key.UnicodeChar));
        switch (Key.UnicodeChar) {
          case 'Y':
          case 'y':
            NoWait = 0;
            *PressYes = TRUE;
            break;
          case 'N':
          case 'n':
            NoWait = 0;
            *PressYes = FALSE;
            break;
          default :
            Print (L"\n");
            break;
        }
        break;
      default:
        break;
        //
        // bad WaitIndex response
        //
    }
  }
  Print (L"\r\n");

  //
  // Done, cancle periodic timer
  //
  gtBS->SetTimer (TimerEvent, TimerCancel, 0);

Done:

  if (DevicePathString != NULL) {
    gtBS->FreePool (DevicePathString);
  }

  return;
}

VOID
ForceFileSystemReinstall ()
/*
  read each Block Io in the file system will force
  those media changed file systems reinstalled
*/
{
  EFI_STATUS              Status;
  EFI_BLOCK_IO_PROTOCOL   *BlockIo;
  EFI_HANDLE              *HandleBuffer;
  UINTN                   NoHandles;
  UINTN                   Index;
  UINTN                   BufferSize;
  UINT8                   *Buffer;


  HandleBuffer = NULL;

  Status = gtBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiBlockIoProtocolGuid,
                   NULL,
                   &NoHandles,
                   &HandleBuffer
                   );
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < NoHandles; Index++) {
    Status = gtBS->HandleProtocol (
                     HandleBuffer[Index],
                     &gEfiBlockIoProtocolGuid,
                     &BlockIo
                     );
    if (EFI_ERROR (Status)) {
      continue;
    }

    BufferSize = BlockIo->Media->BlockSize;
    Status = gtBS->AllocatePool (
                     EfiBootServicesData,
                     BufferSize,
                     (VOID**)&Buffer
                     );
    if (EFI_ERROR (Status)) {
      continue;
    }

    BlockIo->ReadBlocks (BlockIo, BlockIo->Media->MediaId, 0, BufferSize, Buffer);

    gtBS->FreePool (Buffer);
  }

  gtBS->FreePool (HandleBuffer);

  return;
}


VOID
EFIAPI
TestNotifyFunction0 (
  EFI_EVENT Event,
  VOID      *Context
  )
{
  //
  // do nothing
  //
  return;
}

BOOLEAN
IsMediaReadOnly (
  EFI_HANDLE    DeviceHandle
  )
/*
 default is Non-ReadOnly
*/
{
  EFI_STATUS              Status;
  EFI_BLOCK_IO_PROTOCOL   *BlockIo;

  Status = gtBS->HandleProtocol (
                   DeviceHandle,
                   &gEfiBlockIoProtocolGuid,
                   &BlockIo
                   );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return BlockIo->Media->ReadOnly;
}

