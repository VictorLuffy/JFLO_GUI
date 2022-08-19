#ifndef DISPLAY_CONTROL_H
#define DISPLAY_CONTROL_H

/* This section lists the other files that are included in this file.
 */
#include "stdint.h"
#ifdef UNIT_TEST
typedef uint32_t TickType_t;
#endif
#ifndef UNIT_TEST

#include "LogInterface.h"
#include "GuiInterface.h"
#endif

#include "MotorControl/MotorTask.h"
#include "HeaterControl/HeaterTask.h"
#include "Device/SpO2Data.h"
#include "Device/RTC_BQ32002.h"
#include "Alarm/AlarmInterface.h"

//#define JFLO_DEBUG_GUI

/** @brief Num of ticks is equivalent to 1ms - CPUCLK/2 = 100Mhz  */   
#define TICKS_PER_MS       100000        // 1ms

#define DATETIME_UPDATE_CYCLE_MS 1000
#define MONITOR_UPDATE_CYCLE_MS 1000
#define GRAPH_O2_UPDATE_CYCLE_MS 20

/*  @brief  __builtin_mfc0(9, 0):
 * It means Move From Co-processor 0 register 9 select 0. 
 * CP0 is the system co-processor, which controls CPU configuration, interrupts etc.
 * CP0 Register 9 is the Count register, which is a free-running 32bit counter which counts at a rate of CPUCLK/2.
 */
#define GET_TICKS()             __builtin_mfc0(9, 0)

/** @brief The state list of Displaycontrol & all Screen  */  
typedef enum
{
	/* Application's state machine's initial state. */
    eIdleDispState,                 /**< no action state */
    eInitDispState,                 /**< Initial state */
   
    eUpdateScreenIsShowingDispState = 20,   /**< UpdateScreen is showing and interface with users */
            
    eVideoScreenIsShowingDispState = 30,    /**< VideoScreen is showing and interface with users */
    eDecode7zVideoDispState,                /**< Decode .7z video file to raw video */
    ePrePlayVideoDispState,                 /**< Setup memory to play video */
    ePlayVideoDispState,                    /**< Play video */
    eFinishedVideoDispState,                /**< Finish video */
    ePlayingVideoDispState, 
            
    eMainScreenIsShowingDispState = 40,     /**< MainScreen is showing and interface with users */
    eTargetSettingLoadingDispState,         /**< Loading temp & Flow value to target one */
    eGearIsShowingDispState,
    eChangedSetpointAreaShowingDispState,
            
    eSettingScreenIsShowingDispState = 50,  /**< SettingScreen is showing and interface with users */          
    ePreShowingComponentDispState,
    eShowingComponentDispState,  
            
            
    eMaintenanceScreenIsShowingDispState = 60,     /**< MaintenanceScreen is showing and interface with users */ 
            
    eAlarmScreenIsShowingDispState = 65,
    
    ePowerOffScreenIsShowingDispState = 70
	/* TODO: Define states used by the application state machine. */

} E_DisplayState;


/** @brief The state substate of Displaycontrol & all Screen  */ 
typedef enum
{
    eIdleDispSubstate = 0,   /**< no action substate */                   
    eInitDispSubstate,  /**< Initial substate */
    eQuitDispSubstate,  /**< Quit substate */
    eRunningDispSubstate,   /**< Running substate */
            
    eLoadingFinishDispSubstate = 10,               
    eSettingChangedDispSubstate,      
    eQuitSettingScreenDispSubstate,        
        
    eWarmingUpDispSubstate = 20,        
    eWarmingUpFinishedDispSubstate,  
            
    eRecoverSettingMenuSubstate,
          
} E_DisplaySubstates;


/** @brief The type of result in Displaycontrol & all Screen  */ 
typedef enum
{
    eGFalse = -1,
    eGTrue = 0,
    eGNotCompletedYet = eGFalse,
    eGCompleted = eGTrue,
} E_GuiResult;

/** @brief Define the data variables in displaycontrol */

typedef struct
{
    /* The application's current state */
    E_DisplayState state;   /**< The application's current state */
    
    MOTOR_PUBLIC_DATA_t dataFlow;   /**< Flow data get from device */
    HEATER_PUBLIC_DATA_t dataTemp;  /**< Temp data get from device */
    
    float dataO2Concentration;  /**< O2 data get from device */
    SP02_DATA_t spo2Data;
    
    bool isCountOpTime;
    RTC_TIME_t currentRTC;  /**< RTC data get from device */
    RTC_TIME_t preRTC;
    
    uint32_t totalOperatingTime; /**< total operating time */
    uint32_t runningTime; /**< total running time from power up */
    bool isRtcError;
    uint8_t screenShowId; /**< screen id already show */
    
} Display_Data_Struct;

/* Set displaycontrol's state */
inline void DisplayControl_SetState(E_DisplayState state) ;

/* Get displaycontrol's state */
inline E_DisplayState DisplayControl_GetState(void);

/* Return temperature value from device */
inline float DisplayControl_GetDataTemp(void);

/* Return temperature value from device */
inline float DisplayControl_GetBreathCircuitOutTemp(void);

/* Return flow's speed value from device */
inline float DisplayControl_GetDataAirFlow(void);

/* Return flow's speed value from device */
inline float DisplayControl_GetDataO2Flow(void);

/* Return flow's speed value from device */
inline float DisplayControl_GetDataFlow(void);

/* Return O2 Concentration value from device */
inline float DisplayControl_GetDataO2Concentration(void);

/* Return SpO2 value from device */
inline float DisplayControl_GetDataSpO2(void);

/* Return flow's speed value from device */
inline float DisplayControl_GetDataPR(void);

/* Set Alarm Reporting ID in DisplayControl */
inline uint8_t DisplayControl_GetAlarmId(void);

/* Return O2Concentration value from device */
inline float DisplayControl_GetDataO2Concentration(void);

/* Get the current time infomation. */
RTC_TIME_t DisplayControl_GetTime(void);

/* Set the alarm status in UI. */
inline uint32_t DisplayControl_GetTotalOperatingTime(void);

uint32_t DisplayControl_GetRunningTime(void);

/** @brief Update the alarm status in alarm list.
 *  @param [in] E_AlarmId alarmId: id of alarm.
 *  @param [in] E_AlarmStatus status : status of alarm.
 *  @param [out]  None
 *  @return None
 */
void DisplayControl_UpdateAlarm(E_AlarmId alarmId, E_AlarmStatus status, E_AlarmPriority priority, uint8_t *data);


/** @brief Disable current alarm UI
 *  @param [in] None
 *  @param [out]  None
 *  @return None
 */
void DisplayControl_InactiveCurrentAlarm();

/**/
bool DisplayControl_SetStartTime(void);

/***/
void DisplayControl_Initialize (void);

/**/
void DisplayControl_Tasks(void);

/* */
void DisplayControl_SetAlarmPriority(E_AlarmPriority p);

/* */
E_AlarmPriority DisplayControl_GetAlarmPriority(void);

E_AlarmStatus DisplayControl_GetAlarmStatus(void);

// Using for test
void DisplayControl_PrintNum(char* str, int num);

void DisplayControl_PrintHex(char* str, int num);

void DisplayControl_PrintString(const char* str);

/** @brief DisplayControl_CalculateDeltaTick
 *       This calculate delta between last tick and current tick, with handle the overflow
 *  @param [in] TickType_t current
 *  @param [in] TickType_t last
 *  @param [out] None
 *  @return TickType_t
 */
TickType_t DisplayControl_CalculateDeltaTick(TickType_t current, TickType_t last);

/** @brief DisplayControl_GetActiveScreenIndex
 *     This stores current screen show, alternative for laContext_GetActiveScreenIndex()
 *     B/c function flow called will be 
 *           laContext_SetActiveScreen () 
 *           -> LA_EVENT_SCREEN_CHANGE 
 *           -> _laContext_HandleScreenChangeEvent() 
 *           -> _laContext_ShowScreen() 
 *           -> newScr->createCB()
 *           -> newScr->showCB()
 *           -> laContext_GetActive()->activeScreen = newScr
 *     So, laContext_GetActiveScreenIndex() will be corrupted if use in newScr->showCB() [our custom screen init function ]
 *  @param [in] TickType_t current
 *  @param [in] TickType_t last
 *  @param [out] None
 *  @return TickType_t
 */
uint8_t DisplayControl_GetActiveScreenIndex();

/** @brief DisplayControl_SetActiveScreenIndex
 *       Set screen id already show
 *  @param [in] uint8_t id
 *  @param [out] None
 *  @return None
 */
void DisplayControl_SetActiveScreenIndex(uint8_t id);

/** @brief DisplayControl_GoToScreen_SettingScreen_DefaultMenu
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void DisplayControl_GoToScreen_SettingScreen_DefaultMenu();

/** @brief DisplayControl_GoToScreen_SettingScreen_AlarmDataLog
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void DisplayControl_GoToScreen_SettingScreen_AlarmDataLog();

/** @brief DisplayControl_GetCenterPostion
 *       This get x position where widgetB will be displayed as center with widgetA
 *  @param [in] laWidget* widgetA
 *  @param [in] laWidget* widgetB
 *  @param [out] None
 *  @return int32_t
 */
int32_t DisplayControl_GetCenterPostion(laWidget* widgetA, laWidget* widgetB);

/** @brief DisplayControl_UpdateAlarmInfoButton
 *       This control for display alarm info button
 *  @param [in] laWidget* widgetA
 *  @param [in] laWidget* widgetB
 *  @param [out] None
 *  @return int32_t
 */
void DisplayControl_UpdateAlarmInfoButton(E_AlarmStatus alarmStatus);

#ifdef JFLO_DEBUG_GUI
void DisplayControl_TestAlarm();
#endif

#endif

// end of file
