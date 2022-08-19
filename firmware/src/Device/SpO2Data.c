/* ************************************************************************** */
/** @file [SpO2Data.c]
 *  @brief {This file contain source code necessary for connecting with SpO2 sensor and collecting the Pt data }
 *  @author {Le Cuong}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "ApplicationDefinition.h"
#include "AlarmMgr.h"
#include "AlarmInterface.h"
#include "system_config.h"
#include "system_definitions.h"
#include "PtDataTask.h"
#include "UART_4.h"
#include "ADC.h"
#include "SpO2Data.h"


#define SPO2_SYNCRO_BIT             0x80
#define SPO2_NORMAL_DATA            0x40
#define SPO2_INDEX_DONE_CYCLE       100
#define SPO2_SLOWDATA_ADDRESS       0xe
#define SPO2_BAR_GRAPH              0xf

/** @brief  Maximum time to wait for MUTEX to get public data */
#define 	SPO2_MUTEX_MAX_WAIT_MS		(2 / portTICK_PERIOD_MS)         //2ms

/** @brief  Cycle time of SPO2 task */
#define 	SPO2_TASK_DELAY_PERIOD_MS		(20 / portTICK_PERIOD_MS)     //20ms

/** @brief  Cycle time of SPO2 task */
#define 	SPO2_NOT_SEND_DATA_COUNT		(5000 / SPO2_TASK_DELAY_PERIOD_MS) //3000ms


extern volatile ALARM_MONITOR_t stAlarmMonitor;

static SP02_DATA_t stSpO2Data;

/** @brief software revision of ChipO2/ChipO2PM */
static char s_spO2rev[5];    

/** @brief setting to one when beep signal is received; 
 * is cleared by the customer's sound routine. The same variable can 
 * be used to display heart symbol */
static bool s_spO2Beep;                                         


static uint8_t s_plenthWave; 

/** @brief real time bar graph value in the range 0..15. Value 0 is indicated 
 * when finger is removed*/
static uint8_t s_spO2Bargraph;                

/** @brief SpO2 value. When this value equals to 127, customer's program must
 * display dashes '--'*/
static uint8_t s_spO2AveValue = 127;                          

/** @brief Pulse Rate value. When this value equals to 255, customer's program
 * must display dashes '---'*/
static uint8_t s_spO2PulseRate;                          

/** @brief signal strength in the range of 0 to 8, indicates perfusion level */
static uint8_t s_spO2Signal;                     

/** @brief flags byte. Its value is shown below:
 * 0 - no alerts, customer's program should erase message
 * 1 - sensor unplugged
 * 2 - no finger in sensor, or sensor problem
 * 3 - searching for pulse
 * 4 ? searching too long
 * 5 - lost pulse
 * 6 - artifact (applicable to ChipO2PM)*/
static uint8_t s_spO2Flags;                       

/** @brief Instantaneous SpO2 value after each pulse, this value is updated 
 * with each pulse, and is sent only once. All other time it is zero. 
 * This value is used for arbitrary SpO2 averager */
static uint8_t s_spO2InsValue;                        

/** @brief These are red and infra red gain indexes, they are used during production tests */
static uint8_t s_spO2Rgain, s_spO2Igain;             
                             
/** @brief MUTEX to protect SPO2 Data  sharing */
static SemaphoreHandle_t s_SpO2DataMutex = NULL;

/** @brief Flag indicate SPO2 module has error */
static E_DeviceErrorState s_SPO2Error = eDeviceNoError;


/** @brief Initialize SpO2 Data
 * This function should be called 1 times at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void SpO2Data_Inititalize(void) 
{
    //create MUTEX
    s_SpO2DataMutex = xSemaphoreCreateMutex();
    xSemaphoreGive(s_SpO2DataMutex);
}
/** @brief Parsing the buffer data which be sent by sensor
 * the data after received will be parsing to get the information.
 *  @param [in]     uint8_t receiveByte: a byte in the buffer
 *  @param [out]    None
 *  @return None
 */
void SpO2Data_Parse(uint8_t receiveByte)
{
    static int i = 100;
    static int address, msrate;
    static int beep_old = false;

    // catch syncro bit
    if ( receiveByte & SPO2_SYNCRO_BIT )
    {
        i = 0;
    }
    switch(i)
    {
        case 0: // revison or normal data ?
            if ( receiveByte & SPO2_NORMAL_DATA )
            {
                i=4;
            }
            else
            {
                if ( receiveByte & 1 )
                {
                // acoustic beep indicator
                    if ( !beep_old )
                    {
                        s_spO2Beep = true;
                    }
                    beep_old = true;
                }
                else 
                    beep_old = false;

                address = receiveByte & SPO2_SLOWDATA_ADDRESS;
                i = 1;
            }
            break;

        case 1: // second data byte

            // pleth
            //DrawWaveform( a );
            s_plenthWave = receiveByte;
            //SYS_PRINT("%f\n", (float)receiveByte);
            i = 2;
            break;

        case 2: // third data byte
            // bargraph
            s_spO2Bargraph = receiveByte & SPO2_BAR_GRAPH;

            // msbit of the rate
            msrate = receiveByte & SPO2_NORMAL_DATA;

            i = 3;
            break;

        case 3: // forth data byte
            switch ( address )
            {
                case 0: // SpO2
                    s_spO2AveValue = receiveByte;
//                    SYS_PRINT("AveValue = %d \n", s_spO2AveValue);
                    
                    break;

                case 2: // Rate
                    if ( msrate )
                    {
                        s_spO2PulseRate = receiveByte | SPO2_SYNCRO_BIT;
//                        SYS_PRINT("PulseRate = %d \n", s_spO2PulseRate);
                    }
                    else 
                    {
                        s_spO2PulseRate = receiveByte;
//                        SYS_PRINT("PulseRate = %d \n", s_spO2PulseRate);
                    }
                    break;

                case 4: // Signal level
                    s_spO2Signal = receiveByte;
//                    SYS_PRINT("signal = %d \n", receiveByte);
                    break;

                 case 6: // Flags
                    s_spO2Flags = receiveByte;
//                    SYS_PRINT("flags = %d \n", receiveByte);
                    break;

                case 8: // Instantaneous SpO2
                    s_spO2InsValue = receiveByte;
//                    SYS_PRINT("o2r = %d \n", receiveByte);
//                    SYS_PRINT("\n");
                    break;

                case 10: // rgain
                    s_spO2Rgain = receiveByte;
//                    SYS_PRINT("rgain = %d \n", receiveByte);
                    break;

                case 12: // igain
                    s_spO2Igain = receiveByte;
//                    SYS_PRINT("igain = %d \n", receiveByte);
                    break;

                default: break;
            }

            i = 100;
            break;

        case 4: // first byte of revision number
            s_spO2rev[0] = receiveByte;
            s_spO2rev[1] = '.';
            i = 5;
            break;

        case 5: // second byte of revision number
            s_spO2rev[2] = receiveByte;
            i = 6;
            break;

        case 6: // second byte of revision number
            s_spO2rev[3] = receiveByte;
            s_spO2rev[4] = 0;
            // display revision here....
            i = 100;
            break;

        default: break;
      }
}

/** @brief Update the newest information to the SpO2 struct
 * After parsing the data, the needest info will be update (depend on the step time of task) 
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void SpO2Data_Update(void)
{
//    SYS_PRINT("\n----------------------------\n");
//    SYS_PRINT("\n s_spO2AveValue = %d", s_spO2AveValue);
//    SYS_PRINT("\n s_spO2Bargraph = %d", s_spO2Bargraph);
//    SYS_PRINT("\n s_spO2Flags = %d", s_spO2Flags);
//    SYS_PRINT("\n s_spO2InsValue = %d", s_spO2InsValue);
//    SYS_PRINT("\n s_spO2PulseRate = %d", s_spO2PulseRate);
//    SYS_PRINT("\n s_spO2Signal = %d", s_spO2Signal);
//    SYS_PRINT("\n s_spO2Beep = %d", s_spO2Beep);
//    SYS_PRINT("\n s_plenthWave = %d", s_plenthWave);
    
    if (xSemaphoreTake(s_SpO2DataMutex, SPO2_MUTEX_MAX_WAIT_MS) == pdTRUE) 
    {
        //copy data
        stSpO2Data.aveValue = s_spO2AveValue;
        stSpO2Data.bargraph = s_spO2Bargraph;
        stSpO2Data.flags = s_spO2Flags;
        stSpO2Data.insValue = s_spO2InsValue;
        stSpO2Data.pulseRate = s_spO2PulseRate;
        stSpO2Data.signal = s_spO2Signal;
        stSpO2Data.beep = s_spO2Beep; s_spO2Beep = 0;//clear beep after getting
        stSpO2Data.plenthWave = s_plenthWave;
        
//        if (s_plenthWave != 50)
//        {
//            SYS_PRINT("\n s_plenthWave = %d", s_plenthWave);
//        }
        
        //release semaphore
        xSemaphoreGive(s_SpO2DataMutex);
    }    
    else
    {
        SYS_PRINT("Failed to update SpO2 data \n");
    }
}

/** @brief Getting Spo2 value 
 * This function is used if the developer want to get the SpO2 information in all project.
 *  @param [in]     None
 *  @return SP02_DATA_t The SpO2 information 
 */
SP02_DATA_t SpO2Data_ReadData(void)
{
    uint8_t data[100] = {};
    //static uint8_t txData[] = "\n hello UART ";
    static int s_counterErr = 0;

    //Uart4_Send(&txData[0], sizeof (txData));
    //memset(&data[0], 0, 100);
    int8_t numByte = Uart4_ReadReceiveBuffer(&data[0], 100);
    //SYS_PRINT("numByte = %d \n", numByte);
    
    if (numByte > 2) {
        int k = 0;
        for(k; k < numByte; k++)
        {
           SpO2Data_Parse(data[k]);
        } 
    }
    else
    {
        s_spO2Flags = 1;
    }
        
    SpO2Data_Update();
    
    if(numByte <= 0)
    {
        s_counterErr++;
        if(s_counterErr >= SPO2_NOT_SEND_DATA_COUNT)
        {
            s_SPO2Error = eDeviceErrorDetected;
        }
    }
    else
    {
        s_counterErr = 0;
        s_SPO2Error = eDeviceNoError;
    }
    
    return stSpO2Data;
}

/** @brief Updating the states of SpO2 sensor 
 *  It updates the state for the Alarm Task to find the error 
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
//void SpO2Data_UpdateState(void)
//{
//    stAlarmMonitor.spo2Info_struct.spo2Value = s_spO2AveValue;
////    stAlarmMonitor.spo2Info_struct.spo2SetLimit = 50;
//    stAlarmMonitor.spo2Info_struct.spo2Message = s_spO2Flags;
//    if(s_spO2Flags == 1)
//    {
//        stAlarmMonitor.spo2Info_struct.spo2Connected = Flag_Off;
//    }
//    else
//    {
//       stAlarmMonitor.spo2Info_struct.spo2Connected = Flag_On; 
//    }
//}

/** @brief The interface get SpO2 data
 *  @param [in]     SP02_DATA_t* data pointer points to storage place
 *  @param [out]    None
 *  @return None
 */
void SpO2Data_GetData(SP02_DATA_t* data)
{
    if (xSemaphoreTake(s_SpO2DataMutex, SPO2_MUTEX_MAX_WAIT_MS) == pdTRUE) 
    {
        //copy data
        *data = stSpO2Data;
        xSemaphoreGive(s_SpO2DataMutex);
    }
}

uint8_t SPO2Data_GetPlenthValue()
{
  return s_plenthWave;
}

bool SpO2Data_IsMonitoringSpO2Patient(void) {
    bool rtn = true;
    
    if (s_spO2AveValue == 127)
        rtn = false;
    
    return rtn;   
}

bool SpO2Data_IsMonitoringPRPatient(void) {
    bool rtn = true;
    
    if (s_spO2PulseRate == 255)
        rtn = false;
    
    return rtn;    
}

/** @brief Query any error happen with SPO2 module
 *  @param [in]  None   
 *  @param [out] None
 *  @retval true SPO2 module has error
 *  @retval false SPO2 module is OK
 */
bool SpO2Data_IsModuleFailed() {
    if (s_SPO2Error == eDeviceNoError) {
        return false;
    }
    else 
    {
        return true;
    }
}
