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
    SimpleFileSystemExtensiveTest_Auto.c

Abstract:
    for EFI Simple File System Protocol and EFI_FILE Protocol's Automatic
    Extensive Test

--*/

#include "SimpleFileSystemBBTest.h"

#define MAX_DEVICE_PATH_AMOUNT    100
#define SECONDS_TO_100NS  10000000
#define WAIT_10_SECONDS   10

//
// some private functions declaration
//
VOID
BBTestSimpleFileSytemExtensiveTest6_ReadWriteInternal (
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  EFI_FILE                              *Root,
  UINTN                                 Length
  );

/**
 *  @brief Entrypoint for Extensive Test 5.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL
 *  @param ClientInterface A pointer to the interface array under test
 *  @param TestLevel Test "thoroughness" control
 *  @param SupportHandle A handle containing protocols required
 *  @return EFI_SUCCESS
 *  @return EFI_NOT_FOUND
 */
//
// TDS 5.3.5
//
EFI_STATUS
BBTestSimpleFileSytemExtensiveTest5 (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *SimpleFileSystem;
  EFI_FILE                              *Root;
  EFI_STATUS                            Status;
  EFI_FILE                              *FileHandle;
  EFI_FILE                              *DirHandle;
  EFI_FILE                              *OpenFileHandle[100];
  EFI_FILE                              *OpenDirHandle[100];
  UINT64                                Position;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_FILE_INFO                         *FileInfo;
  UINTN                                 InfoBufferSize;
  EFI_FILE                              *NewFileHandle;
  EFI_STATUS                            Status1, Status2;
  UINTN                                 TempSize;
  UINT8                                 Temp;
  UINTN                                 FileIndex;
  CHAR16                                FileName[100];
  CHAR16                                DirName[100];

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

  SimpleFileSystem = (EFI_SIMPLE_FILE_SYSTEM_PROTOCOL*)ClientInterface;

  FileHandle = NULL;
  DirHandle = NULL;
  NewFileHandle = NULL;
  gtBS->SetMem (OpenFileHandle, 100 * sizeof (EFI_FILE*), 0);
  gtBS->SetMem (OpenDirHandle, 100 * sizeof (EFI_FILE*), 0);
  EfiStrCpy (FileName, L"BBTestSimpleFileSytemExtensiveTest5_File");
  EfiStrCpy (DirName, L"BBTestSimpleFileSytemExtensiveTest5_Dir");

  Status = SimpleFileSystem->OpenVolume (SimpleFileSystem, &Root);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"OpenVolume fail",
                   L"%a:%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                 );
    return Status;
  }

  //
  // Create the file and directory
  //
  Status = Root->Open (
                   Root,
                   &DirHandle,
                   DirName,
                   CREATE_FILE_MODE,
                   EFI_FILE_DIRECTORY
                   );
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Create Directory fail",
                   L"%a:%d: Status - %r, DirName - %s",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status,
                   DirName
                   );
    return Status;
  }

  Status = Root->Open (
                   Root,
                   &FileHandle,
                   FileName,
                   CREATE_FILE_MODE,
                   0
                   );
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Create file fail",
                   L"%a:%d: Status - %r, FileName - %s",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status,
                   FileName
                   );
    goto Done;
  }

    //
    // Open the file and directory for 100 times
    //
  for (FileIndex = 0; FileIndex < 100; FileIndex++) {
    Root->Open (
            Root,
            &OpenDirHandle[FileIndex],
            DirName,
            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
            0
            );
    Root->Open (
            Root,
            &OpenFileHandle[FileIndex],
            FileName,
            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
            0
            );
  }

  //
  // Delete the file and the dir
  //
  Status1 = FileHandle->Delete (FileHandle);
  Status2 = DirHandle->Delete (DirHandle);

  FileHandle = NULL;
  DirHandle = NULL;

  if ((Status1 != EFI_SUCCESS) || (Status2 != EFI_SUCCESS)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Delete files fail",
                   L"%a:%d: File - %s Status - %r, Dir - %s Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   FileName,
                   Status1,
                   DirName,
                   Status2
                   );
    goto Done;
  }

  //
  // perform the test for the file
  //
  for (FileIndex = 0; FileIndex < 100; FileIndex++) {

    if (OpenFileHandle[FileIndex] == NULL) {
      continue;
    }

    //
    // Write
    //
    TempSize = 1;
    Status = OpenFileHandle[FileIndex]->Write (
                                          OpenFileHandle[FileIndex],
                                          &TempSize,
                                          &Temp
                                          );
    if (Status == EFI_DEVICE_ERROR) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_AutoAssertionGuid001,
                   L"Extensive Test 5 - Write()",
                   L"%a:%d: Write data to deleted file Status - %r, UpdatedSize - %d",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status,
                   TempSize
                   );
    //
    // Flush
    //
    Status = OpenFileHandle[FileIndex]->Flush (OpenFileHandle[FileIndex]);

    if (Status != EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_AutoAssertionGuid002,
                   L"Extensive Test 5 - Flush()",
                   L"%a:%d: Flush data to deleted file Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    //
    // Read
    //
    TempSize = 1;
    Status = OpenFileHandle[FileIndex]->Read (
                                          OpenFileHandle[FileIndex],
                                          &TempSize,
                                          &Temp
                                          );
    if (Status == EFI_DEVICE_ERROR) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_AutoAssertionGuid003,
                   L"Extensive Test 5 - Read()",
                   L"%a:%d: Read data from deleted file,Status - %r, UpdatedSize - %d",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status,
                   TempSize
                   );
    //
    // SetPosition
    //
    Status = OpenFileHandle[FileIndex]->SetPosition (
                                          OpenFileHandle[FileIndex],
                                          0
                                          );
    if (Status != EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_AutoAssertionGuid004,
                   L"Extensive Test 5 - SetPosition()",
                   L"%a:%d: SetPosition for deleted file, Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    //
    // GetPosition
    //
    Status = OpenFileHandle[FileIndex]->GetPosition (
                                          OpenFileHandle[FileIndex],
                                          &Position
                                          );
    if (Status != EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_AutoAssertionGuid005,
                   L"Extensive Test 5 - GetPosition()",
                   L"%a:%d: GetPosition for deleted file, Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    //
    // GetInfo
    //
    FileInfo = NULL;
    Status = InternalGetInfo (
               OpenFileHandle[FileIndex],
               &FileInfo,
               &InfoBufferSize,
               &gEfiFileInfoGuid
               );
    if (Status != EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_AutoAssertionGuid006,
                   L"Extensive Test 5 - GetInfo()",
                   L"%a:%d: GetInfo for deleted file, Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    if (FileInfo != NULL) {
      gtBS->FreePool (FileInfo);
      FileInfo = NULL;
      InfoBufferSize = 1;
    }
    //
    // SetInfo
    //
    Status = OpenFileHandle[FileIndex]->SetInfo (
                                          OpenFileHandle[FileIndex],
                                          &gEfiFileInfoGuid,
                                          InfoBufferSize,
                                          &Temp
                                          );
    if (Status != EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_AutoAssertionGuid007,
                   L"Extensive Test 5 - SetInfo()",
                   L"%a:%d: SetInfo for deleted file, Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

    //
    // perform the test for the directory
    //
  for (FileIndex = 0; FileIndex < 100; FileIndex++) {

    if (OpenDirHandle[FileIndex] == NULL) {
      continue;
    }

    //
    // create file
    //
    Status = OpenDirHandle[FileIndex]->Open (
                                         OpenDirHandle[FileIndex],
                                         &NewFileHandle,
                                         FileName,
                                         EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,
                                         0
                                         );
    if (EFI_SUCCESS != Status) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_AutoAssertionGuid008,
                   L"Extensive Test 5 - Open()",
                   L"%a:%d: create file under deleted directory, Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    //
    // SetPosition
    //
    Status = OpenDirHandle[FileIndex]->SetPosition (
                                         OpenDirHandle[FileIndex],
                                         0
                                         );
    if (Status != EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_AutoAssertionGuid009,
                   L"Extensive Test 5 - SetPosition()",
                   L"%a:%d: SetPosition for deleted dir, Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    //
    // GetInfo
    //
    FileInfo = NULL;
    Status = InternalGetInfo (
               OpenDirHandle[FileIndex],
               &FileInfo,
               &InfoBufferSize,
               &gEfiFileInfoGuid
               );
    if (Status != EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_AutoAssertionGuid010,
                   L"Extensive Test 5 - GetInfo()",
                   L"%a:%d: GetInfo for deleted dir, Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    if (FileInfo != NULL) {
      gtBS->FreePool (FileInfo);
      FileInfo = NULL;
      InfoBufferSize = 1;
    }
    //
    // SetInfo
    //
    Status = OpenDirHandle[FileIndex]->SetInfo (
                                         OpenDirHandle[FileIndex],
                                         &gEfiFileInfoGuid,
                                         InfoBufferSize,
                                         &Temp
                                         );
    if (Status != EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gSimpleFileSystemExtensiveTest_AutoAssertionGuid011,
                   L"Extensive Test 5 - SetInfo()",
                   L"%a:%d: SetInfo for deleted dir, Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

Done:

  for (FileIndex = 0; FileIndex < 100; FileIndex++) {

    if (OpenDirHandle[FileIndex] != NULL) {
      OpenDirHandle[FileIndex]->Close (OpenDirHandle[FileIndex]);
    }

    if (OpenFileHandle[FileIndex] != NULL) {
      OpenFileHandle[FileIndex]->Close (OpenFileHandle[FileIndex]);
    }
  }

  if (DirHandle != NULL) {
    DirHandle->Delete (DirHandle);
  }

  if (FileHandle != NULL) {
    FileHandle->Delete (FileHandle);
  }

  return EFI_SUCCESS;

}

/**
 *  @brief Entrypoint for Extensive Test 6.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL
 *  @param ClientInterface A pointer to the interface array under test
 *  @param TestLevel Test "thoroughness" control
 *  @param SupportHandle A handle containing protocols required
 *  @return EFI_SUCCESS
 *  @return EFI_NOT_FOUND
 */
//
// TDS 5.3.6
//
EFI_STATUS
BBTestSimpleFileSytemExtensiveTest6 (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
/*
  Write a series length of data to a file, length could be:
    1, 2, 100, 511, 512, 513, 1024, 1025, 1M, 2M - 3, 2M, 2M + 1, 10M.
  Read the data back and compare.

*/
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *SimpleFileSystem;
  EFI_FILE                              *Root;
  UINTN                                 Loop;
  UINTN             LengthArray[] = {1, 2, 100, 511, 512, 513, 1024, 1025, 1024 * 1024,
                                     2048 * 1024 - 3, 2048 * 1024,
                                     2048 * 1024 + 1, 10240 * 1024};

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

  SimpleFileSystem = (EFI_SIMPLE_FILE_SYSTEM_PROTOCOL*)ClientInterface;

  Status = SimpleFileSystem->OpenVolume (SimpleFileSystem, &Root);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"OpenVolume fail",
                   L"%a:%d: Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                 );
    return Status;
  }

  for (Loop = 0; Loop < (sizeof (LengthArray) / sizeof (UINTN)); Loop++) {

    BBTestSimpleFileSytemExtensiveTest6_ReadWriteInternal (StandardLib, Root, LengthArray[Loop]);
  }

  return EFI_SUCCESS;

}

/**
 *  @brief Entrypoint for Open() Stress Test (a stress version of Open() function test
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL
 *  @param ClientInterface A pointer to the interface array under test
 *  @param TestLevel Test "thoroughness" control
 *  @param SupportHandle A handle containing protocols required
 *  @return EFI_SUCCESS
 *  @return EFI_NOT_FOUND
 */
//
// TDS 5.3.7
//
EFI_STATUS
BBTestSimpleFileSytemOpenStress (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *SimpleFileSystem;

  //
  // init
  //
  SimpleFileSystem = (EFI_SIMPLE_FILE_SYSTEM_PROTOCOL*)ClientInterface;

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
  // 5.1.2.2.1  Call Open() to create new files(directories)
  // with valid file names and valid attributes.
  //
  BBTestOpenBasicTestCheckpoint1 (StandardLib, SimpleFileSystem, TRUE);

  //
  // 5.1.2.2.2  Call Open() to Open existing files with all valid OpenMode.
  //
  BBTestOpenBasicTestCheckpoint2 (StandardLib, SimpleFileSystem, TRUE);

  return EFI_SUCCESS;

}

/**
 *  @brief Entrypoint for GetInfo() Stress Test (a stress version of GetInfo() function test
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL
 *  @param ClientInterface A pointer to the interface array under test
 *  @param TestLevel Test "thoroughness" control
 *  @param SupportHandle A handle containing protocols required
 *  @return EFI_SUCCESS
 *  @return EFI_NOT_FOUND
 */
//
// TDS 5.3.8
//
EFI_STATUS
BBTestSimpleFileSytemGetInfoStress (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *SimpleFileSystem;

  //
  // init
  //
  SimpleFileSystem = (EFI_SIMPLE_FILE_SYSTEM_PROTOCOL*)ClientInterface;

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
  // 5.1.10.2.1  Call GetInfo() to retrieve EFI_FILE_INFO
  // from a file/directory.
  //
  BBTestGetInfoBasicTestCheckpoint1 (StandardLib, SimpleFileSystem, TRUE);

  return EFI_SUCCESS;
}

/**
 *  @brief Entrypoint for SetInfo() Stress Test (a stress version of SetInfo() function test
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL
 *  @param ClientInterface A pointer to the interface array under test
 *  @param TestLevel Test "thoroughness" control
 *  @param SupportHandle A handle containing protocols required
 *  @return EFI_SUCCESS
 *  @return EFI_NOT_FOUND
 */
//
// TDS 5.3.9
//
EFI_STATUS
BBTestSimpleFileSytemSetInfoStress (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *SimpleFileSystem;

  //
  // init
  //
  SimpleFileSystem = (EFI_SIMPLE_FILE_SYSTEM_PROTOCOL*)ClientInterface;

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
  // 5.1.11.2.1 Call SetInfo() to set valid EFI_FILE_INFO for a file/directory.
  //
  BBTestSetInfoBasicTestCheckpoint1 (StandardLib, SimpleFileSystem, TRUE);

  return EFI_SUCCESS;
}

VOID
BBTestSimpleFileSytemExtensiveTest6_ReadWriteInternal (
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  EFI_FILE                              *Root,
  UINTN                                 Length
  )
/*
  description:  Sub-function for BBTestSimpleFileSytemExtensiveTest6.
*/
{
  EFI_STATUS                Status;
  EFI_FILE                  *FileHandle = NULL;
  EFI_FILE                  *OpenHandle = NULL;
  CHAR16                    FileName[100];
  UINTN                     WriteSize, ReadSize;
  UINT8                     *Buffer1 = NULL;
  UINT8                     *Buffer2 = NULL;
  UINTN                     Loop;
  EFI_TEST_ASSERTION        AssertionType;

  EfiStrCpy (FileName, L"BBTestSimpleFileSytemExtensiveTest6_File");

  //
  // Create a new file
  //
  Status = Root->Open (
                   Root,
                   &FileHandle,
                   FileName,
                   EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE|EFI_FILE_MODE_CREATE,
                   EFI_FILE_ARCHIVE
                   );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Create file fail",
                   L"%a:%d: Status - %r, FileName - %s, Length - 0x%x",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status,
                   FileName,
                   Length
                   );
    return;
  }

  //
  // Allocate buffer1
  //
  Buffer1 = NULL;
  Status = gtBS->AllocatePool (
                   EfiBootServicesData,
                   Length,
                   (VOID**)&Buffer1
                   );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Allocate Pool fail",
                   L"%a:%d: Status - %r,Length - 0x%x",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status,
                   Length
                   );
    goto Done;
  }

  //
  // Allocate buffer2
  //
  Buffer2 = NULL;
  Status = gtBS->AllocatePool (
                   EfiBootServicesData,
                   Length,
                   (VOID**)&Buffer2
                   );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Allocate Pool fail",
                   L"%a:%d: Status - %r,Length -0x%x",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status,
                   Length
                   );
    goto Done;
  }

  //
  // set Buffer1
  //
  for (Loop = 0; Loop < Length; Loop++) {
    Buffer1[Loop] = (UINT8)(Loop % 256);
  }

  //
  // Write test data to file
  //

  Status = Root->Open (
                   Root,
                   &OpenHandle,
                   FileName,
                   EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE,
                   0x0
                   );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Open file fail",
                   L"%a:%d: Status - %r, File - %s, Length - 0x%x",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status,
                   FileName,
                   Length
                   );
    goto Done;
  }

  WriteSize = Length;
  Status = OpenHandle->Write (OpenHandle, &WriteSize, (VOID*)Buffer1);
  if ((EFI_SUCCESS == Status) || (EFI_VOLUME_FULL == Status)) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gSimpleFileSystemExtensiveTest_AutoAssertionGuid012,
                 L"Extensive Test 6 - Write()",
                 L"%a:%d: Status: %r, WriteSize: 0x%x, RequestSize: 0x%x",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 WriteSize,
                 Length
                 );

  OpenHandle->Flush (OpenHandle);
  OpenHandle->Close (OpenHandle);
  OpenHandle = NULL;

  //
  // Read test data from the file
  //
  Status = Root->Open (
                   Root,
                   &OpenHandle,
                   FileName,
                   EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE,
                   0x0
                   );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"Open file fail",
                   L"%a:%d: Status - %r, File - %s, Length - 0x%x",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status,
                   FileName,
                   Length
                   );
    goto Done;
  }

  ReadSize = WriteSize;
  Status = OpenHandle->Read (OpenHandle, &ReadSize, (VOID*)Buffer2);
  if (EFI_SUCCESS == Status) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gSimpleFileSystemExtensiveTest_AutoAssertionGuid013,
                 L"Extensive Test 6 - Read()",
                 L"%a:%d: Status: %r, ReadSize: 0x%x, RequestedSize: 0x%x",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 ReadSize,
                 WriteSize
                 );

  //
  // Check read data with the original ones
  //
  for (Loop = 0; Loop < WriteSize; Loop++) {

    if (Buffer1[Loop] != Buffer2[Loop]) {
      break;
    }
  }

  if (Loop == WriteSize) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gSimpleFileSystemExtensiveTest_AutoAssertionGuid014,
                 L"Extensive Test 6 - compare read/write",
                 L"%a:%d: Length:0x%x, WriteLength:0x%x,LastMatchIndex:0x%x",
                 __FILE__,
                 (UINTN)__LINE__,
                 Length,
                 WriteSize,
                 Loop
                 );

Done:
  //
  // release resources
  //

  if (OpenHandle != NULL) {
    OpenHandle->Close (OpenHandle);
  }

  if (FileHandle != NULL) {
    FileHandle->Delete (FileHandle);
  }

  if (Buffer1 != NULL) {
    gtBS->FreePool (Buffer1);
  }

  if (Buffer2 != NULL) {
    gtBS->FreePool (Buffer2);
  }

  return;
}