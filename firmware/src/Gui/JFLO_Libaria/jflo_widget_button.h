/*******************************************************************************
 Module for Microchip Graphics Library - Aria User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    libaria_widget_button.h

  Summary:
    Defines a button widget
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
#ifndef JFLO_BUTTON_1_H
#define JFLO_BUTTON_1_H
//DOM-IGNORE-END

#include "gfx/libaria/inc/libaria_common.h"

#if JFLO_BUTTON_WIDGET_ENABLED

#include "gfx/libaria/inc/libaria_widget.h"
#include "gfx/libaria/inc/libaria_string.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    jfloButtonState_t

  Summary:
    Controls the button pressed state
*/

typedef enum jfloButtonState_t
{
    JFLO_BUTTON_STATE_UP,
    JFLO_BUTTON_STATE_DOWN,
    JFLO_BUTTON_STATE_TOGGLED
} jfloButtonState;

typedef struct jfloButtonWidget_t jfloButtonWidget;

typedef void (*jfloButtonWidget_PressedEvent)(jfloButtonWidget*);
typedef void (*jfloButtonWidget_ReleasedEvent)(jfloButtonWidget*);

// *****************************************************************************
/* Structure:
    jfloButtonWidget_t

  Summary:
    Implementation of a button widget.  A button is an interactive element
    that simulates a typical button with a pressed an released state.

  Description:
    

  Remarks:
    None.
*/
typedef struct jfloButtonWidget_t
{
    laWidget widget;  // base widget header

    jfloButtonState state; // button state
    uint8_t toggleable; // indicates if the button is toggleable

    laString text; // the string that holds the button text

    laHAlignment halign; // horizontal alignment of the button
    laVAlignment valign; // vertical alignment of the button

    GFXU_ImageAsset* pressedImage; // button pressed icon image
    GFXU_ImageAsset* releasedImage; // button released icon image
    
    laRelativePosition imagePosition; // icon position in relation to text
    uint32_t imageMargin; // distance between text and icon
    int32_t pressedOffset; // pressed text offset distance

    jfloButtonWidget_PressedEvent pressedEvent; // pressed event callback
    jfloButtonWidget_ReleasedEvent releasedEvent; // released event callback

    GFXU_ExternalAssetReader* reader; // external asset reader state
    
    uint32_t arcThickness; //the thickness of the arc
    uint32_t arcAngle;
    
    
    int32_t x; // the origin x cooridnate
    int32_t y; // the origin y coordinate
} jfloButtonWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************


void _jfloButtonWidget_Constructor(jfloButtonWidget* btn);
void _jfloButtonWidget_Destructor(jfloButtonWidget* btn);

void _jfloButtonWidget_SetCornerRadius(jfloButtonWidget* btn, uint32_t radius);

void _jfloButtonWidget_Paint(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    jfloButtonWidget* jfloButtonWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the managment of this memory until the
    widget is added to a widget tree.

  Description:
    Creates a new button widget instance.  Invokes the button constructor

  Returns:
    jfloButtonWidget* - pointer to a new button widget instance
    
  Remarks:
    Caller is responsible for managing the memory allocated by this function
    until the widget is added to a valid widget tree.
*/
LIB_EXPORT jfloButtonWidget* jfloButtonWidget_New();

// *****************************************************************************
/* Function:
    laBool jfloButtonWidget_GetToggleable(jfloButtonWidget* btn)

  Summary:
    Gets the value of this button's toggle flag 

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to reference
    
  Returns:
    laBool - the value of the toggle flag
    
*/
LIB_EXPORT laBool jfloButtonWidget_GetToggleable(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetToggleable(jfloButtonWidget* btn,
                                      laBool toggleable)

  Summary:
    Enables the toggle mode for a button.  When pressed, toggle buttons will
    stay down until pressed again.

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to modify
    laBool toggleable - the desired togglestate
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloButtonWidget_SetToggleable(jfloButtonWidget* btn,
                                                 laBool toggleable);
                                             
// *****************************************************************************
/* Function:
    laBool jfloButtonWidget_GetPressed(jfloButtonWidget* btn)

  Summary:
    Gets the pressed state of a button

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to reference
        
  Returns:
    laBool - the button pressed state
    
  Remarks:
    
*/                                          
LIB_EXPORT laBool jfloButtonWidget_GetPressed(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetPressed(jfloButtonWidget* btn, laBool pressed)

  Summary:
    Sets the pressed state for a button.

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to modify
    laBool pressed - the pressed state
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_SetPressed(jfloButtonWidget* btn, 
                                              laBool pressed);                                    

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_GetText(jfloButtonWidget* btn, laString* str)

  Summary:
    Gets the text for a button.  If the button's string has local data then
    a duplicate of the string will be allocated.  The caller is responsible
    for managing the memory for the duplicated string.  If the button string
    is a string table reference then only the reference ID is copied.

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to reference
    laString* str - pointer to a string to copy the button string into
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_GetText(jfloButtonWidget* btn, laString* str);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetText(jfloButtonWidget* btn, laString str)

  Summary:
    Sets the text for a button.  If the input string has local data then
    the data will be copied into the button's local string, causing a memory
    allocation.  If the input string is a string table reference then only the
    reference will be copied.  The input string can be safely modified and
    the button string will not be affected.  
    
  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to modify
    laString str - the string to set to the button
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_SetText(jfloButtonWidget* btn, laString str);

// *****************************************************************************
/* Function:
    laHAlignment jfloButtonWidget_GetHAlignment(jfloButtonWidget* btn)

  Summary:
    Gets the horizontal alignment setting for a button

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to reference
        
  Returns:
    laHAlignment - the horizontal alignment value
    
  Remarks:
    
*/    
LIB_EXPORT laHAlignment jfloButtonWidget_GetHAlignment(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetHAlignment(jfloButtonWidget* btn,
                                             laHAlignment align)

  Summary:
    Sets the horizontal alignment value for a button

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to modify
    laHAlignment align - the desired alignment value
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_SetHAlignment(jfloButtonWidget* btn,
                                                 laHAlignment align);
                                             
// *****************************************************************************
/* Function:
    laVAlignment jfloButtonWidget_GetVAlignment(jfloButtonWidget* btn)

  Summary:
    Gets the vertical alignment setting for a button

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to reference
        
  Returns:
    laVAlignment - the vertical alignment setting for the button
    
  Remarks:
    
*/    
LIB_EXPORT laVAlignment jfloButtonWidget_GetVAlignment(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetVAlignment(jfloButtonWidget* btn,
                                      laVAlignment align)

  Summary:
    Sets the vertical alignment for a button

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the btn to modify
    laVAlignment align - the desired vertical alignment setting
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_SetVAlignment(jfloButtonWidget* btn,
                                                 laVAlignment align);                                                    

// *****************************************************************************
/* Function:
    GFXU_ImageAsset* jfloButtonWidget_GetPressedImage(jfloButtonWidget* btn)

  Summary:
    Gets the pressed image asset pointer for a button

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to reference
        
  Returns:
    GFXU_ImageAsset* - the pressed asset pointer
    
  Remarks:
    
*/    
LIB_EXPORT GFXU_ImageAsset* jfloButtonWidget_GetPressedImage(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetPressedImage(jfloButtonWidget* btn,
                                        GFXU_ImageAsset* img)

  Summary:
    Sets the image to be used as a pressed icon

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the widget
    GFXU_ImageAsset* img - pointer to an image asset
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_SetPressedImage(jfloButtonWidget* btn,
                                                   GFXU_ImageAsset* img);

// *****************************************************************************
/* Function:
    GFXU_ImageAsset* jfloButtonWidget_GetReleasedImage(jfloButtonWidget* btn)

  Summary:
    Gets the currently used released icon

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to reference
        
  Returns:
    GFXU_ImageAsset* - the released asset pointer
    
  Remarks:
    
*/    
LIB_EXPORT GFXU_ImageAsset* jfloButtonWidget_GetReleasedImage(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetReleasedImage(jfloButtonWidget* btn,
                                             GFXU_ImageAsset* img)

  Summary:
    Sets the image to be used as the released icon

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the widget
    GFXU_ImageAsset* img - the image asset to be used
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_SetReleasedImage(jfloButtonWidget* btn,
                                                    GFXU_ImageAsset* img);
                                                
// *****************************************************************************
/* Function:
    laRelativePosition jfloButtonWidget_GetImagePosition(jfloButtonWidget* btn)

  Summary:
    Gets the position of the button icon

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the button to reference
        
  Returns:
    laRelativePosition
    
  Remarks:
    
*/    
LIB_EXPORT laRelativePosition jfloButtonWidget_GetImagePosition(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetImagePosition(jfloButtonWidget* btn,
                                             laRelativePosition pos)

  Summary:
    Sets the position of the button icon

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the widget
    laRelativePosition pos - the desired image position
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_SetImagePosition(jfloButtonWidget* btn,
                                                    laRelativePosition pos);

// *****************************************************************************
/* Function:
    uint32_t jfloButtonWidget_GetImageMargin(jfloButtonWidget* btn)

  Summary:
    Gets the distance between the icon and the text

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the widget
        
  Returns:
    uint32_t - the distance value
    
  Remarks:
    
*/    
LIB_EXPORT uint32_t jfloButtonWidget_GetImageMargin(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetImageMargin(jfloButtonWidget* btn, 
                                           uint32_t mg)

  Summary:
    Sets the distance between the icon and text

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the widget
    uint32_t - the distance value
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_SetImageMargin(jfloButtonWidget* btn,
                                                  uint32_t mg);

// *****************************************************************************
/* Function:
    int32_t jfloButtonWidget_GetPressedOffset(jfloButtonWidget* btn)

  Summary:
    Gets the offset of the button internals when pressed

  Description:
    
    
  Parameters:
    jfloButtonWidget* btn - the widget
        
  Returns:
    int32_t - the distance value
    
  Remarks:
    
*/    
LIB_EXPORT int32_t jfloButtonWidget_GetPressedOffset(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetPressedOffset(jfloButtonWidget* btn, int32_t offs)

  Summary:
    Sets the offset of the button internals when pressed

  Description:
    This value will be applied to all of the contents of the button 
    when it is pressed.  This helps to visualize the button being
    pressed.
    
  Parameters:
    jfloButtonWidget* btn - the widget
    int32_t - the distance value
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_SetPressedOffset(jfloButtonWidget* btn,
                                                    int32_t offs);

// *****************************************************************************
/* Function:
    jfloButtonWidget_PressedEvent jfloButtonWidget_GetPressedEventCallback(jfloButtonWidget* btn)

  Summary:
    Gets the callback associated with the button pressed event

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the widget
        
  Returns:
    jfloButtonWidget_PressedEvent
    
  Remarks:
    
*/    
LIB_EXPORT jfloButtonWidget_PressedEvent jfloButtonWidget_GetPressedEventCallback(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetPressedEventCallback(jfloButtonWidget* btn,
                                                    jfloButtonWidget_PressedEvent cb)

  Summary:
    Sets the pressed event callback for the button

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the widget
    jfloButtonWidget_PressedEvent cb - a valid callback pointer or NULL
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_SetPressedEventCallback(jfloButtonWidget* btn,
                                                           jfloButtonWidget_PressedEvent cb);
// *****************************************************************************
/* Function:
    jfloButtonWidget_ReleasedEvent jfloButtonWidget_GetReleasedEventCallback(jfloButtonWidget* btn)
    
  Summary:
    Gets the callback for the button released event

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the widget
        
  Returns:
    jfloButtonWidget_ReleasedEvent
    
  Remarks:
    
*/    
LIB_EXPORT jfloButtonWidget_ReleasedEvent jfloButtonWidget_GetReleasedEventCallback(jfloButtonWidget* btn);

// *****************************************************************************
/* Function:
    laResult jfloButtonWidget_SetReleasedEventCallback(jfloButtonWidget* btn,
                                                     jfloButtonWidget_ReleasedEvent cb)

  Summary:
    Sets the callback for the button released event

  Description:
    
  Parameters:
    jfloButtonWidget* btn - the widget
    jfloButtonWidget_ReleasedEvent cb - a valid callback pointer or NULL
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/    
LIB_EXPORT laResult jfloButtonWidget_SetReleasedEventCallback(jfloButtonWidget* btn,
                                                            jfloButtonWidget_ReleasedEvent cb);

// internal use only
void _jfloButtonWidget_GetImageRect(jfloButtonWidget* btn,
                                  GFX_Rect* imgRect,
                                  GFX_Rect* imgSrcRect);
                                  
void _jfloButtonWidget_GetTextRect(jfloButtonWidget* btn,
                                 GFX_Rect* textRect,
                                 GFX_Rect* drawRect);
                                 
void _jfloButtonWidget_InvalidateBorderAreas(jfloButtonWidget* btn);

/** New function for BUTTON*/

uint32_t jfloButtonWidget_GetThickness(jfloButtonWidget* btn);
//
laResult jfloButtonWidget_SetThickness(jfloButtonWidget* btn, uint32_t thickness);

void jfloReclip(jfloButtonWidget* btn);

/****************************/


#endif /* LA_BUTTON_WIDGET_ENABLED */
#endif /* LIBARIA_BUTTON_H */

