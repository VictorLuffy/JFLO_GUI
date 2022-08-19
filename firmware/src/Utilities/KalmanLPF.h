/* ************************************************************************** */
/** @file [KalmanLPF.h]
 *  @brief {This file supply interface to build a Kalman Low pass filter. The filter
 * is designed with 3 state of sample and 3 models data. This filter is modified
 * from "Extended Kalman Filter" }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


#ifndef _KALMAN_LPF_H_
#define _KALMAN_LPF_H_

#include <stdbool.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Number of state */
#define Nsta 3  

    /** @brief Number of model data */
#define Mobs 3    

    /** @brief structure for a Kalman Low Pass Filter */
    typedef struct {
        int n; /* number of state values */
        int m; /* number of observables */

        double x[Nsta]; /* state vector */

        double P[Nsta][Nsta]; /* prediction error covariance */
        double Q[Nsta][Nsta]; /* process noise covariance */
        double R[Mobs][Mobs]; /* measurement error covariance */

        double G[Nsta][Mobs]; /* Kalman gain; a.k.a. K */

        double F[Nsta][Nsta]; /* Jacobian of process model */
        double H[Mobs][Nsta]; /* Jacobian of measurement model */

        double Ht[Nsta][Mobs]; /* transpose of measurement Jacobian */
        double Ft[Nsta][Nsta]; /* transpose of process Jacobian */
        double Pp[Nsta][Nsta]; /* P, post-prediction, pre-update */

        double fx[Nsta]; /* output of user defined f() state-transition function */
        double hx[Mobs]; /* output of user defined h() measurement function */

        /* temporary storage */
        double tmp0[Nsta][Nsta];
        double tmp1[Nsta][Mobs];
        double tmp2[Mobs][Nsta];
        double tmp3[Mobs][Mobs];
        double tmp4[Mobs][Mobs];
        double tmp5[Mobs];

    } KALMAN_LPF_t;


    /** @brief Initializes an KALMAN Low pass filter structure
     *  @param [in]     KALMAN_LPF_t * ekf      pointer to EKF structure to initialize 
     *  @param [out]  None
     *  @return None
     */
    void KalmanLPF_Initialize(KALMAN_LPF_t* ekf);


    /** @brief Runs one step of EKF prediction and update
     *  @param [in]     KALMAN_LPF_t * ekf      pointer to EKF structure 
     *                  double * z              pointer to bundle of data
     *  @param [out]  None
     *  @return None
     */
    bool KalmanLPF_Step(KALMAN_LPF_t* ekf, double * z);


    /** @brief Returns the state element at a given index
     *  @param [in]     KALMAN_LPF_t * ekf      pointer to EKF structure 
     *                  int i       the index (at least 0 and less than <i>n</i>
     *  @param [out]    None
     *  @return         double      state value at index 
     */
    double KalmanLPF_GetX(KALMAN_LPF_t* ekf, int i);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _KALMAN_LPF_H_ */
