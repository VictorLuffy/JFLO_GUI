/** @file AlarmEventHandler.c
 *  @brief Contains functions for interface with Alarm, handle alarm event in Device
 *  @author Viet Le
 */

#include "system_config.h"
#include "system_definitions.h"
#include "AlarmEventHandler.h"
#include "DeviceTask.h"
#include "HeaterTask.h"
#include "MotorTask.h"
#include "GuiInterface.h"
#include "OperationManager.h"
#include "WaterSupplyCtrl.h"

static E_AlarmStatus gs_AlarmStatus[eNoOfAlarmId];
static bool gs_IsAlarmOccur = false;



/** @brief This operation send alarm event to alarmEventQueue to communicate with DEVICE task
 *  @param [in] uint8_t id: ID of the event	
 *  @param [in] long data: content of the event 
 *  @param [out] None
 *  @return bool : true if event was sent successful, false is event was sent failed
 */
inline bool alarmEvenHandlerInterface_SendEventAlarm(uint8_t alarmId, uint8_t status, uint8_t *data) {
    //return value
    bool rtn = true;
    //event to send
    DEVICE_TASK_ALARM_EVENT_t event;
    event.id = alarmId;
    event.status = status;
    memcpy(event.data, data, 5);
    //send event
    if (xQueueSendToBack(g_alarmEventQueue, &event, 2) != pdPASS) {
        //        SYS_PRINT("\n send failed: event to DEVICE task");
        xQueueReset(g_alarmEventQueue);
        rtn = false;
    }

    return rtn;
}

static bool alarmEvenHandler_CheckStartCondition()
{
    if(gs_AlarmStatus[eBreathingCircuitNotConnectedAlarmId] == eActive)//E001
    {
        return false;
    }
    else if(gs_AlarmStatus[eNonGenuineCircuitAlarmId] == eActive)//E004
    {
        return false;
    }
    else if(gs_AlarmStatus[eCheckConnectionChamberAlarmId] == eActive)//E007
    {
        return false;
    }
    else if(gs_AlarmStatus[eLowTemperatureAlarmId] == eActive)//E010
    {
        return false;
    }
    else if(gs_AlarmStatus[eHighTemperatureAlarmId] == eActive)//E011
    {
        return false;
    }
    else if(gs_AlarmStatus[eHighTemperatureAbnormalityAlarmId] == eActive)//E012
    {
        return false;
    }
    else if(gs_AlarmStatus[eDeviceErrorToAchieveTargetTemperatureAlarmId] == eActive)//E015
    {
        return false;
    }
    else if(gs_AlarmStatus[eStopFunctionAlarmId] == eActive)//E023
    {
        return false;
    }
    else if(gs_AlarmStatus[ePowerNotEnoughAlarmId] == eActive)//E024
    {
        return false;
    }
    else if(gs_AlarmStatus[eCradleBatteryDisconnectedAlarmId] == eActive)//E034
    {
        return false;
    }
    
    int n;
    
    for(n = eFailureOxygenFlowMeasurementFlowSensorAlarmId; n <=  eFailureBlowerAlarmId; n++)
    {
        if(gs_AlarmStatus[n] == eActive)
        {
            return false;
        }
    }
    return true;
}
/** @brief This operation check status of alarm is active or inactive and handle
 *  @param [in] E_AlarmStatus status
 *  @param [out] None
 *  @return None
 */
static void alarmEvenHandler_HandleBreathingCircuitNotConnectedAlarm(E_AlarmStatus status)
{
    if(status == eActive)
    {
                   
        //If it occurs at startup, Do not turn on IH, water pump, blower, tube heater
        MotorTask_DisableMotor();
        
        //Turn off IH, water pump and tube heater
        HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId, .iData = 0};
        HeaterTask_SendEvent(hEvent);
        
        //Continue blower operation until the chamber outlet temperature drops below 30 ? C. Then turn Off blower
      
        //send event to Flow Control Task to prepare to stop motor
        MOTOR_CTRL_EVENT_t event = {.id = eMotorPrepareStopId, .iData = 0};
        MotorTask_SendEvent(event);
        waterSupplyCtrl_TurnOffAndDisablePump();
    }
    else if(alarmEvenHandler_CheckStartCondition() == true)
    {
        
        MotorTask_EnableMotor();
        
        //initializes same as startup then starts operation normally.       
        HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStartId, .iData = 0};
        HeaterTask_SendEvent(hEvent);
        
        MOTOR_CTRL_EVENT_t event = {.id = eMotorStartId, .iData = 0};
        MotorTask_SendEvent(event);       
    }
}

/** @brief This operation check status of alarm is active or inactive and handle
 *  @param [in] E_AlarmStatus status
 *  @param [out] None
 *  @return None
 */
static void alarmEvenHandler_HandleBreathingCircuitChangedAlarm(E_AlarmStatus status)
{
//    if(status == eActive)
//    {
//      /*Adult circuit ? Pediatric circuit 
//            Switch to the Pediatric mode, operation continues
//       
//       Pediatric circuit ? Adult circuit 
//            Switch to the adult mode, operation continues
//       
//       Adult / Pediatric circuit ? Disinfection circuit
//            Stop chamber heating / humidification, breathing circuit heater, blower, and water supply
//       
//       Disinfection circuit ? Adult / Pediatric circuits
//            Stop chamber heating / humidification, breathing circuit heater, blower, and water supply*/
//
//
//    }
//    else{
//        //initializes same as startup then starts operation normaly.
//    }
    
    if(status == eActive)
    {
                   
        //If it occurs at startup, Do not turn on IH, water pump, blower, tube heater
        
        MotorTask_DisableMotor();
        
//        //Turn off IH, water pump and tube heater
//        HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId, .iData = 0};
//        HeaterTask_SendEvent(hEvent);
//        //Continue blower operation until the chamber outlet temperature drops below 30 ? C. Then turn Off blower
//      
//        //send event to Flow Control Task to prepare to stop motor
//        MOTOR_CTRL_EVENT_t event = {.id = eMotorStopId, .iData = 0};
//        MotorTask_SendEvent(event);


    }
    else if(alarmEvenHandler_CheckStartCondition() == true)
    {
        
        MotorTask_EnableMotor();
//        //initializes same as startup then starts operation normally.
//        
//        HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStartId, .iData = 0};
//        HeaterTask_SendEvent(hEvent);
//        
//        MOTOR_CTRL_EVENT_t event = {.id = eMotorStartId, .iData = 0};
//        MotorTask_SendEvent(event);
        
    }
}


/** @brief This operation check alarm event from alarm event queue and process them
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void alarmEvenHandler_HandleEvent(void)
{
    DEVICE_TASK_ALARM_EVENT_t event;
    //wait for queue event
    if (xQueueReceive(g_alarmEventQueue, &event, 0) == pdTRUE) //wait 0 tick (do not wait)
    {
      
        gs_AlarmStatus[event.id] = event.status;
        
        switch (event.id)
        {
        //Respiratory circuit abnormality 
        case eBreathingCircuitNotConnectedAlarmId://E001
        {
            alarmEvenHandler_HandleBreathingCircuitNotConnectedAlarm(event.status);  
            break;
        }
        
        case eCheckLeakAlarmId://E002
        {  
            //do nothing
            break;
        }
        
        case eCheckBlockageAlarmId://E003
        {  
            //do nothing
            break;
        }
       
        case eNonGenuineCircuitAlarmId://E004
        {  
            /*
            ?If it occurs at startup
                Do not turn on IH, water pump, blower, tube heater
            ? If this occurs when the device is operating 
                Turn off IH, water pump, blower and tube heater
           */
            waterSupplyCtrl_TurnOffAndDisablePump();
            MotorTask_DisableMotor();
            if(event.status == eActive)
            {
                HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId, .iData = 0};
                HeaterTask_SendEvent(hEvent);

                MOTOR_CTRL_EVENT_t mEvent = {.id = eMotorStopId, .iData = 0};
                MotorTask_SendEvent(mEvent);
            }
            break;
        }
        
        case eBreathingCircuitChangedAlarmId://E005
        {  
            alarmEvenHandler_HandleBreathingCircuitChangedAlarm(event.status);
            break;
        }
//        
//        case eExpiredCircuitAlarmId://E006
//        {  
//            //Operation continues
//            //do nothing
//            break;
//        }
        
        //Chamber abnormality
        case eCheckConnectionChamberAlarmId://E007
        {  
            /*
            ?If it occurs at startup
                Do not turn on IH, water pump, blower, tube heater
            ? If this occurs when the device is operating 
                Turn off IH, water pump, blower and tube heater
           */
            waterSupplyCtrl_TurnOffAndDisablePump();
            MotorTask_DisableMotor();
            if(event.status == eActive)
            {
                HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId, .iData = 0};
                HeaterTask_SendEvent(hEvent);

                MOTOR_CTRL_EVENT_t mEvent = {.id = eMotorStopId, .iData = 0};
                MotorTask_SendEvent(mEvent);
            }
            
            break;
        }
        
//        case eNoMoreWaterInChamberAlarmId://E008
//        {  
//            //Continue operating normally
//            //do nothing
//            break;
//        }
//        
//        case eRunOutOfWaterAlarmId://E009
//        {  
//            //Continue operating normally
//            break;
//        }
//        
        //temperature abnormality
        case eLowTemperatureAlarmId://E010
        {  
            //if environmentTemp > 18.0
            if(event.data[eMessageTypeDataIdx] == eE010AmbientTempEqualOrOverThan18CelsiusDegree)
            {
                HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId, .iData = 0};
                HeaterTask_SendEvent(hEvent);

                MOTOR_CTRL_EVENT_t event = {.id = eMotorStopId, .iData = 0};
                MotorTask_SendEvent(event);
                
                waterSupplyCtrl_TurnOffAndDisablePump();
            }
            //Continue operating normally if environment temperature < 18 C (it is not error)
            break;
        }
        case eHighTemperatureAlarmId://E011
        {  
            //if(environmentTemp < setTemp)
            if(event.data[eMessageTypeDataIdx] == eE011AmbientTempLowerThanSettingTemp)
            {
                HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId, .iData = 0};
                HeaterTask_SendEvent(hEvent);

                MOTOR_CTRL_EVENT_t event = {.id = eMotorStopId, .iData = 0};
                MotorTask_SendEvent(event);
                
                waterSupplyCtrl_TurnOffAndDisablePump();
            }
            //Continue operating normally if environment temperature < 18 C (it is not error)
            break;
        }
        case eHighTemperatureAbnormalityAlarmId://E012
        {  

            //if(warmingup = false) && (settingChange == false)
            {
                HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId, .iData = 0};
                HeaterTask_SendEvent(hEvent);

                MOTOR_CTRL_EVENT_t event = {.id = eMotorStopId, .iData = 0};
                MotorTask_SendEvent(event);
                
                waterSupplyCtrl_TurnOffAndDisablePump();
            }

            break;
        }
        case eRoomTemperatureLowToAchieveTargetTemperatureAlarmId://E013
        {
            //do nothing
            break;
        }
        
        case eCheckOperatingConditionsAlarmID://E014
        {
            //turn off heater control
            HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId, .iData = 0};
            HeaterTask_SendEvent(hEvent);
                
            //if(environmentTemp >= 42.0)
            if (event.data[eMessageTypeDataIdx] == eE011AmbientTempEqualOrOverThan42CelsiusDegree)
            {
                //turn off motor control
                MOTOR_CTRL_EVENT_t event = {.id = eMotorStopId, .iData = 0};
                MotorTask_SendEvent(event);
            }
            
            waterSupplyCtrl_TurnOffAndDisablePump();
                
            break;
        }
        
        case eDeviceErrorToAchieveTargetTemperatureAlarmId://E015
        {
            HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId, .iData = 0};
            HeaterTask_SendEvent(hEvent);

            MOTOR_CTRL_EVENT_t event = {.id = eMotorStopId, .iData = 0};
            MotorTask_SendEvent(event);     
            
            waterSupplyCtrl_TurnOffAndDisablePump();
            
            break;
        }
        
        //Abnormal oxygen concentration
        case eOxygenHighAlarmId://E016
        case eOxygenLowAlarmId://E017
            //do nothing
            break;
            
        //Abnormal device position
        case eDevicePostureAbnormalAlarmId://E018     
        case eDevicePostureBadAlarmId://E019
            waterSupplyCtrl_TurnOffAndDisablePump();
            break;
            
       // abnormal Battery
        case eSwitchToBatteryModeAlarmId://E020
        case eBatteryLowAlarmId://E021
            //do nothing
            break;
            
        case eBatteryGetsRunOutAlarmId://E022
        {

            waterSupplyCtrl_TurnOffAndDisablePump();
            break;
        }

        case ePowerNotEnoughAlarmId://E024
            MotorTask_DisableMotor();
        case eStopFunctionAlarmId://E023  
        {
            waterSupplyCtrl_TurnOffAndDisablePump();//do nothing
                       
            HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId, .iData = 0};
            HeaterTask_SendEvent(hEvent);

            MOTOR_CTRL_EVENT_t event = {.id = eMotorStopId, .iData = 0};
            MotorTask_SendEvent(event);                           
            break;
        }
        
        //SpO2
        case eSpO2FailedAlarmId://E025
        case eSpO2LowAlarmId://E026
        case eSpO2SensorProbeAlarmId://E027
        case eSpO2SignalLossAlarmId://E028
        case eSpO2SensorUnpluggedAlarmId://E029
            //do nothing
            break;
            
        case eWaterSupplyCheckAlarmId://E030
        case eWaterTankConnectionCheckAlarmId://E031
        case eMainUnitBatteryDisconnectedAlarmId://E034
        {
            waterSupplyCtrl_TurnOffAndDisablePump();
            break;
        }
        
        case eFailureOxygenFlowMeasurementFlowSensorAlarmId://E100
        case eFailureAirFlowMeasurementFlowSensorAlarmId://E101
        case eFailureChamberTemperatureSensorAlarmId://E102
        case eFailureCoilTemperatureSendorAlarmId://E103
        case eFailureBreathingCircuitOutTemperatureSensorAlarmId://E104
        case eFailureEnvironmentSensorAlarmId://E105
        case eFailureCurrentSensorAlarmId://E106
        case eFailureBlowerAlarmId://E107
        case eFailureWaterLevelSensorAlarmId://E109
        case eFailureExternalFlashMemoryAlarmId://E110
        {
            MotorTask_DisableMotor();
            HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId, .iData = 0};
            HeaterTask_SendEvent(hEvent);

            MOTOR_CTRL_EVENT_t event = {.id = eMotorStopId, .iData = 0};
            MotorTask_SendEvent(event);   
            break;
            waterSupplyCtrl_TurnOffAndDisablePump();
        }
        case eFailureAccelerationSensorAlarmId://E111
        case eMainUnitBatteryCommunicationErrorAlarmId://E113
        case eCradleBatteryCommunicationErrorAlarmId://E114
        case eCradleCommunicationErrorAlarmId://E115
        case eMainMCUFailedOrOutOfControlAlarmId://E116
        case eESP32FailedAlarmId://E117
        case eBreathingCircuitHeaterWireBrokenAlarmId://E118
            waterSupplyCtrl_TurnOffAndDisablePump();
            break;
                
        default:
            break;
        }
        
        //update alarm status
        gs_IsAlarmOccur = false;
        int alarmId = 0;
        for (alarmId = eFirsAlarmId; alarmId < eLastAlarmId; alarmId++)
        {
            if(gs_AlarmStatus[alarmId] == eActive)
            {
                gs_IsAlarmOccur = true;
                break;
            }
        }
    }
    static uint32_t s_counter = 0;
    if(gs_AlarmStatus[eStopFunctionAlarmId] == eActive)
    {
        if(s_counter >= 180*1000/10)
        {
            SYS_PRINT("E023 stop function >> auto power off\n");
            //display pop up on GUI
            guiInterface_SendEvent(eGuiTurnOffMachineId, 0);
            OperationMgr_PowerOffSystem();
        }
        else{
            s_counter++;
        }
    }
    else{
        s_counter = 0;
    }
    return;
}

/** @brief This operation get status of alarms
 *  @param [in] None
 *  @param [out] None
 *  @return true if there is any a alarm occur, false if there is no alarm
 */
bool alarmEventHandler_GetAlarmsStatus()
{
    return gs_IsAlarmOccur;
}

/* end of file */