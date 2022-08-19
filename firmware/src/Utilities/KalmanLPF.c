/* ************************************************************************** */
/** @file [KalmanLPF.c]
 *  @brief {This file supply interface to build a Kalman Low pass filter. The filter
 * is designed with 3 state of sample and 3 models data. This filter is modified
 * from "Extended Kalman Filter" }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */

#include <KalmanLPF.h>

/** @brief local functions  */
extern void ekf_init(void * v, int n, int m);
extern int ekf_step(void * ekf, double * z);
static void KalmanLPF_SetP(KALMAN_LPF_t* ekf, int i, int j, double value);
static void KalmanLPF_SetQ(KALMAN_LPF_t* ekf, int i, int j, double value);
static void KalmanLPF_SetR(KALMAN_LPF_t* ekf, int i, int j, double value);
static void KalmanLPF_CalculateModel(KALMAN_LPF_t* ekf, double fx[Nsta], double F[Nsta][Nsta], double hx[Mobs], double H[Mobs][Nsta]);

/** @brief Initializes an KALMAN Low pass filter structure
 *  @param [in]     KALMAN_LPF_t * ekf      pointer to EKF structure to initialize 
 *  @param [out]  None
 *  @return None
 */
void KalmanLPF_Initialize(KALMAN_LPF_t* ekf) {
    ekf_init(ekf, Nsta, Mobs);

    // We approximate the process noise using a small constant
    KalmanLPF_SetQ(ekf, 0, 0, 0.0001); //0.0001
    KalmanLPF_SetQ(ekf, 1, 1, 0.0001);
    KalmanLPF_SetQ(ekf, 2, 2, 0.0001);

    // Same for measurement noise
    KalmanLPF_SetR(ekf, 0, 0, 0.0077); //0.0077
    KalmanLPF_SetR(ekf, 1, 1, 0.1); //0.0010
    KalmanLPF_SetR(ekf, 2, 2, 0.1); //0.0010
}

/** @brief Runs one step of EKF prediction and update
 *  @param [in]     KALMAN_LPF_t * ekf      pointer to EKF structure 
 *                  double * z              pointer to bundle of data
 *  @param [out]  None
 *  @return None
 */
bool KalmanLPF_Step(KALMAN_LPF_t* ekf, double * z) {
    KalmanLPF_CalculateModel(ekf, ekf->fx, ekf->F, ekf->hx, ekf->H);

    return ekf_step(ekf, z) ? false : true;
}

/** @brief Returns the state element at a given index
 *  @param [in]     KALMAN_LPF_t * ekf      pointer to EKF structure 
 *                  int i       the index (at least 0 and less than <i>n</i>
 *  @param [out]    None
 *  @return         double      state value at index 
 */
double KalmanLPF_GetX(KALMAN_LPF_t* ekf, int i) {
    return ekf->x[i];
}

/** @brief Sets the specified value of the prediction error covariance. <i>P<sub>i,
 * j</sub> = value</i>
 *  @param [in]     KALMAN_LPF_t * ekf      pointer to EKF structure 
 *                  int i       row index
 *                  int j       column index
 *                  double value    value to set
 *  @param [out]    None
 *  @return         None
 */
void KalmanLPF_SetP(KALMAN_LPF_t* ekf, int i, int j, double value) {
    ekf->P[i][j] = value;
}

/** @brief Sets the specified value of the process noise covariance. <i>Q<sub>i,
 * j</sub> = value</i>
 *  @param [in]     KALMAN_LPF_t * ekf      pointer to EKF structure 
 *                  int i       row index
 *                  int j       column index
 *                  double value    value to set
 *  @param [out]    None
 *  @return         None
 */
void KalmanLPF_SetQ(KALMAN_LPF_t* ekf, int i, int j, double value) {
    ekf->Q[i][j] = value;
}

/** @brief Sets the specified value of the observation noise covariance. <i>R<sub>i,
 * j</sub> = value</i>
 *  @param [in]     KALMAN_LPF_t * ekf      pointer to EKF structure 
 *                  int i       row index
 *                  int j       column index
 *                  double value    value to set
 *  @param [out]    None
 *  @return         None
 */
void KalmanLPF_SetR(KALMAN_LPF_t* ekf, int i, int j, double value) {
    ekf->R[i][j] = value;
}

/** @brief Do Kalman Low Pass Filter calculation
 *  @param [in]     KALMAN_LPF_t * ekf      pointer to EKF structure 
 *                  double fx[]         state-transition function
 *                  double F[][]        Jacobian of process model
 *                  double hx[]         measurement function
 *                  double H[][]        Jacobian of measurement model
 *  @param [out]    None
 *  @return         None
 */
void KalmanLPF_CalculateModel(KALMAN_LPF_t* ekf, double fx[Nsta], double F[Nsta][Nsta], double hx[Mobs], double H[Mobs][Nsta]) {
    double* x = ekf->x;
    // Process model is f(x) = x
    fx[0] = x[0];
    fx[1] = x[1];
    fx[2] = x[2];

    // So process model  Jacobianis identity matrix
    F[0][0] = 1;
    F[1][1] = 1;
    F[2][2] = 1;

    // Measurement function simplifies the relationship between state and sensor readings for convenience.
    // A more realistic measurement function would distinguish between state value and measured value; e.g.:
    //   hx[0] = pow(this->x[0], 1.03);
    //   hx[1] = 1.005 * this->x[1];
    //   hx[2] = .9987 * this->x[1] + .001;
    hx[0] = x[0]; // Barometric pressure from previous state
    hx[1] = x[1]; // Baro temperature from previous state
    hx[2] = x[2]; // LM35 temperature from previous state

    // Jacobian of measurement function
    H[0][0] = 1; // Barometric pressure from previous state
    H[1][1] = 1; // Baro temperature from previous state
    H[2][2] = 1; // LM35 temperature from previous state
}
