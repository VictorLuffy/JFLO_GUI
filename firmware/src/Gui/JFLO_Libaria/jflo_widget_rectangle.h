#ifndef JFLO_RECTANGLE_H
#define JFLO_RECTANGLE_H
//DOM-IGNORE-END

#include "gfx/libaria/inc/libaria_common.h"

#if LA_RECTANGLE_WIDGET_ENABLED

#include "gfx/libaria/inc/libaria_widget.h"

typedef struct jfloRectangleWidget_t jfloRectangleWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    jfloRectangleWidget_t

  Summary:
    Implementation of a rectangle widget struct

  Description:
    A rectangle widget draws a basic rectangle of a given thickness using the
    widget's bounding box as the dimensions.

  Remarks:
    None.
*/
typedef struct jfloRectangleWidget_t
{
    laWidget widget; // widget base class

    int32_t thickness; // rectangle border thickness
} jfloRectangleWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    jfloRectangleWidget* jfloRectangleWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:
    

  Parameters:
    
  Returns:
    jfloRectangleWidget*
    
  Remarks:
    
*/
LIB_EXPORT jfloRectangleWidget* jfloRectangleWidget_New();

// *****************************************************************************
/* Function:
    int32_t jfloRectangleWidget_GetThickness(jfloRectangleWidget* rect)

  Summary:
    Gets the rectangle border thickness setting

  Description:
    

  Parameters:
    jfloRectangleWidget* rect - the widget
    
  Returns:
    int32_t - the border thickness setting
    
  Remarks:
    
*/
LIB_EXPORT int32_t jfloRectangleWidget_GetThickness(jfloRectangleWidget* rect);

// *****************************************************************************
/* Function:
    laResult jfloRectangleWidget_SetThickness(jfloRectangleWidget* rect,
                                            int32_t thk)

  Summary:
    Sets the rectangle border thickness setting

  Description:
    

  Parameters:
    jfloRectangleWidget* rect - the widget
    int32_t thk - the thickness setting
    
  Returns:
    laResult - the operation result
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloRectangleWidget_SetThickness(jfloRectangleWidget* rect,
                                                   int32_t thk);

// internal use only
void _jfloRectangleWidget_InvalidateRect(jfloRectangleWidget* rct);

#endif // LA_RECTANGLE_WIDGET_ENABLED
#endif /* LIBARIA_RECTANGLE_H */