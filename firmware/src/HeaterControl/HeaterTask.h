/* ************************************************************************** */
/** @file [HeaterTask.h]
 *  @brief {This file contain source code necessary for Heater Control. It implements
 * a Heater Control Task, handle event come to Heater Control Task, initialize dependency
 * components such as Temperature sensors, PWM for IH circuit, ... and control 
 * them during operation }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */



#ifndef _HEATER_TASK_H    /* Guard against multiple inclusion */
#define _HEATER_TASK_H


/* This section lists the other files that are included in this file.
 */
#include <stdbool.h>
#include "FreeRTOS.h"
#include "queue.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
    typedef enum {
        eWarmingUp,
        eWarmingUpFinished,
        eNoneWarmingUp,               
    }E_WarmingUpStatus;
    
    /** @brief define operation mode */
    typedef enum {
        eHeaterAdultOrChildOperationMode,
        eHeaterMoistHeatDisinfectionMode,
        eDryHeatSterilizationMode,
    } E_HeaterOperationMode;
    
    /** @brief define phase of Moist Heat Disinfection operation mode */
        typedef enum {
        eWarmingUpPhase,
        eDisinfectPhase,
        eToDryPhase,
        eDisinfectionFinished,
    } E_DisinfectionPhase;
    
   /** @brief define heating state of Temperature and Humidity control */
    typedef enum {
        ePreHeating,
        eAfterPreHeating,
        eNoneHeating,               
    }E_HeatingState;
    
    
    /** @brief define state machine for Heater control task */
    typedef enum {

        /**< Idle state. In this state, Heater has been turned off, there are no 
         * control here */
        eHeaterIdleState = 0,

        /**< Start state. In this state, Start IH Power control, BME280 sensor, thermalmiter
         sensor before running the Heater controller */
        eHeaterStartState,

        /**< Prepare before operate. In this state, Every module relate to Heater Task
         * has been started and prepare to go to operate state. Depend on operation mode, 
         * this state will do some peparation before enter operation */
        eHeaterPreOperateState,

        /**< Operation state. In this state, Heater components are ready to use. A
         * close loop for Heater control will implement here */
        eHeaterOperateState,

        /**< Stop state. In this state, IH power control, BME280 and Thermalmeter 
         * will turm off */
        eHeaterStopState,

        /**< ERROR state. In this state, Heater task has detected some error and should report
         * to ALARM task in this state. After reporting, Heater should return to IDLE state */
        eHeaterErrorState,
                

    } E_HeaterState;

    /** @brief define all event ID that used to communicate with Heater Control Task */
    typedef enum {
        eHeaterStartId = 0, /*start Heater Control Task*/
        eHeaterStopId, /*stop Heater Control Task*/
        eHeaterTempId, /*change setting temperature*/
        eHeaterStopWaterPumpId,
        eHeaterStartWaterPumpId,
        eHeaterFlowRateSettingId,       
        //define event id here
    } E_HeaterEventID;

    /** @brief structure for an item in HEATER CONTROL event Queue. Any other task
     would like to send event to HEATER CONTROL task should follow that structure */
    typedef struct {
        E_HeaterEventID id;

        union {
            float fData;
            int32_t iData;
        };
    } HEATER_CTRL_EVENT_t;

    /** @brief structure to store all public data to share*/
    typedef struct {
        float chamberOutTargetTemp; /**< temperature of GAS at Chamber outlet */
        float chamberOutTemp; /**< temperature of GAS at Chamber outlet */
        float breathCircuitOutTemp;
        float breathCircuitOutTargetTemp;
        float IHcoreTemp; /**< Temperature at the surface of IH core */
        float powerOut; /**< Power out, in percentage, currently supply for IH core */
        float currentPower; /**< current power, in W, for Humidity control */
        float targetPower;  /**< target power, in W, for Humidity control */
        float bumperFrequency;  /**< frequency control bumper, in Hz, for Humidity control */
        float setTemp;
        float envTemp;
        float envHum;
        E_WarmingUpStatus warmingUpState; 
    } HEATER_PUBLIC_DATA_t;

    /** @brief Support API to send event to HEATER CONTROL task from other tasks
     *  @param [in]     HEATER_CTRL_EVENT_t event    event to send
     *  @param [out]  None
     *  @return None
     *  @retval true sending event to HEATER CONTROL queue OK
     *  @retval false sending event to HEATER CONTROL queue ERROR
     */
    inline bool HeaterTask_SendEvent(HEATER_CTRL_EVENT_t event);

    /** @brief Get data sharing from Heater Control such as: Temperature at Chamber
     * outlet, Temperature at IH Core surface, Current power supply for IH core 
     * in percent ...
     *  @param [in]  None   
     *  @param [out]  HEATER_PUBLIC_DATA_t* data   external pointer to store data
     *  @return None
     *  @retval true    getting data OK
     *  @retval false   getting data Failed
     */
    bool HeaterTask_GetPublicData(HEATER_PUBLIC_DATA_t* data);

    /** @brief Initialize HEATER CONTROL task and all components that dependency, 
     * including I2C4 for Temperature & Humidity sensor communication; PWM for IH 
     * circuit; PWM for Water Bumper control
     * This function should be called 1 times at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void HeaterTask_Initialize(void);


    /** @brief Create a Real Time task to do Heater control. 
     *  @param [in]     None
     *  @param [out]    None
     *  @return None
     */
    
    
    /** @brief Function to run HEATER CONTROL task. This function needs to be called
    * in a loop
    *  @param [in]     None
    *  @param [out]    None
    *  @return None
    */
    void HeaterTask_Run(void);
    
    //void HeaterTask_Create(void);
    
    
    void HeaterTask_TurnOnWaterPump(void);
    
    
    void HeaterTask_TurnOffWaterPump(void);
    
    
    //void HeaterTask_HandleEvent(void);
    //void HeaterTask_Maintain(void);
    void HeaterTask_SetState(E_HeaterState state);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _HEATER_TASK_H */

/* *****************************************************************************
 End of File
 */
