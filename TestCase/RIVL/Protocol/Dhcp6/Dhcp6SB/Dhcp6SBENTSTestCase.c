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

  Dhcp6SBENTSTestCase.c

Abstract:

--*/

#include "Dhcp6SBENTSTestCase.h"

//
// MnpServiceBinding.CreateChild
//
static EFI_HANDLE *Dhcp6CreateChildHandle;
static EFI_STATUS *Dhcp6CreateChildStatus;

ENTS_ARG_FIELD    gDhcp6CreateChildArgField[] = {
  {
    POINTER,
    &Dhcp6CreateChildHandle
  },
  {
    POINTER,
    &Dhcp6CreateChildStatus
  },
  0
};

EFI_STATUS
Dhcp6CreateChild_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Dhcp6 CreateChild Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS   - Operation succeeded.
  EFI_NOT_FOUND - No instance was found.

--*/
{
  EFI_DHCP6_SERVICE_BINDING_PROTOCOL  *Dhcp6;
  UINTN                               NoHandles;
  UINTN                               HandleIndex;
  EFI_HANDLE                          *HandleBuffer;
  EFI_ENTS_PROTOCOL                   *Interface;
  EFI_STATUS                          Status;

  Dhcp6 = (EFI_DHCP6_SERVICE_BINDING_PROTOCOL *) ClientInterface;

  *Dhcp6CreateChildStatus = Dhcp6->CreateChild (
                                    Dhcp6,
                                    Dhcp6CreateChildHandle
                                    );

  if (EFI_ERROR (*Dhcp6CreateChildStatus)) {
    return EFI_SUCCESS;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiEntsProtocolGuid,
                  NULL,
                  &NoHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || (NoHandles == 0)) {
    return EFI_NOT_FOUND;
  }
  //
  // Walk through each instance need to be tested
  //
  for (HandleIndex = 0; HandleIndex < NoHandles; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gEfiEntsProtocolGuid,
                    (VOID **)&Interface
                    );
    if (EFI_ERROR (Status)) {
      goto Done;
    }
    //
    // match the instance
    //
    if (EntsStrCmp (Interface->ClientName, L"Dhcp6") == 0) {
      //
      // find it
      //
      Interface->ClientHandle = *Dhcp6CreateChildHandle;
      Status = EFI_SUCCESS;
      goto Done;
    }
  }

  Status = EFI_NOT_FOUND;
Done:
  if (HandleBuffer != NULL) {
    gBS->FreePool(HandleBuffer);
  }
  return Status;
}

//
// Dhcp6ServiceBinding.DestroyChild
//
static EFI_HANDLE Dhcp6DestroyChildHandle;
static EFI_STATUS *Dhcp6DestroyChildStatus;

ENTS_ARG_FIELD    gDhcp6DestroyChildArgField[] = {
  {
    OCTETN,
    &Dhcp6DestroyChildHandle
  },
  {
    POINTER,
    &Dhcp6DestroyChildStatus
  },
  0
};

EFI_STATUS
Dhcp6DestroyChild_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Dhcp6 DestroyChild Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS   - Operation succeeded.
  EFI_NOT_FOUND - No instance was found.

--*/
{
  EFI_DHCP6_SERVICE_BINDING_PROTOCOL  *Dhcp6;
  UINTN                               NoHandles;
  UINTN                               HandleIndex;
  EFI_HANDLE                          *HandleBuffer;
  EFI_ENTS_PROTOCOL                   *Interface;
  EFI_STATUS                          Status;

  Dhcp6 = (EFI_DHCP6_SERVICE_BINDING_PROTOCOL *) ClientInterface;
  *Dhcp6DestroyChildStatus = Dhcp6->DestroyChild (
                                      Dhcp6,
                                      Dhcp6DestroyChildHandle
                                      );
  if (EFI_ERROR (*Dhcp6DestroyChildStatus)) {
    return EFI_SUCCESS;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiEntsProtocolGuid,
                  NULL,
                  &NoHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || (NoHandles == 0)) {
    return EFI_NOT_FOUND;
  }
  //
  // Walk through each instance need to be tested
  //
  for (HandleIndex = 0; HandleIndex < NoHandles; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gEfiEntsProtocolGuid,
                    (VOID **)&Interface
                    );
    if (EFI_ERROR (Status)) {
      goto Done;
    }
    //
    // match the instance
    //
    if (EntsStrCmp (Interface->ClientName, L"Dhcp6") == 0) {
      if (NULL == Interface->ClientHandle) {
        continue;
      }
      //
      // find it
      //
      if (EntsCompareMem (Interface->ClientHandle, &Dhcp6DestroyChildStatus, sizeof (EFI_HANDLE))) {
        Interface->ClientHandle = NULL;
      }
      Status = EFI_SUCCESS;
      goto Done;
    }
  }

  Status = EFI_NOT_FOUND;
Done:
  if (HandleBuffer != NULL) {
    gBS->FreePool(HandleBuffer);
  }
  return Status;
}
