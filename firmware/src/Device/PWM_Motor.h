/* ************************************************************************** */
/** @file [PWM_Motor.h]
 *  @brief {Generate PWM with various frequency to control DRV8308 motor driver.
 * the Motor speed will respond follow that input PWM. Motor speed for that application
 * is specified from 400 Hz to 2.0 Khz, respectively with RPM from 6 krpm to 31 krpm.
 * Since the motor has 8 poles (4 pole pairs), 3 phase PMSM, 3 hall sensors mounted on ROTO, the 
 * formula to convert from RPM to Hz will be: f(Hz) = 4 * RPM / 60
 * All settings and configurations on this file relate tightly with the MPLAB HARMONY configuration.
 * Change the configuration of the HARMONY may lead these function failed}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


#ifndef _PWM_MOTOR_H    /* Guard against multiple inclusion */
#define _PWM_MOTOR_H


/* This section lists the other files that are included in this file.
 */

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    //function to initialize PWM channels
    void PWM_Motor_init();

    //function to start generating output signal to PWM_MOTOR CONTROL specified PIN
    void PWM_Motor_start();

    //function to stop generating PWM on specified PIN
    void PWM_Motor_stop();

    //function to set frequency, range from 400.0 Hz to 2000.0 Hz
    //void PWM_Motor_setFrequency(float freInHz);

    float PWM_Motor_MonitorSpeed(void);
    
    float PWM_Motor_GetLatestSpeed(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PWM_MOTOR_H */

/* *****************************************************************************
 End of File
 */
