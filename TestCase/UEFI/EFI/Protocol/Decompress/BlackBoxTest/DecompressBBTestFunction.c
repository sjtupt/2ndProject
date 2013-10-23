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

  DecompressBBTestFunction.c

Abstract:

  function test source file for Decompress protocol

--*/

#include "DecompressBBTestMain.h"
#include "DecompressBBTestSupport.h"

#define SECTION_NAME_GETINFO_BASIC_TEST      L"GetInfo_Func"
#define SECTION_NAME_DECOMPRESS_BASIC_TEST   L"Decompress_Func"



/**
 *  test the basic funtionality of GetInfo.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
//TDS 4.1.1
//
EFI_STATUS
GetInfo_Func (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STATUS                              Status;
  EFI_DECOMPRESS_PROTOCOL                 *Decompress;
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL      *StandardLib;
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL      *ProfileLib;
  UINT32                                  UncompressedFileSize;
  UINT32                                  CompressedFileSize;
  UINT32                                  DestinationSize;
  UINT32                                  SecondTimeSize;
  UINT32                                  ScratchSize;
  UINT32                                  ScratchSizeOrg;
  UINTN                                   Index;
  VOID                                    *Buffer;
  UINTN                                   BufferSize;
  EFI_TEST_ASSERTION                      AssertionType;
  EFI_INI_FILE_HANDLE                     FileHandle;
  EFI_FILE_HANDLE                         CompressedFHandle;
  UINTN                                   MaxOrder;
  CHAR16                                  *FileName;

  //
  //get tested interface.
  //
  Decompress = (EFI_DECOMPRESS_PROTOCOL *)ClientInterface;

  //
  // Get the test Supported Library Interface
  //
  Status = GetTestSupportLibrary (
             SupportHandle,
             &StandardLib,
             &ProfileLib
             );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get the system device path and file path
  //
  Status = GetSystemData (ProfileLib);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  //open the ini file.
  //
  Status = OpenTestIniFile (ProfileLib, &FileHandle);

  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_DECOMPRESS_PROTOCOL.GetInfo() -no item found for this test case.",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    return Status;
  }

  //
  //we initialize this variable because it was taken as UINT32 type, in fact it
  //is UINT64 value in ipf platform.
  //

  MaxOrder = 0;

  Status = FileHandle->GetOrderNum (
                         FileHandle,
                         SECTION_NAME_GETINFO_BASIC_TEST,
                         (UINT32 *)&MaxOrder
                         );

  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_DECOMPRESS_PROTOCOL.GetInfo() -no item found for this test case",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    CloseTestIniFile (ProfileLib, FileHandle);
    return Status;
  }

  for (Index = 0; Index < MaxOrder; Index++) {
    //
    //get compressed FileName and Uncompressed File Size.
    //
    Status = GetUncompressedFileSize (
               FileHandle,
               SECTION_NAME_GETINFO_BASIC_TEST,
               Index,
               &UncompressedFileSize
               );

    if (EFI_ERROR(Status)) {
      continue;
    }

    Status = GetCompressedFileName (
               FileHandle,
               SECTION_NAME_GETINFO_BASIC_TEST,
               Index,
               &FileName
               );

    if (EFI_ERROR(Status)) {
      continue;
    }

    //
    //read the Compressed file into memory.
    //
    Status = OpenFileAndGetSize (
               FileName,
               &CompressedFHandle,
               &CompressedFileSize
               );

    if (EFI_ERROR(Status)) {
      Print (L"Can not open the File :%s\r\n", FileName);
      gtBS->FreePool (FileName);
      continue;
    }

    gtBS->FreePool (FileName);

    Buffer = AllocatePool (CompressedFileSize);

    if (Buffer == NULL) {
      CompressedFHandle->Close (CompressedFHandle);
      Print (L"Can not allocate %xh buffer.\r\n", CompressedFileSize);
      continue;
    }

    BufferSize = CompressedFileSize;

    Status = CompressedFHandle->Read (
                                  CompressedFHandle,
                                  &BufferSize,
                                  Buffer
                                  );

    if (EFI_ERROR(Status)) {
      CompressedFHandle->Close (CompressedFHandle);
      Print (L"File Read Error Status %r\r\n", Status);
      gtBS->FreePool (Buffer);
      continue;
    }

    //
    //now verify the check points.
    //
    Status = Decompress->GetInfo (
                           Decompress,
                           Buffer,
                           CompressedFileSize,
                           &DestinationSize,
                           &ScratchSize
                           );

    if (Status == EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    //
    //record assertion
    //
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gDecompressBBTestFunctionAssertionGuid001,
                   L"EFI_DECOMPRESS_PROTOCOL.GetInfo() - return Status should be EFI_SUCCESS",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    if (DestinationSize == UncompressedFileSize) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    //
    //record assertion
    //
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gDecompressBBTestFunctionAssertionGuid002,
                   L"EFI_DECOMPRESS_PROTOCOL.GetInfo() - the Destination Size should be equal with the Uncompressed File Size",
                   L"%a:%d:Destination Size - %d, Uncompressed File Size - %d",
                   __FILE__,
                   (UINTN)__LINE__,
                   (UINT32)DestinationSize,
                   (UINT32)UncompressedFileSize
                   );

    //
    //call GetInfo again see if the returned value is the same.
    //
    ScratchSizeOrg = ScratchSize;
    Status = Decompress->GetInfo (
                           Decompress,
                           Buffer,
                           CompressedFileSize,
                           &SecondTimeSize,
                           &ScratchSize
                           );

    if (Status == EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    //
    //record assertion
    //
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gDecompressBBTestFunctionAssertionGuid003,
                   L"EFI_DECOMPRESS_PROTOCOL.GetInfo() - call the second time, return Status should be EFI_SUCCESS",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    if (SecondTimeSize == DestinationSize) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    //
    //record assertion
    //
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gDecompressBBTestFunctionAssertionGuid004,
                   L"EFI_DECOMPRESS_PROTOCOL.GetInfo() - two time returned Destination size must be equal",
                   L"%a:%d:First Time - %08x, Second Time- %08x",
                   __FILE__,
                   (UINTN)__LINE__,
                   DestinationSize,
                   SecondTimeSize
                   );

    if (ScratchSize == ScratchSizeOrg) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    //
    //record assertion
    //
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gDecompressBBTestFunctionAssertionGuid005,
                   L"EFI_DECOMPRESS_PROTOCOL.GetInfo() - two time returned Scratch size must be equal",
                   L"%a:%d:First Time - %08x, Second Time- %08x",
                   __FILE__,
                   (UINTN)__LINE__,
                   ScratchSizeOrg,
                   ScratchSize
                   );

    //
    //free the resources
    //
    gtBS->FreePool (Buffer);
    CompressedFHandle->Close (CompressedFHandle);
  }

  CloseTestIniFile (ProfileLib, FileHandle);

  //
  //done successfully
  //
  return EFI_SUCCESS;
}

/**
 *  test the basic funtionality of Decompress.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
//TDS 4.1.2
//
EFI_STATUS
Decompress_Func (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STATUS                              Status;
  EFI_DECOMPRESS_PROTOCOL                 *Decompress;
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL      *StandardLib;
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL      *ProfileLib;
  UINT32                                  UncompressedFileSize;
  UINT32                                  CompressedFileSize;
  UINT32                                  DestinationSize;
  UINT32                                  ScratchSize;
  UINTN                                   Index;
  UINTN                                   SubIndex;
  VOID                                    *CompressedFileBuffer;
  VOID                                    *UncompressedFileBuffer;
  UINT8                                   *DecompressBuffer;
  VOID                                    *ScratchBuffer;
  UINTN                                   BufferSize;
  EFI_TEST_ASSERTION                      AssertionType;
  EFI_INI_FILE_HANDLE                     FileHandle;
  EFI_FILE_HANDLE                         CompressedFHandle;
  EFI_FILE_HANDLE                         UncompressedFHandle;
  UINTN                                   MaxOrder;
  CHAR16                                  *CompressedFileName;
  CHAR16                                  *UncompressedFileName;

  //
  //get tested interface.
  //
  Decompress = (EFI_DECOMPRESS_PROTOCOL *)ClientInterface;

  //
  // Get the test Supported Library Interface
  //
  Status = GetTestSupportLibrary (
             SupportHandle,
             &StandardLib,
             &ProfileLib
             );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get the system device path and file path
  //
  Status = GetSystemData (ProfileLib);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  //open the ini file.
  //
  Status = OpenTestIniFile (ProfileLib, &FileHandle);

  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_DECOMPRESS_PROTOCOL.Decompress() -not found the profile.",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  //
  //we initialize this variable because it was taken as UINT32 type, in fact it
  //is UINT64 value in ipf platform.
  //

  MaxOrder = 0;

  Status = FileHandle->GetOrderNum (
                         FileHandle,
                         SECTION_NAME_DECOMPRESS_BASIC_TEST,
                         (UINT32 *)&MaxOrder
                         );

  if (EFI_ERROR(Status)) {
    CloseTestIniFile (ProfileLib, FileHandle);
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_DECOMPRESS_PROTOCOL.Decompress() -no item found for this test case.",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  for (Index = 0; Index < MaxOrder; Index++) {

    CompressedFileBuffer   = NULL;
    UncompressedFileBuffer = NULL;
    DecompressBuffer       = NULL;
    ScratchBuffer          = NULL;

    //
    //get compressed FileName and file size.
    //
    Status = GetCompressedFileName (
               FileHandle,
               SECTION_NAME_DECOMPRESS_BASIC_TEST,
               Index,
               &CompressedFileName
               );

    if (EFI_ERROR(Status)) {
      Print (L"Get CompressedFileName Error\r\n");
      continue;
    }

    Status = GetUncompressedFileName (
               FileHandle,
               SECTION_NAME_DECOMPRESS_BASIC_TEST,
               Index,
               &UncompressedFileName
               );

    if (EFI_ERROR(Status)) {
      gtBS->FreePool (CompressedFileName);
      Print (L"Get UncompressedFileName Error\r\n");
      continue;
    }

    //
    //Open the Compressed file and get file size.
    //
    Status = OpenFileAndGetSize (
               CompressedFileName,
               &CompressedFHandle,
               &CompressedFileSize
               );

    if (EFI_ERROR(Status)) {
      Print (L"Can not open the File :%s\r\n", CompressedFileName);
      gtBS->FreePool (CompressedFileName);
      gtBS->FreePool (UncompressedFileName);
      continue;
    }
    gtBS->FreePool (CompressedFileName);

    //
    //Open the Uncompressed file and get file size.
    //
    Status = OpenFileAndGetSize (
               UncompressedFileName,
               &UncompressedFHandle,
               &UncompressedFileSize
               );

    if (EFI_ERROR(Status)) {
      Print (L"Can not open the File :%s\r\n", UncompressedFileName);
      CompressedFHandle->Close (CompressedFHandle);
      gtBS->FreePool (UncompressedFileName);
      continue;
    }
    gtBS->FreePool (UncompressedFileName);

    //
    //then read the Compressed File into memory.
    //
    CompressedFileBuffer = AllocatePool (CompressedFileSize);

    if (CompressedFileBuffer == NULL) {
      CompressedFHandle->Close (CompressedFHandle);
      UncompressedFHandle->Close (UncompressedFHandle);
      Print (L"Can not allocate %xh buffer.\r\n", CompressedFileSize);
      continue;
    }

    BufferSize = CompressedFileSize;

    Status = CompressedFHandle->Read (
                                  CompressedFHandle,
                                  &BufferSize,
                                  CompressedFileBuffer
                                  );

    CompressedFHandle->Close (CompressedFHandle);

    if (EFI_ERROR(Status)) {
      UncompressedFHandle->Close (UncompressedFHandle);
      Print (L"File Read Error Status %r\r\n", Status);
      gtBS->FreePool (CompressedFileBuffer);
      continue;
    }

    if (UncompressedFileSize == 0) {

      UncompressedFHandle->Close (UncompressedFHandle);

      Status = Decompress->GetInfo (
                             Decompress,
                             CompressedFileBuffer,
                             CompressedFileSize,
                             &DestinationSize,
                             &ScratchSize
                             );

      if (Status == EFI_SUCCESS) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gTestGenericFailureGuid,
                     L"EFI_DECOMPRESS_PROTOCOL.GetInfo() - return Status Should be EFI_SUCCESS",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );

      if (DestinationSize == 0) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gTestGenericFailureGuid,
                     L"EFI_DECOMPRESS_PROTOCOL.GetInfo() - the Destination Size should be equal with the Uncompressed File Size",
                     L"%a:%d:destination Size - %d, Uncompressed File Size - %d",
                     __FILE__,
                     (UINTN)__LINE__,
                     (UINT32)DestinationSize,
                     (UINT32)UncompressedFileSize
                     );


      //
      // Fill the buffer with 0x00, and check whether the buffer is changed later
      //
      DecompressBuffer = AllocateZeroPool (128);

      if (DecompressBuffer == NULL) {
        gtBS->FreePool (CompressedFileBuffer);
        Print (L"Can not allocate %xh buffer.\r\n", 128);
        continue;
      }

      ScratchBuffer = AllocatePool (ScratchSize);

      if (ScratchBuffer == NULL) {
        gtBS->FreePool (CompressedFileBuffer);
        gtBS->FreePool (DecompressBuffer);
        Print (L"Can not allocate %xh buffer.\r\n", ScratchSize);
        continue;
      }

      Status = Decompress->Decompress (
                             Decompress,
                             CompressedFileBuffer,
                             CompressedFileSize,
                             DecompressBuffer,
                             DestinationSize,
                             ScratchBuffer,
                             ScratchSize
                             );

      if (!EFI_ERROR(Status)) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }
      //
      //record assertion
      //
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gDecompressBBTestFunctionAssertionGuid007,
                     L"EFI_DECOMPRESS_PROTOCOL.Decompess() - decompress zero file, return Status should be EFI_SUCCESS",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );

      AssertionType = EFI_TEST_ASSERTION_PASSED;

      //
      // Check whether the buffer is changed
      //
      for (SubIndex = 0; SubIndex < 128; SubIndex++) {
        if (DecompressBuffer[SubIndex] != (UINT8)0x00) {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
          break;
        }
      }
      SubIndex = (SubIndex < 127) ? SubIndex : 127;
      //
      //record assertion
      //
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gDecompressBBTestFunctionAssertionGuid008,
                     L"EFI_DECOMPRESS_PROTOCOL.Decompess() - Decompressed zero length file, the buffer should not be modified",
                     L"%a:%d:buffer - 0x%02x, expected - 0x00",
                     __FILE__,
                     (UINTN)__LINE__,
                     DecompressBuffer[SubIndex]
                     );

      //
      // Fill the buffer with 0xff, and check whether the buffer is changed later
      //
      for (SubIndex = 0; SubIndex < 128; SubIndex++) {
        DecompressBuffer[SubIndex] = (UINT8)0xff;
      }

      Status = Decompress->Decompress (
                             Decompress,
                             CompressedFileBuffer,
                             CompressedFileSize,
                             DecompressBuffer,
                             DestinationSize,
                             ScratchBuffer,
                             ScratchSize
                             );

      if (!EFI_ERROR(Status)) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }
      //
      //record assertion
      //
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gDecompressBBTestFunctionAssertionGuid007,
                     L"EFI_DECOMPRESS_PROTOCOL.Decompess() - decompress zero file, return Status should be EFI_SUCCESS",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );

      AssertionType = EFI_TEST_ASSERTION_PASSED;

      //
      // Check whether the buffer is changed
      //
      for (SubIndex = 0; SubIndex < 128; SubIndex++) {
        if (DecompressBuffer[SubIndex] != (UINT8)0xff) {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
          break;
        }
      }
      SubIndex = (SubIndex < 127) ? SubIndex : 127;
      //
      //record assertion
      //
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gDecompressBBTestFunctionAssertionGuid008,
                     L"EFI_DECOMPRESS_PROTOCOL.Decompess() - Decompressed zero length file, the buffer should not be modified",
                     L"%a:%d:buffer - 0x%02x, expected - 0xff",
                     __FILE__,
                     (UINTN)__LINE__,
                     DecompressBuffer[SubIndex]
                     );

      //
      //free the resources.
      //
      gtBS->FreePool (CompressedFileBuffer);
      gtBS->FreePool (DecompressBuffer);
      gtBS->FreePool (ScratchBuffer);
      continue;
    }

    //
    //the Uncompressed file is not null so read it into memory.
    //
    UncompressedFileBuffer = AllocatePool (UncompressedFileSize);

    if (UncompressedFileBuffer == NULL) {
      UncompressedFHandle->Close (UncompressedFHandle);
      gtBS->FreePool (CompressedFileBuffer);
      Print (L"Can not alloate %xh buffer.\r\n", UncompressedFileSize);
      continue;
    }

    BufferSize = UncompressedFileSize;

    Status = UncompressedFHandle->Read (
                                    UncompressedFHandle,
                                    &BufferSize,
                                    UncompressedFileBuffer
                                    );

    UncompressedFHandle->Close (UncompressedFHandle);

    if (EFI_ERROR(Status)) {
      gtBS->FreePool (CompressedFileBuffer);
      gtBS->FreePool (UncompressedFileBuffer);
      Print (L"File Read Error Status %r\r\n", Status);
      continue;
    }

    //
    //now verify the check points.
    //

    Status = Decompress->GetInfo (
                           Decompress,
                           CompressedFileBuffer,
                           CompressedFileSize,
                           &DestinationSize,
                           &ScratchSize
                           );

    if (Status == EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gTestGenericFailureGuid,
                   L"EFI_DECOMPRESS_PROTOCOL.GetInfo() - return Status should be EFI_SUCCESS",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    if (DestinationSize == UncompressedFileSize) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gTestGenericFailureGuid,
                   L"EFI_DECOMPRESS_PROTOCOL.GetInfo() - the Destination Size should be equal with the Uncompressed File Size",
                   L"%a:%d:destination Size - %d, Uncompressed File Size - %d",
                   __FILE__,
                   (UINTN)__LINE__,
                   (UINT32)DestinationSize,
                   (UINT32)UncompressedFileSize
                   );

    //
    //allocate Decompess buffer and Decompress the compessed data.
    //
    DecompressBuffer = AllocatePool (DestinationSize);
    if (DecompressBuffer == NULL) {
      gtBS->FreePool (CompressedFileBuffer);
      gtBS->FreePool (UncompressedFileBuffer);
      Print (L"Can not allocate %xh buffer.\r\n", DestinationSize);
      continue;
    }

    ScratchBuffer = AllocatePool (ScratchSize);
    if (ScratchBuffer == NULL) {
      gtBS->FreePool (CompressedFileBuffer);
      gtBS->FreePool (UncompressedFileBuffer);
      gtBS->FreePool (DecompressBuffer);
      Print (L"Can not allocate %xh buffer.\r\n", ScratchSize);
      continue;
    }

    Status = Decompress->Decompress (
                           Decompress,
                           CompressedFileBuffer,
                           CompressedFileSize,
                           DecompressBuffer,
                           DestinationSize,
                           ScratchBuffer,
                           ScratchSize
                           );

    if (!EFI_ERROR(Status)) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    //
    //record assertion
    //
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gDecompressBBTestFunctionAssertionGuid009,
                   L"EFI_DECOMPRESS_PROTOCOL.Decompess() - decompress unzero file, return Status should be EFI_SUCCESS",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    if (CompareMem (DecompressBuffer,
                    UncompressedFileBuffer, UncompressedFileSize) == 0) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    //
    //record assertion
    //
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gDecompressBBTestFunctionAssertionGuid010,
                   L"EFI_DECOMPRESS_PROTOCOL.Decompess() - Decompressed data should be equal with Uncompressed data",
                   L"%a:%d",
                   __FILE__,
                   (UINTN)__LINE__
                   );

    //
    //free the resources
    //
    gtBS->FreePool (CompressedFileBuffer);
    gtBS->FreePool (UncompressedFileBuffer);
    gtBS->FreePool (DecompressBuffer);
    gtBS->FreePool (ScratchBuffer);
  }

  CloseTestIniFile (ProfileLib, FileHandle);

  //
  //done successfully
  //
  return EFI_SUCCESS;
}
