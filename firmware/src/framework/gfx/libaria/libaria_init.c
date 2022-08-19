/*******************************************************************************
  MPLAB Harmony Graphics Composer Generated Implementation File

  File Name:
    libaria_init.c

  Summary:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

  Description:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

    Created with MPLAB Harmony Version 2.06
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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
// DOM-IGNORE-END

#include "gfx/libaria/libaria_init.h"

laImageWidget* LogoWidget;
laLabelWidget* TitleLabel;
laButtonWidget* SloganButton;


static void ScreenCreate_screen1(laScreen* screen);


int32_t libaria_initialize(void)
{
    laScreen* screen;

    GFX_Set(GFXF_DRAW_PIPELINE_MODE, GFX_PIPELINE_GCUGPU);
    laContext_SetStringTable(&stringTable);

    screen = laScreen_New(LA_FALSE, LA_FALSE, &ScreenCreate_screen1);
    laScreen_SetOrientation(screen, LA_SCREEN_ORIENTATION_270);
    laContext_AddScreen(screen);

    laContext_SetActiveScreen(0);

	return 0;
}

static void ScreenCreate_screen1(laScreen* screen)
{
    laLayer* layer0;

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 272, 480);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laLayer_SetBufferCount(layer0, 1);

    laScreen_SetLayer(screen, 0, layer0);

    LogoWidget = laImageWidget_New();
    laWidget_SetPosition((laWidget*)LogoWidget, 43, 60);
    laWidget_SetSize((laWidget*)LogoWidget, 184, 136);
    laWidget_SetBackgroundType((laWidget*)LogoWidget, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LogoWidget, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(LogoWidget, &NewHarmonyLogo_Small);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)LogoWidget);

    TitleLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)TitleLabel, 14, 16);
    laWidget_SetSize((laWidget*)TitleLabel, 235, 25);
    laWidget_SetBackgroundType((laWidget*)TitleLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)TitleLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(TitleLabel, laString_CreateFromID(string_TitleString));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)TitleLabel);

    SloganButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SloganButton, 21, 281);
    laWidget_SetSize((laWidget*)SloganButton, 238, 37);
    laWidget_SetBackgroundType((laWidget*)SloganButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SloganButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(SloganButton, laString_CreateFromID(string_Slogan));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SloganButton);

}



