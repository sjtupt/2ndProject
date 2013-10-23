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

  BlockIoBBTestConformance.c

Abstract:

  Consistency Test Cases of Block I/O Protocol

--*/


#include "BlockIoBBTestMain.h"


/**
 *  Entrypoint for EFI_BLOCK_IO_PROTOCOL.ReadBlocks() Consistency Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 4.1
//
EFI_STATUS
BBTestReadBlocksConformanceAutoTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib;
  EFI_STATUS                           Status, FailStatus;
  EFI_BLOCK_IO_PROTOCOL                *BlockIo;
  EFI_TEST_ASSERTION                   AssertionType;
  UINT32                               MediaId;
  BOOLEAN                              RemovableMedia;
  BOOLEAN                              MediaPresent;
  BOOLEAN                              LogicalPartition;
  BOOLEAN                              ReadOnly;
  BOOLEAN                              WriteCaching;
  UINT32                               BlockSize;
  UINT32                               IoAlign;
  EFI_LBA                              LastBlock;

  UINTN                                BufferSize;
  UINT8                                *Buffer = NULL;

  UINT32                               BlockNumber;

  UINTN                                Index;
  UINT32                               NewMediaId;
  UINTN                                NewBufferSize;
  EFI_LBA                              NewLba;
  UINT32                               NewIoAlign;

  EFI_DEVICE_PATH_PROTOCOL             *DevicePath;

  CHAR16                               *DevicePathStr;
  EFI_STRING                           ErrorOutput = L"";


  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );

  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"BS.HandleProtocol - Handle standard test library",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  BlockIo = (EFI_BLOCK_IO_PROTOCOL *)ClientInterface;

  //
  // Locate & record DevicePath for further investigation
  //
  LocateDevicePathFromBlockIo (BlockIo, &DevicePath, StandardLib);

  DevicePathStr = DevicePathToStr (DevicePath);
  if (DevicePathStr != NULL) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_DEFAULT,
                   L"Current Device: %s",
                   DevicePathStr
                   );

    Status = gtBS->FreePool (DevicePathStr);
    if (EFI_ERROR(Status)) {
      StandardLib->RecordAssertion (
                     StandardLib,
                     EFI_TEST_ASSERTION_FAILED,
                     gTestGenericFailureGuid,
                     L"BS.FreePool - Free device path string",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
      return Status;
    }

    DevicePathStr = NULL;
  }


  //
  // Initialize variable
  //
  MediaId           = BlockIo->Media->MediaId;
  RemovableMedia    = BlockIo->Media->RemovableMedia;
  MediaPresent      = BlockIo->Media->MediaPresent;
  LogicalPartition  = BlockIo->Media->LogicalPartition;
  ReadOnly          = BlockIo->Media->ReadOnly;
  WriteCaching      = BlockIo->Media->WriteCaching;
  BlockSize         = BlockIo->Media->BlockSize;
  IoAlign           = BlockIo->Media->IoAlign;
  LastBlock         = BlockIo->Media->LastBlock;

  BlockNumber       = (UINT32) MINIMUM(LastBlock, MAX_NUMBER_OF_READ_BLOCK_BUFFER);
  BufferSize        = BlockNumber * BlockSize;

  if (BufferSize == 0) {
    BufferSize = BlockSize;
  }

  //
  // allocate aligned buffer
  //
  Buffer = AllocateAlignedPool(
             EfiBootServicesData, 
             BufferSize, 
             IoAlign
             );
  if (Buffer == NULL) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"AllocateAlignedPool - Allocate aligned buffer for testing",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   EFI_OUT_OF_RESOURCES
                   );
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Assertion Points 4.1.2.1
  // ReadBlocks should not succeed to read from device
  // When MedialId is not the id for the current media in the device
  //

  if (MediaPresent == TRUE) {
    for (Index = 0; Index < 5; Index++) {
      //
      // initialize test data
      //
      switch (Index) {
      case 0:
        NewMediaId = MediaId + 5;
        break;
      case 1:
        NewMediaId = MediaId + 1;
        break;
      case 2:
        NewMediaId = MediaId - 1;
        break;
      case 3:
        NewMediaId = MediaId - 5;
        break;
      case 4:
        NewMediaId = 0;
        break;
      default:
        NewMediaId = MediaId;
      }

      //
      // Verify test data
      //
      if (NewMediaId == MediaId) {
        continue;
      }

      //
      // Call ReadBlocks with false MediaID
      //
      Status = BlockIo->ReadBlocks (
                          BlockIo,
                          NewMediaId,
                          0,
                          BufferSize,
                          (VOID*)Buffer
                          );
      FailStatus = Status;
      if (Status != EFI_MEDIA_CHANGED) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        ErrorOutput = L"False MediaID";
      } else {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      }

      //
      // Call ReadBlocks with false MediaID & invalid LBA
      //
      Status = BlockIo->ReadBlocks (
                          BlockIo,
                          NewMediaId,
                          LastBlock + 1,
                          BufferSize,
                          (VOID*)Buffer
                          );
      if (Status != EFI_MEDIA_CHANGED) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        FailStatus = Status;
        ErrorOutput = L"False MediaID & Invalid LBA";
      }
      
      //
      // Call ReadBlocks with false MediaID & invalid BufferSize
      //
      Status = BlockIo->ReadBlocks (
                          BlockIo,
                          NewMediaId,
                          0,
                          BufferSize - 1,
                          (VOID*)Buffer
                          );
      if (Status != EFI_MEDIA_CHANGED) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        FailStatus = Status;
        ErrorOutput = L"False MediaID & Invalid BufferSize";
      }

      //
      // Call ReadBlocks with false MediaID & invalid 0 BufferSize
      //
      Status = BlockIo->ReadBlocks (
                          BlockIo,
                          NewMediaId,
                          0,
                          0,
                          (VOID*)Buffer
                          );
      if (Status != EFI_MEDIA_CHANGED) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        FailStatus = Status;
        ErrorOutput = L"False MediaID & Invalid 0 BufferSize";
      }
      
      //
      // Call ReadBlocks with false MediaID & wrong aligned Buffer
      //
      if (IoAlign > 1) {
        Status = BlockIo->ReadBlocks (
                            BlockIo,
                            NewMediaId,
                            0,
                            BufferSize - BlockSize,
                            (VOID*)(Buffer + 1)
                            );
        if (Status != EFI_MEDIA_CHANGED) {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
          FailStatus = Status;
          ErrorOutput = L"False MediaID & Wrong aligned Buffer";
        }
      }

      //
      // Call ReadBlocks with false MediaID & invalid NULL Buffer
      //
      Status = BlockIo->ReadBlocks (
                          BlockIo,
                          NewMediaId,
                          0,
                          BufferSize,
                          NULL
                          );
      if (Status != EFI_MEDIA_CHANGED) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        FailStatus = Status;
        ErrorOutput = L"false MediaID & Invalid NULL Buffer";
      }
      
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gBlockIoConformanceTestAssertionGuid001,
                     L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block with MediaId not being the id for the current media in the device",
                     L"%a:%d: %s Actual MediaID=%d, MediaID=%d, Status=%r, Expected=%r",
                     __FILE__,
                     (UINTN)__LINE__,
                     ErrorOutput,
                     MediaId,
                     NewMediaId,
                     FailStatus,
                     EFI_MEDIA_CHANGED
                     );
    }
  }

  //
  // Assertion point 4.1.2.2
  // ReadBlocks should not suceed to read from device when BufferSize is not
  // a multiple of the intrinsic block size of the device
  //
  if ((MediaPresent == TRUE) && (BlockSize != 1)) {
    for (Index = 0; Index < 4; Index++) {
      //
      // initialize test data
      //
      switch (Index) {
      case 0:
        NewBufferSize = BlockSize + 1;
        break;
      case 1:
        NewBufferSize = 2 * BlockSize - 1;
        break;
      case 2:
        NewBufferSize = 2 * BlockSize + 1;
        break;
      case 3:
        NewBufferSize = 3 * BlockSize - 1;
        break;
      default:
        NewBufferSize = BufferSize;
        break;
      }

      //
      // Verify test data
      //
      if (((NewBufferSize % BlockSize) == 0) || (NewBufferSize > BufferSize)) {
        continue;
      }

      //
      // Call ReadBlocks with invalid BufferSize
      //
      Status = BlockIo->ReadBlocks (
                          BlockIo,
                          MediaId,
                          0,
                          NewBufferSize,
                          (VOID*)Buffer
                          );
      if (Status != EFI_BAD_BUFFER_SIZE) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      }

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gBlockIoConformanceTestAssertionGuid002,
                     L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block with BufferSize not being a multiple of the intrinsic block size of the device",
                     L"%a:%d:BufferSize=%d, BlockSize=%d, Status=%r, Expected=%r",
                     __FILE__,
                     (UINTN)__LINE__,
                     NewBufferSize,
                     BlockSize,
                     Status,
                     EFI_BAD_BUFFER_SIZE
                     );
    }
  }

  //
  // Assertion Point 4.1.2.3
  // ReadBlocks should not succeed to read from device with invalid LBA parameter
  //
  if (MediaPresent == TRUE) {
    for (Index = 0; Index < 6; Index++) {
      //
      // initialize test data
      //
      switch (Index) {
      case 0:
        NewLba = LastBlock + 1;
        break;
      case 1:
        NewLba = LastBlock + 100;
        break;
      case 2:
        NewLba = LastBlock - BufferSize/BlockSize + 1;
        break;
      case 3:
        NewLba = LastBlock - BufferSize/BlockSize + 2;
        break;
      case 4:
        NewLba = LastBlock - BufferSize/BlockSize + 3;
        break;
      case 5:
        NewLba = LastBlock - BufferSize/BlockSize + 100;
        break;
      default:
        NewLba = 0;
        break;
      }

      //
      // Verify test data
      //
      if ((NewLba + BufferSize/BlockSize) <= (LastBlock + 1)) {
        continue;
      }

      //
      // Call ReadBlocks with invalid LBA
      //
      Status = BlockIo->ReadBlocks (
                          BlockIo,
                          MediaId,
                          NewLba,
                          BufferSize,
                          (VOID*)Buffer
                          );
      if (Status != EFI_INVALID_PARAMETER) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      }

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gBlockIoConformanceTestAssertionGuid003,
                     L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block with invalid LBA parameter",
                     L"%a:%d:BufferSize=%d, BlockSize=%d, LBA=0x%lx, LastBlock=0x%lx, Status=%r, Expected=%r",
                     __FILE__,
                     (UINTN)__LINE__,
                     BufferSize,
                     BlockSize,
                     NewLba,
                     LastBlock,
                     Status,
                     EFI_INVALID_PARAMETER
                     );
    }
  }

  //
  // Assertion Point 4.1.2.4
  // ReadBlocks should not succeed to read from device that with no media present in the device
  //
  if (MediaPresent == FALSE) {
    ErrorOutput = L"";

    //
    // Call ReadBlocks with proper parameter
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    FailStatus = Status;
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    //
    // Call ReadBlocks with invalid LBA
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        LastBlock + 1,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
      FailStatus = Status;
      ErrorOutput = L"Invalid LBA";
    }
    
    //
    // Call ReadBlocks with invalid BufferSize
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize - 1,
                        (VOID*)Buffer
                        );
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
      FailStatus = Status;
      ErrorOutput = L"Invalid BufferSize";
    }

    //
    // Call ReadBlocks with invalid 0 BufferSize
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        0,
                        (VOID*)Buffer
                        );
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
      FailStatus = Status;
      ErrorOutput = L"Zero BufferSize";
    }

    //
    // Call ReadBlocks with unaligned Buffer
    //
    if (IoAlign > 1) {
      Status = BlockIo->ReadBlocks (
                          BlockIo,
                          MediaId,
                          0,
                          BufferSize - BlockSize,
                          (VOID*)(Buffer + 1)
                          );
      if (Status != EFI_NO_MEDIA) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        FailStatus = Status;
        ErrorOutput = L"Unaligned Buffer";
      }
    }

    //
    // Call ReadBlocks with invalid NULL Buffer
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        NULL
                        );
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
      FailStatus = Status;
      ErrorOutput = L"NULL Buffer";
    }
      
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid006,
                   L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block from device without media present in the device",
                   L"%a:%d: %s BufferSize=%d, Status=%r, Expected=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   ErrorOutput,
                   BufferSize,
                   FailStatus,
                   EFI_NO_MEDIA
                   );
  }

  //
  // Assertion Point 4.1.2.5
  // ReadBlocks should not succed to read from device with Buffer is not on proper alignment
  //
  if ((MediaPresent == TRUE) && (IoAlign > 1)) {
    //
    // if IoAlign is large than 1, then it must be a power of 2
    //
    if ((IoAlign & (IoAlign - 1)) == 0) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid031,
                   L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block aligment should be power of 2",
                   L"%a:%d:Aligment=0x%lx",
                   __FILE__,
                   (UINTN)__LINE__,
                   IoAlign
                   );
   
    for (NewIoAlign = (IoAlign >> 1); NewIoAlign > 0 ; NewIoAlign >>= 1) {
      //
      // Call ReadBlocks with buffer of wrong aligment 
      //
      Status = BlockIo->ReadBlocks (
                          BlockIo,
                          MediaId,
                          0,
                          BlockSize,
                          (VOID*)(Buffer + NewIoAlign)
                          );
      if (Status == EFI_INVALID_PARAMETER) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }
    
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gBlockIoConformanceTestAssertionGuid009,
                     L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block with unaligned data buffer",
                     L"%a:%d:Aligment=%d, Status=%r,Expected=%r",
                     __FILE__,
                     (UINTN)__LINE__,
                     NewIoAlign,
                     Status,
                     EFI_INVALID_PARAMETER
                     );
    }
  }

  //
  // Free aligned buffer
  //
  Status = FreeAlignedPool (Buffer);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"FreeAlignedPool - Free aligned buffer for testing",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_BLOCK_IO_PROTOCOL.ReadBlocks() Manually Consistency Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 4.2
//
EFI_STATUS
BBTestReadBlocksConformanceManualTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib;
  EFI_STATUS                           Status;
  EFI_BLOCK_IO_PROTOCOL                *BlockIo;
  EFI_TEST_ASSERTION                   AssertionType;
  UINT32                               MediaId;
  BOOLEAN                              RemovableMedia;
  BOOLEAN                              MediaPresent;
  BOOLEAN                              LogicalPartition;
  BOOLEAN                              ReadOnly;
  BOOLEAN                              WriteCaching;
  UINT32                               BlockSize;
  UINT32                               IoAlign;
  EFI_LBA                              LastBlock;

  UINTN                                BufferSize;
  UINT8                                *Buffer;

  UINT32                               BlockNumber;

  EFI_DEVICE_PATH_PROTOCOL             *DevicePath;
  CHAR16                               *DevicePathStr;


  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );

  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"BS.HandleProtocol - Handle standard test library",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  BlockIo = (EFI_BLOCK_IO_PROTOCOL *)ClientInterface;

  //
  // Locate & record DevicePath for further investigation
  //
  LocateDevicePathFromBlockIo (BlockIo, &DevicePath, StandardLib);

  DevicePathStr = DevicePathToStr (DevicePath);
  if (DevicePathStr != NULL) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_DEFAULT,
                   L"Current Device: %s",
                   DevicePathStr
                   );

    Status = gtBS->FreePool (DevicePathStr);
    if (EFI_ERROR(Status)) {
      StandardLib->RecordAssertion (
                     StandardLib,
                     EFI_TEST_ASSERTION_FAILED,
                     gTestGenericFailureGuid,
                     L"BS.FreePool - Free device path string",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
      return Status;
    }

    DevicePathStr = NULL;
  }


  //
  // Initialize variable
  //
  MediaId           = BlockIo->Media->MediaId;
  RemovableMedia    = BlockIo->Media->RemovableMedia;
  MediaPresent      = BlockIo->Media->MediaPresent;
  LogicalPartition  = BlockIo->Media->LogicalPartition;
  ReadOnly          = BlockIo->Media->ReadOnly;
  WriteCaching      = BlockIo->Media->WriteCaching;
  BlockSize         = BlockIo->Media->BlockSize;
  IoAlign           = BlockIo->Media->IoAlign;
  LastBlock         = BlockIo->Media->LastBlock;

  BlockNumber       = (UINT32) MINIMUM(LastBlock, MAX_NUMBER_OF_READ_BLOCK_BUFFER);
  BufferSize        = BlockNumber * BlockSize;

  if (BufferSize == 0) {
    BufferSize = BlockSize;
  }

  //
  // allocate buffer
  //
  Status = gtBS->AllocatePool (EfiBootServicesData, BufferSize, &Buffer);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"BS.AllocatePool - Allocate buffer for testing",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  //
  // Assertion Points 4.2.2.1
  // ReadBlocks should not succeed to read from device
  // When MedialId is not the id for the current media in the device
  //
  if ((MediaPresent == TRUE) && (RemovableMedia == TRUE)) {
    //
    // Call ReadBlocks with valid MediaID and parameter
    // to verify the success of reading from device
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (EFI_ERROR(Status)) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid010,
                   L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block from device",
                   L"%a:%d:BufferSize=%d, Lba=0, Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   BufferSize,
                   Status
                   );
    if (EFI_ERROR(Status)) {
      goto AssertionPoints2;
    }

    //
    // Display instruction to tell user change the media
    //
    Print (L"Change media in device ");

    DevicePathStr = DevicePathToStr (DevicePath);
    if (DevicePathStr != NULL) {
      Print (DevicePathStr);

      Status = gtBS->FreePool (DevicePathStr);
      if (EFI_ERROR(Status)) {
        StandardLib->RecordAssertion (
                       StandardLib,
                       EFI_TEST_ASSERTION_FAILED,
                       gTestGenericFailureGuid,
                       L"BS.FreePool - Free pool",
                       L"%a:%d:Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );
        goto FreeBuffer;
      }

      DevicePathStr = NULL;
    }

    if (AutoJudge (50, FALSE, L"\r\nReady?\r\n") != TRUE) {
      goto AssertionPoints2;
    }

    //
    // wait for 0.5s for re-install BlockIo interface
    //
    gtBS->Stall (500000);

    //
    // Call ReadBlocks with the same MediaID and same parameter again
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (Status != EFI_MEDIA_CHANGED) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid011,
                   L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block with MediaId not being the id for the current media in the device",
                   L"%a:%d:ActualMediaID=%d, MediaID=%d, BufferSize=%d, Lba=0, Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   BlockIo->Media->MediaId,
                   MediaId,
                   BufferSize,
                   Status
                   );

    //
    // Begin to re-locate the BlockIo interface according to the DevicePath
    //
    LocateBlockIoFromDevicePath (&BlockIo, DevicePath, StandardLib);

    //
    // Initialize variable
    //
    MediaId           = BlockIo->Media->MediaId;
    RemovableMedia    = BlockIo->Media->RemovableMedia;
    MediaPresent      = BlockIo->Media->MediaPresent;
    LogicalPartition  = BlockIo->Media->LogicalPartition;
    ReadOnly          = BlockIo->Media->ReadOnly;
    WriteCaching      = BlockIo->Media->WriteCaching;
    BlockSize         = BlockIo->Media->BlockSize;
    IoAlign           = BlockIo->Media->IoAlign;
    LastBlock         = BlockIo->Media->LastBlock;
  }

AssertionPoints2:
  //
  // Assertion Points 4.2.2.2 ReadBlocks should not succeed to
  // read from device with no media in the device
  //
  if ((MediaPresent == TRUE) && (RemovableMedia == TRUE)) {
    //
    // Call ReadBlocks with valid MediaID and parameter
    // to verify the success of reading from device
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (EFI_ERROR(Status)) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid012,
                   L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block from device",
                   L"%a:%d:BufferSize=%d, Lba=0, Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   BufferSize,
                   Status
                   );
    if (EFI_ERROR(Status)) {
      goto FreeBuffer;
    }

    //
    // Display instruction to tell user remove the media
    //
    Print (L"Remove media in device ");
    DevicePathStr = DevicePathToStr (DevicePath);
    if (DevicePathStr != NULL) {
      Print (DevicePathStr);
      Status = gtBS->FreePool (DevicePathStr);
      if (EFI_ERROR(Status)) {
        StandardLib->RecordAssertion (
                       StandardLib,
                       EFI_TEST_ASSERTION_FAILED,
                       gTestGenericFailureGuid,
                       L"BS.FreePool - Free device path string",
                       L"%a:%d:Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );
        goto FreeBuffer;
      }

      DevicePathStr = NULL;
    }

    if (AutoJudge (50, FALSE, L"\r\nReady?\r\n") != TRUE) {
      goto FreeBuffer;
    }

    //
    // wait for 0.5s for re-install BlockIo interface
    //
    gtBS->Stall (500000);

    //
    // Call ReadBlocks with the same MediaID and same parameter again
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid013,
                   L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block with no media in the device",
                   L"%a:%d:ActualMediaID=%d, MediaID=%d, BufferSize=%d, Lba=0, Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   BlockIo->Media->MediaId,
                   MediaId,
                   BufferSize,
                   Status
                   );

    //
    // Display instruction to tell user remove the media
    //
    Print (L"Re-insert media into device ");

    DevicePathStr = DevicePathToStr (DevicePath);
    if (DevicePathStr != NULL) {
      Print (DevicePathStr);

      Status = gtBS->FreePool (DevicePathStr);
      if (EFI_ERROR(Status)) {
        StandardLib->RecordAssertion (
                       StandardLib,
                       EFI_TEST_ASSERTION_FAILED,
                       gTestGenericFailureGuid,
                       L"BS.FreePool - Free device path string",
                       L"%a:%d:Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );
        goto FreeBuffer;
      }

      DevicePathStr = NULL;
    }

    if (AutoJudge (50, FALSE, L"\r\nReady?\r\n") != TRUE) {
      goto FreeBuffer;
    }

    //
    // Call ReadBlocks to force re-install device
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BlockSize,
                        (VOID*)Buffer
                        );
    if (EFI_ERROR(Status)) {
      //
      // No Status check here.
      //
    }
  }

FreeBuffer:
  Status = gtBS->FreePool (Buffer);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"BS.FreePool - Free buffer for testing",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_BLOCK_IO_PROTOCOL.WriteBlocks() Consistency Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 4.3
//
EFI_STATUS
BBTestWriteBlocksConformanceAutoTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib;
  EFI_STATUS                           Status, FailStatus;
  EFI_BLOCK_IO_PROTOCOL                *BlockIo;
  EFI_TEST_ASSERTION                   AssertionType;
  UINT32                               MediaId;
  BOOLEAN                              RemovableMedia;
  BOOLEAN                              MediaPresent;
  BOOLEAN                              LogicalPartition;
  BOOLEAN                              ReadOnly;
  BOOLEAN                              WriteCaching;
  UINT32                               BlockSize;
  UINT32                               IoAlign;
  EFI_LBA                              LastBlock;

  UINTN                                BufferSize;
  UINT8                                *Buffer = NULL;


  UINT32                               BlockNumber;

  UINTN                                Index;
  UINT32                               NewMediaId;
  UINTN                                NewBufferSize;
  EFI_LBA                              NewLba;
  UINT32                               NewIoAlign;

  EFI_DEVICE_PATH_PROTOCOL             *DevicePath;

  CHAR16                               *DevicePathStr;
  EFI_STRING                           ErrorOutput = L"";


  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );

  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"BS.HandleProtocol - Handle standard test library",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  BlockIo = (EFI_BLOCK_IO_PROTOCOL *)ClientInterface;

  //
  // Locate & record DevicePath for further investigation
  //
  LocateDevicePathFromBlockIo (BlockIo, &DevicePath, StandardLib);

  DevicePathStr = DevicePathToStr (DevicePath);
  if (DevicePathStr != NULL) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_DEFAULT,
                   L"Current Device: %s",
                   DevicePathStr
                   );

    Status = gtBS->FreePool (DevicePathStr);
    if (EFI_ERROR(Status)) {
      StandardLib->RecordAssertion (
                     StandardLib,
                     EFI_TEST_ASSERTION_FAILED,
                     gTestGenericFailureGuid,
                     L"BS.FreePool - Free device path string",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
      return Status;
    }

    DevicePathStr = NULL;
  }

  //
  // Initialize variable
  //
  MediaId           = BlockIo->Media->MediaId;
  RemovableMedia    = BlockIo->Media->RemovableMedia;
  MediaPresent      = BlockIo->Media->MediaPresent;
  LogicalPartition  = BlockIo->Media->LogicalPartition;
  ReadOnly          = BlockIo->Media->ReadOnly;
  WriteCaching      = BlockIo->Media->WriteCaching;
  BlockSize         = BlockIo->Media->BlockSize;
  IoAlign           = BlockIo->Media->IoAlign;
  LastBlock         = BlockIo->Media->LastBlock;

  BlockNumber       = (UINT32) MINIMUM(LastBlock, MAX_NUMBER_OF_READ_BLOCK_BUFFER);
  BufferSize        = BlockNumber * BlockSize;

  if (BufferSize == 0) {
    BufferSize = BlockSize;
  }

  //
  // allocate aligned buffer
  //
  Buffer = AllocateAlignedPool(
             EfiBootServicesData, 
             BufferSize, 
             IoAlign
             );
  if (Buffer == NULL) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"AllocateAlignedPool - Allocate aligned buffer for testing",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   EFI_OUT_OF_RESOURCES
                   );
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Assertion Points 4.3.2.1
  // WriteBlocks should not succeed to write to device
  // When MedialId is not the id for the current media in the device
  //

  if ((MediaPresent == TRUE) && (ReadOnly == FALSE)) {
    for (Index = 0; Index < 5; Index++) {
      //
      // initialize test data
      //
      switch (Index) {
      case 0:
        NewMediaId = MediaId + 5;
        break;
      case 1:
        NewMediaId = MediaId + 1;
        break;
      case 2:
        NewMediaId = MediaId - 1;
        break;
      case 3:
        NewMediaId = MediaId - 5;
        break;
      case 4:
        NewMediaId = 0;
        break;
      default:
        NewMediaId = MediaId;
      }

      //
      // Verify test data
      //
      if (NewMediaId == MediaId) {
        continue;
      }

      //
      // Add ReadBlocks() here just for avoiding WriteBlocks() may destroy the
      // file system
      //
      BlockIo->ReadBlocks (
                 BlockIo,
                 NewMediaId,
                 0,
                 BufferSize,
                 (VOID*)Buffer
                 );

      //
      // Call WriteBlocks with false MediaID
      //
      Status = BlockIo->WriteBlocks (
                          BlockIo,
                          NewMediaId,
                          0,
                          BufferSize,
                          (VOID*)Buffer
                          );
      FailStatus = Status;
      if (Status != EFI_MEDIA_CHANGED) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        ErrorOutput = L"False MediaID";
      } else {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      }

      //
      // Call WriteBlocks with false MediaID & invalid LBA
      //
      Status = BlockIo->WriteBlocks (
                          BlockIo,
                          NewMediaId,
                          LastBlock + 1,
                          BufferSize,
                          (VOID*)Buffer
                          );
      if (Status != EFI_MEDIA_CHANGED) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        FailStatus = Status;
        ErrorOutput = L"False MediaID & Invalid LBA";
      }

      //
      // Call WriteBlocks with false MediaID & invalid BufferSize
      //
      Status = BlockIo->WriteBlocks (
                          BlockIo,
                          NewMediaId,
                          0,
                          BufferSize - 1,
                          (VOID*)Buffer
                          );
      if (Status != EFI_MEDIA_CHANGED) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        FailStatus = Status;
        ErrorOutput = L"False MediaID & Invalid BufferSize";
      }
      
      //
      // Call WriteBlocks with false MediaID & invalid 0 BufferSize
      //
      Status = BlockIo->WriteBlocks (
                          BlockIo,
                          NewMediaId,
                          0,
                          0,
                          (VOID*)Buffer
                          );
      if (Status != EFI_MEDIA_CHANGED) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        FailStatus = Status;
        ErrorOutput = L"False MediaID & Invalid 0 BufferSize";
      }
      
      //
      // Call WriteBlocks with false MediaID & wrong aligned Buffer
      //
      if (IoAlign > 1) {
        Status = BlockIo->WriteBlocks (
                            BlockIo,
                            NewMediaId,
                            0,
                            BufferSize - BlockSize,
                            (VOID*)(Buffer + 1)
                            );
        if (Status != EFI_MEDIA_CHANGED) {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
          FailStatus = Status;
          ErrorOutput = L"False MediaID & Wrong aligned Buffer";
        }
      }

      //
      // Call WriteBlocks with false MediaID & invalid NULL Buffer
      //
      Status = BlockIo->WriteBlocks (
                          BlockIo,
                          NewMediaId,
                          0,
                          BufferSize,
                          NULL
                          );
      if (Status != EFI_MEDIA_CHANGED) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        FailStatus = Status;
        ErrorOutput = L"False MediaID & Invalid NULL Buffer";
      }
      
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gBlockIoConformanceTestAssertionGuid014,
                     L"EFI_BLOCK_IO_PROTOCOL.WriteBlocks - Write block with MediaId not being the id for the current media in the device",
                     L"%a:%d: %s ActualMediaID=%d, MediaID=%d, Status=%r, Expected=%r",
                     __FILE__,
                     (UINTN)__LINE__,
                     ErrorOutput,
                     MediaId,
                     NewMediaId,
                     FailStatus,
                     EFI_MEDIA_CHANGED
                     );
    }
  }

  //
  // Assertion point 4.3.2.2
  // WriteBlocks should not suceed to write to device when BufferSize is not
  // a multiple of the intrinsic block size of the device
  //
  if ((MediaPresent == TRUE) && (BlockSize != 1) && (ReadOnly == FALSE)) {
    for (Index = 0; Index < 4; Index++) {
      //
      // initialize test data
      //
      switch (Index) {
      case 0:
        NewBufferSize = BlockSize + 1;
        break;
      case 1:
        NewBufferSize = 2 * BlockSize - 1;
        break;
      case 2:
        NewBufferSize = 2 * BlockSize + 1;
        break;
      case 3:
        NewBufferSize = 3 * BlockSize - 1;
        break;
      default:
        NewBufferSize = BufferSize;
        break;
      }
      //
      // Verify test data
      //
      if ((NewBufferSize % BlockSize == 0) || (NewBufferSize > BufferSize)) {
        continue;
      }

      //
      // Add ReadBlocks() here just for avoiding WriteBlocks() may destroy the
      // file system
      //
      BlockIo->ReadBlocks (
                 BlockIo,
                 MediaId,
                 0,
                 NewBufferSize,
                 (VOID*)Buffer
                 );

      //
      // Call WriteBlocks with invalid BufferSize
      //
      Status = BlockIo->WriteBlocks (
                          BlockIo,
                          MediaId,
                          0,
                          NewBufferSize,
                          (VOID*)Buffer
                          );
      if (Status != EFI_BAD_BUFFER_SIZE) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      }

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gBlockIoConformanceTestAssertionGuid015,
                     L"EFI_BLOCK_IO_PROTOCOL.WriteBlocks - Write block with BufferSize not being a multiple of the intrinsic block size of the device",
                     L"%a:%d:BufferSize=%d, BlockSize=%d, Status=%r",
                     __FILE__,
                     (UINTN)__LINE__,
                     NewBufferSize,
                     BlockSize,
                     Status
                     );
    }
  }

  //
  // Assertion Point 4.3.2.3
  // WriteBlocks should not succeed to write to device with invalid LBA parameter
  //
  if ((MediaPresent == TRUE) && (ReadOnly == FALSE)) {
    for (Index = 0; Index < 6; Index++) {
      //
      // initialize test data
      //
      switch (Index) {
      case 0:
        NewLba = LastBlock + 1;
        break;
      case 1:
        NewLba = LastBlock + 100;
        break;
      case 2:
        NewLba = LastBlock - BufferSize/BlockSize + 1;
        break;
      case 3:
        NewLba = LastBlock - BufferSize/BlockSize + 2;
        break;
      case 4:
        NewLba = LastBlock - BufferSize/BlockSize + 3;
        break;
      case 5:
        NewLba = LastBlock - BufferSize/BlockSize + 100;
        break;
      default:
        NewLba = 0;
        break;
      }

      //
      // Verify test data
      //
      if (NewLba + BufferSize/BlockSize <= LastBlock + 1) {
        continue;
      }

      //
      // Add ReadBlocks() here just for avoiding WriteBlocks() may destroy the
      // file system
      //
      BlockIo->ReadBlocks (
                 BlockIo,
                 MediaId,
                 NewLba,
                 BufferSize,
                 (VOID*)Buffer
                 );

      //
      // Call WriteBlocks with invalid LBA
      //
      Status = BlockIo->WriteBlocks (
                          BlockIo,
                          MediaId,
                          NewLba,
                          BufferSize,
                          (VOID*)Buffer
                          );
      if (Status != EFI_INVALID_PARAMETER) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      }

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gBlockIoConformanceTestAssertionGuid016,
                     L"EFI_BLOCK_IO_PROTOCOL.WriteBlocks - Write block with invalid LBA parameter",
                     L"%a:%d:BufferSize=%d, BlockSize=%d, LBA=0x%lx, LastBlock=0x%lx, Status=%r",
                     __FILE__,
                     (UINTN)__LINE__,
                     BufferSize,
                     BlockSize,
                     NewLba,
                     LastBlock,
                     Status
                     );
    }
  }

  //
  // Assertion Point 4.3.2.6
  // WriteBlocks should not succeed to write to a write-protected device
  //
  if ((MediaPresent == TRUE) && (ReadOnly == TRUE)) {
    //
    // Add ReadBlocks() here just for avoiding WriteBlocks() may destroy the
    // file system
    //
    BlockIo->ReadBlocks (
               BlockIo,
               MediaId,
               0,
               BufferSize,
               (VOID*)Buffer
               );

    //
    // Call WriteBlocks with proper parameter
    //
    Status = BlockIo->WriteBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (Status != EFI_WRITE_PROTECTED) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid019,
                   L"EFI_BLOCK_IO_PROTOCOL.WriteBlocks - Write block to readonly device",
                   L"%a:%d:Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }


  //
  // Assertion Point 4.3.2.7
  // WriteBlocks should not succeed to write to device that with no media present in the device
  //
  if (MediaPresent == FALSE) {
    ErrorOutput = L"";

    //
    // Add ReadBlocks() here just for avoiding WriteBlocks() may destroy the
    // file system
    //
    BlockIo->ReadBlocks (
               BlockIo,
               MediaId,
               0,
               BufferSize,
               (VOID*)Buffer
               );

    //
    // Call WriteBlocks with proper parameter
    //
    Status = BlockIo->WriteBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    FailStatus = Status;
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    //
    // Call WriteBlocks with invalid LBA
    //
    Status = BlockIo->WriteBlocks (
                        BlockIo,
                        MediaId,
                        LastBlock + 1,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
      FailStatus = Status;
      ErrorOutput = L"Invalid LBA"; 
    }
    
    //
    // Call ReadBlocks with invalid BufferSize
    //
    Status = BlockIo->WriteBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize - 1,
                        (VOID*)Buffer
                        );
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
      FailStatus = Status;
      ErrorOutput = L"Invalid BufferSize"; 
    }

    //
    // Call WriteBlocks with invalid 0 BufferSize
    //
    Status = BlockIo->WriteBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        0,
                        (VOID*)Buffer
                        );
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
      FailStatus = Status;
      ErrorOutput = L"Zero BufferSize";
    }

    
    //
    // Call WriteBlocks with Unaligned Buffer
    //
    if (IoAlign > 1) {
      Status = BlockIo->WriteBlocks (
                          BlockIo,
                          MediaId,
                          0,
                          BufferSize - BlockSize,
                          (VOID*)(Buffer + 1)
                          );
      if (Status != EFI_NO_MEDIA) {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
        FailStatus = Status;
        ErrorOutput = L"Unaligned Buffer";
      }
    }

    //
    // Call WriteBlocks with invalid NULL Buffer
    //
    Status = BlockIo->WriteBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        NULL
                        );
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
      FailStatus = Status;
      ErrorOutput = L"Null Buffer";
    }
     
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid020,
                   L"EFI_BLOCK_IO_PROTOCOL.WriteBlocks - Write block from device without media present in the device",
                   L"%a:%d: %s BufferSize=%d, Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   ErrorOutput,
                   BufferSize,
                   FailStatus
                   );
  }

  //
  // Assertion Point 4.3.2.8
  // WriteBlocks should not succed to write to device with Buffer is not on proper alignment
  //
  if ((ReadOnly == FALSE) && (MediaPresent == TRUE) && (IoAlign > 1)) {
    //
    // if IoAlign is large than 1, then it must be a power of 2
    //
    if ((IoAlign & (IoAlign - 1)) == 0) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid022,
                   L"EFI_BLOCK_IO_PROTOCOL.WriteBlocks - Write block aligment should be power of 2",
                   L"%a:%d:Aligment=0x%lx",
                   __FILE__,
                   (UINTN)__LINE__,
                   IoAlign
                   );
   
    for (NewIoAlign = (IoAlign >> 1); NewIoAlign > 0 ; NewIoAlign >>= 1) {
      //
      // Call WriteBlocks with buffer of wrong aligment 
      //
      Status = BlockIo->WriteBlocks (
                          BlockIo,
                          MediaId,
                          0,
                          BlockSize,
                          (VOID*)(Buffer + NewIoAlign)
                          );
      if (Status == EFI_INVALID_PARAMETER) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }
    
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gBlockIoConformanceTestAssertionGuid023,
                     L"EFI_BLOCK_IO_PROTOCOL.WriteBlocks - Write block with unaligned data buffer",
                     L"%a:%d:Aligment=%d, Status=%r,Expected=%r",
                     __FILE__,
                     (UINTN)__LINE__,
                     NewIoAlign,
                     Status,
                     EFI_INVALID_PARAMETER
                     );
    }
  }


  //
  // Free aligned buffer
  //
  Status = FreeAlignedPool (Buffer);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"FreeAlignedPool - Free aligned buffer for testing",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_BLOCK_IO_PROTOCOL.WriteBlocks() Manually Consistency Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 4.4
//
EFI_STATUS
BBTestWriteBlocksConformanceManualTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib;
  EFI_STATUS                           Status;
  EFI_BLOCK_IO_PROTOCOL                *BlockIo;
  EFI_TEST_ASSERTION                   AssertionType;
  UINT32                               MediaId;
  BOOLEAN                              RemovableMedia;
  BOOLEAN                              MediaPresent;
  BOOLEAN                              LogicalPartition;
  BOOLEAN                              ReadOnly;
  BOOLEAN                              WriteCaching;
  UINT32                               BlockSize;
  UINT32                               IoAlign;
  EFI_LBA                              LastBlock;

  UINTN                                BufferSize;
  UINT8                                *Buffer;

  UINT32                               BlockNumber;

  EFI_DEVICE_PATH_PROTOCOL             *DevicePath;
  CHAR16                               *DevicePathStr;


  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );

  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"BS.HandleProtocol - Handle standard test library",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  BlockIo = (EFI_BLOCK_IO_PROTOCOL *)ClientInterface;

  //
  // Locate & record DevicePath for further investigation
  //
  LocateDevicePathFromBlockIo (BlockIo, &DevicePath, StandardLib);

  DevicePathStr = DevicePathToStr (DevicePath);
  if (DevicePathStr != NULL) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_DEFAULT,
                   L"Current Device: %s",
                   DevicePathStr
                   );

    Status = gtBS->FreePool (DevicePathStr);
    if (EFI_ERROR(Status)) {
      StandardLib->RecordAssertion (
                     StandardLib,
                     EFI_TEST_ASSERTION_FAILED,
                     gTestGenericFailureGuid,
                     L"BS.FreePool - Free device path string",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
      return Status;
    }

    DevicePathStr = NULL;
  }


  //
  // Initialize variable
  //
  MediaId           = BlockIo->Media->MediaId;
  RemovableMedia    = BlockIo->Media->RemovableMedia;
  MediaPresent      = BlockIo->Media->MediaPresent;
  LogicalPartition  = BlockIo->Media->LogicalPartition;
  ReadOnly          = BlockIo->Media->ReadOnly;
  WriteCaching      = BlockIo->Media->WriteCaching;
  BlockSize         = BlockIo->Media->BlockSize;
  IoAlign           = BlockIo->Media->IoAlign;
  LastBlock         = BlockIo->Media->LastBlock;

  BlockNumber       = (UINT32) MINIMUM(LastBlock, MAX_NUMBER_OF_READ_BLOCK_BUFFER);
  BufferSize        = BlockNumber * BlockSize;

  if (BufferSize == 0) {
    BufferSize = BlockSize;
  }

  //
  // allocate buffer
  //
  Status = gtBS->AllocatePool (EfiBootServicesData, BufferSize, &Buffer);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"BS.AllocatePool - Allocate buffer for testing",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                 );
    return Status;
  }


  //
  // Assertion Points 4.4.2.1
  // WriteBlocks should not succeed to write to device
  // When MediaId is not the id for the current media in the device
  //
  if ((MediaPresent == TRUE) && (RemovableMedia == TRUE) && (ReadOnly == FALSE)) {
    //
    // Call ReadBlocks with valid MediaID and parameter
    // to verify the success of reading from device
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (EFI_ERROR(Status)) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gTestGenericFailureGuid,
                   L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block from device",
                   L"%a:%d:BufferSize=%d, Lba=0, Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   BufferSize,
                   Status
                   );
    if (EFI_ERROR(Status)) {
      goto AssertionPoints2;
    }

    //
    // Display instruction to tell user change the media
    //
    Print (L"Change media in device ");
    DevicePathStr = DevicePathToStr (DevicePath);
    if (DevicePathStr != NULL) {
      Print (DevicePathStr);

      Status = gtBS->FreePool (DevicePathStr);
      if (EFI_ERROR(Status)) {
        StandardLib->RecordAssertion (
                    StandardLib,
                    EFI_TEST_ASSERTION_FAILED,
                    gTestGenericFailureGuid,
                    L"BS.FreePool - Free device path string",
                    L"%a:%d:Status - %r",
                    __FILE__,
                    (UINTN)__LINE__,
                    Status
                    );
        goto FreeBuffer;
      }

      DevicePathStr = NULL;
    }

    if (AutoJudge (50, FALSE, L"\r\nReady?\r\n") != TRUE) {
      goto AssertionPoints2;
    }

    //
    // wait for 0.5s for re-install BlockIo interface
    //
    gtBS->Stall (500000);

    //
    // Call WriteBlocks with the same MediaID and same parameter again
    //
    Status = BlockIo->WriteBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (Status != EFI_MEDIA_CHANGED) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid025,
                   L"EFI_BLOCK_IO_PROTOCOL.WriteBlocks - Write block with MediaId not being the id for the current media in the device",
                   L"%a:%d:ActualMediaID=%d, MediaID=%d, BufferSize=%d, Lba=0, Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   BlockIo->Media->MediaId,
                   MediaId,
                   BufferSize,
                   Status
                   );

    //
    // Begin to re-locate the BlockIo interface according to the DevicePath
    //
    LocateBlockIoFromDevicePath (&BlockIo, DevicePath, StandardLib);

    //
    // Initialize variable
    //
    MediaId           = BlockIo->Media->MediaId;
    RemovableMedia    = BlockIo->Media->RemovableMedia;
    MediaPresent      = BlockIo->Media->MediaPresent;
    LogicalPartition  = BlockIo->Media->LogicalPartition;
    ReadOnly          = BlockIo->Media->ReadOnly;
    WriteCaching      = BlockIo->Media->WriteCaching;
    BlockSize         = BlockIo->Media->BlockSize;
    IoAlign           = BlockIo->Media->IoAlign;
    LastBlock         = BlockIo->Media->LastBlock;
  }

AssertionPoints2:
  //
  // Assertion Points 4.4.2.2 WriteBlocks should not succeed to
  // write from device with no media in the device
  //
  if ((MediaPresent == TRUE) && (RemovableMedia == TRUE)) {
    //
    // Call ReadBlocks with valid MediaID and parameter
    // to verify the success of reading from device
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (EFI_ERROR(Status)) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gTestGenericFailureGuid,
                   L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block from device",
                   L"%a:%d:BufferSize=%d, Lba=0, Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   BufferSize,
                   Status
                   );

    if (EFI_ERROR(Status)) {
      goto FreeBuffer;
    }

    //
    // Display instruction to tell user remove the media
    //
    Print (L"Remove media in device ");
    DevicePathStr = DevicePathToStr (DevicePath);
    if (DevicePathStr != NULL) {
      Print (DevicePathStr);

      Status = gtBS->FreePool (DevicePathStr);
      if (EFI_ERROR(Status)) {
        StandardLib->RecordAssertion (
                       StandardLib,
                       EFI_TEST_ASSERTION_FAILED,
                       gTestGenericFailureGuid,
                       L"BS.FreePool - Free device path string",
                       L"%a:%d:Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );
        goto FreeBuffer;
      }

      DevicePathStr = NULL;
    }

    if (AutoJudge (50, FALSE, L"\r\nReady?\r\n") != TRUE) {
      goto AssertionPoint3;
    }

    //
    // wait for 0.5s for re-install BlockIo interface
    //
    gtBS->Stall (500000);


    //
    // Call WriteBlocks with the same MediaID and same parameter again
    //
    Status = BlockIo->WriteBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid027,
                   L"EFI_BLOCK_IO_PROTOCOL.WriteBlocks - Write block with no media in the device",
                   L"%a:%d:ActualMediaID=%d, MediaID=%d, BufferSize=%d, Lba=0, Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   BlockIo->Media->MediaId,
                   MediaId,
                   BufferSize,
                   Status
                   );

    //
    // Display instruction to tell user to re-insert the media
    //
    Print (L"Re-insert media into device ");
    DevicePathStr = DevicePathToStr (DevicePath);
    if (DevicePathStr != NULL) {
      Print (DevicePathStr);

      Status = gtBS->FreePool (DevicePathStr);
      if (EFI_ERROR(Status)) {
        StandardLib->RecordAssertion (
                       StandardLib,
                       EFI_TEST_ASSERTION_FAILED,
                       gTestGenericFailureGuid,
                       L"BS.FreePool - Free device path string",
                       L"%a:%d:Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );
        goto FreeBuffer;
      }

      DevicePathStr = NULL;
    }

    if (AutoJudge (50, FALSE, L"\r\nReady?\r\n") != TRUE) {
      goto FreeBuffer;
    }

    //
    // Call ReadBlocks to force re-install device
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BlockSize,
                        (VOID*)Buffer
                        );
    if (EFI_ERROR(Status)) {
      //
      // No Status check here.
      //
    }

    //
    // Begin to re-locate the BlockIo interface according to the DevicePath
    //
    LocateBlockIoFromDevicePath (&BlockIo, DevicePath, StandardLib);

    //
    // Initialize variable
    //
    MediaId           = BlockIo->Media->MediaId;
    RemovableMedia    = BlockIo->Media->RemovableMedia;
    MediaPresent      = BlockIo->Media->MediaPresent;
    LogicalPartition  = BlockIo->Media->LogicalPartition;
    ReadOnly          = BlockIo->Media->ReadOnly;
    WriteCaching      = BlockIo->Media->WriteCaching;
    BlockSize         = BlockIo->Media->BlockSize;
    IoAlign           = BlockIo->Media->IoAlign;
    LastBlock         = BlockIo->Media->LastBlock;
  }

AssertionPoint3:
  //
  // Assertion Points 4.4.2.3 WriteBlocks should not succeed to
  // write to device with write-protected media in the device
  //
  if ((MediaPresent == TRUE) && (RemovableMedia == TRUE)) {
    //
    // Call ReadBlocks with valid MediaID and parameter
    // to verify the success of reading from device
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (EFI_ERROR(Status)) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gTestGenericFailureGuid,
                   L"EFI_BLOCK_IO_PROTOCOL.ReadBlocks - Read block from device",
                   L"%a:%d:BufferSize=%d, Lba=0, Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   BufferSize,
                   Status
                   );
    if (EFI_ERROR(Status)) {
      goto FreeBuffer;
    }

    //
    // Display instruction to tell user to make media write-protected
    //
    Print (L"Turn media in device into write-protected ");
    DevicePathStr = DevicePathToStr (DevicePath);
    if (DevicePathStr != NULL) {
      Print (DevicePathStr);

      Status = gtBS->FreePool (DevicePathStr);
      if (EFI_ERROR(Status)) {
        StandardLib->RecordAssertion (
                       StandardLib,
                       EFI_TEST_ASSERTION_FAILED,
                       gTestGenericFailureGuid,
                       L"BS.FreePool - Free device path string",
                       L"%a:%d:Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );
        goto FreeBuffer;
      }

      DevicePathStr = NULL;
    }

    if (AutoJudge (50, FALSE, L"\r\nReady?\r\n") != TRUE) {
      goto FreeBuffer;
    }

    //
    // wait for 0.5s for re-install BlockIo interface
    //
    gtBS->Stall (500000);

    //
    // Call ReadBlocks to force re-install device
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BlockSize,
                        (VOID*)Buffer
                        );
    if (EFI_ERROR(Status)) {
      //
      // No Status check here.
      //
    }

    //
    // Begin to re-locate the BlockIo interface according to the DevicePath
    //
    LocateBlockIoFromDevicePath (&BlockIo, DevicePath, StandardLib);

    //
    // Initialize variable
    //
    MediaId           = BlockIo->Media->MediaId;
    RemovableMedia    = BlockIo->Media->RemovableMedia;
    MediaPresent      = BlockIo->Media->MediaPresent;
    LogicalPartition  = BlockIo->Media->LogicalPartition;
    ReadOnly          = BlockIo->Media->ReadOnly;
    WriteCaching      = BlockIo->Media->WriteCaching;
    BlockSize         = BlockIo->Media->BlockSize;
    IoAlign           = BlockIo->Media->IoAlign;
    LastBlock         = BlockIo->Media->LastBlock;

    //
    // Call WriteBlocks on write-protected device
    //
    Status = BlockIo->WriteBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BufferSize,
                        (VOID*)Buffer
                        );
    if (Status != EFI_WRITE_PROTECTED) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid029,
                   L"EFI_BLOCK_IO_PROTOCOL.WriteBlocks - Write block on write-protected device",
                   L"%a:%d:ActualMediaID=%d, MediaID=%d, BufferSize=%d, Lba=0, Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   BlockIo->Media->MediaId,
                   MediaId,
                   BufferSize,
                   Status
                   );

    //
    // Display instruction to tell user to make media non write-protected
    //

    Print (L"Turn media write-protected off ");
    DevicePathStr = DevicePathToStr (DevicePath);
    if (DevicePathStr != NULL) {
      Print (DevicePathStr);

      Status = gtBS->FreePool (DevicePathStr);
      if (EFI_ERROR(Status)) {
        StandardLib->RecordAssertion (
                       StandardLib,
                       EFI_TEST_ASSERTION_FAILED,
                       gTestGenericFailureGuid,
                       L"BS.FreePool - Free device path string",
                       L"%a:%d:Status - %r",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status
                       );
        goto FreeBuffer;
      }

      DevicePathStr = NULL;
    }

    if (AutoJudge (50, FALSE, L"\r\nReady?\r\n") != TRUE) {
      goto FreeBuffer;
    }

    //
    // wait for 0.5s for re-install BlockIo interface
    //
    gtBS->Stall (500000);

    //
    // Call ReadBlocks to force re-install device
    //
    Status = BlockIo->ReadBlocks (
                        BlockIo,
                        MediaId,
                        0,
                        BlockSize,
                        (VOID*)Buffer
                        );
    if (EFI_ERROR(Status)) {
      //
      // No Status check here.
      //
    }
  }

FreeBuffer:
  Status = gtBS->FreePool (Buffer);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"BS.FreePool - Free buffer for testing",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_BLOCK_IO_PROTOCOL.FlushBlocks() Consistency Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 4.5
//
EFI_STATUS
BBTestFlushBlocksConformanceAutoTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib;
  EFI_STATUS                           Status;
  EFI_BLOCK_IO_PROTOCOL                *BlockIo;
  EFI_TEST_ASSERTION                   AssertionType;
  UINT32                               MediaId;
  BOOLEAN                              RemovableMedia;
  BOOLEAN                              MediaPresent;
  BOOLEAN                              LogicalPartition;
  BOOLEAN                              ReadOnly;
  BOOLEAN                              WriteCaching;
  UINT32                               BlockSize;
  UINT32                               IoAlign;
  EFI_LBA                              LastBlock;

  UINTN                                BufferSize;
  UINT8                                *Buffer;


  UINT32                               BlockNumber;


  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );

  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"BS.HandleProtocol - Handle standard test library",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                 );
    return Status;
  }

  BlockIo = (EFI_BLOCK_IO_PROTOCOL *)ClientInterface;

  //
  // Initialize variable
  //
  MediaId           = BlockIo->Media->MediaId;
  RemovableMedia    = BlockIo->Media->RemovableMedia;
  MediaPresent      = BlockIo->Media->MediaPresent;
  LogicalPartition  = BlockIo->Media->LogicalPartition;
  ReadOnly          = BlockIo->Media->ReadOnly;
  WriteCaching      = BlockIo->Media->WriteCaching;
  BlockSize         = BlockIo->Media->BlockSize;
  IoAlign           = BlockIo->Media->IoAlign;
  LastBlock         = BlockIo->Media->LastBlock;

  BlockNumber       = (UINT32) MINIMUM(LastBlock, MAX_NUMBER_OF_READ_BLOCK_BUFFER);
  BufferSize        = BlockNumber * BlockSize;

  if (BufferSize == 0) {
    BufferSize = BlockSize;
  }

  //
  // allocate buffer
  //
  Status = gtBS->AllocatePool (EfiBootServicesData, BufferSize, &Buffer);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"BS.AllocatePool - Allocate buffer for testing",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                 );
    return Status;
  }

  //
  // Assertion Point 4.5.2.1
  // FlushBlocks should not succeed to flush data to device that with no media present in the device
  //
  if ((MediaPresent == FALSE) && (WriteCaching == TRUE)) {
    //
    // Call FlushBlocks for testing
    //
    Status = BlockIo->FlushBlocks (BlockIo);
    if (Status != EFI_NO_MEDIA) {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    }

    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBlockIoConformanceTestAssertionGuid030,
                   L"EFI_BLOCK_IO_PROTOCOL.FlushBlocks - Flush blocks to device with no media present in the device",
                   L"%a:%d:Status=%r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  //
  // Free resources
  //
  Status = gtBS->FreePool (Buffer);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"BS.FreePool - Free pool",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_BLOCK_IO_PROTOCOL.FlushBlocks() Manually Consistency Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 4.6
//
EFI_STATUS
BBTestFlushBlocksConformanceManualTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  //
  // Current not support!
  //

  return EFI_SUCCESS;
}
