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
#include "FreeRTOS.h"
#include <stdbool.h>
#include "Audio.h"
#include "I2C_2.h"
#include "ApplicationDefinition.h"
#include "AlarmInterface.h"
#include "timers.h"
#include "task.h"
#include "queue.h"
#include "GuiInterface.h"
#include "AudioDefine.h"

/** @brief define state machine for Play alarm sound */
typedef enum {
    
    /**< Idle state. In this state, Motor has been turned off, there are no 
     * control here */
    eIdleState = 0,
           
    ePlayHighAlarmState,
 
    ePlayMediumAlarmState,

    ePlayLow1AlarmState,
            
    ePlayStartupSoundState,
            
    eMuteState,
    
            
} E_PlayAlarmState;





#define TLV320_I2S_CLOCK        8*TLV320_I2S_BAUDRATE
#define TLV320_I2S_BAUDRATE     44100

#define TLV320_I2C_INDEX        DRV_I2C_INDEX_0
#define TLV320_I2S_INDEX        DRV_I2S_INDEX_0


/** @brief I2C slave address of the AUDIO */
#define AUDIO_BASE_ADDR       (0x18)

/** @brief I2C address while writing data to AUDIO
 * write address = base address << 1 + write bit (0))
 */
#define AUDIO_WRITE_ADDR      (AUDIO_BASE_ADDR << 1)

/** @brief I2C address while reading data from AUDIO
 * read address = base address << 1 + read bit(1))
 */
#define AUDIO_READ_ADDR       ((AUDIO_BASE_ADDR << 1) + 1)

/** @brief max time wait for AUDIO communication (in ms) via I2C*/
#define AUDIO_MAX_WAIT_MS      (10/*50*/)

#define HIGH_ALARM_BUZZER_ON_10MS       1
#define HIGH_ALARM_BUZZER_ON_250MS      25
#define HIGH_ALARM_BUZZER_ON_500MS      50
#define HIGH_ALARM_BUZZER_ON_1000MS     100
#define HIGH_ALARM_BUZZER_ON_1250MS     125
#define HIGH_ALARM_BUZZER_ON_2000MS     200
#define HIGH_ALARM_BUZZER_ON_2250MS     225
#define HIGH_ALARM_BUZZER_ON_2500MS     250
#define HIGH_ALARM_BUZZER_ON_3000MS     300
#define HIGH_ALARM_BUZZER_ON_3250MS     325
#define HIGH_ALARM_BUZZER_ON_4000MS     400
#define HIGH_ALARM_BUZZER_ON_10000MS    1000

#define MEDIUM_ALARM_BUZZER_ON_10MS     1
#define MEDIUM_ALARM_BUZZER_ON_400MS    40
#define MEDIUM_ALARM_BUZZER_ON_800MS    80
#define MEDIUM_ALARM_BUZZER_ON_1000MS   100
#define MEDIUM_ALARM_BUZZER_ON_25000MS  2500

#define LOW_ALARM_BUZZER_ON_10MS        1
#define LOW_ALARM_BUZZER_ON_450MS       45
#define LOW_ALARM_BUZZER_ON_650MS       65
#define LOW_ALARM_BUZZER_ON_30000MS     3000

#define POWER_LOST_ALARM_BUZZER_ON_10MS     1
#define POWER_LOST_ALARM_BUZZER_ON_400MS    40
#define POWER_LOST_ALARM_BUZZER_ON_800MS    80
#define POWER_LOST_ALARM_BUZZER_ON_1000MS   100
#define POWER_LOST_ALARM_BUZZER_ON_4000MS   400






#define TIMER_PLAY_SOUND_ID			0
#define TIMER_MUTE_ALARM_ID			1

/** @brief MOTOR CONTROL task periodic */
#define TIMER_PLAY_SOUND_PERIODIC_MS      (50)

#define TICK_TO_WAIT        (10)

#define TIME_COUNT_0MS       0

#define TIME_COUNT_100MS      (100/TIMER_PLAY_SOUND_PERIODIC_MS - 1)

#define TIME_COUNT_250MS     (250/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_500MS     (500/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_750MS     (500/TIMER_PLAY_SOUND_PERIODIC_MS - 1)

#define TIME_COUNT_1000MS    (1000/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_1250MS    (1250/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_1500MS    (1500/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_1750MS    (1750/TIMER_PLAY_SOUND_PERIODIC_MS - 1)

#define TIME_COUNT_2000MS    (2000/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_2250MS    (2250/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_2500MS    (2500/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_2750MS    (2750/TIMER_PLAY_SOUND_PERIODIC_MS - 1)

#define TIME_COUNT_3000MS    (3000/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_3250MS    (3250/TIMER_PLAY_SOUND_PERIODIC_MS - 1)

#define TIME_COUNT_10000MS   (10000/TIMER_PLAY_SOUND_PERIODIC_MS - 1)


#define TIME_COUNT_400MS     (400/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_800MS     (800/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_25000MS   (25000/TIMER_PLAY_SOUND_PERIODIC_MS - 1)

#define TIME_COUNT_450MS     (450/TIMER_PLAY_SOUND_PERIODIC_MS - 1)
#define TIME_COUNT_30000MS   (30000/TIMER_PLAY_SOUND_PERIODIC_MS - 1)




/** @brief Declare gain offset match to the volume (dB) specified in sofware specification,
*It is necessary to choose the appropriate value for the volume at level 10 to reach 74dB for High Alarm */
#define GAIN_OFFSET                     (30) //-5dB TODO 

/** @brief Declare gain offset for high priority alarm*/
#define GAIN_OFFSET_FOR_HIGH_ALARM      (-20)

extern unsigned char audioSquareWave260ms_Medium[];
extern unsigned char audioSquareWave260ms_Low[];
extern unsigned char audioSquareWave210ms_High[];

extern unsigned char audioSquareWave100ms[];
extern unsigned char audioVoiceSync[];
//extern unsigned char audioVoiceStart[];


/** @brief Variable to hole play alarm sound status, useful for state machine of 
 * controller*/
static E_PlayAlarmState gs_CurrentPlayState = eIdleState;

/** @brief Variable to hole play alarm sound status, useful for state machine of 
 * controller*/
static E_PlayAlarmState gs_NewPlayState = eIdleState;

/** @brief Variable to hole play alarm sound status before mute, useful for state machine of 
 * controller*/
static E_PlayAlarmState gs_PlayStateBeforeMute = eIdleState;

static TimerHandle_t gs_TimerPlaySound;
static TimerHandle_t gs_TimerMuteAlarm;

/** @brief Flag indicate I2C4 has error
 * true mean error happened
 * false mean no error */
static E_DeviceErrorState s_AudioError = eDeviceNoError;

//i2s

static DRV_HANDLE s_TLV320HandleI2S = DRV_HANDLE_INVALID;
static DRV_I2S_BUFFER_HANDLE s_TLV320BufferHandleI2S;
static uint16_t s_TickCount = 0;
static uint16_t s_ttickPower = 0;
static uint16_t s_ttickMute = 0;



/** @brief local functions */
static bool Audio_ConfigureRegister(uint8_t regAddress, uint8_t regValue);
static void Audio_ReportError();
static bool Audio_Configure();
static void Audio_SetVolume(E_AudioCommand cmdId, uint16_t value);
void Audio_CreateSoftwareTimer();
void Audio_HandleStartupSound(void);
//function to initialize Audio, include configuring Audio IC and Initialize I2S
bool Audio_Initialize() {
    Audio_Configure();
    Audio_CreateSoftwareTimer();
//    Audio_PlayHighAlarm();
//    
//    vTaskDelay(100);
//    Audio_PlayMediumAlarm();
//    Audio_PlayLowAlarm();

    return true;
}


/******************************************************************************/
/* I2S common function definition *********************************************/
/**
 * @brief I2S data transmit interrupt service routine
 * @param (in) event
 * @param (in) bufferHandle
 * @param (in) contextHandle
 * @reutnr None
 */
void i2s_TLV320StatusCallback (DRV_I2S_BUFFER_EVENT event,
        DRV_I2S_BUFFER_HANDLE bufferHandle, uintptr_t contextHandle) {
    switch (event) {
        case DRV_I2S_BUFFER_EVENT_COMPLETE:
            //tlv320_audioConstDataSend();
            //BSP_LED_3Toggle();
            break;
        case DRV_I2S_BUFFER_EVENT_ERROR:
            break;
        case DRV_I2S_BUFFER_EVENT_ABORT:
            break;
        default:
            break;
    }
}

/**
 * @brief Setup I2S driver, specify callback function and driver parameter
 * @param None
 * @return None 
 */
static void _i2s_TLV320Setup (void) {
    /// Check valid driver handle
    if (s_TLV320HandleI2S == DRV_HANDLE_INVALID) return;
    /// Set up callback function
    DRV_I2S_BufferEventHandlerSet(s_TLV320HandleI2S,
            i2s_TLV320StatusCallback, NULL);
    
    /// Transmit/receive error ignore
    DRV_I2S_TransmitErrorIgnore(s_TLV320HandleI2S, true);
    DRV_I2S_ReceiveErrorIgnore(s_TLV320HandleI2S, true);
    
    /// Set up baud rate
    DRV_I2S_BaudSet(s_TLV320HandleI2S, TLV320_I2S_CLOCK, TLV320_I2S_BAUDRATE);
}

/**
 * @brief Initialize audio I2C driver
 * @param None
 * @return Initialize driver status
 */
bool _i2s_TLV320Init (void) {
    s_TLV320HandleI2S = DRV_I2S_Open(TLV320_I2S_INDEX, 
            DRV_IO_INTENT_WRITE | DRV_IO_INTENT_NONBLOCKING);
    if (s_TLV320HandleI2S == DRV_HANDLE_INVALID) return false;
    
    /// Setup i2c driver callback when handle valid
    _i2s_TLV320Setup();
    return true;
}

/**
 * @brief Low layer i2s data transmit function
 * @param audioBuffer Pointer to data buffer
 * @param size Size of data in byte
 */
void _i2s_TLV320DataTransmit(uint8_t *audioBuffer, size_t size) {
    /// Check if i2s driver handle has been acquired or not
    if (s_TLV320HandleI2S == DRV_HANDLE_INVALID) {
        SYS_PRINT("Driver I2S have not opened yet\n");
        return;
    }
    DRV_I2S_BufferAddWrite(s_TLV320HandleI2S, &s_TLV320BufferHandleI2S,
            audioBuffer, size);
    // Test
    if (s_TLV320BufferHandleI2S == DRV_I2S_BUFFER_HANDLE_INVALID) {
        SYS_PRINT("Cannot schedule transmit i2s data");
    }
}



static bool Audio_Configure() {
  
    Audio_ConfigureRegister(0x00,0x00);// Set register page to 0

    Audio_ConfigureRegister(0x01,0x01);// Software reset resistor
    Audio_ConfigureRegister(0x04,0x03);// Clock-Gen_Muxing PLL_CLKIN = BCLK, CODEC_CLKIN = PLL_CLK
    Audio_ConfigureRegister(0x05,0x91);// PLL P & R values: PLL powered up, P = 1, R = 1
    Audio_ConfigureRegister(0x06,0x3C);// PLL J value: J = 60
    Audio_ConfigureRegister(0x07,0x00);// PLL D value MBS: D = 00
    Audio_ConfigureRegister(0x08,0x00);// PLL D value LBS: D = 00
    Audio_ConfigureRegister(0x0B,0x83);// NDAC is powered up and set to 3
    Audio_ConfigureRegister(0x0C,0x85);// MDAC is powered up and set to 5
    Audio_ConfigureRegister(0x0D,0x00);// DOSR value MSB = 0
    Audio_ConfigureRegister(0x0E,0x80);// DOSR value MSB = 128
    Audio_ConfigureRegister(0x1B,0x00);// Mode is I2S, wordlength is 16, slave mode

    Audio_ConfigureRegister(0x00,0x01);// Set register page to 1
    Audio_ConfigureRegister(0x1F,0x04);// Headphone drivers powered down
    Audio_ConfigureRegister(0x20,0x86);// Power-down Class-D driver, 0x86 is powered-up, 0x06 is powered-down
    Audio_ConfigureRegister(0x21,0x7F);// HP_Output_Drivers_POP_Removal_Settings
    Audio_ConfigureRegister(0x23,0x64);// DAC_Output_Routing + AIN1 input is routed to the left-channel mixer amplifier

    Audio_ConfigureRegister(0x2A,0x1C);// Unmute Class-D, set gain = 24 dB, 0x14 = 18dB
    Audio_ConfigureRegister(0x26,0x80);// Analog Volume to SPK, set 0x92= -9dB, 0x80 = 0dB

    Audio_ConfigureRegister(0x2E,0x0B);//Test detected speaker

    Audio_ConfigureRegister(0x00,0x00);// Set register page to 0
    Audio_ConfigureRegister(0x3F,0xD4);// Power-up DAC left and right channels (soft step enabled)
    Audio_ConfigureRegister(0x41,0x00);// DAC Left gain 0xD4 = -22 dB, 0x02 = 1dB, 0x0A = 5dB
    Audio_ConfigureRegister(0x42,0x00);// DAC Right gain 0xD4 = -22 dB, 0x02 = 1dB, 0x0A = 5dB
    Audio_ConfigureRegister(0x40,0x00);// Unmute DAC left and right channels

    Audio_ConfigureRegister(0x43,0x80);//Test detected speaker

    //Test detected speaker
    Audio_ConfigureRegister(0x00,0x03);//Test detected speaker
    Audio_ConfigureRegister(0x10,0x01);//Test detected speaker
    
    Audio_SetVolume(ePlayLow1Alarm, 4);
    
    return true;
}

/****************************************************************************************/
/*                                                                                      */
/* General 	-audioMgr_SetVolumeVoice                                           		    */
/*                                                                                      */
/* Details  -Set volume	audio														    */
/*																						*/
/*                                                                                      */
/* Arguments : (I) uint16_t value : value volume					 					*/
/*             (O) NONE																	*/
/*                                                                                      */
/* ReturnValue : void		                                                            */
/*                                                                                      */
/****************************************************************************************/
static void Audio_SetVolume(E_AudioCommand cmdId, uint16_t value)
{
    //	audioMgr_WriteRegI2C(0x20,0x8600, 1);
    switch(value)
    {
    case 1 :
            Audio_ConfigureRegister(0x00,0x01);           
            if(cmdId == ePlayHighAlarm){
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+36+GAIN_OFFSET_FOR_HIGH_ALARM);
            }
            else{
              Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+36);
            }
            break;
    case 2 :
            Audio_ConfigureRegister(0x00,0x01);
            
            if(cmdId == ePlayHighAlarm){
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+32+GAIN_OFFSET_FOR_HIGH_ALARM);
            }
            else{
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+32);
            }
            break;
    case 3:
            Audio_ConfigureRegister(0x00,0x01);
            
            if(cmdId == ePlayHighAlarm){
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+28+GAIN_OFFSET_FOR_HIGH_ALARM);
            }
            else{
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+28);
            }
            break;
    case 4 :
            Audio_ConfigureRegister(0x00,0x01);
            
            if(cmdId == ePlayHighAlarm){
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+24+GAIN_OFFSET_FOR_HIGH_ALARM);
            }
            else{
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+24);
            }
            break;
    case 5:
            Audio_ConfigureRegister(0x00,0x01);
            
            if(cmdId == ePlayHighAlarm){
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+20+GAIN_OFFSET_FOR_HIGH_ALARM);
            }
            else{
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+20);
            }
            break;
    case 6 :
            Audio_ConfigureRegister(0x00,0x01);
            
            if(cmdId == ePlayHighAlarm){
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+16+GAIN_OFFSET_FOR_HIGH_ALARM);
            }
            else{
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+16);
            }
            break;
    case 7:
            Audio_ConfigureRegister(0x00,0x01);
            
            if(cmdId == ePlayHighAlarm){
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+12+GAIN_OFFSET_FOR_HIGH_ALARM);
            }
            else{
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+12);
            }
            break;
    case 8:
            Audio_ConfigureRegister(0x00,0x01);
            
            if(cmdId == ePlayHighAlarm){
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+8+GAIN_OFFSET_FOR_HIGH_ALARM);
            }
            else{
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+8);
            }
            break;
    case 9:
            Audio_ConfigureRegister(0x00,0x01);
            
            if(cmdId == ePlayHighAlarm){
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+4+GAIN_OFFSET_FOR_HIGH_ALARM);
            }
            else{
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+4);
            }
            break;
    case 10 :
            Audio_ConfigureRegister(0x00,0x01);
            
            if(cmdId == ePlayHighAlarm){
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+GAIN_OFFSET_FOR_HIGH_ALARM);// 
            }
            else{
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET);
            }
            break; //Volume 10(MAX)
    default :// = level 1
            Audio_ConfigureRegister(0x00,0x01);           
            if(cmdId == ePlayHighAlarm){
                Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+36+GAIN_OFFSET_FOR_HIGH_ALARM);
            }
            else{
              Audio_ConfigureRegister(0x26,128+GAIN_OFFSET+36);
            }
            break;
    }

    Audio_ConfigureRegister(0x00,0x01);//Add 03-10-2016
    Audio_ConfigureRegister(0x20,0x86);//Add 03-10-2016
}


//void Audio_PlayAudio()
//{
////    Audio_SetVolumeVoice(1);
////    _i2s_TLV320DataTransmit(audioSquareWave100ms, 8910);
////    _i2s_TLV320DataTransmit(audioVoiceSync, 8910);//160954
////    _i2s_TLV320DataTransmit(audioVoiceStart, 248828);
//    _i2s_TLV320DataTransmit(audioSquareWave260ms_Medium, 23026);
//}

void Audio_PlaySquareWaveHigh()
{
    _i2s_TLV320DataTransmit(audioSquareWave210ms_High, SIZE_SOUND_800HZ_210MS_HIGH);
}

void Audio_PlaySquareWaveMedium()
{
    _i2s_TLV320DataTransmit(audioSquareWave260ms_Medium, SIZE_SOUND_800HZ_260MS_MEDIUM);
}

void Audio_PlaySquareWaveLow()
{
    _i2s_TLV320DataTransmit(audioSquareWave260ms_Low, SIZE_SOUND_800HZ_260MS_LOW);
}


/**
 * @brief TLV320 I2C register prepare and blocking transmit
 * @param (in) regAddress LTV320 register address
 * @param (in) regValue TLV320 register value
 * @return None
 */
bool Audio_ReadRegister(uint8_t regAddress, uint8_t* regValue) {
  
    uint8_t buffWrite[1] = {'\0'};
    buffWrite[0] = regAddress;

    uint8_t buffRead[1] = {'\0'};
    //buffRead[0] = AUDIO_READ_ADDR;
    
    //write a I2C buffer containing command byte and PEC (crc8) to Smart battery 
    if (I2C2_Write(AUDIO_WRITE_ADDR, 
              (void*)buffWrite, sizeof(buffWrite), 
              AUDIO_MAX_WAIT_MS) == false)
    {
        SYS_PRINT("I2CAudio Write to read Fail\n");
        return false;
    }
    else
    {         
        if(I2C2_Read(AUDIO_READ_ADDR, 
                (void*)buffRead, sizeof(buffRead), 
                AUDIO_MAX_WAIT_MS) == false)
        {            
            SYS_PRINT("I2CAudio Read Fail\n");
            return false;
        }
        else
        {
            *regValue = buffRead[0];
            return true;
        }
    }
}
/**
 * @brief TLV320 I2C register prepare and blocking transmit
 * @param (in) regAddress LTV320 register address
 * @param (in) regValue TLV320 register value
 * @return None
 */
bool Audio_ConfigureRegister(uint8_t regAddress, uint8_t regValue) {
    //check error
    if (s_AudioError != eDeviceNoError) {
        //report error
        Audio_ReportError();
        return false;
    }
    
    uint8_t data[2] = {regAddress, regValue};
    //write data and check result
    int i;
    bool result;
    for (i = 0; i < 2; i++) {      //max try 2 times
        result = I2C2_Write(AUDIO_WRITE_ADDR, (void*) &data[0], sizeof(data), AUDIO_MAX_WAIT_MS);
        if (result == true) {
            break;
        }
        //delay and try again
        vTaskDelay(10);
    }
    if (result == false) {
        //set error flag
        s_AudioError = eDeviceErrorDetected;
        SYS_PRINT("Audio_ConfigureRegister: ERROR\n");
        return false; //error
    }
    else {

        //uint8_t regValue = 0;
        //Audio_ReadRegister(regAddress, &regValue);
        //SYS_PRINT("\nAudio_Register %X: %X", regAddress, regValue);
        //SYS_PRINT("\nAudio_ConfigureRegister: OK");
        //clear error 
        s_AudioError = eDeviceNoError;
        return true;
    }
}


/** @brief Report error if occur during communicate with AUDIO
 *  @param [in]   None
 *  @param [out]  None
 *  @return None
 */
void Audio_ReportError() {
    //check whether an error is detected, then send event to ALarm task
    if (s_AudioError == eDeviceErrorDetected) {
        //send event to alarm task
        alarmInterface_SendEvent(eAudioErrorAlarm, eActive, eHighPriority, 0);
        SYS_PRINT("\n error at: Audio_ReportError");
        //change state
        s_AudioError = eDeviceErrorReported;
    }
}



void Audio_HandleHighAlarm()
{
    if((s_TickCount == TIME_COUNT_0MS)
      ||(s_TickCount == TIME_COUNT_250MS)
      ||(s_TickCount == TIME_COUNT_500MS)
      ||(s_TickCount == TIME_COUNT_1000MS)
      ||(s_TickCount == TIME_COUNT_1250MS)
      ||(s_TickCount == TIME_COUNT_2000MS)
      ||(s_TickCount == TIME_COUNT_2250MS)
      ||(s_TickCount == TIME_COUNT_2500MS)
      ||(s_TickCount == TIME_COUNT_3000MS)
      ||(s_TickCount == TIME_COUNT_3250MS))
    {
        Audio_PlaySquareWaveHigh();
    }
    if(s_TickCount > (TIME_COUNT_1250MS + TIME_COUNT_500MS))
    {
        if(gs_CurrentPlayState != gs_NewPlayState)
        {
            gs_CurrentPlayState = gs_NewPlayState;
            //clear tick count
            s_TickCount = 0;
            return;
        }
    }
    
    s_TickCount++;
    if(s_TickCount >= TIME_COUNT_10000MS)
    {
      s_TickCount = 0;
    }
    return;
}



void Audio_HandleMediumAlarm()
{
    if((s_TickCount == TIME_COUNT_0MS)
      ||(s_TickCount == TIME_COUNT_400MS)
      ||(s_TickCount == TIME_COUNT_800MS))
    {
        Audio_PlaySquareWaveMedium();
    }
    if(s_TickCount > (TIME_COUNT_800MS + TIME_COUNT_500MS))
    {
        if(gs_CurrentPlayState != gs_NewPlayState)
        {
            gs_CurrentPlayState = gs_NewPlayState;
            //clear tick count
            s_TickCount = 0;
            return;
        }
    }
    
    s_TickCount++;
    if(s_TickCount >= TIME_COUNT_25000MS)
    {
      s_TickCount = 0;
    }
    return;
}        

/** @brief This operation handle play low priority alarm sound
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void Audio_HandleStartupSound()
{
    if ((s_TickCount == TIME_COUNT_0MS)
      ||(s_TickCount == TIME_COUNT_250MS)
      ||(s_TickCount == TIME_COUNT_500MS)
      ||(s_TickCount == TIME_COUNT_750MS)
      ||(s_TickCount == TIME_COUNT_1000MS)
      ||(s_TickCount == TIME_COUNT_1250MS)
      ||(s_TickCount == TIME_COUNT_1500MS)
      ||(s_TickCount == TIME_COUNT_1750MS)
      ||(s_TickCount == TIME_COUNT_2000MS)
      ||(s_TickCount == TIME_COUNT_2250MS)
      ||(s_TickCount == TIME_COUNT_2500MS)
      ||(s_TickCount == TIME_COUNT_2750MS)
      ||(s_TickCount == TIME_COUNT_3000MS))
        Audio_PlaySquareWaveHigh();
    if(gs_CurrentPlayState != gs_NewPlayState)
    {
        gs_CurrentPlayState = gs_NewPlayState;
        //clear tick count
        s_TickCount = 0;
        return;
    }
    
    s_TickCount++;
    if(s_TickCount >= TIME_COUNT_3000MS)
    {
      s_TickCount = 0;
    }
    return;
}        

/** @brief This operation handle play start up sound
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void Audio_HandleLowAlarm()
{
    if((s_TickCount == TIME_COUNT_0MS)
      ||(s_TickCount == TIME_COUNT_450MS))
    {
        Audio_PlaySquareWaveLow();
    }
    if(s_TickCount > (TIME_COUNT_450MS + TIME_COUNT_500MS))
    {
        if(gs_CurrentPlayState != gs_NewPlayState)
        {
            gs_CurrentPlayState = gs_NewPlayState;
            //clear tick count
            s_TickCount = 0;
            return;
        }
    }
    
    s_TickCount++;
    if(s_TickCount >= TIME_COUNT_30000MS)
    {
      s_TickCount = 0;
    }
    return;
}  

/** @brief This operation play high priority alarm sound
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void Audio_PlayHighAlarm()
{
    if( xTimerIsTimerActive( gs_TimerPlaySound ) == pdFALSE )
    {
        if (xTimerStart(gs_TimerPlaySound, TICK_TO_WAIT) != pdPASS) 
        {
            SYS_PRINT("ERR: Failed to start gs_TimerPlaySound \n");
        }
    }
    gs_NewPlayState = ePlayHighAlarmState;
    return;
}

/** @brief This operation play medium alarm sound
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void Audio_PlayMediumAlarm()
{
    if (xTimerStart(gs_TimerPlaySound, TICK_TO_WAIT) != pdPASS) 
    {
	SYS_PRINT("ERR: Failed to start gs_TimerPlaySound \n");
    }
    gs_NewPlayState = ePlayMediumAlarmState;
    return;
}

/** @brief This operation play low priority alarm sound
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void Audio_PlayLowAlarm()
{
    if (xTimerStart(gs_TimerPlaySound, TICK_TO_WAIT) != pdPASS) 
    {
	SYS_PRINT("ERR: Failed to start gs_TimerPlaySound \n");
    }
    gs_NewPlayState = ePlayLow1AlarmState;
    return;
}

/** @brief This operation play low priority alarm sound
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void Audio_PlayStartupSound()
{
    if (xTimerStart(gs_TimerPlaySound, TICK_TO_WAIT) != pdPASS) 
    {
	SYS_PRINT("ERR: Failed to start gs_TimerPlaySound \n");
    }
    gs_NewPlayState = ePlayStartupSoundState;
    return;
}

/** @brief This operation stop play alarm sound
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void Audio_StopAlarm()
{
//    s_TickCount = 0;
    gs_NewPlayState = eIdleState;
}

/** @brief This operation handle event command to control play alarm sound
 *  @param [in] uint16_t time: period of mute in seconds
 *  @param [out] None
 *  @return None
 */
void Audio_MuteAlarmInPeriod(uint32_t time)
{
    if((gs_CurrentPlayState == ePlayHighAlarmState)
      ||(gs_CurrentPlayState == ePlayMediumAlarmState)
      ||(gs_CurrentPlayState == ePlayLow1AlarmState)
      ||(gs_CurrentPlayState == ePlayStartupSoundState))
    {
        //save state before enter to mute
        gs_PlayStateBeforeMute = gs_CurrentPlayState;

        //set new state to mute
        gs_NewPlayState = eMuteState;
        //stop timer
        if (xTimerStop(gs_TimerMuteAlarm, TICK_TO_WAIT) != pdPASS) 
        {
            SYS_PRINT("ERR: Failed to stop gs_TimerMuteAlarm \n");
        }
        //change period
        if (xTimerChangePeriod( gs_TimerMuteAlarm, time*1000, TICK_TO_WAIT ) != pdPASS)
        {
            SYS_PRINT("ERR: Failed to change period gs_TimerMuteAlarm \n");
        }
        if( xTimerIsTimerActive( gs_TimerMuteAlarm ) == pdFALSE )
        {
            if (xTimerStart(gs_TimerMuteAlarm, TICK_TO_WAIT) != pdPASS) 
            {
                SYS_PRINT("ERR: Failed to start gs_TimerMuteAlarm \n");
            }
        }
    }
}

/** @brief This is call back function when the software timer "Audio_TimerMuteAlarmCallback" timer out
 * This function handle state of playing sound
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void Audio_TimerPlaySoundCallback()
{
    switch (gs_CurrentPlayState)
    {
    case eIdleState:
        if(gs_CurrentPlayState != gs_NewPlayState)
        {
            gs_CurrentPlayState = gs_NewPlayState;
        }
        else
        {
            //stop software timer
            if (xTimerStop(gs_TimerPlaySound, TICK_TO_WAIT) != pdPASS) 
            {
                SYS_PRINT("ERR: Failed to stop gs_TimerPlaySound \n");
            }
        }
        s_TickCount = 0;
        
        SYS_PRINT("\nStop Sound.");
        break;
        
    case ePlayHighAlarmState:
        Audio_HandleHighAlarm();
        break;
        
    case ePlayMediumAlarmState:
        Audio_HandleMediumAlarm();
        break;
        
    case ePlayLow1AlarmState:
        Audio_HandleLowAlarm();
        break;
    case ePlayStartupSoundState:
        Audio_HandleStartupSound();
        break;
    case eMuteState:
        if(gs_CurrentPlayState != gs_NewPlayState)
        {
            gs_CurrentPlayState = gs_NewPlayState;
        }
        break;
        
    default:
        break;
    }
}

/** @brief This is call back function when the software timer "Audio_TimerMuteAlarmCallback" timer out
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void Audio_TimerMuteAlarmCallback()
{
    //restore state before mute
    gs_NewPlayState = gs_PlayStateBeforeMute;
    guiInterface_SendEvent(eGuiMainScreenAlarmMuteIconHide, 0);
    SYS_PRINT("Timer mute time out \n");
}

/** @brief This operation create software timer for playing alarm sound
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void Audio_CreateSoftwareTimer()
{
    
    gs_TimerPlaySound = xTimerCreate( "Play alarm timer",
                                    pdMS_TO_TICKS(50),
                                    pdTRUE,
                                  ( void * ) TIMER_PLAY_SOUND_ID,
                                  Audio_TimerPlaySoundCallback);
    
    gs_TimerMuteAlarm = xTimerCreate( "Mute alarm",
                                    pdMS_TO_TICKS(30*1000),
                                    pdTRUE,
                                  ( void * ) TIMER_MUTE_ALARM_ID,
                                  Audio_TimerMuteAlarmCallback);
    
    return;
}

/** @brief This operation handle event command to control play alarm sound
 *  @param [in] E_AudioCommand cmdId: ID of the event such as ePlayHighAlarm, ePlayMediumAlarm,
 * ePlayLow1Alarm, eStopPlay, eMute
 *  @param [in] uint16_t data: content of the event. If event is ePlayHighAlarm/ePlayMediumAlarm/
 * ePlayMediumAlarm, the data the current volume level of setting. If event is eMute, the data is the mute time (seconds).
 *  @param [out] None
 *  @return None
 */
void Audio_HandleEvent(E_AudioCommand cmdId, uint16_t data)
{
  switch (cmdId)
  {
      case ePlayHighAlarm:
          Audio_SetVolume(ePlayHighAlarm, data);
          Audio_PlayHighAlarm();
          break;
      case ePlayMediumAlarm:
          Audio_SetVolume(ePlayMediumAlarm, data);
          Audio_PlayMediumAlarm();
          break;
      case ePlayLow1Alarm:
          Audio_SetVolume(ePlayLow1Alarm, data);
          Audio_PlayLowAlarm();
          break;
      case eStopPlay:
          Audio_StopAlarm();
          break;
      case eMute:
          //TODO
          Audio_MuteAlarmInPeriod(data);
          break;
      case eTestSound:
        s_TickCount = 0;
        Audio_SetVolume(ePlayLow1Alarm, data);
        Audio_PlayLowAlarm();
        break;
      case ePlayStartupSound:
          Audio_SetVolume(ePlayStartupSound, data);
          Audio_PlayStartupSound();
      default:
           break;

  }
}
/* *****************************************************************************
 End of File
 */
