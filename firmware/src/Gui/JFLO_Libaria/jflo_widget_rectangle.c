#include "gfx/libaria/inc/libaria_widget_rectangle.h"
#include "jflo_widget_rectangle.h"
#if LA_RECTANGLE_WIDGET_ENABLED

#include "gfx/libaria/inc/libaria_context.h"
#include "gfx/libaria/inc/libaria_string.h"
#include "gfx/libaria/inc/libaria_utils.h"
#include "gfx/libaria/inc/libaria_widget.h"

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          100

#define DEFAULT_THICKNESS       2

static void _jfloRectangleWidget_Destructor(jfloRectangleWidget* rect);
void _jfloRectangleWidget_Paint(jfloRectangleWidget* rect);

static void _jfloRectangleWidget_Constructor(jfloRectangleWidget* rect)
{
    _laWidget_Constructor((laWidget*)rect);
    
    rect->widget.destructor = (laWidget_Destructor_FnPtr)&_jfloRectangleWidget_Destructor;

    rect->widget.type = LA_WIDGET_RECTANGLE;

    // override base class methods
    rect->widget.paint = (laWidget_Paint_FnPtr)&_jfloRectangleWidget_Paint;

    rect->widget.rect.width = DEFAULT_WIDTH;
    rect->widget.rect.height = DEFAULT_HEIGHT;

    rect->widget.borderType = LA_WIDGET_BORDER_NONE;
    rect->widget.backgroundType = LA_WIDGET_BACKGROUND_FILL;

    rect->thickness = DEFAULT_THICKNESS;
}

static void _jfloRectangleWidget_Destructor(jfloRectangleWidget* rect)
{
    _laWidget_Destructor((laWidget*)rect);
}

jfloRectangleWidget* jfloRectangleWidget_New()
{
    jfloRectangleWidget* rect = NULL;

    if(laContext_GetActive() == NULL)
        return NULL;

    rect = laContext_GetActive()->memIntf.heap.calloc(1, sizeof(jfloRectangleWidget));

    if(rect == NULL)
        return NULL;
    
    _jfloRectangleWidget_Constructor(rect);

    return rect;
}

int32_t jfloRectangleWidget_GetThickness(jfloRectangleWidget* rect)
{
    if(rect == NULL)
        return 0;
        
    return rect->thickness;
}

laResult jfloRectangleWidget_SetThickness(jfloRectangleWidget* rect, int32_t thk)
{
    if(rect == NULL || thk < 0)
        return LA_FAILURE;
        
    if(rect->thickness == thk)
        return LA_SUCCESS;
        
    _jfloRectangleWidget_InvalidateRect(rect);
        
    rect->thickness = thk;
    
    _jfloRectangleWidget_InvalidateRect(rect);
            
    return LA_SUCCESS;
}

#endif // LA_RECTANGLE_WIDGET_ENABLED