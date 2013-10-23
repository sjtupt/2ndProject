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

  EventTimerTaskPriorityServicesBBTestSignalEvent.c

Abstract:

  Test Driver of SignalEvent() of Event and Timer and Task Priority Services

--*/

#include "EventTimerTaskPriorityServicesBBTestMain.h"

//
// Declarations
//

EFI_STATUS
BBTestSignalEvent_Func_Sub1 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib
  );

//
// Functions
//

EFI_STATUS
BBTestSignalEvent_Func (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STATUS                          Status;
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL  *StandardLib;

  //
  // Locate standard test library
  //
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   (VOID **)&StandardLib
                   );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // SignalEvent() returns EFI_SUCCESS with all valid parameters.
  //
  BBTestSignalEvent_Func_Sub1 (StandardLib);

  //
  // Done
  //
  return EFI_SUCCESS;
}


//
// Sub functions
//

EFI_STATUS
BBTestSignalEvent_Func_Sub1 (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib
  )
{
  EFI_STATUS          Status;
  EFI_STATUS          Status1;
  EFI_STATUS          Status2;
  UINTN               Index;
  UINTN               SubIndex;
  EFI_TPL             OldTpl;
  UINT32              EventTypes[] = {
                        EFI_EVENT_NOTIFY_SIGNAL,
                        0
                      };
  EFI_TPL             NotifyTpls[] = {
                        EFI_TPL_CALLBACK,
                        EFI_TPL_NOTIFY,
                        0
                      };
  EFI_TEST_ASSERTION  AssertionType;
  EFI_EVENT           Event;
  UINTN               Buffer[2];

  //
  // Walk through common event types and notify TPLs
  //
  for (Index = 0; EventTypes[Index] != 0; Index++) {
    for (SubIndex = 0; NotifyTpls[SubIndex] != 0; SubIndex++) {
      //
      // Create event with valid parameters
      //
      Buffer[0] = 0;
      Buffer[1] = 0;
      Status = gtBS->CreateEvent (
                       EventTypes[Index],
                       NotifyTpls[SubIndex],
                       NotifyFunction,
                       Buffer,
                       &Event
                       );
      if (EFI_ERROR (Status)) {
        EFI_TEST_GENERIC_FAILURE (L"BS.SignalEvent - Create event", Status);
        return Status;
      }

      //
      // Signal event in the EFI_TPL_CALLBACK
      //
      OldTpl  = gtBS->RaiseTPL (EFI_TPL_CALLBACK);
      Status1 = gtBS->SignalEvent (Event);
      Status2 = gtBS->SignalEvent (Event);
      gtBS->RestoreTPL (OldTpl);

      if ((Status1   == EFI_SUCCESS) &&
          (Status2   == EFI_SUCCESS) &&
          (((NotifyTpls[SubIndex] >  EFI_TPL_CALLBACK) && (Buffer[1] == 2)) ||
           ((NotifyTpls[SubIndex] <= EFI_TPL_CALLBACK) && (Buffer[1] == 1))
          )) {
        AssertionType = EFI_TEST_ASSERTION_PASSED;
      } else {
        AssertionType = EFI_TEST_ASSERTION_FAILED;
      }

      StandardLib->RecordAssertion (
                     StandardLib,
                     AssertionType,
                     gEventTimerTaskPriorityServicesBBTestSignalEventAssertionGuid001,
                     L"BS.SignalEvent - Signal event with valid parameters.",
                     L"%a:%d:Status1 - %r, Status2 - %r, EventType - 0x%08x, NotifyTpl - %d, Count - %d",
                     __FILE__,
                     (UINTN)__LINE__,
                     Status1,
                     Status2,
                     EventTypes[Index],
                     NotifyTpls[SubIndex],
                     Buffer[1]
                     );

      //
      // Close the event
      //
      gtBS->CloseEvent (Event);
    }
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}
