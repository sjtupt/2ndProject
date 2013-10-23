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

  Sct.c

Abstract:

  EFI SCT Framework

--*/

#include "Sct.h"

//
// Global veriables definition
//

EFI_SCT_FRAMEWORK_TABLE *gFT = NULL;

EFI_GUID gEfiSystemHangAssertionGuid = EFI_SYSTEM_HANG_ASSERTION_GUID;

UINTN gTestCaseMaxRunTimeMax = 0x7FFFFFFF;

BOOLEAN gForceExecution = FALSE;

//
// Internal functions declaration
//

VOID
PrintUsage (
  VOID
  );

EFI_STATUS
ParseCommandLine (
  IN EFI_HANDLE                   ImageHandle
  );

EFI_STATUS
SctMainFunc (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  );

//
// Check MonitorName if it is Mnp, Ip4 or Serial
//
STATIC
EFI_STATUS
CheckMonitorName (
  CHAR16  *Argv
  );

//
// Entry point
//
EFI_DRIVER_ENTRY_POINT(InitializeSct)

EFI_STATUS
InitializeSct (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
/*++

Routine Description:

  Entry point of SCT.

Arguments:

  ImageHandle   - The image handle.
  SystemTable   - The system table.

Returns:

  EFI_SUCCESS   - Successfully.
  OTHERS        - Something failed.

--*/
{
  EFI_STATUS  Status;
  EFI_TPL     OldTPL;

  //
  // Initialize libraries
  //
  EFI_SHELL_APP_INIT (ImageHandle, SystemTable);

  OldTPL = BS->RaiseTPL(EFI_TPL_HIGH_LEVEL);
  if(OldTPL != EFI_TPL_APPLICATION) {
    BS->RestoreTPL(OldTPL);
    Print(L"ERROR: SCT should run at EFI_TPL_APPLICATION level\n");
	return EFI_SUCCESS;
  }
  BS->RestoreTPL(OldTPL);

  //
  // Initialize the framework table
  //
  Status = InitializeFrameworkTable (ImageHandle, SystemTable);
  if (EFI_ERROR (Status)) {
    Print (L"ERROR: Cannot initialize the framework table. Status - %r\n", Status);
    return Status;
  }

  //
  // Initialize EfiInitializeEntsLib.
  //
  EfiInitializeEntsLib (ImageHandle, SystemTable);

  //
  // Parse the command line
  //
  Status = ParseCommandLine (ImageHandle);
  if (EFI_ERROR (Status)) {
    Print (L"ERROR: Invalid command line. Status - %r\n", Status);
    FreeFrameworkTable ();
    return Status;
  }

  //
  // Invoke the SCT main function to do the operations
  //
  Status = SctMainFunc (ImageHandle, SystemTable);
  if (EFI_ERROR (Status)) {
    Print (L"ERROR: Cannot do the operations. Status - %r\n", Status);
    FreeFrameworkTable ();
    return Status;
  }

  //
  // Free the framework table
  //
  Status = FreeFrameworkTable ();
  if (EFI_ERROR (Status)) {
    Print (L"ERROR: Cannot free the framework table. Status - %r\n", Status);
    return Status;
  }

  //
  // Done
  //
  Print (L"Done!\n");
  return EFI_SUCCESS;
}


//
// Internal functions implementation
//

VOID
PrintUsage (
  VOID
  )
/*++

Routine Description:

  Print out the usage model of the SCT.

--*/
{
  Print (
    L"%s\n"
    L"\n"
    L"usage:\n"
    L"%s [-a | -c | -s <seq> | -u | -p <MNP | IP4 | SERIAL>] [-r] [-g <report>][-v]\n"
    L"\n"
    L"  -a    Executes all test cases.\n"
    L"  -c    Continues execute the test cases.\n"
    L"  -g    Generates test report.\n"
    L"  -p    Passive Mode with specified communication layer\n"
    L"  -r    Resets all test results.\n"
    L"  -s    Executes the test cases in the test sequence file.\n"
    L"  -u    Turns into user-friendly interface.\n"
    L"  -f    Force the operation execution, no confirmation from user.\n"
    L"  -v    Verbose function to disable screen output.\n"
    L"\n",
    EFI_SCT_NAME,
    EFI_SCT_SHORT_NAME
    );
}

STATIC UINT32 SCT_VALID_OPERATIONS[] = {
  EFI_SCT_OPERATIONS_NONE,                                                                    // sct -h | sct -?
  EFI_SCT_OPERATIONS_ALL,                                                                     // sct -a
  EFI_SCT_OPERATIONS_ALL | EFI_SCT_OPERATIONS_VERBOSE,                                        // sct -a -v
  EFI_SCT_OPERATIONS_RESET,                                                                   // sct -r
  EFI_SCT_OPERATIONS_ALL | EFI_SCT_OPERATIONS_RESET,                                          // sct -r -a
  EFI_SCT_OPERATIONS_ALL | EFI_SCT_OPERATIONS_RESET | EFI_SCT_OPERATIONS_VERBOSE,             // sct -r -a -v
  EFI_SCT_OPERATIONS_CONTINUE,                                                                // sct -c
  EFI_SCT_OPERATIONS_CONTINUE | EFI_SCT_OPERATIONS_VERBOSE,                                   // sct -c -v
  EFI_SCT_OPERATIONS_REPORT,                                                                  // sct -g
  EFI_SCT_OPERATIONS_SEQUENCE,                                                                // sct -s
  EFI_SCT_OPERATIONS_SEQUENCE | EFI_SCT_OPERATIONS_VERBOSE,                                   // sct -s -v
  EFI_SCT_OPERATIONS_UI,                                                                      // sct -u
  EFI_SCT_OPERATIONS_RESET | EFI_SCT_OPERATIONS_UI,                                           // sct -r -u
  EFI_SCT_OPERATIONS_PASSIVEMODE,                                                             // sct -p
  EFI_SCT_OPERATIONS_PASSIVEMODE | EFI_SCT_OPERATIONS_VERBOSE,                                // sct -p -r
  EFI_SCT_OPERATIONS_PASSIVEMODE | EFI_SCT_OPERATIONS_SEQUENCE,                               // sct -s -p
  EFI_SCT_OPERATIONS_PASSIVEMODE | EFI_SCT_OPERATIONS_SEQUENCE | EFI_SCT_OPERATIONS_VERBOSE,  // sct -s -p -r
  EFI_SCT_OPERATIONS_PASSIVEMODE | EFI_SCT_OPERATIONS_CONTINUE,                               // sct -c -p
  EFI_SCT_OPERATIONS_PASSIVEMODE | EFI_SCT_OPERATIONS_CONTINUE | EFI_SCT_OPERATIONS_VERBOSE,  // sct -c -p -r
  EFI_SCT_OPERATIONS_REPORT | EFI_SCT_OPERATIONS_PASSIVEMODE,                                 // sct -g -p
  EFI_SCT_OPERATIONS_EXTENDED,                                                                // sct -x omit
  EFI_SCT_OPERATIONS_INVALID                                                                  // invalid end of table
};

STATIC
EFI_STATUS
SctCmdLineGetReportName (
  IN EFI_HANDLE                   ImageHandle,
  IN CHAR16                       *ArgStr
  )
{
  EFI_STATUS                      Status;

  //
  // The current working directory is "Report"
  //
  Status = SctChangeDirectory (ImageHandle, EFI_SCT_PATH_REPORT);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = ExpandFileName (
             ArgStr,
             &gFT->RepDevicePath,
             &gFT->RepFileName
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Restore the current working directory
  //
  Status = SctChangeDirectory (ImageHandle, L"..");
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
VOID
SctCmdLineGetMonitorName (
  IN CHAR16                       *ArgStr
  )
{
  EFI_STATUS                      Status;

  Status = CheckMonitorName(ArgStr);
  if (!EFI_ERROR (Status)) {
    gFT->MonitorName = ArgStr;
  } else {
    gFT->MonitorName = NULL;
  }
}

STATIC
EFI_STATUS
SctCmdLineGetSequenceFileName (
  IN EFI_HANDLE                   ImageHandle,
  IN CHAR16                       *ArgStr
  )
{
  EFI_STATUS                      Status;

  //
  // The current working directory is "Sequence"
  //
  Status = SctChangeDirectory (ImageHandle, EFI_SCT_PATH_SEQUENCE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = ExpandFileName (
              ArgStr,
              &gFT->SeqDevicePath,
              &gFT->SeqFileName
              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Restore the current working directory
  //
  Status = SctChangeDirectory (ImageHandle, L"..");
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

#define SCT_OPERATION_MASK(OP)    (gFT->Operations |= (OP))
#define SCT_OPERATION_EQU(OP)     (gFT->Operations == (OP))

EFI_STATUS
ParseCommandLine (
  IN EFI_HANDLE                   ImageHandle
  )
{
  EFI_STATUS  Status;
  UINTN       Index;
  UINTN       Argc;
  CHAR16      **Argv;

  //
  // Get the parameters from the shell interface
  //
  Argc = SI->Argc;
  Argv = SI->Argv;

  gFT->Operations = 0;

  for (Index = 1; Index < Argc; Index++) {
    if (Argv[Index][0] == L'-' && Argv[Index][2] == L'\0') {
      switch(Argv[Index][1]) {
      case L'a': case L'A':
        SCT_OPERATION_MASK(EFI_SCT_OPERATIONS_ALL);
        break;
      case L'c': case L'C':
        SCT_OPERATION_MASK(EFI_SCT_OPERATIONS_CONTINUE);
		break;
      case L'g': case L'G':
        SCT_OPERATION_MASK(EFI_SCT_OPERATIONS_REPORT);
        Index++;
        if (Index == Argc) {
          EFI_SCT_DEBUG((EFI_SCT_D_ERROR, L"Not set the report file name"));
          return EFI_NOT_FOUND;
        }
		Status = SctCmdLineGetReportName(ImageHandle, Argv[Index]);
		if (EFI_ERROR(Status)) {
          EFI_SCT_DEBUG((EFI_SCT_D_ERROR, L"Get report file name fail - %r", Status));
          return Status;
        }
		break;
      case L'p': case L'P':
        SCT_OPERATION_MASK(EFI_SCT_OPERATIONS_PASSIVEMODE);
        Index++;
        if (Index == Argc) {
          EFI_SCT_DEBUG((EFI_SCT_D_ERROR, L"Not set the monitor name"));
          return EFI_INVALID_PARAMETER;
        } else {
          SctCmdLineGetMonitorName(Argv[Index]);
        }
        break;
      case L'r': case L'R':
        SCT_OPERATION_MASK(EFI_SCT_OPERATIONS_RESET);
        break;
      case L's': case L'S':
        SCT_OPERATION_MASK(EFI_SCT_OPERATIONS_SEQUENCE);
        Index++;
        if (Index == Argc) {
          EFI_SCT_DEBUG((EFI_SCT_D_ERROR, L"Not set the sequence file name"));
          return EFI_NOT_FOUND;
        }
        Status = SctCmdLineGetSequenceFileName(ImageHandle, Argv[Index]);
        if (EFI_ERROR(Status)) {
          EFI_SCT_DEBUG((EFI_SCT_D_ERROR, L"get sequence file name fail - %r", Status));
          return Status;
        }
        break;
      case L'u': case L'U':
	  	SCT_OPERATION_MASK(EFI_SCT_OPERATIONS_UI);
        break;
      case L'x': case L'X':
        SCT_OPERATION_MASK(EFI_SCT_OPERATIONS_EXTENDED);
        break;
      case L'f': case L'F':
        gForceExecution = TRUE;
        break;
      case L'v': case L'V':
      	 SCT_OPERATION_MASK(EFI_SCT_OPERATIONS_VERBOSE);
      	 break;
      case L'h': case L'H': case L'?':
        SCT_OPERATION_MASK(EFI_SCT_OPERATIONS_NONE);
        return EFI_SUCCESS;;
      default:
        return EFI_INVALID_PARAMETER;
      }
    } else {
      return EFI_INVALID_PARAMETER;
    }
  }

  Status = EFI_INVALID_PARAMETER;
  for(Index = 0; SCT_VALID_OPERATIONS[Index] != EFI_SCT_OPERATIONS_INVALID; Index++) {
    if (SCT_OPERATION_EQU(SCT_VALID_OPERATIONS[Index])) {
      Status = EFI_SUCCESS;
      break;
    }
  }

  return Status;
}

EFI_STATUS
SctMainFunc (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
/*++

Routine Description:

  The main routine of SCT.

Returns:

  EFI_SUCCESS   - Successfully.
  OTHERS        - Something failed.

--*/
{
  EFI_STATUS  Status;

  //
  // Check the operations
  //
  if (gFT->Operations == EFI_SCT_OPERATIONS_NONE) {
    //
    // No operation. Print out the help information
    //
    PrintUsage ();
    return EFI_SUCCESS;
  }

  //
  // If EFI_SCT_OPERATIONS_PASSIVEMODE unmasked then delete the passive test context 
  //
  if ((gFT->Operations & EFI_SCT_OPERATIONS_PASSIVEMODE) == 0) {
    PassiveTestContextDelete(gFT->DevicePath, SCT_PASSIVE_MODE_RECORD_FILE);
  }

  //
  // Attach the first stage test data (before the test files are loaded)
  //
  Status = AttachFirstStageTestData ();
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Attach first stage test data - %r", Status));
    return Status;
  }

  //
  // Do the first stage operations (before the test files are loaded)
  //
  Status = DoFirstStageOperations ();
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"First stage operations - %r", Status));
    return Status;
  }

  //
  // Attach the second stage test data (after the test files are loaded)
  //
  Status = AttachSecondStageTestData ();
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Attach second stage test data - %r", Status));
    return Status;
  }

  //
  // Do the second stage operations (after the test files are loaded)
  //
  Status = DoSecondStageOperations ();
  if (EFI_ERROR (Status)) {
    EFI_SCT_DEBUG ((EFI_SCT_D_ERROR, L"Second stage operations - %r", Status));
    return Status;
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CheckMonitorName (
  CHAR16  *Argv
  )
{
//  CHAR16 *Temp;
//  Temp = UpperCaseString(Argv);

  if ((StrCmp(Argv, L"MNP") == 0) || (StrCmp(Argv, L"mnp") == 0))
  	return EFI_SUCCESS;
  if ((StrCmp(Argv, L"IP4") == 0) || (StrCmp(Argv, L"ip4") == 0))
  	return EFI_SUCCESS;
  if ((StrCmp(Argv, L"SERIAL") == 0) || (StrCmp(Argv, L"serial") == 0))
  	return EFI_SUCCESS;  

  return EFI_INVALID_PARAMETER;
}

