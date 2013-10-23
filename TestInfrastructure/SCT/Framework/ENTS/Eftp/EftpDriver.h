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
 
  EftpDriver.h
 
Abstract:
 
--*/

#ifndef _EFI_EFTP_DRIVER_H_
#define _EFI_EFTP_DRIVER_H_

#include "EfiPrintLib.h"
#include "Eftp.h"

#include EFI_PROTOCOL_DEFINITION (ManagedNetwork)

#include EFI_PROTOCOL_DEFINITION (ComponentName)
#define EFTP_SB_PRIVATE_SIGNATURE EFI_SIGNATURE_32 ('M', 'T', 'S', 'B')
#define EFTP_SB_PRIVATE_FROM_THIS(a) \
  CR ( \
  a, \
  EFTP_SERVICE_BINDING_PRIVATE, \
  ServiceBinding, \
  EFTP_SB_PRIVATE_SIGNATURE \
  )

//
// Private data used by eftp service binding protocol
//
typedef struct _EFTP_SERVICE_BINDING_PRIVATE {
  UINTN                         Signature;
  EFI_SERVICE_BINDING_PROTOCOL  ServiceBinding;

  //
  // The controller handle eftp service binding protocol is installed on
  //
  EFI_HANDLE                    ControllerHandle;

  //
  // The handle in this driver binding protocol. It is the image handle in most cases
  //
  EFI_HANDLE                    ImageHandle;

  //
  // The instance handle on which ManagedNetwork Protocol is installed.
  //
  EFI_HANDLE                    MnpChildHandle;

  //
  // This is the pre-allocated MNP protocol, will be consumed by first EFTP instance.
  //
  EFI_MANAGED_NETWORK_PROTOCOL  *ManagedNetwork;

  //
  // The configuration for the ManagedNetwork instance.
  // (synchronization needed)
  //
  //  EFI_MANAGED_NETWORK_CONFIG_DATA       MnpConfigData;
  //
  // data member used to manage the children created from eftp service binding protocol
  //
  UINT16                        ChildrenNum;
  NET_LIST_ENTRY                ChildrenList;
  NET_LOCK                      ChildrenListLock;
} EFTP_SERVICE_BINDING_PRIVATE;

EFI_STATUS
EftpDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
/*++

Routine Description:

  Test to see if this driver supports ControllerHandle. 

Arguments:

  This                - Protocol instance pointer.
  Controller          - Handle of device to test.
  RemainingDevicePath - Optional parameter use to pick a specific child device to test.

Returns:

    EFI_SUCCES          - This driver supports this device
    other               - This driver does not support this device

--*/
;

EFI_STATUS
EftpDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
/*++

Routine Description:

  Start the Eftp driver.

Arguments:

  This                    - Protocol instance pointer.
  Controller              - Handle of device to test.
  RemainingDevicePath     - Optional parameter use to pick a specific child device to start.

Returns:

  EFI_ALREADY_STARTED     - The device is already started.
  EFI_OUT_OF_RESOURCES    - No enough resource
  EFI_SUCCESS             - Successfully started the driver.

--*/
;

EFI_STATUS
EftpDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN  EFI_HANDLE                     Controller,
  IN  UINTN                          NumberOfChildren,
  IN  EFI_HANDLE                     *ChildHandleBuffer
  )
/*++
 
  Routine Description:
    Stop this driver on ControllerHandle.
 
  Arguments:
    This                  - Protocol instance pointer.
    Controller            - Handle of device to stop driver on 
    NumberOfChildren      - Number of Handles in ChildHandleBuffer. If number of 
                            children is zero stop the entire bus driver.
    ChildHandleBuffer     - List of Child Handles to Stop.
 
  Returns:
    EFI_SUCCESS           - This driver is removed ControllerHandle
    EFI_DEVICE_ERROR      - Failed to remove the controller
    Other                 - Failed to remove the controller
 
--*/
;

EFI_STATUS
EftpServiceBindingCreateChild (
  IN EFI_SERVICE_BINDING_PROTOCOL  *This,
  IN OUT EFI_HANDLE                *ChildHandle
  )
/*++

Routine Description:

  Create a child of Eftp driver.

Arguments:

  This           - The service binding handle of Eftp driver.
  ChildHandle    - Variable to return the child handler

Returns:

  EFI_ACCESS_DENIED      - Protocol is busy
  EFI_OUT_OF_RESOURCES   - No enough resource
  EFI_SUCCESS            - Successfully created a child of Eftp driver

--*/
;

EFI_STATUS
EftpServiceBindingDestroyChild (
  IN EFI_SERVICE_BINDING_PROTOCOL      *This,
  IN EFI_HANDLE                        ChildHandle
  )
/*++

Routine Description:

  Destory a Eftp child

Arguments:

  This            - The session's service binding handler
  ChildHandle     - Child to destory
  
Returns:

  EFI_ACCESS_DENIED        - Protocol is busy
  EFI_SUCCESS              - Successfully created a child of Eftp driver

--*/
;

#endif
