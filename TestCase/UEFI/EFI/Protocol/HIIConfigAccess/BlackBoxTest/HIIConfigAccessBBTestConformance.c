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
    HIIConfigAccessBBTestConformance.c

Abstract:
    for EFI Driver HII Configuration Access Protocol's conformance Test

--*/
#include "HIIConfigAccessBBTestMain.h"

EFI_BROWSER_ACTION  EFI_BROWSER_ACTION_UNSUPPORTED = 2;

EFI_STATUS
BBTestExtractConfigConformanceTestCheckpoint1 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess
  );

EFI_STATUS
BBTestExtractConfigConformanceTestCheckpoint2 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_HII_CONFIG_ROUTING_PROTOCOL       *HIIConfigRouting,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess
  );

EFI_STATUS
BBTestExtractConfigConformanceTestCheckpoint3 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_HII_CONFIG_ROUTING_PROTOCOL       *HIIConfigRouting,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess
  );

EFI_STATUS
BBTestExtractConfigConformanceTestCheckpoint4 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess
  );
  
EFI_STATUS
BBTestRouteConfigConformanceTestCheckpoint1 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess
  );
  
EFI_STATUS
BBTestRouteConfigConformanceTestCheckpoint2 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_HII_CONFIG_ROUTING_PROTOCOL       *HIIConfigRouting,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess
  );
  
EFI_STATUS
BBTestRouteConfigConformanceTestCheckpoint3 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess
  );
/*  
EFI_STATUS
BBTestCallBackConformanceTestCheckpoint1 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL         *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL             *HIIConfigAccess
  );

EFI_STATUS
BBTestCallBackConformanceTestCheckpoint2 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL         *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL             *HIIConfigAccess
  );
  
EFI_STATUS
BBTestCallBackConformanceTestCheckpoint3 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL         *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL             *HIIConfigAccess
  );

EFI_STATUS
BBTestCallBackConformanceTestCheckpoint4 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL         *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL             *HIIConfigAccess
  );
*/

EFI_STATUS
BBTestExtractConfigConformanceTest (
  IN EFI_BB_TEST_PROTOCOL    *This,
  IN VOID                    *ClientInterface,
  IN EFI_TEST_LEVEL          TestLevel,
  IN EFI_HANDLE              SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess;
  EFI_HII_CONFIG_ROUTING_PROTOCOL       *HIIConfigRouting;
  
  //
  // init
  //
  HIIConfigAccess = (EFI_HII_CONFIG_ACCESS_PROTOCOL*)ClientInterface;

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get the Config Routing Protocol Interface
  //
  Status = GetHIIConfigRoutingInterface( &HIIConfigRouting );
  if ( EFI_ERROR(Status) ) {
    return Status;
  }
  
  //
  //Call check points
  //
  BBTestExtractConfigConformanceTestCheckpoint1( StandardLib, HIIConfigAccess );
  BBTestExtractConfigConformanceTestCheckpoint2( StandardLib, HIIConfigRouting, HIIConfigAccess );
  BBTestExtractConfigConformanceTestCheckpoint3( StandardLib, HIIConfigRouting, HIIConfigAccess );
  BBTestExtractConfigConformanceTestCheckpoint4( StandardLib, HIIConfigAccess );
  
  return EFI_SUCCESS;
}

EFI_STATUS
BBTestRouteConfigConformanceTest (
  IN EFI_BB_TEST_PROTOCOL    *This,
  IN VOID                    *ClientInterface,
  IN EFI_TEST_LEVEL          TestLevel,
  IN EFI_HANDLE              SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess;
  EFI_HII_CONFIG_ROUTING_PROTOCOL       *HIIConfigRouting;
  
  //
  // init
  //
  HIIConfigAccess = (EFI_HII_CONFIG_ACCESS_PROTOCOL*)ClientInterface;

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get the Config Routing Protocol Interface
  //
  Status = GetHIIConfigRoutingInterface( &HIIConfigRouting );
  if ( EFI_ERROR(Status) ) {
    return Status;
  }
  
  //
  //Call check points
  //
  BBTestRouteConfigConformanceTestCheckpoint1( StandardLib, HIIConfigAccess );
  BBTestRouteConfigConformanceTestCheckpoint2( StandardLib, HIIConfigRouting, HIIConfigAccess );
  BBTestRouteConfigConformanceTestCheckpoint3( StandardLib, HIIConfigAccess );
  
  return EFI_SUCCESS;
}

/*
EFI_STATUS
BBTestCallBackConformanceTest (
  IN EFI_BB_TEST_PROTOCOL    *This,
  IN VOID                    *ClientInterface,
  IN EFI_TEST_LEVEL          TestLevel,
  IN EFI_HANDLE              SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess;
  
  //
  // init
  //
  HIIConfigAccess = (EFI_HII_CONFIG_ACCESS_PROTOCOL*)ClientInterface;

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   &StandardLib
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  
   //
  //Call check points
  //
  BBTestCallBackConformanceTestCheckpoint1( StandardLib, HIIConfigAccess );
  BBTestCallBackConformanceTestCheckpoint2( StandardLib, HIIConfigAccess );
  BBTestCallBackConformanceTestCheckpoint3( StandardLib, HIIConfigAccess );
  BBTestCallBackConformanceTestCheckpoint4( StandardLib, HIIConfigAccess );
  
  return EFI_SUCCESS;
}
*/

//
//Check Points
//

EFI_STATUS
BBTestExtractConfigConformanceTestCheckpoint1 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess
  )
{
  EFI_STATUS            Status;
  EFI_TEST_ASSERTION    AssertionType;
 
  EFI_STRING            Progress = NULL;
  EFI_STRING            Results = NULL;
  EFI_STRING            ResultsPtr = NULL;
  UINTN                 Len = 0;
  EFI_STRING            Pointer = NULL;
  UINT8                 IfMulti = 0;
  
  EFI_STRING            Request = NULL;
  //
  // Call ExtractConfig with Request been <MultiConfigRequest>
  //
  Status = HIIConfigAccess->ExtractConfig (
                              HIIConfigAccess,
                              NULL,
                              &Progress,
                              &Results
                              );

  if ( Status == EFI_SUCCESS ) {
    Len = StrLen(Results);
    //
    // Make sure the size of Request is enough to hold <MultiConfigRequest> 
    // if original Results is not Multi
    //
    Request = (EFI_STRING) AllocateZeroPool ( 2 * Len + 2 + 256);
    if (Request == NULL) {
  	  FreePool(Results);
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    return Status;
  }

  Status = MultiAltRespToMultiReq (Results, Request);
  ResultsPtr = Request;

  FreePool(Results);
  Results = NULL;

  if ( EfiStrStr(Request, L"GUID=") != NULL ) {
    Pointer = EfiStrStr(Request, L"GUID=");
    Pointer++;
    if ( EfiStrStr(Pointer, L"GUID=") != NULL )
	  IfMulti = 1;
  }
  
  if ( IfMulti == 0 ) {
    EfiStrCat( Request, L"&GUID=970eb94aa0d449f7b980bdaa47d42527&NAME=006a0069006e0039&PATH=000acf&grag&star");
  }

  Status = HIIConfigAccess->ExtractConfig (
                              HIIConfigAccess,
                              Request,
                              &Progress,
                              &Results
                              );
  
  if ( (EFI_INVALID_PARAMETER != Status) || (EfiStrnCmp (Progress, L"&GUID=", 6) != 0) ) {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gHIIConfigAccessBBTestConformanceAssertionGuid001,
                 L"HII_CONFIG_ACCESS_PROTOCOL.ExtractConfig - ExtractConfig() returns EFI_INVALID_PARAMETER with Request been <MultiConfigRequest>.",
                 L"%a:%d: Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
           );
  FreePool(Request);
  return EFI_SUCCESS;
}
 
EFI_STATUS
BBTestExtractConfigConformanceTestCheckpoint2 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_HII_CONFIG_ROUTING_PROTOCOL       *HIIConfigRouting,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess
  )
{
  EFI_STATUS            Status;
  EFI_TEST_ASSERTION    AssertionType;
  
  EFI_STRING            Request = NULL;
  EFI_STRING            Results = NULL;
  EFI_STRING            MultiConfigAltResp = NULL;
  EFI_STRING            DevicePath = NULL;
  
  //init the Request 
  Status = HIIConfigRouting->ExportConfig(
                               HIIConfigRouting,
                               &MultiConfigAltResp
                               );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = GetDevicePath ( HIIConfigAccess, &DevicePath );
  if (EFI_ERROR(Status)) {
  	gtBS->FreePool(MultiConfigAltResp);
    return Status;
  }

  Request = (EFI_STRING) AllocateZeroPool( 2 * StrLen(MultiConfigAltResp) + 2 );
  if (Request == NULL) {
    gtBS->FreePool(MultiConfigAltResp);
	gtBS->FreePool(DevicePath);
	return EFI_OUT_OF_RESOURCES;
  }
  
  Status = GetCorrespondingRequest (MultiConfigAltResp, DevicePath, Request);
  if (EFI_ERROR(Status)) {
  	gtBS->FreePool(MultiConfigAltResp);
	gtBS->FreePool(DevicePath);
	gtBS->FreePool(Request);
    return Status;
  }
  
  //
  // Call ExtractConfig with Progress been NULL
  //
  Status = HIIConfigAccess->ExtractConfig (
                              HIIConfigAccess,
                              Request,
                              NULL,
                              &Results
                              );
  
  if ( EFI_INVALID_PARAMETER != Status ) {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gHIIConfigAccessBBTestConformanceAssertionGuid002,
                 L"HII_CONFIG_ACCESS_PROTOCOL.ExtractConfig - ExtractConfig() returns EFI_INVALID_PARAMETER with Progress been NULL.",
                 L"%a:%d: Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
           );

  gtBS->FreePool(MultiConfigAltResp);
  gtBS->FreePool(DevicePath);
  gtBS->FreePool(Request);

  return EFI_SUCCESS;
}
 
EFI_STATUS
BBTestExtractConfigConformanceTestCheckpoint3 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_HII_CONFIG_ROUTING_PROTOCOL       *HIIConfigRouting,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess
  )
{
  EFI_STATUS            Status;
  EFI_TEST_ASSERTION    AssertionType;
  
  EFI_STRING            Request = NULL;
  EFI_STRING            Progress = NULL;
  EFI_STRING            MultiConfigAltResp = NULL;
  EFI_STRING            DevicePath = NULL;
  
  //init the Request 
  Status = HIIConfigRouting->ExportConfig(
                               HIIConfigRouting,
                               &MultiConfigAltResp
                               );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = GetDevicePath ( HIIConfigAccess, &DevicePath );
  if (EFI_ERROR(Status)) {
  	gtBS->FreePool(MultiConfigAltResp);
    return Status;
  }

  Request = (EFI_STRING) AllocateZeroPool( 2 * StrLen(MultiConfigAltResp) + 2 );
  if (Request == NULL) {
    gtBS->FreePool(MultiConfigAltResp);
	gtBS->FreePool(DevicePath);
	return EFI_OUT_OF_RESOURCES;
  }
  
  Status = GetCorrespondingRequest (MultiConfigAltResp, DevicePath, Request);
  if (EFI_ERROR(Status)) {
  	gtBS->FreePool(MultiConfigAltResp);
	gtBS->FreePool(DevicePath);
	gtBS->FreePool(Request);
    return Status;
  }
  //
  // Call ExtractConfig with Results been NULL
  //
  Status = HIIConfigAccess->ExtractConfig (
                              HIIConfigAccess,
                              Request,
                              &Progress,
                              NULL
                              );
  
  if ( EFI_INVALID_PARAMETER != Status ) {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gHIIConfigAccessBBTestConformanceAssertionGuid003,
                 L"HII_CONFIG_ACCESS_PROTOCOL.ExtractConfig - ExtractConfig() returns EFI_INVALID_PARAMETER with Results been NULL.",
                 L"%a:%d: Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  gtBS->FreePool(MultiConfigAltResp);
  gtBS->FreePool(DevicePath);
  gtBS->FreePool(Request);

  return EFI_SUCCESS;
}

 
EFI_STATUS
BBTestExtractConfigConformanceTestCheckpoint4 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL         *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL             *HIIConfigAccess
  )
{
  EFI_STATUS            Status;
  EFI_TEST_ASSERTION    AssertionType;
  
  EFI_STRING            Request;
  EFI_STRING            Progress = NULL;
  EFI_STRING            Results = NULL;
  
  //init the Request
  Request = L"GUID=71e430fb2e8849779d7a33ef1139e7ed&NAME=006a0069006e0039&PATH=000acf&Jack&Rons&Nash&Mary";  
  
  //
  // Call ExtractConfig with unknown name 
  //
  Status = HIIConfigAccess->ExtractConfig (
                              HIIConfigAccess,
                              Request,
                              &Progress,
                              &Results
                              );
  
  if ( ((EFI_INVALID_PARAMETER == Status) && (EfiStrnCmp (Progress, L"&Jack&Rons&Nash&Mary", 20) == 0)) ||
    ((EFI_NOT_FOUND == Status) && (Progress != NULL)) ) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
           StandardLib,
           AssertionType,
           gHIIConfigAccessBBTestConformanceAssertionGuid004,
           L"HII_CONFIG_ACCESS_PROTOCOL.ExtractConfig - ExtractConfig() returns EFI_INVALID_PARAMETER/EFI_NOT_FOUND with invalid input.",
           L"%a:%d: Status - %r",
           __FILE__,
           (UINTN)__LINE__,
           Status
           );
 return EFI_SUCCESS;
 }

 
EFI_STATUS
BBTestRouteConfigConformanceTestCheckpoint1 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL         *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL             *HIIConfigAccess
  )
{
  EFI_STATUS                                  Status;
  EFI_TEST_ASSERTION                          AssertionType;
  
  EFI_STRING                                  Progress = NULL;

  
  //
  // Call RouteConfig with Configuration been NULL
  //
  Status = HIIConfigAccess->RouteConfig (
                  HIIConfigAccess,
                  NULL,
                  &Progress
                  );
  
  if ( EFI_INVALID_PARAMETER != Status ) {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  }
  StandardLib->RecordAssertion (
           StandardLib,
           AssertionType,
           gHIIConfigAccessBBTestConformanceAssertionGuid005,
           L"HII_CONFIG_ACCESS_PROTOCOL.RouteConfig - RouteConfig() returns EFI_INVALID_PARAMETER with Configuration been NULL.",
           L"%a:%d: Status - %r",
           __FILE__,
           (UINTN)__LINE__,
           Status
           );
 return EFI_SUCCESS;
 }
 
EFI_STATUS
BBTestRouteConfigConformanceTestCheckpoint2 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib,
  IN EFI_HII_CONFIG_ROUTING_PROTOCOL       *HIIConfigRouting,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL        *HIIConfigAccess
  )
{
  EFI_STATUS            Status;
  EFI_TEST_ASSERTION    AssertionType;
  
  EFI_STRING            Config = NULL;
  EFI_STRING            MultiConfigAltResp = NULL;
  EFI_STRING            DevicePath = NULL;
  
  //init the Request 
  Status = HIIConfigRouting->ExportConfig(
                               HIIConfigRouting,
                               &MultiConfigAltResp
                               );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = GetDevicePath ( HIIConfigAccess, &DevicePath );
  if (EFI_ERROR(Status)) {
  	gtBS->FreePool(MultiConfigAltResp);
    return Status;
  }

  Config = (EFI_STRING) AllocateZeroPool( 2 * StrLen(MultiConfigAltResp) + 2 );
  if (Config == NULL) {
    gtBS->FreePool(MultiConfigAltResp);
	gtBS->FreePool(DevicePath);
	return EFI_OUT_OF_RESOURCES;
  }
  
  Status = GetCorrespondingResp (MultiConfigAltResp, DevicePath, Config);
  if (EFI_ERROR(Status)) {
  	gtBS->FreePool(MultiConfigAltResp);
	gtBS->FreePool(DevicePath);
	gtBS->FreePool(Config);
    return Status;
  }
  
  //
  // Call RouteConfig with Progress been NULL
  //
  Status = HIIConfigAccess->RouteConfig (
                              HIIConfigAccess,
                              Config,
                              NULL
                              );
  
  if ( EFI_INVALID_PARAMETER != Status ) {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gHIIConfigAccessBBTestConformanceAssertionGuid006,
                 L"HII_CONFIG_ACCESS_PROTOCOL.RouteConfig - RouteConfig() returns EFI_INVALID_PARAMETER with Progress been NULL.",
                 L"%a:%d: Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  gtBS->FreePool(MultiConfigAltResp);
  gtBS->FreePool(DevicePath);
  gtBS->FreePool(Config);

  return EFI_SUCCESS;
}
 
EFI_STATUS
BBTestRouteConfigConformanceTestCheckpoint3 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL         *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL             *HIIConfigAccess
  )
{
  EFI_STATUS                                  Status;
  EFI_TEST_ASSERTION                          AssertionType;
  
  EFI_STRING                                  Configuration;
  EFI_STRING                                  Progress = NULL;
  
  //init the Configuration
  Configuration=L"GUID=71e430fb2e8849779d7a33ef1139e7ed&NAME=006a0069006e0039&PATH=0acf&OFFSET=2&WIDTH=3VALUE=45AF";
  
  //
  // Call RouteConfig with no found target for the routing data
  //
  Status = HIIConfigAccess->RouteConfig (
                              HIIConfigAccess,
                              Configuration,
                              &Progress
                              );
  
  if ( EFI_NOT_FOUND != Status ) {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  }
  StandardLib->RecordAssertion (
           StandardLib,
           AssertionType,
           gHIIConfigAccessBBTestConformanceAssertionGuid007,
           L"HII_CONFIG_ACCESS_PROTOCOL.RouteConfig - RouteConfig() returns EFI_NOT_FOUND with no found target for the routing data.",
           L"%a:%d: Status - %r",
           __FILE__,
           (UINTN)__LINE__,
           Status
           );

  return EFI_SUCCESS;
}

/*
EFI_STATUS
BBTestCallBackConformanceTestCheckpoint1 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL         *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL             *HIIConfigAccess
  )
{
  EFI_STATUS                                     Status;
  EFI_TEST_ASSERTION                             AssertionType;
  
  EFI_QUESTION_ID                                QuestionId;
  UINT8                                          Type;
  EFI_IFR_TYPE_VALUE                             Value;
  EFI_BROWSER_ACTION_REQUEST                     ActionRequest;
  
  //init the parameter
  QuestionId=0x1234;
  Type=EFI_IFR_TYPE_NUM_SIZE_8;

 
  //
  // Call CallBack with the Action been NULL
  //
  Status = HIIConfigAccess->CallBack (
                  HIIConfigAccess,
                  NULL,
                  QuestionId,
				  Type,
				  &Value,
				  &ActionRequest
                  );
  
  if ( EFI_INVALID_PARAMETER != Status ) {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_PASSED;

	//check the output ActionRequest
  if ((EFI_BROWSER_ACTION_REQUEST_NONE || EFI_BROWSER_ACTION_REQUEST_RESET || EFI_BROWSER_ACTION_REQUEST_SUBMIT || EFI_BROWSER_ACTION_REQUEST_EXIT) != ActionRequest)
	AssertionType = EFI_TEST_ASSERTION_FAILED;
	
  }
  StandardLib->RecordAssertion (
           StandardLib,
           AssertionType,
           gHIIConfigAccessBBTestConformanceAssertionGuid009,
           L"HII_CONFIG_ACCESS_PROTOCOL.CallBack - CallBack() returns EFI_INVALID_PARAMETER with Action been NULL .",
           L"%a:%d: Status - %r",
           __FILE__,
           (UINTN)__LINE__,
           Status
           );
 return EFI_SUCCESS;
  
}

EFI_STATUS
BBTestCallBackConformanceTestCheckpoint2 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL         *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL             *HIIConfigAccess
  )
{
  EFI_STATUS                                     Status;
  EFI_TEST_ASSERTION                             AssertionType;
  
  EFI_BROWSER_ACTION                             *Action;
  EFI_QUESTION_ID                                QuestionId;
  UINT8                                          Type;
  EFI_IFR_TYPE_VALUE                             Value;

  
  //init the parameter
  Action=EFI_BROWSER_ACTION_REQUEST_CHANGING;
  QuestionId=0x1234;
  Type=EFI_IFR_TYPE_NUM_SIZE_8;

 
  //
  // Call CallBack with the ActionRequest been NULL
  //
  Status = HIIConfigAccess->CallBack (
                  HIIConfigAccess,
                  Action,
                  QuestionId,
				  Type,
				  &Value,
				  NULL
                  );
  
  if ( EFI_INVALID_PARAMETER != Status ) {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
	
  }
  StandardLib->RecordAssertion (
           StandardLib,
           AssertionType,
           gHIIConfigAccessBBTestConformanceAssertionGuid0010,
           L"HII_CONFIG_ACCESS_PROTOCOL.CallBack - CallBack() returns EFI_INVALID_PARAMETER with QuestionId been NULL.",
           L"%a:%d: Status - %r",
           __FILE__,
           (UINTN)__LINE__,
           Status
           );
 return EFI_SUCCESS;
  
}

EFI_STATUS
BBTestCallBackConformanceTestCheckpoint3(
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL         *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL             *HIIConfigAccess
  )
{
  EFI_STATUS                                     Status;
  EFI_TEST_ASSERTION                             AssertionType;
  
  EFI_BROWSER_ACTION                             *Action;
  EFI_QUESTION_ID                                QuestionId;
  UINT8                                          Type;
  EFI_BROWSER_ACTION_REQUEST                     ActionRequest;
  
  //init the parameter
  Action = EFI_BROWSER_ACTION_REQUEST_CHANGING;
  QuestionId = 0x1234;
  Type = EFI_IFR_TYPE_NUM_SIZE_8;

 
  //
  // Call CallBack with the Value been NULL
  //
  Status = HIIConfigAccess->CallBack (
                  HIIConfigAccess,
                  Action,
                  QuestionId,
				  Type,
				  NULL,
				  &ActionRequest
                  );
  
  if ( EFI_INVALID_PARAMETER != Status ) {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
	
	//check the output ActionRequest
  if ((EFI_BROWSER_ACTION_REQUEST_NONE || EFI_BROWSER_ACTION_REQUEST_RESET || EFI_BROWSER_ACTION_REQUEST_SUBMIT || EFI_BROWSER_ACTION_REQUEST_EXIT) != ActionRequest)
	AssertionType = EFI_TEST_ASSERTION_FAILED;
	
  }
  StandardLib->RecordAssertion (
           StandardLib,
           AssertionType,
           gHIIConfigAccessBBTestConformanceAssertionGuid0011,
           L"HII_CONFIG_ACCESS_PROTOCOL.CallBack - CallBack() returns EFI_INVALID_PARAMETER with Value been NULL.",
           L"%a:%d: Status - %r",
           __FILE__,
           (UINTN)__LINE__,
           Status
           );
 return EFI_SUCCESS;
  
}

EFI_STATUS
BBTestCallBackConformanceTestCheckpoint4 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL         *StandardLib,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL             *HIIConfigAccess
  )
{
  EFI_STATUS                                     Status;
  EFI_TEST_ASSERTION                             AssertionType;
  
  EFI_BROWSER_ACTION                             *Action;
  EFI_QUESTION_ID                                QuestionId;
  UINT8                                          Type;
  EFI_IFR_TYPE_VALUE                             Value;
  EFI_BROWSER_ACTION_REQUEST                     ActionRequest;
  
  //init the parameter
  Action=&EFI_BROWSER_ACTION_UNSUPPORTED;
  QuestionId=0x1234;
  Type=EFI_IFR_TYPE_NUM_SIZE_8;

  
  //
  // Call CallBack with  the specified action not supported by the callback
  //
  Status = HIIConfigAccess->CallBack (
                  HIIConfigAccess,
                  Action,
                  QuestionId,
				  Type,
				  &Value,
				  &ActionRequest
                  );
  
  if ( EFI_UNSUPPORTED != Status ) {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
	
	//check the output ActionRequest
  if ((EFI_BROWSER_ACTION_REQUEST_NONE || EFI_BROWSER_ACTION_REQUEST_RESET || EFI_BROWSER_ACTION_REQUEST_SUBMIT || EFI_BROWSER_ACTION_REQUEST_EXIT) != ActionRequest)
	AssertionType = EFI_TEST_ASSERTION_FAILED;
	
  }
  StandardLib->RecordAssertion (
           StandardLib,
           AssertionType,
           gHIIConfigAccessBBTestConformanceAssertionGuid0012,
           L"HII_CONFIG_ACCESS_PROTOCOL.CallBack - CallBack() returns EFI_ UNSUPPORTED with  the specified action not supported by the callback .",
           L"%a:%d: Status - %r",
           __FILE__,
           (UINTN)__LINE__,
           Status
           );
 return EFI_SUCCESS;
  
}
*/
