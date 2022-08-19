/* ************************************************************************** */
/** @file [PID.c]
 *  @brief {A PID controller, modified from a PID controller of author Nick Mosher
 * Support all method to implement an turning a PID controller}
 *  @author {bui phuoc}
 * 
/* ************************************************************************** */



#ifndef _PID_H
#define _PID_H

#include <stdint.h>
#include <stdbool.h>
#include <float.h>

#include "FreeRTOS.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief structure for a PID instance, each PID controller has its own instance. 
     * The instance will refer for every method of PID controller */
    typedef struct pid_controller {
        float p;
        float i;
        float d;
        float target;
        float output;
        uint8_t enabled;
        float currentFeedback;
        float lastFeedback;
        float error;
        float lastError;
        TickType_t currentTime;
        TickType_t lastTime;
        float integralCumulation;
        float maxCumulation;
        float cycleDerivative;

        uint8_t inputBounded;
        float inputLowerBound;
        float inputUpperBound;
        uint8_t outputBounded;
        float outputLowerBound;
        float outputUpperBound;
        uint8_t feedbackWrapped;
        float feedbackWrapLowerBound;
        float feedbackWrapUpperBound;

        uint8_t timeFunctionRegistered;
        float (*pidSource)();
        void (*pidOutput)(float output);
        TickType_t(*getSystemTime)();
        
        float Init_integral;

    } PIDController;

    /** @brief Constructs the PIDController object with PID Gains and function pointers
     * for retrieving feedback (pidSource) and delivering output (pidOutput).
     * All PID gains should be positive, otherwise the system will violently diverge
     * from the target
     *  @param [in]     PIDController* controller   instance of PID controller
     *                  float p         The Proportional gain
     *                  float i         The Integral gain
     *                  float d         The Derivative gain
     *                  float (*pidSource)  The function pointer for retrieving system feedback.
     *  @param [out]    void (*pidOutput)   The function pointer for delivering system output.
     *  @return None
     */
    void PID_CreateController(PIDController* controller, float p, float i, float d, float (*pidSource)(void), void (*pidOutput)(float output));

    /** @brief This method uses the established function pointers to retrieve system
     * feedback, calculate the PID output, and deliver the correction value
     * to the parent of this PIDController.	This method should be run as
     * fast as the source of the feedback in order to provide the highest
     * resolution of control (for example, to be placed in the loop() method)
     *  @param [in]     PIDController *c      instance of PID controller
     *  @param [out]  None
     *  @return None
     */
    void PID_Calculate(PIDController *controller);

    /** @brief Enables or disables this PIDController
     *  @param [in]     PIDController *controller    instance of PID controller
     *                  uint8_t enabled     True to enable, False to disable
     *  @param [out]  None
     *  @return None
     */
    void PID_SetEnabled(PIDController *controller, uint8_t e);

    /** @brief Returns the value that the Proportional component is contributing to the output
     *  @param [in]     PIDController *controller    instance of PID controller
     *  @param [out]  None
     *  @return     float   The value that the Proportional component is contributing to the output.
     */
    float PID_GetProportionalComponent(PIDController *controller);

    /** @brief Returns the value that the Integral component is contributing to the output.
     *  @param [in]     PIDController *controller    instance of PID controller
     *  @param [out]  None
     *  @return     float   The value that the Integral component is contributing to the output.
     */
    float PID_GetIntegralComponent(PIDController *controller);

    /** @brief Returns the value that the Derivative component is contributing to the output.
     *  @param [in]     PIDController *controller    instance of PID controller
     *  @param [out]  None
     *  @return     float   The value that the Derivative component is contributing to the output.
     */
    float PID_GetDerivativeComponent(PIDController *controller);

    /** @brief Sets the maximum value that the integral cumulation can reach.
     *  @param [in]     PIDController *controller    instance of PID controller
     *                  float max       max The maximum value of the integral cumulation.
     *  @param [out]  None
     *  @return     None
     */
    void PID_SetMaxIntegralCumulation(PIDController *controller, float max);

    /** @brief Sets bounds which limit the lower and upper extremes that this PIDController
     * accepts as inputs.	Outliers are trimmed to the lower and upper bounds.
     * Setting input bounds automatically enables input bounds.
     *  @param [in]     PIDController *controller    instance of PID controller
     *                  float lower     The lower input bound.
     *                  float upper     The upper input bound.
     *  @param [out]  None
     *  @return     None
     */
    void PID_SetInputBounds(PIDController *controller, float lower, float upper);

    /** @brief Sets bounds which limit the lower and upper extremes that this PIDController
     * will ever generate as output. Setting output bounds automatically enables
     * output bounds.
     *  @param [in]     PIDController *controller    instance of PID controller
     *                  float lower     The lower output bound.
     *                  float upper     The upper output bound.
     *  @param [out]  None
     *  @return     None
     */
    void PID_SetOutputBounds(PIDController *controller, float lower, float upper);

    /** @brief Sets the bounds which the feedback wraps around. This
     * also enables Input Bounds at the same coordinates to
     * prevent extraneous domain errors.
     *  @param [in]     PIDController *controller    instance of PID controller
     *                  float lower     The lower wrap bound.
     *                  float upper     The upper wrap bound.
     *  @param [out]  None
     *  @return     None
     */
    void PID_SetFeedbackWrapBounds(PIDController *controller, float lower, float upper);

    /*
    void PID_SetPIDSource(PIDController *controller, float (*pidSource)());
    void PID_SetPIDOutput(PIDController *controller, void (*pidOutput)(float output));
    void PID_RegisterTimeFunction(PIDController *controller, unsigned long (*getSystemTime)(void));
     */

    /* Provide C++ Compatibility */
    
     void PID_SetIntegral_Init(PIDController *controller, float initValue);
     
#ifdef __cplusplus
}
#endif

#endif // _PID_H
