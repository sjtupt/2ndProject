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
#ifndef _HII_CONFIG_ACCESS_2_PROTOCOL_H_
#define _HII_CONFIG_ACCESS_2_PROTOCOL_H_

#include "EfiHii.h"

#define EFI_BROWSER_ACTION_REQUEST_NONE    0
#define EFI_BROWSER_ACTION_REQUEST_RESET   1
#define EFI_BROWSER_ACTION_REQUEST_SUBMIT  2
#define EFI_BROWSER_ACTION_REQUEST_EXIT    3

#define EFI_BROWSER_ACTION_REQUEST_CHANGING  0
#define EFI_BROWSER_ACTION_REQUEST_CHANGED   1

//typedef CHAR16  *EFI_STRING;
//typedef UINT16  EFI_STRING_ID;

typedef UINTN   EFI_BROWSER_ACTION;
//typedef UINT16  EFI_QUESTION_ID;
typedef UINTN   EFI_BROWSER_ACTION_REQUEST;

/*
typedef struct {
  UINT8 Hour;
  UINT8 Minute;
  UINT8 Second;
} EFI_HII_TIME;

typedef struct {
  UINT16 Year;
  UINT8  Month;
  UINT8  Day;
} EFI_HII_DATE;

typedef union {
  UINT8           u8;
  UINT16          u16;
  UINT32          u32;
  UINT64          u64;
  BOOLEAN         b;
  EFI_HII_TIME    time;
  EFI_HII_DATE    date;
  EFI_STRING_ID   string;
} EFI_IFR_TYPE_VALUE;
*/
  
EFI_FORWARD_DECLARATION (EFI_HII_CONFIG_ACCESS_PROTOCOL);

typedef
EFI_STATUS
(EFIAPI *EFI_HII_ACCESS_EXTRACT_CONFIG ) (
IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
IN CONST EFI_STRING                        Request,
OUT EFI_STRING                             *Progress,
OUT EFI_STRING                             *Results
);


typedef
EFI_STATUS
(EFIAPI *EFI_HII_ACCESS_ROUTE_CONFIG ) (
IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
IN CONST EFI_STRING                        Configuration,
OUT EFI_STRING                             *Progress
);


typedef
EFI_STATUS
(EFIAPI *EFI_HII_ACCESS_FORM_CALLBACK) (
IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
IN EFI_BROWSER_ACTION                      *Action,
IN EFI_QUESTION_ID                         QuestionId,
IN UINT8                                   Type,
IN EFI_IFR_TYPE_VALUE                      *Value,
OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
);

//
// Interface structure for the HII Configuration Access Protocol
//

struct  _EFI_HII_CONFIG_ACCESS_PROTOCOL {
EFI_HII_ACCESS_EXTRACT_CONFIG  ExtractConfig;
EFI_HII_ACCESS_ROUTE_CONFIG    RouteConfig;
EFI_HII_ACCESS_FORM_CALLBACK   CallBack;
};

#endif
