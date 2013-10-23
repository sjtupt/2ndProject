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

  Mtftp6ServiceBindingENTSTestCase.c

Abstract:

--*/

#include "Mtftp6ServiceBindingENTSTestCase.h"

//
// Mtftp6ServiceBinding.CreateChild
//
static EFI_HANDLE *Mtftp6ServiceBindingCreateChildHandle;
static EFI_STATUS *Mtftp6ServiceBindingCreateChildStatus;

ENTS_ARG_FIELD    gMtftp6ServiceBindingCreateChildArgField[] = {
  {
    POINTER,
    &Mtftp6ServiceBindingCreateChildHandle
  },
  {
    POINTER,
    &Mtftp6ServiceBindingCreateChildStatus
  },
  0
};

EFI_STATUS
Mtftp6ServiceBindingCreateChild_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Mtftp6ServiceBinding CreateChild Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS   - Operation succeeded.
  EFI_NOT_FOUND - No instance was found.

--*/
{
  EFI_MTFTP6_SERVICE_BINDING_PROTOCOL *Mtftp6ServiceBinding;
  UINTN                               NoHandles;
  UINTN                               HandleIndex;
  EFI_HANDLE                          *HandleBuffer;
  EFI_ENTS_PROTOCOL                   *Interface;
  EFI_STATUS                          Status;

  Mtftp6ServiceBinding = (EFI_MTFTP6_SERVICE_BINDING_PROTOCOL *) ClientInterface;

  *Mtftp6ServiceBindingCreateChildStatus = Mtftp6ServiceBinding->CreateChild (
                                                                  Mtftp6ServiceBinding,
                                                                  Mtftp6ServiceBindingCreateChildHandle
                                                                  );
  if (EFI_ERROR (*Mtftp6ServiceBindingCreateChildStatus)) {
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
                    &Interface
                    );
    if (EFI_ERROR (Status)) {
      goto Done;
    }
    //
    // match the instance
    //
    if (EntsStrCmp (Interface->ClientName, L"Mtftp6") == 0) {
      //
      // find it
      //
      Interface->ClientHandle = *Mtftp6ServiceBindingCreateChildHandle;
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
// Mtftp6ServiceBinding.DestroyChild
//
static EFI_HANDLE Mtftp6ServiceBindingDestroyChildHandle;
static EFI_STATUS *Mtftp6ServiceBindingDestroyChildStatus;

ENTS_ARG_FIELD    gMtftp6ServiceBindingDestroyChildArgField[] = {
  {
    OCTETN,
    &Mtftp6ServiceBindingDestroyChildHandle
  },
  {
    POINTER,
    &Mtftp6ServiceBindingDestroyChildStatus
  },
  0
};

EFI_STATUS
Mtftp6ServiceBindingDestroyChild_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Mtftp6ServiceBinding DestroyChild Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS   - Operation succeeded.
  EFI_NOT_FOUND - No instance was found.

--*/
{
  EFI_MTFTP6_SERVICE_BINDING_PROTOCOL *Mtftp6ServiceBinding;
  UINTN                               NoHandles;
  UINTN                               HandleIndex;
  EFI_HANDLE                          *HandleBuffer;
  EFI_ENTS_PROTOCOL                   *Interface;
  EFI_STATUS                          Status;

  Mtftp6ServiceBinding = (EFI_MTFTP6_SERVICE_BINDING_PROTOCOL *) ClientInterface;

  *Mtftp6ServiceBindingDestroyChildStatus = Mtftp6ServiceBinding->DestroyChild (
                                                                    Mtftp6ServiceBinding,
                                                                    Mtftp6ServiceBindingDestroyChildHandle
                                                                    );
  if (EFI_ERROR (*Mtftp6ServiceBindingDestroyChildStatus)) {
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
                    &Interface
                    );
    if (EFI_ERROR (Status)) {
      goto Done;
    }
    //
    // match the instance
    //
    if (EntsStrCmp (Interface->ClientName, L"Mtftp6") == 0) {
      if (NULL == Interface->ClientHandle) {
        continue;
      }
      //
      // find it
      //
      if (EntsCompareMem (Interface->ClientHandle, &Mtftp6ServiceBindingDestroyChildHandle, sizeof (EFI_HANDLE))) {
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
