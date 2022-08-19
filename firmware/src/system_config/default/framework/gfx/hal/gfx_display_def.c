#include "gfx/hal/inc/gfx_display.h"
#include "gfx/hal/inc/gfx_common.h"


GFX_DisplayInfo GFX_DisplayInfoList[] =
{
    {
	    "",  // description
		GFX_COLOR_MODE_RGB_565, // default color mode
		{
			0,  // x position (always 0)
			0,  // y position (always 0)
			480,  // display width
			272, // display height
		},
		{
		    24,  // data bus width
		    {
				41,  // horizontal pulse width
				40,  // horizontal back porch
				5,  // horizontal front porch
		    },
		    {
				12,  // vertical pulse width
				8,  // vertical back porch
				8,  // vertical front porch
		    },
			0,  // inverted left shift
		},
	},
};