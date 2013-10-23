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
                                                                
  Copyright 2006 - 2013 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010 - 2013, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:

  BisBBTestCase.c

Abstract:

  Basic Test Cases of Bis Protocol

--*/

#include "BisBBTestMain.h"

/**
 *  Entrypoint for EFI_BIS_PROTOCOL Manual Test.
 *  @param This a pointer of EFI_BB_TEST_PROTOCOL.
 *  @param ClientInterface a pointer to the interface to be tested.
 *  @param TestLevel test "thoroughness" control.
 *  @param SupportHandle a handle containing protocols required.
 *  @return EFI_SUCCESS Finish the test successfully.
 */
//
// TDS 3.1
//
EFI_STATUS
BBTestManualTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_STATUS                            Status;
  EFI_TEST_ASSERTION                    AssertionType;
  EFI_BIS_PROTOCOL                      *BisInterface;
  UINTN                                 Index;
  BIS_APPLICATION_HANDLE                AppHandle;
  EFI_BIS_VERSION                       InterfaceVersion;
  EFI_BIS_DATA                          TargetAddress;
  EFI_BIS_DATA                          *SignatureData;
  UINT32                                SigInfoCount;
  EFI_BIS_SIGNATURE_INFO                *SignatureInfo;
  EFI_BIS_DATA                          *Certificate;
  EFI_BIS_DATA                          *UpdateToken;
  EFI_BIS_DATA                          RequestCredential;
  BOOLEAN                               CheckIsRequired;
  EFI_BIS_DATA                          DataObject;
  BOOLEAN                               IsVerified;
  EFI_BIS_DATA                          AuthorityCertificate;
  EFI_BIS_DATA                          SectionName;

  //XXX

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
  // Get the EFI_BIS_PROTOCOL Protocol interface to be tested
  //
  BisInterface = (EFI_BIS_PROTOCOL *)ClientInterface;

  //
  // 3.1.2.1 Initialize BIS service
  //
  InterfaceVersion.Major = BIS_CURRENT_VERSION_MAJOR;
  InterfaceVersion.Minor = 0;
  TargetAddress.Length = 0;
  TargetAddress.Data = NULL;
  Status = BisInterface->Initialize (
                           BisInterface,
                           &AppHandle,
                           &InterfaceVersion,
                           &TargetAddress
                           );
  if (Status == EFI_SUCCESS) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gBisBBTestCaseAssertionGuid001,
                 L"EFI_BIS_PROTOCOL.Initialize - Initialize BIS service",
                 L"%a:%d:Status - %r, Version %d.%d",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 InterfaceVersion.Major,
                 InterfaceVersion.Minor
                 );

  if (EFI_ERROR(Status)) {
    return EFI_SUCCESS;
  }

  //
  // 3.1.2.2 Initialize BIS service with invalid parameters
  //
  Print (L"\nDo you want to do invalid parameters test for Initialize()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  if (PressYes ()) {
    Status = BisInterface->Initialize (
                             BisInterface,
                             NULL,    // invalid
                             &InterfaceVersion,
                             &TargetAddress
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid002,
                   L"EFI_BIS_PROTOCOL.Initialize - AppHandle is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    Status = BisInterface->Initialize (
                             BisInterface,
                             &AppHandle,
                             NULL,  // invalid
                             &TargetAddress
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid003,
                   L"EFI_BIS_PROTOCOL.Initialize - InterfaceVersion is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    Status = BisInterface->Initialize (
                             BisInterface,
                             &AppHandle,
                             &InterfaceVersion,
                             NULL   // invalid
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid004,
                   L"EFI_BIS_PROTOCOL.Initialize - TargetAddress is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  } else {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Skip invalid parameters test for Initialize()"
                   );
  }

  //
  // 3.1.2.3 GetSignatureInfo (User interactive)
  //
  Status = BisInterface->GetSignatureInfo (
                           AppHandle,
                           &SignatureData
                           );
  if (Status == EFI_SUCCESS) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gBisBBTestCaseAssertionGuid005,
                 L"EFI_BIS_PROTOCOL.GetSignatureInfo - Get Signature Info",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  if (EFI_ERROR(Status)) {
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }

  //
  // Display all the Signature Info
  //
  SigInfoCount = BIS_GET_SIGINFO_COUNT(SignatureData);
  SignatureInfo = BIS_GET_SIGINFO_ARRAY(SignatureData);
  StandardLib->RecordMessage (
                 StandardLib,
                 EFI_VERBOSE_LEVEL_QUIET,
                 L"Signature Info (%d):",
                 SigInfoCount
                 );
  for (Index = 0; Index < SigInfoCount; Index++) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"  CertificateID: %d, AlgorithmID: %s(%d), KeyLength: %d",
                   SignatureInfo[Index].CertificateID,
                   AlgToStr (SignatureInfo[Index].AlgorithmID),
                   SignatureInfo[Index].AlgorithmID,
                   SignatureInfo[Index].KeyLength
                   );
  }

  //
  // Continue?
  //
  Print (L"\nDo you want to continue?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  if (!PressYes ()) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Quit Bis protocol test"
                   );
    Status = BisInterface->Free (
                             AppHandle,
                             SignatureData
                             );
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }

  //
  // 3.1.2.4 GetSignatureInfo with invalid parameters
  //
  Print (L"\nDo you want to do invalid parameters test for GetSignatureInfo()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  if (PressYes ()) {
    Status = BisInterface->GetSignatureInfo (
                             AppHandle,
                             NULL      // invalid
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid006,
                   L"EFI_BIS_PROTOCOL.GetSignatureInfo - SignatureInfo is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  } else {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Skip invalid parameters test for GetSignatureInfo()"
                   );
  }

  //
  // 3.1.2.5  Free SignatureInfo
  //
  Status = BisInterface->Free (
                           AppHandle,
                           SignatureData
                           );
  if (Status == EFI_SUCCESS) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gBisBBTestCaseAssertionGuid007,
                 L"EFI_BIS_PROTOCOL.Free - Free SignatureInfo",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // 3.1.2.6 Free SignatureInfo again
  //
  Print (L"\nDo you want to do invalid parameters test for Free()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  if (PressYes ()) {
    Status = BisInterface->Free (
                             AppHandle,
                             SignatureData  // invalid
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid008,
                   L"EFI_BIS_PROTOCOL.Free - Free SignatureInfo again",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  } else {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Skip invalid parameters test for Free()"
                   );
  }

  //
  // 3.1.2.7 GetBootObjectAuthorizationCertificate when there is no Certificate
  //
  Status = BisInterface->GetBootObjectAuthorizationCertificate (
                           AppHandle,
                           &Certificate
                           );
  if (Status == EFI_NOT_FOUND) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gBisBBTestCaseAssertionGuid009,
                 L"EFI_BIS_PROTOCOL.GetBootObjectAuthorizationCertificate - when there is no Certificate",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );
  if (Status == EFI_SUCCESS) {
    //
    // Environment incorrect, dump the current certificate and quit the test
    //
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Dump the Certificate to floppy disk file: %s",
                   DUMP_CERTIFICATE_FILE_NAME
                   );
    Status = WriteFloppyFile (
               DUMP_CERTIFICATE_FILE_NAME,
               Certificate->Length,
               Certificate->Data
               );
    Status = BisInterface->Free (
                             AppHandle,
                             Certificate
                             );
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }

  //
  // 3.1.2.8 GetBootObjectAuthorizationUpdateToken
  //
  Status = BisInterface->GetBootObjectAuthorizationUpdateToken (
                           AppHandle,
                           &UpdateToken
                           );
  if (Status == EFI_SUCCESS) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gBisBBTestCaseAssertionGuid010,
                 L"EFI_BIS_PROTOCOL.GetBootObjectAuthorizationUpdateToken",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  if (EFI_ERROR(Status)) {
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }

  //
  // Dump the current Token
  //
  StandardLib->RecordMessage (
                 StandardLib,
                 EFI_VERBOSE_LEVEL_QUIET,
                 L"Dump the Token to floppy disk file: %s",
                 DUMP_TOKEN_FILE_NAME
                 );
  Status = WriteFloppyFile (
             DUMP_TOKEN_FILE_NAME,
             UpdateToken->Length,
             UpdateToken->Data
             );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Can't dump the Token to floppy disk file: %s - %r",
                   DUMP_TOKEN_FILE_NAME,
                   Status
                   );
    Status = BisInterface->Free (
                             AppHandle,
                             UpdateToken
                             );
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }

  Status = BisInterface->Free (
                           AppHandle,
                           UpdateToken
                           );

  //
  // 3.1.2.9 GetBootObjectAuthorizationUpdateToken with invalid parameters
  //
  Print (L"\nDo you want to do invalid parameters test for GetBootObjectAuthorizationUpdateToken()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  if (PressYes ()) {
    Status = BisInterface->GetBootObjectAuthorizationUpdateToken (
                             AppHandle,
                             NULL   // invalid
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid011,
                   L"EFI_BIS_PROTOCOL.GetBootObjectAuthorizationUpdateToken - UpdateToken is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  } else {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Skip invalid parameters test for GetBootObjectAuthorizationUpdateToken()"
                   );
  }

  //
  // 3.1.2.10 Call UpdateBootObjectAuthorization to update Certificate (User interactive)
  //
  Print (L"\nPlease create credential file(%s) on floppy disk for first time Certificate updating\n", CREDENTIAL_FILE_NAME);
  Print (L"Press any key to continue\n");
  PressAnyKey ();

  //
  // Read the credential file
  //
  Status = ReadFloppyFile (
             CREDENTIAL_FILE_NAME,
             &RequestCredential.Length,
             &RequestCredential.Data
             );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Can't read the credential file: %s - %r",
                   CREDENTIAL_FILE_NAME,
                   Status
                   );
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }

  //
  // UpdateBootObjectAuthorization
  //
  Status = BisInterface->UpdateBootObjectAuthorization (
                           AppHandle,
                           &RequestCredential,
                           &UpdateToken
                           );
  if (Status == EFI_SUCCESS) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gBisBBTestCaseAssertionGuid012,
                 L"EFI_BIS_PROTOCOL.UpdateBootObjectAuthorization - Update certificate",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );
  gtBS->FreePool (RequestCredential.Data);
  if (EFI_ERROR(Status)) {
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }

  //
  // Dump the new Token
  //
  StandardLib->RecordMessage (
                 StandardLib,
                 EFI_VERBOSE_LEVEL_QUIET,
                 L"Dump the Token to floppy disk file: %s",
                 DUMP_TOKEN_FILE_NAME
                 );
  Status = WriteFloppyFile (
             DUMP_TOKEN_FILE_NAME,
             UpdateToken->Length,
             UpdateToken->Data
             );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Can't dump the Token to floppy disk file: %s - %r",
                   DUMP_TOKEN_FILE_NAME,
                   Status
                   );
    Status = BisInterface->Free (
                             AppHandle,
                             UpdateToken
                             );
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }
  Status = BisInterface->Free (
                           AppHandle,
                           UpdateToken
                           );

  //
  // 3.1.2.11 Call UpdateBootObjectAuthorization to enable authorization check (User interactive)
  //
  Print (L"\nPlease create credential file(%s) on floppy disk for authorization check enabling\n", CREDENTIAL_FILE_NAME);
  Print (L"Press any key to continue\n");
  PressAnyKey ();

  //
  // Read the credential file
  //
  Status = ReadFloppyFile (
             CREDENTIAL_FILE_NAME,
             &RequestCredential.Length,
             &RequestCredential.Data
             );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Can't read the credential file: %s - %r",
                   CREDENTIAL_FILE_NAME,
                   Status
                   );
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }

  //
  // UpdateBootObjectAuthorization
  //
  Status = BisInterface->UpdateBootObjectAuthorization (
                           AppHandle,
                           &RequestCredential,
                           &UpdateToken
                           );
  if (Status == EFI_SUCCESS) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gBisBBTestCaseAssertionGuid013,
                 L"EFI_BIS_PROTOCOL.UpdateBootObjectAuthorization - Enable check",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );
  gtBS->FreePool (RequestCredential.Data);
  if (EFI_ERROR(Status)) {
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }

  //
  // Dump the new Token
  //
  StandardLib->RecordMessage (
                 StandardLib,
                 EFI_VERBOSE_LEVEL_QUIET,
                 L"Dump the Token to floppy disk file: %s",
                 DUMP_TOKEN_FILE_NAME
                 );
  Status = WriteFloppyFile (
             DUMP_TOKEN_FILE_NAME,
             UpdateToken->Length,
             UpdateToken->Data
             );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Can't dump the Token to floppy disk file: %s - %r",
                   DUMP_TOKEN_FILE_NAME,
                   Status
                   );
    Status = BisInterface->Free (
                             AppHandle,
                             UpdateToken
                             );
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }
  Status = BisInterface->Free (
                           AppHandle,
                           UpdateToken
                           );

  //
  // 3.1.2.12 Call UpdateBootObjectAuthorization with invalid parameters
  //
  Print (L"\nDo you want to do invalid parameters test for UpdateBootObjectAuthorization()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  if (PressYes ()) {
    Status = BisInterface->UpdateBootObjectAuthorization (
                             AppHandle,
                             NULL,  // invalid
                             &UpdateToken
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid014,
                   L"EFI_BIS_PROTOCOL.UpdateBootObjectAuthorization - RequestCredential is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    Status = BisInterface->UpdateBootObjectAuthorization (
                             AppHandle,
                             &RequestCredential,
                             NULL   // invalid
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid015,
                   L"EFI_BIS_PROTOCOL.UpdateBootObjectAuthorization - NewUpdateToken is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  } else {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Skip invalid parameters test for UpdateBootObjectAuthorization()"
                   );
  }

  //
  // 3.1.2.13 Call UpdateBootObjectAuthorization with invalid Credential (User interactive)
  //
  Print (L"\nDo you want to do security violation test for UpdateBootObjectAuthorization()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  while (PressYes ()) {
    Print (L"\nPlease create invalid credential file(%s) on floppy disk\n", CREDENTIAL_FILE_NAME);
    Print (L"Press any key to continue\n");
    PressAnyKey ();

    //
    // Read the invalid credential file
    //
    Status = ReadFloppyFile (
               CREDENTIAL_FILE_NAME,
               &RequestCredential.Length,
               &RequestCredential.Data
               );
    if (EFI_ERROR(Status)) {
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_QUIET,
                     L"Can't read the credential file: %s - %r",
                     CREDENTIAL_FILE_NAME,
                     Status
                     );
      Print (L"\nDo you want to do security violation test for UpdateBootObjectAuthorization() again?\n");
      Print (L"Please press \"Y\" or \"N\"\n");
      continue;
    }

    //
    // UpdateBootObjectAuthorization
    //
    Status = BisInterface->UpdateBootObjectAuthorization (
                             AppHandle,
                             &RequestCredential,
                             &UpdateToken
                             );
    if (Status == EFI_SECURITY_VIOLATION) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid016,
                   L"EFI_BIS_PROTOCOL.UpdateBootObjectAuthorization - Invalid Credential",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    gtBS->FreePool (RequestCredential.Data);

    Print (L"\nDo you want to do security violation test for UpdateBootObjectAuthorization() again?\n");
    Print (L"Please press \"Y\" or \"N\"\n");
  }

  //
  // 3.1.2.14 GetBootObjectAuthorizationCertificate (User interactive)
  //
  Status = BisInterface->GetBootObjectAuthorizationCertificate (
                           AppHandle,
                           &Certificate
                           );
  if (Status == EFI_SUCCESS) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gBisBBTestCaseAssertionGuid017,
                 L"EFI_BIS_PROTOCOL.GetBootObjectAuthorizationCertificate",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );
  if (Status == EFI_SUCCESS) {
    //
    // Dump the current certificate
    //
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Dump the Certificate to floppy disk file: %s",
                   DUMP_CERTIFICATE_FILE_NAME
                   );
    Status = WriteFloppyFile (
               DUMP_CERTIFICATE_FILE_NAME,
               Certificate->Length,
               Certificate->Data
               );
    Status = BisInterface->Free (
                             AppHandle,
                             Certificate
                             );
    Print (L"\nPleaes verify the current certificate. Is current certificate correct?\n");
    Print (L"Please press \"Y\" or \"N\"\n");
    if (PressYes ()) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid018,
                   L"EFI_BIS_PROTOCOL.GetBootObjectAuthorizationCertificate - current certificate is correct",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  //
  // 3.1.2.15 GetBootObjectAuthorizationCertificate with invalid parameters
  //
  Print (L"\nDo you want to do invalid parameters test for GetBootObjectAuthorizationCertificate()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  if (PressYes ()) {
    Status = BisInterface->GetBootObjectAuthorizationCertificate (
                             AppHandle,
                             NULL   // invalid
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid019,
                   L"EFI_BIS_PROTOCOL.GetBootObjectAuthorizationCertificate - Certificate is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  } else {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Skip invalid parameters test for GetBootObjectAuthorizationCertificate()"
                   );
  }

  //
  // 3.1.2.16 GetBootObjectAuthorizationCheckFlag
  //
  Status = BisInterface->GetBootObjectAuthorizationCheckFlag (
                           AppHandle,
                           &CheckIsRequired
                           );
  if ((Status == EFI_SUCCESS) && (CheckIsRequired)) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gBisBBTestCaseAssertionGuid020,
                 L"EFI_BIS_PROTOCOL.GetBootObjectAuthorizationCheckFlag",
                 L"%a:%d:Status - %r, CheckIsRequired - %d",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status,
                 CheckIsRequired
                 );
  if (AssertionType == EFI_TEST_ASSERTION_FAILED) {
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    return EFI_SUCCESS;
  }

  //
  // 3.1.2.17 GetBootObjectAuthorizationCheckFlag with invalid parameters
  //
  Print (L"\nDo you want to do invalid parameters test for GetBootObjectAuthorizationCheckFlag()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  if (PressYes ()) {
    Status = BisInterface->GetBootObjectAuthorizationCheckFlag (
                             AppHandle,
                             NULL   // invalid
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid021,
                   L"EFI_BIS_PROTOCOL.GetBootObjectAuthorizationCheckFlag - CheckIsRequired is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  } else {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Skip invalid parameters test for GetBootObjectAuthorizationCheckFlag()"
                   );
  }

  //
  // 3.1.2.18 VerifyBootObject (User interactive)
  //
  Print (L"\nPlease create credential file(%s) and data file(%s) on floppy disk\n",
         CREDENTIAL_FILE_NAME, DATA_FILE_NAME);
  Print (L"Press any key to continue\n");
  PressAnyKey ();

  //
  // Read the credential file
  //
  Status = ReadFloppyFile (
             CREDENTIAL_FILE_NAME,
             &RequestCredential.Length,
             &RequestCredential.Data
             );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Can't read the credential file: %s - %r",
                   CREDENTIAL_FILE_NAME,
                   Status
                   );
  } else {
    //
    // Read the data file
    //
    Status = ReadFloppyFile (
               DATA_FILE_NAME,
               &DataObject.Length,
               &DataObject.Data
               );
    if (EFI_ERROR(Status)) {
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_QUIET,
                     L"Can't read the data file: %s - %r",
                     DATA_FILE_NAME,
                     Status
                       );
    } else {
      Status = BisInterface->VerifyBootObject (
                               AppHandle,
                               &RequestCredential,
                               &DataObject,
                               &IsVerified
                               );
      if ((Status == EFI_SUCCESS) && (IsVerified)) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }
      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gBisBBTestCaseAssertionGuid022,
                     L"EFI_BIS_PROTOCOL.VerifyBootObject",
                     L"%a:%d:Status - %r, IsVerified - %d",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status,
                     IsVerified
                     );
      gtBS->FreePool (DataObject.Data);
    }
    gtBS->FreePool (RequestCredential.Data);
  }

  //
  // 3.1.2.19 VerifyBootObject with invalid parameters
  //
  Print (L"\nDo you want to do invalid parameters test for VerifyBootObject()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  if (PressYes ()) {
    Status = BisInterface->VerifyBootObject (
                             AppHandle,
                             NULL, // invalid
                             &DataObject,
                             &IsVerified
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid023,
                   L"EFI_BIS_PROTOCOL.VerifyBootObject - Credentials is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    Status = BisInterface->VerifyBootObject (
                             AppHandle,
                             &RequestCredential,
                             NULL, // invalid
                             &IsVerified
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid024,
                   L"EFI_BIS_PROTOCOL.VerifyBootObject - DataObject is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    Status = BisInterface->VerifyBootObject (
                             AppHandle,
                             &RequestCredential,
                             &DataObject,
                             NULL  // invalid
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid025,
                   L"EFI_BIS_PROTOCOL.VerifyBootObject - IsVerified is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  } else {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Skip invalid parameters test for VerifyBootObject()"
                   );
  }

  //
  // 3.1.2.20 VerifyBootObject with invalid Credential or Data object (User interactive)
  //
  Print (L"\nDo you want to do security violation test for VerifyBootObject()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  while (PressYes ()) {
    Print (L"\nPlease create credential file(%s) and data file(%s) on floppy disk\n",
           CREDENTIAL_FILE_NAME, DATA_FILE_NAME);
    Print (L"Press any key to continue\n");
    PressAnyKey ();

    //
    // Read the credential file
    //
    Status = ReadFloppyFile (
               CREDENTIAL_FILE_NAME,
               &RequestCredential.Length,
               &RequestCredential.Data
               );
    if (EFI_ERROR(Status)) {
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_QUIET,
                     L"Can't read the credential file: %s - %r",
                     CREDENTIAL_FILE_NAME,
                     Status
                     );
    } else {
      //
      // Read the data file
      //
      Status = ReadFloppyFile (
                 DATA_FILE_NAME,
                 &DataObject.Length,
                 &DataObject.Data
                 );
      if (EFI_ERROR(Status)) {
        StandardLib->RecordMessage (
                       StandardLib,
                       EFI_VERBOSE_LEVEL_QUIET,
                       L"Can't read the data file: %s - %r",
                       DATA_FILE_NAME,
                       Status
                       );
      } else {
        Status = BisInterface->VerifyBootObject (
                                 AppHandle,
                                 &RequestCredential,
                                 &DataObject,
                                 &IsVerified
                                 );
        if ((Status == EFI_SECURITY_VIOLATION) && (!IsVerified)) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gBisBBTestCaseAssertionGuid026,
                       L"EFI_BIS_PROTOCOL.VerifyBootObject",
                       L"%a:%d:Status - %r, IsVerified - %d",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status,
                       IsVerified
                       );
        gtBS->FreePool (DataObject.Data);
      }
      gtBS->FreePool (RequestCredential.Data);
    }

    Print (L"\nDo you want to do security violation test for VerifyBootObject() again?\n");
    Print (L"Please press \"Y\" or \"N\"\n");
  }

  //
  // 3.1.2.21 VerifyObjectWithCredential (User interactive)
  //
  Print (L"\nPlease create credential file(%s), certificate file(%s) and data file(%s) on floppy disk\n",
         CREDENTIAL_FILE_NAME, CERTIFICATE_FILE_NAME, DATA_FILE_NAME);
  Print (L"The predefined section name: %s\n", SECTION_NAME);
  Print (L"Press any key to continue\n");
  PressAnyKey ();

  //
  // Read the credential file
  //
  Status = ReadFloppyFile (
             CREDENTIAL_FILE_NAME,
             &RequestCredential.Length,
             &RequestCredential.Data
             );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Can't read the credential file: %s - %r",
                   CREDENTIAL_FILE_NAME,
                   Status
                   );
  } else {
    //
    // Read the certificate file
    //
    Status = ReadFloppyFile (
               CERTIFICATE_FILE_NAME,
               &AuthorityCertificate.Length,
               &AuthorityCertificate.Data
               );
    if (EFI_ERROR(Status)) {
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_QUIET,
                     L"Can't read the certificate file: %s - %r",
                     CERTIFICATE_FILE_NAME,
                     Status
                     );
    } else {
      //
      // Read the data file
      //
      Status = ReadFloppyFile (
                 DATA_FILE_NAME,
                 &DataObject.Length,
                 &DataObject.Data
                 );
      if (EFI_ERROR(Status)) {
        StandardLib->RecordMessage (
                       StandardLib,
                       EFI_VERBOSE_LEVEL_QUIET,
                       L"Can't read the data file: %s - %r",
                       DATA_FILE_NAME,
                       Status
                       );
       } else {
        SectionName.Length = sizeof (SECTION_NAME);
        SectionName.Data = (UINT8 *) SECTION_NAME;
        // FOR debug
        Print (L"Length %d, SectionName %s", SectionName.Length, SectionName.Data);
        Status = BisInterface->VerifyObjectWithCredential (
                                 AppHandle,
                                 &RequestCredential,
                                 &DataObject,
                                 &SectionName,
                                 &AuthorityCertificate,
                                 &IsVerified
                                 );
        if ((Status == EFI_SUCCESS) && (IsVerified)) {
          AssertionType = EFI_TEST_ASSERTION_PASSED;
        } else {
          AssertionType = EFI_TEST_ASSERTION_FAILED;
        }
        StandardLib->RecordAssertion (
                       StandardLib,
                       AssertionType,
                       gBisBBTestCaseAssertionGuid027,
                       L"EFI_BIS_PROTOCOL.VerifyBootObject",
                       L"%a:%d:Status - %r, IsVerified - %d",
                       __FILE__,
                       (UINTN)__LINE__,
                       Status,
                       IsVerified
                       );
        gtBS->FreePool (DataObject.Data);
      }
      gtBS->FreePool (AuthorityCertificate.Data);
    }
    gtBS->FreePool (RequestCredential.Data);
  }

  //
  // 3.1.2.22 VerifyObjectWithCredential with invalid parameters
  //
  Print (L"\nDo you want to do invalid parameters test for VerifyObjectWithCredential()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  if (PressYes ()) {
    Status = BisInterface->VerifyObjectWithCredential (
                             AppHandle,
                             NULL,  // invalid
                             &DataObject,
                             &SectionName,
                             &AuthorityCertificate,
                             &IsVerified
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid028,
                   L"EFI_BIS_PROTOCOL.VerifyObjectWithCredential - Credentials is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    Status = BisInterface->VerifyObjectWithCredential (
                             AppHandle,
                             &RequestCredential,
                             NULL,  // invalid
                             &SectionName,
                             &AuthorityCertificate,
                             &IsVerified
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid029,
                   L"EFI_BIS_PROTOCOL.VerifyObjectWithCredential - DataObject is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    Status = BisInterface->VerifyObjectWithCredential (
                             AppHandle,
                             &RequestCredential,
                             &DataObject,
                             NULL,  // invalid
                             &AuthorityCertificate,
                             &IsVerified
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid030,
                   L"EFI_BIS_PROTOCOL.VerifyObjectWithCredential - SectionName is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    Status = BisInterface->VerifyObjectWithCredential (
                             AppHandle,
                             &RequestCredential,
                             &DataObject,
                             &SectionName,
                             NULL, // invalid
                             &IsVerified
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid031,
                   L"EFI_BIS_PROTOCOL.VerifyObjectWithCredential - AuthorityCertificate is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    Status = BisInterface->VerifyObjectWithCredential (
                             AppHandle,
                             &RequestCredential,
                             &DataObject,
                             &SectionName,
                             &AuthorityCertificate,
                             NULL  //  invalid
                             );
    if (Status == EFI_INVALID_PARAMETER) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid032,
                   L"EFI_BIS_PROTOCOL.VerifyObjectWithCredential - IsVerified is NULL",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  } else {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Skip invalid parameters test for VerifyObjectWithCredential()"
                   );
  }

  //
  // 3.1.2.23 VerifyObjectWithCredential with invalid Credential, Certificate or Data object (User interactive)
  //
  Print (L"\nDo you want to do security violation test for VerifyBootObject()?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  while (PressYes ()) {
    Print (L"\nPlease create credential file(%s), certificate file(%s) and data file(%s) on floppy disk\n",
           CREDENTIAL_FILE_NAME, CERTIFICATE_FILE_NAME, DATA_FILE_NAME);
    Print (L"The predefined section name: %s\n", SECTION_NAME);
    Print (L"Press any key to continue\n");
    PressAnyKey ();

    //
    // Read the credential file
    //
    Status = ReadFloppyFile (
               CREDENTIAL_FILE_NAME,
               &RequestCredential.Length,
               &RequestCredential.Data
               );
    if (EFI_ERROR(Status)) {
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_QUIET,
                     L"Can't read the credential file: %s - %r",
                     CREDENTIAL_FILE_NAME,
                     Status
                     );
    } else {
      //
      // Read the certificate file
      //
      Status = ReadFloppyFile (
                 CERTIFICATE_FILE_NAME,
                 &AuthorityCertificate.Length,
                 &AuthorityCertificate.Data
                 );
      if (EFI_ERROR(Status)) {
        StandardLib->RecordMessage (
                       StandardLib,
                       EFI_VERBOSE_LEVEL_QUIET,
                       L"Can't read the certificate file: %s - %r",
                       CERTIFICATE_FILE_NAME,
                       Status
                       );
      } else {
        //
        // Read the data file
        //
        Status = ReadFloppyFile (
                   DATA_FILE_NAME,
                   &DataObject.Length,
                   &DataObject.Data
                   );
        if (EFI_ERROR(Status)) {
          StandardLib->RecordMessage (
                         StandardLib,
                         EFI_VERBOSE_LEVEL_QUIET,
                         L"Can't read the data file: %s - %r",
                         DATA_FILE_NAME,
                         Status
                         );
         } else {
          SectionName.Length = sizeof (SECTION_NAME);
          SectionName.Data = (UINT8 *) SECTION_NAME;
          // FOR debug
          Print (L"Length %d, SectionName %s", SectionName.Length, SectionName.Data);
          Status = BisInterface->VerifyObjectWithCredential (
                                   AppHandle,
                                   &RequestCredential,
                                   &DataObject,
                                   &SectionName,
                                   &AuthorityCertificate,
                                   &IsVerified
                                   );
          if ((Status == EFI_SECURITY_VIOLATION) && (!IsVerified)) {
            AssertionType = EFI_TEST_ASSERTION_PASSED;
          } else {
            AssertionType = EFI_TEST_ASSERTION_FAILED;
          }
          StandardLib->RecordAssertion (
                         StandardLib,
                         AssertionType,
                         gBisBBTestCaseAssertionGuid033,
                         L"EFI_BIS_PROTOCOL.VerifyBootObject",
                         L"%a:%d:Status - %r, IsVerified - %d",
                         __FILE__,
                         (UINTN)__LINE__,
                         Status,
                         IsVerified
                         );
          gtBS->FreePool (DataObject.Data);
        }
        gtBS->FreePool (AuthorityCertificate.Data);
      }
      gtBS->FreePool (RequestCredential.Data);
    }
    Print (L"\nDo you want to do security violation test for VerifyBootObject() again?\n");
    Print (L"Please press \"Y\" or \"N\"\n");
  }

  //
  // 3.1.2.24 Call UpdateBootObjectAuthorization to disable authorization check (User interactive)
  //
  Print (L"\nPlease create credential file(%s) on floppy disk for authorization check disabling\n", CREDENTIAL_FILE_NAME);
  Print (L"Press any key to continue\n");
  PressAnyKey ();

  //
  // Read the credential file
  //
  Status = ReadFloppyFile (
             CREDENTIAL_FILE_NAME,
             &RequestCredential.Length,
             &RequestCredential.Data
             );
  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_QUIET,
                   L"Can't read the credential file: %s - %r",
                   CREDENTIAL_FILE_NAME,
                   Status
                   );
  } else {
    //
    // UpdateBootObjectAuthorization
    //
    Status = BisInterface->UpdateBootObjectAuthorization (
                             AppHandle,
                             &RequestCredential,
                             &UpdateToken
                             );
    if (Status == EFI_SUCCESS) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid034,
                   L"EFI_BIS_PROTOCOL.UpdateBootObjectAuthorization - Disable check",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
    gtBS->FreePool (RequestCredential.Data);

    if (Status == EFI_SUCCESS) {
      //
      // Dump the new Token
      //
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_QUIET,
                     L"Dump the Token to floppy disk file: %s",
                     DUMP_TOKEN_FILE_NAME
                     );
      Status = WriteFloppyFile (
                 DUMP_TOKEN_FILE_NAME,
                 UpdateToken->Length,
                 UpdateToken->Data
                 );
      if (EFI_ERROR(Status)) {
        StandardLib->RecordMessage (
                       StandardLib,
                       EFI_VERBOSE_LEVEL_QUIET,
                       L"Can't dump the Token to floppy disk file: %s - %r",
                       DUMP_TOKEN_FILE_NAME,
                       Status
                       );
      }
      Status = BisInterface->Free (
                               AppHandle,
                               UpdateToken
                               );
    }
  }

  //
  // 3.1.2.25 Shutdown BIS service
  //
  Status = BisInterface->Shutdown (
                           AppHandle
                           );
  if (Status == EFI_SUCCESS) {
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  } else {
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }
  StandardLib->RecordAssertion (
                 StandardLib,
                 AssertionType,
                 gBisBBTestCaseAssertionGuid035,
                 L"EFI_BIS_PROTOCOL.Shutdown - Shutdown BIS service",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  if (EFI_ERROR(Status)) {
    return EFI_SUCCESS;
  }


  //
  // Invalid AppHandle test for all interfaces
  //
  Print (L"\nDo you want to do invalid AppHandle test for all interfaces?\n");
  Print (L"Please press \"Y\" or \"N\"\n");
  if (PressYes ()) {
    //
    // 3.1.2.26 Shutdown BIS when AppHandle is invalid
    //
    Status = BisInterface->Shutdown (
                             AppHandle
                             );
    if (Status == EFI_NO_MAPPING) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid036,
                   L"EFI_BIS_PROTOCOL.Shutdown - AppHandle is invalid",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    //
    // 3.1.2.27 Free when AppHandle is invalid
    //
    Status = BisInterface->Free (
                             AppHandle,
                             SignatureData
                             );
    if (Status == EFI_NO_MAPPING) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid037,
                   L"EFI_BIS_PROTOCOL.Free - AppHandle is invalid",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    //
    // 3.1.2.28 GetSignatureInfo when AppHandle is invalid
    //
    Status = BisInterface->GetSignatureInfo (
                             AppHandle,
                             &SignatureData
                             );
    if (Status == EFI_NO_MAPPING) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid038,
                   L"EFI_BIS_PROTOCOL.GetSignatureInfo - AppHandle is invalid",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    //
    // 3.1.2.29 GetBootObjectAuthorizationCertificate when AppHandle is invalid
    //
    Status = BisInterface->GetBootObjectAuthorizationCertificate (
                             AppHandle,
                             &Certificate
                             );
    if (Status == EFI_NO_MAPPING) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid039,
                   L"EFI_BIS_PROTOCOL.GetBootObjectAuthorizationCertificate - AppHandle is invalid",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    //
    // 3.1.2.30 GetBootObjectAuthorizationCheckFlag when AppHandle is invalid
    //
    Status = BisInterface->GetBootObjectAuthorizationCheckFlag (
                             AppHandle,
                             &CheckIsRequired
                             );
    if (Status == EFI_NO_MAPPING) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid040,
                   L"EFI_BIS_PROTOCOL.GetBootObjectAuthorizationCheckFlag - AppHandle is invalid",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    //
    // 3.1.2.31 GetBootObjectAuthorizationUpdateToken when AppHandle is invalid
    //
    Status = BisInterface->GetBootObjectAuthorizationUpdateToken (
                             AppHandle,
                             &UpdateToken
                             );
    if (Status == EFI_NO_MAPPING) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid041,
                   L"EFI_BIS_PROTOCOL.GetBootObjectAuthorizationUpdateToken - AppHandle is invalid",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    //
    // 3.1.2.32 UpdateBootObjectAuthorization when AppHandle is invalid
    //
    Status = BisInterface->UpdateBootObjectAuthorization (
                             AppHandle,
                             &RequestCredential,
                             &UpdateToken
                             );
    if (Status == EFI_NO_MAPPING) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid042,
                   L"EFI_BIS_PROTOCOL.UpdateBootObjectAuthorization - AppHandle is invalid",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    //
    // 3.1.2.33 VerifyBootObject when AppHandle is invalid
    //
    Status = BisInterface->VerifyBootObject (
                             AppHandle,
                             &RequestCredential,
                             &DataObject,
                             &IsVerified
                             );
    if (Status == EFI_NO_MAPPING) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid043,
                   L"EFI_BIS_PROTOCOL.VerifyBootObject",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );

    //
    // 3.1.2.34 VerifyObjectWithCredential when AppHandle is invalid
    //
    Status = BisInterface->VerifyObjectWithCredential (
                             AppHandle,
                             &RequestCredential,
                             &DataObject,
                             &SectionName,
                             &AuthorityCertificate,
                             &IsVerified
                             );
    if (Status == EFI_NO_MAPPING) {
      AssertionType = EFI_TEST_ASSERTION_PASSED;
    } else {
      AssertionType = EFI_TEST_ASSERTION_FAILED;
    }
    StandardLib->RecordAssertion (
                   StandardLib,
                   AssertionType,
                   gBisBBTestCaseAssertionGuid044,
                   L"EFI_BIS_PROTOCOL.VerifyBootObject",
                   L"%a:%d:Status - %r",
                   __FILE__,
                   (UINTN)__LINE__,
                   Status
                   );
  }

  return EFI_SUCCESS;
}
