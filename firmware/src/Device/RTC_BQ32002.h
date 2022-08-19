/* ************************************************************************** */
/** @file [RTC_BQ32002.h]
 *  @brief {Supply APIs configure and get data from BQ32002 Real-Time Clock 
 *  @author {nguyen truong}
 */
/* ************************************************************************** */


#ifndef _RTC_BQ32002_H    /* Guard against multiple inclusion */
#define _RTC_BQ32002_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "stdint.h"
#include "System/ApplicationDefinition.h"

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/** @brief Time structure definitions for easy manipulate the data */
typedef struct {
    /** Years Register */
    uint8_t YEAR;
    /** Months Register */
    uint8_t MONTH;
    /** Days Register */
    uint8_t DAY;
    /** Date Register */
    uint8_t DATE;
    /** Hours Register */
    uint8_t HOUR;
    /** Minutes Register */
    uint8_t MIN;
    /** Seconds Register */
    uint8_t SEC;
} RTC_TIME_t;


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    void rtc_Init(void);
    
    //Set data time
    void rtc_SetTime(RTC_TIME_t time);
    
    void rtc_CheckValidTime();
    
    void rtc_ConfigForTestAccuracy();
    
    
    RTC_TIME_t rtc_ReadTime(void);
    //Get time
    bool rtc_GetTime(RTC_TIME_t* time);
    
    E_DeviceErrorState rtc_ReportError(void);


    /** @brief Query any error happen with RTC module
    *  @param [in]  None   
    *  @param [out] None
    *  @retval true RTC module has error
    *  @retval false RTC module is OK
    */
    bool rtc_IsModuleFailed();
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _RTC_BQ32002_H */

/* *****************************************************************************
 End of File
 */
