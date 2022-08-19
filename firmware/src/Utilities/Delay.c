/** @file Delay.c
 *  @brief System delay
 *  @author Viet Le
 */
#include "system_config.h"
#include "system_definitions.h"
#include "Delay.h"

/** @brief HardDelay force CPU standing on the HardDelay function.
 *     HardDelay should only call when RTOS is not stared yet,
 *     such as while system initialization
 *  @param [in] unsigned long ms : miliseconds
 *  @param [out] None
 *  @return None
 */
void delay_HardDelay(unsigned long ms)
{
#define SYSTEM_FRE_KHZ			(204000)
#define CYCLE_PER_INSTRUCTION	(4)
#define INSTRUCTION_FRE_KHZ		(SYSTEM_FRE_KHZ/CYCLE_PER_INSTRUCTION)
    long i;
    long j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < INSTRUCTION_FRE_KHZ; j++)
            asm("nop"); //do nothing
    }

    return;
}

/** @brief Soft delay is performed by RTOS,
 *     soft delay just put the calling task to block
 *     it does not hold CPU resource	
 *  @param [in] unsigned long ms : miliseconds
 *  @param [out] None
 *  @return None
 */
void delay_SoftDelay(unsigned long ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);

    return;
}

/** @brief Soft delay is performed by RTOS,
 *     soft delay just put the calling task to block
 *      it does not hold CPU resource	
 *  @param [in] unsigned long ms : miliseconds
 *  @param [out] None
 *  @return None
 */
void delay_MS(unsigned long ms)
{

    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) //scheduler is not started yet
    {
        delay_HardDelay(ms);
    }
    else //scheduler is already started
    {
        vTaskDelay(ms / portTICK_PERIOD_MS);
    }

    return;
}

/* end of file */
