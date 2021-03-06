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

  DriverConfigurationBBTest.h

Abstract:
    head file of test driver of EFI Driver Configuration Protocol Test

--*/

#ifndef _DRIVER_CONFIGURATION_TEST_H
#define _DRIVER_CONFIGURATION_TEST_H


#include "Efi.h"
#include "EfiCommonLib.h"
#include "EfiTestUtilityLib.h"
#include "EfiTestLib.h"
#include "DriverConfigurationProtocol.h"
#include "Guid.h"

#define EFI_DRIVER_CONFIGURATION_TEST_REVISION 0x00010000

extern EFI_HANDLE   mImageHandle;

//////////////////////////////////////////////////////////////////////////////
//
// Entry GUIDs for Conformance Test
//
#define EFI_DRIVER_CONFIGURATION_PROTOCOL_TEST_ENTRY_GUID0101 \
 {0x24a8f4cb, 0x4415, 0x4855, {0xb8, 0x45, 0xce, 0xaa, 0x83, 0xcc, 0x26, 0xa0} }

#define EFI_DRIVER_CONFIGURATION_PROTOCOL_TEST_ENTRY_GUID0102 \
 {0xfe195e07, 0xd42c, 0x48f6, {0x9b, 0x4f, 0xd8, 0x54, 0xc0, 0x7b, 0x3a, 0xf1} }

#define EFI_DRIVER_CONFIGURATION_PROTOCOL_TEST_ENTRY_GUID0103 \
 {0xcd0131e2, 0x2d28, 0x4150, {0x9c, 0x41, 0xda, 0x7, 0x83, 0x71, 0x22, 0x68} }

//////////////////////////////////////////////////////////////////////////////

//
// TestNoInterfaceProtocol1
//
#define TEST_NO_INTERFACE_PROTOCOL_1_GUID \
  {0x7362363, 0x3617, 0x4d92, 0xa1, 0xb1, 0x1d, 0x62, 0xa5, 0x78, 0x91, 0xa5 };

extern EFI_GUID mTestNoInterfaceProtocol1Guid;

//
// TestNoInterfaceProtocol2
//
#define TEST_NO_INTERFACE_PROTOCOL_2_GUID \
  {0x3fd2484, 0xa340, 0x41a7, 0xba, 0x13, 0xb4, 0x93, 0xb0, 0x67, 0xdb, 0x4c };

extern EFI_GUID mTestNoInterfaceProtocol2Guid;


//
// functions declaration
//

EFI_STATUS
InitializeBBTestDriverConfiguration (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

EFI_STATUS
BBTestDriverConfigurationUnload (
  IN EFI_HANDLE       ImageHandle
  );

//
// Conformance test functions
//

//
// TDS 5.2.1
//
EFI_STATUS
BBTestSetOptionsConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.2.2
//
EFI_STATUS
BBTestOptionsValidConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.2.3
//
EFI_STATUS
BBTestForceDefaultsConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// Checkpoint functions for Conformance tests
//

//
// TDS 5.2.1.2.1
//
EFI_STATUS
BBTestSetOptionsConformanceTestCheckpoint1 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_DRIVER_CONFIGURATION_PROTOCOL     *DriverConfig
  );

//
// TDS 5.2.1.2.2
//
EFI_STATUS
BBTestSetOptionsConformanceTestCheckpoint2 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_DRIVER_CONFIGURATION_PROTOCOL     *DriverConfig
  );

//
// TDS 5.2.1.2.3
//
EFI_STATUS
BBTestSetOptionsConformanceTestCheckpoint3 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_DRIVER_CONFIGURATION_PROTOCOL     *DriverConfig
  );

//
// TDS 5.2.2.2.1
//
EFI_STATUS
BBTestOptionsValidConformanceTestCheckpoint1 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_DRIVER_CONFIGURATION_PROTOCOL     *DriverConfig
  );

//
// TDS 5.2.2.2.2
//
EFI_STATUS
BBTestOptionsValidConformanceTestCheckpoint2 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_DRIVER_CONFIGURATION_PROTOCOL     *DriverConfig
  );

//
// TDS 5.2.3.2.1
//
EFI_STATUS
BBTestForceDefaultsConformanceTestCheckpoint1 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_DRIVER_CONFIGURATION_PROTOCOL     *DriverConfig
  );

//
// TDS 5.2.3.2.2
//
EFI_STATUS
BBTestForceDefaultsConformanceTestCheckpoint2 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_DRIVER_CONFIGURATION_PROTOCOL     *DriverConfig
  );

#endif
