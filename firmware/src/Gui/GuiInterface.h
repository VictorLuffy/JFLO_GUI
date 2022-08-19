/** @file GuiInterface.h
 *  @brief Contains functions for interface with Gui
 *  @author Viet Le
 */

#ifndef GUIINTERFACE_H
#define	GUIINTERFACE_H

#include <stdint.h>

#ifndef UNIT_TEST
#include "system_config.h"
#include "system_definitions.h"
#endif

#include "Alarm/AlarmInterface.h"

#define ALARM_ID_DATA_INDEX       (0)
#define ALARM_STATUS_DATA_INDEX   (1) 
#define ALARM_PRIORITY_DATA_INDEX (2) 

typedef enum {
    eBatteryDisconnect,
    eBatteryLevel0,
    eBatteryLevel1,
    eBatteryLevel2,
    eBatteryLevel3,
    eBatteryLevel4,
    eNoOfBatteryStatus
} E_BatteryStatus;

typedef enum {
    eACPowerDisconnect,
    eACPowerConnect,
    eNoOfACPowerStatus
} E_ACPowerStatus;

typedef enum {
    eWifiOff,
    eWifiLevel1,
    eWifiLevel2,
    eWifiLevel3,
    eWifiLevel4,
    eNoOfWifiStatus
} E_WifiStatus;

typedef enum {
    eBluetoothOn,
    eBluetoothOff,
    eNoOfBluetoothStatus        
} E_BluetoothStatus;


/** @brief Define event send from other task to GUI */
typedef enum {
    eGuiFirstEventId = 0,

    //event for main screen
    eGuiChangeToOperScreenId, // request change to operation screen
    eGuiChangeToClinicScreenId, // request change to clinic screen	(should be continue)
    eGuiChangeToMaintenaceScreenId, // request change to maintenance screen	(should be continue)
    eGuiChangeToHistoryScreenId, // request change to history screen	(should be continue)
    eGuiChangeSettingFromCloudId, // request to change setting from cloud device
    eGuiUpdateAnimationId, //Update animation
    eGuiChangeModePersonId, //Change mode
    eGuiSQIMountId, //Mount SQI
    eGuiSQIisEmptyId, //SQI is empty
    eGuiSQIisNotEmptyId, //SQI is not empty
    eGuiSelfDiagnosticSuccessId, //Self diagnostic success
    eGuiSelfDiagnosticFailureId, //Self diagnostic failure
    eGuiO2MonitorId, //O2 monitor
    eGuiTempMonitorId, //Temp monitor
    eGuiFlowMonitorId, //Flow monitor
    eGuiAlarmEventId, //alarm id
    eGuiChangeToPowerOffScreenId,
    eGuiTurnOffMachineId, //alarm id
    eGuiUpdateMotorVersion, // signal to update string of motor version
    
    eGuiMainScreenStartWarmingUp,
    eGuiMainScreenFinishWarmingUp,
    eGuiMainScreenExternalBatteryStatusChange,
    eGuiMainScreenInternalBatteryStatusChange,
    eGuiMainScreenACPowerStatusChange,
    eGuiMainScreenBluetoothStatusChange,
    eGuiMainScreenWifiStatusChange,
    eGuiMainScreenAlarmMuteIconShow,
    eGuiMainScreenAlarmMuteIconHide,
            
    eGuiUpdateScreenMessageUSBNotFound,
    eGuiUpdateScreenMessageFileNotFound,
    eGuiUpdateScreenMessageFileInvalid,
    eGuiUpdateScreenMessageUpdateFailed,
    eGuiUpdateScreenMessageUpdatingMainboardFirmware,// text "Updating Mainboard Firmware"
    eGuiUpdateScreenMessageUpdatingMainboardFirmwareStatus,
    eGuiUpdateScreenMessageUpdatingAssets,// text "Updating Assets"
    eGuiUpdateScreenMessageUpdatingAssetsStatus,
    eGuiUpdateScreenMessageUpdatingChamberFirmware,// text "Updating Chamber Firmware"
    eGuiUpdateScreenMessageUpdatingCradleFirmware,// text "Updating Cradle Firmware"
    eGuiUpdateScreenMessageUpdatingESP32Firmware,// text "Updating ESP32 Firmware"   
    eGuiUpdateScreenMessageUpdateSuccessTurnOffToComplete,// "Update success, turn off device to complete"    
    eGuiUpdateScreenMessageCheckingAsset, // "Checking Assets"
    eNoOfGuiEventId,


            
} E_GuiEventId;

/** @brief Define union of gui event data */
typedef union {
    long data;
    uint8_t alarmData[8];
} Gui_Event_Data;

/** @brief Define GUI event data */
typedef struct {
    E_GuiEventId id;
    Gui_Event_Data eventData;
} GUI_EVENT_t;

/** @brief This operation send event to guiQueue to communicate with GUI task
 *  @param [in] uint8_t id: ID of the event	
 *  @param [in] long data: content of the event 
 *  @param [out] None
 *  @return bool : true if event was sent successful, false is event was sent failed
 */
inline bool guiInterface_SendEvent(uint8_t id, long data);

/** @brief This operation send alarm event to guiQueue to communicate with GUI task
 *  @param [in] uint8_t alarmId
 *  @param [in] uint8_t status 
 *  @param [in] uint8_t priority
 *  @param [in] uint8_t* data - data[0] additional detail message (E_AlarmData_AddtionalMessage)
 *                                      The detail message will be switched with this message every timeout
 *                            - data[1] conditional detail message (E_AlarmData_E005, E_AlarmData_E010, E_AlarmData_E011, E_AlarmData_E014)
 *                                      The detail message will be selected by conditions
 *                            - data[2] alarm reset button status (E_AlarmData_ResetButtonStatus)
 *                            - data[3] reserved (0 when unused)
 *                            - data[4] reserved (0 when unused)
 *  @param [out] None
 *  @return bool : true if event was sent successful, false is event was sent failed
 */
inline bool guiInterface_SendEventAlarm(uint8_t alarmId, uint8_t status, uint8_t priority, uint8_t *data);

#endif	/* GUIINTERFACE_H */

/* end of file */
