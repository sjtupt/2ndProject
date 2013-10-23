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

  KeyFunction.c

Abstract:

  Test Framework Basic User Interface Key Proecess Function.

--*/

//
//include file.
//
#include "Sct.h"
#include "UI.h"
#include "Dialog.h"

//
//global variable.
//

extern EFI_MENU_PAGE    *gMenuPage;
extern BOOLEAN          gMenuExit;

//
//protypes (external)
//
EFI_STATUS
DoDefault (
  IN EFI_MENU_PAGE        *MenuPage,
  IN EFI_INPUT_KEY        *InKey
  );

EFI_STATUS
UpFunc (
  IN EFI_MENU_PAGE        *MenuPage
  );

EFI_STATUS
DownFunc (
  IN EFI_MENU_PAGE        *MenuPage
  );

EFI_STATUS
DoEdit (
  IN EFI_MENU_PAGE        *MenuPage,
  IN OUT EFI_INPUT_KEY    *InKey
  );

EFI_STATUS
DoIterEdit (
  IN EFI_MENU_PAGE        *MenuPage,
  IN OUT EFI_INPUT_KEY    *InKey
  );

EFI_STATUS
SpaceFunc (
  IN EFI_MENU_PAGE        *MenuPage
  );

EFI_STATUS
LeftArrowFunc (
  IN EFI_MENU_PAGE        *MenuPage
  );

EFI_STATUS
RightArrowFunc (
  IN EFI_MENU_PAGE        *MenuPage
  );

EFI_STATUS
MenuPageKeyInput (
  VOID
  );

EFI_STATUS
DisplayMessage (
  IN UINTN                X0,
  IN UINTN                Y0,
  IN UINTN                X1,
  IN UINTN                Y1,
  IN CHAR16               *Str
  );

EFI_STATUS
DefaultDescPrint (
  IN EFI_MENU_ITEM        *MenuItem,
  IN VOID                 *Context,
  IN UINTN                Row,
  IN UINTN                Column
  );

UINT32
GetTestCaseIterations (
  IN EFI_GUID             *Guid
  );

INT32
CompareNumberString (
  IN CHAR16               *str1,
  IN CHAR16               *str2
  );

//
// functions
//

EFI_STATUS
DoDefault (
  IN EFI_MENU_PAGE        *MenuPage,
  IN EFI_INPUT_KEY        *InKey
  )
/*++

Routine Description:

  the default key process for one menupage key events.

Arguments:

  MenuPage    - which menu page to be managed.
  InKey       - the input key to be processed.

Returns:

  EFI_SUCCESS - the input key was processed successfully.

--*/
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;
  //
  //here we give the default key process.
  //
  return Status;
}

EFI_STATUS
UpFunc (
  IN EFI_MENU_PAGE        *MenuPage
  )
/*++

Routine Description:

  the Up Arrow process function, based on if we need scroll menuitem
  it called MenuBodyRefresh or MenuItemRefresh respectively.

Arguments:

  MenuPage    - which menu page the UP key was taking effect on.

Returns:

  EFI_SUCCESS           - the UP key was responsed successfully.
  EFI_INVALID_PARAMETER

--*/
{
  EFI_STATUS            Status;
  EFI_MENU_ITEM         *MenuItem;
  UINTN                 TotalDisplayLine;
  UINTN                 Index;

  //
  // if there was no item to display return EFI_SUCCESS.
  //
  if (MenuPage == NULL || MenuPage->Body.ItemList == NULL ||
      MenuPage->Body.CurrentSelected == NULL) {
    return EFI_SUCCESS;
  }

  if (MenuPage->Body.CurrentSelected != MenuPage->Body.FirstInDisplay) {
    MenuPage->Body.CurrentSelected = MenuPage->Body.CurrentSelected->Prev;
    //
    //refresh these two items.
    //
    Status = MenuItemRefresh(
               MenuPage,
               MenuPage->Body.CurrentSelected->Next,
               TRUE
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = MenuItemRefresh (
               MenuPage,
               MenuPage->Body.CurrentSelected,
               TRUE
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    return EFI_SUCCESS;
  }

  if (MenuPage->Body.CurrentSelected->Prev == NULL) {
    return EFI_SUCCESS;
  }

  //
  //now should scroll screent to display the previous item.
  //
  TotalDisplayLine = MenuPage->Body.BodyRect.BottomRight.Row - MenuPage->Body.SplitPoint.Row - 1;
  MenuPage->Body.FirstInDisplay  = MenuPage->Body.FirstInDisplay->Prev;
  MenuPage->Body.CurrentSelected = MenuPage->Body.CurrentSelected->Prev;

  //
  //caculate the LastInDisplay  Menu Item position
  //
  MenuItem = MenuPage->Body.FirstInDisplay;
  Index    = 0;
  Index   += (MenuItem->Skip + 1);
  while (MenuItem != NULL && Index < TotalDisplayLine) {
    MenuItem = MenuItem->Next;
    Index   += (MenuItem->Skip + 1);
  }
  MenuPage->Body.LastInDisplay = MenuItem;
  //
  //refresh the menu items
  //
  Status = MenuBodyItemsRefresh (MenuPage);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //done successfully return EFI_SUCCESS
  //
  return EFI_SUCCESS;
}

EFI_STATUS
DownFunc (
  IN EFI_MENU_PAGE        *MenuPage
  )
/*++

Routine Description:

  the Down Arrow process function, based on if we need scroll menuitem
  it called menubodyrefresh or menuitemrefresh respectively.

Arguments:

  MenuPage    - which menu page the DOWN key was taking effect on.

Returns:

  EFI_SUCCESS           - the DOWN key was resonsed successfully.
  EFI_INVALID_PARAMETER

--*/
{
  EFI_STATUS            Status;
  EFI_MENU_ITEM         *MenuItem;
  UINTN                 ScrolledLine;
  UINTN                 LastItemLine;
  UINTN                 TotalDisplayLine;
  UINTN                 Index;

  //
  // if there was no item to display return EFI_SUCCESS.
  //
  if (MenuPage == NULL || MenuPage->Body.ItemList == NULL ||
      MenuPage->Body.CurrentSelected == NULL) {
    return EFI_SUCCESS;
  }
  //
  //initialize varibles
  //
  Status   = EFI_SUCCESS;
  MenuItem = MenuPage->Body.FirstInDisplay;

  if (MenuPage->Body.CurrentSelected != MenuPage->Body.LastInDisplay) {
    MenuPage->Body.CurrentSelected = MenuPage->Body.CurrentSelected->Next;
    //
    //refresh these two items.
    //
    Status = MenuItemRefresh(
               MenuPage,
               MenuPage->Body.CurrentSelected->Prev,
               TRUE
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = MenuItemRefresh (
               MenuPage,
               MenuPage->Body.CurrentSelected,
               TRUE
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    return EFI_SUCCESS;
  }

  if (MenuPage->Body.CurrentSelected->Next == NULL) {
    return EFI_SUCCESS;
  }

  //
  //we should scroll screen to display the next item
  //
  TotalDisplayLine = MenuPage->Body.BodyRect.BottomRight.Row - MenuPage->Body.SplitPoint.Row - 1;
  LastItemLine     = 0;
  MenuItem         = MenuPage->Body.FirstInDisplay;
  while (MenuItem != NULL && MenuItem != MenuPage->Body.LastInDisplay) {
    LastItemLine++;
    LastItemLine += MenuItem->Skip;
    MenuItem = MenuItem->Next;
  }
  LastItemLine++;

  ScrolledLine  = 1 + MenuPage->Body.LastInDisplay->Skip;
  ScrolledLine -= (TotalDisplayLine - LastItemLine);

  MenuItem      = MenuPage->Body.FirstInDisplay;
  Index         = MenuItem->Skip + 1;
  MenuItem      = MenuItem->Next;
  while (Index < ScrolledLine && MenuItem != NULL) {
    Index  += MenuItem->Skip + 1;
    MenuItem  = MenuItem->Next;
  }
  MenuPage->Body.FirstInDisplay = MenuItem;

  //
  //caculate the last menu item in display after scrolling
  //
  MenuItem = MenuPage->Body.FirstInDisplay;
  Index    = 0;
  Index   += (MenuItem->Skip + 1);
  while (MenuItem != NULL && Index < TotalDisplayLine) {
    MenuItem   = MenuItem->Next;
    Index += (MenuItem->Skip + 1);
  }
  MenuPage->Body.LastInDisplay   = MenuItem;
  MenuPage->Body.CurrentSelected = MenuPage->Body.CurrentSelected->Next;
  //
  //refresh all menu items
  //
  Status = MenuBodyItemsRefresh (MenuPage);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //done successfully return EFI_SUCCESS
  //
  return EFI_SUCCESS;
}

EFI_STATUS
PageUpFunc (
  IN EFI_MENU_PAGE        *MenuPage
  )
/*++

Routine Description:

  the Page Up process function, based on if we need scroll menuitem
  it called menubodyrefresh or menuitemrefresh respectively.

Arguments:

  MenuPage    - which menu page the Page Up key was taking effect on.

Returns:

  EFI_SUCCESS - the Page Up key was resonsed successfully.

--*/
{
  EFI_STATUS            Status;
  EFI_MENU_ITEM         *MenuItem;
  UINTN                 LastItemLine;
  UINTN                 TotalLine;
  UINTN                 Index;

  //
  // if there was no item to display return EFI_SUCCESS.
  //
  if (MenuPage == NULL || MenuPage->Body.ItemList == NULL ||
      MenuPage->Body.CurrentSelected == NULL) {
    return EFI_SUCCESS;
  }

  if (MenuPage->Body.CurrentSelected->Prev == NULL) {
    //
    //now it has come to the end so nothing to be done.
    //
    return EFI_SUCCESS;
  }
  if (MenuPage->Body.CurrentSelected != MenuPage->Body.FirstInDisplay) {
    //
    //in this page first make current selected pointer point to the last item.
    //
    MenuItem = MenuPage->Body.CurrentSelected;
    MenuPage->Body.CurrentSelected = MenuPage->Body.FirstInDisplay;
    //
    //refresh the two item.
    //
    Status = MenuItemRefresh(
               MenuPage,
               MenuItem,
               TRUE
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Status = MenuItemRefresh (
               MenuPage,
               MenuPage->Body.CurrentSelected,
               TRUE
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    //done successfully return EFI_SUCCESS.
    //
    return EFI_SUCCESS;
  }

  //
  //now should scroll the menu items.
  //first caculate if the remain items can fill up the menu item display area.
  //

  TotalLine = MenuPage->Body.BodyRect.BottomRight.Row - MenuPage->Body.SplitPoint.Row - 1;
  MenuItem  = MenuPage->Body.FirstInDisplay->Prev;
  LastItemLine = 0;
  while (MenuItem != NULL) {
    LastItemLine++;
    LastItemLine += MenuItem->Skip;
    if (LastItemLine >= TotalLine || MenuItem->Prev == NULL) {
      break;
    }
    MenuItem = MenuItem->Prev;
  }

  MenuPage->Body.FirstInDisplay  = MenuItem;
  MenuPage->Body.CurrentSelected = MenuItem;
  //
  //caculate the last in display position.
  //
  Index    = 0;
  Index   += (MenuItem->Skip + 1);
  while (MenuItem->Next != NULL && Index < TotalLine) {
    MenuItem   = MenuItem->Next;
    Index += (MenuItem->Skip + 1);
  }
  MenuPage->Body.LastInDisplay = MenuItem;
  //
  //now refresh all the menu items.
  //
  MenuBodyItemsRefresh (MenuPage);

  return EFI_SUCCESS;
}

EFI_STATUS
PageDownFunc (
  IN EFI_MENU_PAGE        *MenuPage
  )
/*++

Routine Description:

  the Page Down process function, based on if we need scroll menuitem
  it called menubodyrefresh or menuitemrefresh respectively.

Arguments:

  MenuPage    - which menu page the Page Down key was taking effect on.

Returns:

  EFI_SUCCESS - the Page Down key was resonsed successfully.

--*/
{
  EFI_STATUS            Status;
  EFI_MENU_ITEM         *MenuItem;
  UINTN                 LastItemLine;
  UINTN                 TotalLine;
  UINTN                 Index;

  //
  // if there was no item to display return EFI_SUCCESS.
  //
  if (MenuPage == NULL || MenuPage->Body.ItemList == NULL ||
      MenuPage->Body.CurrentSelected == NULL) {
    return EFI_SUCCESS;
  }

  if (MenuPage->Body.CurrentSelected->Next == NULL) {
    //
    //now it has come to the end so nothing to be done.
    //
    return EFI_SUCCESS;
  }
  if (MenuPage->Body.CurrentSelected != MenuPage->Body.LastInDisplay) {
    //
    //in this page first make current selected pointer point to the last item.
    //
    MenuItem = MenuPage->Body.CurrentSelected;
    MenuPage->Body.CurrentSelected = MenuPage->Body.LastInDisplay;
    //
    //refresh the two item.
    //
    Status = MenuItemRefresh(
               MenuPage,
               MenuItem,
               TRUE
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Status = MenuItemRefresh (
               MenuPage,
               MenuPage->Body.CurrentSelected,
               TRUE
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    //done successfully return EFI_SUCCESS.
    //
    return EFI_SUCCESS;
  }

  //
  //now should scroll the menu items.
  //first caculate if the remain items can fill up the menu item display area.
  //

  TotalLine = MenuPage->Body.BodyRect.BottomRight.Row - MenuPage->Body.SplitPoint.Row - 1;
  MenuItem  = MenuPage->Body.LastInDisplay->Next;
  LastItemLine = 0;
  while (MenuItem != NULL) {
    LastItemLine++;
    LastItemLine += MenuItem->Skip;
    if (LastItemLine >= TotalLine || MenuItem->Next == NULL) {
      break;
    }
    MenuItem = MenuItem->Next;
  }

  if (LastItemLine >= TotalLine) {
    //
    //the remain item call fill up the menu item display area so move down one
    //page.
    //
    MenuPage->Body.FirstInDisplay  = MenuPage->Body.LastInDisplay->Next;
    MenuPage->Body.LastInDisplay   = MenuItem;
    MenuPage->Body.CurrentSelected = MenuItem;
    //
    //now refresh all the menu items.
    //
    MenuBodyItemsRefresh (MenuPage);
    return EFI_SUCCESS;

  } else {

   //
   //the remain menu items can not fill up the menu item display area so
   //re locate the position of first and last in display menu item.
   //
   LastItemLine = LastItemLine + MenuPage->Body.LastInDisplay->Skip - MenuItem->Skip;

   MenuPage->Body.LastInDisplay   = MenuItem;
   MenuPage->Body.CurrentSelected = MenuItem;

   //
   //move down FirstInDisplay 'LastItemLine' that just caculated.
   //
   MenuItem = MenuPage->Body.FirstInDisplay;
   Index = 0;
   while (Index < LastItemLine) {
     Index    = 1 + Index + MenuItem->Skip;
     MenuItem = MenuItem->Next;
   }
   MenuPage->Body.FirstInDisplay = MenuItem;
    //
    //now refresh all the menu items.
    //
    MenuBodyItemsRefresh (MenuPage);
    return EFI_SUCCESS;
  }
}


EFI_STATUS
DoEdit (
  IN EFI_MENU_PAGE        *MenuPage,
  IN OUT EFI_INPUT_KEY    *InKey
  )
/*++

Routine Description:

  if current selected menu item type is EFI_ITEM_HAVE_EDIT then
  this function will process the input key that can not be processed
  by the well-known key process function.

Arguments:

  MenuPage  - which menu page the input key was taking effect on.
  InKey     - the input key to be processed.

Returns:

  EFI_SUCCESS           - the key was processed in current edit successfully.
  EFI_INVALID_PARAMETER

--*/
{
  EFI_STATUS              Status;
  EFI_MENU_ITEM           *MenuItem;
  INTN                    Ypos;
  INTN                    XLeft;
  EFI_INPUT_KEY           Key;
  INTN                    Index;
  BOOLEAN                 WAITING;
  CHAR16                  Buffer[EFI_MAX_EDIT_LENGTH + 1];
  CHAR16                  TempStr[EFI_MAX_ITEM_VALUE_LENGTH + 1];
  INTN                    XOffSetInScreen;
  INTN                    XOffSetInBuffer;
  INTN                    FirstInDisplay;
  INTN                    LastInDisplay;
  INTN                    CurrentLength;
  UINTN                   TempValue;
  EFI_DIALOG_CONTEXT      MsgDialogContext;
  CHAR16                  *MsgDialogTitle;
  CHAR16                  *MaxValueStr;
  CHAR16                  *MinValueStr;
  UINTN                   MaxValue;
  UINTN                   MinValue;
  UINTN                   DefaultValue;

  //
  // if there was no item  return EFI_SUCCESS.
  //
  if (MenuPage == NULL || MenuPage->Body.ItemList == NULL||
      MenuPage->Body.CurrentSelected == NULL) {
    return EFI_SUCCESS;
  }

  //
  //initialize varibles
  //
  Status  = EFI_SUCCESS;
  WAITING = TRUE;

  TempStr[0] = L'\0';
  Index      = 0;

  //
  //caculate current selected item line offset
  //
  MenuItem = MenuPage->Body.FirstInDisplay;
  while (MenuItem != NULL && MenuItem != MenuPage->Body.CurrentSelected) {
    Index += 1 + MenuItem->Skip;
    MenuItem   = MenuItem->Next;
  }
  Index++;

  Ypos   = MenuPage->Body.SplitPoint.Row + Index;
  XLeft  = MenuPage->Body.SplitPoint.Col - 2 - EFI_MAX_ITEM_VALUE_LENGTH;

  CurrentLength = StrLen (MenuPage->Body.CurrentSelected->ItemValue);

  StrCpy (Buffer, MenuPage->Body.CurrentSelected->ItemValue);

  XOffSetInScreen = 0;
  XOffSetInBuffer = 0;
  FirstInDisplay  = 0;

  if (CurrentLength > EFI_MAX_ITEM_VALUE_LENGTH) {
    LastInDisplay = EFI_MAX_ITEM_VALUE_LENGTH - 1;
  } else if (CurrentLength  > 0) {
    LastInDisplay = CurrentLength - 1;
  } else {
    LastInDisplay = 0;
  }

  //
  //enable cursor
  //
  ST->ConOut->EnableCursor (ST->ConOut, TRUE);

  //
  //
  //White color is better
  //
  ST->ConOut->SetAttribute (
            ST->ConOut,
            EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
            );
  ST->ConOut->SetCursorPosition (ST->ConOut, XLeft, Ypos);

  if (InKey != NULL && (InKey->ScanCode != 0 || InKey->UnicodeChar != 0)) {
    Key.ScanCode    = InKey->ScanCode;
    Key.UnicodeChar = InKey->UnicodeChar;
  } else {
    Key.ScanCode    = SCAN_NULL;
    Key.UnicodeChar = CHAR_NULL;
  }

  while (WAITING) {
    switch (Key.ScanCode) {
      case SCAN_F3:
        InKey->ScanCode    = Key.ScanCode;
        InKey->UnicodeChar = Key.UnicodeChar;

        //
        //White color is better
        //
        ST->ConOut->SetAttribute (
                  ST->ConOut,
                  EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                  );
        ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft,
                Ypos
                );
        WAITING = FALSE;
        break;

      case SCAN_ESC:
      case SCAN_UP:
      case SCAN_DOWN:
        //
        //save the string value into orginal buffer
        //
        TempValue = Atoi(Buffer);
        if ((MenuPage->Body.CurrentSelected->ItemType & EFI_EDIT_SUB_TYPE_MASK) == EFI_ITEM_EDIT_NUMBER
          && MenuPage->Body.CurrentSelected->CheckValue == TRUE) {
          MaxValue = MenuPage->Body.CurrentSelected->MaxValue;
          MinValue = MenuPage->Body.CurrentSelected->MinValue;
          DefaultValue = MenuPage->Body.CurrentSelected->DefaultValue;
          MaxValueStr = PoolPrint (L"%d", MaxValue);
          MinValueStr = PoolPrint (L"%d", MinValue);

          if ( CompareNumberString (Buffer, MaxValueStr) > 0 ||
               CompareNumberString (Buffer, MinValueStr) < 0) {
            MsgDialogTitle = PoolPrint (
                               L"Please Input value of <%s> between [%d,%d]",
                               MenuPage->Body.CurrentSelected->ItemString.Text,
                               MinValue,
                               MaxValue
                               );
            MsgDialogContext.Type = EFI_DIALOG_TYPE_MESSAGE;
            DoDialog (MsgDialogTitle, &MsgDialogContext);
            for (Index = 0; Index < EFI_MAX_ITEM_VALUE_LENGTH; Index++) {
              if (Buffer[Index] != 0) {
                Buffer[Index] = L' ';
              } else {
                break;
              }
            }
            StrCpy (
              MenuPage->Body.CurrentSelected->ItemValue,
              Buffer
              );
            Buffer[Index] = L'\0';

            MenuPageRefresh (MenuPage);
            WAITING = TRUE;

            ST->ConOut->SetAttribute (
                      ST->ConOut,
                      EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                      );
            Status = TestPrintAt (XLeft, Ypos, Buffer);
            SPrint (
              TempStr,
              EFI_MAX_EDIT_LENGTH + 1,
              L"%d",
              DefaultValue
              );

            for (Index = 0; Index < EFI_MAX_ITEM_VALUE_LENGTH; Index++) {
              Buffer[Index] = TempStr[Index];
              if (TempStr[Index] == 0) {
                break;
              }
            }
            Status = TestPrintAt (XLeft, Ypos, TempStr);

            XOffSetInScreen = 0;
            XOffSetInBuffer = 0;
            FirstInDisplay  = 0;
            CurrentLength = StrLen (Buffer);
            if (CurrentLength > EFI_MAX_ITEM_VALUE_LENGTH) {
              LastInDisplay = EFI_MAX_ITEM_VALUE_LENGTH - 1;
            } else if (CurrentLength  > 0) {
              LastInDisplay = CurrentLength - 1;
            } else {
              LastInDisplay = 0;
            }

            ST->ConOut->EnableCursor (ST->ConOut, TRUE);

            //
            //White color is better
            //
            ST->ConOut->SetAttribute (
                      ST->ConOut,
                      EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                      );
            ST->ConOut->SetCursorPosition (ST->ConOut, XLeft, Ypos);
            break;
          }

          FreePool (MaxValueStr);
          FreePool (MinValueStr);
        }
        if ((MenuPage->Body.CurrentSelected->ItemType & EFI_EDIT_SUB_TYPE_MASK)
               == EFI_ITEM_EDIT_NUMBER) {
          TempValue = Atoi (Buffer);
          SPrint (
              Buffer,
              EFI_MAX_EDIT_LENGTH + 1,
              L"%d",
              TempValue
              );
        }
        StrCpy (
          MenuPage->Body.CurrentSelected->ItemValue,
          Buffer
          );
        Status = MenuItemRefresh (
                 MenuPage,
                 MenuPage->Body.CurrentSelected,
                 FALSE
                 );
        if (EFI_ERROR (Status)) {
          return Status;
        }

        WAITING = FALSE;
        MenuItem = MenuPage->Body.CurrentSelected;
        if (((Key.ScanCode == SCAN_UP) && (MenuItem->Prev == NULL)) ||
            ((Key.ScanCode == SCAN_DOWN) && (MenuItem->Next == NULL))) {
          //
          //no need to call the UpFunc or DownFunc so don't return the scancode
          //
          InKey->ScanCode    = SCAN_NULL;
          InKey->UnicodeChar = CHAR_NULL;

          //
          //White color is better
          //
          ST->ConOut->SetAttribute (
                    ST->ConOut,
                    EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                    );
          ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft,
                Ypos
                );
        } else {
          ST->ConOut->EnableCursor (ST->ConOut, FALSE);
          InKey->ScanCode    = Key.ScanCode;
          InKey->UnicodeChar = Key.UnicodeChar;
        }
        break;

      case SCAN_LEFT:
        if (XOffSetInScreen > 0) {

          //
          //move cursor directly in screen and in buffer.
          //
          XOffSetInScreen--;
          if (XOffSetInBuffer > 0) {
            XOffSetInBuffer--;
          }

          //
          //White color is better
          //
          ST->ConOut->SetAttribute (
                    ST->ConOut,
                    EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                    );
          ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft + XOffSetInScreen,
                Ypos
                );
        } else if (FirstInDisplay >0 ) {
          //
          //on the left of cursor there is char also,
          //so move the char to right so let it been displayed.
          //
          if ((LastInDisplay - FirstInDisplay + 1) == EFI_MAX_ITEM_VALUE_LENGTH) {
            LastInDisplay--;
          }
          FirstInDisplay--;

          if (XOffSetInBuffer > 0) {
            XOffSetInBuffer--;
          }
          //
          //refresh display in screen
          //
          BS->CopyMem (
            TempStr,
            Buffer + FirstInDisplay,
            EFI_MAX_ITEM_VALUE_LENGTH * sizeof(CHAR16)
            );
          TempStr[EFI_MAX_ITEM_VALUE_LENGTH] = L'\0';
          ST->ConOut->SetAttribute (
                        ST->ConOut,
                        EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                        );
          Status = TestPrintAt (XLeft, Ypos, TempStr);
          if (EFI_ERROR (Status)) {
            return Status;
          }

          //
          //White color is better
          //
          ST->ConOut->SetAttribute (
                    ST->ConOut,
                    EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                    );
          ST->ConOut->SetCursorPosition (
                        ST->ConOut,
                        XLeft + XOffSetInScreen,
                        Ypos
                        );
        }
        break;

      case SCAN_RIGHT:

        if (XOffSetInBuffer < CurrentLength) {

          if (XOffSetInScreen < EFI_MAX_ITEM_VALUE_LENGTH -1) {
            //
            //before the cursor in screen point to the LastInDisplay
            //move the cursor to next char.
            //
            XOffSetInBuffer++;
            XOffSetInScreen++;
            //
            //set cursor
            //
            ST->ConOut->SetAttribute (
                  ST->ConOut,
                  EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                  );

            ST->ConOut->SetCursorPosition (
                  ST->ConOut,
                  XLeft + XOffSetInScreen,
                  Ypos
                  );
            break;
          } else {
            if (LastInDisplay + 1 < CurrentLength) {
              XOffSetInBuffer++;
              LastInDisplay++;
              FirstInDisplay++;
              //
              //refresh edit area.
              //
              BS->CopyMem (
                TempStr,
                Buffer + FirstInDisplay,
                EFI_MAX_ITEM_VALUE_LENGTH * sizeof(CHAR16)
                );
              TempStr[EFI_MAX_ITEM_VALUE_LENGTH] = L'\0';
              ST->ConOut->SetAttribute (
                            ST->ConOut,
                            EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                            );
              Status = TestPrintAt (XLeft, Ypos, TempStr);
              if (EFI_ERROR (Status)) {
                return Status;
              }
              //
              //reset cursor position
              //
              ST->ConOut->SetAttribute (
                            ST->ConOut,
                            EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                            );
              ST->ConOut->SetCursorPosition (
                            ST->ConOut,
                            XLeft + XOffSetInScreen,
                            Ypos
                            );
              break;
            } else if (LastInDisplay + 1 == CurrentLength) {
              FirstInDisplay++;
              XOffSetInBuffer++;
              for (Index = 0; Index < LastInDisplay - FirstInDisplay + 1; Index++) {
                TempStr[Index] = Buffer[Index + FirstInDisplay];
              }
              TempStr[EFI_MAX_ITEM_VALUE_LENGTH -1] = L' ';
              TempStr[EFI_MAX_ITEM_VALUE_LENGTH]    = L'\0';
              //
              //refresh edit area
              //
              ST->ConOut->SetAttribute (
                            ST->ConOut,
                            EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                            );
              Status = TestPrintAt (XLeft, Ypos, TempStr);
              if (EFI_ERROR (Status) ) {
                return Status;
              }
              //
              //reset cursor
              //
              ST->ConOut->SetAttribute (
                    ST->ConOut,
                    EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                    );
              ST->ConOut->SetCursorPosition (
                            ST->ConOut,
                            XLeft + XOffSetInScreen,
                            Ypos
                            );
            }
          }
        }

        break;

      case SCAN_HOME:
        if (CurrentLength>0) {
          //
          //reset the position varibles
          //
          FirstInDisplay   = 0;
          XOffSetInScreen  = 0;
          XOffSetInBuffer  = 0;
          if (CurrentLength >= EFI_MAX_ITEM_VALUE_LENGTH) {
            LastInDisplay = EFI_MAX_ITEM_VALUE_LENGTH - 1;
          } else {
            LastInDisplay = CurrentLength - 1;
          }
          for (Index = 0; Index < LastInDisplay + 1; Index++) {
            TempStr[Index] = Buffer[Index];
          }
          TempStr[Index] = L'\0';
          //
          //refresh the edit area in screen
          //
          ST->ConOut->SetAttribute (
                ST->ConOut,
                EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                );
          Status = TestPrintAt (XLeft, Ypos, TempStr);
          if ( EFI_ERROR (Status)) {
            return Status;
          }
          //
          //reset cursor position.
          //
          ST->ConOut->SetAttribute (
                ST->ConOut,
                EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                );
          ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft + XOffSetInScreen,
                Ypos
                );
        }
        break;

      case SCAN_END:
        if (CurrentLength > 0) {
          //
          //reset these position varibles.
          //
          LastInDisplay = CurrentLength - 1;
          if (CurrentLength < EFI_MAX_ITEM_VALUE_LENGTH) {
            FirstInDisplay  = 0;
            XOffSetInBuffer = CurrentLength;
            XOffSetInScreen = CurrentLength;
          } else {
            XOffSetInScreen = EFI_MAX_ITEM_VALUE_LENGTH - 1;
            XOffSetInBuffer = CurrentLength;
            FirstInDisplay  = LastInDisplay - EFI_MAX_ITEM_VALUE_LENGTH + 2;
          }
          //
          //first clear the edit display area
          //
          for (Index = 0; Index < EFI_MAX_ITEM_VALUE_LENGTH; Index++ ) {
            TempStr[Index] = L' ';
          }
          TempStr[Index] = L'\0';
          ST->ConOut->SetAttribute (
                ST->ConOut,
                EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                );
          Status = TestPrintAt (XLeft, Ypos, TempStr);
          if (EFI_ERROR (Status) ) {
            return Status;
          }
          for (Index = 0; Index < LastInDisplay - FirstInDisplay + 1; Index++ ) {
            TempStr[Index] = Buffer[Index + FirstInDisplay];
          }
          TempStr[Index] = L'\0';
          ST->ConOut->SetAttribute (
                ST->ConOut,
                EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                );
          Status = TestPrintAt (XLeft, Ypos, TempStr);
          if (EFI_ERROR (Status) ) {
            return Status;
          }
          //
          //reset cusor.
          //
          ST->ConOut->SetAttribute (
                ST->ConOut,
                EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                );
          ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft + XOffSetInScreen,
                Ypos
                );
        }
        break;

      case SCAN_DELETE:
        if (Buffer[XOffSetInBuffer] != L'\0' ) {
          //
          //delete this char in buffer
          //
          for(Index = XOffSetInBuffer; Index < CurrentLength; Index++) {
            Buffer[Index] = Buffer[Index + 1];
          }
          if ((CurrentLength - FirstInDisplay) <= EFI_MAX_ITEM_VALUE_LENGTH &&
               LastInDisplay > FirstInDisplay) {
            LastInDisplay--;
          }
          if ((CurrentLength - FirstInDisplay) <= EFI_MAX_ITEM_VALUE_LENGTH) {
            //
            //first clear the edit area
            //
            for(Index = 0; Index < EFI_MAX_ITEM_VALUE_LENGTH; Index++) {
              TempStr[Index] = L' ';
            }
            TempStr[Index] = L'\0';
            ST->ConOut->SetAttribute (
                  ST->ConOut,
                  EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                  );
            Status = TestPrintAt (XLeft, Ypos, TempStr);
            if (EFI_ERROR (Status)) {
              return Status;
            }
          }
          //
          //refresh the edit area
          //
          for (Index = 0; Index < LastInDisplay - FirstInDisplay + 1; Index++) {
            TempStr[Index] = Buffer[Index + FirstInDisplay];
          }
          TempStr[Index] = L'\0';
          ST->ConOut->SetAttribute (
                ST->ConOut,
                EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                );
          Status = TestPrintAt (XLeft, Ypos, TempStr);
          if (EFI_ERROR (Status)) {
            return Status;
          }
          //
          //set cursor
          //
          ST->ConOut->SetAttribute (
                ST->ConOut,
                EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                );
          ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft + XOffSetInScreen,
                Ypos
                );
          CurrentLength--;
        }
        break;

      case SCAN_NULL:
        switch (Key.UnicodeChar) {
          case L' ':

            break;
          case CHAR_NULL:
            break;

          case CHAR_BACKSPACE:
            if (XOffSetInBuffer > 0) {
              //
              //remove the char before current pointed char in buffer.
              //
              for(Index = XOffSetInBuffer; Index <= CurrentLength; Index++) {
                Buffer[Index - 1] = Buffer[Index];
              }
              XOffSetInBuffer--;

              if (XOffSetInScreen > 0) {
                XOffSetInScreen--;
                //
                //clear the Edit area
                //
                for(Index = 0; Index < EFI_MAX_ITEM_VALUE_LENGTH; Index++) {
                  TempStr[Index] = L' ';
                }
                TempStr[Index] = L'\0';
                ST->ConOut->SetAttribute (
                      ST->ConOut,
                      EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                      );
                Status = TestPrintAt (XLeft, Ypos, TempStr);
                if (EFI_ERROR (Status)) {
                  return Status;
                }
                if (LastInDisplay > FirstInDisplay &&
                   (CurrentLength - FirstInDisplay) <= EFI_MAX_ITEM_VALUE_LENGTH) {
                  LastInDisplay--;
                }
                //
                //refresh the Edit area
                //
                for (Index = 0; Index < LastInDisplay - FirstInDisplay + 1; Index++) {
                  TempStr[Index] = Buffer[Index + FirstInDisplay];
                }
                TempStr[Index] = L'\0';
                ST->ConOut->SetAttribute (
                      ST->ConOut,
                      EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                      );
                Status = TestPrintAt (XLeft, Ypos, TempStr);
                if (EFI_ERROR (Status)) {
                  return Status;
                }
                //
                //set cursor
                //
                ST->ConOut->SetAttribute (
                      ST->ConOut,
                      EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                      );
                ST->ConOut->SetCursorPosition (
                      ST->ConOut,
                      XLeft + XOffSetInScreen,
                      Ypos
                      );
              } else if ( FirstInDisplay > 0 ) {
                FirstInDisplay--;
                LastInDisplay--;
              }
              CurrentLength--;
            }
            break;

          case CHAR_LINEFEED:
            break;

          case CHAR_TAB:
            break;

          default:

            if (CurrentLength < EFI_MAX_EDIT_LENGTH) {
              if (Key.UnicodeChar == L'|') {
                break;
              }
              if ((MenuPage->Body.CurrentSelected->ItemType & EFI_EDIT_SUB_TYPE_MASK)
                   == EFI_ITEM_EDIT_NUMBER) {
                //
                //if this edit sub type is EFI_ITEM_EDIT_NUBER then
                //skip the char out of '0' to '9'.
                //
                if (Key.UnicodeChar < L'0' || Key.UnicodeChar > L'9') {
                  break;
                }
              }
              if (Key.UnicodeChar < 0x20 || Key.UnicodeChar >= 0x7F || Key.UnicodeChar == L'|') {
                break;
              }
              //
              //add this input key into the Buffer and refresh the display
              //
              if (XOffSetInScreen < EFI_MAX_ITEM_VALUE_LENGTH - 1) {
                //
                //set last char and first char in screen position.
                //
                if ((LastInDisplay - FirstInDisplay + 1) < EFI_MAX_ITEM_VALUE_LENGTH &&
                     LastInDisplay != CurrentLength) {

                  LastInDisplay++;

                }
                XOffSetInScreen++;
                //
                //add this char into buffer.
                //
                for (Index = CurrentLength; Index >= XOffSetInBuffer; Index--) {
                  Buffer[Index + 1] = Buffer[Index];
                }
                Buffer[XOffSetInBuffer] = Key.UnicodeChar;
                XOffSetInBuffer++;
                //
                //the count of char increase one
                //
                CurrentLength++;

                for (Index = 0; Index < LastInDisplay - FirstInDisplay + 1; Index++) {
                  TempStr[Index] = Buffer[Index + FirstInDisplay];
                }
                TempStr[Index] = L'\0';
                ST->ConOut->SetAttribute (
                      ST->ConOut,
                      EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                      );
                Status = TestPrintAt (XLeft, Ypos, TempStr);
                if (EFI_ERROR (Status)) {
                  return Status;
                }
                ST->ConOut->SetAttribute (
                      ST->ConOut,
                      EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                      );
                ST->ConOut->SetCursorPosition (
                      ST->ConOut,
                      XLeft + XOffSetInScreen,
                      Ypos
                      );
              } else {
                LastInDisplay  = LastInDisplay + 1;
                FirstInDisplay = FirstInDisplay + 1;
                //
                //add this char into buffer.
                //
                if (Buffer[XOffSetInBuffer] == L'\0') {
                  Buffer[XOffSetInBuffer] = Key.UnicodeChar;
                  XOffSetInBuffer++;
                  Buffer[XOffSetInBuffer] = L'\0';
                } else {
                  for (Index = CurrentLength; Index >= XOffSetInBuffer; Index--) {
                    Buffer[Index + 1] = Buffer[Index];
                  }
                  Buffer[XOffSetInBuffer] = Key.UnicodeChar;
                  XOffSetInBuffer++;
                }
                //
                //the count of char increase one
                //
                CurrentLength++;

                for (Index = 0; Index < LastInDisplay - FirstInDisplay + 1; Index++) {
                  TempStr[Index] = Buffer[Index + FirstInDisplay];
                }
                TempStr[Index] = L'\0';
                ST->ConOut->SetAttribute (
                      ST->ConOut,
                      EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                      );
                Status = TestPrintAt (XLeft, Ypos, TempStr);
                if (EFI_ERROR (Status) ) {
                  return Status;
                }
                ST->ConOut->SetAttribute (
                      ST->ConOut,
                      EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                      );
                ST->ConOut->SetCursorPosition (
                      ST->ConOut,
                      XLeft + XOffSetInScreen,
                      Ypos
                      );
              }
            }
            break;

        }//end switch UnicodeChar
      default :
        break;

    }//end switch ScanCode.

    if (WAITING) {

      Status = BS->WaitForEvent (1, &(ST->ConIn->WaitForKey), (UINTN *)&Index);
      if (!EFI_ERROR(Status)) {
        Status = ST->ConIn->ReadKeyStroke (ST->ConIn, &Key);
      }

      if ( EFI_ERROR(Status)) {
        break;
      }
    }
  }

  return Status;
}

EFI_STATUS
SpaceFunc(
  IN EFI_MENU_PAGE        *MenuPage
  )
/*++

Routine Description:

  if current menu item type is EFI_ITEM_HAVE_CHECKBOX or
  EFI_ITEM_HAVE_RADIOBOX when space key was pressed the value of checkbox
  or radioboxessed will be changed.

Arguments:

  MenuPage    - which menu page the Space key was taking effect on.

Returns:

  EFI_SUCCESS           - the Space key was processed successfully.
  EFI_INVALID_PARAMETER

--*/
{
  EFI_STATUS      Status;
  EFI_MENU_ITEM   *MenuItem;
  EFI_INPUT_KEY   Key;
  EFI_HOT_KEY     *HotKey;

  //
  // if there was no item  return EFI_SUCCESS.
  //
  if (MenuPage == NULL || MenuPage->Body.ItemList == NULL ||
      MenuPage->Body.CurrentSelected == NULL) {
    return EFI_SUCCESS;
  }

  Status  = EFI_SUCCESS;
  MenuItem    = MenuPage->Body.CurrentSelected;
  if (MenuPage->HasBody) {

    switch (MENU_ITEM_TYPE(MenuItem->ItemType)) {

      case EFI_ITEM_HAVE_CHECKBOX:
        //
        //reset the item select status
        //
        if ( MenuItem->Status == EFI_ITEM_SELECT_NONE ) {
          MenuItem->Status = EFI_ITEM_SELECT_ALL;
        } else if ( MenuItem->Status == EFI_ITEM_SELECT_ALL ) {
          MenuItem->Status = EFI_ITEM_SELECT_NONE;
        } else if ( MenuItem->Status == EFI_ITEM_SELECT_SOME ) {
          MenuItem->Status = EFI_ITEM_SELECT_NONE;
        }
        //
        //refresh the menu item.
        //
        Status = MenuItemRefresh (
                   MenuPage,
                   MenuPage->Body.CurrentSelected,
                   FALSE
                   );
        break;

      case EFI_ITEM_HAVE_RADIOBOX:
        if (MenuItem->IsSelected) {
          MenuItem->IsSelected = FALSE;
        } else {
          MenuItem->IsSelected = TRUE;
        }
        //
        //refresh the menu item.
        //
        Status = MenuItemRefresh (
                   MenuPage,
                   MenuPage->Body.CurrentSelected,
                   FALSE
                   );

        break;

      case EFI_ITEM_HAVE_EDIT:

        Key.ScanCode    = SCAN_NULL;
        Key.UnicodeChar = L' ';
        //
        //call edit function
        //

        Status = DoEdit (MenuPage, &Key);
        if (!EFI_ERROR(Status)) {
          //
          //check if there is not responsed key.
          //
          if (Key.ScanCode == SCAN_UP || Key.ScanCode == SCAN_DOWN ||
              Key.ScanCode == SCAN_ESC) {
            HotKey = MenuPage->Footer.HotKeyList;
            while(HotKey != NULL) {
              if (HotKey->KeyCode.ScanCode == Key.ScanCode &&
                  HotKey->KeyCode.UnicodeChar == Key.UnicodeChar) {
                break;
              }
              HotKey = HotKey->Next;
            }
            if (HotKey != NULL && HotKey->Context != NULL) {
              Status = (HotKey->Context) (MenuPage);
            }
          }
        }
        break;

      default :
        break;
    }
  }
  return Status;
}

EFI_STATUS
LeftArrowFunc(
  IN EFI_MENU_PAGE        *MenuPage
  )
/*++

Routine Description:

  if current menu item type is EFI_ITEM_HAVE_COMBOLIST, when Left Arrow
  key was pressed the value of Current Menu Item may be changed.

Arguments:

  MenuPage    - which menu page the LeftArrow key was taking effect on.

Returns:

  EFI_SUCCESS           - the LeftArrow key was processed successfully.
  EFI_INVALID_PARAMETER

--*/
{
  EFI_ITEM_VALUE_QUEUE  *ValueQueue;
  EFI_STATUS            Status;
  EFI_INPUT_KEY         Key;
  EFI_HOT_KEY           *HotKey;

  //
  // if there was no item return EFI_SUCCESS.
  //
  if (MenuPage == NULL || MenuPage->Body.ItemList == NULL||
      MenuPage->Body.CurrentSelected == NULL) {
    return EFI_SUCCESS;
  }

  Status      = EFI_SUCCESS;
  ValueQueue  = NULL;

  if (MENU_ITEM_TYPE(MenuPage->Body.CurrentSelected->ItemType) == EFI_ITEM_HAVE_COMBOLIST) {
    if (MenuPage->Body.CurrentSelected->ValueQueue != NULL &&
        MenuPage->Body.CurrentSelected->ItemValue != NULL) {

      ValueQueue = MenuPage->Body.CurrentSelected->ValueQueue;
      //
      //found current pointed value queue
      //
      if (ValueQueue->Prev != ValueQueue ) {
        while (ValueQueue->StringValue != MenuPage->Body.CurrentSelected->ItemValue) {
          ValueQueue = ValueQueue->Prev;
        }
        //
        //reset the itemvalue
        //
        MenuPage->Body.CurrentSelected->ItemValue = ValueQueue->Prev->StringValue;
        Status = MenuItemRefresh (
                   MenuPage,
                   MenuPage->Body.CurrentSelected,
                   FALSE
                   );
      }
    }

  } else if (MENU_ITEM_TYPE(MenuPage->Body.CurrentSelected->ItemType) == EFI_ITEM_HAVE_EDIT) {
    Key.ScanCode    = SCAN_LEFT;
    Key.UnicodeChar = CHAR_NULL;
    //
    //call edit function
    //
    Status = DoEdit (MenuPage, &Key);
    if (!EFI_ERROR(Status)) {
      //
      //check if there is not responsed key.
      //
      if (Key.ScanCode == SCAN_UP || Key.ScanCode == SCAN_DOWN ||
          Key.ScanCode == SCAN_ESC) {
        HotKey = MenuPage->Footer.HotKeyList;
        while(HotKey != NULL) {
          if (HotKey->KeyCode.ScanCode == Key.ScanCode &&
              HotKey->KeyCode.UnicodeChar == Key.UnicodeChar) {
            break;
          }
          HotKey = HotKey->Next;
        }
        if (HotKey != NULL && HotKey->Context != NULL) {
          Status = (HotKey->Context) (MenuPage);
        }
      }
    }
  }

  return Status;
}

EFI_STATUS
RightArrowFunc(
  IN EFI_MENU_PAGE *MenuPage
  )
/*++

Routine Description:

  if current menu item type is EFI_ITEM_HAVE_COMBOLIST, when Right Arrow
  key was pressed the value of Current Menu Item may be changed.

Arguments:

  MenuPage    - which menu page the RightArrow key was taking effect on.

Returns:

  EFI_SUCCESS           - the RightArrow key was processed successfully.
  EFI_INVALID_PARAMETER

--*/
{
  EFI_ITEM_VALUE_QUEUE  *ValueQueue;
  EFI_STATUS            Status;
  EFI_INPUT_KEY         Key;
  EFI_HOT_KEY           *HotKey;

  //
  // if there was no item return EFI_SUCCESS.
  //
  if (MenuPage == NULL || MenuPage->Body.ItemList == NULL||
      MenuPage->Body.CurrentSelected == NULL) {
    return EFI_SUCCESS;
  }

  Status      = EFI_SUCCESS;
  ValueQueue  = NULL;

  if (MENU_ITEM_TYPE(MenuPage->Body.CurrentSelected->ItemType) == EFI_ITEM_HAVE_COMBOLIST) {
    if (MenuPage->Body.CurrentSelected->ValueQueue != NULL &&
        MenuPage->Body.CurrentSelected->ItemValue != NULL) {

      ValueQueue = MenuPage->Body.CurrentSelected->ValueQueue;
      //
      //found current pointed value queue
      //
      if (ValueQueue->Next!=ValueQueue ) {
        while (ValueQueue->StringValue != MenuPage->Body.CurrentSelected->ItemValue) {
          ValueQueue = ValueQueue->Next;
        }
        //
        //reset the itemvalue
        //
        MenuPage->Body.CurrentSelected->ItemValue = ValueQueue->Next->StringValue;
        Status = MenuItemRefresh (
                   MenuPage,
                   MenuPage->Body.CurrentSelected,
                   FALSE
                   );
      }
    }
  } else if (MENU_ITEM_TYPE(MenuPage->Body.CurrentSelected->ItemType) == EFI_ITEM_HAVE_EDIT) {
    Key.ScanCode    = SCAN_RIGHT;
    Key.UnicodeChar = CHAR_NULL;
    //
    //call edit function
    //

    Status = DoEdit (MenuPage, &Key);
    if (!EFI_ERROR(Status)) {
      //
      //check if there is not responsed key.
      //
      if (Key.ScanCode == SCAN_UP || Key.ScanCode == SCAN_DOWN ||
          Key.ScanCode == SCAN_ESC) {

        HotKey = MenuPage->Footer.HotKeyList;
        while(HotKey != NULL) {
          if (HotKey->KeyCode.ScanCode == Key.ScanCode &&
              HotKey->KeyCode.UnicodeChar == Key.UnicodeChar) {
            break;
          }
          HotKey=HotKey->Next;
        }
        if (HotKey != NULL && HotKey->Context != NULL) {
          Status = (HotKey->Context) (MenuPage);
        }
      }
    }
  }

  return Status;
}

EFI_STATUS
MenuPageKeyInput (
  VOID
  )
/*++

Routine Description:

  the menupage key input event response function.
  what ever key was pressed this function will give correct response.

Returns:

  EFI_SUCCESS           - the keys was processed successfully.
  EFI_INVALID_PARAMETER

--*/
{
  EFI_STATUS       Status;
  EFI_INPUT_KEY    Key;
  INTN             Index;
  EFI_HOT_KEY      *HotKey;

  //
  //initialize varibles
  //
  Status          = EFI_SUCCESS;
  Key.ScanCode    = 0x00;
  Key.UnicodeChar = 0x00;
  HotKey          = NULL;
  Index           = 0;

  do {
    //
    //waiting for key press event
    //
    Status=BS->WaitForEvent (1, &(ST->ConIn->WaitForKey), (UINTN *)&Index);
    if (!EFI_ERROR(Status)) {

      //
      //read pressed key
      //
      Status = ST->ConIn->ReadKeyStroke (ST->ConIn, &Key);
      if (!EFI_ERROR(Status)) {
        if (gMenuPage->HasFooter) {
          //
          //search Hot Key function
          //
          HotKey = gMenuPage->Footer.HotKeyList;
          while(HotKey != NULL) {
            if (HotKey->KeyCode.ScanCode == Key.ScanCode &&
                HotKey->KeyCode.UnicodeChar == Key.UnicodeChar) {
              break;
            }
            HotKey = HotKey->Next;
          }
          if (HotKey != NULL && HotKey->Context != NULL) {
            //
            //call Hot Key function
            //
            Status = (HotKey->Context)(gMenuPage);
          } else if (gMenuPage->Body.CurrentSelected != NULL &&
                     ((MENU_ITEM_TYPE(gMenuPage->Body.CurrentSelected->ItemType) == EFI_ITEM_HAVE_EDIT) ||
                      (MENU_ITEM_TYPE(gMenuPage->Body.CurrentSelected->ItemType) == EFI_ITEM_CASE))) {
            //
            //call default edit funtion
            //
            if (MENU_ITEM_TYPE (gMenuPage->Body.CurrentSelected->ItemType) == EFI_ITEM_CASE) {
              Status = DoIterEdit (gMenuPage, &Key);
            } else {
              Status = DoEdit (gMenuPage, &Key);
            }
            if (!EFI_ERROR(Status)) {
              //
              //check if there is not responsed key.
              //
              if (Key.ScanCode == SCAN_UP || Key.ScanCode == SCAN_DOWN ||
                  Key.ScanCode == SCAN_ESC || Key.ScanCode == SCAN_F3 || Key.UnicodeChar == L' ' ||
                  Key.ScanCode == SCAN_F9 || Key.ScanCode == SCAN_F8 || Key.UnicodeChar == CHAR_CARRIAGE_RETURN ||
                  Key.ScanCode == SCAN_PAGE_UP || Key.ScanCode == SCAN_PAGE_DOWN) {
                HotKey = gMenuPage->Footer.HotKeyList;
                while(HotKey != NULL) {
                  if (HotKey->KeyCode.ScanCode == Key.ScanCode &&
                      HotKey->KeyCode.UnicodeChar == Key.UnicodeChar) {
                    break;
                  }
                  HotKey=HotKey->Next;
                }
                if (HotKey != NULL && HotKey->Context != NULL) {
                  Status = (HotKey->Context) (gMenuPage);
                }
              }
            }
          }
        } else {
          Status = DoDefault (gMenuPage, &Key);
        }
      }
    }
  } while (!gMenuExit);

  return Status;
}

EFI_STATUS
DisplayMessage (
  IN UINTN           X0,
  IN UINTN           Y0,
  IN UINTN           X1,
  IN UINTN           Y1,
  IN CHAR16          *Str
  )
/*++

Routine Description:

  display message in rectangle area

Argument:

  X0          - the top left X position in screen
  Y0          - the top left Y position in screen
  X1          - the bottom right X position in screen
  Y1          - the bottom right Y positoin in screen

Returns:

  EFI_SUCCESS - the message was displayed successfully

--*/
{
  EFI_STATUS        Status;
  UINTN             CurrentYpos;
  UINTN             CurrentXpos;
  CHAR16            *Buffer;
  CHAR16            *Space;
  CHAR16            *TempStr;
  CHAR16            *OneLine;

  if (Str == NULL || StrLen (Str) == 0) {
    return EFI_SUCCESS;
  }

  Buffer = StrDuplicate (Str);
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  TempStr        = Buffer;
  CurrentXpos = X0;
  CurrentYpos = Y0;

  while (*TempStr != L'\0' && CurrentYpos <= Y1) {

    //
    //get next SPACE position and set it as L'\0'
    //
    Space = TempStr;
    while (Space != NULL && *Space != L' ') {
      if (*Space == L'\0') {
        Space = (CHAR16 *)NULL;
      } else {
        Space++;
      }
    }

    if (Space != NULL) {
      *Space = L'\0';
    }
    if (CurrentXpos > X1) {
      CurrentXpos = X0;
      CurrentYpos = CurrentYpos + 1;
      if (CurrentYpos > Y1) {
        break;
      }
    }

    if (StrLen (TempStr) + CurrentXpos > X1 + 1 ) {
      //
      //if the word length less or equal X1-X0-1,
      //it will be displayed in one line.
      //
      if (StrLen (TempStr) <= X1 - X0 + 1) {
        CurrentXpos = X0;
        CurrentYpos = CurrentYpos + 1;
        if (CurrentYpos > Y1) {
          break;
        }
        Status = TestPrintAt (CurrentXpos, CurrentYpos, TempStr);
        if (EFI_ERROR(Status)) {
          BS->FreePool (Buffer);
          return Status;
        }
        CurrentXpos = CurrentXpos + StrLen (TempStr) + 1 ;

      } else {
        //
        //if one line can not display the string
        //it will displayed in several lines.
        //
        Status = BS->AllocatePool (
                       EfiBootServicesData,
                       (X1 - X0 + 2) * sizeof(CHAR16),
                       (VOID*)&OneLine
                       );
        if (EFI_ERROR(Status)) {
          BS->FreePool (Buffer);
          return EFI_OUT_OF_RESOURCES;
        }

        BS->SetMem (OneLine, (X1 - X0 + 2) * sizeof(CHAR16), 0);

        if (CurrentXpos < X1) {
          BS->CopyMem (OneLine, TempStr, (X1 - CurrentXpos) * sizeof(CHAR16));

          OneLine[X1 - CurrentXpos]    = L'-';
          OneLine[X1- CurrentXpos + 1] = L'\0';

          Status = TestPrintAt (CurrentXpos, CurrentYpos, OneLine);
          if (EFI_ERROR(Status)) {
            BS->FreePool (Buffer);
            BS->FreePool (OneLine);
            return Status;
          }

          TempStr        = TempStr + (X1 - CurrentXpos);
          CurrentXpos = X0;
          CurrentYpos = CurrentYpos + 1;

          if ( CurrentYpos > Y1) {
            break;
          }
        } else {
         CurrentXpos = X0;
         CurrentYpos = CurrentYpos + 1;
        }

        while ((StrLen (TempStr) > X1 - X0 + 1)&& CurrentYpos <= Y1 ) {

          BS->CopyMem (OneLine, TempStr, (X1 - X0) * sizeof(CHAR16));
          OneLine[X1 - X0]     = L'-';
          OneLine[X1 - X0 + 1] = L'\0';

          Status = TestPrintAt (CurrentXpos, CurrentYpos, OneLine);
          if (EFI_ERROR(Status)) {
            BS->FreePool (Buffer);
            BS->FreePool (OneLine);
            return Status;
          }

          TempStr        = TempStr + (X1 - X0);
          CurrentYpos = CurrentYpos + 1;
        }

        if (CurrentYpos > Y1) {
          break;
        }

        Status = TestPrintAt (CurrentXpos, CurrentYpos, TempStr);
        if (EFI_ERROR(Status)) {
          BS->FreePool (Buffer);
          BS->FreePool (OneLine);
          return Status;
        }

        BS->FreePool (OneLine);
        CurrentXpos = CurrentXpos + StrLen (TempStr) + 1 ;
      }
    } else {
      //
      //this word can be displayed in current line
      //
      Status = TestPrintAt (CurrentXpos, CurrentYpos, TempStr);
      if (EFI_ERROR(Status)) {
        BS->FreePool (Buffer);
        return Status;
      }
      CurrentXpos = CurrentXpos + StrLen (TempStr) + 1;
    }

    if (Space != NULL) {
      TempStr = Space + 1;
    } else {
      break;
    }
    while (*TempStr == L' ') {
      TempStr = TempStr + 1;
    }
  }

  //
  //all the message have been displayed so return EFI_SUCCESS
  //
  BS->FreePool (Buffer);

  return EFI_SUCCESS;
}

EFI_STATUS
DefaultDescPrint (
  IN EFI_MENU_ITEM           *MenuItem,
  IN VOID                    *Context,
  IN UINTN                   Row,
  IN UINTN                   Column
  )
/*++

Routine Description:

  the default MenuItem description information displaying function.

Arguments:

  MenuItem    - The EFI_MENU_ITEM instance pointer.
  Context     - the EFI_MENU_PAGE instance pointer.
  Row         - The beginning Row on Output Console to display the
                description message.
  Column      - The beginning Column on Output Console to display the
                description message.

Returns:

  EFI_SUCCESS - the keys was processed successfully.
  other       - error condition.

--*/
{
  INTN              Attrib;
  EFI_STATUS        Status;
  EFI_MENU_PAGE     *Page;
  UINTN             X0;
  UINTN             Y0;
  UINTN             X1;
  UINTN             Y1;
  UINTN             Index;
  UINTN             BufLen;
  CHAR16            *Buffer;

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

  if (X0>X1||Y0>Y1) {
    return EFI_INVALID_PARAMETER;
  }

  Buffer = MenuItem->ItemDesc.Text;
  if (Buffer == NULL) {
    return EFI_SUCCESS;
  }
  if (MenuItem == Page->Body.CurrentSelected) {

    if (StrLen (Buffer) == 0 ) {
      return EFI_SUCCESS;
    } else {
      //
      //Display the string.
      //
      Status = ST->ConOut->SetAttribute (
                     ST->ConOut,
                     EFI_ITEM_DESC_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                     );
      if (EFI_ERROR(Status)) {
        return Status;
      }

      Status = DisplayMessage (X0, Y0, X1, Y1, Buffer);

      Status = ST->ConOut->SetAttribute (ST->ConOut, Attrib);
      return Status;
    }
  } else {
    if (StrLen (Buffer) == 0) {
      return EFI_SUCCESS;
    }
    //
    //clear the display area.
    //
    Status = ST->ConOut->SetAttribute (
                   ST->ConOut,
                   EFI_MENUPAGE_BODY_BGCOLOR >> 4 | EFI_MENUPAGE_BODY_BGCOLOR
                   );
    if (EFI_ERROR(Status)) {
      return Status;
    }
    BufLen = (X1 - X0 + 2) * sizeof(CHAR16);
    Status = BS->AllocatePool (
                   EfiBootServicesData,
                   BufLen,
                   (VOID *)&Buffer
                   );
    if (EFI_ERROR(Status)) {
      return EFI_OUT_OF_RESOURCES;
    }
    for (Index = 0; (Index < BufLen / 2 - 1); Index++) {
      Buffer[Index] = L' ';
    }
    Buffer[Index] = L'\0';

    for (Index = Y0; Index <= Y1; Index++) {

      Status = TestPrintAt (X0, Index, Buffer);
      if (EFI_ERROR(Status)) {
        BS->FreePool (Buffer);
        return Status;
      }
    }

    BS->FreePool (Buffer);
    //
    //done successfully return Status code EFI_SUCCESS
    //
    return EFI_SUCCESS;
  }
}

EFI_STATUS
DoIterEdit (
  IN EFI_MENU_PAGE        *MenuPage,
  IN OUT EFI_INPUT_KEY    *InKey
  )
/*++

Routine Description:

  if current selected menu item type is EFI_ITEM_CASE then
  this function will process the input key that can not be processed
  by the well-known key process function.

Arguments:

  MenuPage  - which menu page the input key was taking effect on.
  InKey     - the input key to be processed.

Returns:

  EFI_SUCCESS           - the key was processed in current edit successfully.
  EFI_INVALID_PARAMETER

--*/
{
  EFI_STATUS      Status;
  EFI_MENU_ITEM   *MenuItem;
  INTN            Ypos;
  INTN            XLeft;
  UINTN           X0;
  EFI_INPUT_KEY   Key;
  INTN            Index;
  BOOLEAN         WAITING;
  BOOLEAN         BufferChanged;
  CHAR16          Buffer[EFI_MAX_ITER_EDIT_LENGTH + 1];
  CHAR16          TempStr[EFI_MAX_ITER_EDIT_LENGTH + 1];
  INTN            XOffSetInBuffer;
  INTN            CurrentLength;
  UINT32          TempValue;

  //
  // if there was no item  return EFI_SUCCESS.
  //
  if (MenuPage == NULL || MenuPage->Body.ItemList == NULL||
      MenuPage->Body.CurrentSelected == NULL) {
    return EFI_SUCCESS;
  }
  //
  //initialize varibles
  //
  Status  = EFI_SUCCESS;
  WAITING = TRUE;

  TempStr[0] = L'\0';
  Index      = 0;

  //
  //caculate current selected item line offset
  //
  MenuItem = MenuPage->Body.FirstInDisplay;
  while (MenuItem != NULL && MenuItem != MenuPage->Body.CurrentSelected) {
    Index += 1 + MenuItem->Skip;
    MenuItem   = MenuItem->Next;
  }
  Index++;

  X0     = MenuPage->Body.BodyRect.TopLeft.Col;
  Ypos   = MenuPage->Body.SplitPoint.Row + Index;
  XLeft  = X0 + EFI_ITEM_TYPE_TAG_LENGTH + 2 + EFI_MAX_CASE_ITEM_NAME_LENGTH;

  CurrentLength = StrLen (MenuPage->Body.CurrentSelected->ItemValue);

  StrCpy (Buffer, MenuPage->Body.CurrentSelected->ItemValue);

  XOffSetInBuffer = 0;

  //
  //enable cursor
  //
  ST->ConOut->EnableCursor (ST->ConOut, TRUE);

  //
  //White color is better
  //
  ST->ConOut->SetAttribute (
              ST->ConOut,
              EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
              );
  ST->ConOut->SetCursorPosition (ST->ConOut, XLeft, Ypos);

  if (InKey != NULL && (InKey->ScanCode != 0 || InKey->UnicodeChar != 0)) {
    Key.ScanCode    = InKey->ScanCode;
    Key.UnicodeChar = InKey->UnicodeChar;
  } else {
    Key.ScanCode    = SCAN_NULL;
    Key.UnicodeChar = CHAR_NULL;
  }

  while (WAITING) {
    BufferChanged = FALSE;
    switch (Key.ScanCode) {
      case SCAN_F3:
        InKey->ScanCode    = Key.ScanCode;
        InKey->UnicodeChar = Key.UnicodeChar;

        //
        //White color is better
        //
        ST->ConOut->SetAttribute (
              ST->ConOut,
              EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
              );
        ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft,
                Ypos
                );
        WAITING = FALSE;
        break;

      case SCAN_ESC:
      case SCAN_UP:
      case SCAN_DOWN:
      case SCAN_F8:
      case SCAN_F9:
      case SCAN_PAGE_UP:
      case SCAN_PAGE_DOWN:
        //
        //save the string value into orginal buffer
        //

        StrCpy (
          MenuPage->Body.CurrentSelected->ItemValue,
          Buffer
          );
        for (Index = 0; Index < EFI_MAX_ITER_EDIT_LENGTH; Index++) {
          TempStr[Index] = Buffer[Index];
          if (Buffer[Index] == 0) {
            break;
          }
        }
        TempStr[Index] = L'\0';
        //
        //refresh edit area
        //
        ST->ConOut->SetAttribute (
                      ST->ConOut,
                      EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                      );
        Status = TestPrintAt (XLeft, Ypos, TempStr);
        if (EFI_ERROR (Status)) {
          return Status;
        }

        WAITING = FALSE;
        MenuItem = MenuPage->Body.CurrentSelected;
        if (((Key.ScanCode == SCAN_UP) && (MenuItem->Prev == NULL)) ||
            ((Key.ScanCode == SCAN_DOWN) && (MenuItem->Next == NULL))) {
          //
          //no need to call the UpFunc or DownFunc so don't return the scancode
          //
          InKey->ScanCode    = SCAN_NULL;
          InKey->UnicodeChar = CHAR_NULL;

          //
          //White color is better
          //
          ST->ConOut->SetAttribute (
              ST->ConOut,
              EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
              );
          ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft,
                Ypos
                );
        } else {
          InKey->ScanCode    = Key.ScanCode;
          InKey->UnicodeChar = Key.UnicodeChar;
        }
        break;

      case SCAN_LEFT:

          //
          //move cursor directly in screen and in buffer.
          //

          if (XOffSetInBuffer > 0) {
            XOffSetInBuffer--;
          }

          //
          //White color is better
          //
          ST->ConOut->SetAttribute (
              ST->ConOut,
              EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
              );
          ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft + XOffSetInBuffer,
                Ypos
                );

        break;

      case SCAN_RIGHT:

        if (XOffSetInBuffer < CurrentLength) {

            //
            //before the cursor in screen point to the LastInDisplay
            //move the cursor to next char.
            //
            if (XOffSetInBuffer < EFI_MAX_ITER_EDIT_LENGTH - 1) {
              XOffSetInBuffer++;
            }

            //
            //White color is better
            //
            ST->ConOut->SetAttribute (
                          ST->ConOut,
                          EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                          );
            ST->ConOut->SetCursorPosition (
                  ST->ConOut,
                  XLeft + XOffSetInBuffer,
                  Ypos
                  );
            break;

        }

        break;

      case SCAN_HOME:
        if (CurrentLength>0) {
          //
          //reset the position varibles
          //

          XOffSetInBuffer  = 0;

          //
          //White color is better
          //
          ST->ConOut->SetAttribute (
                    ST->ConOut,
                    EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                    );
          ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft + XOffSetInBuffer,
                Ypos
                );
        }
        break;

      case SCAN_END:
        if (CurrentLength > 0) {
          //
          //reset these position varibles.
          //

          XOffSetInBuffer = CurrentLength - 1;

          //
          //White color is better
          //
          ST->ConOut->SetAttribute (
                    ST->ConOut,
                    EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                    );
          ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft + XOffSetInBuffer,
                Ypos
                );
        }
        break;

      case SCAN_DELETE:
        if (Buffer[XOffSetInBuffer] != L'\0' ) {
          //
          //delete this char in buffer
          //
          for(Index = XOffSetInBuffer; Index < CurrentLength; Index++) {
            Buffer[Index] = Buffer[Index + 1];
          }

          //
          //first clear the edit area
          //
          for(Index = 0; Index < EFI_MAX_ITER_EDIT_LENGTH; Index++) {
            TempStr[Index] = L' ';
          }
          TempStr[Index] = L'\0';
          ST->ConOut->SetAttribute (
                ST->ConOut,
                EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                );
          ST->ConOut->EnableCursor (ST->ConOut, FALSE);
          Status = TestPrintAt (XLeft, Ypos, TempStr);
          if (EFI_ERROR (Status)) {
            return Status;
          }

          //
          //refresh the edit area
          //
          for (Index = 0; Index < EFI_MAX_ITER_EDIT_LENGTH; Index++) {
            TempStr[Index] = Buffer[Index];
          }
          TempStr[Index] = L'\0';
          ST->ConOut->SetAttribute (
                ST->ConOut,
                EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                );
          Status = TestPrintAt (XLeft, Ypos, TempStr);
          if (EFI_ERROR (Status)) {
            return Status;
          }

          //
          //White color is better
          //
          ST->ConOut->SetAttribute (
                    ST->ConOut,
                    EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                    );
          ST->ConOut->SetCursorPosition (
                ST->ConOut,
                XLeft + XOffSetInBuffer,
                Ypos
                );
          ST->ConOut->EnableCursor (ST->ConOut, TRUE);
          CurrentLength--;
          BufferChanged = TRUE;
        }
        break;

      case SCAN_NULL:
        switch (Key.UnicodeChar) {
          case L' ':
          case CHAR_CARRIAGE_RETURN:
            StrCpy (
              MenuPage->Body.CurrentSelected->ItemValue,
              Buffer
              );
            for (Index = 0; Index < EFI_MAX_ITER_EDIT_LENGTH; Index++) {
              TempStr[Index] = Buffer[Index];
              if (Buffer[Index] == 0) {
                break;
              }
            }
            TempStr[Index] = L'\0';
            //
            //refresh edit area
            //
            ST->ConOut->SetAttribute (
                          ST->ConOut,
                          EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                          );
            ST->ConOut->EnableCursor (ST->ConOut, FALSE);
            Status = TestPrintAt (XLeft, Ypos, TempStr);
            ST->ConOut->EnableCursor (ST->ConOut, TRUE);
            if (EFI_ERROR (Status)) {
              return Status;
            }

            WAITING = FALSE;
            InKey->ScanCode    = Key.ScanCode;
            InKey->UnicodeChar = Key.UnicodeChar;
            break;

          case CHAR_NULL:
            break;

          case CHAR_BACKSPACE:
            if (XOffSetInBuffer > 0) {
              //
              //remove the char before current pointed char in buffer.
              //
              for(Index = XOffSetInBuffer; Index <= CurrentLength; Index++) {
                Buffer[Index - 1] = Buffer[Index];
              }
              XOffSetInBuffer--;

              //
              //clear the Edit area
              //
              for(Index = 0; Index < EFI_MAX_ITER_EDIT_LENGTH; Index++) {
                TempStr[Index] = L' ';
              }
              TempStr[Index] = L'\0';
              ST->ConOut->SetAttribute (
                    ST->ConOut,
                    EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                    );
              ST->ConOut->EnableCursor (ST->ConOut, FALSE);
              Status = TestPrintAt (XLeft, Ypos, TempStr);
              if (EFI_ERROR (Status)) {
                return Status;
              }

              //
              //refresh the Edit area
              //
              for (Index = 0; Index < EFI_MAX_ITER_EDIT_LENGTH; Index++) {
                TempStr[Index] = Buffer[Index];
              }

              ST->ConOut->SetAttribute (
                    ST->ConOut,
                    EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                    );
              Status = TestPrintAt (XLeft, Ypos, TempStr);
              if (EFI_ERROR (Status)) {
                return Status;
              }

              //
              //White color is better
              //
              ST->ConOut->SetAttribute (
                        ST->ConOut,
                        EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                        );
              ST->ConOut->SetCursorPosition (
                    ST->ConOut,
                    XLeft + XOffSetInBuffer,
                    Ypos
                    );
              ST->ConOut->EnableCursor (ST->ConOut, TRUE);
              CurrentLength--;
              BufferChanged = TRUE;
            }
            break;

          case CHAR_LINEFEED:
            break;

          case CHAR_TAB:
            break;

          default:

            if (CurrentLength <= EFI_MAX_ITER_EDIT_LENGTH) {
              if (Key.UnicodeChar == L'|') {
                break;
              }
              if ((MenuPage->Body.CurrentSelected->ItemType & EFI_EDIT_SUB_TYPE_MASK)
                   == EFI_ITEM_EDIT_NUMBER) {
                //
                //if this edit sub type is EFI_ITEM_EDIT_NUBER then
                //skip the char out of '0' to '9'.
                //
                if (Key.UnicodeChar < L'0' || Key.UnicodeChar > L'9') {
                  break;
                }
              }
              if (Key.UnicodeChar < 0x20 || Key.UnicodeChar >= 0x7F || Key.UnicodeChar == L'|') {
                break;
              }
              //
              //add this input key into the Buffer and refresh the display
              //
              if (XOffSetInBuffer < EFI_MAX_ITER_EDIT_LENGTH) {
                //
                //the count of char increase one
                //
                if (Buffer[XOffSetInBuffer] == L'\0' ) {
                  CurrentLength++;
                }
                Buffer[XOffSetInBuffer] = Key.UnicodeChar;
                if (XOffSetInBuffer < EFI_MAX_ITER_EDIT_LENGTH - 1) {
                  XOffSetInBuffer++;
                }
                Buffer [CurrentLength] = '\0';

                //
                //refresh edit area
                //
                for (Index = 0; Index < EFI_MAX_ITER_EDIT_LENGTH; Index++) {
                  TempStr[Index] = Buffer[Index];
                }
                TempStr[Index] = L'\0';
                ST->ConOut->SetAttribute (
                      ST->ConOut,
                      EFI_MENUPAGE_BODY_BGCOLOR | EFI_ITEM_EDIT_FORECOLOR
                      );
                ST->ConOut->EnableCursor (ST->ConOut, FALSE);
                Status = TestPrintAt (XLeft, Ypos, TempStr);
                if (EFI_ERROR (Status)) {
                  return Status;
                }

                //
                //White color is better
                //
                ST->ConOut->SetAttribute (
                          ST->ConOut,
                          EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                          );
                ST->ConOut->SetCursorPosition (
                      ST->ConOut,
                      XLeft + XOffSetInBuffer,
                      Ypos
                      );
                ST->ConOut->EnableCursor (ST->ConOut, TRUE);
                BufferChanged = TRUE;
              }
            }
            break;

        }//end switch UnicodeChar
      default :
        break;

    }//end switch ScanCode.

    //
    //Update iteration number
    //
    if (BufferChanged) {
      StrCpy (
          MenuPage->Body.CurrentSelected->ItemValue,
          Buffer
          );
      TempValue = (UINT32) Atoi(Buffer);
      ST->ConOut->EnableCursor (ST->ConOut, FALSE);
      Status = UpdateIter (MenuPage, MenuItem, TempValue);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      //
      //White color is better
      //
      ST->ConOut->SetAttribute (
                ST->ConOut,
                EFI_MENUPAGE_ITEM_SELECTED_FORECOLOR | EFI_MENUPAGE_BODY_BGCOLOR
                );
      ST->ConOut->SetCursorPosition (
                    ST->ConOut,
                    XLeft + XOffSetInBuffer,
                    Ypos
                    );
      ST->ConOut->EnableCursor (ST->ConOut, TRUE);
    }

    if (WAITING) {

      Status = BS->WaitForEvent (1, &(ST->ConIn->WaitForKey), (UINTN *)&Index);
      if (!EFI_ERROR(Status)) {
        Status = ST->ConIn->ReadKeyStroke (ST->ConIn, &Key);
      }

      if ( EFI_ERROR(Status)) {
        break;
      }
    }
  }

  return Status;
}

INT32
CompareNumberString (
  IN CHAR16               *str1,
  IN CHAR16               *str2
  )
{
  UINTN Index;

  ASSERT ((str1 != 0) && (str2 != 0));

  //
  // skip preceeding 0
  //
  while (*str1 == '0') {
    str1 += 1;
  }
  while (*str2 == '0') {
    str2 += 1;
  }

  //
  //Compare length first
  //
  if (StrLen (str1) < StrLen (str2)) {
    return -1;
  } else if (StrLen (str1) > StrLen (str2)) {
    return 1;
  }

  for (Index = 0; Index < StrLen (str1); Index ++) {
    if (str1[Index] < str2[Index]) {
      return -1;
    }
    if (str1[Index] > str2[Index]) {
      return 1;
    }
  }

  return 0;
}
