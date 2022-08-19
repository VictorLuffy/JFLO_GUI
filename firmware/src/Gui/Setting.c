 /** @file Setting.c
 *  @brief Setting contains all setting for JlowMini.
 *  Setting provides the interface to read and write settings in JFlo Mini 
 *  @author Viet Le
 */
#include "string.h"
#include "stdbool.h"
#include "system_config.h"
#include "system_definitions.h"
#include "Setting.h"
#include "system/debug/sys_debug.h"
#include "crc.h"
#include "peripheral/nvm/plib_nvm.h"
#include "File.h"
#include "GuiInterface.h"
#include "LogInterface.h"
#include "MotorTask.h"
#include "HeaterTask.h"

extern SYS_FS_HANDLE g_settingFile;

/** @brief Define max for setting */
#define NUM_OF_SETTING			eLastSettingId

/** @brief Define setting data length */
#define SETTING_DATA_LENGTH             (NUM_OF_SETTING + 2) 

/** @brief Declare setting list */
static SETTING_ITEM_t s_settingList[NUM_OF_SETTING];

/** @brief Declare old setting */
static uint8_t s_oldSettingData[NUM_OF_SETTING];

/** @brief Mutext protect for task sync */
static SemaphoreHandle_t s_SettingDataMutex = NULL;

/** @brief  */
static bool s_isInit = false;

/** @brief init threshold setting
 *  @param [in] : None
 *  @param [out] : None
 *  @return None
 */
static void setting_Threshold(void)
{
    if(s_SettingDataMutex != NULL && xSemaphoreTake( s_SettingDataMutex, 5 ) == pdTRUE )
    {
        //initial default value for setting list
        s_settingList[eTreatmentModeSettingId].min = eAdultMode;
        s_settingList[eTreatmentModeSettingId].max = eChildMode;
        s_settingList[eTreatmentModeSettingId].step = 1;

        s_settingList[eRoomModeSettingId].min = eHomeMode;
        s_settingList[eRoomModeSettingId].max = eHospitalMode;
        s_settingList[eRoomModeSettingId].step = 1;

        s_settingList[eTempSettingId].min = 31;
        s_settingList[eTempSettingId].max = 37;
        s_settingList[eTempSettingId].step = 1;

        s_settingList[eFlowSettingId].min = 15;
        s_settingList[eFlowSettingId].max = 60;
        s_settingList[eFlowSettingId].step = 5;

        s_settingList[eFiO2SettingId].min = 21;
        s_settingList[eFiO2SettingId].max = 100;
        s_settingList[eFiO2SettingId].step = 1;

        s_settingList[eBrightnessLevelSettingId].min = 1;
        s_settingList[eBrightnessLevelSettingId].max = 10;
        s_settingList[eBrightnessLevelSettingId].step = 1;

        s_settingList[eBrightnessModeSettingId].min = eAutoBrightnessMode;
        s_settingList[eBrightnessModeSettingId].max = eManualBrightnessMode;
        s_settingList[eBrightnessModeSettingId].step = 1;      

        s_settingList[eLanguageSettingId].min = eJapanese;
        s_settingList[eLanguageSettingId].max = eVietnamese;
        s_settingList[eLanguageSettingId].step = 1;

        s_settingList[eBluetoothSettingId].min = eSettingOff;
        s_settingList[eBluetoothSettingId].max = eSettingOn;
        s_settingList[eBluetoothSettingId].step = 1;

        s_settingList[eWifiSettingId].min = eSettingOff;
        s_settingList[eWifiSettingId].max = eSettingOn;
        s_settingList[eWifiSettingId].step = 1;      

        s_settingList[eSpeakerVolumeSettingId].min = 1;
        s_settingList[eSpeakerVolumeSettingId].max = 10;
        s_settingList[eSpeakerVolumeSettingId].step = 1;      

        s_settingList[eSpO2AlarmSettingLowerLimitId].min = 90;
        s_settingList[eSpO2AlarmSettingLowerLimitId].max = 95;
        s_settingList[eSpO2AlarmSettingLowerLimitId].step = 1;

        s_settingList[eOxygenSourceSettingId].min = e90OxygenSource;
        s_settingList[eOxygenSourceSettingId].max = e100OxygenSource;
        s_settingList[eOxygenSourceSettingId].step = 1;

        s_settingList[eHourSettingId].min = 0;
        s_settingList[eHourSettingId].max = 23;
        s_settingList[eHourSettingId].step = 1;  

        s_settingList[eMinuteSettingId].min = 0;
        s_settingList[eMinuteSettingId].max = 59;
        s_settingList[eMinuteSettingId].step = 1; 

        s_settingList[eSecondSettingId].min = 0;
        s_settingList[eSecondSettingId].max = 59;
        s_settingList[eSecondSettingId].step = 1;    

        s_settingList[eDateSettingId].min = 1;
        s_settingList[eDateSettingId].max = 31;
        s_settingList[eDateSettingId].step = 1;  

        s_settingList[eMonthSettingId].min = 1;
        s_settingList[eMonthSettingId].max = 12;
        s_settingList[eMonthSettingId].step = 1;  

        s_settingList[eYear1SettingId].min = 20;
        s_settingList[eYear1SettingId].max = 20;
        s_settingList[eYear1SettingId].step = 1;   

        s_settingList[eYear2SettingId].min = 00;
        s_settingList[eYear2SettingId].max = 99;
        s_settingList[eYear2SettingId].step = 1;       

        s_settingList[eOxygenAlarmSettingUpperLimitId].min = 30;
        s_settingList[eOxygenAlarmSettingUpperLimitId].max = 100;
        s_settingList[eOxygenAlarmSettingUpperLimitId].step = 5;

        s_settingList[eOxygenAlarmSettingLowerLimitId].min = 20;
        s_settingList[eOxygenAlarmSettingLowerLimitId].max = 25;
        s_settingList[eOxygenAlarmSettingLowerLimitId].step = 5;

        s_settingList[eAdultModeTempSettingId].min = MIN_TEMP_SETTING_IN_ADULT_MODE;
        s_settingList[eAdultModeTempSettingId].max = MAX_TEMP_SETTING_IN_ADULT_MODE;
        s_settingList[eAdultModeTempSettingId].step = STEP_TEMP_SETTING_IN_ADULT_MODE;

        s_settingList[eAdultModeFlowSettingId].min = MIN_FLOW_SETTING_IN_ADULT_MODE;
        s_settingList[eAdultModeFlowSettingId].max = MAX_FLOW_SETTING_IN_ADULT_MODE_AC_POWER;
        s_settingList[eAdultModeFlowSettingId].step = STEP_FLOW_SETTING_IN_ADULT_MODE;

        s_settingList[eChildModeTempSettingId].min = MIN_TEMP_SETTING_IN_CHILD_MODE;
        s_settingList[eChildModeTempSettingId].max = MAX_TEMP_SETTING_IN_CHILD_MODE;
        s_settingList[eChildModeTempSettingId].step = STEP_TEMP_SETTING_IN_CHILD_MODE;

        s_settingList[eChildModeFlowSettingId].min = MIN_FLOW_SETTING_IN_CHILD_MODE;
        s_settingList[eChildModeFlowSettingId].max = MAX_FLOW_SETTING_IN_CHILD_MODE;
        s_settingList[eChildModeFlowSettingId].step = STEP_FLOW_SETTING_IN_CHILD_MODE;
        
        s_settingList[eCalibrationGainAirSettingId].min = 1;
        s_settingList[eCalibrationGainAirSettingId].max = 255;
        s_settingList[eCalibrationGainAirSettingId].step = 1;

        s_settingList[eCalibrationOffsetAirSettingId].min = 0;
        s_settingList[eCalibrationOffsetAirSettingId].max = 255;
        s_settingList[eCalibrationOffsetAirSettingId].step = 1;

        s_settingList[eCalibrationGainO2SettingId].min = 1;
        s_settingList[eCalibrationGainO2SettingId].max = 255;
        s_settingList[eCalibrationGainO2SettingId].step = 1;

        s_settingList[eCalibrationOffsetO2SettingId].min = 0;
        s_settingList[eCalibrationOffsetO2SettingId].max = 255;
        s_settingList[eCalibrationOffsetO2SettingId].step = 1;
        
        xSemaphoreGive( s_SettingDataMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take s_SettingDataMutex \n");
    }
    
    return;
}

/** @brief Set default all setting
 *  @param [in] : None
 *  @param [out] : None
 *  @return None
 */
void setting_SetDefault(void)
{
    if(s_SettingDataMutex != NULL && xSemaphoreTake( s_SettingDataMutex, 5 ) == pdTRUE )
    {
        //initial ID
        int i;
        for (i = 0; i < NUM_OF_SETTING; i++)
        {
            s_settingList[i].id = (E_SettingId) i;
        }

        //initial default value for setting list
        s_settingList[eTreatmentModeSettingId].data = eAdultMode;
        s_settingList[eRoomModeSettingId].data = eHomeMode;
        s_settingList[eTempSettingId].data = 37;
        s_settingList[eFlowSettingId].data = 30;
        s_settingList[eFiO2SettingId].data = 21;
        s_settingList[eBrightnessLevelSettingId].data = 7;
        s_settingList[eBrightnessModeSettingId].data = eAutoBrightnessMode;
        s_settingList[eLanguageSettingId].data = eEnglish;
        s_settingList[eBluetoothSettingId].data = eSettingOff;
        s_settingList[eWifiSettingId].data = eSettingOff;
        s_settingList[eSpeakerVolumeSettingId].data = 1;
        s_settingList[eSpO2AlarmSettingLowerLimitId].data = 90;
        s_settingList[eOxygenSourceSettingId].data = e90OxygenSource;
        s_settingList[eHourSettingId].data = 0;
        s_settingList[eMinuteSettingId].data = 0;
        s_settingList[eSecondSettingId].data = 0;
        s_settingList[eDateSettingId].data = 1;
        s_settingList[eMonthSettingId].data = 1;
        s_settingList[eYear1SettingId].data = 20;
        s_settingList[eYear2SettingId].data = 21;

        s_settingList[eOxygenAlarmSettingUpperLimitId].data = 90;
        s_settingList[eOxygenAlarmSettingLowerLimitId].data = 20;
        
        s_settingList[eAdultModeFlowSettingId].data = 30;
        s_settingList[eAdultModeTempSettingId].data = 37;

        s_settingList[eChildModeFlowSettingId].data = 10;
        s_settingList[eChildModeTempSettingId].data = 34;
        
        s_settingList[eCalibrationGainAirSettingId].data = 1;
        s_settingList[eCalibrationOffsetAirSettingId].data = 0;
        s_settingList[eCalibrationGainO2SettingId].data = 1;
        s_settingList[eCalibrationOffsetO2SettingId].data = 0;
#ifdef FUNCTION_DISABLE_ALARM
        for(i = eFirsAlarmOperationModeSettingId; i <= eLastAlarmOperationModeSettingId; i++ )
        {
            s_settingList[i].data = eAlarmEnable;
        }
#endif        
        xSemaphoreGive( s_SettingDataMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take s_SettingDataMutex \n");
    }    
    return;
}

/** @brief Save to flash when check crc sucess
 *  @param [in] : None
 *  @param [out] : None
 *  @return none
 */
void setting_Save(void)
{
//    SYS_PRINT("\n setting_Save \n");
    uint8_t dataLength = SETTING_DATA_LENGTH; //add 2 bytes for CRC check
    int8_t setting[dataLength];
    //copy data from settings to data array
    int j;
    if(s_SettingDataMutex != NULL && xSemaphoreTake( s_SettingDataMutex, 5) == pdTRUE )
    {
        for (j = eFirstSettingId; j < eLastSettingId; j++)
        {
            setting[j] = s_settingList[j].data;
        }
        xSemaphoreGive( s_SettingDataMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take s_SettingDataMutex \n");
    }
    //Check setting is change or not change
    if (memcmp(setting, s_oldSettingData, NUM_OF_SETTING) == 0)
    {
//        SYS_PRINT("\n setting is not change");
    }
    else
    {
//        SYS_PRINT("\n setting is change");
        //add 2 byte CRC
        unsigned short temp = crc_CheckNoInit(dataLength - 2, &setting[0]);

        //Check crc
        setting[dataLength - 2] = (int8_t) (temp & 0xFF);
        setting[dataLength - 1] = (int8_t) ((temp >> 8)&0xFF);

        //save setting to SQI flash
        file_Seek(g_settingFile, 0, SYS_FS_SEEK_SET);
        file_Write(setting, dataLength, g_settingFile);

        //Send event to log task
//        logInterface_WriteMacineLog(eSettingChangedLogId);

        memcpy(&s_oldSettingData[0], setting, sizeof (s_oldSettingData));
    }
    return;
}

/** @brief Restore setting from SQI flash
 *  @param [in] None
 *  @param [out] None
 *  @return bool : true if restore success, else if restore failure
 */
bool setting_Restore(void)
{
    bool rtn = false; //return value
    int i;
    int8_t data[SETTING_DATA_LENGTH] = {0}; //buffer to store data read from SQI flash
    
    //Read data from SQI flash
    file_Read(data, SETTING_DATA_LENGTH, g_settingFile);

    //check crc for the data has been read
    unsigned short temp = crc_CheckNoInit(SETTING_DATA_LENGTH, &data[0]);

    if (temp == 0) //crc good
    {
        //use settings had just restored for application
        for (i = eFirstSettingId; i < eLastSettingId; i++)
        {
            setting_Set((E_SettingId) i, data[i]);
        }
        
        //set flag to indicate restore process is success
        rtn = true;
    }
    else
    {
        char buff[255];
        sprintf(buff, "setting_Restore crc failure %d \n", temp);
        LogInterface_WriteDebugLogFile(buff);
        SYS_PRINT("\n setting_Restore crc failure %d \n", temp);
    }
    return rtn;
}

/** @brief Initialize all setting in JFlo Mini
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void setting_Init(void)
{
    if ( s_SettingDataMutex == NULL )
    {
        s_SettingDataMutex = xSemaphoreCreateMutex();
        xSemaphoreGive( s_SettingDataMutex );
    }
    if ( s_SettingDataMutex == NULL )
    {
        SYS_PRINT("Error: s_SettingDataMutex is NULL \n");
        return;
    }
    //read setting from SQI flash
    bool result = setting_Restore();
    if (result == false) //data not available or wrong data
    {
        SYS_MESSAGE("\n use default setting \n");
        //use default settings
        setting_SetDefault();
        setting_Save();
    }

    //Set threshold setting
    setting_Threshold();

    //Copy current setting to old setting
    if(s_SettingDataMutex != NULL && xSemaphoreTake( s_SettingDataMutex, 5) == pdTRUE )
    {
        memcpy(s_oldSettingData, s_settingList, sizeof (s_oldSettingData));
        xSemaphoreGive( s_SettingDataMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take s_SettingDataMutex \n");
    }
    
    s_isInit = true;
    return;
}

/** @brief Get value of a setting id
 *  @param [in] E_SettingId id : id of setting
 *  @param [out] None
 *  @return uint8_t : value of setting
 */
uint8_t setting_Get(E_SettingId id)
{
    uint8_t ret = 0;
    if(s_SettingDataMutex != NULL && xSemaphoreTake( s_SettingDataMutex, 5) == pdTRUE )
    {
        ret = s_settingList[id].data;
        xSemaphoreGive( s_SettingDataMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take s_SettingDataMutex \n");
    }
    return ret;
}

/** @brief Set value for a setting
 *  @param [in] E_SettingId id : id of setting
 *  @param [in] uint8_t value : value to set
 *  @param [out] None
 *  @return None
 */
void setting_Set(E_SettingId id, uint8_t value)
{
    if(s_SettingDataMutex != NULL && xSemaphoreTake( s_SettingDataMutex, 5) == pdTRUE )
    {
        s_settingList[id].data = value;
        xSemaphoreGive( s_SettingDataMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take s_SettingDataMutex \n");
    }
    return;
}

/** @brief Get min value of a setting id
 *  @param [in] E_SettingId id : id of setting
 *  @param [out] None
 *  @return uint8_t :min value of setting
 */
uint8_t setting_GetMin(E_SettingId id)
{
    uint8_t ret = 0;
    if(s_SettingDataMutex != NULL && xSemaphoreTake( s_SettingDataMutex, 5) == pdTRUE )
    {
        ret = s_settingList[id].min;
        xSemaphoreGive( s_SettingDataMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take s_SettingDataMutex \n");
    }
    return ret;
}

/** @brief Get max value of a setting id
 *  @param [in] E_SettingId id : id of setting
 *  @param [out] None
 *  @return uint8_t :max value of setting
 */
uint8_t setting_GetMax(E_SettingId id)
{
    uint8_t ret = 0;
    if(s_SettingDataMutex != NULL && xSemaphoreTake( s_SettingDataMutex, 5) == pdTRUE )
    {
        ret = s_settingList[id].max;
        xSemaphoreGive( s_SettingDataMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take s_SettingDataMutex \n");
    }
    return ret;
}

/** @brief Get step value of a setting id
 *  @param [in] E_SettingId id : id of setting
 *  @param [out] None
 *  @return uint8_t :step value of setting
 */
uint8_t setting_GetStep(E_SettingId id)
{
    uint8_t ret = 0;
    if(s_SettingDataMutex != NULL && xSemaphoreTake( s_SettingDataMutex, 5) == pdTRUE )
    {
        ret = s_settingList[id].step;
        xSemaphoreGive( s_SettingDataMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take s_SettingDataMutex \n");
    }
    return ret;
}

/** @brief Get data in range
 *  @param [in] uint8_t data
 *  @param [in] uint8_t max
 *  @param [in] uint8_t min
 *  @param [out] None
 *  @return uint8_t data
 */
uint8_t setting_GetDataInRange(uint8_t data, uint8_t max, uint8_t min)
{
    if (data > max)
        data = max;
    if (data < min)
        data = min;
    return data;                
}

/** @brief Update temp, flow setting up to treatment mode
 *  @param [in] E_TreatmentMode treatmentPreMode - previous mode
 *  @param [in] E_TreatmentMode treatmentMode - mode need to switch
 *  @param [out] None
 *  @return void
 */
void setting_UpdateSettingByTreatmentMode(E_TreatmentMode treatmentPreMode, E_TreatmentMode treatmentMode)
{
    if (treatmentMode == treatmentPreMode)
    {
        return;
    }
    if(s_SettingDataMutex != NULL && xSemaphoreTake( s_SettingDataMutex, 5) == pdTRUE )
    {
        //backup current setting if previous mode is Adult or Child
        if (treatmentPreMode == eChildMode)
        {
            s_settingList[eChildModeFlowSettingId].data = s_settingList[eFlowSettingId].data;
            s_settingList[eChildModeTempSettingId].data = s_settingList[eTempSettingId].data;   
        }
        else if (treatmentPreMode == eAdultMode)
        {   
            s_settingList[eAdultModeFlowSettingId].data = s_settingList[eFlowSettingId].data;
            s_settingList[eAdultModeTempSettingId].data = s_settingList[eTempSettingId].data;        
        }
        else {    }  

        // load new setting if new mode is Adult or child
        if (treatmentMode == eAdultMode)
        {
            //set new setting
            s_settingList[eFlowSettingId].max = s_settingList[eAdultModeFlowSettingId].max;
            s_settingList[eFlowSettingId].min = s_settingList[eAdultModeFlowSettingId].min;
            s_settingList[eFlowSettingId].step = s_settingList[eAdultModeFlowSettingId].step;
            s_settingList[eFlowSettingId].data = s_settingList[eAdultModeFlowSettingId].data;

            s_settingList[eTempSettingId].max = s_settingList[eAdultModeTempSettingId].max;
            s_settingList[eTempSettingId].min = s_settingList[eAdultModeTempSettingId].min;
            s_settingList[eTempSettingId].step = s_settingList[eAdultModeTempSettingId].step;
            s_settingList[eTempSettingId].data = s_settingList[eAdultModeTempSettingId].data;

        }
        else if (treatmentMode == eChildMode )
        {
            //set new setting
            s_settingList[eFlowSettingId].max = s_settingList[eChildModeFlowSettingId].max;
            s_settingList[eFlowSettingId].min = s_settingList[eChildModeFlowSettingId].min;
            s_settingList[eFlowSettingId].step = s_settingList[eChildModeFlowSettingId].step;
            s_settingList[eFlowSettingId].data = s_settingList[eChildModeFlowSettingId].data;

            s_settingList[eTempSettingId].max = s_settingList[eChildModeTempSettingId].max;
            s_settingList[eTempSettingId].min = s_settingList[eChildModeTempSettingId].min;
            s_settingList[eTempSettingId].step = s_settingList[eChildModeTempSettingId].step;
            s_settingList[eTempSettingId].data = s_settingList[eChildModeTempSettingId].data;
        }
        else {}

        s_settingList[eTreatmentModeSettingId].data = treatmentMode;

        s_settingList[eFlowSettingId].data = setting_GetDataInRange(
                s_settingList[eFlowSettingId].data, 
                s_settingList[eFlowSettingId].max, 
                s_settingList[eFlowSettingId].min 
                );
        s_settingList[eTempSettingId].data = setting_GetDataInRange(
                s_settingList[eTempSettingId].data, 
                s_settingList[eTempSettingId].max, 
                s_settingList[eTempSettingId].min 
                );
        
        xSemaphoreGive( s_SettingDataMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take s_SettingDataMutex \n");
    }
}

/** @brief Update temp, flow setting up to power mode
 *  @param [in] bool isACPower - AC Power or Battery Power
 *  @param [in] bool isForceUpdate - this is the first call / or force check
 *  @param [out] None
 *  @return void
 */
void setting_UpdateSettingToPowerMode(bool isACPower, bool isForceUpdate)
{    
    static bool preStatus = false;
    if (isForceUpdate)
    {}
    else
    {
        if (preStatus == isACPower)
        {
            // no change status
            return;
        }       
    }
    if(s_SettingDataMutex != NULL && xSemaphoreTake( s_SettingDataMutex, 5) == pdTRUE )
    {
    // only effect max flow in adult mode
        if (isACPower)
        {
            if (s_settingList[eTreatmentModeSettingId].data == eAdultMode)
            {
                s_settingList[eFlowSettingId].max = MAX_FLOW_SETTING_IN_ADULT_MODE_AC_POWER;
                s_settingList[eFlowSettingId].data = setting_GetDataInRange(
                    s_settingList[eFlowSettingId].data, 
                    s_settingList[eFlowSettingId].max, 
                    s_settingList[eFlowSettingId].min
                    );            
                SYS_PRINT("setting_UpdateSettingToPowerMode isACPower-eAdultMode\n");
            }
        }
        else
        {
            if (s_settingList[eTreatmentModeSettingId].data == eAdultMode)
            {
                s_settingList[eFlowSettingId].max = MAX_FLOW_SETTING_IN_ADULT_MODE_BATTERY_POWER;
                s_settingList[eFlowSettingId].data = setting_GetDataInRange(
                    s_settingList[eFlowSettingId].data, 
                    s_settingList[eFlowSettingId].max, 
                    s_settingList[eFlowSettingId].min
                    );
                SYS_PRINT("setting_UpdateSettingToPowerMode BatterryPower-eAdultMode\n");
            }
        }
        preStatus = isACPower;
        xSemaphoreGive( s_SettingDataMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take s_SettingDataMutex \n");
    }
}

/** @brief check setting is init
 *  @return bool true if init
 */
bool setting_IsInit()
{
    return s_isInit;
}

void setting_SetTemp(uint8_t t)
{       
    uint8_t logData[2];
    uint8_t current_t = setting_Get(eTempSettingId);
    if (t != current_t)
    {
        //Send temperature setting
        //send event to Flow Control Task to start motor
        HEATER_CTRL_EVENT_t eventTemp = {.id = eHeaterTempId, .fData = (float)t};
        HeaterTask_SendEvent(eventTemp);
        
        logData[0] = current_t;
        logData[1] = t;
        logInterface_WriteEventLog(2,(void*)logData , eChangeTargetTemperatureSettingEventLogId);  
        
        setting_Set(eTempSettingId, t);
        setting_Save();
    }
}
void setting_SetAirFlow(uint8_t t)
{
    uint8_t logData[2];
    uint8_t current_t = setting_Get(eFlowSettingId);
    if (t != current_t)
    {
        //Send flow setting
        MOTOR_CTRL_EVENT_t eventFlow = {.id = eMotorFlowId, .fData = (float) t};
        //send event to Flow Control Task to start motor
        MotorTask_SendEvent(eventFlow);
        
        logData[0] = current_t;
        logData[1] = t;
        logInterface_WriteEventLog(2,(void*)logData , eChangeTargetFlowSettingEventLogId);  

        setting_Set(eFlowSettingId, t);
        setting_Save();
    }                
}
void setting_SetOxygen(uint8_t t)
{
    // TBD
}
void setting_SetLanguage(E_Language t)
{
    uint8_t logData[2];
    uint8_t current_t = setting_Get(eLanguageSettingId);
    if (t != current_t)
    {
        logData[0] = current_t;
        logData[1] = t;
        logInterface_WriteEventLog(2,(void*)logData , eChangeLanguageSettingEventLogId);  

        setting_Set(eLanguageSettingId, t);
        setting_Save();
    }
}
void setting_SetBrightnessMode(E_BrightnessMode t)
{
    uint8_t logData[2];
    uint8_t current_t = setting_Get(eBrightnessModeSettingId);
    if (t != current_t)
    {
        logData[0] = current_t;
        logData[1] = t;
        logInterface_WriteEventLog(2,(void*)logData , eChangeIlluminanceSettingEventLogId);  

        setting_Set(eBrightnessModeSettingId, t);
        setting_Save();
    }
}
void setting_SetBrightnessLevel(uint8_t t)
{
    uint8_t logData[2];
    uint8_t current_t = setting_Get(eBrightnessLevelSettingId);
    if (t != current_t)
    {
        logData[0] = current_t;
        logData[1] = t;
        logInterface_WriteEventLog(2,(void*)logData , eChangeIlluminanceLevelSettingEventLogId);  

        setting_Set(eBrightnessLevelSettingId, t);
        setting_Save();
    }
}
void setting_SetMode(E_TreatmentMode mode)
{
    SYS_PRINT("setting_SetMode %d\n ", mode);
    if (mode == setting_Get(eTreatmentModeSettingId))
        return;
    uint8_t tempSetting = setting_Get(eTempSettingId);
    uint8_t flowSetting = setting_Get(eFlowSettingId);
    setting_UpdateSettingByTreatmentMode(setting_Get(eTreatmentModeSettingId), mode);
    setting_UpdateSettingToPowerMode(powerManagement_GetACConnectionState(), true);
    setting_Save();
    
    // Write a log
    uint32_t op_time = DisplayControl_GetTotalOperatingTime();
    SYS_PRINT("op_time %d\n ", op_time);
    DisplayControl_ResetTotalOperatingTime();            
    uint8_t logData[2];
    if (setting_Get(eTreatmentModeSettingId) == eAdultMode)
    {
        logData[0] = TREATMENT_MODE_P_TO_A >> 8;
        logData[1] = (uint8_t)TREATMENT_MODE_P_TO_A;
        DeviceInformation_AddOperationTimeAdult(op_time);
        DeviceInformation_Save();
    }
    else if (setting_Get(eTreatmentModeSettingId) == eChildMode)
    {
        logData[0] = TREATMENT_MODE_A_TO_P >> 8;
        logData[1] = (uint8_t)TREATMENT_MODE_A_TO_P;
        DeviceInformation_AddOperationTimeChild(op_time);
        DeviceInformation_Save();
    }
    else {}
    logInterface_WriteEventLog(2,(void*)logData , eTreatmentModeChangedEventLogId);  
    
    if (tempSetting != setting_Get(eTempSettingId))
    {
        //Send temperature setting
        //send event to Flow Control Task to start motor
        HEATER_CTRL_EVENT_t eventTemp = {.id = eHeaterTempId, .fData = (float) setting_Get(eTempSettingId)};
        HeaterTask_SendEvent(eventTemp);
        
        logData[0] = tempSetting;
        logData[1] = setting_Get(eTempSettingId);
        logInterface_WriteEventLog(2,(void*)logData , eChangeTargetTemperatureSettingEventLogId);  
    }
    if (flowSetting != setting_Get(eFlowSettingId))
    {
        //Send flow setting
        MOTOR_CTRL_EVENT_t eventFlow = {.id = eMotorFlowId, .fData = (float) setting_Get(eFlowSettingId)};
        //send event to Flow Control Task to start motor
        MotorTask_SendEvent(eventFlow);
        
        logData[0] = flowSetting;
        logData[1] = setting_Get(eFlowSettingId);
        logInterface_WriteEventLog(2,(void*)logData , eChangeTargetFlowSettingEventLogId);  
    }
    
}

void setting_SetSpeaker(uint8_t t)
{
    uint8_t logData[2];
    uint8_t current_t = setting_Get(eSpeakerVolumeSettingId);
    if (t != current_t)
    {
        logData[0] = current_t;
        logData[1] = t;
        logInterface_WriteEventLog(2,(void*)logData , eChangeSpeakerLevelSettingEventLogId);

        setting_Set(eSpeakerVolumeSettingId, t);
        setting_Save();
    }
}

void setting_SetWifi(E_OnOff t)
{
    uint8_t logData[2];
    uint8_t current_t = setting_Get(eWifiSettingId);
    if (t != current_t)
    {
        // Write a log
        uint8_t logData[2];
        if (t == eSettingOff)
        {
            logData[0] = WIFI_ON_TO_OFF >> 8;
            logData[1] = (uint8_t)WIFI_ON_TO_OFF;
        }
        else
        {
            logData[0] = WIFI_OFF_TO_ON >> 8;
            logData[1] = (uint8_t)WIFI_OFF_TO_ON;            
        }               
        logInterface_WriteEventLog(2,(void*)logData , eWifiSettingChangeEventLogId);      
        
        setting_Set(eWifiSettingId, t);
        setting_Save();
    }
}

void setting_CalibrationAirSetting(uint8_t gain, uint8_t offset)
{
    uint8_t current_gain = setting_Get(eCalibrationGainAirSettingId);
    uint8_t current_offset = setting_Get(eCalibrationOffsetAirSettingId);
    if (gain != current_gain)
    {
        setting_Set(eCalibrationGainAirSettingId, gain);
    }
    if (offset != current_offset)
    {
        setting_Set(eCalibrationOffsetAirSettingId, gain);
    }
    if ( gain != current_gain ||  offset != current_offset)
    {
        setting_Save();
    }
}

void setting_CalibrationO2Setting(uint8_t gain, uint8_t offset)
{
    uint8_t current_gain = setting_Get(eCalibrationGainO2SettingId);
    uint8_t current_offset = setting_Get(eCalibrationOffsetO2SettingId);
    if (gain != current_gain)
    {
        setting_Set(eCalibrationGainO2SettingId, gain);
    }
    if (offset != current_offset)
    {
        setting_Set(eCalibrationOffsetO2SettingId, gain);
    }
    if ( gain != current_gain ||  offset != current_offset)
    {
        setting_Save();
    }    
}

void setting_CheckPowerMode()
{  
    // Check power mode for change setting
    if (setting_IsInit())
    {
        // check flow, temp setting by power mode
        uint8_t tempSetting = setting_Get(eTempSettingId);
        uint8_t flowSetting = setting_Get(eFlowSettingId);
        setting_UpdateSettingToPowerMode(powerManagement_GetACConnectionState(), false);

        if (tempSetting != setting_Get(eTempSettingId) || flowSetting != setting_Get(eFlowSettingId))
        {
            setting_Save();
        }
        else
        {
            return;
        }
        uint8_t logData[2];
        if (tempSetting != setting_Get(eTempSettingId))
        {
            //Send temperature setting
            //send event to Flow Control Task to start motor
            HEATER_CTRL_EVENT_t eventTemp = {.id = eHeaterTempId, .fData = (float) setting_Get(eTempSettingId)};
            HeaterTask_SendEvent(eventTemp);

            logData[0] = tempSetting;
            logData[1] = setting_Get(eTempSettingId);
            logInterface_WriteEventLog(2,(void*)logData , eChangeTargetTemperatureSettingEventLogId);  
        }
        if (flowSetting != setting_Get(eFlowSettingId))
        {
            //Send flow setting
            MOTOR_CTRL_EVENT_t eventFlow = {.id = eMotorFlowId, .fData = (float) setting_Get(eFlowSettingId)};
            //send event to Flow Control Task to start motor
            MotorTask_SendEvent(eventFlow);

            logData[0] = flowSetting;
            logData[1] = setting_Get(eFlowSettingId);
            logInterface_WriteEventLog(2,(void*)logData , eChangeTargetFlowSettingEventLogId);  
        }
    }
}

/* end of file */

