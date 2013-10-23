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
                                                                
  Copyright 2006, 2007, 2008, 2009, 2010, 2011 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:

  PxeBaseCodeBBTestConformance.c

Abstract:

  Conformance Test Cases of SimpleNetwork Protocol

--*/

#include "PxeBaseCodeBBTestMain.h"
#include "PxeBaseCodeBBTestSupport.h"

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.Start() Conformance Test.
 *  It is the new case to add IPv6 into the test scope. The original test case is switched off.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
EFI_STATUS
BBTestNewStartConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;

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
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  //
  // Disable EFI_PXE_BASE_CODE_PROTOCOL Protocol interface
  //
  if (BcInterface->Mode->Started) {
    Status = BcInterface->Stop (BcInterface);
    if (EFI_ERROR (Status)) {
      StandardLib->RecordAssertion (
                     StandardLib,
                     EFI_TEST_ASSERTION_FAILED,
                     gTestGenericFailureGuid,
                     L"EFI_PXE_BASE_CODE_PROTOCOL.Stop - Can't change the PXE state to disabled",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
      return Status;
    }
  }

  //
  // Enable EFI_PXE_BASE_CODE_PROTOCOL Protocol interface in IPv4
  //
  Status = BcInterface->Start (BcInterface, FALSE);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Start - Can't change the PXE state to enabled",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  //
  // Start() while EFI_PXE_BASE_CODE_PROTOCOL Protocol already started
  //
  Status = BcInterface->Start (BcInterface, FALSE);
  if (Status == EFI_ALREADY_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else if ((Status == EFI_DEVICE_ERROR) || (Status == EFI_OUT_OF_RESOURCES)) {
    AssertionType = EFI_TEST_ASSERTION_WARNING;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestNewConformanceAssertionGuid001,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Start - PXE already started in IPv4",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // Disable EFI_PXE_BASE_CODE_PROTOCOL Protocol interface
  //
  Status = BcInterface->Stop (BcInterface);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Stop - Can't change the PXE state to disabled",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  if (BcInterface->Mode->Ipv6Supported && BcInterface->Mode->Ipv6Available) {
    //
    // Enable EFI_PXE_BASE_CODE_PROTOCOL Protocol interface in IPv6
    //
    Status = BcInterface->Start (BcInterface, TRUE);
    if (EFI_ERROR (Status)) {
      StandardLib->RecordAssertion (
                     StandardLib,
                     EFI_TEST_ASSERTION_FAILED,
                     gTestGenericFailureGuid,
                     L"EFI_PXE_BASE_CODE_PROTOCOL.Start - Can't change the PXE state to enabled",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );      
      return Status;
    }

    //
    // Start while EFI_PXE_BASE_CODE_PROTOCOL Protocol already started
    //
    Status = BcInterface->Start (BcInterface, TRUE);
    if (Status == EFI_ALREADY_STARTED) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else if ((Status == EFI_DEVICE_ERROR) || (Status == EFI_OUT_OF_RESOURCES)) {
      AssertionType = EFI_TEST_ASSERTION_WARNING;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                             StandardLib,
                             AssertionType,
                             gPxeBaseCodeBBTestNewConformanceAssertionGuid002,
                             L"EFI_PXE_BASE_CODE_PROTOCOL.Start - PXE already started in IPv6",
                             L"%a:%d:Status - %r",
                             __FILE__,
                             (UINTN)__LINE__,
                            Status
                            );
  } else {
    //
    // Start while EFI_PXE_BASE_CODE_PROTOCOL Protocol is not supported
    //
    Status = BcInterface->Start (BcInterface, TRUE);
    if (Status == EFI_UNSUPPORTED) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                             StandardLib,
                             AssertionType,
                             gPxeBaseCodeBBTestNewConformanceAssertionGuid003,
                             L"EFI_PXE_BASE_CODE_PROTOCOL.Start - IPV6 not supported",
                             L"%a:%d:Status - %r",
                             __FILE__,
                             (UINTN)__LINE__,
                             Status
                             );	
  }

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.Stop() Conformance Test.
 *  It is the new case to add IPv6 into the test scope. The original test case is switched off.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.2
//
EFI_STATUS
BBTestNewStopConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;

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
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  //
  // Disable EFI_PXE_BASE_CODE_PROTOCOL Protocol interface
  //
  if (BcInterface->Mode->Started) {
    Status = BcInterface->Stop (BcInterface);
    if (EFI_ERROR (Status)) {
      StandardLib->RecordAssertion (
                     StandardLib,
                     EFI_TEST_ASSERTION_FAILED,
                     gTestGenericFailureGuid,
                     L"EFI_PXE_BASE_CODE_PROTOCOL.Stop - Can't change the PXE state to disabled",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
      return Status;
    }
  }

  if (BcInterface->Mode->Ipv6Supported && BcInterface->Mode->Ipv6Available) {
    //
    // Enable EFI_PXE_BASE_CODE_PROTOCOL Protocol interface in Ipv6
    //
		Status = BcInterface->Start (BcInterface, TRUE);
    if (EFI_ERROR (Status)) {
      StandardLib->RecordAssertion (
                     StandardLib,
                     EFI_TEST_ASSERTION_FAILED,
                     gTestGenericFailureGuid,
                     L"EFI_PXE_BASE_CODE_PROTOCOL.Start - Can't change the PXE state to enabled",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
    }

    //
    // Disable EFI_PXE_BASE_CODE_PROTOCOL Protocol interface
    //
    Status = BcInterface->Stop (BcInterface);
    if (EFI_ERROR (Status)) {
      StandardLib->RecordAssertion (
                     StandardLib,
                     EFI_TEST_ASSERTION_FAILED,
                     gTestGenericFailureGuid,
                     L"EFI_PXE_BASE_CODE_PROTOCOL.Stop - Can't change the PXE state to disabled",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
    }

    //
    // Call Stop() while EFI_PXE_BASE_CODE_PROTOCOL already disabled
    //
    Status = BcInterface->Stop (BcInterface);
    if (Status == EFI_NOT_STARTED) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else if (Status == EFI_DEVICE_ERROR) {
      AssertionType = EFI_TEST_ASSERTION_WARNING;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gPxeBaseCodeBBTestNewConformanceAssertionGuid004,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Stop - PXE not started in IPv6",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  } 

  //
  // Enable EFI_PXE_BASE_CODE_PROTOCOL Protocol interface in Ipv4
  //
  Status = BcInterface->Start (BcInterface, FALSE);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Start - Can't change the PXE state to enabled",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  //
  // Disable EFI_PXE_BASE_CODE_PROTOCOL Protocol interface
  //
  Status = BcInterface->Stop (BcInterface);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Stop - Can't change the PXE state to disabled",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  //
  // Call Stop() while EFI_PXE_BASE_CODE_PROTOCOL already disabled
  //
  Status = BcInterface->Stop (BcInterface);
  if (Status == EFI_NOT_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else if (Status == EFI_DEVICE_ERROR) {
    AssertionType = EFI_TEST_ASSERTION_WARNING;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestNewConformanceAssertionGuid005,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Stop - PXE not started in IPv4",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );


  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.Start() Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.1
//
EFI_STATUS
BBTestStartConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;

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
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  //
  // Change EFI_PXE_BASE_CODE_PROTOCOL Protocol state to enabled
  //
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, TRUE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Start()  while EFI_PXE_BASE_CODE_PROTOCOL Protocol already started
  //
  Status = BcInterface->Start (BcInterface, FALSE);
  if (Status == EFI_ALREADY_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid001,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Start - PXE already started",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

   //
  // Change EFI_PXE_BASE_CODE_PROTOCOL Protocol to disabled
  //
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Start EFI_PXE_BASE_CODE_PROTOCOL using IPV6 while IPV6 not supported
  //
  BcInterface->Mode->Ipv6Supported = FALSE;

  Status = BcInterface->Start (BcInterface, TRUE);
  if (Status == EFI_UNSUPPORTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid002,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Start - IPV6 not supported",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.Stop() Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.2
//
EFI_STATUS
BBTestStopConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;

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
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  //
  // Make sure the EFI_PXE_BASE_CODE_PROTOCOL Protocol is disabled
  //
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Stop - Can change the PXE state to disabled",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return EFI_UNSUPPORTED;
  }

  //
  // Call Stop() while EFI_PXE_BASE_CODE_PROTOCOL already disabled
  //
  Status = BcInterface->Stop (BcInterface);
  if (Status == EFI_NOT_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid003,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Stop - PXE not started",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.Dhcp() Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.3
//
EFI_STATUS
BBTestDhcpConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;

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
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  //
  // Change EFI_PXE_BASE_CODE_PROTOCOL Protocol to disabled
  //
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // 5.3.2.1 Call Dhcp() while EFI_PXE_BASE_CODE_PROTOCOL already disabled
  //
  Status = BcInterface->Dhcp (BcInterface, FALSE);
  if (Status == EFI_NOT_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid004,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Dhcp - PXE not started",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // Change EFI_PXE_BASE_CODE_PROTOCOL Protocol to enabled
  //
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, TRUE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Wait for user setup test environment
  //
  Print (L"\r\nPlease Shutdown DHCP Server\r\n");
  WaitForAnyInput ();

  //
  // 5.3.2.2 Call Dhcp() while DHCP Server shutdown
  //
  Status = BcInterface->Dhcp (BcInterface, FALSE);
  if (Status == EFI_TIMEOUT) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid005,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Dhcp -  DHCP server shutdown",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // Wait for close PXE option
  //
  Print (L"\r\nClose PXE Option of DHCP Server\r\n");
  WaitForAnyInput ();

  //
  // 5.3.2.3 Call Dhcp() while DHCP Server only assign IP address
  //
  Status = BcInterface->Dhcp (BcInterface, FALSE);
  if (Status == EFI_NO_RESPONSE) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid006,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Dhcp - Only Traditional DHCP Server",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // 4.3.2.4  Call Back Function abort while Dhcp()
  //

  //
  // Hook the Callback function
  //
  Status = HookReturnAbortCallBack (EFI_PXE_BASE_CODE_FUNCTION_DHCP, BcInterface);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Dhcp - HookReturnAbortCallBack",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  //
  // Wait for setup Dhcp Server
  //
  Print (L"\r\nSetup Dhcp Server\r\n");
  WaitForAnyInput ();

  Status = BcInterface->Dhcp (BcInterface, FALSE);
  if ((Status == EFI_ABORTED) && gCallBackInvoked) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid007,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Dhcp - Callback abort",
                 L"%a:%d:Status - %r, CallbackInvoked - %d",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 gCallBackInvoked
                 );

  //
  // Unhook the Callback function
  //
  UnHookReturnAbortCallBack (BcInterface);

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.Discover() Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.4
//
EFI_STATUS
BBTestDiscoverConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;

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
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  //
  // Change EFI_PXE_BASE_CODE_PROTOCOL Protocol to disabled
  //
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // 5.4.2.1 Call Discover() while EFI_PXE_BASE_CODE_PROTOCOL is disabled
  //
  Status = BcInterface->Discover (
             BcInterface,
             EFI_PXE_BASE_CODE_BOOT_TYPE_BOOTSTRAP,
             EFI_PXE_BASE_CODE_BOOT_LAYER_INITIAL,
             FALSE,
             NULL
             );
  if (Status == EFI_NOT_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid008,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Discover - PXE Disabled",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );


  //
  // Change EFI_PXE_BASE_CODE_PROTOCOL Protocol to enabled
  //
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, TRUE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get IP
  //
  Print (L"\r\nPlease Setup DHCP Server\r\n");
  WaitForAnyInput ();
  Status = BcInterface->Dhcp (BcInterface, FALSE);
  if (EFI_ERROR(Status))
  {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Discover - Can't DHCP",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  //
  // Wait for user setup test environment
  //
  Print (L"\r\nPlease Shutdown Boot Server\r\n");
  WaitForAnyInput ();

  //
  // 5.4.2.2 Call Discover() while Boot Server shutdown
  //
  Status = BcInterface->Discover (
                          BcInterface,
                          EFI_PXE_BASE_CODE_BOOT_TYPE_BOOTSTRAP,
                          EFI_PXE_BASE_CODE_BOOT_LAYER_INITIAL,
                          FALSE,
                          NULL
                          );
  if (Status == EFI_TIMEOUT) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid009,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Discover -  Boot server shutdown",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );


  //
  // 4.4.2.3 Call Back Function abort while Discover()
  //

  Status = HookReturnAbortCallBack (EFI_PXE_BASE_CODE_FUNCTION_DISCOVER, BcInterface);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Discover - HookReturnAbortCallBack",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  //
  // Wait for setup Boot Server
  //
  Print (L"\r\nSetup BOOT Server environment\r\n");
  WaitForAnyInput ();

  Status = BcInterface->Discover (
                          BcInterface,
                          EFI_PXE_BASE_CODE_BOOT_TYPE_BOOTSTRAP,
                          EFI_PXE_BASE_CODE_BOOT_LAYER_INITIAL,
                          FALSE,
                          NULL
                          );
  if ((Status == EFI_ABORTED) && gCallBackInvoked) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid010,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Discover - Callback abort",
                 L"%a:%d:Status - %r, CallbackInvoked - %d",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 gCallBackInvoked
                 );

  //
  // Unhook the Callback function
  //
  UnHookReturnAbortCallBack (BcInterface);

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.Mtftp() Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.5
//
EFI_STATUS
BBTestMtftpConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib;
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL    *ProfileLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;
  UINTN                                 BlockSize=512;
  EFI_IP_ADDRESS                        ServerIp;
  CHAR8                                 *FileName;
  EFI_INI_FILE_HANDLE                   FileHandle;
  UINT32                                Order = 0;
  UINT64                                Size;
  VOID                                  *Buffer;
  //
  // Get the Standard and Profile Library Interface
  //
  Status = GetTestSupportLibrary (
             SupportHandle,
             &StandardLib,
             &ProfileLib,
             NULL
             );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  //
  // Fill Parameters (ServerIp, FileName, BufferPtr)
  // Get parameter from ini file.
  //
  Status = OpenTestIniFile (ProfileLib, &FileHandle);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - OpenTestIniFile",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  Status = GetIpAddrFromFile (FileHandle, L"MTFTP_CONF", L"TFTPServer", Order, &ServerIp);
  if (EFI_ERROR(Status))
  {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - GetIpAddrFromFile",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    CloseTestIniFile (ProfileLib, FileHandle);
    return Status;
  }
  Status = GetAsciiStringFromFile (FileHandle, L"MTFTP_CONF", L"FileName", Order, &FileName);
  if (EFI_ERROR(Status))
  {

    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - GetAsciiStringFromFile",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    CloseTestIniFile (ProfileLib, FileHandle);
    return Status;
  }

  CloseTestIniFile (ProfileLib, FileHandle);

  //
  // 5.4.2.1 Call Mtftp() while EFI_PXE_BASE_CODE_PROTOCOL is disabled
  //

  //
  // Change EFI_PXE_BASE_CODE_PROTOCOL Protocol to disabled
  //
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - Disable PXE",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  Status = BcInterface->Mtftp (
                          BcInterface,
                          EFI_PXE_BASE_CODE_TFTP_GET_FILE_SIZE,
                          NULL,
                          FALSE,
                          &Size,
                          &BlockSize,
                          &ServerIp,
                          FileName,
                          NULL,
                          FALSE
                          );
  if (Status == EFI_NOT_STARTED){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid011,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - while PXE Disabled",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  // Enable EFI_PXE_BASE_CODE_PROTOCOL protocol if needed
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, TRUE);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - Enable PXE",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  //
  // Get IP
  //
  Print (L"\r\nPlease Setup DHCP Server\r\n");
  WaitForAnyInput ();
  Status = BcInterface->Dhcp (BcInterface, FALSE);
  if (EFI_ERROR(Status))
  {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - Can't DHCP",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  //
  // 4.5.2.4  Call Mtftp() while M/TFTP Service stopped
  //

  //
  // Stop TFTP Server
  //
  Print (L"\r\nStop TFTP Server\r\n");
  WaitForAnyInput ();

  Status = BcInterface->Mtftp (
                          BcInterface,
                          EFI_PXE_BASE_CODE_TFTP_GET_FILE_SIZE,
                          NULL,
                          FALSE,
                          &Size,
                          &BlockSize,
                          &ServerIp,
                          FileName,
                          NULL,
                          FALSE
                          );
  if (Status == EFI_ICMP_ERROR){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid012,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - while M/TFTP Stopped - check Return Code",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  Print (L"\r\nDoes ICMP Destination Unreachable Packet Captured by Sniffer?\r\n");
  if (TRUE == WaitForUserCheck ()){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid013,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - while M/TFTP Stopped - check Packet",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // 4.5.2.5  Call Mtftp() while M/TFTP Server shutdown
  //
  //
  // Shutdown TFTP Server
  //
  Print (L"\r\nShutdown M/TFTP Server\r\n");
  WaitForAnyInput ();

  Status = BcInterface->Mtftp (
                          BcInterface,
                          EFI_PXE_BASE_CODE_TFTP_GET_FILE_SIZE,
                          NULL,
                          FALSE,
                          &Size,
                          &BlockSize,
                          &ServerIp,
                          FileName,
                          NULL,
                          FALSE
                          );
  if (Status == EFI_TIMEOUT){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid014,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - while M/TFTP Stopped - check Return Code",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // 4.5.2.6  Call Mtftp() while Request File not exist
  //
  //
  // Delete requested file
  //
  Print (L"\r\nDelete Requested File from M/TFTP Server\r\n");
  WaitForAnyInput ();

  Status = BcInterface->Mtftp (
                          BcInterface,
                          EFI_PXE_BASE_CODE_TFTP_GET_FILE_SIZE,
                          NULL,
                          FALSE,
                          &Size,
                          &BlockSize,
                          &ServerIp,
                          FileName,
                          NULL,
                          FALSE
                          );
  if (Status == EFI_TFTP_ERROR){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid015,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - No Requested File - check Return Code",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // 4.5.2.7  Call Mtftp() while Request Directory not exist
  //

  // Get the directory name
  Status = GetAsciiStringFromFile (FileHandle, L"MTFTP_CONF", L"Directory_Name", 0, &FileName);
  if (EFI_ERROR(Status))
  {

    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - GetAsciiStringFromFile",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    CloseTestIniFile (ProfileLib, FileHandle);
    return Status;
  }

  Status = BcInterface->Mtftp (
                          BcInterface,
                          EFI_PXE_BASE_CODE_TFTP_READ_DIRECTORY,
                          NULL,
                          FALSE,
                          &Size,
                          &BlockSize,
                          &ServerIp,
                          FileName,
                          NULL,
                          FALSE
                          );
  if (Status == EFI_TFTP_ERROR){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid016,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - No Requested Directory - check Return Code",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // 4.5.2.8  Call Mtftp() while Tring to write read-only file
  //
  //
  // Change File to READ-ONLY
  //
  Print (L"\r\nChange the requested file to READ-ONLY\r\n");
  WaitForAnyInput ();

  Status = gtBS->AllocatePool (EfiBootServicesData, (UINT32)Size, &Buffer);
  if (EFI_ERROR(Status))
  {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - Allocate Buffer",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    CloseTestIniFile (ProfileLib, FileHandle);
    return Status;
  }
  Status = BcInterface->Mtftp (
                          BcInterface,
                          EFI_PXE_BASE_CODE_TFTP_WRITE_FILE,
                          Buffer,
                          FALSE,
                          &Size,
                          &BlockSize,
                          &ServerIp,
                          FileName,
                          NULL,
                          FALSE
                          );
  if (Status == EFI_TFTP_ERROR){
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid017,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - No Requested Directory - check Return Code",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  gtBS->FreePool (Buffer);

  //
  // 4.5.2.9 Call Back Function abort while MTFTP()
  //

  //
  // Hook the Callback function
  //
  Status = HookReturnAbortCallBack (EFI_PXE_BASE_CODE_FUNCTION_MTFTP, BcInterface);
  if (EFI_ERROR(Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_WARNING,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - HookReturnAbortCallBack",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  Status = BcInterface->Mtftp (
                          BcInterface,
                          EFI_PXE_BASE_CODE_TFTP_GET_FILE_SIZE,
                          NULL,
                          FALSE,
                          &Size,
                          &BlockSize,
                          &ServerIp,
                          FileName,
                          NULL,
                          FALSE
                          );
  if ((Status == EFI_ABORTED) && gCallBackInvoked) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid018,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Mtftp - Callback abort",
                 L"%a:%d:Status - %r, CallbackInvoked - %d",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 gCallBackInvoked
                 );

  //
  // Unhook the Callback function
  //
  UnHookReturnAbortCallBack (BcInterface);

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.UdpWrite() Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.6
//
EFI_STATUS
BBTestUdpWriteConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;
  EFI_IP_ADDRESS                        DestIp;
  EFI_PXE_BASE_CODE_UDP_PORT            DestPort;
  UINTN                                 BufferSize;
  VOID                                  *BufferPtr;

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

  BufferSize = MAX_UDP_SIZE;
  Status = gtBS->AllocatePool (EfiBootServicesData, BufferSize, &BufferPtr);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  // Disable EFI_PXE_BASE_CODE_PROTOCOL protocol if needed
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // 4.6.2.1 Call UdpWrite() while PXE Base Code Protocol Disabled
  //
  DestIp.Addr[0] = 172;
  DestIp.Addr[1] = 16;
  DestIp.Addr[2] = 220;
  DestIp.Addr[3] = 100;
  DestPort = 200;
  BufferSize = MAX_UDP_SIZE/2;
  Status = BcInterface->UdpWrite (
                          BcInterface,
                          0,
                          &DestIp,
                          &DestPort,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          &BufferSize,
                          BufferPtr
                          );

  // Check Return Code
  if (Status == EFI_NOT_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid019,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.UdpWrite - PXE not started",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  // Enable EFI_PXE_BASE_CODE_PROTOCOL protocol if needed
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, TRUE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get IP
  //
  Print (L"\r\nPlease Setup DHCP Server\r\n");
  WaitForAnyInput ();
  Status = BcInterface->Dhcp (BcInterface, FALSE);
  if (EFI_ERROR(Status))
  {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.UdpWrite - Can't DHCP",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  //
  // 4.6.2.3 Require to send oversized UDP Packet
  //
  DestIp.Addr[0] = 172;
  DestIp.Addr[1] = 16;
  DestIp.Addr[2] = 220;
  DestIp.Addr[3] = 100;
  DestPort = 200;
  BufferSize = MAX_UDP_SIZE*2;
  Status = BcInterface->UdpWrite (
                          BcInterface,
                          0,
                          &DestIp,
                          &DestPort,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          &BufferSize,
                          BufferPtr
                          );

  // Check Return Code
  if (Status == EFI_BAD_BUFFER_SIZE) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid020,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.UdpWrite - Send oversized UDP Packet",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // 4.6.2.4 Call Back Function abort while UdpWrite()
  //
  Status = HookReturnAbortCallBack (EFI_PXE_BASE_CODE_FUNCTION_UDP_WRITE, BcInterface);

  DestIp.Addr[0] = 172;
  DestIp.Addr[1] = 16;
  DestIp.Addr[2] = 220;
  DestIp.Addr[3] = 100;
  DestPort = 200;
  BufferSize = MAX_UDP_SIZE/2;
  Status = BcInterface->UdpWrite (
                          BcInterface,
                          0,
                          &DestIp,
                          &DestPort,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          &BufferSize,
                          BufferPtr
                          );

  // Check Return Code
  if ((Status == EFI_ABORTED) && gCallBackInvoked) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid021,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.UdpWrite - Callback abort",
                 L"%a:%d:Status - %r, CallbackInvoked - %d",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 gCallBackInvoked
                 );

  //
  // Unhook the Callback function
  //
  UnHookReturnAbortCallBack (BcInterface);

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.UdpRead() Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.7
//
EFI_STATUS
BBTestUdpReadConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;
  UINT16                                OpFlags;
  UINTN                                 BufferSize;
  VOID                                  *BufferPtr;
  EFI_PXE_BASE_CODE_UDP_PORT            DestPort;
  EFI_IP_ADDRESS                        SrcIp;
  EFI_PXE_BASE_CODE_UDP_PORT            SrcPort;

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
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  // Malloc for data
  BufferSize = MAX_UDP_SIZE;
  Status = gtBS->AllocatePool (EfiBootServicesData, BufferSize, &BufferPtr);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Disable EFI_PXE_BASE_CODE_PROTOCOL protocol if needed
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  OpFlags = EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_DEST_PORT |
            EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_IP |
            EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_PORT;

  Status = BcInterface->UdpRead (
                          BcInterface,
                          OpFlags,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          &BufferSize,
                          BufferPtr
                          );
  if (Status == EFI_NOT_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid022,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.UdpRead - PXE not started",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  // Enable EFI_PXE_BASE_CODE_PROTOCOL protocol if needed
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, TRUE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get IP
  //
  Print (L"\r\nPlease Setup DHCP Server\r\n");
  WaitForAnyInput ();
  Status = BcInterface->Dhcp (BcInterface, FALSE);
  if (EFI_ERROR(Status))
  {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Discover - Can't DHCP",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  //
  // 4.7.2.2 Call UdpRead() with invalid parameter
  //

  // ANY_DEST_PORT bit in OpFlags is 0, DestPort is NULL
  BufferSize = MAX_UDP_SIZE;
  OpFlags = EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_IP |
            EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_PORT;

  Status = BcInterface->UdpRead (
                        BcInterface,
                        OpFlags,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        &BufferSize,
                        BufferPtr
                        );
  if (Status == EFI_INVALID_PARAMETER) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid023,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.UdpRead - DestPort is NUL",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  // ANY_SRC_PORT bit in OpFlags is 0, SrctPort is NULL
  BufferSize = MAX_UDP_SIZE;
  OpFlags = EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_DEST_PORT |
            EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_IP;

  Status = BcInterface->UdpRead (
                        BcInterface,
                        OpFlags,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        &BufferSize,
                        BufferPtr
                        );
  if (Status == EFI_INVALID_PARAMETER) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid024,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.UdpRead - SrctPort is NULL",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  // ANY_SRC_IP bit in OpFlags is 0, SrctIp is NULL
  BufferSize = MAX_UDP_SIZE;
  OpFlags = EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_DEST_PORT |
            EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_PORT;

  Status = BcInterface->UdpRead (
                          BcInterface,
                          OpFlags,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          &BufferSize,
                          BufferPtr
                          );
  if (Status == EFI_INVALID_PARAMETER) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid025,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.UdpRead - SrctIp is NULL",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // 4.7.2.4 Call UdpRead() while no packet incoming
  //
  BufferSize = MAX_UDP_SIZE;
  OpFlags = 0;
  DestPort = 200;
  SrcIp.Addr[0] = 172;
  SrcIp.Addr[1] = 16;
  SrcIp.Addr[2] = 220;
  SrcIp.Addr[3] = 123;
  SrcPort = 200;

  Status = BcInterface->UdpRead (
                          BcInterface,
                          OpFlags,
                          NULL,
                          &DestPort,
                          &SrcIp,
                          &SrcPort,
                          NULL,
                          NULL,
                          &BufferSize,
                          BufferPtr
                          );
  if (Status == EFI_TIMEOUT) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid026,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.UdpRead - EFI_TIMEOUT",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // 4.7.2.5 Call Back Function abort while UdpRead()
  //
  Status = HookReturnAbortCallBack (EFI_PXE_BASE_CODE_FUNCTION_UDP_READ, BcInterface);

  //Wait for user send packet
  Print (L"\r\nSend UDP Packet to this host");
  WaitForAnyInput ();

  BufferSize = MAX_UDP_SIZE;
  OpFlags = EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_DEST_PORT |
            EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_IP |
            EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_PORT;

  Status = BcInterface->UdpRead (
                          BcInterface,
                          OpFlags,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          &BufferSize,
                          BufferPtr
                          );
  if ((Status == EFI_ABORTED) && gCallBackInvoked) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid027,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.UdpRead - Callback abort",
                 L"%a:%d:Status - %r, CallbackInvoked - %d",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 gCallBackInvoked
                 );

  //
  // Unhook the Callback function
  //
  UnHookReturnAbortCallBack (BcInterface);

  gtBS->FreePool (BufferPtr);
  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.SetIpFilter() Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.8
//
EFI_STATUS
BBTestSetIpFilterConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;
  EFI_PXE_BASE_CODE_IP_FILTER           BcIpFilter;

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
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  // Disable EFI_PXE_BASE_CODE_PROTOCOL protocol if needed
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // 4.8.2.1 Call SetIpFilter() while PXE Base Code Protocol Disabled
  //
  BcIpFilter.Filters = EFI_PXE_BASE_CODE_IP_FILTER_STATION_IP;
  BcIpFilter.IpCnt =2;
  SetIpAddress ((EFI_IP_ADDRESS *)&(BcIpFilter.IpList[0]), 0x12345678);
  SetIpAddress ((EFI_IP_ADDRESS *)&(BcIpFilter.IpList[1]), 0x87654321);

  Status = BcInterface->SetIpFilter (BcInterface, &BcIpFilter);
  if (Status == EFI_NOT_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid028,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.SetIpFilter - PXE not started",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  return EFI_SUCCESS;
}


/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.Arp() Conformance Auto Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.9
//
EFI_STATUS
BBTestArpConformanceAutoTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL     *ProfileLib;
  EFI_TEST_LOGGING_LIBRARY_PROTOCOL     *LoggingLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;
  EFI_IP_ADDRESS                        IpAddr;

  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  //
  // Get support library (Standard Lib, Profile Lib, Logging Lib)
  //
  Status = GetTestSupportLibrary (
             SupportHandle,
             &StandardLib,
             &ProfileLib,
             &LoggingLib
             );
  if (EFI_ERROR(Status)){
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.ARP - Get Support Library Failed",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  //
  // Disable EFI_PXE_BASE_CODE_PROTOCOL Protocol interface
  //
  if (BcInterface->Mode->Started) {
    Status = BcInterface->Stop (BcInterface);
    if (EFI_ERROR (Status)) {
      StandardLib->RecordAssertion (
                     StandardLib,
                     EFI_TEST_ASSERTION_FAILED,
                     gTestGenericFailureGuid,
                     L"EFI_PXE_BASE_CODE_PROTOCOL.Stop - Can't change the PXE state to disabled",
                     L"%a:%d:Status - %r",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status
                     );
      return Status;
    }
  }

  //
  // Enable EFI_PXE_BASE_CODE_PROTOCOL Protocol interface in Ipv4
  //
  if (BcInterface->Mode->Ipv6Supported == TRUE) {
    Status = BcInterface->Start (BcInterface, TRUE);
  } else {
    Status = BcInterface->Start (BcInterface, FALSE);
  }
  
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Start - Check Return Code",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  //
  // Test EFI_INVALID_PARAMETER case 
  // 
  Status = BcInterface->Arp(BcInterface, NULL, NULL);
  if (Status == EFI_INVALID_PARAMETER) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid035,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Arp - IpAddr is NULL",
                 L"%a:%d:Status - %r Expected Status %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 EFI_INVALID_PARAMETER
                 );
  //
  // if Ip6 support test EFI_UNSUPPORT case
  //
  if (BcInterface->Mode->UsingIpv6 == TRUE) {
    MemSet(IpAddr.v6.Addr, 0, sizeof(IpAddr.v6.Addr));
    
    Status = BcInterface->Arp(BcInterface, &IpAddr, NULL);
    if (Status == EFI_UNSUPPORTED) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }

    StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid036,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Arp - UsingIp6 Enabled",
                 L"%a:%d:Status - %r Expected Status %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 EFI_UNSUPPORTED
                 );
  }

  return EFI_SUCCESS;
}
/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.Arp() Conformance Manual Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.9
//
EFI_STATUS
BBTestArpConformanceManualTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL     *ProfileLib;
  EFI_TEST_LOGGING_LIBRARY_PROTOCOL     *LoggingLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;
  EFI_IP_ADDRESS                        IpAddr;
  EFI_MAC_ADDRESS                       MacAddr;
  EFI_INI_FILE_HANDLE                   FileHandle;

  //
  // Get support library (Standard Lib, Profile Lib, Logging Lib)
  //
  Status = GetTestSupportLibrary (
             SupportHandle,
             &StandardLib,
             &ProfileLib,
             &LoggingLib
             );
  if (EFI_ERROR(Status)){
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.ARP - Get Support Library Failed",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    return Status;
  }

  //
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  // Disable EFI_PXE_BASE_CODE_PROTOCOL protocol if needed
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get the interface of the INI file.
  //
  Status = OpenTestIniFile (ProfileLib, &FileHandle);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Get IP to be resolved From configuration file
  Status = GetIpAddrFromFile (FileHandle, L"ARP_CONF", L"IP_Addr", 0, &IpAddr);
  if (EFI_ERROR(Status)){
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.ARP - Get IP from config file",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    CloseTestIniFile (ProfileLib, FileHandle);
    return Status;
  }

  //
  // 4.9.2.1 Call Arp() while PXE Base Code Protocol Disabled
  //
  Status = BcInterface->Arp (BcInterface, &IpAddr, &MacAddr);
  if (Status == EFI_NOT_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid029,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Arp - PXE not started",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  // Get non-existent IP
  Status = GetIpAddrFromFile (FileHandle, L"ARP_CONF", L"NOEXIST_IP", 0, &IpAddr);
  if (EFI_ERROR(Status)){
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.ARP - Get IP from config file",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    CloseTestIniFile (ProfileLib, FileHandle);
    return Status;
  }

  // Enable EFI_PXE_BASE_CODE_PROTOCOL protocol if needed
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, TRUE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get IP
  //
  Print (L"\r\nPlease Setup DHCP Server\r\n");
  WaitForAnyInput ();
  Status = BcInterface->Dhcp (BcInterface, FALSE);
  if (EFI_ERROR(Status))
  {
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.Discover - Can't DHCP",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  //
  // 4.9.2.3 Call Arp() for un-existed host
  //
  Status = BcInterface->Arp (BcInterface, &IpAddr, &MacAddr);
  if (Status == EFI_TIMEOUT) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid030,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Arp - Non-existent IP",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  // Get IP to be resolved From configuration file
  Status = GetIpAddrFromFile (FileHandle, L"ARP_CONF", L"IP_Addr", 0, &IpAddr);
  if (EFI_ERROR(Status)){
    StandardLib->RecordAssertion (
                   StandardLib,
                   EFI_TEST_ASSERTION_FAILED,
                   gTestGenericFailureGuid,
                   L"EFI_PXE_BASE_CODE_PROTOCOL.ARP - Get IP from config file",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    CloseTestIniFile (ProfileLib, FileHandle);
    return Status;
  }

  CloseTestIniFile (ProfileLib, FileHandle);

  //
  // 4.9.2.4 Call Back Function abort while Arp()
  //
  Status = HookReturnAbortCallBack (EFI_PXE_BASE_CODE_FUNCTION_ARP, BcInterface);

  Status = BcInterface->Arp (BcInterface, &IpAddr, &MacAddr);
  if ((Status == EFI_ABORTED) && gCallBackInvoked) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid031,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.Arp - Callback abort",
                 L"%a:%d:Status - %r, CallbackInvoked - %d",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 gCallBackInvoked
                 );

  //
  // Unhook the Callback function
  //
  UnHookReturnAbortCallBack (BcInterface);

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.SetParameters() Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.10
//
EFI_STATUS
BBTestSetParametersConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;

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
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  // Disable EFI_PXE_BASE_CODE_PROTOCOL protocol if needed
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // 4.10.2.1 Call SetParameter() while PXE Base Code Protocol Disabled
  //
  Status = BcInterface->SetParameters (
                          BcInterface,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL
                          );
  if (Status ==  EFI_NOT_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid032,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.SetParameters - PXE not started",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.SetStationIp() Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.11
//
EFI_STATUS
BBTestSetStationIpConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;

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
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  // Disable EFI_PXE_BASE_CODE_PROTOCOL protocol if needed
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // 4.11.2.1 Call SetStationIp() while PXE Base Code Protocol Disabled
  //
  Status = BcInterface->SetStationIp (BcInterface, NULL, NULL);
  if (Status ==  EFI_NOT_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid033,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.SetStationIp - PXE not started",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  return EFI_SUCCESS;
}

/**
 *  Entrypoint for EFI_PXE_BASE_CODE_PROTOCOL.SetPackets() Conformance Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 5.12
//
EFI_STATUS
BBTestSetPacketsConformanceTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_PXE_BASE_CODE_PROTOCOL            *BcInterface;

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
  // Get the EFI_PXE_BASE_CODE_PROTOCOL Protocol interface to be tested
  //
  BcInterface = (EFI_PXE_BASE_CODE_PROTOCOL *)ClientInterface;

  // Disable EFI_PXE_BASE_CODE_PROTOCOL protocol if needed
  Status = ChangePxeState (BcInterface, FALSE, BcInterface->Mode->Started, FALSE);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // 4.12.2.1 Call SetPackets() while PXE Base Code Protocol Disabled
  //
  Status = BcInterface->SetPackets (
                          BcInterface,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL
                          );
  if (Status ==  EFI_NOT_STARTED) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gPxeBaseCodeBBTestConformanceAssertionGuid034,
                 L"EFI_PXE_BASE_CODE_PROTOCOL.SetPackets - PXE not started",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  return EFI_SUCCESS;
}

