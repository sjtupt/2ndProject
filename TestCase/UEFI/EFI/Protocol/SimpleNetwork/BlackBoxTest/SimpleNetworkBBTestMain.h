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

  SimpleNetworkBBTestMain.h

Abstract:

  BB test header file of SimpleNetwork Protocol

--*/


#ifndef _SIMPLE_NETWORK_H_
#define _SIMPLE_NETWORK_H_

#include "Efi.h"
#include "Guid.h"
#include "EfiTestUtilityLib.h"
#include "EfiTestLib.h"
#include "SimpleNetworkProtocol.h"
#include "EfiDriverLib.h"

#include EFI_TEST_PROTOCOL_DEFINITION(TestLoggingLibrary)

#define SIMPLE_NETWORK_TEST_REVISION 0x00010001

//
// Prototypes of Function Test
//

//
// TDS 4.1
//
EFI_STATUS
BBTestStartFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.2
//
EFI_STATUS
BBTestStopFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.3
//
EFI_STATUS
BBTestInitializeFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.4
//
EFI_STATUS
BBTestResetFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.5
//
EFI_STATUS
BBTestShutdownFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.6
//
EFI_STATUS
BBTestReceiveFilterFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.7
//
EFI_STATUS
BBTestStationAddressFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.8
//
EFI_STATUS
BBTestStatisticsFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.9
//
EFI_STATUS
BBTestMCastIPtoMACFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.10
//
EFI_STATUS
BBTestNVDataFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.11
//
EFI_STATUS
BBTestGetStatusFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.12
//
EFI_STATUS
BBTestTransmitFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 4.13
//
EFI_STATUS
BBTestReceiveFunctionTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// Prototypes of Conformance Test
//

//
// TDS 5.1
//
EFI_STATUS
BBTestStartConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.2
//
EFI_STATUS
BBTestStopConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.3
//
EFI_STATUS
BBTestInitializeConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.4
//
EFI_STATUS
BBTestResetConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.5
//
EFI_STATUS
BBTestShutdownConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.6
//
EFI_STATUS
BBTestReceiveFilterConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.7
//
EFI_STATUS
BBTestStationAddressConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.8
//
EFI_STATUS
BBTestStatisticsConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.9
//
EFI_STATUS
BBTestMCastIPtoMACConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.10
//
EFI_STATUS
BBTestNVDataConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.11
//
EFI_STATUS
BBTestGetStatusConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.12
//
EFI_STATUS
BBTestTransmitConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// TDS 5.13
//
EFI_STATUS
BBTestReceiveConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );


//
// Prototypes of Internal Functions
//


//
// Entry GUIDs
//

#define SNP_START_FUNCTION_TEST_GUID \
  { 0xe6215c64, 0xfc31, 0x4a15, 0xaa, 0xa, 0xbd, 0x9e, 0xf2, 0x97, 0xb7, 0x6a }

#define SNP_STOP_FUNCTION_TEST_GUID \
  { 0x3649957d, 0x51da, 0x423d, 0xa8, 0xab, 0x3c, 0x81, 0x16, 0x1f, 0x11, 0x44 }

#define SNP_INITIALIZE_FUNCTION_TEST_GUID \
  { 0x87689738, 0x8dc6, 0x4e4b, 0xae, 0x76, 0x17, 0xcd, 0x96, 0x39, 0x9, 0x56 }

#define SNP_RESET_FUNCTION_TEST_GUID \
  { 0xba628a58, 0xe318, 0x4e4a, 0xba, 0xc2, 0xa7, 0x83, 0x3a, 0xdf, 0x42, 0xd9 }

#define SNP_SHUTDOWN_FUNCTION_TEST_GUID \
  { 0x6dd174af, 0x4706, 0x4de2, 0x81, 0x3c, 0x23, 0x9b, 0x25, 0xe, 0x72, 0x89 }

#define SNP_RECEIVEFILTERS_FUNCTION_TEST_GUID \
  { 0x7a1e587d, 0x53ac, 0x444e, 0xa5, 0xa1, 0x81, 0xcd, 0x3f, 0x4f, 0x4, 0xd4 }

#define SNP_STATIONADDRESS_FUNCTION_TEST_GUID \
  { 0x762fccb6, 0x5cdb, 0x4e23, 0x88, 0x90, 0x45, 0xcd, 0x23, 0x3c, 0xd1, 0x6f }

#define SNP_STATISTICS_FUNCTION_TEST_GUID \
  { 0x43e4bba9, 0x7570, 0x4adf, 0xbc, 0xe7, 0x1c, 0xde, 0x38, 0x55, 0xb6, 0x68 }

#define SNP_MCASTIPTOMAC_FUNCTION_TEST_GUID \
  { 0x3de1c94c, 0x4cbb, 0x47c4, 0x94, 0x7, 0xc, 0xf, 0x20, 0xfa, 0x66, 0xe7 }

#define SNP_NVDATA_FUNCTION_TEST_GUID \
  { 0x426be7f5, 0x3f47, 0x4919, 0x9f, 0x77, 0xc1, 0x42, 0xbb, 0xc, 0x9b, 0x27 }

#define SNP_GETSTATUS_FUNCTION_TEST_GUID \
  { 0x85270bdf, 0x5c38, 0x47fc, 0xa7, 0xdf, 0xf3, 0xc, 0x9a, 0x74, 0xa, 0xae }

#define SNP_TRANSMIT_FUNCTION_TEST_GUID \
  { 0xf3c3c828, 0x223f, 0x47c7, 0x99, 0x3f, 0xd5, 0xba, 0xad, 0xbf, 0xd0, 0xbe }

#define SNP_RECEIVE_FUNCTION_TEST_GUID \
  { 0xb0c12c51, 0xc966, 0x4b55, 0x8a, 0x81, 0x59, 0xdb, 0xc9, 0x7f, 0x28, 0x66 }

#define SNP_START_CONFORMANCE_TEST_GUID \
  { 0x352e6069, 0x8282, 0x49f8, 0xbc, 0xed, 0x85, 0xf, 0xf8, 0x7c, 0xa3, 0x1b }

#define SNP_STOP_CONFORMANCE_TEST_GUID \
  { 0x23cb3934, 0x7327, 0x45b1, 0xb6, 0xd9, 0x3b, 0x8f, 0x84, 0x61, 0x3c, 0x9e }

#define SNP_INITIALIZE_CONFORMANCE_TEST_GUID \
  { 0xbbdcee3c, 0x3e90, 0x4ae3, 0xab, 0xe5, 0x6c, 0xac, 0xec, 0x8b, 0x6b, 0x23 }

#define SNP_RESET_CONFORMANCE_TEST_GUID \
  { 0xd15dd19b, 0x9cbf, 0x40b8, 0x97, 0x76, 0xc1, 0x5e, 0xc5, 0x3f, 0x60, 0x89 }

#define SNP_SHUTDOWN_CONFORMANCE_TEST_GUID \
  { 0x54c6ee8b, 0x6b40, 0x4536, 0x8d, 0xab, 0x68, 0x2f, 0xe1, 0x3b, 0x25, 0xb4 }

#define SNP_RECEIVEFILTERS_CONFORMANCE_TEST_GUID \
  { 0x534a9e0f, 0xb31b, 0x40c5, 0xad, 0x60, 0xdf, 0x2, 0xc5, 0x53, 0x61, 0xd4 }

#define SNP_STATIONADDRESS_CONFORMANCE_TEST_GUID \
  { 0xec8714e0, 0x2ab9, 0x46e9, 0x8f, 0x35, 0x50, 0x83, 0x34, 0xb1, 0x18, 0xa7 }

#define SNP_STATISTICS_CONFORMANCE_TEST_GUID \
  { 0x122dd1f0, 0xaf8e, 0x4d3b, 0x88, 0x4b, 0x14, 0xc2, 0xff, 0x7a, 0xef, 0xa7 }

#define SNP_MCASTIPTOMAC_CONFORMANCE_TEST_GUID \
  { 0xd924b177, 0x7e44, 0x445d, 0x92, 0xaa, 0x5e, 0x15, 0xc0, 0xfd, 0x2e, 0x27 }

#define SNP_NVDATA_CONFORMANCE_TEST_GUID \
  { 0x5408eb9d, 0xb40a, 0x47dd, 0xbd, 0x4, 0xe9, 0xa8, 0xb9, 0x6, 0x66, 0x1f }

#define SNP_GETSTATUS_CONFORMANCE_TEST_GUID \
  { 0x574c0b74, 0xadc3, 0x46d9, 0xa8, 0xff, 0xc8, 0xa4, 0x4e, 0x66, 0x46, 0xfa }

#define SNP_TRANSMIT_CONFORMANCE_TEST_GUID \
  { 0x5ddf5c49, 0x7aca, 0x4724, 0x95, 0x53, 0x61, 0x4a, 0x53, 0xee, 0x99, 0x8a }

#define SNP_RECEIVE_CONFORMANCE_TEST_GUID \
  { 0xe648317f, 0x90aa, 0x4c8a, 0xb0, 0xa7, 0x88, 0x80, 0xf3, 0x22, 0xda, 0x54 }

#endif
