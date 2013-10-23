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

  Ip6ConfigENTSTest.c

Abstract:

--*/

#include "Efi.h"
#include "EfiDriverLib.h"
#include "Ip6ConfigENTSTestCase.h"
#include EFI_PROTOCOL_DEFINITION (LoadedImage)
#ifdef EFIARM
extern EFI_GUID  gEfiIp6ConfigProtocolGuid;
#else
EFI_GUID  gEfiIp6ConfigProtocolGuid = EFI_IP6_CONFIG_PROTOCOL_GUID;
#endif
static CHAR16     gIp6ConfigProtocolName[] = L"Ip6Config";

CHAR16            *gIp6ConfigRuntimeInfo;
UINTN             gIp6ConfigRuntimeInfoSize;

ENTS_INTERFACE    gIp6ConfigEntsInterfaceList[] = {
  {
    L"SetData",
    Ip6ConfigSetData_EntsTest,
    gIp6ConfigSetDataArgField
  },
  {
    L"GetData",
    Ip6ConfigGetData_EntsTest,
    gIp6ConfigGetDataArgField
  },
  {
    L"RegisterDataNotify",
    Ip6ConfigRegisterDataNotify_EntsTest,
    gIp6ConfigRegisterDataNotifyArgField
  },
  {
    L"UnregisterDataNotify",
	Ip6ConfigUnregisterDataNotify_EntsTest,
	gIp6ConfigUnregisterDataNotifyArgField
  },
  0
};

EFI_ENTS_PROTOCOL *gIp6ConfigEntsProtocolInterface = NULL;

EFI_STATUS
Ip6ConfigENTSTestUnload (
  IN EFI_HANDLE                ImageHandle
  );

EFI_DRIVER_ENTRY_POINT (Ip6ConfigENTSTestMain)

EFI_STATUS
Ip6ConfigENTSTestMain (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
/*++

Routine Description:

  Entry point for Ip6Config protocol ENTS Test.

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

  LoadedImage->Unload = Ip6ConfigENTSTestUnload;

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (EFI_ENTS_PROTOCOL),
                  (VOID **)&gIp6ConfigEntsProtocolInterface
                  );
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  Status = EntsNetworkServiceBindingGetControllerHandle (
             &gEfiIp6ConfigProtocolGuid,
             &ClientHandle
             );
  if (EFI_ERROR(Status)) {
  	goto Error;
  }

  gIp6ConfigEntsProtocolInterface->ClientName         = gIp6ConfigProtocolName;
  gIp6ConfigEntsProtocolInterface->ClientAttribute    = ENTS_PROTOCOL_ATTRIBUTE_PROTOCOL;
  gIp6ConfigEntsProtocolInterface->ClientGuid         = &gEfiIp6ConfigProtocolGuid;
  gIp6ConfigEntsProtocolInterface->ClientHandle       = ClientHandle;
  gIp6ConfigEntsProtocolInterface->ClientInterface    = NULL;
  gIp6ConfigEntsProtocolInterface->EntsInterfaceList  = gIp6ConfigEntsInterfaceList;
  gIp6ConfigEntsProtocolInterface->RuntimeInfo        = gIp6ConfigRuntimeInfo;
  gIp6ConfigEntsProtocolInterface->RuntimeInfoSize    = gIp6ConfigRuntimeInfoSize;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiEntsProtocolGuid,
                  gIp6ConfigEntsProtocolInterface,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  return EFI_SUCCESS;

Error:
  if (gIp6ConfigEntsProtocolInterface != NULL) {
    gBS->FreePool (gIp6ConfigEntsProtocolInterface);
  }

  return Status;
}

EFI_STATUS
Ip6ConfigENTSTestUnload (
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
                  gIp6ConfigEntsProtocolInterface,
                  NULL
                  );

  if (gIp6ConfigEntsProtocolInterface != NULL) {
    gBS->FreePool (gIp6ConfigEntsProtocolInterface);
  }

  return Status;
}
