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

  EbcBBTestMain.c

Abstract:

  Test Driver of Ebc Protocol

--*/

#include "EbcBBTestMain.h"

EFI_GUID gEfiEbcDriverProtocolGuid = EBC_DRIVER_TEST_PROTOCOL;

EFI_PEI_FLUSH_INSTRUCTION_CACHE_PROTOCOL  *gEfiPeiFlushInstructionCache = NULL;
EFI_PEI_PE_COFF_LOADER_PROTOCOL           *gEfiPeiPeCoffLoader          = NULL;

EFI_DEVICE_PATH_PROTOCOL        *gDevicePath = NULL;
CHAR16                          *gFilePath   = NULL;
EFI_HANDLE                      gDummyImageHandle = NULL;

BOOLEAN                         gFlushICache;

STATIC EFI_LOADED_IMAGE_PROTOCOL             DummyImageProtocol;
STATIC IMAGE_FILE_HANDLE                     FHand;
STATIC EFI_PEI_PE_COFF_LOADER_IMAGE_CONTEXT  ImageContext;
STATIC EFI_PHYSICAL_ADDRESS                  ImageAddress;
STATIC UINTN                                 NumberOfPages;

EFI_BB_TEST_PROTOCOL_FIELD gBBTestProtocolField = {
  EBC_TEST_REVISION,
  IHV_EBC_PROTOCOL_GUID,
  L"Ebc Protocol Test",
  L"UEFI EFI Byte Code Protocol Test"
};

EFI_GUID gSupportProtocolGuid1[2] = {
  EFI_STANDARD_TEST_LIBRARY_GUID,
  EFI_NULL_GUID
};

EFI_BB_TEST_ENTRY_FIELD gBBTestEntryField[] = {
  {
    EBC_GETVERSION_BASIC_TEST_GUID,
    L"GetVersion_Func",
    L"Basic Test for GetVersion",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    BBTestGetVersionBasicTest
  },
  {
    EBC_UNLOADIMAGE_CONFORMANCE_TEST_GUID,
    L"UnloadImage_Conf",
    L"Conformance Test for UnloadImage",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    BBTestUnloadImageConformanceTest
  },
  {
    EBC_GETVERSION_CONFORMANCE_TEST_GUID,
    L"GetVersion_Conf",
    L"Conformance Test for GetVersion",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO,
    BBTestGetVersionConformanceTest
  },

#ifdef EFI_TEST_EXHAUSTIVE
  //
  // Change the CreateThunk_Func, CreateThunk_Conf, UnloadImage_Func,
  // and RegisterICacheFlush_Func to DESTRUCTIVE. Since all of them are
  // implementation related. Need to change to WB tests later.
  //
  {
    EBC_CREATETHUNK_BASIC_TEST_GUID,
    L"CreateThunk_Func",
    L"Basic Test for CreateThunk",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO | EFI_TEST_CASE_DESTRUCTIVE,
    BBTestCreateThunkBasicTest
  },
  {
    EBC_UNLOADIMAGE_BASIC_TEST_GUID,
    L"UnloadImage_Func",
    L"Basic Test for UnloadImage",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO | EFI_TEST_CASE_DESTRUCTIVE,
    BBTestUnloadImageBasicTest
  },
  {
    EBC_REGISTERICACHEFLUSH_BASIC_TEST_GUID,
    L"RegisterICacheFlush_Func",
    L"Basic Test for RegisterICacheFlush",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO | EFI_TEST_CASE_DESTRUCTIVE,
    BBTestRegisterICacheFlushBasicTest
  },
  {
    EBC_CREATETHUNK_CONFORMANCE_TEST_GUID,
    L"CreateThunk_Conf",
    L"Conformance Test for CreateThunk",
    EFI_TEST_LEVEL_MINIMAL,
    gSupportProtocolGuid1,
    EFI_TEST_CASE_AUTO | EFI_TEST_CASE_DESTRUCTIVE,
    BBTestCreateThunkConformanceTest
  },
#endif

  EFI_NULL_GUID
};

EFI_BB_TEST_PROTOCOL *gBBTestProtocolInterface;

EFI_STATUS
BBTestEbcUnload (
  IN EFI_HANDLE       ImageHandle
  );

EFI_DRIVER_ENTRY_POINT(InitializeBBTestEbcProtocol)

/**
 *  Ebc Protocol Test Driver Entry point.
 *  @param ImageHandle the driver image handle.
 *  @param SystemTable the system table.
 *  @return EFI_SUCCESS the driver is loaded successfully.
 */
EFI_STATUS
InitializeBBTestEbcProtocol (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                          Status;
  CHAR16                              *TempFilePath;
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL   *ProfileLib;

  //
  // Initialize library
  //
  EfiInitializeTestLib (ImageHandle, SystemTable);
  InitializeLib (ImageHandle, SystemTable);
  EfiInitializeDriverLib (ImageHandle, SystemTable);

  //
  // Get Pei Protocols
  //
  GetPeiProtocol (&gEfiPeiFlushInstructionCacheGuid, &gEfiPeiFlushInstructionCache);
  GetPeiProtocol (&gEfiPeiPeCoffLoaderGuid, &gEfiPeiPeCoffLoader);

  //
  // Use profile lib at here just for less effect on the current system. It is
  // dependent on the implementation of test framework. So need to be updated
  // later.
  //
  Status = gtBS->LocateProtocol (
                   &gEfiTestProfileLibraryGuid,
                   NULL,
                   &ProfileLib
                   );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get the system device path and file path
  //
  Status = ProfileLib->EfiGetSystemDevicePath (
                          ProfileLib,
                          &gDevicePath,
                          &TempFilePath
                          );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gFilePath = PoolPrint (L"%s\\%s\\%s", TempFilePath, DEPENDECY_DIR_NAME, EBC_DRIVER_NAME);
  gtBS->FreePool (TempFilePath);
  if (gFilePath == NULL) {
    gtBS->FreePool (gDevicePath);
    return EFI_OUT_OF_RESOURCES;
  }

  return EfiInitAndInstallIHVBBTestInterface (
           &ImageHandle,
           &gBBTestProtocolField,
           gBBTestEntryField,
           BBTestEbcUnload,
           &gBBTestProtocolInterface
           );
}

/**
 *  The driver's Unload function.
 *  @param ImageHandle the test driver image handle.
 *  @return EFI_SUCCESS unload successfully.
 */
EFI_STATUS
BBTestEbcUnload (
  IN EFI_HANDLE       ImageHandle
  )
{
  gtBS->FreePool (gDevicePath);
  gtBS->FreePool (gFilePath);

  return EfiUninstallAndFreeIHVBBTestInterface (
           ImageHandle,
           gBBTestProtocolInterface
           );
}

/**
 *  Get the Pei Protocol Interface from the Hob list.
 *  @param ProtocolGuid Pei Protocol Guid.
 *  @param Interface Pei Protocol interface to be returned.
 *  @return EFI_SUCCESS the protocol interface is got successfully.
 *  @return EFI_NOT_FOUND the protocol Guid is not found
 */
EFI_STATUS
GetPeiProtocol (
  IN EFI_GUID      *ProtocolGuid,
  IN OUT VOID      **Interface
  )
{
  EFI_STATUS  Status;
  VOID        *HobList;
  VOID        *Buffer;

  Status = EfiLibGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetNextGuidHob (&HobList, ProtocolGuid, &Buffer, NULL);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  *Interface = (VOID *)(*(UINTN *)(Buffer));

  return Status;
}

/**
 *  Read the Ebc Driver from the FAT to memory.
 *  @param Buffer Contain the pointer where the Ebc driver is stored in memory.
 *  @return EFI_SUCCESS the Ebc driver is read into memory successfully.
 */
EFI_STATUS
ReadEbcDriver (
  IN OUT VOID   **Buffer,
  IN OUT UINTN  *Size
)
{
  EFI_STATUS                          Status;
  EFI_HANDLE                          DeviceHandle;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL     *Vol;
  EFI_FILE_HANDLE                     RootDir;
  EFI_FILE_HANDLE                     Handle;
  EFI_FILE_INFO                       *FileInfo;
  UINTN                               FileInfoSize;
  VOID*                               FileBuffer;
  UINTN                               FileSize;
  EFI_DEVICE_PATH_PROTOCOL            *DevicePath;

  //
  //  Determine device handle for fs protocol on specified device path
  //
  DevicePath = gDevicePath;
  Status = gtBS->LocateDevicePath (
                   &gEfiSimpleFileSystemProtocolGuid,
                   &DevicePath,
                   &DeviceHandle
                   );
  if ( EFI_ERROR ( Status ) ) {
    return Status;
  }

  //
  //  Determine volume for file system on device handle
  //
  Status = gtBS->HandleProtocol (
                   DeviceHandle,
                   &gEfiSimpleFileSystemProtocolGuid,
                   (VOID*)&Vol
                   );
  if ( EFI_ERROR ( Status ) ) {
    return Status;
  }

  //
  // Open volume for file system on device path
  //
  Status = Vol->OpenVolume ( Vol, &RootDir );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Open Ebc driver for read
  //
  Status = RootDir->Open(
                      RootDir,
                      &Handle,
                      gFilePath,
                      EFI_FILE_MODE_READ,
                      0
                      );
  if ( EFI_ERROR ( Status ) ) {
    RootDir->Close( RootDir );
    return Status;
  }


  //
  // Get Ebc driver size
  //
  FileInfoSize = 1;
  Status = gtBS->AllocatePool(
                   EfiBootServicesData,
                   FileInfoSize,
                   &FileInfo
                   );
  if ( EFI_ERROR ( Status ) ) {
    Handle->Close( Handle );
    RootDir->Close( RootDir );
    return Status;
  }
  //
  // the return code should be EFI_BUFFER_TOO_SMALL
  //
  Status = Handle->GetInfo(
                     Handle,
                     &gEfiFileInfoGuid,
                     &FileInfoSize,
                     FileInfo
                     );

  gtBS->FreePool(FileInfo);
  Status = gtBS->AllocatePool(
                   EfiBootServicesData,
                   FileInfoSize,
                   &FileInfo
                   );
  if ( EFI_ERROR ( Status ) ) {
    Handle->Close( Handle );
    RootDir->Close( RootDir );
    return Status;
  }
  Status = Handle->GetInfo(
                     Handle,
                     &gEfiFileInfoGuid,
                     &FileInfoSize,
                     FileInfo
                     );
  FileSize = (UINTN)FileInfo->FileSize;
  gtBS->FreePool(FileInfo);

  //
  // Read the Ebc Driver
  //
  Status = gtBS->AllocatePool(
                   EfiBootServicesData,
                   FileSize,
                   &FileBuffer
                   );
  if ( EFI_ERROR ( Status ) ) {
    Handle->Close( Handle );
    RootDir->Close( RootDir );
    return Status;
  }
  Status = Handle->Read (
                     Handle,
                     &FileSize,
                     FileBuffer
                     );
  if (EFI_ERROR ( Status ) ) {
    gtBS->FreePool(FileBuffer);
  } else {
    *Buffer = FileBuffer;
    *Size = FileSize;
  }
  Handle->Close (Handle);
  RootDir->Close (RootDir);
  return Status;
}

EFI_STATUS
ReadImageFile (
  IN     VOID    *UserHandle,
  IN     UINTN   Offset,
  IN OUT UINTN   *ReadSize,
  OUT    VOID    *Buffer
  )
/*++

Routine Description:

Arguments:

Returns:

--*/
{
  UINTN               EndPossition;
  IMAGE_FILE_HANDLE  *FHand;

  FHand = UserHandle;
  //
  // Move data from our local copy of the file
  //
  EndPossition = Offset + *ReadSize;
  if (EndPossition > FHand->SourceSize) {
    *ReadSize = (UINT32)(FHand->SourceSize - Offset);
    if (Offset >= FHand->SourceSize) {
      *ReadSize = 0;
    }
  }

  EfiCommonLibCopyMem (Buffer, (CHAR8 *)FHand->Source + Offset, *ReadSize);
  return EFI_SUCCESS;
}

EFI_STATUS
FlushICache (
  IN EFI_PHYSICAL_ADDRESS     Start,
  IN UINT64                   Length
  )
{
  EFI_STATUS Status;

  Status = gEfiPeiFlushInstructionCache->Flush (
                                           gEfiPeiFlushInstructionCache,
                                           Start,
                                           Length
                                           );
  return Status;
}

EFI_STATUS
TestFlushICache (
  IN EFI_PHYSICAL_ADDRESS     Start,
  IN UINT64                   Length
  )
{
  EFI_STATUS Status;

  gFlushICache = TRUE;

  Status = FlushICache (Start, Length);

  return Status;
}

/**
 *  Load the Ebc Driver and get the entry point.
 *  @param EntryPoint Return the Ebc driver entry point.
 *  @return EFI_SUCCESS the Ebc driver loaded successfully.
 */
EFI_STATUS
LoadEbcDriver (
  IN OUT VOID                 **EntryPoint
)
{
  EFI_STATUS                            Status;
  UINTN                                 Size;

  //
  // Create the dummy image handle
  //
  EfiCommonLibZeroMem (&DummyImageProtocol, sizeof (DummyImageProtocol));
  Status = gtBS->InstallProtocolInterface (
                   &gDummyImageHandle,
                   &gEfiLoadedImageProtocolGuid,
                   EFI_NATIVE_INTERFACE,
                   &DummyImageProtocol
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Init FHand and ImageContext
  //
  EfiCommonLibZeroMem (&FHand, sizeof (IMAGE_FILE_HANDLE));
  EfiCommonLibZeroMem (&ImageContext, sizeof (ImageContext));
  FHand.Signature = IMAGE_FILE_HANDLE_SIGNATURE;
  Status = ReadEbcDriver (&FHand.Source, &FHand.SourceSize);
  if ( EFI_ERROR(Status) ) {
    return Status;
  }
  FHand.FreeBuffer = TRUE;
  ImageContext.Handle    = &FHand;
  ImageContext.ImageRead = (EFI_PEI_PE_COFF_LOADER_READ_FILE)ReadImageFile;

  //
  // Get information about the image being loaded
  //
  Status = gEfiPeiPeCoffLoader->GetImageInfo (gEfiPeiPeCoffLoader, &ImageContext);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Allocate memory of the correct memory type aligned on the required image boundry
  //
  if (ImageContext.SectionAlignment > EFI_PAGE_SIZE) {
    Size = (UINTN)ImageContext.ImageSize + ImageContext.SectionAlignment;
  } else {
    Size = (UINTN)ImageContext.ImageSize;
  }
  NumberOfPages = EFI_SIZE_TO_PAGES (Size);

  //
  // If the image relocations have not been stripped, then load at any address.
  // Otherwise load at the address at which it was linked.
  //
  Status = gtBS->AllocatePages (
                  (ImageContext.RelocationsStripped) ? AllocateAddress : AllocateAnyPages,
                  ImageContext.ImageCodeMemoryType,
                  NumberOfPages,
                  &ImageAddress
                  );
  if (EFI_ERROR (Status)) {
    NumberOfPages = 0;
    return Status;
  }

  ImageContext.ImageAddress =
      (ImageAddress + ImageContext.SectionAlignment - 1) &
      ~((UINTN)ImageContext.SectionAlignment - 1);
  //
  // Load the image from the file into the allocated memory
  //
  Status = gEfiPeiPeCoffLoader->LoadImage (gEfiPeiPeCoffLoader, &ImageContext);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Relocate the image in memory
  //
  Status = gEfiPeiPeCoffLoader->RelocateImage (gEfiPeiPeCoffLoader, &ImageContext);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Flush the Instruction Cache
  //
  Status = FlushICache (
             ImageContext.ImageAddress,
             ImageContext.ImageSize
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (ImageContext.Machine != EFI_IMAGE_MACHINE_EBC) {
    return EFI_NOT_FOUND;
  }

  //
  // Get the image entry point
  //
  *EntryPoint   = (VOID*)(UINTN)ImageContext.EntryPoint;

  return EFI_SUCCESS;
}

/**
 *  Unload the Ebc Driver and free all the memory allocated by LoadEbcDriver.
 *  @return EFI_SUCCESS the Ebc driver is unloaded successfully.
 */
EFI_STATUS
UnloadEbcDriver ()
{
  EFI_STATUS    Status;

  Status = gtBS->UninstallProtocolInterface (
                  gDummyImageHandle,
                  &gEfiEbcDriverProtocolGuid,
                  NULL
                  );

  Status = gtBS->UninstallProtocolInterface (
                   gDummyImageHandle,
                   &gEfiLoadedImageProtocolGuid,
                   NULL
                   );

  if (FHand.Source != NULL) {
    gtBS->FreePool (FHand.Source);
  }

  if (NumberOfPages != 0) {
    gtBS->FreePages (ImageAddress, NumberOfPages);
  };

  return EFI_SUCCESS;
}
