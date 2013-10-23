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

  ConfigMenu.c

Abstract:

  Implements the interface to display and handle Config menu

--*/

#include "Sct.h"
#include "UI.h"
#include "BuildMenu.h"

//
// const Definitions
//

#define CONFIG_MENU_TEST_CASE_MAX_RUN_TIME             0
#define CONFIG_MENU_TEST_LEVEL                         1
#define CONFIG_MENU_VERBOSE_LEVEL                      2
#define CONFIG_MENU_ENABLE_SCREEN_OUTPUT               3
#define CONFIG_MENU_LOGGING_FILE_PATH                  4
#define CONFIG_MENU_BIOSID                             5
#define CONFIG_MENU_PLATFORM_NUMBER                    6
#define CONFIG_MENU_CONFIGURATION_NUMBER               7
#define CONFIG_MENU_SCENARIO_STRING                    8
#define CONFIG_MENU_EDIT_COMMAND                       9

BOOLEAN           ItemValueChanged = FALSE;

//
// Prototypes (external)
//

EFI_STATUS
DisplayConfigMenu (
  IN EFI_MENU_PAGE          *ParentPage
  );

EFI_STATUS
ConfigMenuEscFunc (
  IN EFI_MENU_PAGE          *Page
  );

EFI_STATUS
ConfigMenuSetDefaultFunc (
  IN EFI_MENU_PAGE          *Page
  );

EFI_STATUS
ConfigMenuDescPrintFunc (
  IN EFI_MENU_ITEM          *MenuItem,
  IN VOID                   *Context,
  IN UINTN                  Row,
  IN UINTN                  Column
  );

//
// Functions
//

EFI_STATUS
DisplayConfigMenu (
  IN EFI_MENU_PAGE          *ParentPage
  )
/*++

Routine Description:

  Display the Config menu

Arguments:

  ParentPage    - the EFI_MENU_PAGE pointer.

Returns:

  EFI_SUCCESS   - Display the menu successfully
  Other Value   - Something failed

--*/
{
  EFI_STATUS            Status;
  EFI_MENU_PAGE         *Page;
  CHAR16                *EditBuffer;
  EFI_ITEM_VALUE_QUEUE  *TempValueQueue;
  EFI_MENU_ITEM         *MenuItem;

  //
  // Create a standard menu
  //
  Status = CreateStandardMenuPage (
             L"Test Environment Configuration",
             &Page
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Page->Parent = ParentPage;

  //
  // Config Menu Items
  //  -Test Case Max Run Time
  //  -Enable Screen Output
  //
  //  -Bios Id
  //  -Platform Number
  //  -Configuration Number
  //  -Scenario String
  //

  EditBuffer = NULL;
  Status = BS->AllocatePool(
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH+1)*sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // set current value
  //
  SPrint (
    EditBuffer,
    EFI_MAX_EDIT_LENGTH + 1,
    L"%d",
    gFT->ConfigData->TestCaseMaxRunTime
    );

  Status = AddEditMenuItem (
             EFI_ITEM_EDIT_NUMBER,
             L"Test Case Max Run Time",
             L"Sets the maximum run time for one test case, in second (0 means unlimited)",
             EditBuffer,
             (VOID *)CONFIG_MENU_TEST_CASE_MAX_RUN_TIME,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    BS->FreePool (EditBuffer);
    return Status;
  }

  //
  //Add number range for MAX_RUN_TIME
  //
  Status = AddMenuItemCheckValue(
             L"Test Case Max Run Time",
             gTestCaseMaxRunTimeMax,
             TEST_CASE_MAX_RUN_TIME_MIN,
             TEST_CASE_MAX_RUN_TIME_DEFAULT,
             Page
             );

  //
  //create ValueQueue for Enable Screen Output
  //
  TempValueQueue = NULL;
  Status = InsertValueQueue (
             &TempValueQueue,
             L"True",
             1
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return Status;
  }

  Status = InsertValueQueue (
             &TempValueQueue,
             L"False",
             0
             );
  if (EFI_ERROR (Status)) {
    DestroyValueQueue (TempValueQueue);
    DestroyMenuPage (Page);
    return Status;
  }

  if (( gFT->ConfigData->EnableScreenOutput && (TempValueQueue->IntValue == 0)) ||
      (!gFT->ConfigData->EnableScreenOutput && (TempValueQueue->IntValue == 1))) {
    TempValueQueue = TempValueQueue ->Next;
  }

  //
  //add Enable Screen Output Menu Item
  //
  Status = AddComboListMenuItem (
             L"Enable Screen Output",
             L"Sets to display the test log information on the screen or not",
             TempValueQueue,
             (VOID*)(UINTN)CONFIG_MENU_ENABLE_SCREEN_OUTPUT,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyValueQueue (TempValueQueue);
    DestroyMenuPage (Page);
    return Status;
  }

  TempValueQueue = NULL;

  //
  //add a separate line
  //
  Status = AddSeparateLine (1, Page);
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return Status;
  }

  //
  //add BiosId Menu Item
  //
  EditBuffer = NULL;
  Status = BS->AllocatePool (
            EfiBootServicesData,
            (EFI_MAX_EDIT_LENGTH + 1) * sizeof(CHAR16),
            (VOID **)&EditBuffer
            );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return EFI_OUT_OF_RESOURCES;
  }
  if (gFT->ConfigData->BiosId != NULL) {
    SPrint (EditBuffer, EFI_MAX_EDIT_LENGTH + 1, L"%s", gFT->ConfigData->BiosId);
  } else {
    EditBuffer[0] = L'\0';
  }

  Status = AddEditMenuItem (
             EFI_ITEM_EDIT_STRING,
             L"Bios Id",
             L"Allows user to set a string to identify the software under test",
             EditBuffer,
             (VOID *)(UINTN)CONFIG_MENU_BIOSID,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    BS->FreePool (EditBuffer);
    return Status;
  }

  //
  //add Platform Number Menu Item
  //
  EditBuffer  = NULL;
  Status = BS->AllocatePool (
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH + 1) * sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return EFI_OUT_OF_RESOURCES;
  }
  SPrint(EditBuffer,EFI_MAX_EDIT_LENGTH+1,L"%d",gFT->ConfigData->PlatformNumber);

  Status = AddEditMenuItem (
             EFI_ITEM_EDIT_NUMBER,
             L"Platform Number",
             L"Allow user to set a number to identify the platform under test, and as a marker for different platforms",
             EditBuffer,
             (VOID *)(UINTN)CONFIG_MENU_PLATFORM_NUMBER,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    BS->FreePool (EditBuffer);
    return Status;
  }

  //
  //Add number range for PLATFORM_NUMBER
  //
  Status = AddMenuItemCheckValue(
             L"Platform Number",
             PLATFORM_NUMBER_MAX,
             PLATFORM_NUMBER_MIN,
             PLATFORM_NUMBER_DEFAULT,
             Page
             );

  //
  //add Configuration Number Menu Item
  //
  EditBuffer = NULL;
  Status = BS->AllocatePool (
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH + 1) * sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return EFI_OUT_OF_RESOURCES;
  }
  SPrint (
    EditBuffer,
    EFI_MAX_EDIT_LENGTH + 1,
    L"%d",
    gFT->ConfigData->ConfigurationNumber
    );

  Status = AddEditMenuItem (
             EFI_ITEM_EDIT_NUMBER,
             L"Configuration Number",
             L"Allows user to set a number to identify the configuration under test, and as a marker for different configurations",
             EditBuffer,
             (VOID *)(UINTN)CONFIG_MENU_CONFIGURATION_NUMBER,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    BS->FreePool (EditBuffer);
    return Status;
  }

  //
  //Add number range for CONFIGURATION_NUMBER
  //
  Status = AddMenuItemCheckValue(
             L"Configuration Number",
             CONFIGURATION_NUMBER_MAX,
             CONFIGURATION_NUMBER_MIN,
             CONFIGURATION_NUMBER_DEFAULT,
             Page
             );

  //
  //add Scenario String Menu Item
  //
  EditBuffer  = NULL;
  Status = BS->AllocatePool (
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH + 1) * sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return EFI_OUT_OF_RESOURCES;
  }
  if (gFT->ConfigData->ScenarioString != NULL) {
    SPrint (EditBuffer, EFI_MAX_EDIT_LENGTH + 1, L"%s", gFT->ConfigData->ScenarioString);
  } else {
    EditBuffer[0] = L'\0';
  }

  Status = AddEditMenuItem (
             EFI_ITEM_EDIT_STRING,
             L"Scenario String",
             L"Allows user to set a string to identify the scenario under test",
             EditBuffer,
             (VOID *)(UINTN)CONFIG_MENU_SCENARIO_STRING,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    BS->FreePool (EditBuffer);
    return Status;
  }

  //
  //add edit command String Menu Item
  //
  EditBuffer  = NULL;
  Status = BS->AllocatePool (
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH + 1) * sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return EFI_OUT_OF_RESOURCES;
  }

  if (gFT->ConfigData->EditCommandString != NULL) {
    SPrint (EditBuffer, EFI_MAX_EDIT_LENGTH + 1, L"%s", gFT->ConfigData->EditCommandString);
  } else {
    EditBuffer[0] = L'\0';
  }

  Status = AddEditMenuItem (
             EFI_ITEM_EDIT_STRING,
             L"Edit Command",
             L"Allows user to set a file for edit command",
             EditBuffer,
             (VOID *)(UINTN)CONFIG_MENU_EDIT_COMMAND,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    BS->FreePool (EditBuffer);
    return Status;
  }

  MenuItem = Page->Body.ItemList;

  //
  //set MenuItem DescPrint function.
  //
  while (MenuItem != NULL) {
    MenuItem->DescPrint = ConfigMenuDescPrintFunc;
    MenuItem = MenuItem -> Next;
  }

  //
  // Config Menu Hot Keys
  //  - Up, Down, Left, Right, Enter, Esc
  //
  Status = AddHotKey (
             L"Up/Dn",
             L"Select Item",
             SCAN_UP,
             CHAR_NULL,
             UpFunc,
             TRUE,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return Status;
  }

  Status = AddHotKey (
             L"Down",
             L"Select Item",
             SCAN_DOWN,
             CHAR_NULL,
             DownFunc,
             FALSE,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return Status;
  }

  Status = AddHotKey (
             L"Lft/Rt",
             L"Change Value",
             SCAN_LEFT,
             CHAR_NULL,
             LeftArrowFunc,
             FALSE,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return Status;
  }

  Status = AddHotKey (
             L"Right",
             L"Change Value",
             SCAN_RIGHT,
             CHAR_NULL,
             RightArrowFunc,
             FALSE,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return Status;
  }

  Status = AddHotKey (
             L"F3",
             L"Set to Default",
             SCAN_F3,
             CHAR_NULL,
             ConfigMenuSetDefaultFunc,
             TRUE,
             Page
             );
  if (EFI_ERROR(Status)) {
    DestroyMenuPage (Page);
    return Status;
  }


  Status = AddHotKey (
             L"ESC",
             L"Save & Exit",
             SCAN_ESC,
             CHAR_NULL,
             ConfigMenuEscFunc,
             TRUE,
             Page
             );
  if (EFI_ERROR (Status)) {
    DestroyMenuPage (Page);
    return Status;
  }
  ItemValueChanged = FALSE;

  //
  // Display the Config menu
  //
  MenuPageRefresh (Page);

  return EFI_SUCCESS;
}

EFI_STATUS
ConfigMenuSetDefaultFunc (
  IN EFI_MENU_PAGE          *MenuPage
  )
/*++

Routine Description:

  Handle F3 key in Config menu to set default value

Arguments:

  MenuPage      - A pointer to the menu

Returns:

  EFI_SUCCESS   - Handle ESC key successfully
  Other Value   - Something failed

--*/
{
  EFI_MENU_ITEM         *MenuItem;
  CHAR16                *EditBuffer;
  EFI_STATUS             Status;
  EFI_ITEM_VALUE_QUEUE  *ValueQueue;


  EditBuffer = NULL;

  MenuItem = MenuPage->Body.ItemList;

  while(MenuItem != NULL) {
    switch ((UINTN)MenuItem->Context) {
      case CONFIG_MENU_TEST_CASE_MAX_RUN_TIME:
        //
        // convert default value to string
        //
        Status = BS->AllocatePool(
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH+1)*sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
        if (EFI_ERROR (Status)) {
          DestroyMenuPage (MenuPage);
          return EFI_OUT_OF_RESOURCES;
        }
        SPrint (
          EditBuffer,
          EFI_MAX_EDIT_LENGTH + 1,
          L"%d",
          TEST_CASE_MAX_RUN_TIME_DEFAULT
          );

        //
        //copy default value to menu item
        //
        StrCpy (
          MenuItem ->ItemValue,
          EditBuffer
          );
        break;

      case CONFIG_MENU_ENABLE_SCREEN_OUTPUT:
        //
        // convert default value to string
        //
        Status = BS->AllocatePool(
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH+1)*sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
        if (EFI_ERROR (Status)) {
          DestroyMenuPage (MenuPage);
          return EFI_OUT_OF_RESOURCES;
        }

        if (ENABLE_SCREEN_OUTPUT_DEFAULT == TRUE) {
          SPrint (
            EditBuffer,
            EFI_MAX_EDIT_LENGTH + 1,
            L"True"
            );
        } else {
          SPrint (
            EditBuffer,
            EFI_MAX_EDIT_LENGTH + 1,
            L"False"
            );
        }
        if (MenuItem->ValueQueue != NULL) {
          ValueQueue = NULL;
          ValueQueue = MenuItem->ValueQueue;
          //
          //found current pointed value queue
          //
          if (ValueQueue->Next!=ValueQueue ) {
            while (StriCmp (ValueQueue->StringValue, EditBuffer) != 0) {
              ValueQueue = ValueQueue->Next;
            }
            //
            //reset the itemvalue
            //
            MenuItem->ItemValue = ValueQueue->StringValue;
          }
        }
        break;

      case CONFIG_MENU_BIOSID:
        //
        // convert default value to string
        //
        Status = BS->AllocatePool(
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH+1)*sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
        if (EFI_ERROR (Status)) {
          DestroyMenuPage (MenuPage);
          return EFI_OUT_OF_RESOURCES;
        }
        SPrint (
          EditBuffer,
          EFI_MAX_EDIT_LENGTH + 1,
          L"%s",
          BIOS_ID_DEFAULT
          );

        //
        //copy default value to menu item
        //
        StrCpy (
          MenuItem->ItemValue,
          EditBuffer
          );
        break;

      case CONFIG_MENU_PLATFORM_NUMBER:
        //
        // convert default value to string
        //
        Status = BS->AllocatePool(
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH+1)*sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
        if (EFI_ERROR (Status)) {
          DestroyMenuPage (MenuPage);
          return EFI_OUT_OF_RESOURCES;
        }
        SPrint (
          EditBuffer,
          EFI_MAX_EDIT_LENGTH + 1,
          L"%d",
          PLATFORM_NUMBER_DEFAULT
          );
        StrCpy (
          MenuItem->ItemValue,
          EditBuffer
          );
        break;

      case CONFIG_MENU_CONFIGURATION_NUMBER:
        //
        // convert default value to string
        //
        Status = BS->AllocatePool(
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH+1)*sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
        if (EFI_ERROR (Status)) {
          DestroyMenuPage (MenuPage);
          return EFI_OUT_OF_RESOURCES;
        }
        SPrint (
          EditBuffer,
          EFI_MAX_EDIT_LENGTH + 1,
          L"%d",
          CONFIGURATION_NUMBER_DEFAULT
          );
        StrCpy (
          MenuItem->ItemValue,
          EditBuffer
          );
        break;

      case CONFIG_MENU_SCENARIO_STRING:
        //
        // convert default value to string
        //
        Status = BS->AllocatePool(
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH+1)*sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
        if (EFI_ERROR (Status)) {
          DestroyMenuPage (MenuPage);
          return EFI_OUT_OF_RESOURCES;
        }
        SPrint (
          EditBuffer,
          EFI_MAX_EDIT_LENGTH + 1,
          L"%s",
          SCENARIO_STRING_DEFAULT
          );
        StrCpy (
          MenuItem->ItemValue,
          EditBuffer
          );
        break;

      case CONFIG_MENU_EDIT_COMMAND:
        //
        // convert default value to string
        //
        Status = BS->AllocatePool(
                 EfiBootServicesData,
                 (EFI_MAX_EDIT_LENGTH+1)*sizeof(CHAR16),
                 (VOID **)&EditBuffer
                 );
        if (EFI_ERROR (Status)) {
          DestroyMenuPage (MenuPage);
          return EFI_OUT_OF_RESOURCES;
        }
        SPrint (
          EditBuffer,
          EFI_MAX_EDIT_LENGTH + 1,
          L"%s",
          EDIT_COMMAND_DEFAULT
          );
        StrCpy (
          MenuItem->ItemValue,
          EditBuffer
          );
        break;

      default:
        break;
    }
    MenuItem = MenuItem->Next;
  }

  MenuPageRefresh (MenuPage);

  return EFI_SUCCESS;
}

EFI_STATUS
ConfigMenuEscFunc (
  IN EFI_MENU_PAGE          *Page
  )
/*++

Routine Description:

  Handle ESC key in Config menu

Arguments:

  Page          - A pointer to the menu

Returns:

  EFI_SUCCESS   - Handle ESC key successfully
  Other Value   - Something failed

--*/
{
  EFI_STATUS              Status;
  EFI_MENU_ITEM           *MenuItem;
  CHAR16                  *FileName;
  UINTN                   TempValue;
  BOOLEAN                 TrueOrFalse;

  //
  //save current value into config data
  //

  MenuItem = Page->Body.ItemList;
  while(MenuItem != NULL) {
    switch ((UINTN)MenuItem->Context) {
      case CONFIG_MENU_TEST_CASE_MAX_RUN_TIME:
        TempValue = Atoi(MenuItem->ItemValue);
        if (TempValue == (UINTN) -1) {
          TempValue = 0;
        }
        if (TempValue !=  gFT->ConfigData->TestCaseMaxRunTime) {
          gFT->ConfigData->TestCaseMaxRunTime = TempValue;
          ItemValueChanged = TRUE;
        }
        break;

      case CONFIG_MENU_ENABLE_SCREEN_OUTPUT:
        if (StriCmp (MenuItem->ItemValue, L"True") == 0) {
          TrueOrFalse = TRUE;
        } else {
          TrueOrFalse = FALSE;
        }
        if (gFT->ConfigData->EnableScreenOutput != TrueOrFalse) {
          gFT->ConfigData->EnableScreenOutput = TrueOrFalse;
          ItemValueChanged = TRUE;
        }
        break;

      case CONFIG_MENU_BIOSID:
        if (StriCmp (gFT->ConfigData->BiosId, MenuItem->ItemValue) != 0) {
          if (gFT->ConfigData->BiosId != NULL) {
            BS->FreePool (gFT->ConfigData->BiosId);
          }
          gFT->ConfigData->BiosId = StrDuplicate (MenuItem->ItemValue);
          ItemValueChanged = TRUE;
        }
        break;

      case CONFIG_MENU_PLATFORM_NUMBER:
        TempValue = Atoi (MenuItem->ItemValue);
        if (TempValue == (UINTN) -1) {
          TempValue = 0;
        }
        if (TempValue != gFT->ConfigData->PlatformNumber) {
          gFT->ConfigData->PlatformNumber = TempValue;
          ItemValueChanged = TRUE;
        }
        break;

      case CONFIG_MENU_CONFIGURATION_NUMBER:
        TempValue = Atoi (MenuItem->ItemValue);
        if (TempValue == (UINTN) -1) {
          TempValue = 0;
        }
        if (gFT->ConfigData->ConfigurationNumber != TempValue) {
          gFT->ConfigData->ConfigurationNumber = TempValue;
          ItemValueChanged = TRUE;
        }
        break;

      case CONFIG_MENU_SCENARIO_STRING:
        if (StriCmp (gFT->ConfigData->ScenarioString, MenuItem->ItemValue) != 0) {
          if (gFT->ConfigData->ScenarioString != NULL) {
            BS->FreePool (gFT->ConfigData->ScenarioString);
          }
          gFT->ConfigData->ScenarioString = StrDuplicate (MenuItem->ItemValue);
          ItemValueChanged = TRUE;
        }
        break;

      case CONFIG_MENU_EDIT_COMMAND:
        if (StriCmp (gFT->ConfigData->EditCommandString, MenuItem->ItemValue) != 0) {
          if (gFT->ConfigData->EditCommandString != NULL) {
            BS->FreePool (gFT->ConfigData->EditCommandString);
          }
          gFT->ConfigData->EditCommandString = StrDuplicate (MenuItem->ItemValue);
          ItemValueChanged = TRUE;
        }
        break;

      default:
        break;
    }
    MenuItem = MenuItem->Next;
  }
  if (ItemValueChanged) {
    FileName = PoolPrint (L"%s\\%s",gFT->FilePath,EFI_SCT_FILE_CONFIG);

    //
    //save config data into profile
    //
    Status = SaveConfigData (
               gFT->DevicePath,
               FileName,
               gFT->ConfigData
               );
    BS->FreePool (FileName);
  }
  //
  // Display the parent menu page
  //
  MenuPageRefresh (Page->Parent);

  //
  // Destroy the Config menu page
  //
  DestroyMenuPage (Page);

  return EFI_SUCCESS;
}

EFI_STATUS
ConfigMenuDescPrintFunc (
  IN EFI_MENU_ITEM          *MenuItem,
  IN VOID                   *Context,
  IN UINTN                  Row,
  IN UINTN                  Column
  )
/*++

Routine Description:

  the  MenuItem description information displaying function.

Arguments:

  MenuItem    - The EFI_MENU_ITEM instance pointer.
  Context     - the EFI_MENU_PAGE instance pointer, the MenuItem belongs
                to this MenuPage.
  Row         - The beginning Row on Output Console to display the
                description message.
  Column      - The beginning Column on Output Console to display the
                description message.

Returns:

  EFI_SUCCESS - the description was displayed successfully.
  other       - error condition.

--*/
{
  UINTN           X0;
  UINTN           X1;
  UINTN           Y0;
  UINTN           Y1;
  UINTN           Index;
  UINTN           BufLen;
  UINTN           Attrib;
  UINTN           MaxValue;
  UINTN           MinValue;
  EFI_STATUS      Status;
  EFI_MENU_PAGE   *Page;
  CHAR16          *Buffer;

  if (Context == NULL || MenuItem == NULL ) {
    return EFI_INVALID_PARAMETER;
  }

  BufLen  = 0;
  Page    = (EFI_MENU_PAGE *)Context;
  Attrib  = ST->ConOut->Mode->Attribute;
  X0      = Column;
  Y0      = Row;
  X1      = Page->Body.BodyRect.BottomRight.Col - 1;
  Y1      = Page->Body.BodyRect.BottomRight.Row - 1;

  if (X0 > X1 || Y0 > Y1) {
    return EFI_INVALID_PARAMETER;
  }


  if (MenuItem == Page->Body.CurrentSelected) {
    Status = ST->ConOut->SetAttribute (
                           ST->ConOut,
                           EFI_ITEM_DESC_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                           );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //
    //First we display the default description information given by the menu
    //item directly
    //

    Buffer = MenuItem->ItemDesc.Text;
    if (Buffer != NULL && StrLen (Buffer) > 0 ) {
      //
      //Display the default description.
      //
      if (Y1 >= Y0 + 4) {
        Status = DisplayMessage(X0, Y0, X1, Y1 - 4, Buffer);
      }
    }

    //
    //then we display specific information for this menu item
    //
    Status = BS->AllocatePool (
                   EfiBootServicesData,
                   MAX_STRING_LEN * sizeof (CHAR16),
                   (VOID **)&Buffer
                   );
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }

    //
    //first clear this area (the last 3 lines).
    //
    BufLen = (X1 - X0 + 2) * sizeof (CHAR16);

    for (Index = 0; Index < BufLen / 2 - 1; Index++) {
      Buffer[Index] = L' ';
    }
    Buffer[Index] = L'\0';

    for (Index = Y1 - 3; Index <= Y1; Index++) {
      Status = TestPrintAt (X0, Index, Buffer);
      if (EFI_ERROR (Status)) {
        BS->FreePool (Buffer);
        return Status;
      }
    }

    //
    //If current menu item is combolist, its value is changed by <Right> and <Left> key
    //
    if (MenuItem->ItemType == EFI_ITEM_HAVE_COMBOLIST) {
      Buffer[0] = L'\0';
      StrCat (Buffer, L"Please change value by press <Right> or <Left> key.");
      Status = DisplayMessage(X0, Y1 - 3, X1, Y1, Buffer);
    }
    if ((MenuItem->ItemType & EFI_EDIT_SUB_TYPE_MASK) == EFI_ITEM_EDIT_NUMBER
      && MenuItem->CheckValue == TRUE) {

      //
      //display Value range.
      //
      Buffer[0] = L'\0';
      StrCat (Buffer, L"Value Range:");

      Status = TestPrintAt (X0, Y1 - 3, Buffer);

      MaxValue = MenuItem->MaxValue;

      Buffer[0] = L'\0';
      StrCat (Buffer, L"Max:");
      SPrint (Buffer, MAX_STRING_LEN, L"%s   %d", Buffer, MaxValue);

      Status = TestPrintAt (X0, Y1 - 2, Buffer);

      MinValue = MenuItem->MinValue;;

      Buffer[0] = L'\0';
      StrCat (Buffer, L"Min:");
      SPrint (Buffer, MAX_STRING_LEN, L"%s   %d", Buffer, MinValue);

      Status = TestPrintAt (X0, Y1 - 1, Buffer);
    }
    BS->FreePool (Buffer);
    ST->ConOut->SetAttribute (ST->ConOut, Attrib);

    //
    //successfully displayed the message
    //
    return EFI_SUCCESS;

  }  else {

    //
    //clear the display area.
    //
    Status = ST->ConOut->SetAttribute (
                           ST->ConOut,
                           EFI_MENUPAGE_BODY_BGCOLOR >> 4 | EFI_MENUPAGE_BODY_BGCOLOR
                           );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    BufLen = (X1 - X0 + 2) * sizeof(CHAR16);
    Status = BS->AllocatePool (
                   EfiBootServicesData,
                   BufLen,
                   (VOID **)&Buffer
                   );
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }
    for (Index = 0; Index < BufLen / 2 - 1; Index++) {
      Buffer[Index] = L' ';
    }
    Buffer[Index] = L'\0';

    for (Index = Y0; Index <= Y1; Index ++) {
      Status = TestPrintAt (X0, Index, Buffer);
      if (EFI_ERROR (Status)) {
        BS->FreePool (Buffer);
        return Status;
      }
    }

    BS->FreePool (Buffer);
    ST->ConOut->SetAttribute (ST->ConOut, Attrib);
    return EFI_SUCCESS;
  }
}
