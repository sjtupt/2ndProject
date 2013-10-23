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

  Ip6ENTSTestCase.h

Abstract:

--*/

#ifndef _IP6_ENTS_TESTCASE_H_
#define _IP6_ENTS_TESTCASE_H_

#include "Efi.h"
#include "EfiDriverLib.h"
#include "EntsProtocol.h"
#include "Eas.h"
#include "EntsLib.h"
#include "Ip6.h"

EFI_STATUS
Ip6GetModeData_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Ip6 GetModeData Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
;

EFI_STATUS
Ip6Configure_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Ip6 Configure Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
;

EFI_STATUS
Ip6Groups_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Ip6 Groups Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
;

EFI_STATUS
Ip6Neighbors_EntsTest (
  VOID *ClientInterface
  );

EFI_STATUS
Ip6Routes_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Ip6 Routes Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
;

EFI_STATUS
Ip6Transmit_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Ip6 Transmit Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
;

EFI_STATUS
Ip6Receive_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Ip6 Receive Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
;

EFI_STATUS
Ip6Cancel_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Ip6 Cancel Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
;

EFI_STATUS
Ip6Poll_EntsTest (
  VOID *ClientInterface
  )
/*++

Routine Description:

  Ip6 Poll Ents test entry point.

Arguments:

  ClientInterface - RPC calling interface.

Returns:

  EFI_SUCCESS - Operation succeeded.

--*/
;

extern CHAR16         *gIp6RuntimeInfo;
extern UINTN          gIp6RuntimeInfoSize;

extern ENTS_ARG_FIELD gIp6GetModeDataArgField[];
extern ENTS_ARG_FIELD gIp6ConfigureArgField[];
extern ENTS_ARG_FIELD gIp6GroupsArgField[];
extern ENTS_ARG_FIELD gIp6RoutesArgField[];
extern ENTS_ARG_FIELD gIp6NeighborsArgField[];
extern ENTS_ARG_FIELD gIp6TransmitArgField[];
extern ENTS_ARG_FIELD gIp6ReceiveArgField[];
extern ENTS_ARG_FIELD gIp6CancelArgField[];
extern ENTS_ARG_FIELD gIp6PollArgField[];

#endif
