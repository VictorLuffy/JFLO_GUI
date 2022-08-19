/*******************************************************************************
 Module for Microchip Graphics Library - Aria User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    libaria_widget_line_graph.h

  Summary:
    

  Description:
    This module implements line graph drawing widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2018 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END

#ifndef JFLO_WIDGET_LINE_GRAPH_H
#define JFLO_WIDGET_LINE_GRAPH_H

#include "gfx/libaria/inc/libaria_common.h"
#include "gfx/libaria/inc/libaria_string.h"

#if JFLO_LINE_GRAPH_WIDGET_ENABLED

#include "gfx/libaria/inc/libaria_widget.h"
#include "gfx/libaria/inc/libaria_utils.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

typedef struct jfloLineGraphDataSeries_t
{
    laScheme * scheme;
    laArray data;
    laLineGraphValueAxis axis;
    laLineGraphDataPointType pointType;
    uint32_t pointSize;
    laBool fillPoints;
    laBool drawLines;
} jfloLineGraphDataSeries;

typedef struct jfloLineGraphCategory_t
{
    laString text;
    int32_t stackValue;
} jfloLineGraphCategory;

// *****************************************************************************
/* Structure:
    jfloLineGraphWidget_t

  Summary:
    Implementation of a line graph widget.

  Description:
    A line graph widget draws a line graph. All coordinates are expressed in local widget space.

    The color of the graph is determined by the widget scheme's 'foreground'
    color.

  Remarks:
    None.
*/
typedef struct jfloLineGraphWidget_t
{
    laWidget widget; // base widget header

    uint32_t tickLength;
    laBool fillGraphArea;
    laBool fillValueArea;
    laBool clearPlottingArea;               // Hiep custom

    //Value axis properties
    int32_t maxValue;
    int32_t minValue;
    int32_t indexNextValueToDraw;           // Hiep custom
    uint32_t tickInterval;
    uint32_t subtickInterval;
    laBool valueAxisLabelsVisible;
    laBool valueAxisTicksVisible;
    laBool valueAxisSubticksVisible;
    laBool valueGridlinesVisible;
    laBool stacked;
    laArray dataSeries;
    GFXU_StringTableAsset * stringTable; //string table
    uint32_t ticksLabelsStringID; //ID of Superset string containing numbers
    laLineGraphTickPosition valueAxisTicksPosition;
    laLineGraphTickPosition valueAxisSubticksPosition;
    
    //Category axis properties
    laBool categAxisLabelsVisible;
    laBool categAxisTicksVisible;
    laLineGraphTickPosition categAxisTicksPosition;
    laArray categories;
    
    GFX_Point valuePoint;               // Hiep cústom      if we have dataSeries > 1 --> have to defin valuePoint1, topvalue1,...
    int32_t topValue;
    int32_t stackValue;
    
    GFXU_ExternalAssetReader* reader; // asset reader
} jfloLineGraphWidget;

void _jfloLineGraphWidget_Constructor(jfloLineGraphWidget* graph);
void _jfloLineGraphWidget_Destructor(jfloLineGraphWidget* graph);

void _jfloLineGraphWidget_Paint(jfloLineGraphWidget* graph);

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    jfloLineGraphWidget* jfloLineGraphWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the management of this memory until the
    widget is added to a widget tree.

  Description:
  
  Parameters:
    void
    
  Returns:
    jfloLineGraphWidget*
    
  Remarks:
    
*/
LIB_EXPORT jfloLineGraphWidget* jfloLineGraphWidget_New();

// *****************************************************************************
/* Function:
    uint32_t jfloLineGraphWidget_GetTickLength(jfloLineGraphWidget* graph)

  Summary:
    Returns the length of the ticks

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    uint32_t - tick length
    
  Remarks:
    
*/
LIB_EXPORT uint32_t jfloLineGraphWidget_GetTickLength(jfloLineGraphWidget* graph);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetTickLength(jfloLineGraphWidget* graph, uint32_t length)

  Summary:
    Sets the length of the ticks

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    uint32_t length - length in pixels
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetTickLength(jfloLineGraphWidget* graph, uint32_t length);

// *****************************************************************************
/* Function:
    uint32_t jfloLineGraphWidget_GetMaxValue(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis)

  Summary:
    Returns the max value of the axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    uint32_t - max value
    
  Remarks:
    
*/
LIB_EXPORT uint32_t jfloLineGraphWidget_GetMaxValue(jfloLineGraphWidget* graph, laLineGraphValueAxis axis);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetMaxValue(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis, int32_t value)

  Summary:
    Sets the max value of the axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    int32_t value - max value
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetMaxValue(jfloLineGraphWidget* graph, laLineGraphValueAxis axis, int32_t value);

// *****************************************************************************
/* Function:
    uint32_t jfloLineGraphWidget_GetMinValue(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis)

  Summary:
    Returns the min value of the axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    uint32_t - min value
    
  Remarks:
    
*/
LIB_EXPORT uint32_t jfloLineGraphWidget_GetMinValue(jfloLineGraphWidget* graph, laLineGraphValueAxis axis);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetMinValue(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis, int32_t value)

  Summary:
    Sets the min value of the axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    int32_t value - min value
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetMinValue(jfloLineGraphWidget* graph, laLineGraphValueAxis axis, int32_t value);

// *****************************************************************************
/* Function:
    laBool jfloLineGraphWidget_GetValueAxisLabelsVisible(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis)

  Summary:
    Returns GFX_TRUE if the value axis labels are visible

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laBool - GFX_TRUE if the value axis labels are visible
    
  Remarks:
    
*/
LIB_EXPORT laBool jfloLineGraphWidget_GetValueAxisLabelsVisible(jfloLineGraphWidget* graph, laLineGraphValueAxis axis);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetValueAxisLabelsVisible(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis, laBool visible)

  Summary:
    Shows/Hides the labels in the value axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    laBool visible - shows the labels if GFX_TRUE
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetValueAxisLabelsVisible(jfloLineGraphWidget* graph, laLineGraphValueAxis axis, laBool visible);

// *****************************************************************************
/* Function:
    laBool jfloLineGraphWidget_GetValueAxisTicksVisible(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis)

  Summary:
    Returns GFX_TRUE if the value axis ticks are visible

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laBool - GFX_TRUE if the value axis ticks are visible
    
  Remarks:
    
*/
LIB_EXPORT laBool jfloLineGraphWidget_GetValueAxisTicksVisible(jfloLineGraphWidget* graph, laLineGraphValueAxis axis);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetValueAxisTicksVisible(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis, laBool visible)

  Summary:
    Shows/Hides the ticks in the value axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    laBool visible - shows the ticks if GFX_TRUE
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetValueAxisTicksVisible(jfloLineGraphWidget* graph, laLineGraphValueAxis axis, laBool visible);

// *****************************************************************************
/* Function:
    laBool jfloLineGraphWidget_GetValueAxisSubticksVisible(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis)

  Summary:
    Returns GFX_TRUE if the value axis subticks are visible

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laBool - GFX_TRUE if the value axis subticks are visible
    
  Remarks:
    
*/
LIB_EXPORT laBool jfloLineGraphWidget_GetValueAxisSubticksVisible(jfloLineGraphWidget* graph, laLineGraphValueAxis axis);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetValueAxisSubticksVisible(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis, laBool visible)

  Summary:
    Shows/Hides the subticks in the value axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    laBool visible - shows the subticks if GFX_TRUE
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetValueAxisSubticksVisible(jfloLineGraphWidget* graph, laLineGraphValueAxis axis, laBool visible);

// *****************************************************************************
/* Function:
    uint32_t jfloLineGraphWidget_GetValueAxisTickInterval(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis)

  Summary:
    Returns the interval between major ticks in the value axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    
  Returns:
    uint32_t - ticks in pixels
    
  Remarks:
    
*/
LIB_EXPORT uint32_t jfloLineGraphWidget_GetValueAxisTickInterval(jfloLineGraphWidget* graph, laLineGraphValueAxis axis);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetValueAxisTickInterval(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis, uint32_t interval)

  Summary:
    Sets the tick interval in the value axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    uint32_t interval - tick interval in pixels
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetValueAxisTickInterval(jfloLineGraphWidget* graph, laLineGraphValueAxis axis, uint32_t interval);

// *****************************************************************************
/* Function:
    uint32_t jfloLineGraphWidget_GetValueAxisSubtickInterval(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis)

  Summary:
    Returns the interval between minor ticks in the value axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    
  Returns:
    uint32_t - ticks in pixels
    
  Remarks:
    
*/
LIB_EXPORT uint32_t jfloLineGraphWidget_GetValueAxisSubtickInterval(jfloLineGraphWidget* graph, laLineGraphValueAxis axis);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetValueAxisSubtickInterval(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis, uint32_t interval)

  Summary:
    Sets the minor tick interval in the value axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    uint32_t interval - tick interval in pixels
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetValueAxisSubtickInterval(jfloLineGraphWidget* graph, laLineGraphValueAxis axis, uint32_t interval);



// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_AddSeries(jfloLineGraphWidget* graph, uint32_t * seriesID)

  Summary:
    Adds a series to the graph

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    uint32_t * seriesID - destination of the returned series ID
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_AddSeries(jfloLineGraphWidget* graph, uint32_t * seriesID);

// *****************************************************************************
/* Function:
    laScheme * jfloLineGraphWidget_GetSeriesScheme(jfloLineGraphWidget* graph, uint32_t seriesID)

  Summary:
    Returns scheme of the specified series

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laScheme * - scheme of the specified series
    
  Remarks:
    
*/
LIB_EXPORT laScheme * jfloLineGraphWidget_GetSeriesScheme(jfloLineGraphWidget* graph, uint32_t seriesID);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetSeriesScheme(jfloLineGraphWidget* graph, uint32_t seriesID, laScheme * scheme)

  Summary:
    Sets the color scheme of the series

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    int32_t seriesID - the series ID, if negative the last series is referenced
    laScheme * scheme - the color scheme
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetSeriesScheme(jfloLineGraphWidget* graph, int32_t seriesID, laScheme * scheme);

// *****************************************************************************
/* Function:
   laBool jfloLineGraphWidget_GetSeriesFillPoints(jfloLineGraphWidget* graph, uint32_t seriesID)

  Summary:
    Returns GFX_TRUE if the series points are filled

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laBool - GFX_TRUE if the series points are filled
    
  Remarks:
    
*/
LIB_EXPORT laBool jfloLineGraphWidget_GetSeriesFillPoints(jfloLineGraphWidget* graph, uint32_t seriesID);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetSeriesFillPoints(jfloLineGraphWidget* graph, int32_t seriesID, laBool fill)

  Summary:
    Sets the series points filled

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    int32_t seriesID - the series ID, if negative the last series is referenced
    laBool fill - fills the points if GFX_TRUE
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetSeriesFillPoints(jfloLineGraphWidget* graph, int32_t seriesID, laBool fill);

// *****************************************************************************
/* Function:
   laBool jfloLineGraphWidget_GetSeriesLinesVisible(jfloLineGraphWidget* graph, uint32_t seriesID)

  Summary:
    Returns GFX_TRUE if the series lines are visible

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laBool - GFX_TRUE if the series lines are visible
    
  Remarks:
    
*/
LIB_EXPORT laBool jfloLineGraphWidget_GetSeriesLinesVisible(jfloLineGraphWidget* graph, uint32_t seriesID);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetSeriesLinesVisible(jfloLineGraphWidget* graph, int32_t seriesID, laBool visible)

  Summary:
    Shows/hides the lines between series points

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    int32_t seriesID - the series ID, if negative the last series is referenced
    laBool fill - Shows the lines between series data points if GFX_TRUE
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetSeriesLinesVisible(jfloLineGraphWidget* graph, int32_t seriesID, laBool visible);

// *****************************************************************************
/* Function:
   jfloLineGraphDataPointType jfloLineGraphWidget_GetSeriesPointType(jfloLineGraphWidget* graph, uint32_t seriesID)

  Summary:
    Returns the type of point drawn for the series data points

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    uint32_t seriesID - the series ID,
    
  Returns:
    jfloLineGraphDataPointType - the point type
    
  Remarks:
    
*/
LIB_EXPORT laLineGraphDataPointType jfloLineGraphWidget_GetSeriesPointType(jfloLineGraphWidget* graph, uint32_t seriesID);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetSeriesPointType(jfloLineGraphWidget* graph, int32_t seriesID, jfloLineGraphDataPointType type)

  Summary:
    Sets the type of point drawn for the series data points

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    int32_t seriesID - the series ID, if negative the last series is referenced
    jfloLineGraphDataPointType type - point type
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetSeriesPointType(jfloLineGraphWidget* graph, int32_t seriesID, laLineGraphDataPointType type);

// *****************************************************************************
/* Function:
    uint32_t jfloLineGraphWidget_GetSeriesPointSize(jfloLineGraphWidget* graph, uint32_t seriesID)

  Summary:
    Returns the size of the drawn point

  Description:
    For circular points, this value is the radius
    For square points, the length of each side is twice the value
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    uint32_t seriesID - the series ID
    
  Returns:
    uint32_t - the point size
    
  Remarks:
    
*/
LIB_EXPORT uint32_t jfloLineGraphWidget_GetSeriesPointSize(jfloLineGraphWidget* graph, uint32_t seriesID);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetSeriesPointSize(jfloLineGraphWidget* graph, int32_t seriesID, uint32_t size)

  Summary:
    Sets the size of the point drawn for the series data

  Description:
    For circular points, this value sets the radius
    For square points, the length of each side is twice the value
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    int32_t seriesID - the series ID, if negative the last series is referenced
    uint32_t size - size in pixels
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetSeriesPointSize(jfloLineGraphWidget* graph, int32_t seriesID, uint32_t size);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_AddDataToSeries(jfloLineGraphWidget* graph, uint32_t seriesID, uint32_t categoryID, int32_t value)

  Summary:
    Adds a data (value) to the specified series at categoryID index

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    uint32_t seriesID - the series ID
    int32_t value - the value
    uint32_t * index - the destination to return the index of the added data
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_AddDataToSeries(jfloLineGraphWidget* graph, uint32_t seriesID, int32_t value, uint32_t * index);


LIB_EXPORT laResult jfloLineGraphWidget_RemoveDataToSeries(jfloLineGraphWidget* graph, uint32_t seriesID, uint32_t index);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetDataInSeries(jfloLineGraphWidget* graph, 
                                              uint32_t seriesID,
                                              uint32_t index,
                                              int32_t value);
  Summary:
    Sets the value of the entry in the series index. The entry should have
    been previously 

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    uint32_t seriesID - the series ID
    uint32_t index - the index of the data
    int32_t value - the value
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetDataInSeries(jfloLineGraphWidget* graph, 
                                          uint32_t seriesID,
                                          uint32_t index,
                                          int32_t value);


// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_DestroyAll(jfloLineGraphWidget* graph)

  Summary:
    Destroys data, series and categories and frees the memory allocated

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_DestroyAll(jfloLineGraphWidget* graph);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetStringTable(jfloLineGraphWidget* graph, GFXU_StringTableAsset * stringTable)

  Summary:
    Sets the string table used for the generated axis labels

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    GFXU_StringTableAsset * stringTable - the string table
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetStringTable(jfloLineGraphWidget* graph, GFXU_StringTableAsset * stringTable);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetTicksLabelsStringID(jfloLineGraphWidget* graph, uint32_t stringID)

  Summary:
    Sets the ID of the superset string used for the value axis tick labels

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    uint32_t stringID - the string ID
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetTicksLabelsStringID(jfloLineGraphWidget* graph, uint32_t stringID);

// *****************************************************************************
/* Function:
    laBool jfloLineGraphWidget_GetGridlinesVisible(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis)

  Summary:
    Returns GFX_TRUE if the axis gridlines are visible

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    
  Returns:
    laBool - GFX_TRUE if the axis gridlines are visible
    
  Remarks:
    
*/
LIB_EXPORT laBool jfloLineGraphWidget_GetGridlinesVisible(jfloLineGraphWidget* graph, laLineGraphValueAxis axis);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetGridlinesVisible(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis, laBool visible)

  Summary:
    Shows/Hides the gridlines

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - category ID
    laBool visible - shows the gridlines if GFX_TRUE
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetGridlinesVisible(jfloLineGraphWidget* graph, laLineGraphValueAxis axis, laBool visible);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_AddCategory(jfloLineGraphWidget* graph, uint32_t * id)

  Summary:
    Adds a category to the graph

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    uint32_t * id - destination of the ID of the new category
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_AddCategory(jfloLineGraphWidget* graph, uint32_t * id);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_GetCategoryText(jfloLineGraphWidget* graph, uint32_t categoryID, laString * str)

  Summary:
    Gets a copy of the string used to label the category

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    uint32_t categoryID - category ID
    laString * str - destination of the copied string 
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_GetCategoryText(jfloLineGraphWidget* graph, uint32_t categoryID, laString * str);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetCategoryText(jfloLineGraphWidget* graph, uint32_t categoryID, laString str)

  Summary:
    Sets the string used to label the category

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    int32_t categoryID - category ID, if -1 the last category is assigned
    laString str - the string to use
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetCategoryText(jfloLineGraphWidget* graph, int32_t categoryID, laString str);

// *****************************************************************************
/* Function:
    laBool jfloLineGraphWidget_GetStacked(jfloLineGraphWidget* graph)

  Summary:
    Returns GFX_TRUE if the bars are stacked

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laBool jfloLineGraphWidget_GetStacked(jfloLineGraphWidget* graph);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetStacked(jfloLineGraphWidget* graph, laBool stacked)

  Summary:
    Stacks the line graph

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    laBool stacked - if GFX_TRUE, the bars are stacked
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetStacked(jfloLineGraphWidget* graph, laBool stacked);

// *****************************************************************************
/* Function:
    laBool jfloLineGraphWidget_GetCategoryAxisTicksVisible(jfloLineGraphWidget* graph)

  Summary:
    Returns GFX_TRUE if the category axis ticks are visible

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laBool - GFX_TRUE if the category axis ticks are visible
    
  Remarks:
    
*/
LIB_EXPORT laBool jfloLineGraphWidget_GetCategoryAxisTicksVisible(jfloLineGraphWidget* graph);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetCategoryAxisTicksVisible(jfloLineGraphWidget* graph, laBool visible)

  Summary:
    Shows/Hides the category axis ticks

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    laBool visible - if GFX_TRUE, the axis ticks are shown
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetCategoryAxisTicksVisible(jfloLineGraphWidget* graph, laBool visible);

// *****************************************************************************
/* Function:
    laBool jfloLineGraphWidget_GetCategoryAxisLabelsVisible(jfloLineGraphWidget* graph)

  Summary:
    Returns GFX_TRUE if the category axis labels are visible

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laBool - GFX_TRUE if the category axis labels are visible
    
  Remarks:
    
*/
LIB_EXPORT laBool jfloLineGraphWidget_GetCategoryAxisLabelsVisible(jfloLineGraphWidget* graph);

// *****************************************************************************
/* Function:
    LIB_EXPORT laResult jfloLineGraphWidget_SetCategoryAxisLabelsVisible(jfloLineGraphWidget* graph, laBool visible)

  Summary:
    Shows/Hides the category axis labels

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    laBool visible - if GFX_TRUE, the axis labels are shown
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetCategoryAxisLabelsVisible(jfloLineGraphWidget* graph, laBool visible);

// *****************************************************************************
/* Function:
    laBool jfloLineGraphWidget_GetFillGraphArea(jfloLineGraphWidget* graph)

  Summary:
    Returns GFX_TRUE if the graph area is filled

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laBool - GFX_TRUE if the graph area is filled
    
  Remarks:
    
*/
LIB_EXPORT laBool jfloLineGraphWidget_GetFillGraphArea(jfloLineGraphWidget* graph);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetFillGraphArea(jfloLineGraphWidget* graph, laBool fill)

  Summary:
    Sets the graph area filled or not

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    laBool fill - if GFX_TRUE, fills the graph area
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetFillGraphArea(jfloLineGraphWidget* graph, laBool fill);

// *****************************************************************************
/* Function:
    laBool jfloLineGraphWidget_GetFillSeriesArea(jfloLineGraphWidget* graph)

  Summary:
    Returns GFX_TRUE if the series area are filled

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    laBool - GFX_TRUE if the series area is filled
    
  Remarks:
    
*/
LIB_EXPORT laBool jfloLineGraphWidget_GetFillSeriesArea(jfloLineGraphWidget* graph);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetFillGraphArea(jfloLineGraphWidget* graph, laBool fill)

  Summary:
    Sets the series area filled or not

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    laBool fill - if GFX_TRUE, fills the series area
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetFillSeriesArea(jfloLineGraphWidget* graph, laBool fill);

// *****************************************************************************
/* Function:
    jfloLineGraphTickPosition jfloLineGraphWidget_GetValueAxisTicksPosition(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis)

  Summary:
    Returns the position of the ticks in the axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the index of the value axis
    
  Returns:
    jfloLineGraphTickPosition - the tick position
    
  Remarks:
    
*/
LIB_EXPORT laLineGraphTickPosition jfloLineGraphWidget_GetValueAxisTicksPosition(jfloLineGraphWidget* graph, laLineGraphValueAxis axis);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetValueAxisTicksPosition(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis, jfloLineGraphTickPosition position)

  Summary:
    Sets the position of the ticks in the value axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    jfloLineGraphTickPosition position - the tick position
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetValueAxisTicksPosition(jfloLineGraphWidget* graph, laLineGraphValueAxis axis, laLineGraphTickPosition position);

// *****************************************************************************
/* Function:
    jfloLineGraphTickPosition jfloLineGraphWidget_GetValueAxisSubticksPosition(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis)

  Summary:
    Returns the position of the subticks in the axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the index of the value axis
    
  Returns:
    jfloLineGraphTickPosition - the subtick position
    
  Remarks:
    
*/
LIB_EXPORT laLineGraphTickPosition jfloLineGraphWidget_GetValueAxisSubticksPosition(jfloLineGraphWidget* graph, laLineGraphValueAxis axis);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetValueAxisSubticksPosition(jfloLineGraphWidget* graph, jfloLineGraphValueAxis axis, jfloLineGraphTickPosition position)

  Summary:
    Sets the position of the subticks in the value axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphValueAxis axis - the value axis index
    jfloLineGraphTickPosition position  - position of the subticks
    
  Returns:
    laResult - the result of the operation
    
  Remarks:
    
*/
LIB_EXPORT laResult jfloLineGraphWidget_SetValueAxisSubticksPosition(jfloLineGraphWidget* graph, laLineGraphValueAxis axis, laLineGraphTickPosition position);

// *****************************************************************************
/* Function:
    jfloLineGraphTickPosition jfloLineGraphWidget_GetCategoryAxisTicksPosition(jfloLineGraphWidget* graph)

  Summary:
    Returns the position of the ticks in the category axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    
  Returns:
    jfloLineGraphTickPosition - position of the ticks in the category axis
    
  Remarks:
    
*/
LIB_EXPORT laLineGraphTickPosition jfloLineGraphWidget_GetCategoryAxisTicksPosition(jfloLineGraphWidget* graph);

// *****************************************************************************
/* Function:
    laResult jfloLineGraphWidget_SetCategoryAxisTicksPosition(jfloLineGraphWidget* graph, jfloLineGraphTickPosition position)

  Summary:
    Sets the position of the ticks in the category axis

  Description:
    
  Parameters:
    jfloLineGraphWidget* graph - the widget
    jfloLineGraphTickPosition position  - position of the ticks
    
  Returns:
    laResult - the result of the operation
    
  Remarks:

*/
LIB_EXPORT laResult jfloLineGraphWidget_SetCategoryAxisTicksPosition(jfloLineGraphWidget* graph, laLineGraphTickPosition position);

#endif // LA_LINE_GRAPH_WIDGET_ENABLED
#endif /* LIBARIA_WIDGET_LINE_GRAPH_H */