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
/**
 *  This file is a test image for the Image Boot Services Test
 */
/*++

Module Name:
    CombinationImage4.c

Abstract:       
    for Image Boot Services Black Box Test

--*/

#include "ProtocolDefinition.h"
#include "Misc.h"
#include "EfiTestUtilityLib.h"
#include "EfiTestLib.h"

//
// data definition here
//

EFI_STATUS
InitializeCombinationImage4 (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

EFI_DRIVER_ENTRY_POINT(InitializeCombinationImage4)

EFI_STATUS
InitializeCombinationImage4 (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                  Status;  
  EFI_HANDLE                  Handle;
  EFI_DEVICE_PATH_PROTOCOL    *FilePath;
  EFI_HANDLE                  AnotherImageHandle;
  CHAR16                      *ExitData;
  UINTN                       ExitDataSize;
  
  //
  // Init
  //
  FilePath = NULL;
  Handle = NULL;
  AnotherImageHandle = NULL;
  Status = EFI_SUCCESS;
  ExitData = NULL;

  EfiInitializeTestLib (ImageHandle, SystemTable);
  
  InitializeLib (ImageHandle, SystemTable);
  
  gtBS->InstallProtocolInterface (
                      &Handle,
                      &mImageTestNoInterfaceProtocol4Guid,
                      EFI_NATIVE_INTERFACE,
                      NULL
                      );
                      
  Status = ImageTestComposeSimpleFilePath (
                   NULL,ImageHandle,
                   COMBINATION_IMAGE_5_NAME,
                   &FilePath
                   );
  if (EFI_ERROR(Status)) {
    goto Done;
  }
  
  //
  // load image 5
  //
  Status = gtBS->LoadImage (
                    FALSE,
                    ImageHandle,
                    FilePath,
                    NULL,
                    0,
                    &AnotherImageHandle
                    );
  if (EFI_ERROR(Status)) {
    goto Done;
  }
  
  //
  // start image 5
  //
  Status = gtBS->StartImage (AnotherImageHandle, &ExitDataSize, &ExitData);
  
Done:

  if (Handle != NULL) {
    gtBS->UninstallProtocolInterface (
              Handle,
              &mImageTestNoInterfaceProtocol4Guid,
              NULL
              );
  }
  
  if (ExitData != NULL) {
    gtBS->FreePool (ExitData);
  }
  
  if (FilePath != NULL) {
    gtBS->FreePool (FilePath);
  }
  
  if (AnotherImageHandle != NULL) {
    gtBS->UnloadImage (AnotherImageHandle);
  }
  
  gtBS->Exit (ImageHandle,Status,0,NULL);

  //
  // actually should not arrive here. Just to pass compiling.
  //
  return EFI_SUCCESS;
}