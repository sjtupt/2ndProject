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
                                                                
  Copyright 2006 - 2012 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010 - 2012, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:

  SupportFile.c

Abstract:

  This file provides the services to load/unload the test support libraries.

--*/

#include "Sct.h"

//
// Internal functions declaration
//

EFI_STATUS
LoadSingleSupportFile (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevicePath,
  IN CHAR16                       *FileName,
  OUT EFI_SCT_TEST_FILE           **SupportFile
  );

EFI_STATUS
LoadSingleProxyFile (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevicePath,
  IN CHAR16                       *FileName,
  OUT EFI_SCT_TEST_FILE           **SupportFile
  );

EFI_STATUS
CreateSingleSupportFile (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevicePath,
  IN CHAR16                       *FileName,
  IN EFI_HANDLE                   ImageHandle,
  IN VOID                         *Context,
  OUT EFI_SCT_TEST_FILE           **SupportFile
  );

EFI_STATUS
UnloadSingleSupportFile (
  IN EFI_SCT_TEST_FILE            *SupportFile
  );

EFI_STATUS
CreateSingleProxyFile (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevicePath,
  IN CHAR16                       *FileName,
  IN EFI_HANDLE                   ImageHandle,
  IN VOID                         *Context,
  OUT EFI_SCT_TEST_FILE           **SupportFile
  );

EFI_STATUS
UnloadSingleProxyFile (
  IN EFI_SCT_TEST_FILE            *SupportFile
  );

EFI_STATUS
OpenSingleSupportFile (
  IN EFI_GUID                     *Guid,
  OUT VOID                        **Protocol,
  OUT VOID                        **Interface
  );

EFI_STATUS
CloseSingleSupportFile (
  IN EFI_GUID                     *Guid
  );


//
// External functions implementation
//

EFI_STATUS
LoadSupportFiles (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevicePath,
  IN CHAR16                       *FilePath,
  IN BOOLEAN                      Recursive,
  OUT EFI_LIST_ENTRY              *SupportFileList
  )
/*++

Routine Description:

  Load all test support files.

Arguments:

  DevicePath      - Device path of the files.
  FilePath        - Path of the files.
  Recursive       - Recursively.
  SupportFileList - Pointer to the support file list.

Returns:

  EFI_SUCCESS     - Successfully.
  Other value     - Something failed.

--*/
{
  EFI_STATUS                        Status;
  EFI_HANDLE                        DeviceHandle;
  EFI_FILE_HANDLE                   RootDir;
  EFI_FILE_HANDLE                   SupportDir;
  CHAR16                            *SubDir;
  CHAR16                            *FileName;
  UINTN                             FileInfoSize;
  EFI_FILE_INFO                     *FileInfo;
  EFI_SCT_TEST_FILE                 *SupportFile;
  EFI_DEVICE_PATH_PROTOCOL          *RemainderPath;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *Vol;

  //
  // Check parameters
  //
  if ((DevicePath == NULL) || (FilePath == NULL) || (SupportFileList == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Debug information
  //
  EFI_SCT_DEBUG ((EFI_SCT_D_TRACE, L"Load support files from <%s>", FilePath));

  //
  // Locate the device handle from the device path
  //
  RemainderPath = DevicePath;

  Status = BS->LocateDevicePath (
                 &gEfiSimpleFileSystemProtocolGuid,
                 &RemainderPath,
                 &DeviceHandle
                 );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Locate device handle - %r", Status));
    return Status;
  }

  //
  // Locate the volume of the file system
  //
  Status = BS->HandleProtocol (
                 DeviceHandle,
                 &gEfiSimpleFileSystemProtocolGuid,
                 (VOID **)&Vol
                 );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Locate file system - %r", Status));
    return Status;
  }

  //
  // Open the root volume
  //
  Status = Vol->OpenVolume (Vol, &RootDir);
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Open root volume - %r", Status));
    return Status;
  }

  //
  // Open the support directory
  //
  Status = RootDir->Open (
                      RootDir,
                      &SupportDir,
                      FilePath,
                      EFI_FILE_MODE_READ,
                      EFI_FILE_DIRECTORY
                      );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Open support directory - %r", Status));
    RootDir->Close (RootDir);
    return Status;
  }

  RootDir->Close (RootDir);

  //
  // Allocate memory for the entries in the directory
  //
  FileInfoSize = sizeof(EFI_FILE_INFO) + 1024;

  Status = BS->AllocatePool (
                 EfiBootServicesData,
                 FileInfoSize,
                 (VOID **)&FileInfo
                 );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Allocate pool - %r", Status));
    SupportDir->Close (SupportDir);
    return Status;
  }

  //
  // Walk through each file in the directory
  //
  while (TRUE) {
    //
    // Read a file entry
    //
    FileInfoSize = sizeof(EFI_FILE_INFO) + 1024;

    Status = SupportDir->Read (
                           SupportDir,
                           &FileInfoSize,
                           FileInfo
                           );
    if (EFI_ERROR (Status) || (FileInfoSize == 0)) {
      break;
    }

    if ((FileInfo->Attribute & EFI_FILE_DIRECTORY) == 0) {
      //
      // This is a file
      //
      if (SctStrEndWith (FileInfo->FileName, L".efi")) {
        //
        // Load the support file
        //
        FileName = PoolPrint (L"%s\\%s", FilePath, FileInfo->FileName);
        if (FileName == NULL) {
          EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"PoolPrint: Out of resources"));
          break;
        }

        Status = LoadSingleSupportFile (
                   DevicePath,
                   FileName,
                   &SupportFile
                   );
        if (EFI_ERROR (Status)) {
          EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Load a support file - %r", Status));
          BS->FreePool (FileName);
          continue;
        }

        BS->FreePool (FileName);

        //
        // Add the support file to the support file list
        //
        InsertTailList (SupportFileList, &SupportFile->Link);
      }
    } else {
      //
      // This is a directory
      //
      if (Recursive) {
        //
        // Skip the '.' and '..' dir
        //
        if ((StrCmp (FileInfo->FileName, L".")  == 0) ||
            (StrCmp (FileInfo->FileName, L"..") == 0)) {
          continue;
        }

        //
        // Load the support files under the sub directory
        //
        SubDir = PoolPrint (L"%s\\%s", FilePath, FileInfo->FileName);
        if (SubDir == NULL) {
          EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"PoolPrint: Out of resources"));
          break;
        }

        Status = LoadSupportFiles (
                   DevicePath,
                   SubDir,
                   Recursive,
                   SupportFileList
                   );
        if (EFI_ERROR (Status)) {
          EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Load support files - %r", Status));
          BS->FreePool (SubDir);
          continue;
        }

        BS->FreePool (SubDir);
      }
    }
  }

  //
  // Free resources
  //
  BS->FreePool (FileInfo);
  SupportDir->Close (SupportDir);

  //
  // Done
  //
  return EFI_SUCCESS;
}


EFI_STATUS
UnloadSupportFiles (
  IN EFI_LIST_ENTRY               *SupportFileList
  )
/*++

Routine Description:

  Unload all test support files.

Arguments:

  SupportFileList - Pointer to the support file list.

Returns:

  EFI_SUCCESS     - Successfully.
  Other value     - Something failed.

--*/
{
  EFI_SCT_TEST_FILE   *SupportFile;

  //
  // Check parameters
  //
  if (SupportFileList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Debug information
  //
  EFI_SCT_DEBUG ((EFI_SCT_D_TRACE, L"Unload support files"));

  //
  // Walk through all test support files
  //
  while (!IsListEmpty (SupportFileList)) {
    SupportFile = CR (SupportFileList->Flink, EFI_SCT_TEST_FILE, Link, EFI_SCT_TEST_FILE_SIGNATURE);

    RemoveEntryList (&SupportFile->Link);
    UnloadSingleSupportFile (SupportFile);
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}


EFI_STATUS
OpenStandardSupportFiles (
  VOID
  )
/*++

Routine Description:

  Open the standard support files.

Returns:

  EFI_SUCCESS   - Successfully.
  Other value   - Something failed.

--*/
{
  EFI_STATUS  Status;
  CHAR16      *FileName;

  //
  // Debug information
  //
  EFI_SCT_DEBUG ((EFI_SCT_D_TRACE, L"Open standard support files"));

  //
  // EFI standard test support file
  //
  Status = OpenSingleSupportFile (
             &gEfiStandardTestLibraryGuid,
             &gFT->StslProtocol,
             &gFT->StslInterface
             );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Open standard test - %r", Status));
    CloseStandardSupportFiles ();
    return Status;
  }

  //
  // EFI test profile support file
  //
  Status = OpenSingleSupportFile (
             &gEfiTestProfileLibraryGuid,
             &gFT->TplProtocol,
             NULL
             );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Open test profile - %r", Status));
    CloseStandardSupportFiles ();
    return Status;
  }

  //
  // EFI test recovery support file
  //
  Status = OpenSingleSupportFile (
             &gEfiTestRecoveryLibraryGuid,
             &gFT->TrlProtocol,
             &gFT->TrlInterface
             );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Open test recovery - %r", Status));
    CloseStandardSupportFiles ();
    return Status;
  }

  //
  // EFI test logging support file
  //
  Status = OpenSingleSupportFile (
             &gEfiTestLoggingLibraryGuid,
             &gFT->TllProtocol,
             &gFT->TllInterface
             );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Open test logging - %r", Status));
    CloseStandardSupportFiles ();
    return Status;
  }

  //
  // Set the config data of test profile
  //
  Status = gFT->TplProtocol->EfiSetSystemDevicePath (
                               gFT->TplProtocol,
                               gFT->DevicePath,
                               gFT->FilePath
                               );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Set profile config - %r", Status));
    CloseStandardSupportFiles ();
    return Status;
  }

  //
  // Set the config data of test recovery
  //
  FileName = PoolPrint (L"%s\\%s", gFT->FilePath, EFI_SCT_FILE_RECOVERY);
  if (FileName == NULL) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"PoolPrint: Out of resources"));
    CloseStandardSupportFiles ();
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gFT->TrlInterface->SetConfig (
                                gFT->TrlInterface,
                                gFT->DevicePath,
                                FileName
                                );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Set recovery config - %r", Status));
    CloseStandardSupportFiles ();
    BS->FreePool (FileName);
    return Status;
  }

  BS->FreePool (FileName);

  //
  // Done
  //
  return EFI_SUCCESS;
}


EFI_STATUS
CloseStandardSupportFiles (
  VOID
  )
/*++

Routine Description:

  Close the standard support files.

Returns:

  EFI_SUCCESS   - Successfully.
  Other value   - Something failed.

--*/
{
  //
  // Debug information
  //
  EFI_SCT_DEBUG ((EFI_SCT_D_TRACE, L"Close standard support files"));

  //
  // EFI standard test support file
  //
  if ((gFT->StslProtocol != NULL) || (gFT->StslInterface != NULL)) {
    CloseSingleSupportFile (&gEfiStandardTestLibraryGuid);
    gFT->StslProtocol  = NULL;
    gFT->StslInterface = NULL;
  }

  //
  // EFI test profile support file
  //
  if (gFT->TplProtocol != NULL) {
    CloseSingleSupportFile (&gEfiTestProfileLibraryGuid);
    gFT->TplProtocol = NULL;
  }

  //
  // EFI test recovery support file
  //
  if ((gFT->TrlProtocol != NULL) || (gFT->TrlInterface != NULL)) {
    CloseSingleSupportFile (&gEfiTestRecoveryLibraryGuid);
    gFT->TrlProtocol  = NULL;
    gFT->TrlInterface = NULL;
  }

  //
  // EFI test logging support file
  //
  if ((gFT->TllProtocol != NULL) || (gFT->TllInterface != NULL)) {
    CloseSingleSupportFile (&gEfiTestLoggingLibraryGuid);
    gFT->TllProtocol  = NULL;
    gFT->TllInterface = NULL;
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}


EFI_STATUS
OpenExtendedSupportFiles (
  EFI_GUID                        *Guids
  )
/*++

Routine Description:

  Open the extended support files.

Arguments:

  Guids         - Specifies the GUIDs to open by.

Returns:

  EFI_SUCCESS   - Successfully.
  Other value   - Something failed.

--*/
{
  EFI_STATUS  Status;
  EFI_GUID    *Guid;

  //
  // Walk through all support GUIDs
  //
  for (Guid = Guids; CompareGuid (Guid, &gEfiNullGuid) != 0; Guid ++) {
    //
    // Skip the standard test support files
    //
    if ((CompareGuid (Guid, &gEfiStandardTestLibraryGuid) == 0) ||
        (CompareGuid (Guid, &gEfiTestProfileLibraryGuid)  == 0) ||
        (CompareGuid (Guid, &gEfiTestRecoveryLibraryGuid) == 0) ||
        (CompareGuid (Guid, &gEfiTestLoggingLibraryGuid)  == 0)) {
      continue;
    }

    //
    // Open the support file
    //
    Status = OpenSingleSupportFile (Guid, NULL, NULL);
    if (EFI_ERROR(Status)) {
      CloseExtendedSupportFiles (Guids);
      return Status;
    }
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}


EFI_STATUS
CloseExtendedSupportFiles (
  EFI_GUID                        *Guids
  )
/*++

Routine Description:

  Close the extended support files.

Arguments:

  Guids         - Soecifices the GUIDs to close by.

Returns:

  EFI_SUCCESS   - Successfully.
  Other value   - Something failed.

--*/
{
  EFI_GUID      *Guid;

  //
  // Walk through all support GUIDs
  //
  for (Guid = Guids; CompareGuid (Guid, &gEfiNullGuid) != 0; Guid ++) {
    //
    // Skip the standard test support files
    //
    if ((CompareGuid (Guid, &gEfiStandardTestLibraryGuid) == 0) ||
        (CompareGuid (Guid, &gEfiTestProfileLibraryGuid)  == 0) ||
        (CompareGuid (Guid, &gEfiTestRecoveryLibraryGuid) == 0) ||
        (CompareGuid (Guid, &gEfiTestLoggingLibraryGuid)  == 0)) {
      continue;
    }

    //
    // Close the support file
    //
    CloseSingleSupportFile (Guid);
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}


//
// Internal functions implementation
//

EFI_STATUS
LoadSingleSupportFile (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevicePath,
  IN CHAR16                       *FileName,
  OUT EFI_SCT_TEST_FILE           **SupportFile
  )
/*++

Routine Description:

  Load a test support file.

--*/
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  ImageHandle;
  EFI_DEVICE_PATH_PROTOCOL    *FileNode;
  EFI_DEVICE_PATH_PROTOCOL    *FilePath;
  EFI_LOADED_IMAGE_PROTOCOL   *LoadedImage;
  UINTN                       ExitDataSize;
  CHAR16                      *ExitData;
  EFI_TSL_INIT_INTERFACE      *TslInit;

  //
  // Check parameters
  //
  if ((DevicePath == NULL) || (FileName == NULL) || (SupportFile == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Debug information
  //
  EFI_SCT_DEBUG ((EFI_SCT_D_TRACE, L"Load support file <%s>", FileName));

  //
  // Add the file path to the device path
  //
  FileNode = FileDevicePath (NULL, FileName);
  if (FileNode == NULL) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"FileDevicePath: Out of resources"));
    return EFI_OUT_OF_RESOURCES;
  }

  FilePath = AppendDevicePath (DevicePath, FileNode);
  if (FilePath == NULL) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"AppendDevicePath: Out of resources"));
    BS->FreePool (FileNode);
    return EFI_OUT_OF_RESOURCES;
  }

  BS->FreePool (FileNode);

  //
  // Load the support file
  //
  Status = BS->LoadImage (
                 FALSE,
                 gFT->ImageHandle,
                 FilePath,
                 NULL,
                 0,
                 &ImageHandle
                 );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Load image - %r", Status));
    BS->FreePool (FilePath);
    return Status;
  }

  BS->FreePool (FilePath);

  //
  // Verify the image is a driver or not
  //
  Status = BS->HandleProtocol (
                 ImageHandle,
                 &gEfiLoadedImageProtocolGuid,
                 (VOID **)&LoadedImage
                 );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Locate loaded image - %r", Status));
    return Status;
  }

  if ((LoadedImage->ImageCodeType == EfiBootServicesCode   ) ||
      (LoadedImage->ImageCodeType == EfiRuntimeServicesCode)) {
    //
    // It is a driver, and then verify it is a TSL (test support library)
    //
    Status = BS->StartImage (
                   ImageHandle,
                   &ExitDataSize,
                   &ExitData
                   );
    if (EFI_ERROR (Status)) {
      EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Start image - %r", Status));
      return Status;
    }

    //
    // Is it a test support library?
    //
    Status = BS->HandleProtocol (
                   ImageHandle,
                   &gEfiTslInitInterfaceGuid,
                   (VOID **)&TslInit
                   );
    if (!EFI_ERROR (Status)) {
      Status = CreateSingleSupportFile (
                 DevicePath,
                 FileName,
                 ImageHandle,
                 TslInit,
                 SupportFile
                 );
      if (EFI_ERROR (Status)) {
        EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Create a support file - %r", Status));
        BS->UnloadImage (ImageHandle);
        return Status;
      }

      return EFI_SUCCESS;
    }

    BS->UnloadImage (ImageHandle);
  }

  //
  // Unsupported file
  //
  EFI_SCT_DEBUG ((EFI_SCT_D_DEBUG, L"Unsupported file"));
  return EFI_UNSUPPORTED;
}


EFI_STATUS
CreateSingleSupportFile (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevicePath,
  IN CHAR16                       *FileName,
  IN EFI_HANDLE                   ImageHandle,
  IN VOID                         *Context,
  OUT EFI_SCT_TEST_FILE           **SupportFile
  )
/*++

Routine Description:

  Create a test support file data structure.

--*/
{
  EFI_STATUS  Status;

  //
  // Check parameter
  //
  if ((DevicePath == NULL) || (FileName == NULL) || (SupportFile == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Allocate memory for the support file
  //
  Status = BS->AllocatePool (
                 EfiBootServicesData,
                 sizeof(EFI_SCT_TEST_FILE),
                 (VOID **)SupportFile
                 );
  if (EFI_ERROR(Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Allocate pool - %r", Status));
    return Status;
  }

  ZeroMem (*SupportFile, sizeof(EFI_SCT_TEST_FILE));

  //
  // Initialize the items of support file
  //
  (*SupportFile)->Signature   = EFI_SCT_TEST_FILE_SIGNATURE;
  (*SupportFile)->Revision    = EFI_SCT_TEST_FILE_REVISION;

  (*SupportFile)->DevicePath  = DuplicateDevicePath (DevicePath);
  (*SupportFile)->FileName    = StrDuplicate (FileName);
  (*SupportFile)->ImageHandle = ImageHandle;
  (*SupportFile)->Type        = EFI_SCT_TEST_FILE_TYPE_SUPPORT;
  (*SupportFile)->Context     = Context;

  //
  // Done
  //
  return EFI_SUCCESS;
}


EFI_STATUS
UnloadSingleSupportFile (
  IN EFI_SCT_TEST_FILE            *SupportFile
  )
/*++

Routine Description:

  Unload a test support file.

--*/
{
  //
  // Free the items of support file
  //
  if (SupportFile->DevicePath != NULL) {
    BS->FreePool (SupportFile->DevicePath);
    SupportFile->DevicePath = NULL;
  }

  if (SupportFile->FileName != NULL) {
    BS->FreePool (SupportFile->FileName);
    SupportFile->FileName = NULL;
  }

  switch (SupportFile->Type) {
  case EFI_SCT_TEST_FILE_TYPE_SUPPORT:
    //
    // Test support file
    //
    BS->UnloadImage (SupportFile->ImageHandle);
    break;

  default:
    //
    // Unsupported file
    //
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Unsupported file"));
    return EFI_UNSUPPORTED;
  }

  //
  // Free the support file itself
  //
  BS->FreePool (SupportFile);

  //
  // Done
  //
  return EFI_SUCCESS;
}


EFI_STATUS
OpenSingleSupportFile (
  IN EFI_GUID                     *Guid,
  OUT VOID                        **Protocol,
  OUT VOID                        **Interface
  )
/*++

Routine Description:

  Open a test support file to get the public interface and private interface.

--*/
{
  EFI_STATUS              Status;
  EFI_LIST_ENTRY          *Link;
  EFI_SCT_TEST_FILE       *SupportFile;
  EFI_TSL_INIT_INTERFACE  *TslInit;
  VOID                    *TempProtocol;
  VOID                    *TempInterface;

  //
  // Walk through all support files
  //
  for (Link = gFT->SupportFileList.Flink; Link != &gFT->SupportFileList; Link = Link->Flink) {
    SupportFile = CR (Link, EFI_SCT_TEST_FILE, Link, EFI_SCT_TEST_FILE_SIGNATURE);

    //
    // Each support file should provide an EFI_TSL_INIT_INTERFACE instance
    //
    TslInit = (EFI_TSL_INIT_INTERFACE *) SupportFile->Context;

    if (CompareGuid (&TslInit->LibraryGuid, Guid) == 0) {
      //
      // Open the support file
      //
      Status = TslInit->Open (
                          TslInit,
                          &gFT->SupportHandle,
                          &TempInterface
                          );
      if (EFI_ERROR (Status)) {
        EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"TSL open - %r", Status));
        return Status;
      }

      //
      // Get the public interface
      //
      Status = BS->HandleProtocol (
                     gFT->SupportHandle,
                     Guid,
                     (VOID **)&TempProtocol
                     );
      if (EFI_ERROR (Status)) {
        EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Locate public interface - %r", Status));
        return Status;
      }

      if (Protocol != NULL) {
        *Protocol = TempProtocol;
      }

      if (Interface != NULL) {
        *Interface = TempInterface;
      }

      //
      // Got it!
      //
      return EFI_SUCCESS;
    }
  }

  //
  // No matched protocol
  //
  return EFI_NOT_FOUND;
}


EFI_STATUS
CloseSingleSupportFile (
  IN EFI_GUID                     *Guid
  )
/*++

Routine Description:

  Close a test support file to free the public interface.

--*/
{
  EFI_STATUS              Status;
  EFI_LIST_ENTRY          *Link;
  EFI_SCT_TEST_FILE       *SupportFile;
  EFI_TSL_INIT_INTERFACE  *TslInit;

  //
  // Walk through all support files
  //
  for (Link = gFT->SupportFileList.Flink; Link != &gFT->SupportFileList; Link = Link->Flink) {
    SupportFile = CR (Link, EFI_SCT_TEST_FILE, Link, EFI_SCT_TEST_FILE_SIGNATURE);

    //
    // Each support file should provide an EFI_TSL_INIT_INTERFACE instance
    //
    TslInit = (EFI_TSL_INIT_INTERFACE *) SupportFile->Context;

    if (CompareGuid (&TslInit->LibraryGuid, Guid) == 0) {
      //
      // Close the support file
      //
      Status = TslInit->Close (
                          TslInit,
                          gFT->SupportHandle
                          );
      if (EFI_ERROR (Status)) {
        EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"TSL close - %r", Status));
        return Status;
      }

      //
      // Got it!
      //
      return EFI_SUCCESS;
    }
  }

  //
  // No matched protocol
  //
  return EFI_NOT_FOUND;
}

EFI_STATUS
LoadProxyFiles (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevicePath,
  IN CHAR16                       *FilePath,
  IN BOOLEAN                      Recursive,
  OUT EFI_LIST_ENTRY              *ProxyFileList
  )
/*++

Routine Description:

  Load all proxy files.

Arguments:

  DevicePath      - Device path of the files.
  FilePath        - Path of the files.
  Recursive       - Recursively.
  ProxyFileList - Pointer to the support file list.

Returns:

  EFI_SUCCESS     - Successfully.
  Other value     - Something failed.

--*/
{
  EFI_STATUS                        Status;
  EFI_HANDLE                        DeviceHandle;
  EFI_FILE_HANDLE                   RootDir;
  EFI_FILE_HANDLE                   ProxyDir;
  CHAR16                            *SubDir;
  CHAR16                            *FileName;
  UINTN                             FileInfoSize;
  EFI_FILE_INFO                     *FileInfo;
  EFI_SCT_TEST_FILE                 *ProxyFile;
  EFI_DEVICE_PATH_PROTOCOL          *RemainderPath;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *Vol;

  //
  // Check parameters
  //
  if ((DevicePath == NULL) || (FilePath == NULL) || (ProxyFileList == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Debug information
  //
  EFI_SCT_DEBUG ((EFI_SCT_D_TRACE, L"Load proxy files from <%s>", FilePath));

  //
  // Locate the device handle from the device path
  //
  RemainderPath = DevicePath;

  Status = BS->LocateDevicePath (
                 &gEfiSimpleFileSystemProtocolGuid,
                 &RemainderPath,
                 &DeviceHandle
                 );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Locate device handle - %r", Status));
    return Status;
  }

  //
  // Locate the volume of the file system
  //
  Status = BS->HandleProtocol (
                 DeviceHandle,
                 &gEfiSimpleFileSystemProtocolGuid,
                 (VOID **)&Vol
                 );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Locate file system - %r", Status));
    return Status;
  }

  //
  // Open the root volume
  //
  Status = Vol->OpenVolume (Vol, &RootDir);
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Open root volume - %r", Status));
    return Status;
  }

  //
  // Open the support directory
  //
  Status = RootDir->Open (
                      RootDir,
                      &ProxyDir,
                      FilePath,
                      EFI_FILE_MODE_READ,
                      EFI_FILE_DIRECTORY
                      );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Open proxy directory - %r", Status));
    RootDir->Close (RootDir);
    return Status;
  }

  RootDir->Close (RootDir);

  //
  // Allocate memory for the entries in the directory
  //
  FileInfoSize = sizeof(EFI_FILE_INFO) + 1024;

  Status = BS->AllocatePool (
                 EfiBootServicesData,
                 FileInfoSize,
                 (VOID **)&FileInfo
                 );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Allocate pool - %r", Status));
    ProxyDir->Close (ProxyDir);
    return Status;
  }

  //
  // Walk through each file in the directory
  //
  while (TRUE) {
    //
    // Read a file entry
    //
    FileInfoSize = sizeof(EFI_FILE_INFO) + 1024;

    Status = ProxyDir->Read (
                           ProxyDir,
                           &FileInfoSize,
                           FileInfo
                           );
    if (EFI_ERROR (Status) || (FileInfoSize == 0)) {
      break;
    }

    if ((FileInfo->Attribute & EFI_FILE_DIRECTORY) == 0) {
      //
      // This is a file
      //
      if (SctStrEndWith (FileInfo->FileName, L".efi")) {
        //
        // Load the support file
        //
        FileName = PoolPrint (L"%s\\%s", FilePath, FileInfo->FileName);
        if (FileName == NULL) {
          EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"PoolPrint: Out of resources"));
          break;
        }

        Status = LoadSingleProxyFile (
                   DevicePath,
                   FileName,
                   &ProxyFile
                   );
        if (EFI_ERROR (Status)) {
          EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Load a proxy file - %r", Status));
          BS->FreePool (FileName);
          continue;
        }

        BS->FreePool (FileName);

        //
        // Add the support file to the support file list
        //
        InsertTailList (ProxyFileList, &ProxyFile->Link);
      }
    } else {
      //
      // This is a directory
      //
      if (Recursive) {
        //
        // Skip the '.' and '..' dir
        //
        if ((StrCmp (FileInfo->FileName, L".")  == 0) ||
            (StrCmp (FileInfo->FileName, L"..") == 0)) {
          continue;
        }

        //
        // Load the support files under the sub directory
        //
        SubDir = PoolPrint (L"%s\\%s", FilePath, FileInfo->FileName);
        if (SubDir == NULL) {
          EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"PoolPrint: Out of resources"));
          break;
        }

        Status = LoadProxyFiles (
                   DevicePath,
                   SubDir,
                   Recursive,
                   ProxyFileList
                   );
        if (EFI_ERROR (Status)) {
          EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Load proxy files - %r", Status));
          BS->FreePool (SubDir);
          continue;
        }

        BS->FreePool (SubDir);
      }
    }
  }

  //
  // Free resources
  //
  BS->FreePool (FileInfo);
  ProxyDir->Close (ProxyDir);

  //
  // Done
  //
  return EFI_SUCCESS;
}


EFI_STATUS
UnloadProxyFiles (
  IN EFI_LIST_ENTRY               *ProxyFileList
  )
/*++

Routine Description:

  Unload all proxy files.

Arguments:

  ProxyFileList - Pointer to the proxy file list.

Returns:

  EFI_SUCCESS     - Successfully.
  Other value     - Something failed.

--*/
{
  EFI_SCT_TEST_FILE   *ProxyFile;

  //
  // Check parameters
  //
  if (ProxyFileList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Debug information
  //
  EFI_SCT_DEBUG ((EFI_SCT_D_TRACE, L"Unload proxy files"));

  //
  // Walk through all test proxy files
  //
  while (!IsListEmpty (ProxyFileList)) {
    ProxyFile = CR (ProxyFileList->Flink, EFI_SCT_TEST_FILE, Link, EFI_SCT_TEST_FILE_SIGNATURE);

    RemoveEntryList (&ProxyFile->Link);
    UnloadSingleProxyFile (ProxyFile);
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}

EFI_STATUS
LoadSingleProxyFile (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevicePath,
  IN CHAR16                       *FileName,
  OUT EFI_SCT_TEST_FILE           **ProxyFile
  )
/*++

Routine Description:

  Load a test proxy file.

--*/
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  ImageHandle;
  EFI_DEVICE_PATH_PROTOCOL    *FileNode;
  EFI_DEVICE_PATH_PROTOCOL    *FilePath;
  EFI_LOADED_IMAGE_PROTOCOL   *LoadedImage;
  UINTN                       ExitDataSize;
  CHAR16                      *ExitData;

  //
  // Check parameters
  //
  if ((DevicePath == NULL) || (FileName == NULL) || (ProxyFile == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Debug information
  //
  EFI_SCT_DEBUG ((EFI_SCT_D_TRACE, L"Load proxy file <%s>", FileName));

  //
  // Add the file path to the device path
  //
  FileNode = FileDevicePath (NULL, FileName);
  if (FileNode == NULL) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"FileDevicePath: Out of resources"));
    return EFI_OUT_OF_RESOURCES;
  }

  FilePath = AppendDevicePath (DevicePath, FileNode);
  if (FilePath == NULL) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"AppendDevicePath: Out of resources"));
    BS->FreePool (FileNode);
    return EFI_OUT_OF_RESOURCES;
  }

  BS->FreePool (FileNode);

  //
  // Load the support file
  //
  Status = BS->LoadImage (
                 FALSE,
                 gFT->ImageHandle,
                 FilePath,
                 NULL,
                 0,
                 &ImageHandle
                 );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Load image - %r", Status));
    BS->FreePool (FilePath);
    return Status;
  }

  BS->FreePool (FilePath);

  //
  // Verify the image is a driver or not
  //
  Status = BS->HandleProtocol (
                 ImageHandle,
                 &gEfiLoadedImageProtocolGuid,
                 (VOID **)&LoadedImage
                 );
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Locate loaded image - %r", Status));
    return Status;
  }

  if ((LoadedImage->ImageCodeType == EfiBootServicesCode   ) ||
      (LoadedImage->ImageCodeType == EfiRuntimeServicesCode)) {

    //
    // It is a driver
    //
    Status = BS->StartImage (
                   ImageHandle,
                   &ExitDataSize,
                   &ExitData
                   );
    if (EFI_ERROR (Status)) {
      EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Start image - %r", Status));
      return Status;
    }

	EFI_SCT_DEBUG ((EFI_SCT_D_TRACE, L"point 1: file <%s>", FileName));

    Status = CreateSingleProxyFile (
               DevicePath,
               FileName,
               ImageHandle,
               NULL,
               ProxyFile
               );
    if (EFI_ERROR (Status)) {
      EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Create a support file - %r", Status));
      BS->UnloadImage (ImageHandle);
      EFI_SCT_DEBUG ((EFI_SCT_D_TRACE, L"point 2: file <%s>", FileName));
      return Status;
    }

	EFI_SCT_DEBUG ((EFI_SCT_D_TRACE, L"Return EFI_SUCCESS", FileName));
    return EFI_SUCCESS;
  }

  //
  // Unsupported file
  //
  EFI_SCT_DEBUG ((EFI_SCT_D_DEBUG, L"Unsupported file"));
  return EFI_UNSUPPORTED;
}


EFI_STATUS
CreateSingleProxyFile (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevicePath,
  IN CHAR16                       *FileName,
  IN EFI_HANDLE                   ImageHandle,
  IN VOID                         *Context,
  OUT EFI_SCT_TEST_FILE           **ProxyFile
  )
/*++

Routine Description:

  Create a test proxy file data structure.

--*/
{
  EFI_STATUS  Status;

  //
  // Check parameter
  //
  if ((DevicePath == NULL) || (FileName == NULL) || (ProxyFile == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Allocate memory for the support file
  //
  Status = BS->AllocatePool (
                 EfiBootServicesData,
                 sizeof(EFI_SCT_TEST_FILE),
                 (VOID **)ProxyFile
                 );
  if (EFI_ERROR(Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Allocate pool - %r", Status));
    return Status;
  }

  ZeroMem (*ProxyFile, sizeof(EFI_SCT_TEST_FILE));

  //
  // Initialize the items of support file
  //
  (*ProxyFile)->Signature   = EFI_SCT_TEST_FILE_SIGNATURE;
  (*ProxyFile)->Revision    = EFI_SCT_TEST_FILE_REVISION;

  (*ProxyFile)->DevicePath  = DuplicateDevicePath (DevicePath);
  (*ProxyFile)->FileName    = StrDuplicate (FileName);
  (*ProxyFile)->ImageHandle = ImageHandle;
  (*ProxyFile)->Type        = EFI_SCT_TEST_FILE_TYPE_SUPPORT;
  (*ProxyFile)->Context     = Context;

  //
  // Done
  //
  return EFI_SUCCESS;
}


EFI_STATUS
UnloadSingleProxyFile (
  IN EFI_SCT_TEST_FILE            *ProxyFile
  )
/*++

Routine Description:

  Unload a test proxy file.

--*/
{
  //
  // Free the items of proxy file
  //
  if (ProxyFile->DevicePath != NULL) {
    BS->FreePool (ProxyFile->DevicePath);
    ProxyFile->DevicePath = NULL;
  }

  if (ProxyFile->FileName != NULL) {
    BS->FreePool (ProxyFile->FileName);
    ProxyFile->FileName = NULL;
  }

  switch (ProxyFile->Type) {
  case EFI_SCT_TEST_FILE_TYPE_SUPPORT:
    //
    // Test proxy file
    //
    BS->UnloadImage (ProxyFile->ImageHandle);
    break;

  default:
    //
    // Unsupported file
    //
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Unsupported file"));
    return EFI_UNSUPPORTED;
  }

  //
  // Free the proxy file itself
  //
  BS->FreePool (ProxyFile);

  //
  // Done
  //
  return EFI_SUCCESS;
}