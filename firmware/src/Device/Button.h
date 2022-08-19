/* ************************************************************************** */
/** @file [Button.h]
 *  @brief {Supply APIs to get pressed state of the buttons
 *  @author {nguyen truong}
 */
/* ************************************************************************** */




#ifndef _BUTTON_H    /* Guard against multiple inclusion */
#define _BUTTON_H


/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>



typedef enum
{
	eButtonIdle,
	ePress,
	ePressHold,
}E_ButtonStateEvent;

typedef enum
{
    ePowerButton,
	eFirstButton = ePowerButton,
    eMuteButton,
	eLastButton = eMuteButton,
	eNumOfButton,
}E_ButtonID;




/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /** @brief Function to initialize original state of button 
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void button_Init();

    /** @brief Function to check state of all buttons
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void button_UpdateStateOfAllButton();

    /** @brief Function to reset state of all buttons
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */

    E_ButtonStateEvent button_GetButtonState(E_ButtonID ID);
    
    void button_Handle();
    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _BUTTON_H */

/* *****************************************************************************
 End of File
 */
