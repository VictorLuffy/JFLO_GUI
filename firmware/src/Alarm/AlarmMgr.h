/* ************************************************************************** */
/** @file [AlarmMgr.c]
 *  @brief {Manager of JFLO alarm}
 *  @author {Cuong Le}
 */
/* ************************************************************************** */

#include "AlarmInterface.h"

#ifndef ALARMMGR_H
#define	ALARMMGR_H

#define UPPER_AMBIENT_TEMPERATURE_RANGE                                                 29 //TBD        
#define UNDER_AMBIENT_TEMPERATURE_RANGE                                                 18 //TBD
#define UPPER_OXYGEN_RANGE_DEFAULTH                                                                               //%
#define UNDER_OXYGEN_RANGE_DEFAULTH                                                     21 //%
#define AVERAGE_WATER_SENSOR_HZ                                                         30 // Hz
#define METRAN_CODE                                                                     0x33
//Define wait time for alarm
#define TIMER_1S 20
#define OXYGENCONCENTRATE_ALARM_TIMEOUT                                                    60 * TIMER_1S
#define TARGETTEMPERATURECANNOTBEACHIEVEDTEMPERATURE_ALARM_TIMEOUT                    30 * 60 * TIMER_1S
#define CONNECTIONOFCHAMBER_ALARM_TIMEOUT                                                   1 * TIMER_1S
#define NOMOREWATERCHAMBER_ALARM_TIMEOUT                                                1* 60 * TIMER_1S
#define DEVICEABONORMALITY_ALARM_TIMEOUT                                              30 * 60 * TIMER_1S
#define RUNOUTOFWATER_ALARM_TIMEOUT                                                        30 * TIMER_1S 
#define TEMPERATURE_ALARM_TIMEOUT                                                      5 * 60 * TIMER_1S   
#define CHECKFORLEAKS_ALARM_TIMEOUT 					                                    30 * TIMER_1S
#define CHECKFORBLOCKAGE_ALARM_TIMEOUT 					                                    30 * TIMER_1S  
#define FAILURECOILTEMPERATURESENSOR_ALARM_TIMEOUT                                          1 * TIMER_1S
#define FAILUREBREATHINGCIRCUITOUTTEMPERATURESENSOR_ALARM_TIMEOUT 					        1 * TIMER_1S
#define SPO2LOSSPULSE_ALARM_TIMEOUT 					                                    3 * TIMER_1S 
#define SPO2SENSORPROBE_ALARM_TIMEOUT 					                                    3 * TIMER_1S
#define TWO_MINUTES_AFTER_START                                                        2 * 60 * TIMER_1S
#define FIVE_MINUTES_AFTER_START                                                       5 * 60 * TIMER_1S
#define WATER_SUPPLY_CHECK_TIMEOUT                                                     3 * 60 * TIMER_1S
#define AIR_FLOW_ABNORMAL_CHECK_TIMEOUT                                                    10 * TIMER_1S
#define POSTURE_SAMPLE                                                                 TIMER_1S / 10
// Add by Thanh
/** @brief Define macro check if setting was change in 5 min or not*/
#define TEMP_CHANGE_SETTING_TIMEOUT                                                     5 * 60 * TIMER_1S
#define FLOW_CHANGE_SETTING_TIMEOUT                                                         30 * TIMER_1S
#define TOO_MUCH_WATER_IN_THE_CHAMBER_TIMEOUT                                           5 * 60 * TIMER_1S
#define NUM_SAMPLE_IN_2MINS                                                             (2*60/1)
#define NUM_SAMPLE_IN_5MINS                                                             (5*60/1)
//#define ALARM_TASK_DELAY                                                                (50)
#define TICK_PER_SECOND                                                                 (1000/50) 
#define EVT_TEMP_NUM_SAMPLE_IN_1SECOND                                                  (1000/50)
#define CHAMBER_OUT_TEMP_NUM_SAMPLE_IN_10SECOND                                         (10000/50)
#define IH_POWER_NUM_SAMPLE_IN_1MINS                                                    (1 * 60)
//Initial Alarm
void alarmMgr_InitAlarm(void);

//Clear all of alarm when turn off device
void alarmMgr_ClearAllAlarm(void);

//Update Alarm monitor value for alarm task
void alarmMgr_UpdateAlarmMonitor(void);

//check all alarm trigger condition
void alarmMgr_CheckAllAlarm(void);

//update status for all of alarm
void alarmMgr_UpdateAllAlarm(void);


#endif	/* ALARMMGR_H */

/* *****************************************************************************
 End of File
 */