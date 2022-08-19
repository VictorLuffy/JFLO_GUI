///** @file RTC.h
// *  @brief This file contains RTC which include RTC initialization,
// *   RTC interrupt handler, and APIs for RTC access.
// *  @author Viet Le
// */
//
//#ifndef RTC_H
//#define	RTC_H
//
//#include "stdint.h"
//
///** @brief Time structure definitions for easy manipulate the data */
//typedef struct {
//    /** Years Register */
//    uint8_t YEAR;
//    /** Months Register */
//    uint8_t MONTH;
//    /** Days Register */
//    uint8_t DAY;
//    /** Hours Register */
//    uint8_t HOUR;
//    /** Minutes Register */
//    uint8_t MIN;
//    /** Seconds Register */
//    uint8_t SEC;
//} RTC_TIME_t;
//
////Initialize rtc
//void rtc_Init(void);
//
////Set data time
//void rtc_Set(RTC_TIME_t time);
//
////Get time
//RTC_TIME_t rtc_Get(void);
//
////Update and send to GUI
//void rtc_CheckAndUpdate(void);
//
//#endif	/* RTC_H */
//
///* end of file */