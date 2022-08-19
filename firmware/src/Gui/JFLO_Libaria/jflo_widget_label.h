/*******************************************************************************
 Module for Microchip Graphics Library - Aria User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    libaria_widget_label.h

  Summary:
    

  Description:
    This module implements label (text) widget functions.
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
#ifndef JFLO_LABEL_H
#define JFLO_LABEL_H
//DOM-IGNORE-END

#include "gfx/libaria/inc/libaria_common.h"

#if JFLO_LABEL_WIDGET_ENABLED

#include "gfx/libaria/inc/libaria_widget.h"
#include "gfx/libaria/inc/libaria_string.h"

typedef struct jfloLabelWidget_t jfloLabelWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Structure:
    jfloLabelWidget_t

  Summary:
    Implementation of a label widget struct

  Description:
    A label widget is a simple widget that draws a string of text.

  Remarks:
    None.
*/
typedef struct jfloLabelWidget_t
{
    laWidget widget; // widget base class

    laString text; // string to draw

    laHAlignment halign; // horizontal alignment of string
    laVAlignment valign; // vertical alignment of string

    GFXU_ExternalAssetReader* reader; // asset reader
} jfloLabelWidget;

void _jfloLabelWidget_Constructor(jfloLabelWidget* lbl);
void _jfloLabelWidget_Destructor(jfloLabelWidget* lbl);

void _jfloLabelWidget_Paint(jfloLabelWidget* lbl);


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    jfloLabelWidget* jfloLabelWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:
    

  Parameters:
    
  Returns:
    jfloLabelWidget*
    
  Remarks:
    
*/
LIB_EXPORT jfloLabelWidget* jfloLabelWidget_New();

// *****************************************************************************
/* Function:
    laResult jfloLabelWidget_GetText(jfloLabelWidget* lbl, laString* str)

  Summary:
    Gets the text value for the label.

  Description:
    This function allocates memory and initializes the input string pointer.  The
    caller is responsible for managing the memory once this function returns.

  Parameters:
    jfloLabelWidget* lbl - the widget
    laString* str - a pointer to an laString object
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLabelWidget_GetText(jfloLabelWidget* lbl, laString* str);

// *****************************************************************************
/* Function:
    laResult jfloLabelWidget_SetText(jfloLabelWidget* lbl, laString str)

  Summary:
    Sets the text value for the label.

  Description:
    This function copies the contents of the input string into its internal
    string buffer.  The input string can then be freed or altered without 
    affecting the label's internal string value.

  Parameters:
    jfloLabelWidget* lbl - the widget
    laString str - an laString object
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLabelWidget_SetText(jfloLabelWidget* lbl, laString str);

// *****************************************************************************
/* Function:
    laHAlignment jfloLabelWidget_GetHAlignment(jfloLabelWidget* lbl)

  Summary:
    Gets the text horizontal alignment value.

  Description:
    

  Parameters:
    jfloLabelWidget* - the widget
        
  Returns:
    laHAlignment - the horizontal alignment value
    
  Remarks:
    
*/
LIB_EXPORT laHAlignment jfloLabelWidget_GetHAlignment(jfloLabelWidget* lbl);

// *****************************************************************************
/* Function:
    laResult jfloLabelWidget_SetHAlignment(jfloLabelWidget* lbl,
                                         laHAlignment align)

  Summary:
    Sets the text horizontal alignment value

  Description:
    

  Parameters:
    jfloLabelWidget* - the widget
    laHAlignment align - the horizontal alignment value
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLabelWidget_SetHAlignment(jfloLabelWidget* lbl,
                                                laHAlignment align);

// *****************************************************************************
/* Function:
    laVAlignment jfloLabelWidget_GetVAlignment(jfloLabelWidget* lbl)

  Summary:
    Gets the current vertical text alignment

  Description:
    

  Parameters:
    jfloLabelWidget* - the widget
        
  Returns:
    laVAlignment - the vertical alignment setting
    
  Remarks:
    
*/
LIB_EXPORT laVAlignment jfloLabelWidget_GetVAlignment(jfloLabelWidget* lbl);

// *****************************************************************************
/* Function:
    laResult jfloLabelWidget_SetVAlignment(jfloLabelWidget* lbl,
                                         laVAlignment align)

  Summary:
    Sets the vertical text alignment value

  Description:
    

  Parameters:
    jfloLabelWidget* - the widget
    laVAlignment align - the vertical alignment setting
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLabelWidget_SetVAlignment(jfloLabelWidget* lbl,
                                                laVAlignment align);


// internal use only                                                
void _jfloLabelWidget_GetTextRect(jfloLabelWidget* lbl,
                                GFX_Rect* textRect,
                                GFX_Rect* drawRect);                                                                                                

#endif // LA_LABEL_WIDGET_ENABLED
#endif /* LIBARIA_LABEL_H */