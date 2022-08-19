#include "gfx/libaria/inc/libaria_widget_button.h"
#include "jflo_widget_button.h"

#if JFLO_BUTTON_WIDGET_ENABLED

#include "gfx/libaria/inc/libaria_context.h"
#include "gfx/libaria/inc/libaria_layer.h"
#include "gfx/libaria/inc/libaria_string.h"
#include "gfx/libaria/inc/libaria_utils.h"
#include "gfx/libaria/inc/libaria_widget.h"
#include "system/debug/sys_debug.h"

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          100

#define DEFAULT_RADIUS          20

#define DEFAULT_IMAGE_MARGIN    10

#define DEFAULT_PRESSED_OFFSET  1

static void touchDown(jfloButtonWidget* btn, laInput_TouchDownEvent* evt);
static void touchUp(jfloButtonWidget* btn, laInput_TouchUpEvent* evt);
static void touchMoved(jfloButtonWidget* btn, laInput_TouchMovedEvent* evt);

static void invalidateImageRect(jfloButtonWidget* btn)
{
    GFX_Rect imgRect, imgSrcRect;
    
    _jfloButtonWidget_GetImageRect(btn, &imgRect, &imgSrcRect);
    
    laLayer_AddDamageRect(laUtils_GetLayer((laWidget*)btn),
		                  &imgRect,
						  LA_FALSE);
}

static void invalidateTextRect(jfloButtonWidget* btn)
{
    GFX_Rect textRect, drawRect;
    
    _jfloButtonWidget_GetTextRect(btn, &textRect, &drawRect);
    
    laLayer_AddDamageRect(laUtils_GetLayer((laWidget*)btn),
		                  &drawRect,
					      LA_FALSE);
}

static void invalidateContents(jfloButtonWidget* btn)
{
    if(laString_IsEmpty(&btn->text) == LA_FALSE)
        invalidateTextRect(btn);
        
    if((btn->state != LA_BUTTON_STATE_UP && btn->pressedImage != NULL) ||
       (btn->state == LA_BUTTON_STATE_UP && btn->releasedImage != NULL))
        invalidateImageRect(btn);
}

static void languageChanging(jfloButtonWidget* btn)
{
    if(laString_IsEmpty(&btn->text) == LA_FALSE)
        invalidateContents(btn);
}

void _jfloButtonWidget_Constructor(jfloButtonWidget* btn)
{
    _laWidget_Constructor((laWidget*)btn);

    btn->widget.destructor = (laWidget_Destructor_FnPtr)&_jfloButtonWidget_Destructor;

    btn->widget.languageChangeEvent = (laWidget_LanguageChangingEvent_FnPtr)&languageChanging;

    btn->widget.type = LA_WIDGET_BUTTON;

    // override base class methods
    btn->widget.paint = (laWidget_Paint_FnPtr)&_jfloButtonWidget_Paint;
    
    btn->widget.rect.width = DEFAULT_WIDTH;
    btn->widget.rect.height = DEFAULT_HEIGHT;
    
    /*custom*/
    btn->x= 150;
    btn->y= 61;
    btn->arcThickness = 2;
//    btn->radius = DEFAULT_RADIUS;


    btn->state = LA_BUTTON_STATE_UP;
    btn->toggleable = LA_FALSE;

    btn->widget.borderType = LA_WIDGET_BORDER_BEVEL;

    laString_Initialize(&btn->text);
    
    btn->imageMargin = DEFAULT_IMAGE_MARGIN;
    btn->pressedOffset = DEFAULT_PRESSED_OFFSET;

    btn->halign = LA_HALIGN_CENTER;
    btn->valign = LA_VALIGN_MIDDLE;
    
    btn->pressedImage = NULL;
    btn->releasedImage = NULL;

    btn->widget.touchDown = (laWidget_TouchDownEvent_FnPtr)&touchDown;
    btn->widget.touchUp = (laWidget_TouchUpEvent_FnPtr)&touchUp;
    btn->widget.touchMoved = (laWidget_TouchMovedEvent_FnPtr)&touchMoved;
    
    btn->widget.invalidateBorderAreas = (laWidget_InvalidateBorderAreas_FnPtr)&_jfloButtonWidget_InvalidateBorderAreas;
}

void _jfloButtonWidget_Destructor(jfloButtonWidget* btn)
{
    laString_Destroy(&btn->text);
    
    // free any existing memory reader
    if(btn->reader != NULL)
    {
        laContext_GetActive()->memIntf.heap.free(btn->reader);
        
        btn->reader = NULL;
    }

    _laWidget_Destructor((laWidget*)btn);
}

jfloButtonWidget* jfloButtonWidget_New()
{
    jfloButtonWidget* btn = NULL;

    if(laContext_GetActive() == NULL)
        return NULL;

    btn = laContext_GetActive()->memIntf.heap.calloc(1, sizeof(jfloButtonWidget));
    
    _jfloButtonWidget_Constructor(btn);

    return btn;
}

laBool jfloButtonWidget_GetToggleable(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return LA_FALSE;

    return btn->toggleable;
}

laResult jfloButtonWidget_SetToggleable(jfloButtonWidget* btn, laBool toggleable)
{
    if(btn == NULL)
        return LA_FAILURE;

    btn->toggleable = toggleable == 0 ? LA_FALSE : LA_TRUE;
    
    return LA_SUCCESS;
}

laBool jfloButtonWidget_GetPressed(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return LA_FALSE;

    return btn->state == LA_BUTTON_STATE_DOWN || btn->state == LA_BUTTON_STATE_TOGGLED;
}

laResult jfloButtonWidget_SetPressed(jfloButtonWidget* btn, laBool pressed)
{
    laBool dirty = LA_FALSE;

    if(btn == NULL || (pressed == LA_TRUE && btn->state == LA_BUTTON_STATE_TOGGLED))
        return LA_FAILURE;
        
    if(pressed == LA_TRUE)
    {
        if(btn->pressedEvent != NULL)
            btn->pressedEvent(btn);
            
        // move to toggled state
        if(btn->toggleable == LA_TRUE)
        {
            btn->state = LA_BUTTON_STATE_TOGGLED;
        }
        // throw released event immediately
        else
        {
            if(btn->releasedEvent != NULL)
                btn->releasedEvent(btn);
        }
    }
    else
    {
        btn->state = LA_BUTTON_STATE_UP;
        
        if(btn->releasedEvent != NULL)
                btn->releasedEvent(btn);
    }
    
    // try to find a reason to redraw
    if(btn->pressedImage != NULL && btn->pressedImage != btn->releasedImage)
        dirty = LA_TRUE;
        
    if(btn->widget.borderType == LA_WIDGET_BORDER_BEVEL)
        dirty = LA_TRUE;
        
    if(btn->pressedOffset != 0 &&
       (btn->pressedImage != NULL ||
        laString_IsEmpty(&btn->text) == LA_FALSE))
        dirty = LA_TRUE;
        
    if(dirty == LA_TRUE)
        laWidget_Invalidate((laWidget*)btn);
        
    return LA_SUCCESS;
}

laHAlignment jfloButtonWidget_GetHAlignment(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return 0;

    return btn->halign;
}

laResult jfloButtonWidget_SetHAlignment(jfloButtonWidget* btn,
                                  laHAlignment align)
{
    if(btn == NULL)
        return LA_FAILURE;
        
    if(btn->halign == align)
        return LA_SUCCESS;

    invalidateContents(btn);

    btn->halign = align;

    invalidateContents(btn);
    
    return LA_SUCCESS;
}

laVAlignment jfloButtonWidget_GetVAlignment(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return 0;

    return btn->valign;
}

laResult jfloButtonWidget_SetVAlignment(jfloButtonWidget* btn,
                                      laVAlignment align)
{
    if(btn == NULL)
        return LA_FAILURE;

    if(btn->valign == align)
        return LA_SUCCESS;

    invalidateContents(btn);

    btn->valign = align;

    invalidateContents(btn);
    
    return LA_SUCCESS;
}

laResult jfloButtonWidget_GetText(jfloButtonWidget* btn, laString* str)
{
    if(btn == NULL)
        return LA_FAILURE;
    
    return laString_Copy(str, &btn->text);
}

laResult jfloButtonWidget_SetText(jfloButtonWidget* btn, laString str)
{
    if(btn == NULL)
        return LA_FAILURE;
        
    if(laString_Compare(&btn->text, &str) == 0)
        return LA_FAILURE;
        
    invalidateContents(btn);
        
    if(laString_Copy(&btn->text, &str) == LA_FAILURE)
        return LA_FAILURE;
    
    invalidateContents(btn);
    
    return LA_SUCCESS;
}

GFXU_ImageAsset* jfloButtonWidget_GetPressedImage(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return NULL;

    return btn->pressedImage;
}

laResult jfloButtonWidget_SetPressedImage(jfloButtonWidget* btn,
                                        GFXU_ImageAsset* img)
{
    if(btn == NULL || btn->pressedImage == img)
        return LA_FAILURE;

    if(btn->pressedImage == img)
        return LA_SUCCESS;
        
    invalidateContents(btn);

    btn->pressedImage = img;

    invalidateContents(btn);
    
    return LA_SUCCESS;
}

GFXU_ImageAsset* jfloButtonWidget_GetReleasedImage(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return NULL;

    return btn->releasedImage;
}

laResult jfloButtonWidget_SetReleasedImage(jfloButtonWidget* btn,
                                         GFXU_ImageAsset* img)
{
    if(btn == NULL || btn->releasedImage == img)
        return LA_FAILURE;

    if(btn->releasedImage == img)
        return LA_SUCCESS;

    invalidateContents(btn);

    btn->releasedImage = img;

    invalidateContents(btn);
    
    return LA_SUCCESS;
}

laRelativePosition jfloButtonWidget_GetImagePosition(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return LA_RELATIVE_POSITION_LEFTOF;
        
    return btn->imagePosition;
}

laResult jfloButtonWidget_SetImagePosition(jfloButtonWidget* btn,
                                         laRelativePosition pos)
{
    if(btn == NULL || btn->imagePosition == pos)
        return LA_FAILURE;
    
    if(btn->imagePosition == pos)
        return LA_SUCCESS;
    
    invalidateContents(btn);
    
    btn->imagePosition = pos;
    
    invalidateContents(btn);
    
    return LA_SUCCESS;
}

uint32_t jfloButtonWidget_GetImageMargin(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return LA_RELATIVE_POSITION_LEFTOF;
        
    return btn->imageMargin;
}

laResult jfloButtonWidget_SetImageMargin(jfloButtonWidget* btn, uint32_t mg)
{
    if(btn == NULL)
        return LA_FAILURE;
    
    if(btn->imageMargin == mg)
        return LA_SUCCESS;
        
    invalidateContents(btn);
    
    btn->imageMargin = mg;
    
    invalidateContents(btn);
    
    return LA_SUCCESS;
}

int32_t jfloButtonWidget_GetPressedOffset(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return 0;
        
    return btn->pressedOffset;
}

laResult jfloButtonWidget_SetPressedOffset(jfloButtonWidget* btn, int32_t offs)
{
    if(btn == NULL || btn->pressedOffset == offs)
        return LA_FAILURE;
        
    if(btn->pressedOffset == offs)
        return LA_SUCCESS;
        
    if(btn->state != LA_BUTTON_STATE_UP)
        invalidateContents(btn);
        
    btn->pressedOffset = offs;
    
    if(btn->state != LA_BUTTON_STATE_UP)
        invalidateContents(btn);
        
    return LA_SUCCESS;
}

jfloButtonWidget_PressedEvent jfloButtonWidget_GetPressedEventCallback(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return NULL;

    return btn->pressedEvent;
}

laResult jfloButtonWidget_SetPressedEventCallback(jfloButtonWidget* btn,
                                                jfloButtonWidget_PressedEvent cb)
{
    if(btn == NULL || btn->pressedEvent == cb)
        return LA_FAILURE;

    btn->pressedEvent = cb;
    
    return LA_SUCCESS;
}

jfloButtonWidget_ReleasedEvent jfloButtonWidget_GetReleasedEventCallback(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return NULL;

    return btn->releasedEvent;
}

laResult jfloButtonWidget_SetReleasedEventCallback(jfloButtonWidget* btn,
                                                 jfloButtonWidget_ReleasedEvent cb)
{
    if(btn == NULL || btn->releasedEvent == cb)
        return LA_FAILURE;

    btn->releasedEvent = cb;
    
    return LA_SUCCESS;
}

static void touchDown(jfloButtonWidget* btn, laInput_TouchDownEvent* evt)
{
    
//    SYS_PRINT("\n touchDown"); 
    laBool dirty = LA_FALSE;
    
    evt->event.accepted = LA_TRUE;

    if(btn->toggleable == LA_TRUE)
    {
        if(btn->state != LA_BUTTON_STATE_TOGGLED)
        {
            btn->state = LA_BUTTON_STATE_DOWN;

            if(btn->pressedEvent != NULL)
                btn->pressedEvent(btn);
        }
    }
    else
    {
        btn->state = LA_BUTTON_STATE_DOWN;

        if(btn->pressedEvent != NULL)
            btn->pressedEvent(btn);
    }

    // try to find a reason to redraw
    if(btn->pressedImage != NULL && btn->pressedImage != btn->releasedImage)
        dirty = LA_TRUE;
        
    if(btn->widget.borderType == LA_WIDGET_BORDER_BEVEL)
        dirty = LA_TRUE;
        
    if(btn->pressedOffset != 0 &&
       (btn->pressedImage != NULL ||
        laString_IsEmpty(&btn->text) == LA_FALSE))
        dirty = LA_TRUE;
        
    if(dirty == LA_TRUE)
        laWidget_Invalidate((laWidget*)btn);
        
    //printf("btn touch down\n");
}

static void touchUp(jfloButtonWidget* btn, laInput_TouchUpEvent* evt)
{
//    SYS_PRINT("\n touchUp");
    GFX_Rect rect = laUtils_WidgetLocalRect((laWidget*)btn);
    GFX_Point pnt;
    
    laBool dirty = LA_FALSE;

    pnt.x = evt->x;
    pnt.y = evt->y;

    evt->event.accepted = LA_TRUE;

    laUtils_ClipRectToParent(btn->widget.parent, &rect);
    laUtils_RectToLayerSpace((laWidget*)btn, &rect);
    
    //Toggle buttons will not respond to touch outside the button area
    if ((GFX_RectContainsPoint(&rect, &pnt) == GFX_FALSE) && 
        (btn->toggleable == LA_TRUE))
    {
        return;
    }

    if(GFX_RectContainsPoint(&rect, &pnt) == GFX_TRUE)
    {
        if(btn->toggleable == LA_TRUE)
        {
            if(btn->state == LA_BUTTON_STATE_TOGGLED)
            {
                btn->state = LA_BUTTON_STATE_UP;

                if(btn->releasedEvent != NULL)
                    btn->releasedEvent(btn);

                //printf("btn released\n");
            }
            else
                btn->state = LA_BUTTON_STATE_TOGGLED;
        }
        else
        {
            btn->state = LA_BUTTON_STATE_UP;

            if(btn->releasedEvent != NULL)
                btn->releasedEvent(btn);

            //printf("btn released\n");
        }
    }
    else
    {
        if(btn->toggleable == LA_TRUE)
        {
            if(btn->state != LA_BUTTON_STATE_TOGGLED)
                btn->state = LA_BUTTON_STATE_UP;
        }
        else
        {
            btn->state = LA_BUTTON_STATE_UP;
        }
    }
    
    // try to find a reason to redraw
    if(btn->pressedImage != NULL && btn->pressedImage != btn->releasedImage)
        dirty = LA_TRUE;
        
    if(btn->widget.borderType == LA_WIDGET_BORDER_BEVEL)
        dirty = LA_TRUE;
        
    if(btn->pressedOffset != 0 &&
       (btn->pressedImage != NULL ||
        laString_IsEmpty(&btn->text) == LA_FALSE))
        dirty = LA_TRUE;
        
    if(dirty == LA_TRUE)
        laWidget_Invalidate((laWidget*)btn);
    
    //printf("btn touch up\n");
}

static void touchMoved(jfloButtonWidget* btn, laInput_TouchMovedEvent* evt)
{
//    SYS_PRINT("\n touchMoved");
    GFX_Rect rect = laUtils_WidgetLocalRect((laWidget*)btn);
    GFX_Point pnt;
    
    laBool dirty = LA_FALSE;

    pnt.x = evt->x;
    pnt.y = evt->y;

    evt->event.accepted = LA_TRUE;
    
    //Toggle buttons will not respond to touch outside the button area
    if ((GFX_RectContainsPoint(&rect, &pnt) == GFX_FALSE) && 
        (btn->toggleable == LA_TRUE))
    {
        return;
    }

    if(btn->state == LA_BUTTON_STATE_DOWN)
    {
        laUtils_ClipRectToParent(btn->widget.parent, &rect);
        laUtils_RectToLayerSpace((laWidget*)btn, &rect);

        if(GFX_RectContainsPoint(&rect, &pnt) == GFX_FALSE)
        {
            btn->state = LA_BUTTON_STATE_UP;
            
            // try to find a reason to redraw
            if(btn->pressedImage != NULL && btn->pressedImage != btn->releasedImage)
                dirty = LA_TRUE;
                
            if(btn->widget.borderType == LA_WIDGET_BORDER_BEVEL)
                dirty = LA_TRUE;
                
            if(btn->pressedOffset != 0 &&
               (btn->pressedImage != NULL ||
                laString_IsEmpty(&btn->text) == LA_FALSE))
                dirty = LA_TRUE;
                
            if(dirty == LA_TRUE)
                laWidget_Invalidate((laWidget*)btn);
        }
    }
    else if(btn->state == LA_BUTTON_STATE_UP)
    {
        laUtils_ClipRectToParent(btn->widget.parent, &rect);
        laUtils_RectToLayerSpace((laWidget*)btn, &rect);

        if(GFX_RectContainsPoint(&rect, &pnt) == GFX_TRUE)
        {
            btn->state = LA_BUTTON_STATE_DOWN;

            // try to find a reason to redraw
            if(btn->pressedImage != NULL && btn->pressedImage != btn->releasedImage)
                dirty = LA_TRUE;
                
            if(btn->widget.borderType == LA_WIDGET_BORDER_BEVEL)
                dirty = LA_TRUE;
                
            if(btn->pressedOffset != 0 &&
               (btn->pressedImage != NULL ||
                laString_IsEmpty(&btn->text) == LA_FALSE))
                dirty = LA_TRUE;
                
            if(dirty == LA_TRUE)
                laWidget_Invalidate((laWidget*)btn);
        }
    }
}

/** New function for BUTTON*/
uint32_t jfloButtonWidget_GetThickness(jfloButtonWidget* btn)
{
    if(btn == NULL)
        return 0;
        
    return btn->arcThickness;
}

laResult jfloButtonWidget_SetThickness(jfloButtonWidget* btn, uint32_t thickness)
{
    if(btn == NULL)
        return LA_FAILURE;

    if (thickness > btn->widget.cornerRadius)
        return LA_FAILURE;

    if(btn->arcThickness == thickness)
        return LA_SUCCESS;
        
    btn->arcThickness = thickness;
    
    laWidget_Invalidate((laWidget*)btn);
        
    return LA_SUCCESS;
}
/****************************/

#endif // LA_BUTTON_WIDGET_ENABLED