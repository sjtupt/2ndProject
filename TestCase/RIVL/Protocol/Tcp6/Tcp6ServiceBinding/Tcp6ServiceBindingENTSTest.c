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

  Tcp6ServiceBindingENTSTest.c

Abstract:

--*/

#include "Efi.h"
#include "EfiDriverLib.h"
#include "Tcp6ServiceBindingENTSTestCase.h"
#include EFI_PROTOCOL_DEFINITION (LoadedImage)

static CHAR16     gTcp6ServiceBindingProtocolName[] = L"Tcp6ServiceBinding";

CHAR16            *gTcp6ServiceBindingRuntimeInfo;
UINTN             gTcp6ServiceBindingRuntimeInfoSize;

ENTS_INTERFACE    gTcp6ServiceBindingEntsInterfaceList[] = {
  {
    L"CreateChild",
    Tcp6ServiceBindingCreateChild_EntsTest,
    gTcp6ServiceBindingCreateChildArgField
  },
  {
    L"DestroyChild",
    Tcp6ServiceBindingDestroyChild_EntsTest,
    gTcp6ServiceBindingDestroyChildArgField
  },
  0
};

EFI_ENTS_PROTOCOL *gTcp6ServiceBindingEntsProtocolInterface = NULL;

EFI_STATUS
Tcp6ServiceBindingENTSTestUnload (
  IN EFI_HANDLE                ImageHandle
  );

EFI_DRIVER_ENTRY_POINT (Tcp6ServiceBindingENTSTestMain)

EFI_STATUS
Tcp6ServiceBindingENTSTestMain (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
/*++

Routine Description:

  Entry point for Tcp6 service binding protocol ENTS Test.

Arguments:

  ImageHandle           - The image handle.
  SystemTable           - The system table.

Returns:

  EFI_SUCCESS - Operation succeeded.
  Others      - Some failure happened.

--*/
{
  EFI_STATUS                Status;
  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
  EFI_HANDLE                ClientHandle;

  EfiInitializeDriverLib (ImageHandle, SystemTable);
  EfiInitializeEntsLib (ImageHandle, SystemTable);

  gBS->HandleProtocol (
        ImageHandle,
        &gEfiLoadedImageProtocolGuid,
        (VOID **) &LoadedImage
        );

  LoadedImage->Unload = Tcp6ServiceBindingENTSTestUnload;

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (EFI_ENTS_PROTOCOL),
                  &gTcp6ServiceBindingEntsProtocolInterface
                  );
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  Status = EntsNetworkServiceBindingGetControllerHandle (
             &gEfiTcp6ServiceBindingProtocolGuid, 
             &ClientHandle
             );
  if (EFI_ERROR(Status)) {
  	goto Error;
  }

  gTcp6ServiceBindingEntsProtocolInterface->ClientName        = gTcp6ServiceBindingProtocolName;
  gTcp6ServiceBindingEntsProtocolInterface->ClientAttribute   = ENTS_PROTOCOL_ATTRIBUTE_PROTOCOL;
  gTcp6ServiceBindingEntsProtocolInterface->ClientGuid        = &gEfiTcp6ServiceBindingProtocolGuid;
  gTcp6ServiceBindingEntsProtocolInterface->ClientHandle      = ClientHandle;
  gTcp6ServiceBindingEntsProtocolInterface->ClientInterface   = NULL;
  gTcp6ServiceBindingEntsProtocolInterface->EntsInterfaceList = gTcp6ServiceBindingEntsInterfaceList;
  gTcp6ServiceBindingEntsProtocolInterface->RuntimeInfo       = gTcp6ServiceBindingRuntimeInfo;
  gTcp6ServiceBindingEntsProtocolInterface->RuntimeInfoSize   = gTcp6ServiceBindingRuntimeInfoSize;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiEntsProtocolGuid,
                  gTcp6ServiceBindingEntsProtocolInterface,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  return EFI_SUCCESS;

Error:
  if (gTcp6ServiceBindingEntsProtocolInterface != NULL) {
    gBS->FreePool (gTcp6ServiceBindingEntsProtocolInterface);
  }

  return Status;
}

EFI_STATUS
Tcp6ServiceBindingENTSTestUnload (
  IN EFI_HANDLE                ImageHandle
  )
/*++

Routine Description:

  Unload routine.

Arguments:

  ImageHandle - The image handle.

Returns:

  EFI_SUCCESS - Operation succeeded.
  Others      - Some failure happened.

--*/
{
  EFI_STATUS  Status;

  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ImageHandle,
                  &gEfiEntsProtocolGuid,
                  gTcp6ServiceBindingEntsProtocolInterface,
                  NULL
                  );

  if (gTcp6ServiceBindingEntsProtocolInterface != NULL) {
    gBS->FreePool (gTcp6ServiceBindingEntsProtocolInterface);
  }

  return Status;
}
