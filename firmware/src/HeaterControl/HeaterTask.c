/* ************************************************************************** */
/** @file [HeaterTask.c]
 *  @brief {This file contain source code necessary for Heater Control. It implements
 * a Heater Control Task, handle event come to Heater Control Task, initialize dependency
 * components such as Temperature sensors, PWM for IH circuit, ... and control 
 * them during operation }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system_config.h"
#include "system_definitions.h"

#include "HeaterTask.h"
#include "PWM_IH.h"
#include "ADC.h"
#include "I2C_4.h"
#include "PWM_Bumper.h"
#include "BME280.h"
#include "TemperatureController.h"
#include "HumidityController.h"
#include "AlarmInterface.h"
#include "PC_Monitoring.h"
#include "GuiInterface.h"
#include "MotorTask.h"
#include "HumidityPower.h"
#include "BreathCircuitTemperatureController.h"
#include "ChamberUnit.h"
#include <RCFilter.h>
#include "Setting.h"
#include <math.h>

/** @brief HEATER CONTROL task priority */
#define		HEATER_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )

/** @brief HEATER CONTROL stack size */
#define 	HEATER_TASK_STACK			( 1 * 1024 )        //1K Words = 4KB

/** @brief HEATER CONTROL queue size */
#define 	HEATER_QUEUE_SIZE			(16)

/** @brief HEATER CONTROL task preodic */
#define 	HEATER_TASK_PRIODIC_MS      (200 / portTICK_PERIOD_MS)

/** @brief HEATER CONTROL max time to wait while sending data to queue  */
#define 	HEATER_QUEUE_MAX_WAIT_MS	(2 / portTICK_PERIOD_MS)

/** @brief  Maximum time to wait for MUTEX to get public data */
#define 	HEATER_MUTEX_MAX_WAIT		(2 / portTICK_PERIOD_MS)         //5ms

#define     EVT_PREHEATING_TEMPERATURE  (60)

RCflt_t s_chamberOutletTempF;

RCflt_t s_envTempF;
RCflt_t s_envHumF;
RCflt_t s_flowF;


/** @brief MUTEX to protect Heater Data  sharing */
static SemaphoreHandle_t s_HeaterDataMutex = NULL;

/** @brief HEATER CONTROL task Queue */
static QueueHandle_t s_HeaterQueue;

/** @brief Heater operation mode, indicate which operation mode should perform:
 * Normal operation/ Moist Heat Disinfection/ Dry Heat Sterilization */
static E_HeaterOperationMode s_HeaterOperationMode = eHeaterAdultOrChildOperationMode;

/** @brief Operation phase in disinfection mode, use when running disinfection mode */
static E_DisinfectionPhase s_DisinfectionPhase = eWarmingUpPhase;

static E_HeatingState s_TempAndHumiHeatingState = ePreHeating;


/** @brief Variable to hole time count for timing*/
static uint32_t s_countTime = 0;

/** @brief Variable to hole Heater control status, useful for state machine of 
 * controller*/
static E_HeaterState s_HeaterCtrlState = eHeaterStopState;

/** @brief Target Temperature, use when running Heater control loop */
static float s_TemperatureSetting = 0;

/** @brief Breath Ciruit Target Temperature, use when running Heater control loop */
static float s_TemperatureBreathCiruitSetting = 0;

/** @brief Target Temperature, use when running Heater control loop */
static float s_TemperatureChamberOutTarget = 0;

/** @brief Breath Ciruit Target Temperature, use when running Heater control loop */
static float s_TemperatureBreathCiruitOutTarget = 0;

/** @brief Operating PID Temperature, use when ramp up  */
static float s_TemperatureCharmberOutTargetPID = 0;

/** @brief Breath Ciruit Target Temperature, use when running Heater control loop */
static float s_TemperatureBreathCiruitOutTargetPID = 0;

/** @brief Measured Chamber out temperature */
static float s_TemperatureChamberOut = 0;

/** @brief Measured Chamber IH core temperature */
static float s_TemperatureChamberIHCore = 0;

/** @brief Measured water level tank */
static E_TankWaterLevel s_TankWaterLevel = 0;


/** @brief Measured Breathing ciruit out temperature */
static float s_TemperatureBreathCiruitOut = 0;

/** @brief Heater task Public data for sharing */
static HEATER_PUBLIC_DATA_t s_HeaterPublicData;

static bool needToStartRampUpHumidity = false;

static bool s_IsWaterPumpOperate = true;
static float s_FlowSetting;
static float s_breathCircuitTempRampUpStep = 0.02;
static float s_targetPowerRamup = 0;
static float s_evtR = 0;
//static float  chamberTemp_OFFSET = 0;

static E_WarmingUpStatus s_WarmingUpStatus = eNoneWarmingUp;

/** @brief local functions  */
//static void HeaterTask_Func(void);
static void HeaterTask_HandleEvent(void);
static void HeaterTask_Maintain(void);
static bool HeaterTask_Operate(void);
static float HeaterTask_TargetHumidity(float tempSetting);
static void HeaterTask_ChamberOutletTemperatureRampUp();
static void HeaterTask_BeathCircuitTemperatureRampUp();
static void HeaterTask_HumidityRampUp(float target, float current);

float HeaterTask_HumidityCalcPumpStartValue(float TemperatureSetting, float FlowSetting, float envTemp, float envHum);
//bool IsHeaterTask_HumidityRampUp_Done();
int8_t getMaxNumberOfPwCnt(float temp);
bool getEvtR(float *evtR);
float calcEvtTempDiff(float currTemp);

/** @brief Support API to send event to HEATER CONTROL task from other tasks
 *  @param [in]     HEATER_CTRL_EVENT_t event    event to send
 *  @param [out]  None
 *  @return None
 *  @retval true sending event to HEATER CONTROL queue OK
 *  @retval false sending event to HEATER CONTROL queue ERROR
 */
inline bool HeaterTask_SendEvent(HEATER_CTRL_EVENT_t event) {
    //return value
    bool rtn = true;
    //send event
    if (xQueueSendToBack(s_HeaterQueue, &event, HEATER_QUEUE_MAX_WAIT_MS) != pdPASS) {
        xQueueReset(s_HeaterQueue);
        rtn = false;
    }
    return rtn;
}

/** @brief Initialize HEATER CONTROL task and all components that dependency, 
 * including I2C4 for Temperature & Humidity sensor communication; PWM for IH 
 * circuit; PWM for Water Bumper control
 * This function should be called 1 times at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void HeaterTask_Initialize(void) {
    //create HEATER CONTROL queue to communicate with other tasks
    s_HeaterQueue = xQueueCreate(HEATER_QUEUE_SIZE, sizeof (HEATER_CTRL_EVENT_t));
    //initialize Heater PWM channels
    PWM_IH_Initialize();
    //initialize Heater PID control
    TemperatureController_Initialize();
    //initialize Breathing Circuit PID Temperature control
    BreathCircuitTemperatureController_Initialize();
    
    /* HUMIDITY*/
    
    Chamber_Initialize();

    //initialize PWM for Water Bumper control
    //PWM_Bumper_Initialize();
    //initialize Humidity PID control
    HumidityController_Initialize();
    
    //create MUTEX
    s_HeaterDataMutex = xSemaphoreCreateMutex();
    xSemaphoreGive(s_HeaterDataMutex);
    
    //initialize variables
    s_HeaterCtrlState = eHeaterStopState;
    s_TemperatureSetting = 30;
    s_TemperatureCharmberOutTargetPID = 25;
    s_TemperatureChamberOutTarget = 35;
    s_TemperatureBreathCiruitOutTargetPID = 25;
    s_TemperatureBreathCiruitOutTarget  = 35;
    
    //Lpf_Init(&Temp);
    Lpf_Init(&s_envTempF, 20);
    Lpf_Init(&s_envHumF, 0);
    Lpf_Init(&s_flowF, 0);
}

///** @brief Create a Real Time task to do Heater control. 
// *  @param [in]     None
// *  @param [out]    None
// *  @return None
// */
//void HeaterTask_Create(void) {
//    xTaskCreate((TaskFunction_t) HeaterTask_Func,
//            "Heater Control Task",
//            HEATER_TASK_STACK, NULL, HEATER_TASK_PRIORITY, NULL);
//}

/** @brief Get data sharing from Heater Control such as: Temperature at Chamber
 * outlet, Temperature at IH Core surface, Current power supply for IH core 
 * in percent ...
 *  @param [in]  None   
 *  @param [out]  HEATER_PUBLIC_DATA_t* data   external pointer to store data
 *  @return None
 *  @retval true    getting data OK
 *  @retval false   getting data Failed
 */
bool HeaterTask_GetPublicData(HEATER_PUBLIC_DATA_t* data) {
    //take resource 
    if (xSemaphoreTake(s_HeaterDataMutex, HEATER_MUTEX_MAX_WAIT) == pdTRUE) {
        //copy data
        *data = s_HeaterPublicData;
        //release semaphore
        xSemaphoreGive(s_HeaterDataMutex);
        
        return true;
    }
    else {
        return false;
    }
}

/** @brief Function to run HEATER CONTROL task. This function needs to be called
 * in a loop
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void HeaterTask_Run(void) {
    //process input events
    HeaterTask_HandleEvent();
    //Maintain Heater control task
    HeaterTask_Maintain();
}
///** @brief Function to maintain HEATER CONTROL task. This function will be executed
// * automatically by RTOS after HeaterTask_Initialize() function is called
// *  @param [in]     None
// *  @param [out]    None
// *  @return None
// */
//void HeaterTask_Func(void) {
//    
//    //variable record execution time 
//    static TickType_t s_HeaterTaskWakeTime;
//    //declare time scheduler
//    const TickType_t cycleTime = HEATER_TASK_PRIODIC_MS;
//
//    /*HUMIDITY*/
//
//    //shutdown Bumper at the beginning
//    
//    //PUMB_ENABLEOff();
//    /*HUMIDITY*/
//    
//    //record execution time
//    s_HeaterTaskWakeTime = xTaskGetTickCount();
//
//    while (1) {
//        
////        TickType_t static s_lastTick = 0;
////        TickType_t curr = xTaskGetTickCount();
////        SYS_PRINT("%d\n", curr - s_lastTick);
////        s_lastTick = curr;
//        //process input events
//        HeaterTask_HandleEvent();
//        
//        //Maintain Heater control task
//        HeaterTask_Maintain();
//        
//        //wait until next turn
//        vTaskDelayUntil(&s_HeaterTaskWakeTime, cycleTime);
//    }
//}

/** @brief Maintain HEATER CONTROL task, including reading temperature and humidity
 * data, control power drop on IH circuit, control Water Bumper ...
 * drive Blower, ...
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void HeaterTask_Maintain(void) {
  
        //SYS_PRINT("s_HeaterCtrlState_ %d\n", s_HeaterCtrlState);
        switch (s_HeaterCtrlState) {
        case eHeaterIdleState:
        {
            //do something on IDLE state
                          
            //update public data
            if (xSemaphoreTake(s_HeaterDataMutex, HEATER_MUTEX_MAX_WAIT) == pdTRUE) {

                s_HeaterPublicData.chamberOutTemp = Chamber_GetChamberOutTemp();
                s_HeaterPublicData.breathCircuitOutTemp = Chamber_GetBreathingCircuitTemperature();   
                s_HeaterPublicData.warmingUpState = s_WarmingUpStatus;
                //release semaphore
                xSemaphoreGive(s_HeaterDataMutex);
            }
        }
            break;   
        case eHeaterStartState: 
        {
            guiInterface_SendEvent(eGuiMainScreenStartWarmingUp, 0);
            s_WarmingUpStatus = eWarmingUp;
            //start IH Heater PWM
            PWM_IH_Start();
            //enable HEATER CONTROL 
            IH_ENABLEOn();
            
            /*HUMIDITY*/
            //Start Bumper
            //PWM_Bumper_Start();
            //enable Bumper
            //PUMB_ENABLEOn();
            /*HUMIDITY*/
            //go to prepare operate state
            s_HeaterCtrlState = eHeaterPreOperateState;
        }
            break;
        case eHeaterPreOperateState:
        {
            //Enable Heater controller
            TemperatureController_Enable(true);
            //Set initial value for target temperature (PID) 
            //s_TemperatureCharmberOutTargetPID = 31;
            
            // Added by JP: Enable breathing 
            BreathCircuitTemperatureController_Enable(true);
            /*HUMIDITY*/
            //Enable Humidity controller
            HumidityController_Enable(true);
            /*HUMIDITY*/

            //go to operate state
            s_HeaterCtrlState = eHeaterOperateState;
        }
            break;
        case eHeaterOperateState:
        {
            //Ramp temperature slowly to Temperature setting
            HeaterTask_ChamberOutletTemperatureRampUp();
            HeaterTask_BeathCircuitTemperatureRampUp();
            //do motor operation
            if(HeaterTask_Operate() == false) {
                //an error detected, change state to HandleError
                s_HeaterCtrlState = eHeaterErrorState;
            }
        }
            break;
        case eHeaterStopState:
        {
            s_WarmingUpStatus = eNoneWarmingUp;
            //stop IH Heater PWM
            PWM_IH_Stop();
            //disable HEATER CONTROL 
            IH_ENABLEOff();
            //disable Heater controller
            TemperatureController_Enable(false);
            // Added by JP: Enable breathing 
            BreathCircuitTemperatureController_Enable(false);
            /*HUMIDITY*/
            //disable Humidity controller
            HumidityController_Enable(false);
            //stop Bumper
            Chamber_SetPumpFreq(0);
            
            //turn off chamber
            //CHAMBER_ENABLEOff();
            /*HUMIDITY*/
            
            //go to IDLE state
            s_HeaterCtrlState = eHeaterIdleState;
        }
            break;

        case eHeaterErrorState:
        {
            //stop IH Heater PWM
            PWM_IH_Stop();
            //disable HEATER CONTROL 
            IH_ENABLEOff();
            //disable Heater controller
            TemperatureController_Enable(false);
            // Added by JP: Enable breathing 
            BreathCircuitTemperatureController_Enable(false); 
            /*HUMIDITY*/
            //disable Humidity controller
            HumidityController_Enable(false);
            //stop Bumper
            //PWM_Bumper_Stop();
            //shutdown Bumper
            //PUMB_ENABLEOff();
            Chamber_SetPumpFreq(0);
            
            //turn off chamber
            //CHAMBER_ENABLEOff();
            
            /*HUMIDITY*/
            
            //send event to Alarm task
//            alarmInterface_SendEvent(eHeaterTaskErrorAlarm, eActive, eHighPriority);

            //change state to IDLE
            s_HeaterCtrlState = eHeaterIdleState;
        }
            break;
        default:
            s_HeaterCtrlState = eHeaterIdleState;
            break;
    }
}

/** @brief Handle event sent to HEATER CONTROL task via HEATER CONTROl Queue
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void HeaterTask_HandleEvent(void) {

#define TEMPERATURE_TARGET_OFFSET   13.0  
#define BC_TEMPERATURE_TARGET_OFFSET   2.0  
    
    static float s_currTemp = 0;
    static float s_currFLowrate = 0;
    
    HEATER_CTRL_EVENT_t event;
    //check queue and process data
    while (xQueueReceive(s_HeaterQueue, &event, 0) == pdTRUE) {
        E_TreatmentMode mode = (E_TreatmentMode)setting_Get(eTreatmentModeSettingId);
        switch (s_HeaterCtrlState) {
            case eHeaterIdleState:
            case eHeaterStartState:
            case eHeaterPreOperateState:
            case eHeaterStopState:
            {
                if(event.id == eHeaterStartId) {
                    if((mode == eAdultMode)||(mode == eChildMode)){
                        s_HeaterCtrlState = eHeaterStartState;
                        needToStartRampUpHumidity = true;
                    }
                    else if(mode == eDisinfectionMode){
                        s_TemperatureChamberOutTarget = 87.0;
                    }
                }
                else if (event.id == eHeaterTempId) 
                {
                    if((mode == eAdultMode)||(mode == eChildMode)){
                        float temp = event.fData;
                        if (temp > 37.0) {
                            temp = 37.0;
                        }
                        else if (temp < 31.0) {
                            temp = 31.0;
                        }
                        s_TemperatureSetting = temp;
                        s_TemperatureChamberOutTarget = s_TemperatureSetting + TEMPERATURE_TARGET_OFFSET;
                        s_TemperatureBreathCiruitOutTarget = s_TemperatureSetting + BC_TEMPERATURE_TARGET_OFFSET;//+ (8 - 0.1*s_FlowSetting);
                        //SYS_PRINT("\n s_TemperatureBreathCiruitOutTarget 1 %.2f, s_FlowSetting %.2f\n", s_TemperatureBreathCiruitOutTarget, s_FlowSetting);
                        if(s_currTemp != temp)
                        {
                            s_currTemp = temp;
                            needToStartRampUpHumidity = true;
                            s_TempAndHumiHeatingState = ePreHeating;
                            s_breathCircuitTempRampUpStep = (s_TemperatureBreathCiruitOutTarget - s_TemperatureBreathCiruitOutTargetPID)/(60*1000/200);
                        }
                    }
                    
                }
                else if (event.id == eHeaterFlowRateSettingId)
                {
                    if((mode == eAdultMode)||(mode == eChildMode)){
                        if(s_currFLowrate != event.fData)
                        {
                            s_currFLowrate = event.fData;
                            needToStartRampUpHumidity = true;
                            s_TempAndHumiHeatingState = ePreHeating;
                            s_FlowSetting = event.fData;
                        }
                    }
                }
            }
                break;
            case eHeaterOperateState:
            {
                switch(event.id) {
                    case eHeaterStopId:
                    {
                        s_HeaterCtrlState = eHeaterStopState;
                    }
                        break;
                    case eHeaterTempId:
                    {
                        if((mode == eAdultMode)||(mode == eChildMode)){
                            float temp = event.fData;
                            if (temp > 37.0) {
                                temp = 37.0;
                            } else if (temp < 31.0) {
                                temp = 31.0;
                            }
                            s_TemperatureSetting = temp;
                            s_TemperatureChamberOutTarget = s_TemperatureSetting + TEMPERATURE_TARGET_OFFSET;
                            s_TemperatureBreathCiruitOutTarget = s_TemperatureSetting + BC_TEMPERATURE_TARGET_OFFSET;//  + (8 - 0.1*s_FlowSetting);;
                        //SYS_PRINT("\n s_TemperatureBreathCiruitOutTarget 2 %.2f, s_FlowSetting %.2f \n", s_TemperatureBreathCiruitOutTarget, s_FlowSetting);
                            if(s_currTemp != temp)
                            {
                                s_currTemp = temp;
                                needToStartRampUpHumidity = true;
                                s_TempAndHumiHeatingState = ePreHeating;
                                s_breathCircuitTempRampUpStep = (s_TemperatureBreathCiruitOutTarget - s_TemperatureBreathCiruitOutTargetPID)/(60*1000/200);
                            }
                        }
                    }
                        break;
                        
                    case eHeaterStopWaterPumpId:
                    {                      
                       //This is the event turning off from maintenance mode
                        
                       s_IsWaterPumpOperate = false;//prevent turning on water pump by the others of software
                       //disable Humidity controller
                       HumidityController_Enable(false);
                       //stop Bumper
                       //PWM_Bumper_Stop();
                       //shutdown Bumper
                       Chamber_SetPumpFreq(0);
                       //PUMB_ENABLEOff();
                    }
                    break;
                    
                    case eHeaterStartWaterPumpId:
                    {           
                        //start Bumper PWM signal
                        //PWM_Bumper_Start();
                        //enable Bumper
                        //PUMB_ENABLEOn();
                        //Enable Humidity controller
                        HumidityController_Enable(true);
                        
                        s_IsWaterPumpOperate = true;//allow turning on water pump by the others of software
                    }
                    break;
                    case eHeaterFlowRateSettingId:
                    {
                        if((mode == eAdultMode)||(mode == eChildMode)){
                            if(s_currFLowrate != event.fData)
                            {
                                s_currFLowrate = event.fData;
                                needToStartRampUpHumidity = true;
                                s_TempAndHumiHeatingState = ePreHeating;
                                s_FlowSetting = event.fData;
                            }
                        }
                    }
                    break; 
                    default:
                        break;
                }
            }
                break;
            default:
                break;
        }
    }
}

static void HeaterTask_UpdateWarmingUpStatusToScreen()
{
  
  #define NUM_SAMPLE  25

    static float temperature[NUM_SAMPLE] = {};
    static uint16_t curIndex = 0;
    static uint16_t numSample = 0;
    
    switch (s_WarmingUpStatus)
    {
    case eWarmingUp:
    
        temperature[curIndex] = /*s_TemperatureChamberOut;*/s_TemperatureBreathCiruitOut;

        curIndex++;
        if(numSample < curIndex) numSample = curIndex;
        curIndex = curIndex % NUM_SAMPLE;

        float averageTemp = 0;

        if(numSample == NUM_SAMPLE)
        {   
            uint16_t i;
            for(i = 0; i < NUM_SAMPLE; i++)
            {
                averageTemp = averageTemp + (temperature[i] / NUM_SAMPLE);
            }

            if ((averageTemp > (s_TemperatureBreathCiruitOutTarget/*s_TemperatureChamberOutTarget*/-0.5)) && (averageTemp < (s_TemperatureBreathCiruitOutTarget/*s_TemperatureChamberOutTarget*/+0.5)))              
            {
                s_WarmingUpStatus = eWarmingUpFinished;
                guiInterface_SendEvent(eGuiMainScreenFinishWarmingUp, 0);
                SYS_PRINT("\n warming up finished\n");
            }
        }
        break;
    case eNoneWarmingUp:
        //reset data
        curIndex = 0;
        numSample = 0;
        
    case eWarmingUpFinished:
        break;
        
    default:
        break;
    }
}

/** @brief Function perform Heater Task operation. It shall contain 2 PID controller
 * inside: one for Temperature Control and the other for Humidity Control. Temperature
 * controller maintain the setting temperature while Humidity control adjust Water
 * Bumper to maintain setting Humidity
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 *  @retval true    performing operation good
 *  @retval false   performing operation has some error
 */

bool HeaterTask_Operate(void) {
    
    static float humCtrlSignal = 0;
    float currentPower;
    float targetPower;
    float tempCtrlSignal ;
    
    static bool s_startup = true;
    float envTemp, envHum/*, envPress*/;
    static bool s_TempDone = false;
    //read environment condition
    envTemp = BME280_GetLastsTemperature();
    //SYS_PRINT("envTemp %.1f\n", envTemp);
    envHum = BME280_GetLastsHumidity();
   
    //get chamber outlet temperature
    s_TemperatureChamberOut = Chamber_GetChamberOutTemp();
 
    //get breathing circuit outlet temperature
    s_TemperatureBreathCiruitOut = Chamber_GetBreathingCircuitTemperature();

    
    //get chamber IH core temperature
    s_TemperatureChamberIHCore = Chamber_GetEVTTemp();
    
    //read tank water level
    //s_TankWaterLevel = Chamber_GetTankWaterLevel();
    
    //data filter
    s_TemperatureChamberOut = LPF(&s_chamberOutletTempF, s_TemperatureChamberOut);// s_chamberOutletTempF.clpf = 0.8;
    envTemp = LPF(&s_envTempF, envTemp);
    envHum = LPF(&s_envHumF, envHum);
    
    if(s_startup == true)
    {
        s_startup = false;
        s_chamberOutletTempF.clpf = 0.8;
        s_envTempF.clpf = 0.8;
        s_envHumF.clpf = 0.8;

        //update initial value for ramping up target temperature once time when start up
        envTemp = BME280_GetLastsTemperature();
        if((envTemp != 0)&&(s_TemperatureChamberOut != 0))
        {
            //the target temperature ramp up from environment temperature
            s_TemperatureCharmberOutTargetPID = envTemp;
            s_TemperatureBreathCiruitOutTargetPID = envTemp;
            s_breathCircuitTempRampUpStep = (s_TemperatureBreathCiruitOutTarget - s_TemperatureBreathCiruitOutTargetPID)/(60*1000/200);
        }
    }
    
    //get flow rate from motor data 
    MOTOR_PUBLIC_DATA_t motorData;
    if (MotorTask_GetPublicData(&motorData) == false) {
        //SYS_PRINT("MotorTask_GetPublicData Error\n");
        return false;
    }
    

    
    switch (s_HeaterOperationMode) 
    {
        case eHeaterAdultOrChildOperationMode:
        {

            //calculate current power of IH, for HUMIDITY control
            float currentPower;
            HumidityPower_Measure(&currentPower);
            if (currentPower < 0)
                currentPower = 0;

            //calculate target power , for HUMIDITY control
            float targetAbsHum = HeaterTask_TargetHumidity(s_TemperatureSetting);
            float totalFlow = motorData.measureTotalFlow; //motorData.airFlow + motorData.o2Flow;//edited
            totalFlow = LPF(&s_flowF, totalFlow);
            float targetPower = HumidityPower_Target(targetAbsHum, envTemp, envHum, totalFlow, s_TemperatureCharmberOutTargetPID); //s_TemperatureChamberOut//edited

//            s_TemperatureBreathCiruitOutTarget = s_TemperatureSetting + BC_TEMPERATURE_TARGET_OFFSET + (8 - 0.1*s_FlowSetting);
          //              SYS_PRINT("\n s_TemperatureBreathCiruitOutTarget 1 %.2f, s_FlowSetting %.2f\n", s_TemperatureBreathCiruitOutTarget, s_FlowSetting);

            // calculate IH limit duty
            float supplyVoltage = HumidityPower_GetVoltageSupply();
            float supplyPower = 0;

            float pwUpperLimit = 0; //(targetPower * 0.7) / supplyPower;
            float ihDutyLimit = 0; //100 * sqrt(pwUpperLimit);
            supplyPower = supplyVoltage * supplyVoltage / 3.0;

            static float humCtrlSignal = 0;
            static float humCtrlSignalLimitUpper = 0;
            static float humCtrlSignalLimitLower = 0;
//            SYS_PRINT("\n heating stat: [%d]", s_TempAndHumiHeatingState);
            switch (s_TempAndHumiHeatingState)
            {
                case ePreHeating:

                    //calculate the limit for temperature control during pre-heating state
                    pwUpperLimit = (targetPower * 0.5) / supplyPower;
                    ihDutyLimit = 100 * sqrt(pwUpperLimit);
//                    SYS_PRINT("ihDutyLimit: %.2f ,currentPower: %.2f, targetPower: %.2f, supplyPower : %.2f,  supplyVoltage: %.2f, s_TemperatureChamberOut: %.2f,s_TemperatureChamberIHCore: %.2f\n", ihDutyLimit, currentPower, targetPower, supplyPower, supplyVoltage, s_TemperatureChamberOut, s_TemperatureChamberIHCore);
                    TemperatureController_SetPwUpperLimit(400 * pwUpperLimit);
                    TemperatureController_SetMaxintegral(400 * pwUpperLimit);

                    // checking if pre-heating is finished?
                    if ((s_TemperatureChamberOut > s_TemperatureChamberOutTarget)
                        &&(s_TemperatureChamberIHCore > EVT_PREHEATING_TEMPERATURE))
                    {
                        s_TempDone = true;
                        s_TempAndHumiHeatingState = eAfterPreHeating;
                        HeaterTask_HumidityRampUp(targetPower, currentPower);
                        humCtrlSignal = HeaterTask_HumidityCalcPumpStartValue(s_TemperatureSetting, s_FlowSetting, envTemp, envHum);
                        humCtrlSignalLimitUpper = 2 * humCtrlSignal + 2;
                        humCtrlSignalLimitLower = 0;//0.25 * humCtrlSignal; 
                        HumidityControler_Set_Init(humCtrlSignal, targetPower, currentPower);
                        SYS_PRINT("humCtrlSignal : %.2f, humCtrlSignalLimitLower: %.2f, humCtrlSignalLimitUpper : %.2f \n", humCtrlSignal, humCtrlSignalLimitLower, humCtrlSignalLimitUpper);
                    }
//                    SYS_PRINT("Pre humidifying....\n");

                break;

                case eAfterPreHeating:

                    //calculate the limit for temperature control after pre-heating
                    pwUpperLimit = (targetPower *1.2) / supplyPower;
                    ihDutyLimit = 100 * sqrt(pwUpperLimit);
                    //SYS_PRINT("ihDutyLimit: %.2f ,currentPower: %.2f, targetPower: %.2f, supplyPower : %.2f,  supplyVoltage: %.2f, s_TemperatureChamberOut: %.2f,s_TemperatureChamberIHCore: %.2f\n", ihDutyLimit, currentPower, targetPower, supplyPower, supplyVoltage, s_TemperatureChamberOut, s_TemperatureChamberIHCore);
                    TemperatureController_SetPwUpperLimit(400 * pwUpperLimit);
                    TemperatureController_SetMaxintegral(400 * pwUpperLimit);


                    /*do Humidity PID controller*/

                    static uint8_t humctrl_cnt = 0;
                    static int16_t evtCtrl_cnt = 0;
                    humctrl_cnt ++;
                    evtCtrl_cnt ++;

                    if(humctrl_cnt >= 10)// cycle 2 seconds
                    {
                        HeaterTask_HumidityRampUp(targetPower, currentPower);
                        HumidityControler_SetPumpLimit(humCtrlSignalLimitLower, humCtrlSignalLimitUpper);
                        HumidityController_SetMaxintegral(humCtrlSignalLimitUpper);
                        humCtrlSignal = HumidityController_Operate(currentPower, s_targetPowerRamup, s_TemperatureChamberOut, s_TemperatureCharmberOutTargetPID, envTemp);  
//                        SYS_PRINT("humCtrlSignal : %.2f \n", humCtrlSignal);
                        SYS_PRINT("humCtrlSignal : %.2f, humCtrlSignalLimitLower: %.2f, humCtrlSignalLimitUpper : %.2f, I: %.2f, P: %.2f \n", humCtrlSignal, humCtrlSignalLimitLower, humCtrlSignalLimitUpper, GetIntegralComponent(), GetProportionalComponent());
                        SYS_PRINT("currentPower: %.2f, targetPower: %.2f, s_targetPowerRamup: %.2f, ChamberOutTemp : %.2f,evtTemp: %.2f \n",  currentPower, targetPower, s_targetPowerRamup, s_TemperatureChamberOut, s_TemperatureChamberIHCore);
                        humctrl_cnt = 0;
                    }
                    else
                    {
                        // keep current pump control value
                    }

                    if(evtCtrl_cnt >= 100)// cycle 20 seconds
                    {
                        if(s_TemperatureChamberIHCore < 100)
                        {
                            //chamberTemp_OFFSET = (100 - s_TemperatureChamberIHCore)/2; 
                            s_TemperatureChamberOutTarget += 1; //s_TemperatureSetting + chamberTemp_OFFSET;
                            // SYS_PRINT("Set chamber out temp up to : %.2f\n", s_TemperatureChamberOutTarget);
                        }
                        else if(s_TemperatureChamberIHCore > 160)
                        {
                           // chamberTemp_OFFSET = (s_TemperatureChamberIHCore - 160)/2; 
                            s_TemperatureChamberOutTarget -= 1; //s_TemperatureSetting - chamberTemp_OFFSET;
                            // SYS_PRINT("Set chamber out temp down to : %.2f\n", s_TemperatureChamberOutTarget);
                        }
                        else
                        {

                        }      
                        if(s_TemperatureChamberOutTarget > 60){
                            s_TemperatureChamberOutTarget = 60;
                        }
                        else if(s_TemperatureChamberOutTarget < 42){
                            s_TemperatureChamberOutTarget = 42;
                        }
                        evtCtrl_cnt = 0;

                    }
                    else
                    {

                    }
                    
                    break;

                default:
                    break;
            }


            /*do Heater PID controller*/
            float tempCtrlSignal; //= TemperatureController_Operate(s_TemperatureChamberOut, s_TemperatureCharmberOutTargetPID);
            uint8_t maxPwOffCnt = getMaxNumberOfPwCnt(s_TemperatureChamberIHCore);
            if (maxPwOffCnt == 0) 
            {
                TemperatureController_SetPw(10);
                SYS_PRINT("Over temperature \n");
            } 
            else if (maxPwOffCnt >= 4) 
            {
                //do Heater PID controller
                tempCtrlSignal = TemperatureController_Operate(s_TemperatureChamberOut, s_TemperatureCharmberOutTargetPID);
            } 
            else 
            {
                static uint8_t PwOffCnt = 0;
                PwOffCnt++;
                if (PwOffCnt >= maxPwOffCnt) {
                    TemperatureController_SetPw(10);
                    PwOffCnt = 0;
                    SYS_PRINT("StopIHtemporary \n");
                } else {
                    tempCtrlSignal = TemperatureController_Operate(s_TemperatureChamberOut, s_TemperatureCharmberOutTargetPID);
                }
            }
            //SYS_PRINT("s_TemperatureCharmberOutTargetPID: %.2f\n", s_TemperatureCharmberOutTargetPID);
            // SYS_PRINT("%.2f\n", s_TemperatureCharmberOutTargetPID);


            /*do Breathing circuit PID controller*/
            if (supplyVoltage < 17.0) {
                BcTemperatureController_SetPwUpperLimit(55);
                BcTemperatureController_SetMaxintegral(55);
            } else {
                BcTemperatureController_SetPwUpperLimit(65);
                BcTemperatureController_SetMaxintegral(65);
            }
            
            s_TemperatureBreathCiruitOutTarget = BreathCircuitTemperatureController_CalculateBcOutletTargetTemperature(envTemp, s_TemperatureSetting, s_FlowSetting);
            //SYS_PRINT("\n s_TemperatureBreathCiruitOutTarget %.2f, env Temp %.2f, setting Temp %.2f, setting Flow %.2f\n", s_TemperatureBreathCiruitOutTarget, envTemp, s_TemperatureSetting, s_FlowSetting);
            
            float tempBreathCircuitCtrlSignal = BreathCircuitTemperatureController_Operate(s_TemperatureBreathCiruitOut, s_TemperatureBreathCiruitOutTargetPID);
            //    SYS_PRINT("s_TemperatureBreathCiruitOutTargetPID: %.2f\n", s_TemperatureBreathCiruitOutTargetPID);
            //    SYS_PRINT("ChamberOut: %.2f IHCore: %.2f currentPower %.2f targetPowerRamup %.2f \n", s_TemperatureChamberOut, s_TemperatureChamberIHCore, currentPower, s_targetPowerRamup);

            //SYS_PRINT("targetAbsHum = %.2f, envTemp = %.2f, envHum = %.2f, totalFlow = %.2f, s_TempeCharmbOutTargetPID = %.2f\n", targetAbsHum, envTemp, envHum, totalFlow, s_TemperatureCharmberOutTargetPID);
            //SYS_PRINT("BCTempTarget %.2f;BC Temp: %.2f, tempBCCtrl: %.2f, BCTargetPID: %.2f\n", s_TemperatureBreathCiruitOutTarget, s_TemperatureBreathCiruitOut, tempBreathCircuitCtrlSignal, s_TemperatureBreathCiruitOutTargetPID);

        }
        break;
            
        case eHeaterMoistHeatDisinfectionMode:
        {
            switch (s_DisinfectionPhase)
            {
                case eWarmingUpPhase:
                    
                    tempCtrlSignal = TemperatureController_Operate(s_TemperatureChamberOut, s_TemperatureCharmberOutTargetPID);
                    s_countTime++;
                    if(s_countTime >= 5*60*1000/200)
                    {
                        //go to phase 2
                        s_DisinfectionPhase = eDisinfectPhase;
                        //clear countTime
                        s_countTime = 0;
                    }
                    break;
                    
                case eDisinfectPhase:
                    
                    tempCtrlSignal = TemperatureController_Operate(s_TemperatureChamberOut, s_TemperatureCharmberOutTargetPID);
                    s_countTime++;
                    if(s_countTime >= 10*60*1000/200)
                    {
                        //go to phase 3
                        s_DisinfectionPhase = eToDryPhase;
                        //clear countTime
                        s_countTime = 0;
                    }
                    break;
                case eToDryPhase:
                    
                    tempCtrlSignal = TemperatureController_Operate(s_TemperatureChamberOut, s_TemperatureCharmberOutTargetPID);
                    s_countTime++;
                    if(s_countTime <= 2*60*1000/200)
                    {

                    }
                    else if(s_countTime <= 5*60*1000/200)
                    {
                        
                    }
                    else
                    {
                        //go to finished state
                        s_DisinfectionPhase = eDisinfectionFinished;
                        //clear countTime
                        s_countTime = 0;
                    }
                    break;
                    
                case eDisinfectionFinished:
                    
                    break;
                default:
                    break;
            }
                                    
        }
            break;
            
        case eDryHeatSterilizationMode:
        {
            switch (s_DisinfectionPhase)
            {
                case eWarmingUpPhase:
                    
                    tempCtrlSignal = TemperatureController_Operate(s_TemperatureChamberOut, s_TemperatureCharmberOutTargetPID);
                    s_countTime++;
                    if(s_countTime >= 5*60*1000/200)
                    {
                        //go to phase 2
                        s_DisinfectionPhase = eDisinfectPhase;
                        //clear countTime
                        s_countTime = 0;
                    }
                    break;
                    
                case eDisinfectPhase:
                    
                    tempCtrlSignal = TemperatureController_Operate(s_TemperatureChamberOut, s_TemperatureCharmberOutTargetPID);
                    s_countTime++;
                    if(s_countTime >= 10*60*1000/200)
                    {
                        //go to phase 3
                        s_DisinfectionPhase = eToDryPhase;
                        //clear countTime
                        s_countTime = 0;
                    }
                    break;
                case eToDryPhase:
                    
                    tempCtrlSignal = TemperatureController_Operate(s_TemperatureChamberOut, s_TemperatureCharmberOutTargetPID);
                    s_countTime++;
                    if(s_countTime <= 2*60*1000/200)
                    {

                    }
                    else if(s_countTime <= 5*60*1000/200)
                    {
                        
                    }
                    else
                    {
                        //go to finished state
                        s_DisinfectionPhase = eDisinfectionFinished;
                        //clear countTime
                        s_countTime = 0;
                    }
                    break;
                    
                case eDisinfectionFinished:
                    
                    break;
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
    
    
    //update public data
    if (xSemaphoreTake(s_HeaterDataMutex, HEATER_MUTEX_MAX_WAIT) == pdTRUE) {
        s_HeaterPublicData.setTemp = s_TemperatureSetting;//s_TemperatureCharmberOutTargetPID;
        s_HeaterPublicData.chamberOutTargetTemp = s_TemperatureCharmberOutTargetPID;
        s_HeaterPublicData.chamberOutTemp = s_TemperatureChamberOut;
        s_HeaterPublicData.breathCircuitOutTargetTemp = s_TemperatureBreathCiruitOutTargetPID;
        s_HeaterPublicData.breathCircuitOutTemp = s_TemperatureBreathCiruitOut;        
        s_HeaterPublicData.IHcoreTemp = 0;  //not implemented yet
        s_HeaterPublicData.powerOut = tempCtrlSignal;
        s_HeaterPublicData.currentPower = currentPower;
        s_HeaterPublicData.targetPower = targetPower;
        s_HeaterPublicData.bumperFrequency = humCtrlSignal;
        s_HeaterPublicData.envTemp = envTemp;
        s_HeaterPublicData.envHum = envHum;
        s_HeaterPublicData.warmingUpState = s_WarmingUpStatus;
        //release semaphore
        xSemaphoreGive(s_HeaterDataMutex);
    }

    HeaterTask_UpdateWarmingUpStatusToScreen();

    return true;
}

/** @brief Obtain target absolute humidity from temperature setting
 *  @param [in]     None
 *  @param [out]    None
 *  @return float   target absolute Humidity for Humidity control
 */
float HeaterTask_TargetHumidity(float tempSetting) {
    float targetAbsHumidity = 28.83; //19.22;//;
    uint8_t intTempSetting = (uint8_t) (tempSetting);
    switch (intTempSetting) {
            /*   Humidity ~ 70%    */
        /*
        case 31: targetAbsHumidity = 22.22;
            break;
        case 32: targetAbsHumidity = 23.23;
            break;
        case 33: targetAbsHumidity = 24.38;
            break;
        case 34: targetAbsHumidity = 26.54;
            break;
        case 35: targetAbsHumidity = 26.70;
            break;
        case 36: targetAbsHumidity = 27.84;
            break;
        case 37: targetAbsHumidity = 28.99;
            break;
         */ 
         
             case 31: targetAbsHumidity = 28.83;
                 break;
             case 32: targetAbsHumidity = 30.42;
                 break;
             case 33: targetAbsHumidity = 32.10;
                 break;
             case 34: targetAbsHumidity = 33.81;
                 break;
             case 35: targetAbsHumidity = 35.60;
                 break;
             case 36: targetAbsHumidity = 37.50;
                 break;
             case 37: targetAbsHumidity = 39.50;
                 break;
        default: break;

    }
    return targetAbsHumidity;
}

static void HeaterTask_ChamberOutletTemperatureRampUp() {
#define RAMP_UP_FLOW_STEP    (0.1)   //0.1 LPM
#define RAMP_UP_TIME_STEP    (200)   //100 MS
    static TickType_t lastTimeTick = 0;
    //check condition to see need to ramp up or not
    if (s_TemperatureCharmberOutTargetPID >= s_TemperatureChamberOutTarget) {
        s_TemperatureCharmberOutTargetPID = s_TemperatureChamberOutTarget;
        return;
    }

    TickType_t currentTimeTick = xTaskGetTickCount();
    if ((currentTimeTick - lastTimeTick) >= RAMP_UP_TIME_STEP) {
        //        SYS_PRINT("\n RAMP: %.2f", s_MotorOperatingFlow);
        s_TemperatureCharmberOutTargetPID += RAMP_UP_FLOW_STEP;
        lastTimeTick = currentTimeTick;
    }

}

static void HeaterTask_BeathCircuitTemperatureRampUp() {
#define RAMP_UP_BC_TEMP_STEP    (0.025)   //0.1 LPM
#define RAMP_UP_TIME_STEP    (200)   //100 MS
    static TickType_t lastTimeTick = 0;
    float rampUpStep = RAMP_UP_BC_TEMP_STEP;
    if (s_breathCircuitTempRampUpStep > 0) {
        rampUpStep = s_breathCircuitTempRampUpStep;
    }
    //check condition to see need to ramp up or not
    if (s_TemperatureBreathCiruitOutTargetPID >= s_TemperatureBreathCiruitOutTarget) {
        s_TemperatureBreathCiruitOutTargetPID = s_TemperatureBreathCiruitOutTarget;
        return;
    }

    TickType_t currentTimeTick = xTaskGetTickCount();
    if ((currentTimeTick - lastTimeTick) >= RAMP_UP_TIME_STEP) {
        s_TemperatureBreathCiruitOutTargetPID += rampUpStep;
        lastTimeTick = currentTimeTick;
    }

}

static void HeaterTask_HumidityRampUp(float target, float current) {
#define HUM_RAMP_UP_BC_TEMP_STEP    (1)   // 0.1 Wat per step
#define HUM_RAMP_UP_TIME_STEP       (100)   //100 ms for 1 step
    static TickType_t lastTimeTick = 0;

    TickType_t currentTimeTick = xTaskGetTickCount();
    if ((currentTimeTick - lastTimeTick) >= HUM_RAMP_UP_TIME_STEP) {
        s_targetPowerRamup += HUM_RAMP_UP_BC_TEMP_STEP;
        lastTimeTick = currentTimeTick;
    }

    if (s_targetPowerRamup < current)
    {
        s_targetPowerRamup = current;
    }
    if (s_targetPowerRamup >= target)
    {
        s_targetPowerRamup = target;
    }

}

//bool IsHeaterTask_HumidityRampUp_Done() {
//#define RAMP_UP_HUMIDITY_TIME  (120000) // 1min
//    static bool HumidityRampUp_Done_Flag = false;
//    static TickType_t startRampUpTime = 0;
//    TickType_t currentTimeTick;
//    if (needToStartRampUpHumidity == true) {
//        startRampUpTime = xTaskGetTickCount();
//        needToStartRampUpHumidity = false;
//        HumidityRampUp_Done_Flag = false;
//        SYS_PRINT("\nStart RampUp Humidity \n");
//    } else {
//        currentTimeTick = xTaskGetTickCount();
//        if (HumidityRampUp_Done_Flag == false) {
//            if ((currentTimeTick - startRampUpTime) >= RAMP_UP_HUMIDITY_TIME) {
//                HumidityRampUp_Done_Flag = true; // 
//                SYS_PRINT("\nRampUp Humidity is finished \n");
//                //float pumpFreq = HeaterTask_HumidityCalcPumpStartValue(s_TemperatureSetting, s_FlowSetting, );
//                //HumidityControler_Set_Init(pumpFreq, s_HeaterPublicData.currentPower, s_HeaterPublicData.targetPower);
//            }
//        }
//    }
//    return HumidityRampUp_Done_Flag;
//}

float HeaterTask_HumidityCalcPumpStartValue(float TemperatureSetting, float FlowSetting, float envTemp, float envHum) {
    static float pumpCtrlFreq = 0;
    float envAbsHum = 0;
    envAbsHum = HumidityPower_EnvAbsHumidity(envTemp, envHum);
    float targetHum = HeaterTask_TargetHumidity(TemperatureSetting);
    pumpCtrlFreq = (FlowSetting / 150) *( targetHum - envAbsHum );
    SYS_PRINT("\nenviHum %.2f \n", envAbsHum);
    /*    switch ((int) TemperatureSetting) {
        case 31:
            pumpCtrlFreq = 0.68 * (FlowSetting + 0) / 2;
            break;
        case 32:
            pumpCtrlFreq = 0.7 * (FlowSetting + 0) / 2;
            break;
        case 33:
            pumpCtrlFreq = 0.72 * (FlowSetting + 0) / 2;
            break;
        case 34:
            pumpCtrlFreq = 0.74 * (FlowSetting + 0) / 2;
            break;
        case 35:
            pumpCtrlFreq = 0.76 * (FlowSetting + 0) / 2;
            break;
        case 36:
            pumpCtrlFreq = 0.78 * (FlowSetting + 0) / 2;
            break;
        case 37:
            pumpCtrlFreq = 0.8 * (FlowSetting + 0) / 2;
            break;
        default:
            pumpCtrlFreq = 0;
            break;
    }
     */
    return pumpCtrlFreq;
}

void HeaterTask_TurnOnWaterPump(void) {
    if (s_IsWaterPumpOperate == true) {

        //start Bumper PWM signal
        //PWM_Bumper_Start();
        //enable Bumper
        //PUMB_ENABLEOn();
        //Enable Humidity controller
        HumidityController_Enable(true);
    }
}

void HeaterTask_TurnOffWaterPump(void) {
    //disable Humidity controller
    HumidityController_Enable(false);
    //stop Bumper
    //PWM_Bumper_Stop();
    Chamber_SetPumpFreq(0);
    //shutdown Bumper
    //PUMB_ENABLEOff();
}

void HeaterTask_SetState(E_HeaterState state) {
    s_HeaterCtrlState = state;
}

int8_t getMaxNumberOfPwCnt(float temp) {
    int8_t maxCnt = 0;
    float diff = calcEvtTempDiff(temp);
    if (temp < 180) {
        maxCnt = 4;
    } else if (temp < 185) {
        maxCnt = 3;
    } else if (temp < 190) {
        maxCnt = 2;
    } else if (temp < 195) {
        maxCnt = 1;
    } else {
        maxCnt = 0;
    }
    if (diff > 10) // temperature raise rapidly
    {
        maxCnt = 1;
        SYS_PRINT("temperature raise rapidly \n");
    }
    return maxCnt;
}

bool getEvtR(float *evtR) 
{
#define EVT_R_MEAS_TIME       (1000)   //1000 ms 
    static TickType_t lastTimeTick = 0;
    static bool start = true;
    if (start == true) 
    {
        lastTimeTick = xTaskGetTickCount();
        start = false;
        return false;
    } 
    else 
    {
        static float temp = 0;
        TickType_t currentTimeTick = 0;
        currentTimeTick = xTaskGetTickCount();
        if ((currentTimeTick - lastTimeTick) >= EVT_R_MEAS_TIME) 
        {
            HumiditiPower_GetevtR(&temp);
            *evtR = temp;
            return true;
        } 
        else
            return false;
    }
}

float calcEvtTempDiff(float currTemp) {
    static bool start = true;
    static float prevTemp = 0;

    if (start == true) {
        prevTemp = currTemp;
        start = false;
    }
    float diff = currTemp - prevTemp;
    prevTemp = currTemp;
    // SYS_PRINT("Evt Temp Diff: %.2f \n", diff);
    return (diff);
}
/* *****************************************************************************
 End of File
 */
