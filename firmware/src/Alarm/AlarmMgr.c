// ************************************************************************** */
/** @file [AlarmMgr.c]
 *  @brief {Manager of JFLO Alarm.
 * It contains definition alarm functions and the checking, updating process}
 *  @author {Cuong Le}
 */
/* History                                                                    */
/*============================================================================*/
/* [Number]     [Date]              [Editor]                    [Explaination]*/
/*-----------+-------------------+---------------------+----------------------*/
/*  001         01/17/2022          Thanh Duong                 modify        */
/*  Content: Modify E006 + E10 - E11 + E012                                   */
/*  002         01/18/2022          Thanh Duong                 modify`       */
/*  Content: Modify E020 + Add E030 - E032                                    */
/*  003         01/24/2022          Thanh Duong                 modify        */
/*  Content: seperate E016-E017                                               */
/*  004         02/22/2022 +        Thanh Duong                 modify        */
/*  Content: change E010-E011 remove isSetFlowChangeFlag                      */
/*           & isSetTemperatureFlag                                           */
/*           Change E100 - E101 - E105                                        */
/*  005         04/04/2022          Thanh Duong                 modify        */
/*  Content: Update Alarms following specs v0.08                              */
/*  006         04/29/2022          Thanh Duong                 modify        */
/*  Content: Update Alarms following specs v0.09                              */
/*  007         06/24/2022          Thanh Duong                 modify        */
/*  Content: Update Alarms following specs v0.11                              */
/* ************************************************************************** */


#include "AlarmMgr.h"
#include "AlarmInterface.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "portmacro.h"
#include "DeviceTask.h"
#include "system_config.h"
#include "Setting.h"
#include "AlarmExpression.h"


/** @brief Counter for Alarm functions  */
static uint32_t s_OxygenConcentrationIsHigherMediumPriorityCounter = 0;
static uint32_t s_OxygenConcentrationIsHigherHighPriorityCounter = 0;
static uint32_t s_OxygenConcentrationIsLowerMediumPriorityCounter = 0;
static uint32_t s_OxygenConcentrationIsLowerHighPriorityCounter = 0;
static uint32_t s_TargetTemperatureCannotBeAchievedTemperatureLowerCounter = 0;
static uint32_t s_TargetTemperatureCannotBeAchievedTemperatureCounter = 0;
static uint32_t s_TargetTemperatureCannotBeAchievedTemperatureHigherCounter = 0;
static uint32_t s_ConnectionOfChamberCounter = 0;
static uint32_t s_NoMoreWaterChamberCounter = 0;
static uint32_t s_clearNoMoreWaterChamberCounter = 0;
static uint32_t s_DevicePostureAbnormalCounter = 0; 
//static uint32_t s_TemperatureCounter = 0;
//static uint32_t s_RunOutOfWaterCounter = 0;
static uint32_t s_CheckForLeaksCounter = 0;  
static uint32_t s_CheckForBlockageFlowCheckCounter = 0;
static uint32_t s_CheckForBlockageBlowerSpeedMoreThanUpperLimitCounter = 0;
static uint32_t s_CheckForBlockageBlowerSpeedLessThanUpperLimitCounter = 0;
static uint32_t s_CheckForSpo2LossPulseCounter = 0;
static uint32_t s_CheckForSpo2SensorProbeCounter = 0;
static uint32_t s_FailureCoilTemperatureSensorCounter = 0;
static uint32_t s_FailureChamberTemperatureSensorCounter = 0;
static uint32_t s_FailureBreathingCircuitOutTemperatureSensorCounter = 0;
static uint32_t s_TimeNotChangeFlowTempCounter = 0;
static uint32_t s_TimeStartHighCounter1 = 0;
static uint32_t s_TimeStartHighCounter = 0;
static uint32_t s_TimeStartLowCounter = 0;
static uint32_t s_TimeStartAirflowAbnormalCounter = 0;
static uint32_t s_CheckBlockageTimeStartCounter = 0;
static uint32_t s_PostureCounter = 0;
static uint32_t s_CheckWaterSupplyCounter = 0;
static uint32_t s_AirFlowAbnormalCounter = 0;
// Add by Thanh Duong
static uint32_t s_checkNoMoreWaterChangeSettingTimeoutCounter = 0;
static uint32_t s_checkForBlockageChangeSettingTimeoutCounter = 0;
static uint32_t s_ChangeSettingTimeoutCounter = 0;
static uint32_t s_CurrentIHPowerHigher10PerCentTargetPowerCounter = 0;
static uint32_t s_CurrentIHPowerLower10PerCentTargetPowerCounter = 0;
__attribute__((section(".ddr_data"), space(prog))) static float s_E010Temperature[NUM_SAMPLE_IN_5MINS] = {};
__attribute__((section(".ddr_data"), space(prog))) static float s_E011Temperature[NUM_SAMPLE_IN_5MINS] = {};
__attribute__((section(".ddr_data"), space(prog))) static float s_E012Temperature[NUM_SAMPLE_IN_2MINS] = {};
__attribute__((section(".ddr_data"), space(prog))) static float s_ChamberCoilTemperature[EVT_TEMP_NUM_SAMPLE_IN_1SECOND] = {};
__attribute__((section(".ddr_data"), space(prog))) static float s_ChamberOutTemperature[CHAMBER_OUT_TEMP_NUM_SAMPLE_IN_10SECOND] = {};
__attribute__((section(".ddr_data"), space(prog))) static float s_IHPower[IH_POWER_NUM_SAMPLE_IN_1MINS] = {};
static float s_ambientTemperature[NUM_SAMPLE_IN_2MINS] = {};
/** @brief Saving the last Temperature Setting */
static float pre_TemperatureSetting = 0;
/** @brief Saving the last Flow Setting */
static float pre_FlowSetting = 0;

/** @brief Declare alarm list */
#ifndef UNIT_TEST
__attribute__((section(".ddr_data"), space(prog))) static ALARM_STAT_t s_alarmList[eNoOfAlarmId];
#else
ALARM_STAT_t s_alarmList[eNoOfAlarmId];
#endif

/** @brief The current Alarm Monitors. They just be used in this file for finding alarm process */
static ALARM_MONITOR_t s_currentAlarmMonitor;

/** @brief Alarm Monitors which are updated immediately. After the circle, this value is update to s_currentAlarmMonitor*/
volatile ALARM_MONITOR_t stAlarmMonitor;

///** @brief Flag is raised if the time changing temp or flow is under 5 minutes */
//volatile bool timeSettingChange_Flag = Flag_On;                

/** @brief MUTEX to protect accessing Alarm set of values */
extern SemaphoreHandle_t s_alarmMonitorMutex;



/** @brief Sets the current status and previous status of all alarms to 
 *         default value (eInActive) except Anypal alarm
 *  @param [in] None
 *  @param [out] : None
 *  @return None
 */
void alarmMgr_ClearAllAlarm(void)
{
    int i;
    for (i = eFirsAlarmId; i < eLastAlarmId; i++)
    {
        s_alarmList[i].previousStatus = eInactive;
        s_alarmList[i].currentStatus = eInactive;
    }
    static uint32_t s_OxygenConcentrationIsHigherCounter = 0;
static uint32_t s_OxygenConcentrationIsLowerCounter = 0;
static uint32_t s_TargetTemperatureCannotBeAchievedTemperatureLowerCounter = 0;
static uint32_t s_TargetTemperatureCannotBeAchievedTemperatureHigherCounter = 0;
static uint32_t s_ConnectionOfChamberCounter = 0;
static uint32_t s_NoMoreWaterChamberCounter = 0;
static uint32_t s_DevicePostureAbnormalCounter = 0; 
//static uint32_t s_TemperatureCounter = 0;
//static uint32_t s_RunOutOfWaterCounter = 0;
static uint32_t s_CheckForLeaksCounter = 0;  
static uint32_t s_CheckForBlockageCounter = 0;
static uint32_t s_CheckForSpo2LossPulseCounter = 0;
static uint32_t s_CheckForSpo2SensorProbeCounter = 0;
static uint32_t s_FailureCoilTemperatureSensorCounter = 0;
static uint32_t s_FailureBreathingCircuitOutTemperatureSensorCounter = 0;
static uint32_t s_TimeNotChangeFlowTempCounter = 0;
static uint32_t s_TimeStartCounter = 0;
static uint32_t s_PostureCounter = 0;
static uint32_t s_CheckWaterSupplyCounter = 0;
static uint32_t s_AirFlowAbnormalCounter = 0;
    return;
}

/** @brief Update current  value of all alarm monitor for device task
 *  @param [in] None
 *  @param [out] : None
 *  @return None
 */
void alarmMgr_UpdateAlarmMonitor(void)
{
    //if(xSemaphoreTake(s_alarmMonitorMutex, 10) == pdTRUE) //Wait 10 ticks
    //{
	// = stAlarmMonitor;
	//xSemaphoreGive(s_alarmMonitorMutex);
    //}
    DeviceTask_GetAlarmMonitorStruct(&s_currentAlarmMonitor);
    return;
}

/** @brief Check status of The breathing circuit is not connected or not connected 
 * firmly to the device
 * Trigger this alarm when using a API to read the breathing circuit type, and 
 * if the result is not "pediatric", "adult", or "disinfecting", then the breathing
 * circuit is not connected
 * Clear this alarm when the breathing circuit type can be read and the read breathing 
 * circuit type is either "for pediatric ", "for adults", or "for disinfection" 
 * or Power down and restart by power button
 *  @param [in] E_BreathingCircuitType BreathingCircuitType The Type of Breathing Circuit, can be:
 * - type_Pediatric
 * - type_Adult
 * - type_Disinfection
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckBreathingCircuitNotConnectedAlarmStatus(E_BreathingCircuitType BreathingCircuitType,
                                                                  bool isChamberConnected)//E001
{
  static uint8_t r_Counter = 0;
  static uint8_t s_counter = 0;
    // DEBUG
    // Invalid time check
    if (isChamberConnected == false) 
    {
        return;
    }
    if(s_alarmList[eBreathingCircuitNotConnectedAlarmId].currentStatus == eInactive)
    {
        if((BreathingCircuitType != eTypePediatric) && (BreathingCircuitType != eTypeAdult) && (BreathingCircuitType != eTypeDisinfection))
        {  
           s_counter++;
           if(s_counter >= (1000/50))
           {
              s_counter = 0;
              s_alarmList[eBreathingCircuitNotConnectedAlarmId].currentStatus = eActive;
              if(s_currentAlarmMonitor.currentO2Flow > 1.0)
              {
                  s_alarmList[eBreathingCircuitNotConnectedAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
              }
              else
              {
                  s_alarmList[eBreathingCircuitNotConnectedAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
              }
           }
           return;
        }
        else{
            s_counter = 0;
        }
    }

    else if(s_alarmList[eBreathingCircuitNotConnectedAlarmId].currentStatus == eActive)
    {
        if((BreathingCircuitType == eTypePediatric) || (BreathingCircuitType == eTypeAdult) || (BreathingCircuitType == eTypeDisinfection))
        {  
            
            r_Counter++;
        }
        if (r_Counter >= 1000/50)
        {
            s_alarmList[eBreathingCircuitNotConnectedAlarmId].currentStatus = eInactive;
            r_Counter = 0;
        }
    }
    return; 
}

/** @brief Check status of the Leaks
 * Trigger this alarm when the blower rotation speed is below the lower limit for
 *  the set flow rate for 5 seconds.
 * Clear this alarm when blower speed is returns to the specified range
 * or Power down and restart by power button.
 *  @param [in] : int setFlow Setting flow
 *  @param [in] : float currentBlowerRotationSpeed Blower Rotation Speed
 *  @param [out] : None
 *  @return None
 */
//static void alarmMgr_CheckForLeaksAlarmStatus(int setFlow,
//                                              float currentBlowerRotationSpeed,
//                                              E_BreathingCircuitType BreathingCircuitType,
//                                              bool isChamberConnected)//E002
//{
//    // DEBUG
//    // Add Invalid time check
//    bool noBreathingCircuit = false;
//    if((BreathingCircuitType != eTypePediatric) && (BreathingCircuitType != eTypeAdult) && (BreathingCircuitType != eTypeDisinfection))
//    {
//        noBreathingCircuit = true;
//    }
//    if((isChamberConnected == false) || (noBreathingCircuit == true))
//    {
//        return;
//    }
//    switch (setFlow)
//    {
//        case 15:
//            if(currentBlowerRotationSpeed < 1700)
//            {
//                if(s_CheckForLeaksCounter >= CHECKFORLEAKS_ALARM_TIMEOUT)
//                {
//                    s_alarmList[eCheckLeakAlarmId].currentStatus = eActive;
//                    return;
//                }
//                s_CheckForLeaksCounter++;
//            }
//            else
//            {
//                s_CheckForLeaksCounter = 0;
//                s_alarmList[eCheckLeakAlarmId].currentStatus = eInactive;
//            }
//            break;

//        case 20:
//            if(currentBlowerRotationSpeed < 2400)
//            {
//                if(s_CheckForLeaksCounter >= CHECKFORLEAKS_ALARM_TIMEOUT)
//                {
//                    s_alarmList[eCheckLeakAlarmId].currentStatus = eActive;
//                    return;
//                }
//                s_CheckForLeaksCounter++;
//            }
//            else
//            {
//                s_CheckForLeaksCounter = 0;
//                s_alarmList[eCheckLeakAlarmId].currentStatus = eInactive;
//            }
//            break;

//        case 25:
//            if(currentBlowerRotationSpeed < 2900)
//            {
//                if(s_CheckForLeaksCounter >= CHECKFORLEAKS_ALARM_TIMEOUT)
//                {
//                    s_alarmList[eCheckLeakAlarmId].currentStatus = eActive;
//                    return;
//                }
//                s_CheckForLeaksCounter++;
//            }
//            else
//            {
//                s_CheckForLeaksCounter = 0;
//                s_alarmList[eCheckLeakAlarmId].currentStatus = eInactive;
//            }
//            break;

//        case 30:
//            if(currentBlowerRotationSpeed < 3300)
//            {
//                if(s_CheckForLeaksCounter >= CHECKFORLEAKS_ALARM_TIMEOUT)
//                {
//                    s_alarmList[eCheckLeakAlarmId].currentStatus = eActive;
//                    return;
//                }
//                s_CheckForLeaksCounter++;
//            }
//            else
//            {
//                s_CheckForLeaksCounter = 0;
//                s_alarmList[eCheckLeakAlarmId].currentStatus = eInactive;
//            }
//            break;

//        case 35:
//            if(currentBlowerRotationSpeed < 3900)
//            {
//                if(s_CheckForLeaksCounter >= CHECKFORLEAKS_ALARM_TIMEOUT)
//                {
//                    s_alarmList[eCheckLeakAlarmId].currentStatus = eActive;
//                    return;
//                }
//                s_CheckForLeaksCounter++;
//            }
//            else
//            {
//                s_CheckForLeaksCounter = 0;
//                s_alarmList[eCheckLeakAlarmId].currentStatus = eInactive;
//            }
//            break;

//        case 40:
//            if(currentBlowerRotationSpeed < 4700)
//            {
//                if(s_CheckForLeaksCounter >= CHECKFORLEAKS_ALARM_TIMEOUT)
//                {
//                    s_alarmList[eCheckLeakAlarmId].currentStatus = eActive;
//                    return;
//                }
//                s_CheckForLeaksCounter++;
//            }
//            else
//            {
//                s_CheckForLeaksCounter = 0;
//                s_alarmList[eCheckLeakAlarmId].currentStatus = eInactive;
//            }
//            break;

//        case 45:
//            if(currentBlowerRotationSpeed < 5100)
//            {
//                if(s_CheckForLeaksCounter >= CHECKFORLEAKS_ALARM_TIMEOUT)
//                {
//                    s_alarmList[eCheckLeakAlarmId].currentStatus = eActive;
//                    return;
//                }
//                s_CheckForLeaksCounter++;
//            }
//            else
//            {
//                s_CheckForLeaksCounter = 0;
//                s_alarmList[eCheckLeakAlarmId].currentStatus = eInactive;
//            }
//            break;

//        case 50:
//            if(currentBlowerRotationSpeed < 5700)
//            {
//                if(s_CheckForLeaksCounter >= CHECKFORLEAKS_ALARM_TIMEOUT)
//                {
//                    s_alarmList[eCheckLeakAlarmId].currentStatus = eActive;
//                    return;
//                }
//                s_CheckForLeaksCounter++;
//            }
//            else
//            {
//                s_CheckForLeaksCounter = 0;
//                s_alarmList[eCheckLeakAlarmId].currentStatus = eInactive;
//            }
//            break;

//        case 55:
//            if(currentBlowerRotationSpeed < 6100)
//            {
//                if(s_CheckForLeaksCounter >= CHECKFORLEAKS_ALARM_TIMEOUT)
//                {
//                    s_alarmList[eCheckLeakAlarmId].currentStatus = eActive;
//                    return;
//                }
//                s_CheckForLeaksCounter++;
//            }
//            else
//            {
//                s_CheckForLeaksCounter = 0;
//                s_alarmList[eCheckLeakAlarmId].currentStatus = eInactive;
//            }
//            break;

//        case 60:
//            if(currentBlowerRotationSpeed < 6800)
//            {
//                if(s_CheckForLeaksCounter >= CHECKFORLEAKS_ALARM_TIMEOUT)
//                {
//                    s_alarmList[eCheckLeakAlarmId].currentStatus = eActive;
//                    return;
//                }
//                s_CheckForLeaksCounter++;
//            }
//            else
//            {
//                s_CheckForLeaksCounter = 0;
//                s_alarmList[eCheckLeakAlarmId].currentStatus = eInactive;
//            }
//            break;

//        default:
//            break;
//    }
//    return;
//}

/** @brief Check Upper limitation of the blower speed
 *  @param [in] : int setFlow Setting flow
 *  @param [in] : int currentFlow
 *  @param [in] : int blowerRotationSpeedUpperLimit
 *  @param [in] : float currentBlowerRotationSpeed Blower Rotation Speed
 *  @param [out] : None
 *  @return None
 */
#ifndef UNIT_TEST
static void CheckForBlockageCheckUpperLimitAlarmStatus( int setFlow,
                                                        int currentFlow,
                                                        float blowerRotationSpeedUpperLimit,
                                                        float currentBlowerRotationSpeed)
#else
void CheckForBlockageCheckUpperLimitAlarmStatus( int setFlow,
                                                        int currentFlow,
                                                        float blowerRotationSpeedUpperLimit,
                                                        float currentBlowerRotationSpeed)
#endif
{
    if(currentBlowerRotationSpeed >= blowerRotationSpeedUpperLimit)
    {
        if (s_alarmList[eCheckBlockageAlarmId].currentStatus == eInactive)
        {
            if(s_CheckForBlockageBlowerSpeedMoreThanUpperLimitCounter >= CHECKFORBLOCKAGE_ALARM_TIMEOUT)
            {
                s_alarmList[eCheckBlockageAlarmId].currentStatus = eActive;
                return;
            }
            s_CheckForBlockageBlowerSpeedMoreThanUpperLimitCounter++;
        }
            s_CheckForBlockageBlowerSpeedLessThanUpperLimitCounter = 0;
    }
    else
    {
        if (s_alarmList[eCheckBlockageAlarmId].currentStatus == eActive)
        {
            if(s_CheckForBlockageBlowerSpeedLessThanUpperLimitCounter >= CHECKFORBLOCKAGE_ALARM_TIMEOUT)
            {
                if (currentFlow >= 0.95 * setFlow)
                {
                    s_CheckForBlockageFlowCheckCounter = 0;
                    s_CheckForBlockageBlowerSpeedMoreThanUpperLimitCounter = 0;
                    s_CheckForBlockageBlowerSpeedLessThanUpperLimitCounter = 0;
                    s_alarmList[eCheckBlockageAlarmId].currentStatus = eInactive;
                    return;
                }
            }
            s_CheckForBlockageBlowerSpeedLessThanUpperLimitCounter++;
        }
        
        s_CheckForBlockageBlowerSpeedMoreThanUpperLimitCounter = 0;
    }
}

/** @brief Check status of the Blockage
 * Trigger this alarm when the blower rotation speed is below the upper limit for
 * the set flow rate for 5 seconds
 * Clear this alarm when blower speed is returns to the specified range 
 * Or Power down and restart by power button
 *  @param [in] : int setFlow Setting flow
 *  @param [in] : float currentBlowerRotationSpeed Blower Rotation Speed
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckForBlockageAlarmStatus(float setFlow,
                                                 float currentFlow,
                                                 float currentBlowerRotationSpeed,
                                                 E_TreatmentMode machineMode,
                                                 E_BreathingCircuitType BreathingCircuitType,
                                                 bool isChamberConnected,
                                                 float blowerControlValue,
                                                 bool isACConnected)//E003
{
    // Add Invalid time check
    bool noBreathingCircuit = false;
    static bool settingChangedFlag = false;
    static int flowSettingTemperary = -1;
    
    if (flowSettingTemperary == -1) // 1
    {
        flowSettingTemperary = setFlow;
    }
    
    if ((flowSettingTemperary != setFlow)) // 3
    {
        s_CheckForBlockageFlowCheckCounter = 0;
        s_CheckForBlockageBlowerSpeedMoreThanUpperLimitCounter = 0;
        s_CheckForBlockageBlowerSpeedLessThanUpperLimitCounter = 0;
        settingChangedFlag = true;
        flowSettingTemperary = setFlow;
    }
    
    if (settingChangedFlag == true) // 4
    {
        s_checkForBlockageChangeSettingTimeoutCounter++;
    }
    
    if(s_CheckBlockageTimeStartCounter <= FIVE_MINUTES_AFTER_START) // 2
    {
        s_CheckBlockageTimeStartCounter++;
    }
    
    if((BreathingCircuitType != eTypePediatric) && (BreathingCircuitType != eTypeAdult) && (BreathingCircuitType != eTypeDisinfection))
    {
        noBreathingCircuit = true;
    }
    
    if ((s_checkForBlockageChangeSettingTimeoutCounter <= FLOW_CHANGE_SETTING_TIMEOUT && (settingChangedFlag == true))|| (s_CheckBlockageTimeStartCounter <= FIVE_MINUTES_AFTER_START)
        || (isChamberConnected == false) || (noBreathingCircuit == true))
        return;
    

//    s_CheckBlockageTimeStartCounter = FIVE_MINUTES_AFTER_START;
    s_checkForBlockageChangeSettingTimeoutCounter = 0;
    settingChangedFlag = false;
    // Alarm check
    
    // Check Alarm with gasflow
    if (s_alarmList[eCheckBlockageAlarmId].currentStatus == eInactive)
    {
        if ((currentFlow <= 0.8 * setFlow)&&(blowerControlValue > 0))
        {
            if(s_CheckForBlockageFlowCheckCounter >= CHECKFORBLOCKAGE_ALARM_TIMEOUT)
            {
                s_alarmList[eCheckBlockageAlarmId].currentStatus = eActive;
                return;
            } 
            s_CheckForBlockageFlowCheckCounter++;
        }
        else
        {
            s_CheckForBlockageFlowCheckCounter = 0;
        }
    }
    // Check blower speed
    if (machineMode == eAdultMode) // 
    {
        switch ((int)setFlow)
        {
            case 15:
                CheckForBlockageCheckUpperLimitAlarmStatus( setFlow, 
                                                            currentFlow ,
                                                            23550, 
                                                            currentBlowerRotationSpeed);
                break;

            case 20:
                CheckForBlockageCheckUpperLimitAlarmStatus( setFlow, 
                                                            currentFlow ,
                                                            25800, 
                                                            currentBlowerRotationSpeed);
                break;

            case 25:
                 CheckForBlockageCheckUpperLimitAlarmStatus(setFlow, 
                                                            currentFlow ,
                                                            27975, 
                                                            currentBlowerRotationSpeed);
                break;

            case 30:
                 CheckForBlockageCheckUpperLimitAlarmStatus(setFlow, 
                                                            currentFlow ,
                                                            31200, 
                                                            currentBlowerRotationSpeed);
                break;

            case 35:
                 CheckForBlockageCheckUpperLimitAlarmStatus(setFlow, 
                                                            currentFlow ,
                                                            32550, 
                                                            currentBlowerRotationSpeed);
                break;

            case 40:
                if (isACConnected == true)
                {
                    CheckForBlockageCheckUpperLimitAlarmStatus(setFlow, 
                                                               currentFlow ,
                                                               35250, 
                                                               currentBlowerRotationSpeed);
                }
                else
                {
                    CheckForBlockageCheckUpperLimitAlarmStatus(setFlow, 
                                                               currentFlow ,
                                                               32800, 
                                                               currentBlowerRotationSpeed);
                }
                break;
            case 45:
                if (isACConnected == true)
                {
                    CheckForBlockageCheckUpperLimitAlarmStatus(setFlow, 
                                                               currentFlow ,
                                                               35600, 
                                                               currentBlowerRotationSpeed);
                }
                break;
            case 50:
                if (isACConnected == true)
                {
                    CheckForBlockageCheckUpperLimitAlarmStatus(setFlow, 
                                                               currentFlow ,
                                                               36000, 
                                                               currentBlowerRotationSpeed);
                }
                break;
            case 55:
                if (isACConnected == true)
                {
                    CheckForBlockageCheckUpperLimitAlarmStatus(setFlow, 
                                                               currentFlow ,
                                                               36500, 
                                                               currentBlowerRotationSpeed);
                }
                break;
            case 60:
                if (isACConnected == true)
                {
                    CheckForBlockageCheckUpperLimitAlarmStatus(setFlow, 
                                                               currentFlow ,
                                                               36750, 
                                                               currentBlowerRotationSpeed);
                }
                break;
            default:
                break;                   
        }
    }
    else if (machineMode == eChildMode)
    {
        if ((setFlow >= 5)&&(setFlow <= 10))
        {
            CheckForBlockageCheckUpperLimitAlarmStatus( setFlow, 
                                                        currentFlow ,
                                                        20000, 
                                                        currentBlowerRotationSpeed);
        }
        else if ((setFlow >= 11) && (setFlow <= 15))
        {
            CheckForBlockageCheckUpperLimitAlarmStatus( setFlow, 
                                                        currentFlow ,
                                                        23550, 
                                                        currentBlowerRotationSpeed);
        }
        else if ((setFlow >= 16) && (setFlow <= 20))
        {
            CheckForBlockageCheckUpperLimitAlarmStatus( setFlow, 
                                                        currentFlow ,
                                                        25800, 
                                                        currentBlowerRotationSpeed);
        }
        else if ((setFlow >= 21) && (setFlow <= 25))
        {
            CheckForBlockageCheckUpperLimitAlarmStatus( setFlow, 
                                                        currentFlow ,
                                                        27975, 
                                                        currentBlowerRotationSpeed);
        }
    }
    return;
}

/** @brief Check status of non-genuine circuit
 * Trigger this alarm if the read code of the circuit does not match the Metran code
 * Cleared when a genuine breathing circuit is connected or power down and restart by power button.
 *  @param [in] float breathingCode Code of the current circuit
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckNonGenuineCircuitAlarmStatus(uint8_t breathingCode, bool isChamberConnected)//E004
{
    uint32_t alarmTaskPeriod = 50;//50ms
    static uint32_t s_counter = 0;
    if ((isChamberConnected == false) || (s_currentAlarmMonitor.isBreathingCircuitConnected == false))
        return;
    
    if(s_alarmList[eNonGenuineCircuitAlarmId].currentStatus == eInactive)
    {
        if(breathingCode != METRAN_CODE)
        {  
            s_counter++;
            if(s_counter >= 500/alarmTaskPeriod){
                s_alarmList[eNonGenuineCircuitAlarmId].currentStatus = eActive;
                if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                {
                    s_alarmList[eNonGenuineCircuitAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                }
                else
                {
                    s_alarmList[eNonGenuineCircuitAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                }
            }
        }
        else{
            s_counter = 0;
        }
    }
    else if(s_alarmList[eNonGenuineCircuitAlarmId].currentStatus == eActive)
    {
        if(breathingCode == METRAN_CODE)
        {  
            s_alarmList[eNonGenuineCircuitAlarmId].currentStatus = eInactive; 
        }
    }
    
    return;
}

/** @brief Check incorrect breathing circuit change
 * Trigger alarm when:
  - When startup: Adult circuit ? Pediatric circuit
  - During running: when the breathing circuit type is changed during running.
 * Clear this alarm when power down and restart by power button. 
 * Or when removing the breathing circuit
 * Or Adult circuit ? Pediatric circuit, when the OK button is pressed
 * Or Adult / Pediatric circuit ? Disinfection circuit When the adult / Pediatric 
 * circuit is reconnected or when the OK button is pressed 
 *  @param [in] E_BreathingCircuitType log_BreathingCircuitType Breathing Circuit Type is saved in log
 *  @param [in] E_BreathingCircuitType current_BreathingCircuitType The current Breathing Circuit Type
 *  @param [in] bool isOKPress Flag is raised if user presses button OK
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckBreathingCircuitChangeAlarmStatus(E_BreathingCircuitType BreathingCircuitType,
                                                            bool isChamberConnected,
                                                            E_TreatmentMode machineMode,
                                                            bool self_check
                                                            )//E005
{
    // Check invalid
    if (isChamberConnected == false)
    {
        return;
    }
    // Remove circuitType clear alarm
    if((BreathingCircuitType != eTypePediatric) && (BreathingCircuitType != eTypeAdult) && (BreathingCircuitType != eTypeDisinfection))
    {
        s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eInactive;
        return;
    }
    // Check for setting alarm
    if (s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus == eInactive)
    {
        // If setting mode different circuit type -> active alarm
        if (self_check == true)
        {
            if ((machineMode == eAdultMode) && (BreathingCircuitType == eTypePediatric))
            {
                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eActive;
                s_alarmList[eBreathingCircuitChangedAlarmId].data[eMessageTypeDataIdx] = eE005PediatricCircuitToAdultMode;
                return;
            }

            if ((machineMode == eChildMode) && (BreathingCircuitType == eTypeAdult))
            {
                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eActive;
                s_alarmList[eBreathingCircuitChangedAlarmId].data[eMessageTypeDataIdx] = eE005AdultCircuitToPediatricMode;
                return;
            }
        }
        else
        {
            if ((machineMode == eAdultMode)&&(BreathingCircuitType == eTypePediatric))
            {
                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eActive;
                s_alarmList[eBreathingCircuitChangedAlarmId].data[eMessageTypeDataIdx] = eE005PediatricCircuitToAdultMode;
                return;
            }

            if ((machineMode == eChildMode)&&(BreathingCircuitType == eTypeAdult))
            {
                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eActive;
                s_alarmList[eBreathingCircuitChangedAlarmId].data[eMessageTypeDataIdx] = eE005AdultCircuitToPediatricMode;
                return;
            }

            if ((machineMode == eAdultMode)&&(BreathingCircuitType == eTypeDisinfection))
            {
                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eActive;
                s_alarmList[eBreathingCircuitChangedAlarmId].data[eMessageTypeDataIdx] = eE005DisinfectionCircuitToAdultMode;
                return;
            }

            if ((machineMode == eChildMode)&&(BreathingCircuitType == eTypeDisinfection))
            {
                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eActive;
                s_alarmList[eBreathingCircuitChangedAlarmId].data[eMessageTypeDataIdx] = eE005DisinfectionCircuitToPediatricMode;
                return;
            }

            if ((machineMode == eDisinfectionMode)&&(BreathingCircuitType == eTypeAdult))
            {
                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eActive;
                s_alarmList[eBreathingCircuitChangedAlarmId].data[eMessageTypeDataIdx] = eE005AdultCircuitToDisinfectionMode;
                return;
            }

            if ((machineMode == eDisinfectionMode)&&(BreathingCircuitType == eTypePediatric))
            {
                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eActive;
                s_alarmList[eBreathingCircuitChangedAlarmId].data[eMessageTypeDataIdx] = eE005PediatricCircuitToDisinfectionMode;
                return;
            }

        }
    }
    // Check for clearing alarm
    else if (s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus == eActive)
    {
        if ((s_alarmList[eBreathingCircuitChangedAlarmId].data[eMessageTypeDataIdx] == eE005DisinfectionCircuitToAdultMode)
            || (s_alarmList[eBreathingCircuitChangedAlarmId].data[eMessageTypeDataIdx] == eE005DisinfectionCircuitToPediatricMode))
        {
            if ((machineMode == eAdultMode) && (BreathingCircuitType == eTypeAdult))
            {
                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eInactive;
                return;
            }
            if ((machineMode == eChildMode) && (BreathingCircuitType == eTypePediatric))
            {
                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eInactive;
                return;
            }
//            if ((machineMode == eDisinfectionMode) && (BreathingCircuitType == eTypeDisinfection))
//            {
//                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eInactive;
//                return;
//            }

            //breath circuit disconnected
//            if (BreathingCircuitType == eTypeOther)
//            {
//                s_alarmList[eBreathingCircuitChangedAlarmId].currentStatus = eInactive;
//                return;
//            }
        }
    }
}

/** @brief Check status of expired circuit (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckExpiredCircuitAlarmStatus(uint32_t dateManufactureBreathingCircuit,
                                                    uint32_t dateCurrentBreathingCircuit,
                                                    E_BreathingCircuitType BreathingCircuitType)//E006
{
    //SYS_PRINT("\n\nE006: [%d]   -   dateManu: [%d]    -   dateCurrent: [%d]", s_alarmList[eExpiredCircuitAlarmId].currentStatus, dateManufactureBreathingCircuit, dateCurrentBreathingCircuit);
    // Check clear Alarm
    if(dateManufactureBreathingCircuit == 0)
        return;
    if((BreathingCircuitType != eTypePediatric) && (BreathingCircuitType != eTypeAdult) && (BreathingCircuitType != eTypeDisinfection))
    {
            s_alarmList[eExpiredCircuitAlarmId].currentStatus = eInactive;
            return;
    }
    // Check active alarm    
    if(s_alarmList[eExpiredCircuitAlarmId].currentStatus == eInactive)
    {
        if (dateCurrentBreathingCircuit > (dateManufactureBreathingCircuit + 2*10000))// over 2 year
        {
            s_alarmList[eExpiredCircuitAlarmId].currentStatus = eActive;   
//            SYS_PRINT("\n\nE006: [%d]   -   dateManu: [%d]    -   dateCurrent: [%d]", s_alarmList[eExpiredCircuitAlarmId].currentStatus, dateManufactureBreathingCircuit, dateCurrentBreathingCircuit);
        }
    }
    return;
}

/** @brief Check status of connection of chamber
 * Trigger this alarm if the chamber is not recovered even after retrying and 
 * retrying up to 3 times, it is judged that the chamber is not connected
 * Clear this alarm when power down and restart by power button
 *  @param [in] noConnectionChamber Flag is raised when Main board couldn't communicate with Chamber 3 times in a row. 
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckConnectionOfChamberAlarmStatus(bool isChamberConnected)//E007
{
    static uint8_t s_counter = 0;
    
    if (isChamberConnected == false)
    {
        if (s_counter >= 1000/50)
        {
            s_counter = 0;
            s_alarmList[eCheckConnectionChamberAlarmId].currentStatus = eActive;
            if(s_currentAlarmMonitor.currentO2Flow > 1.0)
            {
                s_alarmList[eCheckConnectionChamberAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
            }
            else{
                s_alarmList[eCheckConnectionChamberAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
            }
        }
        else
        {
            s_counter++;
        }
        return;
    }
}

/** @brief Check status of having more water in the chamber or not
 * Trigger this alarm when the measured power of IH is 30% lower than the target
 * power more than 1 minutes
 * Clear this alarm when the average IH power for one minute returns to the target power ? 5%
 * Or power down and restart by power button
 *  @param [in] float currentIHPower Current IH power
 *  @param [in] float setTargetPower Set target power 
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckNoMoreWaterChamberAlarmStatus(float currentIHPower,
                                                        float setTargetPower,
                                                        E_TreatmentMode machineMode,
                                                        bool isIHOperating,
                                                        float setFlow,
                                                        float setTemperature,
                                                        E_WarmingUpStatus warmingUpStatus
                                                        )//E008
{
    // Check invalid time
    // Set when flow and temp change -- off after setting for 5 mins
    static bool settingChangedFlag = false;
    static int flowSettingTemp = -1;
    static float temperatureSettingTemp = -1;
    static uint8_t s_numSamp = 0;
    static uint8_t s_idx = 0;
    static float s_avgIHPower;
    static uint8_t s_cntOneSecondCycle = 0;
    
    if (flowSettingTemp == -1 || temperatureSettingTemp == -1)
    {
        flowSettingTemp = setFlow;
        temperatureSettingTemp = setTemperature;
    }
    
    if ((flowSettingTemp != setFlow) || (temperatureSettingTemp != setTemperature))
    {
        
        settingChangedFlag = true;
        flowSettingTemp = setFlow;
        temperatureSettingTemp = setTemperature;
    }
    
    if (settingChangedFlag == true)
    {
        if(s_checkNoMoreWaterChangeSettingTimeoutCounter <= TEMP_CHANGE_SETTING_TIMEOUT)
        {
            s_checkNoMoreWaterChangeSettingTimeoutCounter++;
            
            // Reset sampling
            s_numSamp = 0;
            s_idx = 0;
            return;
        }
    }
    s_checkNoMoreWaterChangeSettingTimeoutCounter = 0;
    settingChangedFlag = false;
    
    if ((isIHOperating == false) || (machineMode == eDryMode) || (warmingUpStatus != eWarmingUpFinished))
    {
        // Reset sampling
        s_numSamp = 0;
        s_idx = 0;
        return;
    }
    
    s_cntOneSecondCycle++;
//    SYS_PRINT("\n-----------------------[%d]", s_cntOneSecondCycle);
    if (s_cntOneSecondCycle == TICK_PER_SECOND)
    {
        s_cntOneSecondCycle = 0;
//        static int j = 0;
//        SYS_PRINT("\nEnter alarm: [%d]", j++);
        s_IHPower[s_idx] = currentIHPower;
        s_idx = (++s_idx) % IH_POWER_NUM_SAMPLE_IN_1MINS;
        
        if (s_numSamp < IH_POWER_NUM_SAMPLE_IN_1MINS)
        {
            s_numSamp++;
        }
        if (s_numSamp >= IH_POWER_NUM_SAMPLE_IN_1MINS)
        {
            int i = 0;
            s_avgIHPower = 0;
            SYS_PRINT("\nSample: ");
            for (i = 0; i < IH_POWER_NUM_SAMPLE_IN_1MINS; i++)
            {   SYS_PRINT(" [%f] ",s_IHPower[i]);
                s_avgIHPower += s_IHPower[i]/IH_POWER_NUM_SAMPLE_IN_1MINS;
            }
            
            SYS_PRINT("\nAvg_IHPower: [%f]", s_avgIHPower);
            if(s_alarmList[eNoMoreWaterInChamberAlarmId].currentStatus == eInactive)
            {
                if (s_avgIHPower <= (0.5 * setTargetPower))
                {
                    s_alarmList[eNoMoreWaterInChamberAlarmId].currentStatus = eActive;
                }
            }
            else if (s_alarmList[eNoMoreWaterInChamberAlarmId].currentStatus == eActive)
            {
                if (s_avgIHPower <= (1.05 * setTargetPower) && (s_avgIHPower >= 0.95 * setTargetPower))
                {
                    s_alarmList[eNoMoreWaterInChamberAlarmId].currentStatus = eInactive;
                }
            }
        }
    }
    return;
}

/** @brief Check status of running out of water in chamber.
 * Trigger this alarm when the average water sensor output for 30seconds is lower than TBD Hz
 * Clear This alarm when the average water sensor output for 30 seconds is higher than TBD Hz
 * Or Power down and restart by power button.
 *  @param [in] currentWaterSensor data of WaterSensor
 *  temp or flow is under 5 minutes
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckRunOutOfWaterAlarmStatus(float currentWaterSensor)//E009, is Disable
{
//#define NUM_SAMPLE_IN_30SEC  (30*1000/500)
//#define ALARM_TASK_DELAY    (50)
//#define TICK_PER_500MS (500/ALARM_TASK_DELAY)
    
//    static float s_WaterSensor[NUM_SAMPLE_IN_30SEC] = {};
//    static uint8_t s_cntOneCycle = 0;
//    static uint16_t s_idx = 0;
//    static uint16_t s_numSamp = 0;
//    float averageWaterSensor = 0;
    
//    s_cntOneCycle++;
    
//    if(s_cntOneCycle == TICK_PER_500MS)
//    {
//        s_cntOneCycle = 0;
//        s_WaterSensor[s_idx] = currentWaterSensor;
//        s_idx = (s_idx++) % NUM_SAMPLE_IN_30SEC;
//        if(s_numSamp < NUM_SAMPLE_IN_30SEC)
//        {
//            s_numSamp++;
//        }
//        else
//        {
//            uint16_t i = 0;
//            for(i = 0; i < NUM_SAMPLE_IN_30SEC; i++)
//            {
//                averageWaterSensor = averageWaterSensor + s_WaterSensor[i]/NUM_SAMPLE_IN_30SEC;
//            }
            
//            if(s_alarmList[eRunOutOfWaterAlarmId].currentStatus == eInactive)
//            {
//                if (averageWaterSensor < AVERAGE_WATER_SENSOR_HZ) //TBD HZ
//                {
//                    s_alarmList[eRunOutOfWaterAlarmId].currentStatus = eActive;
//                }
//            }
//            else
//            {
//                if (averageWaterSensor >= AVERAGE_WATER_SENSOR_HZ) //TBD HZ
//                {
//                    s_alarmList[eRunOutOfWaterAlarmId].currentStatus = eInactive;
//                }
//            }
//        }
//    }
     
    return;
}

/** @brief Check status of average mouth temperature
 * Low Temperature: Trigger this alarm when the average of calculated mouth 
 * temperature for 5 minute  ? set temperature  - 2 ?
 * High Temperature: Trigger this alarm when the average of calculated mouth?
 * temperature for 5 minute ?set temperature  +2 ?
 * Clear this alarm when the mouth temperature returns to the setting range
 * Or Power down and restart by power button.
 *  @param [in] currentMouthTemperature Mouth Temperature
 *  @param [in] float setTemperature Setting temperature
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckLowTemperatureAlarmStatus(float currentMouthTemperature,
                                                    uint8_t setTemperature,
                                                    uint8_t setFlow)//E010 -- 18/02/2022
{   
    
    float averageMouthTemperature = 0;
    float sumTemperature = 0;
    static uint8_t s_cntOneSecondCycle = 0;
    static uint16_t s_idx = 0;
    static uint16_t s_numSamp = 0;
    // Set when flow and temp change -- off after setting for 5 mins
    static bool settingChangedFlag = false;
    // Check invalid time
    static int flowSettingTemp = -1;
    static float temperatureSettingTemp = -1;
    if (flowSettingTemp == -1 || temperatureSettingTemp == -1)
    {
        flowSettingTemp = setFlow;
        temperatureSettingTemp = setTemperature;
        return;
    }
    
    if ((flowSettingTemp != setFlow) || (temperatureSettingTemp != setTemperature))
    {
        
        settingChangedFlag = true;
        flowSettingTemp = setFlow;
        temperatureSettingTemp = setTemperature;
    }
    
    if (settingChangedFlag == true)
    {
        if(s_ChangeSettingTimeoutCounter <= TEMP_CHANGE_SETTING_TIMEOUT)
        {
            s_ChangeSettingTimeoutCounter++; 
            return;
        }
        s_numSamp = 0;
        s_idx = 0;
    }
    s_ChangeSettingTimeoutCounter = 0;
    settingChangedFlag = false;

    if(s_currentAlarmMonitor.warmingUpStatus != eWarmingUpFinished){
        return;
    }
    
    // Check Alarm
    s_cntOneSecondCycle++;
    if(s_cntOneSecondCycle == TICK_PER_SECOND)
    {
        s_cntOneSecondCycle = 0;
        s_E010Temperature[s_idx] = currentMouthTemperature;
        //SYS_PRINT("\nE010 - [Index: %d] -- [Mouth: %f] - [currentMouth: %f]", s_idx, s_E010Temperature[s_idx], currentMouthTemperature);
        s_idx = (++s_idx) % NUM_SAMPLE_IN_5MINS;
        
        if(s_numSamp < NUM_SAMPLE_IN_5MINS)
        {
            s_numSamp++;
        }
        if (s_numSamp >= NUM_SAMPLE_IN_5MINS)
        {
            uint16_t i = 0;
            for(i = 0; i < NUM_SAMPLE_IN_5MINS; i++)
            {
                sumTemperature = sumTemperature + s_E010Temperature[i];
            }
            averageMouthTemperature = sumTemperature / NUM_SAMPLE_IN_5MINS;
    
    //just for test
//    averageMouthTemperature = setTemperature - 2;
//    s_currentAlarmMonitor.currenAmbientTemperature = 19;
    //end test
            if(s_alarmList[eLowTemperatureAlarmId].currentStatus == eInactive)
            {
                if (averageMouthTemperature <= (setTemperature - 2))
                {
                    if(s_currentAlarmMonitor.currenAmbientTemperature >= 18.0)
                    {
                        s_alarmList[eLowTemperatureAlarmId].data[eResetButtonStatusDataIdx] = eResetButtonHide;
                        s_alarmList[eLowTemperatureAlarmId].data[eMessageTypeDataIdx] = eE010AmbientTempEqualOrOverThan18CelsiusDegree;
                        if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                        {
                            s_alarmList[eLowTemperatureAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                        }
                        else
                        {
                            s_alarmList[eLowTemperatureAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                        }
                    }
                    else
                    {
                        s_alarmList[eLowTemperatureAlarmId].data[eResetButtonStatusDataIdx] = eResetButtonShowReset;
                        s_alarmList[eLowTemperatureAlarmId].data[eMessageTypeDataIdx] = eE010AmbientTempLowerThan18CelsiusDegree;
                    }
                    s_alarmList[eLowTemperatureAlarmId].currentStatus = eActive;
                    s_numSamp = 0;
                    return;
                }
            }
            else
            {
                if (averageMouthTemperature >= (setTemperature - 0.5))
                {
                    s_alarmList[eLowTemperatureAlarmId].currentStatus = eInactive;
                    s_numSamp = 0;
                    return;
                }
            }
        }
    }
    return;
}

/** @brief Check status of average mouth temperature
 * Low Temperature: Trigger this alarm when the average of calculated mouth 
 * temperature for 5 minute  ? set temperature  - 2 ?
 * High Temperature: Trigger this alarm when the average of calculated mouth?
 * temperature for 5 minute ?set temperature  +2 ?
 * Clear this alarm when the mouth temperature returns to the setting range
 * Or Power down and restart by power button.
 *  @param [in] currentMouthTemperature Mouth Temperature
 *  @param [in] float setTemperature Setting temperature
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckHighTemperatureAlarmStatus(   float currentMouthTemperature,
                                                        float setTemperature,
                                                        float ambientTemperature,
                                                        int setFlow,
                                                        E_TreatmentMode machineMode)//E011 -- 18/02/2022
{  
    float averageMouthTemperature = 0;
    float sumTemperature = 0;
//    static float s_temperature[NUM_SAMPLE_IN_5MINS] = {};
    static uint8_t s_cntOneSecondCycle = 0;
    static uint16_t s_idx = 0;
    static uint16_t s_numSamp = 0;
    // Set when flow and temp change -- off after setting for 5 mins
    static bool settingChangedFlag = false;
    // Check invalid time
    static int flowSettingTemp = -1;
    static float temperatureSettingTemp = -1;
    
    if (flowSettingTemp == -1 || temperatureSettingTemp == -1)
    {
        flowSettingTemp = setFlow;
        temperatureSettingTemp = setTemperature;
        return;
    }
    
    if ((flowSettingTemp != setFlow) || (temperatureSettingTemp != setTemperature))
    {
        
        settingChangedFlag = true;
        flowSettingTemp = setFlow;
        temperatureSettingTemp = setTemperature;
    }
    
    if (settingChangedFlag == true)
    {
        if(s_ChangeSettingTimeoutCounter <= TEMP_CHANGE_SETTING_TIMEOUT)
        {
            s_ChangeSettingTimeoutCounter++;
//            s_alarmList[eHighTemperatureAlarmId].currentStatus = eInactive;
            return;
        }
        s_numSamp = 0;
        s_idx = 0;
    }
    s_ChangeSettingTimeoutCounter = 0;
    settingChangedFlag = false;

    if (machineMode == eDryMode || machineMode == eDisinfectionMode || s_currentAlarmMonitor.warmingUpStatus != eWarmingUpFinished)
    {
        s_numSamp = 0;
        s_idx = 0;
        return;
    }
    if (s_alarmList[eCheckOperatingConditionsAlarmID].currentStatus == eActive)
    {
        s_numSamp = 0;
        s_idx = 0;
        return;
    }
//    if(s_currentAlarmMonitor.warmingUpStatus != eWarmingUpFinished){
//        return;
//    }
    
    // Check Alarm
    s_cntOneSecondCycle++;
    if(s_cntOneSecondCycle == TICK_PER_SECOND)
    {
        s_cntOneSecondCycle = 0;
        s_E011Temperature[s_idx] = currentMouthTemperature;
        // SYS_PRINT("\nE011 - [Index: %d] -- [Mouth: %f] - [currentMouth: %f]", s_idx, s_E011Temperature[s_idx], currentMouthTemperature);
        s_idx = (++s_idx) % NUM_SAMPLE_IN_5MINS;
        if(s_numSamp < NUM_SAMPLE_IN_5MINS)
        {
            s_numSamp++;
        }
        if (s_numSamp >= NUM_SAMPLE_IN_5MINS)
        {
            uint16_t i = 0;
            for(i = 0; i < NUM_SAMPLE_IN_5MINS; i++)
            {
                sumTemperature = sumTemperature + s_E011Temperature[i];
            }
            averageMouthTemperature = sumTemperature / NUM_SAMPLE_IN_5MINS;
            if(s_alarmList[eHighTemperatureAlarmId].currentStatus == eInactive)
            {
                if ((averageMouthTemperature >= (setTemperature + 2)) && (ambientTemperature <= setTemperature))
                {
                    s_alarmList[eHighTemperatureAlarmId].currentStatus = eActive;
                    s_alarmList[eHighTemperatureAlarmId].data[eMessageTypeDataIdx] = eE011AmbientTempLowerThanSettingTemp;
                    s_alarmList[eHighTemperatureAlarmId].data[eResetButtonStatusDataIdx] = eResetButtonHide;
                    
                    if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                    {
                        s_alarmList[eHighTemperatureAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                    }
                    else
                    {
                        s_alarmList[eHighTemperatureAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                    }
                    
                    s_numSamp = 0;
                    return; 
                }
            }    
            else
            {
                if (averageMouthTemperature <= (setTemperature + 0.5))
                {
                    s_alarmList[eHighTemperatureAlarmId].currentStatus = eInactive;
                    s_numSamp = 0;
                    return;
                }
            }
        }
    }
    return;
}
/** @brief Check status of High temperature abnormality
 *  Trigger this alarm when the calculated mouth temperature at 1 sampling ?42?
 *  Clear this alarm when the mouth temperature returns to the setting range
 *  Or Power down and restart by power button.
 *  @param [in] currentMouthTemperature Mouth Temperature
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckHighTemperatureAbnormalityAlarmStatus(float currentMouthTemperature,
                                                                float ambientTemperature,
                                                                int setFlow,
                                                                float setTemperature,
                                                                E_TreatmentMode machineMode)//E012
{      
    float averageMouthTemperature = 0;
    float sumTemperature = 0;
//    static float s_temperature[NUM_SAMPLE_IN_2MINS] = {};
    static uint8_t s_cntOneSecondCycle = 0;
    static uint16_t s_idx = 0;
    static uint16_t s_numSamp = 0;
    // Check Invalid time
    if (machineMode == eDryMode || machineMode == eDisinfectionMode)
    {
        return;
    }
    
    s_cntOneSecondCycle++;
    if(s_cntOneSecondCycle == TICK_PER_SECOND)
    {
        s_cntOneSecondCycle = 0;
        s_E012Temperature[s_idx] = currentMouthTemperature;
//        SYS_PRINT("\nE012 - [Index: %d] -- [Mouth: %f] - [currentMouth: %f]", s_idx, s_E012Temperature[s_idx], currentMouthTemperature);
        s_idx = (++s_idx) % NUM_SAMPLE_IN_2MINS;
        if(s_numSamp < NUM_SAMPLE_IN_2MINS)
        {
            s_numSamp++;
        }
        if (s_numSamp >= NUM_SAMPLE_IN_2MINS)
        {
            uint16_t i = 0;
            for(i = 0; i < NUM_SAMPLE_IN_2MINS; i++)
            {
                sumTemperature = sumTemperature + s_E012Temperature[i];
            }
            averageMouthTemperature = sumTemperature / NUM_SAMPLE_IN_2MINS;
            
            if ((ambientTemperature <= setTemperature) && (averageMouthTemperature >= 42))
            {
                s_numSamp = 0;
                s_alarmList[eHighTemperatureAbnormalityAlarmId].currentStatus = eActive;
                if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                {
                    s_alarmList[eHighTemperatureAbnormalityAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                }
                else
                {
                    s_alarmList[eHighTemperatureAbnormalityAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                }
            }
        }
    }
    return;
}

/** @brief Check status of target temperature cannot be achieved
 *  Trigger this alarm when the mouth temperature did not reach to the target
 *  temperature after 30 minute running from started operation and the temperature
 *  of environment is lower or higher than specification environment
 *  Clear this alarm when the mouth temperature returns to the setting range
 *  Or Power down and restart by power button.
 *  @param [in] currentMouthTemperature Mouth temperature
 *  @param [in] setMouthTemperature Target mouth temperature
 *  @param [in] currenAmbientTemperature  Environment temperature
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckRoomTempTooLowToAchieveTargetTempAlarmStatus(float currentMouthTemperature, 
                                                                       float setMouthTemperature, 
                                                                       float currenAmbientTemperature,
                                                                       E_TreatmentMode machineMode)//E013
{
    if (machineMode == eDryMode || machineMode == eDisinfectionMode || s_currentAlarmMonitor.warmingUpStatus != eWarmingUp)
    {
        return;
    }
    
    if((currentMouthTemperature < setMouthTemperature) || (currentMouthTemperature > setMouthTemperature))
    {
        if(s_TargetTemperatureCannotBeAchievedTemperatureLowerCounter < TARGETTEMPERATURECANNOTBEACHIEVEDTEMPERATURE_ALARM_TIMEOUT)
        {
            s_TargetTemperatureCannotBeAchievedTemperatureLowerCounter++;
        }
    }
    else
    {
        s_TargetTemperatureCannotBeAchievedTemperatureLowerCounter = 0;
        s_alarmList[eRoomTemperatureLowToAchieveTargetTemperatureAlarmId].currentStatus = eInactive;
        return;
    }

    if((s_TargetTemperatureCannotBeAchievedTemperatureLowerCounter >= TARGETTEMPERATURECANNOTBEACHIEVEDTEMPERATURE_ALARM_TIMEOUT))
    {
        if(currenAmbientTemperature < UNDER_AMBIENT_TEMPERATURE_RANGE)
        {
            s_alarmList[eRoomTemperatureLowToAchieveTargetTemperatureAlarmId].currentStatus = eActive;
            return;
        }
    }
}

/** @brief Check status of target temperature cannot be achieved
 *  Trigger this alarm when the mouth temperature did not reach to the target
 *  temperature after 30 minute running from started operation and the temperature
 *  of environment is lower or higher than specification environment
 *  Clear this alarm when the mouth temperature returns to the setting range
 *  Or Power down and restart by power button.
 *  @param [in] currentMouthTemperature Mouth temperature
 *  @param [in] setMouthTemperature Target mouth temperature
 *  @param [in] currenAmbientTemperature  Environment temperature
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckOperationgConditionsAlarmStatus( float setTemperature, 
                                                           float currenAmbientTemperature,
                                                           E_TreatmentMode machineMode)//E014
{
    if (machineMode == eDryMode || machineMode == eDisinfectionMode || s_currentAlarmMonitor.warmingUpStatus != eWarmingUp)
    {
        return;
    }
    float averageAmbientTemperature = 0;
    float sumTemperature = 0;
    static uint8_t s_cntOneSecondCycle = 0;
    static uint16_t s_idx = 0;
    static uint16_t s_numSamp = 0;
    // Check Invalid time
    s_cntOneSecondCycle++;
    if(s_cntOneSecondCycle == TICK_PER_SECOND)
    {
        s_cntOneSecondCycle = 0;
        s_ambientTemperature[s_idx] = currenAmbientTemperature;
        s_idx = (++s_idx) % NUM_SAMPLE_IN_2MINS;
        if(s_numSamp < NUM_SAMPLE_IN_2MINS)
        {
            s_numSamp++;
        }
        if(s_numSamp >= NUM_SAMPLE_IN_2MINS)
        {
            uint16_t i = 0;
            for(i = 0; i < NUM_SAMPLE_IN_2MINS; i++)
            {
                sumTemperature = sumTemperature + s_ambientTemperature[i];
            }
            averageAmbientTemperature = sumTemperature / NUM_SAMPLE_IN_2MINS;
            
            if (s_alarmList[eCheckOperatingConditionsAlarmID].currentStatus == eInactive)
            {
                if (averageAmbientTemperature > setTemperature)
                {
                    s_numSamp = 0;
                    s_alarmList[eCheckOperatingConditionsAlarmID].currentStatus = eActive;
                    if (averageAmbientTemperature >= 42.0)
                    {
                        s_alarmList[eCheckOperatingConditionsAlarmID].data[eMessageTypeDataIdx] = eE011AmbientTempEqualOrOverThan42CelsiusDegree;
                        s_alarmList[eCheckOperatingConditionsAlarmID].data[eResetButtonStatusDataIdx] = eResetButtonHide;
                                        
                        if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                        {
                            s_alarmList[eCheckOperatingConditionsAlarmID].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                        }
                        else
                        {
                            s_alarmList[eCheckOperatingConditionsAlarmID].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                        }
                    }
                    else
                    {
                        s_alarmList[eCheckOperatingConditionsAlarmID].data[eMessageTypeDataIdx] = eE011AmbientTempLowerThan42CelsiusDegree;
                        s_alarmList[eCheckOperatingConditionsAlarmID].data[eResetButtonStatusDataIdx] = eResetButtonShowReset;
                    }
                    return;
                }
            }
            else
            {
                if (averageAmbientTemperature < setTemperature)
                {
                    s_numSamp = 0;
                    s_alarmList[eCheckOperatingConditionsAlarmID].currentStatus = eInactive;
                    return;
                }
            }
        }
    }
    return;
}

static void alarmMgr_CheckTargetTemperatureCannotBeAchievedAlarmStatus(float currentMouthTemperature, 
                                                                       float setMouthTemperature, 
                                                                       float currenAmbientTemperature)//E015
{
    if((currentMouthTemperature < setMouthTemperature)/*||(currentMouthTemperature > setMouthTemperature)*/)
    {
        if(s_TargetTemperatureCannotBeAchievedTemperatureCounter < TARGETTEMPERATURECANNOTBEACHIEVEDTEMPERATURE_ALARM_TIMEOUT)
        {
            s_TargetTemperatureCannotBeAchievedTemperatureCounter++;
        }
    }
    else
    {
        s_TargetTemperatureCannotBeAchievedTemperatureCounter = 0;
        s_alarmList[eDeviceErrorToAchieveTargetTemperatureAlarmId].currentStatus = eInactive;
    }

    if((s_TargetTemperatureCannotBeAchievedTemperatureCounter >= TARGETTEMPERATURECANNOTBEACHIEVEDTEMPERATURE_ALARM_TIMEOUT))
    {
        if((currenAmbientTemperature >= UNDER_AMBIENT_TEMPERATURE_RANGE) && (currenAmbientTemperature <= UPPER_AMBIENT_TEMPERATURE_RANGE))
        {
            s_alarmList[eDeviceErrorToAchieveTargetTemperatureAlarmId].currentStatus = eActive;
                                    
            if(s_currentAlarmMonitor.currentO2Flow > 1.0)
            {
                s_alarmList[eDeviceErrorToAchieveTargetTemperatureAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
            }
            else
            {
                s_alarmList[eDeviceErrorToAchieveTargetTemperatureAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
            }
        }
    }
    
    return;
}

/** @brief Check status of Oxygen concentration is lower than min limit
 *  Trigger this alarm when calculated oxygen concentration lower than the min limit alarm for more than 5 seconds
 *  Clear this alarm when the oxygen concentration returns to the alarm setting range
 *  Or Power down and restart by power button.
 *  @param [in] o2Info_struct Oxygen concentration
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckeOxygenConcentrationLowerAlarmStatus(O2_ALARM_t o2Info_struct)//E017
{
    if(s_currentAlarmMonitor.currentFlow <= 0)// this is the case blower does not operate and there is no oxygen source
    {
        // do not check this alarm
        return;
    }
    if(s_TimeStartLowCounter < TWO_MINUTES_AFTER_START)
    {
        s_TimeStartLowCounter++;
        return;
    }
    else
    {
        s_TimeStartLowCounter = TWO_MINUTES_AFTER_START;
    }
    
    if((o2Info_struct.o2Concentration >= 0.9 * o2Info_struct.minO2RangeSetting) && (o2Info_struct.o2Concentration < o2Info_struct.minO2RangeSetting))
    {
        s_OxygenConcentrationIsLowerMediumPriorityCounter++;
        s_OxygenConcentrationIsLowerHighPriorityCounter = 0;

        if (s_OxygenConcentrationIsLowerMediumPriorityCounter >= OXYGENCONCENTRATE_ALARM_TIMEOUT )
        {
            s_alarmList[eOxygenLowAlarmId].currentStatus = eActive;
            s_alarmList[eOxygenLowAlarmId].currentPriority = eMediumPriority;
            s_OxygenConcentrationIsLowerMediumPriorityCounter = 0;
            return;
        }
    }
    else if(o2Info_struct.o2Concentration < 0.9 * o2Info_struct.minO2RangeSetting)
    {
        s_OxygenConcentrationIsLowerHighPriorityCounter++;
        s_OxygenConcentrationIsLowerMediumPriorityCounter = 0;

        if (s_OxygenConcentrationIsLowerHighPriorityCounter >= OXYGENCONCENTRATE_ALARM_TIMEOUT )
        {
            s_alarmList[eOxygenLowAlarmId].currentStatus = eActive;
            s_alarmList[eOxygenLowAlarmId].currentPriority = eHighPriority;
            s_OxygenConcentrationIsLowerHighPriorityCounter = 0;
            return;
        }
    }
    else
    {
        s_OxygenConcentrationIsLowerHighPriorityCounter = 0;
        s_OxygenConcentrationIsLowerMediumPriorityCounter = 0;
    }
    if (s_alarmList[eOxygenLowAlarmId].currentStatus == eActive)
    {
        if ((o2Info_struct.o2Concentration >= o2Info_struct.minO2RangeSetting) && (o2Info_struct.o2Concentration <= o2Info_struct.maxO2RangeSetting))
        {
            s_alarmList[eOxygenLowAlarmId].currentStatus = eInactive;
            s_OxygenConcentrationIsLowerHighPriorityCounter = 0;
            s_OxygenConcentrationIsLowerMediumPriorityCounter = 0; 
        }
    }
    
    return;
}

/** @brief Check status of Oxygen concentration is higher than max limit
 *  Trigger this alarm when calculated oxygen concentration higher than the max limit alarm for more than 5 seconds
 *  Clear this alarm when the oxygen concentration returns to the alarm setting range
 *  Or Power down and restart by power button.
 *  @param [in] o2Info_struct Oxygen concentration
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckeOxygenConcentrationHigherAlarmStatus(O2_ALARM_t o2Info_struct)//E016 - v1
{
    if(s_TimeStartHighCounter < TWO_MINUTES_AFTER_START)
    {
        s_TimeStartHighCounter++;
        return;
    }
    else
    {
        s_TimeStartHighCounter = TWO_MINUTES_AFTER_START;
    }
    
        if((o2Info_struct.o2Concentration > o2Info_struct.maxO2RangeSetting) && (o2Info_struct.o2Concentration <= 1.1 * o2Info_struct.maxO2RangeSetting))
        {
            s_OxygenConcentrationIsHigherMediumPriorityCounter++;
            s_OxygenConcentrationIsHigherHighPriorityCounter = 0;
            
            if (s_OxygenConcentrationIsHigherMediumPriorityCounter >= OXYGENCONCENTRATE_ALARM_TIMEOUT )
            {
                s_alarmList[eOxygenHighAlarmId].currentStatus = eActive;
                s_alarmList[eOxygenHighAlarmId].currentPriority = eMediumPriority;
                s_OxygenConcentrationIsHigherMediumPriorityCounter = 0;
                return;
            }
        }
        else if(o2Info_struct.o2Concentration > 1.1 * o2Info_struct.maxO2RangeSetting)
        {
            s_OxygenConcentrationIsHigherHighPriorityCounter++;
            s_OxygenConcentrationIsHigherMediumPriorityCounter = 0;

            if (s_OxygenConcentrationIsHigherHighPriorityCounter >= OXYGENCONCENTRATE_ALARM_TIMEOUT )
            {
                
                s_alarmList[eOxygenHighAlarmId].currentStatus = eActive;
                s_alarmList[eOxygenHighAlarmId].currentPriority = eHighPriority;
                s_OxygenConcentrationIsHigherHighPriorityCounter = 0;
                return;
            }
        }
        else
        {
            s_OxygenConcentrationIsHigherHighPriorityCounter = 0;
            s_OxygenConcentrationIsHigherMediumPriorityCounter = 0;
        }
    if (s_alarmList[eOxygenHighAlarmId].currentStatus == eActive)
    {
        if ((o2Info_struct.o2Concentration >= o2Info_struct.minO2RangeSetting) && (o2Info_struct.o2Concentration <= o2Info_struct.maxO2RangeSetting))
        {
            s_alarmList[eOxygenHighAlarmId].currentStatus = eInactive;
            s_OxygenConcentrationIsHigherHighPriorityCounter = 0;
            s_OxygenConcentrationIsHigherMediumPriorityCounter = 0; 
        }
    }
    
}
/** @brief Check status of Device Posture
 *  Refer to posture recognition to identify abnormal posture
 *  @param [in] float xAngleDirection X Angle of device which is detected by accelerate sensor
 *  @param [in] float yAngleDirection Y Angle of device which is detected by accelerate sensor
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckDevicePostureAlarmStatus(float xAngleDirection,
                                                   float yAngleDirection)//E018-E019
{

    static E_Posture xAnglePostureState;
    static E_Posture yAnglePostureState;
    static uint32_t StateCount = 0;
    static uint32_t AbnormalStateCount = 0;
    static uint32_t BadStateCount = 0;
    
    if(s_PostureCounter <= POSTURE_SAMPLE)
    {
        s_PostureCounter ++;
        return;
    }
    else
    {
        s_PostureCounter = 0;
        StateCount++;
        
        if( (xAngleDirection >= -5) && (xAngleDirection <= 100) )
        {
            xAnglePostureState = Normal;
        }
        else if(((xAngleDirection >= -20) && (xAngleDirection < -5)) 
                || ((xAngleDirection > 100) && (xAngleDirection <= 120))) // Need to review, Noted on 24/02/2022
        {
            xAnglePostureState = Bad;
        }
        else
            xAnglePostureState = Abnormal;
        
        
        if(((yAngleDirection >= -5) && (yAngleDirection <= 5)) 
                || ((yAngleDirection >= 175) && (yAngleDirection <= 185)))
        {
            yAnglePostureState = Normal;
        }
        else if(((yAngleDirection >= -20) && (yAngleDirection < -5)) 
                || ((yAngleDirection > 5) && (yAngleDirection <= 20))
                || ((yAngleDirection > 185) && (yAngleDirection <= 200))
                || ((yAngleDirection >= 150) && (yAngleDirection < 175)))
        {
            yAnglePostureState = Bad;
        }
        else
            yAnglePostureState = Abnormal;
    }
    
    if((xAnglePostureState == Abnormal) || (yAnglePostureState == Abnormal))
    {
        AbnormalStateCount++;  
    }
    else if ((xAnglePostureState == Bad)||(yAnglePostureState == Bad) )
    {
        BadStateCount ++;
    }
    
    if(StateCount >= 32 )
    {
        if(AbnormalStateCount >= 16)
        {
            if (s_alarmList[eDevicePostureAbnormalAlarmId].currentStatus == eInactive)
                s_alarmList[eDevicePostureAbnormalAlarmId].currentStatus = eActive;
            s_alarmList[eDevicePostureBadAlarmId].currentStatus = eInactive;
        }
        else if((AbnormalStateCount + BadStateCount) >= 16)
        {
            if (s_alarmList[eDevicePostureBadAlarmId].currentStatus == eInactive)
                s_alarmList[eDevicePostureBadAlarmId].currentStatus = eActive;
            s_alarmList[eDevicePostureAbnormalAlarmId].currentStatus = eInactive;
        }
        else
        {
            if ((s_alarmList[eDevicePostureBadAlarmId].currentStatus == eActive)||(s_alarmList[eDevicePostureAbnormalAlarmId].currentStatus == eActive))
            {
                s_alarmList[eDevicePostureBadAlarmId].currentStatus = eInactive;
                s_alarmList[eDevicePostureAbnormalAlarmId].currentStatus = eInactive;
            }
        }
        StateCount = 0;
        AbnormalStateCount = 0;
        BadStateCount = 0;
    }
    return;
}


/** @brief Check status of switch to battery Mode
 * Trigger this alarm when at the time AC power was disconnected and B1+B2???L3
 * Trigger this alarm when at the time Main unit was disconnected to cradle or AC adapter and  B1???L3
 * Trigger this alarm when the following conditions are detected by the self-check at startup
 * and AC not connected and B1 + B2 ? L3
 * Clear this alarm when AC power is reconnected
 * Or Power down and restart by power button
 *  @param [in] bool isACConnected Flag is raised when AC power is not connected
 *  @param [in] bool mainUnitDisFlag Flag is raised when Main unit disconnected
 *  @param [in] bool self_check self-check at startup
 *  @param [in] BATTERY_ALARM_t batteryInfo_struct Battery informations which are received
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckSwitchBatteryModeAlarmStatus(bool isACConnected,
                                                       float batteryRemainingTimeInMin)//E020 - when Cradle is disconnected -> AC is disconnected too
// But if AC is disconnected not mean 
{
    if (s_alarmList[eSwitchToBatteryModeAlarmId].currentStatus == eActive)
    {
        if ((isACConnected == true) /*&& (isCradleConnected == true)*/)//24/05
        {
            s_alarmList[eSwitchToBatteryModeAlarmId].currentStatus = eInactive;
        }
    }
    else if (s_alarmList[eSwitchToBatteryModeAlarmId].currentStatus == eInactive)
    {
        if(isACConnected == false)
        {
            if (batteryRemainingTimeInMin >= 15)
            {
                s_alarmList[eSwitchToBatteryModeAlarmId].currentStatus = eActive;
            }
        }
    }
    return;
}

/** @brief Check status of battery low
 * Trigger this alarm when at the time AC power was disconnected and L2? B1?B2 ? L3
 * Or Trigger this alarm when at the time Main unit was disconnected to cradle or AC adapter and L2 ? B1? L3
 * Or With no AC connected and B1 + B2 become ? L3
 * Trigger this alarm when the following conditions are detected by the self-check 
 * at startup and AC not connected and L2 ? B1+B2 ??L3
 * Clear alarm when AC power is reconnected Or B1+ B2 become ? L3
 * Or Power down and restart by power button
 *  @param [in] bool isACConnected Flag is raised when AC power is not connected
 *  @param [in] bool mainUnitDisFlag Flag is raised when Main unit disconnected
 *  @param [in] bool self_check self-check at startup
 *  @param [in] BATTERY_ALARM_t batteryInfo_struct Battery informations which are received
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckBatteryLowAlarmStatus(bool isACConnected,      // neu mainUnitDisFlag on th� isACConnected on, neu isACConnected off th� mainUnitDisFlag off        // // neu mainUnitDisFlag on thi B2 =0;
                                                float batteryRemainingTimeInMin)//E021
{
    if (s_alarmList[eBatteryLowAlarmId].currentStatus == eActive)
    {
        if (isACConnected == true)
        {
            s_alarmList[eBatteryLowAlarmId].currentStatus = eInactive;
        }
        else if (batteryRemainingTimeInMin > 16)
        {
            s_alarmList[eBatteryLowAlarmId].currentStatus = eInactive;
        }
    }
    else if (s_alarmList[eBatteryLowAlarmId].currentStatus == eInactive)
    {
        if (isACConnected == false)
        {
            if ((batteryRemainingTimeInMin < 15) && (batteryRemainingTimeInMin >= 5))
            {
                s_alarmList[eBatteryLowAlarmId].currentStatus = eActive;
            }
        }
    }
    return;
}

/** @brief Check status of battery get run out
 * Trigger this alarm when at the time AC power was disconnected and L1? B1?B2 ? L2
 * Or Trigger this alarm when at the time Main unit was disconnected to cradle or AC adapter and L1? B1?B2 ? L2
 * Or With no AC connected and B1 + B2 become < L2
 * Or Trigger this alarm when the following conditions are detected by the self-check at startup
 * and AC not connected and L1 ? B1+B2 ??L2
 * Clear this alarm when AC power is reconnected
 * Or B1+ B2 become ? L2
 * Or Power down and restart by power button.
 *  @param [in] bool isACConnected Flag is raised when AC power is not connected
 *  @param [in] bool mainUnitDisFlag Flag is raised when Main unit disconnected
 *  @param [in] bool self_check self-check at startup
 *  @param [in] BATTERY_ALARM_t batteryInfo_struct Battery informations which are received
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckBatteryGetRunOutAlarmStatus(  bool isACConnected, 
                                                        float batteryRemainingTimeInMin)//E022
{
    if (s_alarmList[eBatteryGetsRunOutAlarmId].currentStatus == eActive)
    {
        if (isACConnected == true)
        {
            s_alarmList[eBatteryGetsRunOutAlarmId].currentStatus = eInactive;
        }
        else if (batteryRemainingTimeInMin > 6)
        {
            s_alarmList[eBatteryGetsRunOutAlarmId].currentStatus = eInactive;
        }
    }
    else if (s_alarmList[eBatteryGetsRunOutAlarmId].currentStatus == eInactive)
    {
        if (isACConnected == false)
        {
            if ((batteryRemainingTimeInMin < 5) && (batteryRemainingTimeInMin >= 1))
            {
                s_alarmList[eBatteryGetsRunOutAlarmId].currentStatus = eActive;
            }
        }
    }
    return;
}

/** @brief Check status of Stop function
 * Trigger this alarm when at the time AC power was disconnected and B1?B2 < L1
 * Or Trigger this alarm when at the time Main unit was disconnected to cradle or AC adapter and B1 < L1
 * Or With no AC connected and B1 + B2 become ? L1
 * Trigger this alarm when the following conditions are detected by the self-check at startup
 * And AC not connected and L0 ? B1+B2 ??L1
 * Clear this alarm When AC power is reconnected
 * Or B1+ B2 become ? L1
 * OrPower down and restart by power button
 *  @param [in] bool isACConnected Flag is raised when AC power is not connected
 *  @param [in] bool mainUnitDisFlag Flag is raised when Main unit disconnected
 *  @param [in] bool self_check  self-check at startup
 *  @param [in] BATTERY_ALARM_t batteryInfo_struct Battery informations which are received
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckStopFunctionAlarmStatus(  bool isACConnected,
                                                    float batteryRemainingTimeInMin)//E023
{
    if (s_alarmList[eStopFunctionAlarmId].currentStatus == eActive)
    {
        if (isACConnected == true)
        {
            s_alarmList[eStopFunctionAlarmId].currentStatus = eInactive;
        }
        else if (batteryRemainingTimeInMin > 2)
        {
            s_alarmList[eStopFunctionAlarmId].currentStatus = eInactive;
        }
    }
    else if (s_alarmList[eStopFunctionAlarmId].currentStatus == eInactive)
    {
        if (isACConnected == false)
        {
            if (batteryRemainingTimeInMin < 1)
            {
                s_alarmList[eStopFunctionAlarmId].currentStatus = eActive;

                if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                {
                    s_alarmList[eStopFunctionAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                }
                else
                {
                    s_alarmList[eStopFunctionAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                }
            }
        }
    }
    return; 
}

/** @brief Check status of power is not enough
 * Trigger this alarm when the following conditions are detected by the self-check at startup
 * AC not connected and L0 ? B1+B2 ??L1
 * Clear this alarm When AC power is reconnected
 * Or B1+ B2 become ? L1
 * Or power down and restart by power button.
 *  @param [in] BATTERY_ALARM_t batteryInfo_struct Battery informations which are received
 *  @param [in] bool isACConnected Flag is raised when AC power is not connected
 *  @param [in] bool self_check  self-check at startup
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckPowerNotEnoughAlarmStatus(float batteryRemainingTimeInMin,
                                                    bool isACConnected,
                                                    bool self_check)//E024
{
    if (self_check == true)
    {
        if (s_alarmList[ePowerNotEnoughAlarmId].currentStatus == eInactive)
        {
            if (isACConnected == false)
            {
                if (batteryRemainingTimeInMin < 1)
                {
                    s_alarmList[ePowerNotEnoughAlarmId].currentStatus = eActive;
                }
            }
        }
        else if (s_alarmList[ePowerNotEnoughAlarmId].currentStatus == eActive)
        {
            if (isACConnected == true)
            {
                s_alarmList[ePowerNotEnoughAlarmId].currentStatus = eInactive;
            }
            else if (batteryRemainingTimeInMin > 2)
            {
                s_alarmList[ePowerNotEnoughAlarmId].currentStatus = eInactive;
            }
        }
    }
    else
    {
        if (s_alarmList[ePowerNotEnoughAlarmId].currentStatus == eActive)
        {       
            if (isACConnected == true)
            {
                s_alarmList[ePowerNotEnoughAlarmId].currentStatus = eInactive;
            }
            else if (batteryRemainingTimeInMin > 2)
            {
                s_alarmList[ePowerNotEnoughAlarmId].currentStatus = eInactive;
            }
        }
    } 
    return;
}

/** @brief Check status of Spo2 Failed: SEARCHING TOO LONG or ARTIFACT
 *  Trigger this alarm when receive a SEARCHING TOO LONG or ARTIFACT signal from the Spo2 device
 *  Clear this alarm When valid Spo2 value is received
 *  Or when the Spo2 module is removed from the device
 *  @param [in] SPO2_ALARM_t spo2_struct Sp02 Informations which are received
 *  @param [out] : None
 *  @return None
 */

static void alarmMgr_CheckSpo2FailAlarmStatus(SPO2_ALARM_t spo2Info_struct)//E025
{   
    static uint16_t s_counter = 0;
    if (spo2Info_struct.spo2Connected == false)
    {
        s_alarmList[eSpO2FailedAlarmId].currentStatus = eInactive;
        return;
    }
    if(s_alarmList[eSpO2FailedAlarmId].currentStatus == eInactive)
    {
            if((spo2Info_struct.spo2Message == spo2SearchingTooLong)||(spo2Info_struct.spo2Message == spo2Artifact))
            {
                s_alarmList[eSpO2FailedAlarmId].currentStatus = eActive;
                return;
            }
    }
    
    else if (s_alarmList[eSpO2FailedAlarmId].currentStatus == eActive)
    {
//        if(spo2Info_struct.spo2Value != 0) 
//        {
//            s_alarmList[eSpO2FailedAlarmId].currentStatus = eInactive;
//            return;
//        }
        if((spo2Info_struct.spo2Message != spo2SearchingTooLong)&&(spo2Info_struct.spo2Message != spo2Artifact))
        {
            s_counter++;
            if(s_counter >= 5000/50) 
            {
                s_counter = 0;
                s_alarmList[eSpO2FailedAlarmId].currentStatus = eInactive;
                return;
            }
        }
        else
        {
            s_counter = 0;
        }
    }
    return;
}

/** @brief Check status of Spo2 value is lower than the alarm setting lower limit
 *  Trigger this alarm when the Spo2 value is lower than the alarm setting lower 
 *  limit (lower limit is 90% or more)
 *  Clear this alarm when Spo2 value ? Spo2 Alarm lower limit value
 *  Or when the Spo2 module is removed from the device
 *  Or Power down and restart by power button.
 *  @param [in] SPO2_ALARM_t spo2_struct Sp02 Informations which are received
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckSpo2LowAlarmStatus(SPO2_ALARM_t spo2_struct)//E026
{
    // Check Invalid time
    if (spo2_struct.spo2Connected == false)
    {
        s_alarmList[eSpO2LowAlarmId].currentStatus = eInactive;
        return;
    }
    // Alarm check
    if(s_alarmList[eSpO2LowAlarmId].currentStatus == eInactive)
    {
        if(spo2_struct.spo2Value < spo2_struct.spo2SetLimit)
        {  
            s_alarmList[eSpO2LowAlarmId].currentStatus = eActive;
            return;
        }
    }
    else if (s_alarmList[eSpO2LowAlarmId].currentStatus == eActive)
    {
        if(spo2_struct.spo2Value >= spo2_struct.spo2SetLimit)
        {
            s_alarmList[eSpO2LowAlarmId].currentStatus = eInactive;
        }
    }
    return;
}

/** @brief Check status of Spo2 sensor probe
 *  Trigger this alarm when receive a NO FINGER OR SENSOR PROBLEM signal from the Spo2 device
 *  Clear this alarm when there is no NO FINGER OR SENSOR PROBLEM signal for 3 seconds or longer
 *  Or when the Spo2 module is removed from the device
 *  Or Power down and restart by power button.
 *  @param [in] SPO2_ALARM_t spo2_struct Sp02 Informations which are received
 *  @param [out] : None
 *  @return None
 */
//static void alarmMgr_CheckSpo2SensorProbeAlarmStatus(SPO2_ALARM_t spo2_struct)//E027 - Disable
//{
//    if(s_alarmList[eSpO2SensorProbeAlarmId].currentStatus == eInactive)
//    {
//        if(spo2_struct.spo2Message == spo2NoFingerOrSensorProblem)
//        {
//            s_alarmList[eSpO2SensorProbeAlarmId].currentStatus = eActive;
//            return;
//        }
//    }
//    else if(s_alarmList[eSpO2SensorProbeAlarmId].currentStatus == eActive)
//    {
//        // Deactive alarm: Remove device
//        if(spo2_struct.spo2Connected == false)
//        {
//            s_alarmList[eSpO2SensorProbeAlarmId].currentStatus = eInactive;
//            s_CheckForSpo2SensorProbeCounter = 0;
//            return;
//        }
//        // Deactive alarm: no signal NO FINGER OR SENSOR PROBLEM
//        if(spo2_struct.spo2Message != spo2NoFingerOrSensorProblem)
//        {
//            if(s_CheckForSpo2SensorProbeCounter >= SPO2SENSORPROBE_ALARM_TIMEOUT)
//            {
//                s_alarmList[eSpO2SensorProbeAlarmId].currentStatus = eInactive;
//                s_CheckForSpo2SensorProbeCounter = 0;
//                return;
//            }
//            s_CheckForSpo2SensorProbeCounter ++;
//        }
//        else
//        {
//            s_CheckForSpo2SensorProbeCounter = 0;
//        }
//    }
    
//}

/** @brief Check status of Spo2 signal loss
 *  Trigger this alarm When receive a LOST PULSE signal from the Spo2 device
 *  Clear this alarm when there is no LOST PULSE signal for more than 3 seconds
 *  Or when the Spo2 module is removed from the device
 *  Or power down and restart by power button.
 *  @param [in] SPO2_ALARM_t spo2_struct Sp02 Informations which are received
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckSpo2SignalLossAlarmStatus(SPO2_ALARM_t spo2_struct)//E028
{
    if (spo2_struct.spo2Connected == false)
    {
        s_CheckForSpo2LossPulseCounter = 0;
        s_alarmList[eSpO2SignalLossAlarmId].currentStatus = eInactive;
        return;
    }
    
    if(s_alarmList[eSpO2SignalLossAlarmId].currentStatus == eInactive)
    {
        if(spo2_struct.spo2Message == spo2LostPulse)
        {
            s_alarmList[eSpO2SignalLossAlarmId].currentStatus = eActive;
            return;
        }
    }
    else if(s_alarmList[eSpO2SignalLossAlarmId].currentStatus == eActive)
    {
        if(spo2_struct.spo2Message != spo2LostPulse)
        {
            if(s_CheckForSpo2LossPulseCounter >= SPO2LOSSPULSE_ALARM_TIMEOUT)
            {
                s_alarmList[eSpO2SignalLossAlarmId].currentStatus = eInactive;
                s_CheckForSpo2LossPulseCounter = 0;
                return;
            }
            s_CheckForSpo2LossPulseCounter ++;
        }
        else 
        {
            s_CheckForSpo2LossPulseCounter = 0;
        }
    }
    
}

/** @brief Check status of Spo2 sensor uplugged
 *  Trigger this alarm When receive a SENSOR UNPLUGGED signal from the Spo2 device
 *  Clear this alarm when the Spo2 module was reconnected to the device
 *  Or power down and restart by power button.
 *  @param [in] spo2 the struct which save SpO2 Information
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckSpo2SensorUnpluggedAlarmStatus(SPO2_ALARM_t spo2Info_struct)//E029
{
    // Check Invalid time
    
    // Check alarm
    if(s_alarmList[eSpO2SensorUnpluggedAlarmId].currentStatus == eInactive)
    {
        if(spo2Info_struct.spo2Connected == false)
        {
            s_alarmList[eSpO2SensorUnpluggedAlarmId].currentStatus = eActive;
            return;
        }
    }
    
    else if (s_alarmList[eSpO2SensorUnpluggedAlarmId].currentStatus == eActive)
    {
        if((spo2Info_struct.spo2Connected == true))
        {
            s_alarmList[eSpO2SensorUnpluggedAlarmId].currentStatus = eInactive;
        }
    }
    return;
}

/** @brief Water supply check
 * 
 * Read water tank status every 2s. If not full, turn on pump on cradle
 * If water tank not full in 3 minutes trigger an alarm
 * 
 *  @param [in] waterTankStatus: Status of water tank output from function Chamber_GetTankWaterLevel
 *  @param [in] isCradleConnected: Cradle connection status
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckWaterSupplyAlarmStatus(E_TankWaterLevel waterTankStatus,
                                                 bool   isCradleConnected)//E030
{
    if (isCradleConnected == false)
    {
        return;
    }
    // Check alarm
    if (s_alarmList[eWaterSupplyCheckAlarmId].currentStatus == eInactive)
    {
        if (waterTankStatus == eTankIsNotFull)
        {
            if (s_CheckWaterSupplyCounter >= WATER_SUPPLY_CHECK_TIMEOUT)
            {
                s_alarmList[eWaterSupplyCheckAlarmId].currentStatus = eActive;
                s_CheckWaterSupplyCounter = 0;
                return;
            }
            s_CheckWaterSupplyCounter++;
            return;
        }
        else
        {
            s_CheckWaterSupplyCounter = 0;
        }
    }
    else if (s_alarmList[eWaterSupplyCheckAlarmId].currentStatus == eActive)
    {
        if (waterTankStatus == eTankIsFull)
        {
            s_alarmList[eWaterSupplyCheckAlarmId].currentStatus = eInactive;
        }
    }
    return;
}

/** @brief Water tank connection check
 * 
 * Read status of water tank every 2 s. If the tank is not connected, trigger an alarm
 * 
 *  @param [in] waterTankStatus: Status of water tank output from function Chamber_GetTankWaterLevel
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckWaterTankConnectionAlarmStatus(E_TankWaterLevel waterTankStatus)// E031
{

    // Check Invalid time
    // Check Alarm
    if (s_alarmList[eWaterTankConnectionCheckAlarmId].currentStatus == eInactive)
    {
        if (waterTankStatus == eTankIsDisconnected)
        {
            s_alarmList[eWaterTankConnectionCheckAlarmId].currentStatus = eActive;
            return;
        }
    }
    else if (s_alarmList[eWaterTankConnectionCheckAlarmId].currentStatus == eActive)
    {
        if ((waterTankStatus != eTankIsDisconnected))// && (waterTankStatus != eTankIsUnknow))
        {
            s_alarmList[eWaterTankConnectionCheckAlarmId].currentStatus = eInactive;
            return;
        }
    }
}

/** @brief Air flow abnormal check
 * 
 * Trigger this alarm when the air flow is more or less than 20% of the set air flow for more than 10 seconds
 * 
 *  @param [in] waterTankLevel: Status of water tank output from function Chamber_GetTankWaterLevel
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckAirFlowAbnormalAlarmStatus(float currentFlow,
                                                     float setFlow,
                                                     E_TreatmentMode machineMode,
                                                     float blowerControlValue)// E032
{
//    static float upperLimit = 0;//1.05 * setFlow;
    
//    upperLimit = 1.05 * setFlow;
    if(s_TimeStartAirflowAbnormalCounter < TWO_MINUTES_AFTER_START)
    {
        s_TimeStartAirflowAbnormalCounter++;
        return;
    }
    else
    {
        s_TimeStartAirflowAbnormalCounter = TWO_MINUTES_AFTER_START;
    }
    
    if (machineMode == eDryMode || machineMode == eDisinfectionMode || (blowerControlValue <= 0))
        return;
    
    if ((s_alarmList[eOxygenLowAlarmId].currentStatus == eActive) || (s_alarmList[eCheckBlockageAlarmId].currentStatus == eActive)) // E017 & E003 is occuring.
    {
        return;
    }
    
    if (s_alarmList[eAirFlowAbnormalAlarmId].currentStatus == eInactive)
    {
        if ((currentFlow < 0.8 * setFlow) || (currentFlow > 1.2 * setFlow))
        {
            if (s_AirFlowAbnormalCounter >= AIR_FLOW_ABNORMAL_CHECK_TIMEOUT)
            {
                s_alarmList[eAirFlowAbnormalAlarmId].currentStatus = eActive;
                s_AirFlowAbnormalCounter = 0;
                return;
            }
            s_AirFlowAbnormalCounter++;
        }
    }
    else if (s_alarmList[eAirFlowAbnormalAlarmId].currentStatus == eActive)
    {
        if ((currentFlow >= 0.95 * setFlow) && (currentFlow <= 1.05 * setFlow))
        {
            if (s_AirFlowAbnormalCounter >= AIR_FLOW_ABNORMAL_CHECK_TIMEOUT)
            {
                s_alarmList[eAirFlowAbnormalAlarmId].currentStatus = eInactive;
                s_AirFlowAbnormalCounter = 0;
                return;
            }
            s_AirFlowAbnormalCounter++;
        }
    }

}


/** @brief Main Unit battery disconnected check
 * 
 * (TBD)
 * 
 *  @param [in] : None
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckMainBatDisconnectedAlarmStatus(bool isMainUnitBatteryConnected,
                                                         bool isCradleConnected,
                                                         bool self_check) // E033
{
    static uint8_t s_counter = 0;
    if (s_alarmList[eMainUnitBatteryDisconnectedAlarmId].currentStatus == eInactive)
    {
        if (isMainUnitBatteryConnected == false)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eMainUnitBatteryDisconnectedAlarmId].currentStatus = eActive;
                if (self_check)
                {
                    if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                    {
                        s_alarmList[eMainUnitBatteryDisconnectedAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                    }
                    else
                    {
                        s_alarmList[eMainUnitBatteryDisconnectedAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                    }
                }
                else
                {
                    if (isCradleConnected)
                    {
                        s_alarmList[eMainUnitBatteryDisconnectedAlarmId].data[eResetButtonStatusDataIdx] = eResetButtonShowReset;
                    }
                    else
                    {
                        s_alarmList[eMainUnitBatteryDisconnectedAlarmId].data[eResetButtonStatusDataIdx] = eResetButtonHide;
                    }
                }
            }
            else
            {
                s_counter++;
            }
            
        }
        else
        {
            s_counter = 0;
        }
    }
    else if (s_alarmList[eMainUnitBatteryDisconnectedAlarmId].currentStatus == eActive)
    {
        if (isMainUnitBatteryConnected == true)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eMainUnitBatteryDisconnectedAlarmId].currentStatus = eInactive;
            }
            else
            {
                s_counter++;
            }
        } 
        else 
        {
            s_counter = 0;
        }
    } 
    return;
}

/** @brief Cradle battery disconnected check
 * 
 * (TBD)
 * 
 *  @param [in] : None
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckCradleBatDisconnectedAlarmStatus(bool isCradleConnected,
                                                           bool isCradleBatteryConnected) // E034
{
    static uint8_t s_counter = 0;
    
    
    if (isCradleConnected == false)
    {
        s_alarmList[eCradleBatteryDisconnectedAlarmId].currentStatus = eInactive;
        return;
    }
    
    if (s_alarmList[eCradleBatteryDisconnectedAlarmId].currentStatus == eInactive)
    {
        if (isCradleBatteryConnected == false)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eCradleBatteryDisconnectedAlarmId].currentStatus = eActive;
            }
            else
            {
                s_counter++;
            }
        }
        else
        {
            s_counter = 0;
        }
    }
    else if (s_alarmList[eCradleBatteryDisconnectedAlarmId].currentStatus == eActive)
    {
        if (isCradleBatteryConnected == true)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eCradleBatteryDisconnectedAlarmId].currentStatus = eInactive;
            }
            else
            {
                s_counter++;
            }
        }
        else
        {
            s_counter = 0;
        }
    } 
    return;
}

/** @brief Check status of Failure of Oxygen flow measurement flow sensor
 *  Retry when communication error and data error are detected. Trigger this alarm 
 *  if the problem is not resolved after three retries
 *  Clear this alarm when passing the self-check at the next startup
 *  @param [in] uint8_t failureO2FlowSensorCounter the number of the trying to reconnect O2 flow sensor
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckFailureOxygenFlowMeasurementFlowSensorAlarmStatus(bool isFailureO2FlowSensor) // E100
{
    // DEBUG
    // Check Alarm
    if(isFailureO2FlowSensor == true){
       s_alarmList[eFailureOxygenFlowMeasurementFlowSensorAlarmId].currentStatus = eActive;
       if(s_currentAlarmMonitor.currentO2Flow > 1.0)
       {
           s_alarmList[eFailureOxygenFlowMeasurementFlowSensorAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
       }
       else{
           s_alarmList[eFailureOxygenFlowMeasurementFlowSensorAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
       }
    }
    else{
       s_alarmList[eFailureOxygenFlowMeasurementFlowSensorAlarmId].currentStatus = eInactive;
    }
    return;
}

/** @brief Check status of Failure of Oxygen flow measurement flow sensor
 *  Retry when communication error and data error are detected. Trigger this alarm
 *  if the problem is not resolved after three retries
 *  Clear this alarm when passing the self-check at the next startup
 *  @param [in] uint8_t failureAirFlowSensorCounter the number of the trying to reconnect Air flow sensor
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckFailureAirFlowMeasurementFlowSensorAlarmStatus(bool isFailureAirFlowSensor) // E101
{
    static uint8_t s_counter = 0;
    
    if(isFailureAirFlowSensor == true){
        if (s_counter >= 2000/50)
        {
            s_counter = 0;
            s_alarmList[eFailureAirFlowMeasurementFlowSensorAlarmId].currentStatus = eActive;
            if(s_currentAlarmMonitor.currentO2Flow > 1.0)
            {
                s_alarmList[eFailureAirFlowMeasurementFlowSensorAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
            }
            else{
                s_alarmList[eFailureAirFlowMeasurementFlowSensorAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
            }            
        }
        else
        {
            s_counter++;
        }
    }
    else
    {
        s_counter = 0;
    }
    return;
}

/** @brief Check status of Failure of chamber temperature sensors
 *  Retry when communication error and data error are detected. Trigger this alarm
 *  if the problem is not resolved after three retries
 *  Clear this alarm when passing the self-check at the next startup
 *  @param [in] uint8_t failureChamberTemperatureSensorCounter the number of the trying to reconnect chamber temperature sensors
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckFailureChamberTemperatureSensorAlarmStatus(float chamberTemperatureSensor,
                                                                     bool isChamberConnected) // E102
{
    static uint8_t s_count = 0;
    //Check Invalid time
    if (isChamberConnected == false)
        return;
    
    //Check Alarm
    if(s_alarmList[eFailureChamberTemperatureSensorAlarmId].currentStatus == eInactive)
    {
        if((chamberTemperatureSensor < -20) || (chamberTemperatureSensor > 100))    
        {  
            s_count++;
            if (s_count >= 2000/50)
            {
                s_count = 0;
                s_alarmList[eFailureChamberTemperatureSensorAlarmId].currentStatus = eActive; 
            }
        }
        else
        {
            s_count = 0;
        }
    }
    return;
}

/** @brief Check status of Failure of Coil temperature sensor
 *  Trigger this alarm when the output voltage of the respiratory circuit outlet 
 *  temperature sensor lower 0.01 V (TBD) for more than 1 second
 *  Clear this alarm when passing the self-check at the next startup
 *  @param [in] float coilTemperatureSensorVolt The current voltage of coil temperature sensor
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckFailureCoilTemperatureSensorAlarmStatus(float coilTemperatureSensorTemp,
                                                                  bool isChamberConnected) // E103
{
    static uint8_t s_count = 0;
    //Check Invalid time
    if (isChamberConnected == false)
        return;
    
    //Check Alarm
    if(s_alarmList[eFailureCoilTemperatureSendorAlarmId].currentStatus == eInactive)
    {
        if((coilTemperatureSensorTemp < -20) || (coilTemperatureSensorTemp > 300))    
        {   
            s_count++;
            if (s_count >= 2000/50)
            {
                s_alarmList[eFailureCoilTemperatureSendorAlarmId].currentStatus = eActive;
                if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                {
                    s_alarmList[eFailureCoilTemperatureSendorAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage;                
                }
                else{
                    s_alarmList[eFailureCoilTemperatureSendorAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                }
            }
        }
        else
        {
            s_count = 0;
        }
    }
    return;
}

/** @brief Check status of Failure of Breathing circuit out temperature sensor
 *  Trigger this alarm when the output voltage of the respiratory circuit outlet 
 *  temperature sensor lower 0.01 V (TBD) for more than 1 second
 *  Clear this alarm when passing the self-check at the next startup
 *  @param [in]  float breathingCircuitTemperatureSensorVolt The current voltage of breathing Circuit temperature sensor
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckFailureBreathingCircuitOutTemperatureSensorAlarmStatus(float breathingCircuitTemperatureSensorTemp,
                                                                                 bool isChamberConnected,
                                                                                 E_BreathingCircuitType circuitType)//E104
{
    static uint8_t s_count = 0;
    
    //Check Invalid time
    if (isChamberConnected == false)
        return;
    else if ((circuitType != eTypeAdult) && (circuitType != eTypePediatric) && (circuitType != eTypeDisinfection))
    {
        return;
    }
    //Check Alarm
    if(s_alarmList[eFailureBreathingCircuitOutTemperatureSensorAlarmId].currentStatus == eInactive)
    {
        if((breathingCircuitTemperatureSensorTemp < -20) || (breathingCircuitTemperatureSensorTemp > 100))    
        {  
            s_count++;
            if (s_count >= 2000/50)
            {
                s_count = 0;
                s_alarmList[eFailureBreathingCircuitOutTemperatureSensorAlarmId].currentStatus = eActive; 
                if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                {
                    s_alarmList[eFailureBreathingCircuitOutTemperatureSensorAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                }
                else{
                    s_alarmList[eFailureBreathingCircuitOutTemperatureSensorAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage ;
                }
            }
        }
        else
        {
            s_count = 0;
        }
    }
    return;
}

/** @brief Check status of Failure of Environment sensor  (Temp, hum, pressure
 *  Retry when communication error and data error are detected. Trigger this alarm 
 *  if the problem is not resolved after three retries
 *  Clear this alarm when passing the self-check at the next startup
 *  @param [in] bool isFailureBME280Sensor the number of the trying to reconnect Environment sensor
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckFailureEnvironmentSensorAlarmStatus(bool isFailureBME280Sensor) // E105
{
    // DEBUG
    if(s_alarmList[eFailureEnvironmentSensorAlarmId].currentStatus == eInactive)
    {
        if(isFailureBME280Sensor == true)
        {
            s_alarmList[eFailureEnvironmentSensorAlarmId].currentStatus = eActive;
            if(s_currentAlarmMonitor.currentO2Flow > 1.0)
            {
                s_alarmList[eFailureEnvironmentSensorAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage;                
            }
            else{
                s_alarmList[eFailureEnvironmentSensorAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
            }
        }
    }
    return;
}

/** @brief Check status of Failure of Current sensor (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckFailureCurrentSensorAlarmStatus(float sensor1Volt, float sensor2Volt) // E106
{
    static int s_counter1 = 0;
    static int s_counter2 = 0;
    
    if ((sensor1Volt < 0)||(sensor1Volt > 3.3))
    {
        s_counter1++;
    }
    else
    {
        s_counter1 = 0;
    }
    
    if ((sensor2Volt < 0) || (sensor2Volt > 3.3))
    {
        s_counter2++;
    }
    else
    {
        s_counter2 = 0;
    }
        
    if ((s_counter1 >= 1000/50) || (s_counter2 >= 1000/50))
    {
        s_alarmList[eFailureCurrentSensorAlarmId].currentStatus = eActive;
        if(s_currentAlarmMonitor.currentO2Flow > 1.0)
        {
            s_alarmList[eFailureCurrentSensorAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage;                
        }
        else{
            s_alarmList[eFailureCurrentSensorAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
        }
    }
    return;
}

/** @brief Check status of Failure of Blower (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckFailureBlowerAlarmStatus(float currentBlowerRotationSpeed,
                                                   float blowerControlValue) // E107
{
    static uint16_t s_count = 0;
    if (s_alarmList[eFailureBlowerAlarmId].currentStatus == eInactive)
    {
        if (blowerControlValue <= 0)
        {
            if(currentBlowerRotationSpeed >= 10)
            {
                s_count++;
                if (s_count >= 20000/50)
                {
                    s_count = 0;
                    s_alarmList[eFailureBlowerAlarmId].currentStatus = eActive;
                    if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                    {
                        s_alarmList[eFailureBlowerAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                    }
                    else{
                        s_alarmList[eFailureBlowerAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                    }
                }
            }
            else
            {
                s_count = 0;
            }
        }
        else
        {
            if (currentBlowerRotationSpeed <= 5000)
            {
                s_count++;
                if (s_count >= 20000/50)
                {
                    s_count = 0;
                    s_alarmList[eFailureBlowerAlarmId].currentStatus = eActive;
                    if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                    {
                        s_alarmList[eFailureBlowerAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                    }
                    else{
                        s_alarmList[eFailureBlowerAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                    }
                }
            }
            else
            {
                s_count = 0;
            }
        }
    }
    return;
}

/** @brief Check status of broken or disconnection speaker
 *  @param [in] bool isSpeakerBrokenOrDisconnected
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckSpeakerBrokenOrDisconnectedStatus(bool isSpeakerBrokenOrDisconnected) // E108
{
    static uint8_t s_counter = 0;
//    SYS_PRINT("\nS_Counter: [%d]", s_counter);
    if (s_alarmList[eSpeakerDisconnectedAlarmId].currentStatus == eInactive)
    {
        if (isSpeakerBrokenOrDisconnected == true)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eSpeakerDisconnectedAlarmId].currentStatus = eActive;                               
            }
            else 
            {
                s_counter++;
            }
            
        }
        else{
            s_counter = 0;
        }
    }
    else
    {
        if (isSpeakerBrokenOrDisconnected == false)
        {
            s_alarmList[eSpeakerDisconnectedAlarmId].currentStatus = eInactive; 
        }
    }
    return;
}

/** @brief Check status of Failure of water level sensor (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckFailureWaterLevelSensorAlarmStatus(bool isFailureWaterLevelSensor) // E109
{
    return;
}

/** @brief Check status of Failure of external flash memory (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckFailureExternalFlashMemoryAlarmStatus(bool isFailureExternalFlashMemory) // E110
{
    static uint8_t s_counter = 0;
//    SYS_PRINT("\nS_Counter: [%d]", s_counter);
    if (s_alarmList[eFailureExternalFlashMemoryAlarmId].currentStatus == eInactive)
    {
        if (isFailureExternalFlashMemory == true)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eFailureExternalFlashMemoryAlarmId].currentStatus = eActive;
                                
                if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                {
                    s_alarmList[eFailureExternalFlashMemoryAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                }
                else
                {
                    s_alarmList[eFailureExternalFlashMemoryAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                }
            }
            else 
            {
                s_counter++;
            }
            
        }
    }
    return;
}

/** @brief Check status of Failure of acceleration sensor (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None
 */
static void alarmMgr_CheckFailureAccelerationSensorAlarmStatus(bool isFailureAccelerationSensor) // E111
{
    static uint8_t s_counter = 0;
    if (s_alarmList[eFailureAccelerationSensorAlarmId].currentStatus == eInactive)
    {
        if (isFailureAccelerationSensor == true)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eFailureAccelerationSensorAlarmId].currentStatus = eActive;
            }
            else
            {
                s_counter++;
            }
        }
    }
    return;
}

/** @brief Check status of Failure of light sensor (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None  
 */   
static void alarmMgr_CheckFailureLightSensorAlarmIdAlarmStatus(bool isFailureLightSensorAlarmId) // E112 - Disable this alarm
{
    return;
}

/** @brief Check status of Failure of Main Unit Battery communication error (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None  
 */   
static void alarmMgr_CheckFailureMainUnitBatteryCommunicationErrorAlarmIdAlarmStatus(bool isFailureMainUnitBatteryCommunication) // E113
{
    static uint8_t s_counter = 0;
    // Check Invalid time
    if (s_alarmList[eMainUnitBatteryDisconnectedAlarmId].currentStatus == eActive)
        return;
    // Check Alarm
    if (s_alarmList[eMainUnitBatteryCommunicationErrorAlarmId].currentStatus == eInactive)
    {
        if (isFailureMainUnitBatteryCommunication == true)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eMainUnitBatteryCommunicationErrorAlarmId].currentStatus = eActive;
            }
            else
            {
                s_counter++;
            }
        }
        else
        {
            s_counter = 0;
        }
    }
    else if (s_alarmList[eMainUnitBatteryCommunicationErrorAlarmId].currentStatus == eActive)
    {
        if (isFailureMainUnitBatteryCommunication == false)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eMainUnitBatteryCommunicationErrorAlarmId].currentStatus = eInactive;
            }
            else
            {
                s_counter++;
            }
        }
        else
        {
            s_counter = 0;
        }
    }
    return;
}

/** @brief Check status of Failure of Cradle Battery communication error (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None  
 */   
static void alarmMgr_CheckFailureCradleBatteryCommunicationErrorAlarmIdAlarmStatus(bool isFailureCradleBatteryCommunication,
                                                                                   bool isCradleConnected,
                                                                                   bool isCradleBatteryConnected,
                                                                                   bool isACConnected) // E114
{
    static uint8_t s_counter = 0;
//    SYS_PRINT("\ns_counter: [%d]", s_counter);
    // For test
//    s_alarmList[eMainUnitBatteryCommunicationErrorAlarmId].currentStatus = eActive;
    // Check Invalid time
    if (isCradleConnected == false)
    {
        s_alarmList[eCradleBatteryCommunicationErrorAlarmId].currentStatus = eInactive;
        return;
    }
    else if (isCradleBatteryConnected == false)
    {
        s_alarmList[eCradleBatteryCommunicationErrorAlarmId].currentStatus = eInactive;
        return;
    }
    // Check Alarm    
    if (s_alarmList[eCradleBatteryCommunicationErrorAlarmId].currentStatus == eInactive)
    {
        if (isFailureCradleBatteryCommunication == true)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eCradleBatteryCommunicationErrorAlarmId].currentStatus = eActive;
//                if (!isACConnected && s_alarmList[eMainUnitBatteryCommunicationErrorAlarmId].currentStatus == eActive)
//                {
//                    s_alarmList[eCradleBatteryCommunicationErrorAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;
//                }
//                else
//                {
//                    s_alarmList[eCradleBatteryCommunicationErrorAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage ;
//                }
            }
            else
            {
                s_counter++;
            }
        }
        else
        {
            s_counter = 0;
        }
    }
    else if (s_alarmList[eCradleBatteryCommunicationErrorAlarmId].currentStatus == eActive)
    {
        if (isFailureCradleBatteryCommunication == false)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eCradleBatteryCommunicationErrorAlarmId].currentStatus = eInactive;
            }
            else
            {
                s_counter++;
            }
        }
        else
        {
            s_counter = 0;
        }
    }
    return;
}

/** @brief Check status of Failure of Cradle communication error (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None  
 */   
static void alarmMgr_CheckFailureCradleCommunicationErrorAlarmIdAlarmStatus(bool isFailureCradleCommunicationError,
                                                                            bool isCradleConnected) // E115
{
    static uint8_t s_counter = 0;
    if (isCradleConnected == false)
    {
        s_alarmList[eCradleCommunicationErrorAlarmId].currentStatus = eInactive;
        return;
    }
    if (s_alarmList[eCradleCommunicationErrorAlarmId].currentStatus == eInactive)
    {
        if (isFailureCradleCommunicationError == true)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eCradleCommunicationErrorAlarmId].currentStatus = eActive;
            }
            else
            {
                s_counter++;
            }
        }
        else
        {
            s_counter = 0;
        }
    }
    else if (s_alarmList[eCradleCommunicationErrorAlarmId].currentStatus == eActive)
    {
        if (isFailureCradleCommunicationError == false)
        {
            if (s_counter >= 2000/50)
            {
                s_counter = 0;
                s_alarmList[eCradleCommunicationErrorAlarmId].currentStatus = eInactive;
            }
            else
            {
                s_counter++;
            }
        }
        else
        {
            s_counter = 0;
        }
    }
    return;
}



/** @brief Check status of Failure of Main Unit run out of control error (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None  
 */   
static void alarmMgr_CheckFailureMainRunOutofControlAlarmStatus(bool isFailureMainRunOutofControl) // E116
{
    return;
}

/** @brief Check status of Failure of ESP32 run out of control error (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None  
 */   
static void alarmMgr_CheckESP32FailedAlarmStatus(bool isESP32Failed) // E117
{
    return;
}

/** @brief Check status of Failure of Breathing circuit heater wire broken (TBD)
 *  @param [in] None
 *  @param [out] : None
 *  @return None  
 */   
static void alarmMgr_CheckBreathingCircuitHeaterWireBrokenAlarmStatus(float breathingCircuitOutTemperature,
                                                                      E_BreathingCircuitType BreathingCircuitType,
                                                                      float breathingCircuitControlPower,
                                                                      bool warmingUpStatus,
                                                                      E_TreatmentMode deviceMode) // E118
{
//    SYS_PRINT("\nBreathing Hot: [%d] - Breathing Type: [%d] - [A - %d]", isBreathingCircuitHeaterWireBroken, (int) BreathingCircuitType, s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].currentStatus);
    static bool s_startFlag = true;
    static uint32_t s_switchToBattTimeOutCounter = 0;
    static bool s_switchToBattFlag = false;
    static uint32_t s_counter = 0;
    
    static float s_temp1 = 0;
    static float s_temp2 = 0;
    // Debug
//    SYS_PRINT("\nE118 Counter: [%d]", s_counter);
    // End Debug
//    if (breathingCircuitControlPower < 100)
//        return;
    
    if (s_switchToBattFlag == false)
    {
        if (s_alarmList[eSwitchToBatteryModeAlarmId].currentStatus == eActive)
        {
            s_switchToBattFlag = true;
            return;
        }
    }
    else
    {
        if((++s_switchToBattTimeOutCounter) >= 300000/50)
        {
            s_switchToBattFlag = false;
            s_switchToBattTimeOutCounter = 0;
        }
        else
            return;        
    }
    // Check invalid time
    if ((s_alarmList[eFailureBreathingCircuitOutTemperatureSensorAlarmId].currentStatus == eActive) || (deviceMode == eDisinfectionMode)
            || (deviceMode == eDryMode))
    {
        return;
    }
    
    // Clear alarm when breathing circuit is removed
    if ((BreathingCircuitType != eTypeAdult) && (BreathingCircuitType != eTypePediatric) && (BreathingCircuitType != eTypePediatric))
    {
        s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].currentStatus = eInactive;
        return;
    }
    
    if (s_startFlag == true)
    {
        s_temp1 = breathingCircuitOutTemperature;
        s_startFlag = false;
    }
    
    if (s_counter >= 60000/50)
    {
        s_counter = 0;
        s_startFlag = true;
        s_temp2 = breathingCircuitOutTemperature;
        
        if (warmingUpStatus == eWarmingUp)
        {
            if ((s_temp2 < s_temp1 + 0.1)&&(breathingCircuitControlPower == 100))
            {
                s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].currentStatus = eActive;

                if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                {
                    s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                }
                else
                {
                    s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                }
            }
            else
            {
                s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].currentStatus = eInactive;
            }
        }
        else
        {
            if ((s_temp2 < s_temp1)&&(breathingCircuitControlPower == 100))
            {
                s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].currentStatus = eActive;
                
                if(s_currentAlarmMonitor.currentO2Flow > 1.0)
                {
                    s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage ;                
                }
                else
                {
                    s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].data[eAdditionalMessageDataIdx] = eDisableAdditionalMessage;
                }
            }
            else
            {
                s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].currentStatus = eInactive;
            }
            
        }
    }
    else
    {
        s_counter++;
    }
    
    
}


/** @brief Check status of Failure of ADXL345 sensor
 *  @param [in] None
 *  @param [out] : None
 *  @return None  
 */  
static void alarmMgr_CheckFailureADXL345SensorAlarmStatus(bool isFailureADXL345Sensor)
{
    if(isFailureADXL345Sensor == true){
       s_alarmList[eADXL345ErrorAlarmID].currentStatus = eActive;
    }
    else{
       s_alarmList[eADXL345ErrorAlarmID].currentStatus = eInactive;
    }
    return;
}

/** @brief Check status of Failure of spo2 module
 *  @param [in] isSpo2ModuleFailure spo2 module failure or not
 *  @param [out] : None
 *  @return None  
 */
static void alarmMgr_CheckFailureSpo2ModuleAlarmStatus(bool isSpo2ModuleFailure, SPO2_ALARM_t spo2Info_struct) // E119
{
    // Check invalid time
    if (spo2Info_struct.spo2Connected == true)
    {
        // check alarm
        if (s_alarmList[eFailureSpo2ModuleAlarmId].currentStatus == eActive)
        {
            if (isSpo2ModuleFailure == false)
            {
                s_alarmList[eFailureSpo2ModuleAlarmId].currentStatus = eInactive;
            }
        }
        else if (s_alarmList[eFailureSpo2ModuleAlarmId].currentStatus == eInactive) 
        {
            if (isSpo2ModuleFailure == true)
            {
                s_alarmList[eFailureSpo2ModuleAlarmId].currentStatus = eActive; 
            }
        }
    }
    return;
}

/** @brief Check status of Failure of RTC module
 *  @param [in] isRTCModuleFailure RTC module failure flag
 *  @param [out] : None
 *  @return None  
 */
static void alarmMgr_CheckFailureRTCModuleAlarmStatus(bool isRTCModuleFailure)// E120
{
    // Check alarm
    if (s_alarmList[eFailureRTCModuleAlarmId].currentStatus == eActive)
    {
        if (isRTCModuleFailure == false)
        {
            s_alarmList[eFailureRTCModuleAlarmId].currentStatus = eInactive;
        }
    }
    else if (s_alarmList[eFailureRTCModuleAlarmId].currentStatus == eInactive) 
    {
        if (isRTCModuleFailure == true)
        {
            s_alarmList[eFailureRTCModuleAlarmId].currentStatus = eActive; 
        }
    }
}

/** @brief Check status of Failure of LCD touch module
 *  @param [in] isLCDTouchModuleFailure LCD touch module failure flag
 *  @param [out] : None
 *  @return None  
 */
static void alarmMgr_CheckFailureLCDTouchModuleAlarmStatus(bool isLCDTouchModuleFailure)// E121
{
    // Check alarm
    if (s_alarmList[eFailureLCDTouchModuleAlarmId].currentStatus == eActive)
    {
        if (isLCDTouchModuleFailure == false)
        {
            s_alarmList[eFailureLCDTouchModuleAlarmId].currentStatus = eInactive;
        }
    }
    else if (s_alarmList[eFailureLCDTouchModuleAlarmId].currentStatus == eInactive) 
    {
        if (isLCDTouchModuleFailure == true)
        {
            s_alarmList[eFailureLCDTouchModuleAlarmId].currentStatus = eActive; 
        }
    }
}

/** @brief Check chamber temperature abnormal
 *  @param [in] coilTemperatureSensorTemp EVT temperature
 *  @param [in] chamberTemperatureSensorTemp Chamber out temperature
 *  @param [out] : None
 *  @return None  
 */
static void alarmMgr_CheckChamberTemperatureAbnormalAlarmStatus(float coilTemperatureSensorTemp,
                                                           float chamberTemperatureSensorTemp)// E124
{
    static uint8_t s_coilTempIdx = 0;
    static uint8_t s_coilTempNumSamp = 0;
    static float s_avgChamberCoilTemperature;
    
    static uint16_t s_chamberOutTempIdx = 0;
    static uint16_t s_chamberOutNumSamp = 0;
    static float s_avgChamberOutTemperature;
    
    s_ChamberCoilTemperature[s_coilTempIdx] = coilTemperatureSensorTemp;
//    SYS_PRINT("\n\nCoil Temp: %f + %d", s_ChamberCoilTemperature[s_coilTempIdx], s_coilTempIdx);
    s_coilTempIdx = (++s_coilTempIdx) % EVT_TEMP_NUM_SAMPLE_IN_1SECOND;
    
    if (s_coilTempNumSamp < EVT_TEMP_NUM_SAMPLE_IN_1SECOND)
    {
        s_coilTempNumSamp++;
    }
    
    if (s_coilTempNumSamp >= EVT_TEMP_NUM_SAMPLE_IN_1SECOND)
    {
        s_avgChamberCoilTemperature = 0;
        int i;
        for (i = 0; i < EVT_TEMP_NUM_SAMPLE_IN_1SECOND; i++)
        {
//            SYS_PRINT("\nTemp [%d]: [%f]", i, s_ChamberCoilTemperature[i]);
            s_avgChamberCoilTemperature += (float)s_ChamberCoilTemperature[i]/EVT_TEMP_NUM_SAMPLE_IN_1SECOND;
        }
        //SYS_PRINT("\navgChamberCoilTemperature: [%f]", s_avgChamberCoilTemperature);
        if (s_alarmList[eChamberTemperatureAbnormalAlarmId].currentStatus == eInactive)
        {
            if (s_avgChamberCoilTemperature >= 200)
            {
                s_alarmList[eChamberTemperatureAbnormalAlarmId].currentStatus = eActive;
                return;
            }
        }
    }
    
    s_ChamberOutTemperature[s_chamberOutTempIdx] = chamberTemperatureSensorTemp;
    s_chamberOutTempIdx = (++s_chamberOutTempIdx)%CHAMBER_OUT_TEMP_NUM_SAMPLE_IN_10SECOND;
    
    if (s_chamberOutNumSamp < CHAMBER_OUT_TEMP_NUM_SAMPLE_IN_10SECOND)
    {
        s_chamberOutNumSamp++;
    }
    
    if (s_chamberOutNumSamp >= CHAMBER_OUT_TEMP_NUM_SAMPLE_IN_10SECOND)
    {
        s_avgChamberOutTemperature = 0;
        int i;
        for (i = 0; i < CHAMBER_OUT_TEMP_NUM_SAMPLE_IN_10SECOND; i++)
        {
            s_avgChamberOutTemperature += (float)s_ChamberOutTemperature[i]/CHAMBER_OUT_TEMP_NUM_SAMPLE_IN_10SECOND;
        }
        //SYS_PRINT("\navgChamberOutTemperature: [%f]", s_avgChamberOutTemperature);
        if (s_alarmList[eChamberTemperatureAbnormalAlarmId].currentStatus == eInactive)
        {
            if (s_avgChamberOutTemperature >= 80)
            {
                s_alarmList[eChamberTemperatureAbnormalAlarmId].currentStatus = eActive;
                return;
            }
        }
    }
    
//    SYS_PRINT("\nCoil: [%f]     Out: [%f]", coilTemperatureSensorTemp, chamberTemperatureSensorTemp);
}

/** @brief Check too much water in the chamber
 *  @param [in] currentPower EVT temperature
 *  @param [in]  Chamber out temperature
 *  @param [out] : None
 *  @return None  
 */
static void alarmMgr_CheckChamberTooMuchWaterAlarmStatus(float currentIHPower,
                                                         float setTargetPower,
                                                         float piezoControlFreq,
                                                         float piezoControlFreqUpperLimit)// E125
{
    static int firstTick;
//    s_CurrentIHPowerHigher10PerCentTargetPowerCounter = TOO_MUCH_WATER_IN_THE_CHAMBER_TIMEOUT;
//    s_CurrentIHPowerLower10PerCentTargetPowerCounter = TOO_MUCH_WATER_IN_THE_CHAMBER_TIMEOUT;
//    SYS_PRINT("\nCounter: [%d]", s_CurrentIHPowerHigher10PerCentTargetPowerCounter);
    if (s_alarmList[eTooMuchWaterInTheChamberAlarmId].currentStatus == eInactive)
    {
        if (s_CurrentIHPowerHigher10PerCentTargetPowerCounter == 0 && s_CurrentIHPowerLower10PerCentTargetPowerCounter == 0)
        {
            firstTick = xTaskGetTickCount();
//            SYS_PRINT("\nFirst tick: [%d]", firstTick);
        }
        
        if (s_CurrentIHPowerHigher10PerCentTargetPowerCounter >= TOO_MUCH_WATER_IN_THE_CHAMBER_TIMEOUT || 
            s_CurrentIHPowerLower10PerCentTargetPowerCounter >= TOO_MUCH_WATER_IN_THE_CHAMBER_TIMEOUT)
        {
            SYS_PRINT("\nActive Tick: [%d]", xTaskGetTickCount() - firstTick);
            s_alarmList[eTooMuchWaterInTheChamberAlarmId].currentStatus = eActive;
        }
        
        if ((currentIHPower > 1.1 * setTargetPower) && (piezoControlFreq == 0))
        {
            s_CurrentIHPowerHigher10PerCentTargetPowerCounter++;
        }
        else
        {
            s_CurrentIHPowerHigher10PerCentTargetPowerCounter = 0;
        }
        
        if ((currentIHPower < 0.9 * setTargetPower) && (piezoControlFreq <= piezoControlFreqUpperLimit))
        {
            s_CurrentIHPowerLower10PerCentTargetPowerCounter++;
        }
        else
        {
            s_CurrentIHPowerLower10PerCentTargetPowerCounter = 0;
        }
    }
}


static void alarmMgr_CheckAlarmE001(void)
{
    alarmMgr_CheckBreathingCircuitNotConnectedAlarmStatus(s_currentAlarmMonitor.BreathingCircuitType, 
                                              s_currentAlarmMonitor.isChamberConnected);//E001
}

static void alarmMgr_CheckAlarmE002(void)
{
    
}

static void alarmMgr_CheckAlarmE003(void)
{
    alarmMgr_CheckForBlockageAlarmStatus( s_currentAlarmMonitor.setFlow,
                                          s_currentAlarmMonitor.currentFlow,  
                                          s_currentAlarmMonitor.currentBlowerRotationSpeed,
                                          s_currentAlarmMonitor.machineMode,
                                          s_currentAlarmMonitor.BreathingCircuitType,
                                          s_currentAlarmMonitor.isChamberConnected,
                                          s_currentAlarmMonitor.blowerControlValue,
                                          s_currentAlarmMonitor.isACConnected);//E003
}

static void alarmMgr_CheckAlarmE004(void)
{
    alarmMgr_CheckNonGenuineCircuitAlarmStatus(s_currentAlarmMonitor.breathingCode,
                                               s_currentAlarmMonitor.isChamberConnected);//E004
}

static void alarmMgr_CheckAlarmE005(void)
{
    alarmMgr_CheckBreathingCircuitChangeAlarmStatus(s_currentAlarmMonitor.BreathingCircuitType,
                                                    s_currentAlarmMonitor.isChamberConnected,
                                                    s_currentAlarmMonitor.machineMode,
                                                    s_currentAlarmMonitor.self_check);// E005
}

static void alarmMgr_CheckAlarmE006(void)
{
    alarmMgr_CheckExpiredCircuitAlarmStatus(s_currentAlarmMonitor.dateManufactureBreathingCircuit,
                                            s_currentAlarmMonitor.dateCurrentBreathingCircuit,
                                            s_currentAlarmMonitor.BreathingCircuitType);//E006
}

static void alarmMgr_CheckAlarmE007(void)
{
    alarmMgr_CheckConnectionOfChamberAlarmStatus(s_currentAlarmMonitor.isChamberConnected);//
}

static void alarmMgr_CheckAlarmE008(void)
{
    alarmMgr_CheckNoMoreWaterChamberAlarmStatus(s_currentAlarmMonitor.currentIHPower,
                                                s_currentAlarmMonitor.setTargetPower,
                                                s_currentAlarmMonitor.machineMode,
                                                s_currentAlarmMonitor.isIHOperating,
                                                s_currentAlarmMonitor.setFlow,
                                                s_currentAlarmMonitor.setTemperature,
                                                s_currentAlarmMonitor.warmingUpStatus); // E008
}

static void alarmMgr_CheckAlarmE009(void)
{
    //alarmMgr_CheckRunOutOfWaterAlarmStatus( s_currentAlarmMonitor.currentWaterSensor);//E009 This alarm is disable in specs
}

static void alarmMgr_CheckAlarmE010(void)
{
    alarmMgr_CheckLowTemperatureAlarmStatus(s_currentAlarmMonitor.currentMouthTemperature,
                                            s_currentAlarmMonitor.setTemperature,
                                            s_currentAlarmMonitor.setFlow);// E010
}

static void alarmMgr_CheckAlarmE011(void)
{
    alarmMgr_CheckHighTemperatureAlarmStatus(s_currentAlarmMonitor.currentMouthTemperature,
                                                                 s_currentAlarmMonitor.setTemperature,
                                                                 s_currentAlarmMonitor.currenAmbientTemperature,
                                                                 s_currentAlarmMonitor.setFlow,
                                                                 s_currentAlarmMonitor.machineMode);//E011
}

static void alarmMgr_CheckAlarmE012(void)
{
    alarmMgr_CheckHighTemperatureAbnormalityAlarmStatus(s_currentAlarmMonitor.currentMouthTemperature,
                                                        s_currentAlarmMonitor.currenAmbientTemperature,
                                                        s_currentAlarmMonitor.setFlow,
                                                        s_currentAlarmMonitor.setTemperature,
                                                        s_currentAlarmMonitor.machineMode); // 
}

static void alarmMgr_CheckAlarmE013(void)
{
    alarmMgr_CheckRoomTempTooLowToAchieveTargetTempAlarmStatus( s_currentAlarmMonitor.currentMouthTemperature, 
                                                                s_currentAlarmMonitor.setMouthTemperature, 
                                                                s_currentAlarmMonitor.currenAmbientTemperature,
                                                                s_currentAlarmMonitor.machineMode);//   
}

static void alarmMgr_CheckAlarmE014(void)
{
    alarmMgr_CheckOperationgConditionsAlarmStatus(s_currentAlarmMonitor.setTemperature,
                          s_currentAlarmMonitor.currenAmbientTemperature,
                          s_currentAlarmMonitor.machineMode);// 
}
static void alarmMgr_CheckAlarmE015(void)
{
    alarmMgr_CheckTargetTemperatureCannotBeAchievedAlarmStatus(s_currentAlarmMonitor.currentMouthTemperature, 
                                                               s_currentAlarmMonitor.setMouthTemperature, 
                                                               s_currentAlarmMonitor.currenAmbientTemperature);//
}

static void alarmMgr_CheckAlarmE016(void)
{
    alarmMgr_CheckeOxygenConcentrationHigherAlarmStatus( s_currentAlarmMonitor.o2Info_struct);// 016
}

static void alarmMgr_CheckAlarmE017(void)
{
    alarmMgr_CheckeOxygenConcentrationLowerAlarmStatus( s_currentAlarmMonitor.o2Info_struct);// E017
}

static void alarmMgr_CheckAlarmE018AndE019(void)
{
    alarmMgr_CheckDevicePostureAlarmStatus(s_currentAlarmMonitor.xAngleDirection,
                                           s_currentAlarmMonitor.yAngleDirection);//E018-E019
}

static void alarmMgr_CheckAlarmE019(void)
{
    
}

static void alarmMgr_CheckAlarmE020(void)
{
    alarmMgr_CheckSwitchBatteryModeAlarmStatus( s_currentAlarmMonitor.isACConnected,
                                                s_currentAlarmMonitor.batteryRemainingTimeInMin);//E020
}

static void alarmMgr_CheckAlarmE021(void)
{
    alarmMgr_CheckBatteryLowAlarmStatus(s_currentAlarmMonitor.isACConnected,
                                        s_currentAlarmMonitor.batteryRemainingTimeInMin);//E021
}

static void alarmMgr_CheckAlarmE022(void)
{
    alarmMgr_CheckBatteryGetRunOutAlarmStatus(s_currentAlarmMonitor.isACConnected,
                                        s_currentAlarmMonitor.batteryRemainingTimeInMin);//E022
}

static void alarmMgr_CheckAlarmE023(void)
{
    alarmMgr_CheckStopFunctionAlarmStatus( s_currentAlarmMonitor.isACConnected,
                            s_currentAlarmMonitor.batteryRemainingTimeInMin);//E023
}

static void alarmMgr_CheckAlarmE024(void)
{
    alarmMgr_CheckPowerNotEnoughAlarmStatus(s_currentAlarmMonitor.batteryRemainingTimeInMin,
                                            s_currentAlarmMonitor.isACConnected,
                                            s_currentAlarmMonitor.self_check);// E024
}

static void alarmMgr_CheckAlarmE025(void)
{
    alarmMgr_CheckSpo2FailAlarmStatus( s_currentAlarmMonitor.spo2Info_struct);//E025
}

static void alarmMgr_CheckAlarmE026(void)
{
    alarmMgr_CheckSpo2LowAlarmStatus(s_currentAlarmMonitor.spo2Info_struct);//E026
}

static void alarmMgr_CheckAlarmE027(void)
{
    //alarmMgr_CheckSpo2SensorProbeAlarmStatus( s_currentAlarmMonitor.spo2Info_struct);//E027 This alarm is disable in specs
}

static void alarmMgr_CheckAlarmE028(void)
{
    alarmMgr_CheckSpo2SignalLossAlarmStatus(s_currentAlarmMonitor.spo2Info_struct);//E028
}

static void alarmMgr_CheckAlarmE029(void)
{
    //alarmMgr_CheckSpo2SensorUnpluggedAlarmStatus(s_currentAlarmMonitor.spo2Info_struct);//E029 This alarm is disable in specs
}

static void alarmMgr_CheckAlarmE030(void)
{
    alarmMgr_CheckWaterSupplyAlarmStatus(s_currentAlarmMonitor.waterTankStatus,
                                         s_currentAlarmMonitor.isCradleConnected);//E030
}

static void alarmMgr_CheckAlarmE031(void)
{
    alarmMgr_CheckWaterTankConnectionAlarmStatus(s_currentAlarmMonitor.waterTankStatus);// E031
}

static void alarmMgr_CheckAlarmE032(void)
{
    alarmMgr_CheckAirFlowAbnormalAlarmStatus(s_currentAlarmMonitor.currentFlow,
                                             s_currentAlarmMonitor.setFlow,
                                             s_currentAlarmMonitor.machineMode,
                                             s_currentAlarmMonitor.blowerControlValue); // E032
}

static void alarmMgr_CheckAlarmE033(void)
{
    alarmMgr_CheckMainBatDisconnectedAlarmStatus(s_currentAlarmMonitor.isMainUnitBatteryConnected,
                                                 s_currentAlarmMonitor.isCradleConnected,
                                                 s_currentAlarmMonitor.self_check);// E033
}

static void alarmMgr_CheckAlarmE034(void)
{
    //alarmMgr_CheckCradleBatDisconnectedAlarmStatus(s_currentAlarmMonitor.isCradleConnected,
    //                                                         s_currentAlarmMonitor.isCradleBatteryConnected);// E034 This alarm is disable in specs
}

static void alarmMgr_CheckAlarmE100(void)
{
    alarmMgr_CheckFailureOxygenFlowMeasurementFlowSensorAlarmStatus( s_currentAlarmMonitor.O2FlowSensorErr); // E100
}

static void alarmMgr_CheckAlarmE101(void)
{
    alarmMgr_CheckFailureAirFlowMeasurementFlowSensorAlarmStatus( s_currentAlarmMonitor.AirFlowSensorErr); // E101
}

static void alarmMgr_CheckAlarmE102(void)
{
    alarmMgr_CheckFailureChamberTemperatureSensorAlarmStatus( s_currentAlarmMonitor.chamberTemperatureSensorTemp,
                                                             s_currentAlarmMonitor.isChamberConnected);//E102
}

static void alarmMgr_CheckAlarmE103(void)
{
    alarmMgr_CheckFailureCoilTemperatureSensorAlarmStatus( s_currentAlarmMonitor.coilTemperatureSensorTemp,
                                                           s_currentAlarmMonitor.isChamberConnected );// E103
}

static void alarmMgr_CheckAlarmE104(void)
{
    alarmMgr_CheckFailureBreathingCircuitOutTemperatureSensorAlarmStatus( s_currentAlarmMonitor.breathingCircuitTemperatureSensorTemp,
                                                                         s_currentAlarmMonitor.isChamberConnected,
                                                                         s_currentAlarmMonitor.BreathingCircuitType);//E104
}

static void alarmMgr_CheckAlarmE105(void)
{
    alarmMgr_CheckFailureEnvironmentSensorAlarmStatus( s_currentAlarmMonitor.BME280SensorErr); // E105
}

static void alarmMgr_CheckAlarmE106(void)
{
    alarmMgr_CheckFailureCurrentSensorAlarmStatus(s_currentAlarmMonitor.currentSensor1Volt, s_currentAlarmMonitor.currentSensor2Volt); // E106
}

static void alarmMgr_CheckAlarmE107(void)
{
    alarmMgr_CheckFailureBlowerAlarmStatus(s_currentAlarmMonitor.currentBlowerRotationSpeed, s_currentAlarmMonitor.blowerControlValue);// E107
}

static void alarmMgr_CheckAlarmE108(void)
{
    alarmMgr_CheckSpeakerBrokenOrDisconnectedStatus(s_currentAlarmMonitor.isSpeakerBrokenOrDisconnected);//E108
}

static void alarmMgr_CheckAlarmE109(void)
{
    alarmMgr_CheckFailureWaterLevelSensorAlarmStatus(s_currentAlarmMonitor.isFailureWaterLevelSensor);// E109
}

static void alarmMgr_CheckAlarmE110(void)
{
    alarmMgr_CheckFailureExternalFlashMemoryAlarmStatus(s_currentAlarmMonitor.isFailureExternalFlashMemory); // E110
}

static void alarmMgr_CheckAlarmE111(void)
{
    alarmMgr_CheckFailureAccelerationSensorAlarmStatus(s_currentAlarmMonitor.isFailureAccelerationSensor); // E111
}

static void alarmMgr_CheckAlarmE112(void)
{
    //alarmMgr_CheckFailureLightSensorAlarmIdAlarmStatus(s_currentAlarmMonitor.isFailureLightSensorAlarmId);// E112 This alarm is disable in specs
}

static void alarmMgr_CheckAlarmE113(void)
{
    alarmMgr_CheckFailureMainUnitBatteryCommunicationErrorAlarmIdAlarmStatus(s_currentAlarmMonitor.isFailureMainUnitBatteryCommunication);//E113
}

static void alarmMgr_CheckAlarmE114(void)
{
    alarmMgr_CheckFailureCradleBatteryCommunicationErrorAlarmIdAlarmStatus(s_currentAlarmMonitor.isFailureCradleBatteryCommunication,
                                                                           s_currentAlarmMonitor.isCradleConnected,
                                                                           s_currentAlarmMonitor.isCradleBatteryConnected,
                                                                           s_currentAlarmMonitor.isACConnected); // E114
}

static void alarmMgr_CheckAlarmE115(void)
{
    alarmMgr_CheckFailureCradleCommunicationErrorAlarmIdAlarmStatus(s_currentAlarmMonitor.isFailureCradleCommunicationError,
                                                                    s_currentAlarmMonitor.isCradleConnected); // E115
}

static void alarmMgr_CheckAlarmE116(void)
{
    alarmMgr_CheckFailureMainRunOutofControlAlarmStatus(s_currentAlarmMonitor.isFailureMainRunOutofControl); // E116
}

static void alarmMgr_CheckAlarmE117(void)
{
    alarmMgr_CheckESP32FailedAlarmStatus(s_currentAlarmMonitor.isESP32Failed); // 
}

static void alarmMgr_CheckAlarmE118(void)
{
    alarmMgr_CheckBreathingCircuitHeaterWireBrokenAlarmStatus(s_currentAlarmMonitor.breathingCircuitTemperatureSensorTemp,
                                                              s_currentAlarmMonitor.BreathingCircuitType,
                                                              s_currentAlarmMonitor.breathingCircuitPowerControl,
                                                              s_currentAlarmMonitor.warmingUpStatus,
                                                              s_currentAlarmMonitor.machineMode); // E118
}
static void alarmMgr_CheckAlarmE119(void)
{
    alarmMgr_CheckFailureSpo2ModuleAlarmStatus(s_currentAlarmMonitor.isSpo2ModuleFailure, s_currentAlarmMonitor.spo2Info_struct); // E119
}

static void alarmMgr_CheckAlarmE120(void)
{
    alarmMgr_CheckFailureRTCModuleAlarmStatus(s_currentAlarmMonitor.isRTCModuleFailure); // E120  
}

static void alarmMgr_CheckAlarmE121(void)
{
    alarmMgr_CheckFailureLCDTouchModuleAlarmStatus(s_currentAlarmMonitor.isLCDTouchModuleFailure); // E121
}

static void alarmMgr_CheckAlarmE122(void)
{
    
}

static void alarmMgr_CheckAlarmE123(void)
{
    
}

static void alarmMgr_CheckAlarmE124(void)
{
    alarmMgr_CheckChamberTemperatureAbnormalAlarmStatus(s_currentAlarmMonitor.coilTemperatureSensorTemp,
                                                        s_currentAlarmMonitor.chamberTemperatureSensorTemp); // E124
}

static void alarmMgr_CheckAlarmE125(void)
{
     alarmMgr_CheckChamberTooMuchWaterAlarmStatus(s_currentAlarmMonitor.currentIHPower,
                                                 s_currentAlarmMonitor.setTargetPower,
                                                 s_currentAlarmMonitor.piezoControlFreq,
                                                 s_currentAlarmMonitor.piezoControlFreqUpperLimit);// E125    
}
/** @brief Create and initialize default value for all alarm
 *  @param [in] None
 *  @param [out] : None
 *  @return None
 */
void alarmMgr_InitAlarm(void)
{
    int i;
    for (i = eFirsAlarmId; i < eLastAlarmId; i++)
    {
        s_alarmList[i].ID = (E_AlarmId) i;
        s_alarmList[i].previousStatus = eInactive;
        s_alarmList[i].currentStatus = eInactive;
        s_alarmList[i].data[eAdditionalMessageDataIdx] = 0;
        s_alarmList[i].data[eMessageTypeDataIdx] = 0;
        s_alarmList[i].data[2] = 0;
        s_alarmList[i].data[3] = 0;
        s_alarmList[i].data[4] = 0;
#ifdef FUNCTION_DISABLE_ALARM
        s_alarmList[i].mode = (E_AlarmOperationMode)setting_Get(i+eFirsAlarmOperationModeSettingId);
        //FIXME cannot get setting at here due to setting have not been initialized yet
#endif
    }
     s_alarmList[eI2C1ErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eI2C1ErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eI2C1ErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eI2C2ErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eI2C2ErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eI2C2ErrorAlarm].checkAlarmFnc = NULL;
        
    s_alarmList[eI2C3ErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eI2C3ErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eI2C3ErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eI2C4ErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eI2C4ErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eI2C4ErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eSPI3ErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eSPI3ErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eSPI3ErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eUart1ErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eUart1ErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eUart1ErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eUart2ErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eUart2ErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eUart2ErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eUart4ErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eUart4ErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eUart4ErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eUart6ErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eUart6ErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eUart6ErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eAirFlowSensorErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eAirFlowSensorErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eAirFlowSensorErrorAlarm].checkAlarmFnc = NULL;
    
    
    s_alarmList[eO2FlowSensorErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eO2FlowSensorErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eO2FlowSensorErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eBME280ErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eBME280ErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eBME280ErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eADXL345ErrorAlarmID].previousPriority = eHighPriority;
    s_alarmList[eADXL345ErrorAlarmID].currentPriority = eHighPriority;
    s_alarmList[eADXL345ErrorAlarmID].checkAlarmFnc = NULL;
    
    s_alarmList[eDRV8308ErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eDRV8308ErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eDRV8308ErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eAudioErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eAudioErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eAudioErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eMotorTaskErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eMotorTaskErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eMotorTaskErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eHeaterTaskErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eHeaterTaskErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eHeaterTaskErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eHumidityTaskErrorAlarm].previousPriority = eHighPriority;
    s_alarmList[eHumidityTaskErrorAlarm].currentPriority = eHighPriority;
    s_alarmList[eHumidityTaskErrorAlarm].checkAlarmFnc = NULL;
    
    s_alarmList[eBreathingCircuitNotConnectedAlarmId].previousPriority = eMediumPriority;// E001
    s_alarmList[eBreathingCircuitNotConnectedAlarmId].currentPriority = eMediumPriority;// E001
    s_alarmList[eBreathingCircuitNotConnectedAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE001;
    
    s_alarmList[eCheckLeakAlarmId].previousPriority = eMediumPriority;//E002
    s_alarmList[eCheckLeakAlarmId].currentPriority = eMediumPriority;//E002
    s_alarmList[eCheckLeakAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE002;
    
    s_alarmList[eCheckBlockageAlarmId].previousPriority = eMediumPriority;//E003
    s_alarmList[eCheckBlockageAlarmId].currentPriority = eMediumPriority;//E003
    s_alarmList[eCheckBlockageAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE003;
    
    s_alarmList[eNonGenuineCircuitAlarmId].previousPriority = eLow1Priority;//E004
    s_alarmList[eNonGenuineCircuitAlarmId].currentPriority = eLow1Priority;//E004
    s_alarmList[eNonGenuineCircuitAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE004;

    s_alarmList[eBreathingCircuitChangedAlarmId].previousPriority = eLow1Priority;//E005
    s_alarmList[eBreathingCircuitChangedAlarmId].currentPriority = eLow1Priority;//E005
    s_alarmList[eBreathingCircuitChangedAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE005;
    
    s_alarmList[eExpiredCircuitAlarmId].previousPriority = eLow1Priority;//E006
    s_alarmList[eExpiredCircuitAlarmId].currentPriority = eLow1Priority;//E006
    s_alarmList[eExpiredCircuitAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE006;
    
    s_alarmList[eCheckConnectionChamberAlarmId].previousPriority = eMediumPriority;//E007
    s_alarmList[eCheckConnectionChamberAlarmId].currentPriority = eMediumPriority;//E007
    s_alarmList[eCheckConnectionChamberAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE007;
    
    s_alarmList[eNoMoreWaterInChamberAlarmId].previousPriority = eLow1Priority;//E008
    s_alarmList[eNoMoreWaterInChamberAlarmId].currentPriority = eLow1Priority;//E008
    s_alarmList[eNoMoreWaterInChamberAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE008;
    
    s_alarmList[eRunOutOfWaterAlarmId].previousPriority = eLow1Priority;//E009
    s_alarmList[eRunOutOfWaterAlarmId].currentPriority = eLow1Priority;//E009
    s_alarmList[eRunOutOfWaterAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE009;
    
    s_alarmList[eLowTemperatureAlarmId].previousPriority = eLow1Priority;//E010
    s_alarmList[eLowTemperatureAlarmId].currentPriority = eLow1Priority;//E010
    s_alarmList[eLowTemperatureAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE010;
    
    s_alarmList[eHighTemperatureAlarmId].previousPriority = eLow1Priority;//E011
    s_alarmList[eHighTemperatureAlarmId].currentPriority = eLow1Priority;//E011
    s_alarmList[eHighTemperatureAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE011;

    s_alarmList[eHighTemperatureAbnormalityAlarmId].previousPriority = eMediumPriority;//E012
    s_alarmList[eHighTemperatureAbnormalityAlarmId].currentPriority = eMediumPriority;//E012
    s_alarmList[eHighTemperatureAbnormalityAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE012;
    
    s_alarmList[eRoomTemperatureLowToAchieveTargetTemperatureAlarmId].previousPriority = eLow1Priority;//E013
    s_alarmList[eRoomTemperatureLowToAchieveTargetTemperatureAlarmId].currentPriority = eLow1Priority;//E013
    s_alarmList[eRoomTemperatureLowToAchieveTargetTemperatureAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE013;
    
//    s_alarmList[eRoomTemperatureHighToAchieveTargetTemperatureAlarmId].currentPriority = eLow1Priority;//E014
    s_alarmList[eCheckOperatingConditionsAlarmID].previousPriority = eLow1Priority; // E014
    s_alarmList[eCheckOperatingConditionsAlarmID].currentPriority = eLow1Priority; // E014
    s_alarmList[eCheckOperatingConditionsAlarmID].checkAlarmFnc = alarmMgr_CheckAlarmE014;
    
    s_alarmList[eDeviceErrorToAchieveTargetTemperatureAlarmId].previousPriority = eLow1Priority;//E015
    s_alarmList[eDeviceErrorToAchieveTargetTemperatureAlarmId].currentPriority = eLow1Priority;//E015
    s_alarmList[eDeviceErrorToAchieveTargetTemperatureAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE015;
    
    s_alarmList[eOxygenHighAlarmId].previousPriority = eMediumPriority;// E016
    s_alarmList[eOxygenHighAlarmId].currentPriority = eMediumPriority;// E016
    s_alarmList[eOxygenHighAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE016;
    
    s_alarmList[eOxygenLowAlarmId].previousPriority = eHighPriority;//E017
    s_alarmList[eOxygenLowAlarmId].currentPriority = eHighPriority;//E017
    s_alarmList[eOxygenLowAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE017;
    
    s_alarmList[eDevicePostureAbnormalAlarmId].previousPriority = eLow1Priority;//E018
    s_alarmList[eDevicePostureAbnormalAlarmId].currentPriority = eLow1Priority;//E018
    s_alarmList[eDevicePostureAbnormalAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE018AndE019;
    
    s_alarmList[eDevicePostureBadAlarmId].previousPriority = eLow1Priority;//E019
    s_alarmList[eDevicePostureBadAlarmId].currentPriority = eLow1Priority;//E019
    s_alarmList[eDevicePostureBadAlarmId].checkAlarmFnc = NULL;
    
    s_alarmList[eSwitchToBatteryModeAlarmId].previousPriority = eLow1Priority;//E020
    s_alarmList[eSwitchToBatteryModeAlarmId].currentPriority = eLow1Priority;//E020
    s_alarmList[eSwitchToBatteryModeAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE020;
    
    s_alarmList[eBatteryLowAlarmId].previousPriority = eLow1Priority;//E021
    s_alarmList[eBatteryLowAlarmId].currentPriority = eLow1Priority;//E021
    s_alarmList[eBatteryLowAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE021;
    
    s_alarmList[eBatteryGetsRunOutAlarmId].previousPriority = eMediumPriority;//E022
    s_alarmList[eBatteryGetsRunOutAlarmId].currentPriority = eMediumPriority;//E022
    s_alarmList[eBatteryGetsRunOutAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE022;
    
    s_alarmList[eStopFunctionAlarmId].previousPriority = eMediumPriority;//E023
    s_alarmList[eStopFunctionAlarmId].currentPriority = eMediumPriority;//E023
    s_alarmList[eStopFunctionAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE023;
    
    s_alarmList[ePowerNotEnoughAlarmId].previousPriority = eLow1Priority;//E024
    s_alarmList[ePowerNotEnoughAlarmId].currentPriority = eLow1Priority;//E024
    s_alarmList[ePowerNotEnoughAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE024;
    
    s_alarmList[eSpO2FailedAlarmId].previousPriority = eLow1Priority;//E025
    s_alarmList[eSpO2FailedAlarmId].currentPriority = eLow1Priority;//E025
    s_alarmList[eSpO2FailedAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE025;
    
    s_alarmList[eSpO2LowAlarmId].previousPriority = eMediumPriority; //E026
    s_alarmList[eSpO2LowAlarmId].currentPriority = eMediumPriority; //E026
    s_alarmList[eSpO2LowAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE026;
    
    s_alarmList[eSpO2SensorProbeAlarmId].previousPriority = eLow1Priority;//E027
    s_alarmList[eSpO2SensorProbeAlarmId].currentPriority = eLow1Priority;//E027
    s_alarmList[eSpO2SensorProbeAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE027;
    
    s_alarmList[eSpO2SignalLossAlarmId].previousPriority = eLow1Priority; //E028
    s_alarmList[eSpO2SignalLossAlarmId].currentPriority = eLow1Priority; //E028
    s_alarmList[eSpO2SignalLossAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE028;
    
    s_alarmList[eSpO2SensorUnpluggedAlarmId].previousPriority = eLow1Priority;//E029
    s_alarmList[eSpO2SensorUnpluggedAlarmId].currentPriority = eLow1Priority;//E029
    s_alarmList[eSpO2SensorUnpluggedAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE029;
    
    s_alarmList[eWaterSupplyCheckAlarmId].previousPriority = eLow1Priority;//E030
    s_alarmList[eWaterSupplyCheckAlarmId].currentPriority = eLow1Priority;//E030
    s_alarmList[eWaterSupplyCheckAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE030;
    
    s_alarmList[eWaterTankConnectionCheckAlarmId].previousPriority = eLow1Priority;//E031
    s_alarmList[eWaterTankConnectionCheckAlarmId].currentPriority = eLow1Priority;//E031
    s_alarmList[eWaterTankConnectionCheckAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE031;
    
    s_alarmList[eAirFlowAbnormalAlarmId].previousPriority = eLow1Priority;//E032
    s_alarmList[eAirFlowAbnormalAlarmId].currentPriority = eLow1Priority;//E032
    s_alarmList[eAirFlowAbnormalAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE032;
    
    s_alarmList[eMainUnitBatteryDisconnectedAlarmId].previousPriority = eLow1Priority;//E033
    s_alarmList[eMainUnitBatteryDisconnectedAlarmId].currentPriority = eLow1Priority;//E033
    s_alarmList[eMainUnitBatteryDisconnectedAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE033;
    
    s_alarmList[eCradleBatteryDisconnectedAlarmId].previousPriority = eMediumPriority;//TBD//E034
    s_alarmList[eCradleBatteryDisconnectedAlarmId].currentPriority = eMediumPriority;//TBD////E034
    s_alarmList[eCradleBatteryDisconnectedAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE034;
    
    
    s_alarmList[eFailureOxygenFlowMeasurementFlowSensorAlarmId].previousPriority = eMediumPriority;//E100
    s_alarmList[eFailureOxygenFlowMeasurementFlowSensorAlarmId].currentPriority = eMediumPriority;//E100
    s_alarmList[eFailureOxygenFlowMeasurementFlowSensorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE100;
    
    s_alarmList[eFailureAirFlowMeasurementFlowSensorAlarmId].previousPriority = eMediumPriority; //E101
    s_alarmList[eFailureAirFlowMeasurementFlowSensorAlarmId].currentPriority = eMediumPriority; //E101
    s_alarmList[eFailureAirFlowMeasurementFlowSensorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE101;
    
    s_alarmList[eFailureChamberTemperatureSensorAlarmId].previousPriority = eMediumPriority;//E102
    s_alarmList[eFailureChamberTemperatureSensorAlarmId].currentPriority = eMediumPriority;//E102
    s_alarmList[eFailureChamberTemperatureSensorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE102;
    
    s_alarmList[eFailureCoilTemperatureSendorAlarmId].previousPriority = eMediumPriority; //E103
    s_alarmList[eFailureCoilTemperatureSendorAlarmId].currentPriority = eMediumPriority; //E103
    s_alarmList[eFailureCoilTemperatureSendorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE103;
    
    s_alarmList[eFailureBreathingCircuitOutTemperatureSensorAlarmId].previousPriority = eMediumPriority; // E104
    s_alarmList[eFailureBreathingCircuitOutTemperatureSensorAlarmId].currentPriority = eMediumPriority; // E104
    s_alarmList[eFailureBreathingCircuitOutTemperatureSensorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE104;
    
    s_alarmList[eFailureEnvironmentSensorAlarmId].previousPriority = eMediumPriority; // E105
    s_alarmList[eFailureEnvironmentSensorAlarmId].currentPriority = eMediumPriority; // E105
    s_alarmList[eFailureEnvironmentSensorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE105;
    
    s_alarmList[eFailureCurrentSensorAlarmId].previousPriority = eMediumPriority; //E106
    s_alarmList[eFailureCurrentSensorAlarmId].currentPriority = eMediumPriority; //E106
    s_alarmList[eFailureCurrentSensorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE106;
    
    s_alarmList[eFailureBlowerAlarmId].previousPriority = eMediumPriority;// E107
    s_alarmList[eFailureBlowerAlarmId].currentPriority = eMediumPriority;// E107
    s_alarmList[eFailureBlowerAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE107;
    
    s_alarmList[eSpeakerDisconnectedAlarmId].previousPriority = eMediumPriority;// E108
    s_alarmList[eSpeakerDisconnectedAlarmId].currentPriority = eMediumPriority;// E108
    s_alarmList[eSpeakerDisconnectedAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE108;
                                                                    
    s_alarmList[eFailureWaterLevelSensorAlarmId].previousPriority = eMediumPriority;//E109 
    s_alarmList[eFailureWaterLevelSensorAlarmId].currentPriority = eMediumPriority;//E109 
    s_alarmList[eFailureWaterLevelSensorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE109;

    s_alarmList[eFailureExternalFlashMemoryAlarmId].previousPriority = eLow1Priority; // E110
    s_alarmList[eFailureExternalFlashMemoryAlarmId].currentPriority = eLow1Priority; // E110
    s_alarmList[eFailureExternalFlashMemoryAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE110;
    
    s_alarmList[eFailureAccelerationSensorAlarmId].previousPriority = eLow1Priority; // E111
    s_alarmList[eFailureAccelerationSensorAlarmId].currentPriority = eLow1Priority; // E111
    s_alarmList[eFailureAccelerationSensorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE111;
    
    s_alarmList[eFailureLightSensorAlarmId].previousPriority = eLow1Priority;// E112
    s_alarmList[eFailureLightSensorAlarmId].currentPriority = eLow1Priority;// E112
    s_alarmList[eFailureLightSensorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE112;
    
    s_alarmList[eMainUnitBatteryCommunicationErrorAlarmId].previousPriority = eMediumPriority;//TBD // E113
    s_alarmList[eMainUnitBatteryCommunicationErrorAlarmId].currentPriority = eMediumPriority;//TBD // E113
    s_alarmList[eMainUnitBatteryCommunicationErrorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE113;
    
    s_alarmList[eCradleBatteryCommunicationErrorAlarmId].previousPriority = eLow1Priority;//TBD // E114
    s_alarmList[eCradleBatteryCommunicationErrorAlarmId].currentPriority = eLow1Priority;//TBD // E114
    s_alarmList[eCradleBatteryCommunicationErrorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE114;
    
    s_alarmList[eCradleCommunicationErrorAlarmId].previousPriority = eLow1Priority;//TBD // E115
    s_alarmList[eCradleCommunicationErrorAlarmId].currentPriority = eLow1Priority;//TBD // E115
    s_alarmList[eCradleCommunicationErrorAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE115;
    
    s_alarmList[eMainMCUFailedOrOutOfControlAlarmId].previousPriority = eMediumPriority; // E116
    s_alarmList[eMainMCUFailedOrOutOfControlAlarmId].currentPriority = eMediumPriority; // E116
    s_alarmList[eMainMCUFailedOrOutOfControlAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE116;
    
    s_alarmList[eESP32FailedAlarmId].previousPriority = eMediumPriority; // E117
    s_alarmList[eESP32FailedAlarmId].currentPriority = eMediumPriority; // E117
    s_alarmList[eESP32FailedAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE117;
    
    s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].previousPriority = eLow1Priority; // E118
    s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].currentPriority = eLow1Priority; // E118
    s_alarmList[eBreathingCircuitHeaterWireBrokenAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE118;
    
    s_alarmList[eFailureSpo2ModuleAlarmId].previousPriority = eLow1Priority;//E119
    s_alarmList[eFailureSpo2ModuleAlarmId].currentPriority = eLow1Priority;//E119
    s_alarmList[eFailureSpo2ModuleAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE119;
    
    s_alarmList[eFailureRTCModuleAlarmId].previousPriority = eLow1Priority;//E120
    s_alarmList[eFailureRTCModuleAlarmId].currentPriority = eLow1Priority;//E120
    s_alarmList[eFailureRTCModuleAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE120;

    s_alarmList[eFailureLCDTouchModuleAlarmId].previousPriority = eLow1Priority;//E121
    s_alarmList[eFailureLCDTouchModuleAlarmId].currentPriority = eLow1Priority;//E121
    s_alarmList[eFailureLCDTouchModuleAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE121;
 
//    s_alarmList[eFailureMainUnitBatteryManagementICAlarmId].previousPriority = eLow1Priority;//E122
//    s_alarmList[eFailureMainUnitBatteryManagementICAlarmId].currentPriority = eLow1Priority;//E122
//    
//    s_alarmList[eFailureCradleBatteryManagementICAlarmId].previousPriority = eLow1Priority;//E123
//    s_alarmList[eFailureCradleBatteryManagementICAlarmId].currentPriority = eLow1Priority;//E123
    
    s_alarmList[eChamberTemperatureAbnormalAlarmId].previousPriority = eMediumPriority;//E124
    s_alarmList[eChamberTemperatureAbnormalAlarmId].currentPriority = eMediumPriority;//E124
    s_alarmList[eChamberTemperatureAbnormalAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE124;
    
    s_alarmList[eTooMuchWaterInTheChamberAlarmId].previousPriority = eLow1Priority;//E125
    s_alarmList[eTooMuchWaterInTheChamberAlarmId].currentPriority = eLow1Priority;//E125
    s_alarmList[eTooMuchWaterInTheChamberAlarmId].checkAlarmFnc = alarmMgr_CheckAlarmE125;
    
    return;
}
/** @brief Check status for all of alarm depend on operation mode
 *  @param [in] None
 *  @param [out] : None
 *  @return None
 */

void alarmMgr_CheckAllAlarm(void)
#define U_TEST
#undef U_TEST
#ifndef U_TEST
{
//    s_alarmList[eDeviceErrorToAchieveTargetTemperatureAlarmId].currentStatus = eActive;
    static uint16_t count = 0;
    if (count < 10)
    {
        count++;
    }
    else
    {
        s_alarmList[eBreathingCircuitNotConnectedAlarmId].currentStatus = eActive;
//        s_alarmList[eBreathingCircuitNotConnectedAlarmId].data[eAdditionalMessageDataIdx] = eEnableAdditionalMessage;
        s_alarmList[eBreathingCircuitNotConnectedAlarmId].data[eResetButtonStatusDataIdx] = eResetButtonShowReset;
    }
//    SYS_PRINT("\nCount: %d", count);
}
#else
{
//    s_alarmList[eFailureLCDTouchModuleAlarmId].currentStatus = eActive;
    if(s_currentAlarmMonitor.self_check == true)
    {
            
        /*alarmMgr_CheckPowerNotEnoughAlarmStatus(s_currentAlarmMonitor.batteryRemainingTimeInMin,
                                            s_currentAlarmMonitor.isACConnected,
                                            s_currentAlarmMonitor.self_check);// E024*/
        
        if((s_alarmList[ePowerNotEnoughAlarmId].mode == eAlarmEnable) 
            &&(s_alarmList[ePowerNotEnoughAlarmId].checkAlarmFnc != NULL))
            {
                s_alarmList[ePowerNotEnoughAlarmId].checkAlarmFnc();//E024
            }
    }
    else
    {           
       //alarmMgr_CheckRunOutOfWaterAlarmStatus( s_currentAlarmMonitor.currentWaterSensor);//E009 This alarm is disable in specs    

        /*alarmMgr_CheckStopFunctionAlarmStatus( s_currentAlarmMonitor.isACConnected,
                            s_currentAlarmMonitor.batteryRemainingTimeInMin);//E023*/
        
        if((s_alarmList[eRunOutOfWaterAlarmId].mode == eAlarmEnable) 
            &&(s_alarmList[eRunOutOfWaterAlarmId].checkAlarmFnc != NULL))
            {
                s_alarmList[eRunOutOfWaterAlarmId].checkAlarmFnc();//E009
            }
        
        if((s_alarmList[eStopFunctionAlarmId].mode == eAlarmEnable) 
            &&(s_alarmList[eStopFunctionAlarmId].checkAlarmFnc != NULL))
            {
                s_alarmList[eStopFunctionAlarmId].checkAlarmFnc();//E023
            }
        
    }
    int i;
    for(i == eBreathingCircuitNotConnectedAlarmId; i < eLastAlarmId; i++ )
    {
        if((i == ePowerNotEnoughAlarmId)
            ||(i == eRunOutOfWaterAlarmId)
                    ||(i == eStopFunctionAlarmId))
        {
            continue;
        }
        
        if((s_alarmList[i].mode == eAlarmEnable) 
        &&(s_alarmList[i].checkAlarmFnc != NULL))
        {
            s_alarmList[i].checkAlarmFnc();
        }
        
    }
    /*alarmMgr_CheckBreathingCircuitNotConnectedAlarmStatus(s_currentAlarmMonitor.BreathingCircuitType, 
                                              s_currentAlarmMonitor.isChamberConnected);//E001


//    alarmMgr_CheckStopFunctionAlarmStatus( s_currentAlarmMonitor.isACConnected,
//                            s_currentAlarmMonitor.batteryRemainingTimeInMin);//E023
//                                
////        alarmMgr_CheckSpo2SensorProbeAlarmStatus( s_currentAlarmMonitor.spo2Info_struct);//E027
//    
    }
//    
//    alarmMgr_CheckBreathingCircuitNotConnectedAlarmStatus(s_currentAlarmMonitor.BreathingCircuitType, 
//                                              s_currentAlarmMonitor.isChamberConnected);//E001
//
//    alarmMgr_CheckForBlockageAlarmStatus( s_currentAlarmMonitor.setFlow,
//                                          s_currentAlarmMonitor.currentFlow,  
//                                          s_currentAlarmMonitor.currentBlowerRotationSpeed,
//                                          s_currentAlarmMonitor.machineMode,
//                                          s_currentAlarmMonitor.BreathingCircuitType,
//                                          s_currentAlarmMonitor.isChamberConnected,
//                                          s_currentAlarmMonitor.blowerControlValue,
//                                          s_currentAlarmMonitor.isACConnected);//E003
//
//    alarmMgr_CheckNonGenuineCircuitAlarmStatus(s_currentAlarmMonitor.breathingCode,
//                                               s_currentAlarmMonitor.isChamberConnected);//E004
//
//    alarmMgr_CheckBreathingCircuitChangeAlarmStatus(s_currentAlarmMonitor.BreathingCircuitType,
//                                                    s_currentAlarmMonitor.isChamberConnected,
//                                                    s_currentAlarmMonitor.machineMode,
//                                                    s_currentAlarmMonitor.self_check);// E005
//
//    alarmMgr_CheckExpiredCircuitAlarmStatus(s_currentAlarmMonitor.dateManufactureBreathingCircuit,
//                                            s_currentAlarmMonitor.dateCurrentBreathingCircuit,
//                                            s_currentAlarmMonitor.BreathingCircuitType);//E006
//                        
//    alarmMgr_CheckConnectionOfChamberAlarmStatus(s_currentAlarmMonitor.isChamberConnected);//E007
//    alarmMgr_CheckNoMoreWaterChamberAlarmStatus(s_currentAlarmMonitor.currentIHPower,
//                                                s_currentAlarmMonitor.setTargetPower,
//                                                s_currentAlarmMonitor.machineMode,
//                                                s_currentAlarmMonitor.isIHOperating,
//                                                s_currentAlarmMonitor.setFlow,
//                                                s_currentAlarmMonitor.setTemperature,
//                                                s_currentAlarmMonitor.warmingUpStatus); // E008
//
//    alarmMgr_CheckLowTemperatureAlarmStatus(s_currentAlarmMonitor.currentMouthTemperature,
//                                            s_currentAlarmMonitor.setTemperature,
//                                            s_currentAlarmMonitor.setFlow);// E010
//            
//    alarmMgr_CheckHighTemperatureAlarmStatus(s_currentAlarmMonitor.currentMouthTemperature,
//                                                                 s_currentAlarmMonitor.setTemperature,
//                                                                 s_currentAlarmMonitor.currenAmbientTemperature,
//                                                                 s_currentAlarmMonitor.setFlow,
//                                                                 s_currentAlarmMonitor.machineMode);//E011
//
//    alarmMgr_CheckHighTemperatureAbnormalityAlarmStatus(s_currentAlarmMonitor.currentMouthTemperature,
//                                                        s_currentAlarmMonitor.currenAmbientTemperature,
//                                                        s_currentAlarmMonitor.setFlow,
//                                                        s_currentAlarmMonitor.setTemperature,
//                                                        s_currentAlarmMonitor.machineMode); // E012
//                        
//    alarmMgr_CheckRoomTempTooLowToAchieveTargetTempAlarmStatus( s_currentAlarmMonitor.currentMouthTemperature, 
//                                                                s_currentAlarmMonitor.setMouthTemperature, 
//                                                                s_currentAlarmMonitor.currenAmbientTemperature,
//                                                                s_currentAlarmMonitor.machineMode);//E013
//
//    alarmMgr_CheckOperationgConditionsAlarmStatus(s_currentAlarmMonitor.setTemperature,
//                          s_currentAlarmMonitor.currenAmbientTemperature,
//                          s_currentAlarmMonitor.machineMode);// E014
//
//    alarmMgr_CheckTargetTemperatureCannotBeAchievedAlarmStatus(s_currentAlarmMonitor.currentMouthTemperature, 
//                                                               s_currentAlarmMonitor.setMouthTemperature, 
//                                                               s_currentAlarmMonitor.currenAmbientTemperature);//E015
//    alarmMgr_CheckeOxygenConcentrationHigherAlarmStatus( s_currentAlarmMonitor.o2Info_struct);// E016
//    alarmMgr_CheckeOxygenConcentrationLowerAlarmStatus( s_currentAlarmMonitor.o2Info_struct);// E017
//    alarmMgr_CheckDevicePostureAlarmStatus(s_currentAlarmMonitor.xAngleDirection,
//                                           s_currentAlarmMonitor.yAngleDirection);//E018-E019
//    alarmMgr_CheckSwitchBatteryModeAlarmStatus( s_currentAlarmMonitor.isACConnected,
//                                                s_currentAlarmMonitor.batteryRemainingTimeInMin);//E020
//    alarmMgr_CheckBatteryLowAlarmStatus(s_currentAlarmMonitor.isACConnected,
//                                        s_currentAlarmMonitor.batteryRemainingTimeInMin);//E021
//    alarmMgr_CheckBatteryGetRunOutAlarmStatus(s_currentAlarmMonitor.isACConnected,
//                                        s_currentAlarmMonitor.batteryRemainingTimeInMin);//E022
//
//    alarmMgr_CheckSpo2FailAlarmStatus( s_currentAlarmMonitor.spo2Info_struct);//E025
//    alarmMgr_CheckSpo2LowAlarmStatus(s_currentAlarmMonitor.spo2Info_struct);//E026
//    alarmMgr_CheckSpo2SignalLossAlarmStatus(s_currentAlarmMonitor.spo2Info_struct);//E028
//    alarmMgr_CheckSpo2SensorUnpluggedAlarmStatus(s_currentAlarmMonitor.spo2Info_struct);//E029
//    alarmMgr_CheckWaterSupplyAlarmStatus(s_currentAlarmMonitor.waterTankStatus,
//                                         s_currentAlarmMonitor.isCradleConnected);//E030
//    alarmMgr_CheckWaterTankConnectionAlarmStatus(s_currentAlarmMonitor.waterTankStatus);// E031
//    alarmMgr_CheckAirFlowAbnormalAlarmStatus(s_currentAlarmMonitor.currentFlow,
//                                             s_currentAlarmMonitor.setFlow,
//                                             s_currentAlarmMonitor.machineMode,
//                                             s_currentAlarmMonitor.blowerControlValue); // E032
    alarmMgr_CheckMainBatDisconnectedAlarmStatus(s_currentAlarmMonitor.isMainUnitBatteryConnected,
                                                 s_currentAlarmMonitor.isCradleConnected,
                                                 s_currentAlarmMonitor.self_check);// E033
//    alarmMgr_CheckCradleBatDisconnectedAlarmStatus(s_currentAlarmMonitor.isCradleConnected,
//                                                             s_currentAlarmMonitor.isCradleBatteryConnected);// E034
//    alarmMgr_CheckFailureOxygenFlowMeasurementFlowSensorAlarmStatus( s_currentAlarmMonitor.O2FlowSensorErr); // E100
//    alarmMgr_CheckFailureAirFlowMeasurementFlowSensorAlarmStatus( s_currentAlarmMonitor.AirFlowSensorErr); // E101
//    alarmMgr_CheckFailureChamberTemperatureSensorAlarmStatus( s_currentAlarmMonitor.chamberTemperatureSensorTemp,
//                                                             s_currentAlarmMonitor.isChamberConnected);//E102
//    alarmMgr_CheckFailureCoilTemperatureSensorAlarmStatus( s_currentAlarmMonitor.coilTemperatureSensorTemp,
//                                                           s_currentAlarmMonitor.isChamberConnected );// E103
//    alarmMgr_CheckFailureBreathingCircuitOutTemperatureSensorAlarmStatus( s_currentAlarmMonitor.breathingCircuitTemperatureSensorTemp,
//                                                                         s_currentAlarmMonitor.isChamberConnected,
//                                                                         s_currentAlarmMonitor.BreathingCircuitType);//E104
//    alarmMgr_CheckFailureEnvironmentSensorAlarmStatus( s_currentAlarmMonitor.BME280SensorErr); // E105
//    alarmMgr_CheckFailureCurrentSensorAlarmStatus(s_currentAlarmMonitor.currentSensor1Volt, s_currentAlarmMonitor.currentSensor2Volt); // E106
//    alarmMgr_CheckFailureBlowerAlarmStatus(s_currentAlarmMonitor.currentBlowerRotationSpeed, s_currentAlarmMonitor.blowerControlValue);// E107
//    alarmMgr_CheckSpeakerBrokenOrDisconnectedStatus(s_currentAlarmMonitor.isSpeakerBrokenOrDisconnected);//E108
//    alarmMgr_CheckFailureWaterLevelSensorAlarmStatus(s_currentAlarmMonitor.isFailureWaterLevelSensor);// E109
//    alarmMgr_CheckFailureExternalFlashMemoryAlarmStatus(s_currentAlarmMonitor.isFailureExternalFlashMemory); // E110
//    alarmMgr_CheckFailureAccelerationSensorAlarmStatus(s_currentAlarmMonitor.isFailureAccelerationSensor); // E111
//    alarmMgr_CheckFailureLightSensorAlarmIdAlarmStatus(s_currentAlarmMonitor.isFailureLightSensorAlarmId);// E112
//    alarmMgr_CheckFailureMainUnitBatteryCommunicationErrorAlarmIdAlarmStatus(s_currentAlarmMonitor.isFailureMainUnitBatteryCommunication);//E113
//    alarmMgr_CheckFailureCradleBatteryCommunicationErrorAlarmIdAlarmStatus(s_currentAlarmMonitor.isFailureCradleBatteryCommunication,
//                                                                           s_currentAlarmMonitor.isCradleConnected,
//                                                                           s_currentAlarmMonitor.isCradleBatteryConnected,
//                                                                           s_currentAlarmMonitor.isACConnected); // E114
//    alarmMgr_CheckFailureCradleCommunicationErrorAlarmIdAlarmStatus(s_currentAlarmMonitor.isFailureCradleCommunicationError,
//                                                                    s_currentAlarmMonitor.isCradleConnected); // E115
//    alarmMgr_CheckFailureMainRunOutofControlAlarmStatus(s_currentAlarmMonitor.isFailureMainRunOutofControl); // E116
//    alarmMgr_CheckESP32FailedAlarmStatus(s_currentAlarmMonitor.isESP32Failed); // E117
//    alarmMgr_CheckBreathingCircuitHeaterWireBrokenAlarmStatus(s_currentAlarmMonitor.breathingCircuitTemperatureSensorTemp,
//                                                              s_currentAlarmMonitor.BreathingCircuitType,
//                                                              s_currentAlarmMonitor.breathingCircuitPowerControl,
//                                                              s_currentAlarmMonitor.warmingUpStatus,
//                                                              s_currentAlarmMonitor.machineMode); // E118
//    alarmMgr_CheckFailureSpo2ModuleAlarmStatus(s_currentAlarmMonitor.isSpo2ModuleFailure, s_currentAlarmMonitor.spo2Info_struct); // E119
//    alarmMgr_CheckFailureRTCModuleAlarmStatus(s_currentAlarmMonitor.isRTCModuleFailure); // E120
//    alarmMgr_CheckFailureLCDTouchModuleAlarmStatus(s_currentAlarmMonitor.isLCDTouchModuleFailure); // E121
//    alarmMgr_CheckChamberTemperatureAbnormalAlarmStatus(s_currentAlarmMonitor.coilTemperatureSensorTemp,
//                                                        s_currentAlarmMonitor.chamberTemperatureSensorTemp); // E124
//    alarmMgr_CheckChamberTooMuchWaterAlarmStatus(s_currentAlarmMonitor.currentIHPower,
//                                                 s_currentAlarmMonitor.setTargetPower,
//                                                 s_currentAlarmMonitor.piezoControlFreq,
//                                                 s_currentAlarmMonitor.piezoControlFreqUpperLimit);// E125
    */
    return;
}
#endif

/** @brief update status for all of alarm
 *  @param [in] None
 *  @param [out] : None
 *  @return None
 */
void alarmMgr_UpdateAllAlarm(void)
{
    int i;
    for (i = eFirsAlarmId; i < eLastAlarmId; i++)
    {
        if (s_alarmList[i].previousStatus != s_alarmList[i].currentStatus)
        {
            if (alarmInterface_SendEvent(s_alarmList[i].ID, s_alarmList[i].currentStatus, s_alarmList[i].currentPriority, s_alarmList[i].data))
            {
                s_alarmList[i].previousStatus = s_alarmList[i].currentStatus;
            }
            else
            {
                SYS_PRINT("\n\nCan not send event");
            }
            //SYS_PRINT("\nAlarm change: [%d] -- Alarm Test: [%d] -- current state: [%d]", i, eBreathingCircuitNotConnectedAlarmId, s_alarmList[i].currentStatus);
            SYS_PRINT("\nAlarm change: [%d] -- current state: [%d]\n", i, /*eBreathingCircuitNotConnectedAlarmId,*/ s_alarmList[i].currentStatus);
            //SYS_PRINT("\nDraft");
            return;
        }
        
        if (s_alarmList[i].previousPriority != s_alarmList[i].currentPriority)
        {
            if (alarmInterface_SendEvent(s_alarmList[i].ID, s_alarmList[i].currentStatus, s_alarmList[i].currentPriority, s_alarmList[i].data))
            {
                s_alarmList[i].previousPriority = s_alarmList[i].currentPriority;
                SYS_PRINT("\nPriority Change: [%d] -- [%d] -- [%d]\n\n\n",i, s_alarmList[i].previousPriority, eOxygenHighAlarmId);
            }
            else
            {
                SYS_PRINT("\n\nCan not send event");
            }
            return;
        }
    }
//    SYS_PRINT("\nE008 current is : [%d]", s_alarmList[eCheckConnectionChamberAlarmId].currentStatus);
    return;
}

/* *****************************************************************************
 End of File
 */