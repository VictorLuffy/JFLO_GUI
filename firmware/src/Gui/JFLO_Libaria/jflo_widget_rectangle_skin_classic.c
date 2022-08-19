#include <math.h>

#include "gfx/libaria/inc/libaria_widget_rectangle.h"
#include "jflo_widget_rectangle.h"

#if LA_RECTANGLE_WIDGET_ENABLED

#include "gfx/libaria/inc/libaria_context.h"
#include "gfx/libaria/inc/libaria_draw.h"
#include "gfx/libaria/inc/libaria_layer.h"
#include "gfx/libaria/inc/libaria_string.h"
#include "gfx/libaria/inc/libaria_utils.h"
#include "gfx/libaria/inc/libaria_widget.h"

#include "gfx/libaria/inc/libaria_widget_skin_classic_common.h"
#include "system/debug/sys_debug.h"
#include "Delay.h"

enum
{
    NOT_STARTED = LA_WIDGET_DRAW_STATE_READY,
    DONE = LA_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_EDGE,
    DRAW_BORDER,
};

static void drawBackground(laRectangleWidget* rct);
static void drawEdge(laRectangleWidget* rct);
static void drawBorder(laRectangleWidget* rct);

void _jfloRectangleWidget_InvalidateRect(jfloRectangleWidget* rct)
{
	GFX_Rect rect, dmgRect;
	
	if(rct->thickness == 0)
	    return;
	    
	rect = laUtils_WidgetLayerRect((laWidget*)rct);
	    
	if(rect.width <= rct->thickness || rect.height <= rct->thickness)
	{
	    laWidget_Invalidate((laWidget*)rct);

		return;
	}

	// left line
	dmgRect.x = rect.x;
	dmgRect.y = rect.y;
	dmgRect.width = rct->thickness;
	dmgRect.height = rect.height;

	laLayer_AddDamageRect(laUtils_GetLayer((laWidget*)rct), &dmgRect, LA_TRUE);

	// top line
	dmgRect.width = rect.width;
	dmgRect.height = rct->thickness;

	laLayer_AddDamageRect(laUtils_GetLayer((laWidget*)rct), &dmgRect, LA_TRUE);

	// right line
	dmgRect.x = rect.x + rect.width - rct->thickness;
	dmgRect.y = rect.y;
	dmgRect.width = rct->thickness;
	dmgRect.height = rect.height;

	laLayer_AddDamageRect(laUtils_GetLayer((laWidget*)rct), &dmgRect, LA_TRUE);

	// bottom line
	dmgRect.x = rect.x;
	dmgRect.y = rect.y + rect.height - rct->thickness;
	dmgRect.width = rect.width;
	dmgRect.height = rct->thickness;

	laLayer_AddDamageRect(laUtils_GetLayer((laWidget*)rct), &dmgRect, LA_TRUE);
}


static void nextState(laRectangleWidget* rct)
{
    switch(rct->widget.drawState)
    {
        case NOT_STARTED:
        {
            if(rct->widget.backgroundType != LA_WIDGET_BACKGROUND_NONE) 
            {
                rct->widget.drawState = DRAW_BACKGROUND;
                rct->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            if(rct->thickness > 0)
            {
                rct->widget.drawState = DRAW_EDGE;
                rct->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&drawEdge;
                
                return;
            }
        }
        case DRAW_EDGE:
        {            
            if(rct->widget.borderType != LA_WIDGET_BORDER_NONE)
            {
                rct->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&drawBorder;
                rct->widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            rct->widget.drawState = DONE;
            rct->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(laRectangleWidget* rct)
{
    if (rct->widget.backgroundType == LA_WIDGET_BACKGROUND_FILL) {
        GFX_Rect layerRect = laUtils_WidgetLayerRect((laWidget*)rct);
        laLayer* layer = laUtils_GetLayer((laWidget*)rct);

        const GFX_Rect* rect = &layerRect;
        GFX_Color clr = rct->widget.scheme->background;
        uint32_t radius = rct->widget.cornerRadius;
        const GFX_Rect* dmgRect = &layer->clippedDrawingRect;  

        GFX_Rect clipRect;
        GFX_Rect drawRect;

        GFX_Set(GFXF_DRAW_COLOR, rct->widget.scheme->background);
        GFX_Set(GFXF_DRAW_MODE, GFX_DRAW_FILL);
        GFX_Set(GFXF_DRAW_MASK_ENABLE, GFX_FALSE);    
        
        //Draw the corners
        //Upper left
        drawRect.x = rect->x;
        drawRect.y = rect->y;
        drawRect.width = radius;
        drawRect.height = radius;

        GFX_Set(GFXF_DRAW_THICKNESS, radius);
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            drawArc_SetMode(0 , 0);
            GFX_DrawArc(drawRect.x + radius,
                        drawRect.y + radius,
                        radius,
                        90,
                        90);
        }

        //Lower left
        drawRect.x = rect->x;
        drawRect.y = rect->y + rect->height - (radius * 2);
        drawRect.width = radius;
        drawRect.height = radius;
        
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            drawArc_SetMode(0 , 0);
            GFX_DrawArc(drawRect.x + radius,
                        drawRect.y + radius,
                        radius,
                        180,
                        90);         
        }        

        //Draw the top rectangle
        drawRect.x = rect->x + radius;
        drawRect.y = rect->y;
        drawRect.width = rect->width - radius; 
        drawRect.height = rect->height;
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            GFX_RectClip(&drawRect, dmgRect, &clipRect);

            GFX_DrawRect(clipRect.x,
                         clipRect.y,
                         clipRect.width,
                         clipRect.height);
        }

        //Draw the bottom rectangle
        drawRect.x = rect->x;
        drawRect.y = rect->y + radius;
        drawRect.width = radius;
        drawRect.height = rect->height - 2 * radius;
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            GFX_RectClip(&drawRect, dmgRect, &clipRect);

            GFX_DrawRect(clipRect.x,
                         clipRect.y,
                         clipRect.width,
                         clipRect.height);
        }
        
        
    }

    nextState(rct);
}

static void drawEdge(laRectangleWidget* rct)
{
    if (rct->widget.backgroundType == LA_WIDGET_BACKGROUND_NONE) {
        GFX_Rect layerRect = laUtils_WidgetLayerRect((laWidget*)rct);
        laLayer* layer = laUtils_GetLayer((laWidget*)rct);

        const GFX_Rect* rect = &layerRect;
        GFX_Color clr = rct->widget.scheme->foreground;
        uint32_t radius = rct->widget.cornerRadius;
        const GFX_Rect* dmgRect = &layer->clippedDrawingRect;  

        GFX_Rect clipRect;
        GFX_Rect drawRect;

        GFX_Set(GFXF_DRAW_COLOR, rct->widget.scheme->foreground);
        GFX_Set(GFXF_DRAW_MODE, GFX_DRAW_FILL);
        GFX_Set(GFXF_DRAW_MASK_ENABLE, GFX_FALSE);    

        //Draw the top rectangle
        drawRect.x = rect->x + radius;
        drawRect.y = rect->y;
        drawRect.width = rect->width - (radius * 2);
        drawRect.height = rct->thickness;
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            GFX_RectClip(&drawRect, dmgRect, &clipRect);

            GFX_DrawRect(clipRect.x,
                         clipRect.y,
                         clipRect.width,
                         rct->thickness);
        }

        //Draw the bottom rectangle
        drawRect.x = rect->x + radius;
        drawRect.y = rect->y + rect->height - rct->thickness;
        drawRect.width = rect->width - (radius * 2);
        drawRect.height = rct->thickness;
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            GFX_RectClip(&drawRect, dmgRect, &clipRect);

            GFX_DrawRect(clipRect.x,
                         clipRect.y,
                         clipRect.width,
                         rct->thickness);
        }

        //Draw the left rectangle
        drawRect.x = rect->x;
        drawRect.y = rect->y + radius;
        drawRect.width = rct->thickness;
        drawRect.height = rect->height - (radius * 2);
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            GFX_RectClip(&drawRect, dmgRect, &clipRect);

            GFX_DrawRect(clipRect.x,
                         clipRect.y,
                         rct->thickness,
                         clipRect.height);
        }

        //Draw the right rectangle
        drawRect.x = rect->x + rect->width - rct->thickness;
        drawRect.y = rect->y + radius;
        drawRect.width = rct->thickness;
        drawRect.height = rect->height - (radius * 2);
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            GFX_RectClip(&drawRect, dmgRect, &clipRect);

            GFX_DrawRect(clipRect.x,
                         clipRect.y,
                         rct->thickness,
                         clipRect.height);
        }    

        //Draw the corners
        //Upper left
        drawRect.x = rect->x;
        drawRect.y = rect->y;
        drawRect.width = radius;
        drawRect.height = radius;

        GFX_Set(GFXF_DRAW_THICKNESS, rct->thickness);
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            drawArc_SetMode(0 , 0);
            GFX_DrawArc(drawRect.x + radius,
                        drawRect.y + radius,
                        radius,
                        90,
                        90);
        }

        //Upper right
        drawRect.x = rect->x + rect->width - radius;
        drawRect.y = rect->y;
        drawRect.width = radius;
        drawRect.height = radius;
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            drawArc_SetMode(0 , 0);
            GFX_DrawArc(drawRect.x,
                        drawRect.y + radius,
                        radius,
                        0,
                        90);
        }

        //Lower left
        drawRect.x = rect->x;
        drawRect.y = rect->y + rect->height - (radius * 2);
        drawRect.width = radius;
        drawRect.height = radius;
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            drawArc_SetMode(0 , 0);
            GFX_DrawArc(drawRect.x + radius,
                        drawRect.y + radius,
                        radius,
                        180,
                        90);
        }
    
        //Lower right
        drawRect.x = rect->x + rect->width - radius;
        drawRect.y = rect->y + rect->height - radius;
        drawRect.width = radius;
        drawRect.height = radius;
        if(GFX_RectIntersects(&drawRect, dmgRect) == GFX_TRUE)
        {
            drawArc_SetMode(0 , 0);
            GFX_DrawArc(drawRect.x,
                        drawRect.y,
                        radius,
                        270,
                        90);
        }    
        
        
//        GFX_Rect widgetRect, edgeRect, clipRect;
//        laLayer* layer = laUtils_GetLayer((laWidget*)rct);
//
//        widgetRect = laUtils_WidgetLocalRect((laWidget*)rct);           /// Same layerRect
//        uint32_t radius = rct->widget.cornerRadius;
//        // move the rect to parent space
//        laUtils_RectToLayerSpace((laWidget*)rct, &widgetRect);
//
//        GFX_Set(GFXF_DRAW_MODE, GFX_DRAW_FILL);
//        GFX_Set(GFXF_DRAW_COLOR, rct->widget.scheme->foreground);
//
//        // left bar
//        edgeRect.x = widgetRect.x ;
//        edgeRect.y = widgetRect.y + radius;
//        edgeRect.width = rct->thickness;
//        edgeRect.height = widgetRect.height - 2*radius; 
//
//        if(GFX_RectIntersects(&layer->clippedDrawingRect, &edgeRect) == GFX_TRUE)
//        {
//            GFX_RectClip(&edgeRect, &layer->clippedDrawingRect, &clipRect);
//
//            GFX_DrawRect(edgeRect.x,
//                         edgeRect.y,
//                         edgeRect.width,
//                         edgeRect.height);
//        }
//
//        // top bar
//        edgeRect.x = widgetRect.x + rct->thickness + radius;
//        edgeRect.width = widgetRect.width - (rct->thickness * 2) - 2*radius;
//        edgeRect.height = rct->thickness; 
//
//        if(GFX_RectIntersects(&layer->clippedDrawingRect, &edgeRect) == GFX_TRUE)
//        {
//            GFX_RectClip(&edgeRect, &layer->clippedDrawingRect, &clipRect);
//
//            GFX_DrawRect(edgeRect.x,
//                         edgeRect.y,
//                         edgeRect.width,
//                         edgeRect.height);
//        }
//
//        // right bar
//        edgeRect.x = widgetRect.x + widgetRect.width - rct->thickness;
//        edgeRect.y = widgetRect.y + radius;
//        edgeRect.width = rct->thickness;
//        edgeRect.height = widgetRect.height - 2 * radius; 
//
//        if(GFX_RectIntersects(&layer->clippedDrawingRect, &edgeRect) == GFX_TRUE)
//        {
//            GFX_RectClip(&edgeRect, &layer->clippedDrawingRect, &clipRect);
//
//            GFX_DrawRect(edgeRect.x,
//                         edgeRect.y,
//                         edgeRect.width,
//                         edgeRect.height);
//        }
//
//        // bottom bar
//        edgeRect.x = widgetRect.x + rct->thickness + radius;
//        edgeRect.y = widgetRect.y + widgetRect.height - rct->thickness;
//        edgeRect.width = widgetRect.width - (rct->thickness * 2) - 2 * radius;
//        edgeRect.height = rct->thickness; 
//
//        if(GFX_RectIntersects(&layer->clippedDrawingRect, &edgeRect) == GFX_TRUE)
//        {
//            GFX_RectClip(&edgeRect, &layer->clippedDrawingRect, &clipRect);
//
//            GFX_DrawRect(edgeRect.x,
//                         edgeRect.y,
//                         edgeRect.width,
//                         edgeRect.height);
//        }
//        
//        // Draw the corner
//        //Upper left
//        edgeRect.x = widgetRect.x;
//        edgeRect.y = widgetRect.y;
//        edgeRect.width = radius;
//        edgeRect.height = radius;
//        GFX_Set(GFXF_DRAW_THICKNESS, rct->thickness);
//        if(GFX_RectIntersects(&layer->clippedDrawingRect, &edgeRect) == GFX_TRUE)
//        {
//            GFX_DrawArc(edgeRect.x + radius,
//                        edgeRect.y + radius,
//                        radius,
//                        90,
//                        90);
//        }
//        //Upper right
//        edgeRect.x = widgetRect.x + widgetRect.width - radius;
//        edgeRect.y = widgetRect.y;
//        edgeRect.width = radius;
//        edgeRect.height = radius;
//        if(GFX_RectIntersects(&layer->clippedDrawingRect, &edgeRect) == GFX_TRUE)
//        {
//            GFX_DrawArc(edgeRect.x,
//                        edgeRect.y + radius,
//                        radius,
//                        0,
//                        90);
//        }
//
//        //Lower left
//        edgeRect.x = widgetRect.x;
//        edgeRect.y = widgetRect.y + widgetRect.height - (radius * 2);
//        edgeRect.width = radius;
//        edgeRect.height = radius;
//        if(GFX_RectIntersects(&layer->clippedDrawingRect, &edgeRect) == GFX_TRUE)
//        {
//            GFX_DrawArc(edgeRect.x + radius,
//                        edgeRect.y + radius,
//                        radius,
//                        180,
//                        90);
//        } 
//        //Lower right
//        edgeRect.x = widgetRect.x + widgetRect.width - radius;
//        edgeRect.y = widgetRect.y + widgetRect.height - radius;
//        edgeRect.width = radius;
//        edgeRect.height = radius;
//        if(GFX_RectIntersects(&layer->clippedDrawingRect, &edgeRect) == GFX_TRUE)
//        {
//            GFX_DrawArc(edgeRect.x,
//                        edgeRect.y,
//                        radius,
//                        270,
//                        90);
//        }         
        
    }
    
    nextState(rct);
}

static void drawBorder(laRectangleWidget* rct)
{
//    if(rct->widget.borderType == LA_WIDGET_BORDER_LINE)
//        laWidget_SkinClassic_DrawStandardLineBorder((laWidget*)rct);
//    else if(rct->widget.borderType == LA_WIDGET_BORDER_BEVEL)
//        laWidget_SkinClassic_DrawStandardRaisedBorder((laWidget*)rct);
    
    nextState(rct);
}

void _jfloRectangleWidget_Paint(laRectangleWidget* rct)
{
    laContext* context = laContext_GetActive();
    
    if(rct->widget.scheme == NULL)
    {
        rct->widget.drawState = DONE;
        
        return;
    }
    
    if(rct->widget.drawState == NOT_STARTED)
        nextState(rct);
    
    while(rct->widget.drawState != DONE)
    {
        rct->widget.drawFunc((laWidget*)rct);
        
        if(context->preemptLevel == LA_PREEMPTION_LEVEL_2)
            break;
    }
}

#endif // LA_RECTANGLE_WIDGET_ENABLED