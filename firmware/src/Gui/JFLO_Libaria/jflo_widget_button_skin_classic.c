#include "gfx/libaria/inc/libaria_widget_button.h"
#include "jflo_widget_button.h"

#if JFLO_BUTTON_WIDGET_ENABLED

#include "gfx/libaria/inc/libaria_context.h"
#include "gfx/libaria/inc/libaria_draw.h"
#include "gfx/libaria/inc/libaria_layer.h"
#include "gfx/libaria/inc/libaria_string.h"
#include "gfx/libaria/inc/libaria_utils.h"
#include "gfx/libaria/inc/libaria_widget.h"

#include "gfx/libaria/inc/libaria_widget_skin_classic_common.h"
#include "system/debug/sys_debug.h"
#include "gfx/hal/inc/gfx_interface.h"
#include "gfx/hal/inc/gfx_hal.h"

#define DEFAULT_NUM_LINES 5

enum
{
    NOT_STARTED = LA_WIDGET_DRAW_STATE_READY,
    DONE = LA_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_IMAGE,
    WAIT_IMAGE,
    DRAW_STRING,
    WAIT_STRING,
    DRAW_BORDER,
};

void _jfloButtonWidget_GetImageRect(jfloButtonWidget* btn,
                                  GFX_Rect* imgRect,
								  GFX_Rect* imgSrcRect)
{
    GFX_Rect textRect;
    GFX_Rect bounds = laUtils_WidgetLocalRect((laWidget*)btn);
    
    imgRect->x = 0;
    imgRect->y = 0;
    
    laString_GetMultiLineRect(&btn->text, &textRect);
    
    if(btn->state != LA_BUTTON_STATE_UP)
    {
        if(btn->pressedImage != NULL)
        {
            imgRect->width = btn->pressedImage->width;
            imgRect->height = btn->pressedImage->height;
        }
    }
    else
    {
        if(btn->releasedImage != NULL)
        {
            imgRect->width = btn->releasedImage->width;
            imgRect->height = btn->releasedImage->height;
        }
    }
    
    *imgSrcRect = *imgRect;
    
    laUtils_ArrangeRectangle(imgRect,
                             textRect,
                             bounds,
                             btn->halign,
                             btn->valign,
                             btn->imagePosition,
                             btn->widget.margin.left,
                             btn->widget.margin.top,
                             btn->widget.margin.right,
                             btn->widget.margin.bottom,
                             btn->imageMargin);
                             
    if(btn->state != LA_BUTTON_STATE_UP)
    {
        imgRect->x += btn->pressedOffset;
        imgRect->y += btn->pressedOffset;
    }                             
                             
    *imgRect = GFX_RectClipAdj(imgRect, &bounds, imgSrcRect); 
    
    laUtils_RectToLayerSpace((laWidget*)btn, imgRect);                         
}

void _jfloButtonWidget_GetTextRect(jfloButtonWidget* btn,
                                 GFX_Rect* textRect,
								 GFX_Rect* drawRect)
{
    GFX_Rect bounds;
    
    GFX_Rect imgRect = {0};
    
    bounds = laUtils_WidgetLocalRect((laWidget*)btn);
    
    laString_GetMultiLineRect(&btn->text, textRect);
    
    if(btn->state != LA_BUTTON_STATE_UP)
    {
        if(btn->pressedImage != NULL)
        {
            imgRect.width = btn->pressedImage->width;
            imgRect.height = btn->pressedImage->height;
        }
    }
    else
    {
        if(btn->releasedImage != NULL)
        {
            imgRect.width = btn->releasedImage->width;
            imgRect.height = btn->releasedImage->height;
        }
    }
    
    // arrange relative to image rect
    laUtils_ArrangeRectangleRelative(textRect,
                                     imgRect,
                                     bounds,
                                     btn->halign,
                                     btn->valign,
                                     btn->imagePosition,
                                     btn->widget.margin.left,
                                     btn->widget.margin.top,
                                     btn->widget.margin.right,
                                     btn->widget.margin.bottom,
                                     btn->imageMargin);

    if(btn->state != LA_BUTTON_STATE_UP)
    {
        textRect->x += btn->pressedOffset;
        textRect->y += btn->pressedOffset;
    }
                                     
    GFX_RectClip(textRect, &bounds, drawRect);

	// move the rects to layer space
	laUtils_RectToLayerSpace((laWidget*)btn, textRect);
    laUtils_RectToLayerSpace((laWidget*)btn, drawRect);
}

void _jfloButtonWidget_InvalidateBorderAreas(jfloButtonWidget* btn)
{
    GFX_Rect rect, dmgRect;
	int32_t left, top, right, bottom;
	laLayer* layer;
	
	if(btn->widget.borderType == LA_WIDGET_BORDER_NONE)
	    return;
	
	rect = laUtils_WidgetLayerRect((laWidget*)btn);
	layer = laUtils_GetLayer((laWidget*)btn);
	
	if(btn->widget.borderType == LA_WIDGET_BORDER_LINE)
	{
	    if(rect.width == 0 || rect.height == 0)
			return;

		if(rect.width <= 1 || rect.height <= 1)
		{
			laLayer_AddDamageRect(layer, &rect, LA_TRUE);

			return;
		}
	
	    left = 1;
	    top = 1;
	    right = 1;
	    bottom = 1;
	}
	else
	{
	    if(rect.width == 0 || rect.height == 0)
			return;

		if(rect.width <= 3 || rect.height <= 3)
		{
			laLayer_AddDamageRect(layer, &rect, LA_TRUE);
			
			return;
        }
        
        right = 2;
        bottom = 2;
        
        if(btn->state == LA_BUTTON_STATE_UP)
        {
            left = 1;
            top = 1;
        }
        else
        {
            left = 2;
	        top = 2;
	    }
	}
	
	// left line
	dmgRect.x = rect.x;
	dmgRect.y = rect.y;
	dmgRect.width = left;
	dmgRect.height = rect.height;

	laLayer_AddDamageRect(layer, &dmgRect, LA_TRUE);

	// top line
	dmgRect.width = rect.width;
	dmgRect.height = top;

	laLayer_AddDamageRect(layer, &dmgRect, LA_TRUE);

	// right line
	dmgRect.x = rect.x + rect.width - right;
	dmgRect.y = rect.y;
	dmgRect.width = right;
	dmgRect.height = rect.height;

	laLayer_AddDamageRect(layer, &dmgRect, LA_TRUE);

	// bottom line
	dmgRect.x = rect.x;
	dmgRect.y = rect.y + rect.height - bottom;
	dmgRect.width = rect.width;
	dmgRect.height = bottom;

	laLayer_AddDamageRect(layer, &dmgRect, LA_TRUE);
}

static void drawBackground(jfloButtonWidget* btn);
static void drawMultiLineString(jfloButtonWidget* lbl);
static void waitString(jfloButtonWidget* btn);
static void drawImage(jfloButtonWidget* btn);
static void waitImage(jfloButtonWidget* btn);
static void drawBorder(jfloButtonWidget* btn);

static void nextState(jfloButtonWidget* btn)
{
    switch(btn->widget.drawState)
    {
        case NOT_STARTED:
        {
            if(btn->widget.backgroundType != LA_WIDGET_BACKGROUND_NONE) 
            {
                btn->widget.drawState = DRAW_BACKGROUND;
                btn->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            if((btn->state != LA_BUTTON_STATE_UP && btn->pressedImage != NULL) ||
               (btn->state == LA_BUTTON_STATE_UP && btn->releasedImage != NULL))
            {
                btn->widget.drawState = DRAW_IMAGE;
                btn->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&drawImage;

                return;
            }
        }
        case DRAW_IMAGE:
        {            
            if(laString_IsEmpty(&btn->text) == LA_FALSE)
            {
                btn->widget.drawState = DRAW_STRING;
                btn->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&drawMultiLineString;

                return;
            }
        }
        case DRAW_STRING:
        {
            if(btn->widget.borderType != LA_WIDGET_BORDER_NONE)
            {
                btn->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&drawBorder;
                btn->widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            btn->widget.drawState = DONE;
            btn->widget.drawFunc = NULL;
        }
    }
}

/**/
static void drawBackground(jfloButtonWidget* btn)
{
    GFX_Point p;
    GFX_Rect arcRect, clipRect;
    laLayer* layer = laUtils_GetLayer((laWidget*)btn);

    p.x = btn->widget.rect.width/2;
    p.y = btn->widget.rect.height/2;
       
    laUtils_PointToLayerSpace((laWidget*)btn, &p);

    arcRect.x = p.x - btn->widget.rect.width/2;
    arcRect.y = p.y - btn->widget.rect.height/2;
    arcRect.width = btn->widget.rect.width;
    arcRect.height = btn->widget.rect.height;

    
    if(GFX_RectIntersects(&layer->clippedDrawingRect, &arcRect) == GFX_TRUE)
    {
        GFX_RectClip(&arcRect, &layer->clippedDrawingRect, &clipRect);
        
        GFX_Set(GFXF_DRAW_CLIP_RECT, &clipRect);
        GFX_Set(GFXF_DRAW_CLIP_ENABLE, &clipRect);
        GFX_Set(GFXF_DRAW_MODE, GFX_DRAW_LINE);
        
//        GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 0xffffffff/*(arc->widget.scheme->foreground & 0xffffff00)*/, btn->widget.scheme->foreground, NULL, NULL);
        if (btn->widget.backgroundType == LA_WIDGET_BACKGROUND_FILL)
        {
            GFX_Set(GFXF_DRAW_COLOR, btn->widget.scheme->background);
            GFX_Set(GFXF_DRAW_THICKNESS, btn->widget.cornerRadius);
            drawArc_SetMode(0, 1);
            GFX_DrawArc(p.x, p.y, btn->widget.cornerRadius, 0, 360);
        } 
        
        if (btn-> widget.borderType == LA_WIDGET_BORDER_LINE)
        {          
            GFX_Color color = GFX_ColorLerp(0x528A,
                                           0x07FF,
                                           70,
                                           GFX_COLOR_MODE_RGB_565);
            
            GFX_Set(GFXF_DRAW_COLOR, color);
            const int correction_position = 1;  // TODO: w/a
            GFX_Set(GFXF_DRAW_THICKNESS, btn->arcThickness);
            
            if (btn->widget.cornerRadius >= btn->widget.rect.width /2 || btn->widget.cornerRadius >= btn->widget.rect.height /2 )
            {
                GFX_Set(GFXF_DRAW_MODE, GFX_DRAW_LINE);
                drawArc_SetMode(1, 1);
                GFX_DrawArc(p.x, p.y, btn->widget.cornerRadius, 0, 360);
            }
            else
            {
                int dx = (btn->widget.rect.width/2 - btn->widget.cornerRadius);
                int dy = (btn->widget.rect.height/2 - btn->widget.cornerRadius);
                GFX_Set(GFXF_DRAW_MODE, GFX_DRAW_FILL);

                GFX_DrawRect(p.x - dx,
                             p.y - btn->widget.rect.height/2 + correction_position,
                             btn->widget.rect.width - btn->widget.cornerRadius * 2,
                             btn->arcThickness);

                GFX_DrawRect(p.x - dx,
                             p.y + btn->widget.rect.height/2 - btn->arcThickness,
                             btn->widget.rect.width - btn->widget.cornerRadius * 2,
                             btn->arcThickness);

                GFX_DrawRect(p.x - btn->widget.rect.width/2 + correction_position,
                             p.y - dy,
                             btn->arcThickness,
                             btn->widget.rect.height - btn->widget.cornerRadius * 2);

                GFX_DrawRect(p.x + btn->widget.rect.width/2 - btn->arcThickness ,
                             p.y - dy,
                             btn->arcThickness,
                             btn->widget.rect.height - btn->widget.cornerRadius * 2);

                GFX_Set(GFXF_DRAW_MODE, GFX_DRAW_LINE);
                //Upper left
                arcRect.x = p.x - dx;
                arcRect.y = p.y - dy;
                drawArc_SetMode(1, 1);
                GFX_DrawArc(arcRect.x,
                            arcRect.y,
                            btn->widget.cornerRadius,
                            90,
                            90);

    //            //Upper right
                arcRect.x = p.x + dx;
                arcRect.y = p.y - dy;
                drawArc_SetMode(1, 1);
                GFX_DrawArc(arcRect.x,
                            arcRect.y,
                            btn->widget.cornerRadius,
                            0,
                            90);

                //Lower left
                arcRect.x = p.x - dx;
                arcRect.y = p.y + dy;
                drawArc_SetMode(1, 1);
                GFX_DrawArc(arcRect.x,
                            arcRect.y,
                            btn->widget.cornerRadius,
                            180,
                            90);

                //Lower right
                arcRect.x = p.x + dx;
                arcRect.y = p.y + dy;
                drawArc_SetMode(1, 1);
                GFX_DrawArc(arcRect.x,
                            arcRect.y,
                            btn->widget.cornerRadius,
                            270,
                            90);      
            }       
        }    
              
    }
 
    nextState(btn);
}

static void drawImage(jfloButtonWidget* btn)
{
    GFX_Rect imgRect, imgSrcRect, clipRect;
    GFXU_ImageAsset* img = NULL;
    laLayer* layer = laUtils_GetLayer((laWidget*)btn);
    
    if(btn->state != LA_BUTTON_STATE_UP)
    {
        if(btn->pressedImage != NULL)
            img = btn->pressedImage;
    }
    else
    {
        if(btn->releasedImage != NULL)
            img = btn->releasedImage;
    }
    
    _jfloButtonWidget_GetImageRect(btn, &imgRect, &imgSrcRect);
    
    if(GFX_RectIntersects(&layer->clippedDrawingRect, &imgRect) == GFX_TRUE)
    {        
        clipRect = GFX_RectClipAdj(&imgRect, &layer->clippedDrawingRect, &imgSrcRect);
        
        GFXU_DrawImage(img,
                       imgSrcRect.x,
                       imgSrcRect.y,
                       imgSrcRect.width,
                       imgSrcRect.height,
                       clipRect.x,
                       clipRect.y,
                       &laContext_GetActive()->memIntf,
                       &btn->reader);
                
        if(btn->reader != NULL)
        {  
            btn->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&waitImage;
            btn->widget.drawState = WAIT_IMAGE;
            
            return;
        }
    }
    
    nextState(btn);
}

static void waitImage(jfloButtonWidget* btn)
{
    if(btn->reader->status != GFXU_READER_STATUS_FINISHED)
    {
        btn->reader->run(btn->reader);

        return;
    }
    else
    {
        // free the reader
        btn->reader->memIntf->heap.free(btn->reader);
        btn->reader = NULL;
    }
            
    btn->widget.drawState = DRAW_IMAGE;
    
    nextState(btn);
}

static void drawMultiLineString(jfloButtonWidget* btn)
{
    GFX_Rect textRect = {0}, clipRect = {0}, drawRect = {0};
    laLayer* layer = laUtils_GetLayer((laWidget*)btn);
	uint32_t newoffset = 0;
    uint32_t end = 0;
	uint32_t numlines = 0;
    uint32_t line = 0;
    uint32_t lineY = 0;
    uint32_t maxLines = DEFAULT_NUM_LINES;
    
    GFX_Rect * lineRect = laContext_GetActive()->memIntf.heap.calloc(maxLines * sizeof(GFX_Rect), 1);
    uint32_t * offset = laContext_GetActive()->memIntf.heap.calloc(maxLines * sizeof(uint32_t), 1);
  
	while (lineRect != NULL && offset != NULL)
	{
		offset[numlines] = newoffset;

		laString_GetLineRect(&btn->text, offset[numlines], &lineRect[numlines], &newoffset);

        if (offset[numlines] == newoffset)
        {
            textRect.height += laString_GetHeight(&btn->text) - 
                               laString_GetAscent(&btn->text);            
            break;
        }

        textRect.height += laString_GetAscent(&btn->text);

		if (lineRect[numlines].width > textRect.width)
		{
			textRect.width = lineRect[numlines].width;
		}

		numlines++;
        
        if (numlines >= maxLines)
        {
            maxLines += DEFAULT_NUM_LINES;
            
            lineRect = laContext_GetActive()->memIntf.heap.realloc(lineRect, maxLines * sizeof(GFX_Rect));
            offset = laContext_GetActive()->memIntf.heap.realloc(offset, maxLines * sizeof(uint32_t));
            
            if (lineRect == NULL || offset == NULL)
            {
                //Unable to allocate for all lines
                numlines = 0;
                break;
            }
        }
	}

	// arrange relative to image rect
    _jfloButtonWidget_GetTextRect(btn, &textRect, &drawRect);
    
    for (line = 0; line < numlines; line++)
    {
        //Horizonally align the line rectangle relative to textRect 
        switch(btn->halign)
        {
            case LA_HALIGN_LEFT:
                //No break. Fall through
            default:
                lineRect[line].x = textRect.x;
                break;
            case LA_HALIGN_CENTER:
                lineRect[line].x = textRect.x + textRect.width/2 - lineRect[line].width/2;
                break;
            case LA_HALIGN_RIGHT:
                lineRect[line].x = textRect.x + textRect.width - lineRect[line].width;
                break;
        }

        lineRect[line].y = textRect.y + lineY;

        //Clip to the line rectangle 
        GFX_RectClip(&textRect, &lineRect[line], &drawRect);

        if(GFX_RectIntersects(&layer->clippedDrawingRect, &drawRect) == GFX_TRUE)
        {
            GFX_Set(GFXF_DRAW_MASK_ENABLE, GFX_FALSE);
            GFX_Set(GFXF_DRAW_COLOR, btn->widget.scheme->text);

            GFX_RectClip(&drawRect, &layer->clippedDrawingRect, &clipRect);

            if (line + 1 < numlines)
            {
                end = offset[line + 1];
            }
            else
            {
                end = newoffset;
            }

            laString_DrawSubStringClipped(&btn->text,
                                         offset[line],
                                         end,
                                         clipRect.x,
                                         clipRect.y,
                                         clipRect.width,
                                         clipRect.height,
                                         lineRect[line].x,
                                         lineRect[line].y,
                                         &btn->reader);
            if(btn->reader != NULL)
            {
                btn->widget.drawFunc = (laWidget_DrawFunction_FnPtr)&waitString;
                btn->widget.drawState = WAIT_STRING;
                
                laContext_GetActive()->memIntf.heap.free(lineRect);
                laContext_GetActive()->memIntf.heap.free(offset);

                return;
            }
        }
        
        lineY += laString_GetAscent(&btn->text);
    }

    nextState(btn);

    laContext_GetActive()->memIntf.heap.free(lineRect);
    laContext_GetActive()->memIntf.heap.free(offset);
    
}

static void waitString(jfloButtonWidget* btn)
{
    if(btn->reader->status != GFXU_READER_STATUS_FINISHED)
    {
        btn->reader->run(btn->reader);
        
        return;
    }
    
    // free the reader
    btn->reader->memIntf->heap.free(btn->reader);
    btn->reader = NULL;
    
    btn->widget.drawState = DRAW_STRING;
    
    nextState(btn);
}

static void drawBorder(jfloButtonWidget* btn)
{
//    if(btn->widget.borderType == LA_WIDGET_BORDER_LINE)
//    {         
//        GFX_Rect localRect = laUtils_WidgetLocalRect((laWidget*)btn);
//        laLayer* layer = laUtils_GetLayer((laWidget*)btn);
//
//        laUtils_RectToLayerSpace((laWidget*)btn, &localRect);
//
//        laWidget_SkinClassic_DrawRoundCornerLineBorder(&localRect,
//                                                       btn->widget.scheme->shadowDark,
//                                                       btn->cornerRadius,
//                                                       &layer->clippedDrawingRect); 
        // hiep
//        laWidget_SkinClassic_DrawBackground((laWidget*) btn, 0x000000);
        
//    }

    
    nextState(btn);
}

void _jfloButtonWidget_SetCornerRadius(jfloButtonWidget* btn, uint32_t radius)
{
    btn->widget.cornerRadius = radius;
}

void _jfloButtonWidget_Paint(jfloButtonWidget* btn)
{
    laContext* context = laContext_GetActive();
    
    if(btn->widget.scheme == NULL)
    {
        btn->widget.drawState = DONE;
        
        return;
    }
    
    if(btn->widget.drawState == NOT_STARTED)
        nextState(btn);
    
    while(btn->widget.drawState != DONE)
    {
        btn->widget.drawFunc((laWidget*)btn);
        
        if(context->preemptLevel == LA_PREEMPTION_LEVEL_2 ||
           btn->widget.drawState == WAIT_IMAGE ||
           btn->widget.drawState == WAIT_STRING)
            break;
    }
}

#endif // LA_BUTTON_WIDGET_ENABLED