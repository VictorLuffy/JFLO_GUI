/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

#include "GuiOperation.h"

#ifndef UNIT_TEST
#include "system_config.h"
#include "system_definitions.h"
#endif

#include "PWM_LCDBacklight.h"
#include "GT911.h"

#include "GuiInterface.h"
#include "LogInterface.h"

#include "AlarmNotificationList.h"
#include "MainScreen.h"
#include "Audio.h"


/** @brief declare GUI queue size has 16 items */
#define 	GUI_QUEUE_SIZE			(16)

/** @brief Declare queue for Gui */
QueueHandle_t g_guiQueue;

/** @brief Declare flag show GUI */
static bool s_isShowGUI = false;


void GUI_Initialize(void)
{
    //
    mem_init();
    mm_init();  
    
    //
    GT911_Initialize();

    PWM_LCDBacklight_Initialize();

    //create GUI queue for GUI task communication with other task
    g_guiQueue = xQueueCreate(GUI_QUEUE_SIZE, sizeof (GUI_EVENT_t));


    DisplayControl_Initialize();
    return;
}

/** @brief This operation check Gui event from Gui queue and process them
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void guiTask_HandleEvent(void)
{
    GUI_EVENT_t guiEvent;
    //wait for queue event
    if (xQueueReceive(g_guiQueue, &guiEvent, 0) == pdTRUE) //wait 0 tick (do not wait)
    {
        switch (guiEvent.id)
        {
        case eGuiSQIisNotEmptyId:
        {
            if (s_isShowGUI == false)
            {
                s_isShowGUI = true;
                
                /* Initializing Log Manager     */
                file_OpenFileOnSQIFlash();
                logMgr_Initialize();
                
                // Write a log - Start Operation
                logInterface_WriteEventLog(0 ,NULL , eStartOperationEventLogId);
                LogInterface_WriteDebugLogFile("Start Operation \n");
                
                DeviceInformation_Init();
                
                setting_Init();
                // check flow, temp setting by power mode
                uint8_t tempSetting = setting_Get(eTempSettingId);
                uint8_t flowSetting = setting_Get(eFlowSettingId);
                E_TreatmentMode mode = setting_Get(eTreatmentModeSettingId);
                setting_UpdateSettingByTreatmentMode(mode, mode);
                setting_UpdateSettingToPowerMode(powerManagement_GetACConnectionState(), true);
                
                if (tempSetting != setting_Get(eTempSettingId) || flowSetting != setting_Get(eFlowSettingId))
                {
                    setting_Save();
                }
                uint8_t logData[2];
                if (tempSetting != setting_Get(eTempSettingId))
                {
                    logData[0] = tempSetting;
                    logData[1] = setting_Get(eTempSettingId);
                    logInterface_WriteEventLog(2,(void*)logData , eChangeTargetTemperatureSettingEventLogId);  
                }
                if (flowSetting != setting_Get(eFlowSettingId))
                {
                    logData[0] = flowSetting;
                    logData[1] = setting_Get(eFlowSettingId);
                    logInterface_WriteEventLog(2,(void*)logData , eChangeTargetFlowSettingEventLogId);  
                }               
                
                //Send flow setting
                MOTOR_CTRL_EVENT_t eventFlow = {.id = eMotorFlowId, .fData = (float) setting_Get(eFlowSettingId)};
                //send event to Flow Control Task to start motor
                MotorTask_SendEvent(eventFlow);

                //Send temperature setting
                //send event to Flow Control Task to start motor
                HEATER_CTRL_EVENT_t eventTemp = {.id = eHeaterTempId, .fData = (float) setting_Get(eTempSettingId)};
                HeaterTask_SendEvent(eventTemp);

            }
            break;
        }
        case eGuiSQIisEmptyId:
            if (s_isShowGUI == false)
            {
                s_isShowGUI = true;
                //set brightness max
                PWM_LCDBacklight_SetDutyCycle(100);          
                SYS_PRINT("\nSuspend device Task alarm Task\n");
                DeviceTask_Suspend();
                alarmTask_Suspend();
                DisplayControl_SetState(eUpdateScreenIsShowingDispState);
                uint8_t id = guiEvent.eventData.data;
                SYS_PRINT(" eGuiSQIisEmptyId %d \n", id);
                guiInterface_SendEvent(id, 0);
            }
            break;
            
        case eGuiChangeToOperScreenId:
            DisplayControl_SetState(eMainScreenIsShowingDispState);
            break;
        case eGuiAlarmEventId:
            SYS_PRINT("\n eGuiAlarmEventId %d %d %d %d %d %d %d %d \n", 
                    guiEvent.eventData.alarmData[ALARM_ID_DATA_INDEX], 
                    guiEvent.eventData.alarmData[ALARM_STATUS_DATA_INDEX],
                    guiEvent.eventData.alarmData[ALARM_PRIORITY_DATA_INDEX], 
                    guiEvent.eventData.alarmData[3],
                    guiEvent.eventData.alarmData[4],
                    guiEvent.eventData.alarmData[5],
                    guiEvent.eventData.alarmData[6],
                    guiEvent.eventData.alarmData[7]);
            AlarmNotification alarmNotification = {
                .alarmId = guiEvent.eventData.alarmData[ALARM_ID_DATA_INDEX],
                .alarmStatus = guiEvent.eventData.alarmData[ALARM_STATUS_DATA_INDEX],
                .alarmPriority = guiEvent.eventData.alarmData[ALARM_PRIORITY_DATA_INDEX],
            };
            memcpy(alarmNotification.alarmData, &guiEvent.eventData.alarmData[3], sizeof(alarmNotification.alarmData));
            AlarmNotificationList_UpdateAlarm(alarmNotification);
            AlarmNotificationList_ProcessAlarmNotificationList();
//            AlarmNotificationList_PrintList();
            
            // update alarm info button
            DisplayControl_UpdateAlarmInfoButton(alarmNotification.alarmStatus);
            
            // update alarm display
            int index = AlarmNotificationList_GetActiveIndex();
            if (index >=0)
            {
                AlarmNotification alarmUpdate;
                AlarmNotificationList_GetItem(index, &alarmUpdate);
                DisplayControl_UpdateAlarm(
                        alarmUpdate.alarmId,
                        alarmUpdate.alarmStatus,
                        alarmUpdate.alarmPriority,
                        alarmUpdate.alarmData
                    );            
            }
            else
            {
                // no active alarm, disable current alarm expression
                DisplayControl_InactiveCurrentAlarm();
            }
            break;
        case eGuiChangeToPowerOffScreenId:
            SYS_PRINT("\n\nPower Off Screen");
            DisplayControl_SetState(ePowerOffScreenIsShowingDispState);
            break;
        case eGuiTurnOffMachineId:
        {
            SYS_PRINT("\n\nTurn off machine.\n");
            uint32_t op_time = DisplayControl_GetTotalOperatingTime();
            DisplayControl_ResetTotalOperatingTime();
            if (setting_Get(eTreatmentModeSettingId) == eAdultMode)
            {
                DeviceInformation_AddOperationTimeAdult(op_time);
                DeviceInformation_Save();
            }
            else if (setting_Get(eTreatmentModeSettingId) == eChildMode)
            {
                DeviceInformation_AddOperationTimeChild(op_time);
                DeviceInformation_Save();
            }
            logInterface_WriteEventLog(0 ,NULL , eStopOperationEventLogId);
            LogInterface_WriteDebugLogFile("Stop Operation \n");
            break;
        }
        case eGuiUpdateScreenMessageUSBNotFound:
            laWidget_SetVisible((laWidget*)btnOK_UpdateScreen, LA_TRUE);
            laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_TRUE);
            laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_USBNotFound));
            break;
        case eGuiUpdateScreenMessageFileNotFound:
            laWidget_SetVisible((laWidget*)btnOK_UpdateScreen, LA_TRUE);
            laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_TRUE);
            laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_FileNotFound));
            break;
        case eGuiUpdateScreenMessageFileInvalid:
            laWidget_SetVisible((laWidget*)btnOK_UpdateScreen, LA_TRUE);
            laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_TRUE);
            laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_FileInvalid));
            break;
        case eGuiUpdateScreenMessageUpdateFailed:
            laWidget_SetVisible((laWidget*)btnOK_UpdateScreen, LA_TRUE);
            laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_TRUE);
            laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_UpdateFailed));            
            break;
        case eGuiUpdateScreenMessageUpdatingMainboardFirmware:
            laWidget_SetVisible((laWidget*)btnOK_UpdateScreen, LA_FALSE);
            laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_TRUE);
            laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_UpdatingMainboardFirmware));            
            break;
        case eGuiUpdateScreenMessageUpdatingMainboardFirmwareStatus:
        {
            char strbuff[255];
            laString strStatus, strNumber, strLabel;

            // Update UI
            laString_Initialize(&strStatus); 
            strLabel = laString_CreateFromID(string_text_UpdateScreen_UpdatingMainboardFirmware);

            sprintf(strbuff, "\n %d%% ", guiEvent.eventData.data);
            strNumber = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S20_Bold_Internal);  

            laString_Append(&strStatus, &strLabel);
            laString_Append(&strStatus, &strNumber);
            laLabelWidget_SetText(lbMessage_UpdateScreen, strStatus );

            laString_Destroy(&strLabel); 
            laString_Destroy(&strNumber); 
            laString_Destroy(&strStatus);  
            break;    
        }            
        case eGuiUpdateScreenMessageUpdatingAssets:
            laWidget_SetVisible((laWidget*)btnOK_UpdateScreen, LA_FALSE);
            laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_TRUE);
            laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_UpdatingAssets));            
//            systemInterface_SendEvent(eSystemUpgradeEventId, 1);
            break;
        case eGuiUpdateScreenMessageCheckingAsset:
            laWidget_SetVisible((laWidget*)btnOK_UpdateScreen, LA_FALSE);
            laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_TRUE);
            laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_CheckingAssets));
            break;
        case eGuiUpdateScreenMessageUpdatingAssetsStatus:
        {
            char strbuff[255];
            laString strStatus, strNumber, strLabel;

            // Update UI
            laString_Initialize(&strStatus); 
            strLabel = laString_CreateFromID(string_text_UpdateScreen_UpdatingAssets);

//            sprintf(strbuff, "\n %.3d / %.3d", (uint16_t)(guiEvent.eventData.data >> 16) , (uint16_t)(guiEvent.eventData.data));
            sprintf(strbuff, "\n %d%% ", guiEvent.eventData.data);
            strNumber = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S20_Bold_Internal);  

            laString_Append(&strStatus, &strLabel);
            laString_Append(&strStatus, &strNumber);
            laLabelWidget_SetText(lbMessage_UpdateScreen, strStatus );

            laString_Destroy(&strLabel); 
            laString_Destroy(&strNumber); 
            laString_Destroy(&strStatus);  
            break;    
        }
        case eGuiUpdateScreenMessageUpdatingChamberFirmware:
            laWidget_SetVisible((laWidget*)btnOK_UpdateScreen, LA_FALSE);
            laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_TRUE);
            laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_UpdatingChamberFirmware));            
            break;
        case eGuiUpdateScreenMessageUpdatingCradleFirmware:
            laWidget_SetVisible((laWidget*)btnOK_UpdateScreen, LA_FALSE);
            laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_TRUE);
            laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_UpdatingCradleFirmware));            
            break;
        case eGuiUpdateScreenMessageUpdatingESP32Firmware:
            laWidget_SetVisible((laWidget*)btnOK_UpdateScreen, LA_FALSE);
            laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_TRUE);
            laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_UpdatingESP32Firmware));            
            break;
        case eGuiUpdateScreenMessageUpdateSuccessTurnOffToComplete:
            laWidget_SetVisible((laWidget*)btnOK_UpdateScreen, LA_FALSE);
            laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_TRUE);
            laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_UpdateSuccessTurnOffDeviceToComplete));            
            break;
            
        case eGuiMainScreenStartWarmingUp:
            MainScreen_SetWarmUpState(eInitWarmUpState);
            break;
        case eGuiMainScreenFinishWarmingUp:
            MainScreen_SetWarmUpState(eFinishedWarmUpState);
            break;
        case eGuiMainScreenExternalBatteryStatusChange:
            StatusBar_SetExternalBatteryStatus((uint8_t)guiEvent.eventData.data);
            break;
        case eGuiMainScreenInternalBatteryStatusChange:
            StatusBar_SetInternalBatteryStatus((uint8_t)guiEvent.eventData.data);
            break;
        case eGuiMainScreenACPowerStatusChange:
            StatusBar_SetACPowerStatus((uint8_t)guiEvent.eventData.data);
            break;
        case eGuiMainScreenWifiStatusChange:
            StatusBar_SetWifiStatus((uint8_t)guiEvent.eventData.data);
            break;
        case eGuiMainScreenAlarmMuteIconShow:
            AlarmExpression_SetAlarmMuteIconShowFlag(true);
            break;
        case eGuiMainScreenAlarmMuteIconHide:
            AlarmExpression_SetAlarmMuteIconShowFlag(false);
            break;
        default:
            break;
        }
    }
    return;
}

void GUI_Task(void)
{
//    static uint16_t testTick = 0;
//    if (testTick == 0) testTick = xTaskGetTickCount();
    //Handle touch Screen
    GT911_Task();
    //Handle system input, including touch event
    SYS_INP_Tasks();
    //Handle event
    guiTask_HandleEvent();

    // Update displaycontrol
    DisplayControl_Tasks();

    //Update log
    logMgr_Task();

#ifdef JFLO_DEBUG_GUI   
    TickType_t xGuiTick = xTaskGetTickCount();
#endif      
    // update the UI library
    GFX_Update();
    
    //Update libaria
    LibAria_Tasks();

#ifdef JFLO_DEBUG_GUI       
    xGuiTick = xTaskGetTickCount() - xGuiTick;
    if (xGuiTick > 100)
        SYS_PRINT("LibAria_Tasks execution %d\n",xGuiTick); 
#endif    
//    SYS_PRINT("\nGUI Task Tick: %d", xTaskGetTickCount() - testTick);
    return;
}

void GUI_Prepare()
{
    PWM_LCDBacklight_Start();
}


/* *****************************************************************************
 End of File
 */
