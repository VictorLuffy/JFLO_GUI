/* ************************************************************************** */
/** @file [IC_8.c]
 *  @brief { This file contains all methods to calculate frequency from PIN M10.
 * It also support 2 measurement unit: Hz and RPM.
 * From Harmony,IC8 is configured as 16 pulse per capture, no interrupt
 * Functions: IC8_GetFrequencyHz() or IC8_GetFrequencyRPM() should called periodically to
 * read value from IC8 and convert it to frequency. Functions IC8_GetLastFrequencyHz()
 * and IC8_GetLastFrequencyRPM() can be used to get latest frequency value without
 * calculation }
 *  @author {nguyen truong, bui phuoc}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"

/** @brief IC8 index on Harmony configuration */
#define IC_8_INDEX                  0

/** @brief Timer6 clock frequency input. Timer8 is used as a part of IC8 to measure
 * input frequency. The input frequency is calculated from PERIPHERAL CLOCK 100MHz, 
 * by passed PRE-SCALE 256 */
#define IC_8_TIMER_CLK              390625      

/** @brief IC8 number of pulse per 1 IC capture. This value is defined in Harmony 
 * configuration */
#define IC_8_PULSE_PER_CAPTURE      16           

/** @brief Formula to convert a frequency value from Hz to RPM */
#define HZ_TO_RPM(x)                (60*x)


/** @brief IC8 handle, use over operation time */
static DRV_HANDLE s_IC8_Handle = DRV_HANDLE_INVALID;

/** @brief The last capture value. This value should memmorize to calculate elapsed
 * time when the next time tick is obtained */
static uint16_t s_IC8lastCaptureTick = 0;

/** @brief The latest frequency calculated, in Hz */
static float s_IC8frequencyInHz = 0;

/** @brief local functions declaration */
static uint8_t IC8_Read(uint16_t* buffer, uint16_t size);

/** @brief Function to initialize IC8, used to calculate frequency from PIN FGOUT 
 * of DRV8308 motor driver. 
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void IC8_Initialize() {
    /* Open the IC driver if it has not been previously opened */
    if (s_IC8_Handle == DRV_HANDLE_INVALID) {
        s_IC8_Handle = DRV_IC_Open(IC_8_INDEX, DRV_IO_INTENT_READWRITE);
    }

    //reset variables
    s_IC8frequencyInHz = 0;
}

/** @brief Function to start IC8 operation
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void IC8_Start() {
    /* Start the input capture operation */
    DRV_IC_Start(s_IC8_Handle, DRV_IO_INTENT_READWRITE);

    /* Start Timer Driver */
    DRV_TMR4_Start();

    //reset variables
    s_IC8lastCaptureTick = DRV_TMR4_CounterValueGet();
    s_IC8frequencyInHz = 0;
}

/** @brief Function to stop IC8 from operation
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void IC8_Stop() {
    /* Stop input capture and timer drivers */
    DRV_IC_Stop(s_IC8_Handle);
    DRV_TMR4_Stop();
}

/** @brief Calculate frequency from the PIN FGOUT of DRV8308 (in Hz) by reading
 * the IC8 FIFO value and convert to Hz
 *  @param [in]  None   
 *  @param [out]  float* frequency      external memory pointer to store frequency
 *  @return None
 *  @retval true        new frequency data is up to dated
 *  @retval false       no new value is obtained
 */
bool IC8_GetFrequencyHz(float* frequency) {
#define IC8_FIFO_SIZE       4   //max 4 data, as the length of IC FIFO
    uint16_t data[IC8_FIFO_SIZE];
    bool rtn = false;
    uint8_t numRead = IC8_Read(&data[0], IC8_FIFO_SIZE);
    if (numRead > 0) {
        //handle data
        uint16_t i = 0;
        uint32_t elapsedTime = 0;

        for (i = 0; i < numRead; i++) {
            //get elapsed time
            elapsedTime += (uint16_t) (data[i] - s_IC8lastCaptureTick);
            //update tick time
            s_IC8lastCaptureTick = data[i];
        }

        float fCapture = (float) IC_8_TIMER_CLK / (float) elapsedTime;
        float fPulse = fCapture * (float) IC_8_PULSE_PER_CAPTURE * (float) numRead;

        s_IC8frequencyInHz = fPulse;
        rtn = true;
    }
    *frequency = s_IC8frequencyInHz;
    return rtn;
}

/** @brief Calculate frequency from the PIN FGOUT of DRV8308 (in RPM) by reading
 * the IC8 FIFO value and convert to RPM
 *  @param [in]  None   
 *  @param [out]  float* frequency      external memory pointer to store frequency
 *  @return None
 *  @retval true        new frequency data is up to dated
 *  @retval false       no new value is obtained
 */
bool IC8_GetFrequencyRPM(float* frequency) {
    float freqInHz = 0;
    bool result = IC8_GetFrequencyHz(&freqInHz);
    float freqInRPM = HZ_TO_RPM(freqInHz);
    *frequency = freqInRPM;
    return result;
}

/** @brief Get latest frequency in Hz, not perform any calculation
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None    float       latest frequency value in Hz
 */
float IC8_GetLastFrequencyHz() {
    return s_IC8frequencyInHz;
}

/** @brief Get latest frequency in RPM, not perform any calculation
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None    float       latest frequency value in Hz
 */
float IC8_GetLastFrequencyRPM() {
    return HZ_TO_RPM(s_IC8frequencyInHz);
}

/** @brief Read data from IC8 FIFO, store them to external buffer
 * IC8 FIFO has maximum 4 sample of data
 *  @param [in]  uint16_t size:      size of external buffer   
 *  @param [out]  uint16_t* buffer:  external buffer to store data
 *  @return None  uint8_t:           number of sample has been read
 */
uint8_t IC8_Read(uint16_t* buffer, uint16_t size) {
    uint16_t i = 0;
    for (i = 0; i < size;) {
        /* Check for buffer empty and read the buffer value */
        if (!DRV_IC_BufferIsEmpty(s_IC8_Handle)) {
            buffer[i++] = DRV_IC_Capture16BitDataRead(s_IC8_Handle);//SYS_PRINT("IC8 buffer exist\n");
        } else { //SYS_PRINT("IC8 buffer empty\n");
            break;
        }
    }
    return i;
}

/* *****************************************************************************
 End of File
 */
