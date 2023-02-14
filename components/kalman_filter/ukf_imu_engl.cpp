/*************************************************************************************************************
 *  
 * In this example, we'll process IMU data from MPU9250 sensor.
 * 
 * See https://github.com/pronenewbits for more!
 ************************************************************************************************************/
#include <Wire.h>
#include "ukf_imu_engl.h"

/* ================================================== The AHRS/IMU variables ================================================== */
/* Gravity vector constant (align with global Z-axis) */
#define IMU_ACC_Z0          (1)


/* ============================================ UKF variables/function declaration ============================================ */
/* Just example; in konfig.h: 
 *  SS_X_LEN = 4
 *  SS_Z_LEN = 3
 *  SS_U_LEN = 3
 */
/* UKF initialization constant -------------------------------------------------------------------------------------- */
#define P_INIT      (10.)
#define Rv_INIT     (1e-6)
#define Rn_INIT_ACC (0.0015)
/* P(k=0) variable -------------------------------------------------------------------------------------------------- */
float_prec UKF_PINIT_data[SS_X_LEN*SS_X_LEN] = {P_INIT, 0,      0,      0,
                                                0,      P_INIT, 0,      0,
                                                0,      0,      P_INIT, 0,
                                                0,      0,      0,      P_INIT};
Matrix UKF_PINIT(SS_X_LEN, SS_X_LEN, UKF_PINIT_data);
/* Q constant ------------------------------------------------------------------------------------------------------- */
float_prec UKF_RVINIT_data[SS_X_LEN*SS_X_LEN] = {Rv_INIT, 0,     0,     0,
                                                0,      Rv_INIT, 0,      0,
                                                0,      0,      Rv_INIT, 0,
                                                0,      0,      0,      Rv_INIT};
Matrix UKF_RvINIT(SS_X_LEN, SS_X_LEN, UKF_RVINIT_data);
/* R constant ------------------------------------------------------------------------------------------------------- */
float_prec UKF_RNINIT_data[SS_Z_LEN*SS_Z_LEN] = {Rn_INIT_ACC, 0,          0,
                                                 0,          Rn_INIT_ACC, 0,
                                                 0,          0,          Rn_INIT_ACC};
Matrix UKF_RnINIT(SS_Z_LEN, SS_Z_LEN, UKF_RNINIT_data);
/* Nonlinear & linearization function ------------------------------------------------------------------------------- */
bool Main_bUpdateNonlinearX(Matrix& X_Next, const Matrix& X, const Matrix& U);
bool Main_bUpdateNonlinearY(Matrix& Y, const Matrix& X, const Matrix& U);
/* UKF variables ---------------------------------------------------------------------------------------------------- */
Matrix quaternionData(SS_X_LEN, 1);
Matrix Y(SS_Z_LEN, 1);
Matrix U(SS_U_LEN, 1);
/* UKF system declaration ------------------------------------------------------------------------------------------- */
UKF UKF_IMU(quaternionData, UKF_PINIT, UKF_RvINIT, UKF_RnINIT, Main_bUpdateNonlinearX, Main_bUpdateNonlinearY);



/* ========================================= Auxiliary variables/function declaration ========================================= */
uint64_t u64compuTime;
char bufferTxSer[100];
/* A SimpleMPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68 */

UKF get_UKF(){ return UKF_IMU; }

void setup() {
    Serial.begin(115200);

    /* UKF initialization ----------------------------------------- */
    /* x(k=0) = [1 0 0 0]' */
    quaternionData.vSetToZero();
    quaternionData[0][0] = 1.0;
    UKF_IMU.vReset(quaternionData, UKF_PINIT, UKF_RvINIT, UKF_RnINIT);
    
    snprintf(bufferTxSer, sizeof(bufferTxSer)-1, "UKF in Teensy 4.0 (%s)\r\n",
                                                 (FPU_PRECISION == PRECISION_SINGLE)?"Float32":"Double64");
    Serial.print(bufferTxSer);
}

void set_sensor_data(Vector accel, Vector gyro){
        /* Input 1:3 = gyroscope */
        U[0][0] = gyro.XAxis;  U[1][0] = gyro.YAxis;  U[2][0] = gyro.ZAxis;
        /* Output 1:3 = accelerometer */
        Y[0][0] = accel.XAxis; Y[1][0] = accel.YAxis; Y[2][0] = accel.ZAxis;

        float_prec _normG = sqrt(Y[0][0] * Y[0][0]) + (Y[1][0] * Y[1][0]) + (Y[2][0] * Y[2][0]);
        Y[0][0] = Y[0][0] / _normG;
        Y[1][0] = Y[1][0] / _normG;
        Y[2][0] = Y[2][0] / _normG;
}

void update_filter(){
        u64compuTime = micros();
        if (!UKF_IMU.bUpdate(Y, U)) {
            quaternionData.vSetToZero();
            quaternionData[0][0] = 1.0;
            UKF_IMU.vReset(quaternionData, UKF_PINIT, UKF_RvINIT, UKF_RnINIT);
            Serial.println("Whoop ");
        }
        u64compuTime = (micros() - u64compuTime);
}

// void loop() {
//     if (timerUKF >= SS_DT_MILIS) {
//         timerUKF = 0;
        
        
//         /* ================== Read the sensor data / simulate the system here ================== */
        
//         /* ------------------ Read the sensor data / simulate the system here ------------------ */
        
        
//         /* ============================= Update the Kalman Filter ============================== */
//         /* ----------------------------- Update the Kalman Filter ------------------------------ */
//     }
    
    
//     /* The serial data is sent by responding to command from the PC running Processing scipt */
//     if (Serial.available()) {
//         cmd = Serial.read();
//         if (cmd == 'v') {
//             snprintf(bufferTxSer, sizeof(bufferTxSer)-1, "UKF in Teensy 4.0 (%s)\r\n",
//                                                          (FPU_PRECISION == PRECISION_SINGLE)?"Float32":"Double64");
//             Serial.print(bufferTxSer);
//             Serial.print('\n');
//         } else if (cmd == 'q') {
//             /* =========================== Print to serial (for plotting) ========================== */
//             quaternionData = UKF_IMU.GetX();

//             while (!Serial.available());
//             uint8_t count = Serial.read();
//             for (uint8_t _i = 0; _i < count; _i++) {
//                 serialFloatPrint(quaternionData[0][0]);
//                 Serial.print(",");
//                 serialFloatPrint(quaternionData[1][0]);
//                 Serial.print(",");
//                 serialFloatPrint(quaternionData[2][0]);
//                 Serial.print(",");
//                 serialFloatPrint(quaternionData[3][0]);
//                 Serial.print(",");
//                 serialFloatPrint((float)u64compuTime);
//                 Serial.print(",");
//                 Serial.println("");
//             }
//             /* --------------------------- Print to serial (for plotting) -------------------------- */
//         }
//     }
// }


/* Function to interface with the Processing script in the PC */
void serialFloatPrint(float f) {
    byte * b = (byte *) &f;
    for (int i = 0; i < 4; i++) {
        byte b1 = (b[i] >> 4) & 0x0f;
        byte b2 = (b[i] & 0x0f);

        char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
        char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;

        Serial.print(c1);
        Serial.print(c2);
    }
}


bool Main_bUpdateNonlinearX(Matrix& X_Next, const Matrix& X, const Matrix& U)
{
    /* Insert the nonlinear update transformation here
     *          x(k+1) = f[x(k), u(k)]
     *
     * The quaternion update function:
     *  q0_dot = 1/2. * (  0   - p*q1 - q*q2 - r*q3)
     *  q1_dot = 1/2. * ( p*q0 +   0  + r*q2 - q*q3)
     *  q2_dot = 1/2. * ( q*q0 - r*q1 +  0   + p*q3)
     *  q3_dot = 1/2. * ( r*q0 + q*q1 - p*q2 +  0  )
     * 
     * Euler method for integration:
     *  q0 = q0 + q0_dot * dT;
     *  q1 = q1 + q1_dot * dT;
     *  q2 = q2 + q2_dot * dT;
     *  q3 = q3 + q3_dot * dT;
     */
    float_prec q0, q1, q2, q3;
    float_prec p, q, r;
    
    q0 = X[0][0];
    q1 = X[1][0];
    q2 = X[2][0];
    q3 = X[3][0];
    
    p = U[0][0];
    q = U[1][0];
    r = U[2][0];
    
    X_Next[0][0] = (0.5 * (+0.00 -p*q1 -q*q2 -r*q3))*SS_DT + q0;
    X_Next[1][0] = (0.5 * (+p*q0 +0.00 +r*q2 -q*q3))*SS_DT + q1;
    X_Next[2][0] = (0.5 * (+q*q0 -r*q1 +0.00 +p*q3))*SS_DT + q2;
    X_Next[3][0] = (0.5 * (+r*q0 +q*q1 -p*q2 +0.00))*SS_DT + q3;

    
    
    /* ======= Additional ad-hoc quaternion normalization to make sure the quaternion is a unit vector (i.e. ||q|| = 1) ======= */
    if (!X_Next.bNormVector()) {
        /* System error, return false so we can reset the UKF */
        return false;
    }
    
    return true;
}

bool Main_bUpdateNonlinearY(Matrix& Y, const Matrix& X, const Matrix& U)
{
    /* Insert the nonlinear measurement transformation here
     *          y(k)   = h[x(k), u(k)]
     *
     * The measurement output is the gravitational and magnetic projection to the body:
     *     DCM     = [(+(q0**2)+(q1**2)-(q2**2)-(q3**2)),                    2*(q1*q2+q0*q3),                    2*(q1*q3-q0*q2)]
     *               [                   2*(q1*q2-q0*q3), (+(q0**2)-(q1**2)+(q2**2)-(q3**2)),                    2*(q2*q3+q0*q1)]
     *               [                   2*(q1*q3+q0*q2),                    2*(q2*q3-q0*q1), (+(q0**2)-(q1**2)-(q2**2)+(q3**2))]
     * 
     *  G_proj_sens = DCM * [0 0 1]             --> Gravitational projection to the accelerometer sensor
     *  M_proj_sens = DCM * [Mx My Mz]          --> (Earth) magnetic projection to the magnetometer sensor
     */
    float_prec q0, q1, q2, q3;
    float_prec q0_2, q1_2, q2_2, q3_2;

    q0 = X[0][0];
    q1 = X[1][0];
    q2 = X[2][0];
    q3 = X[3][0];

    q0_2 = q0 * q0;
    q1_2 = q1 * q1;
    q2_2 = q2 * q2;
    q3_2 = q3 * q3;
    
    Y[0][0] = (2*q1*q3 -2*q0*q2) * IMU_ACC_Z0;

    Y[1][0] = (2*q2*q3 +2*q0*q1) * IMU_ACC_Z0;

    Y[2][0] = (+(q0_2) -(q1_2) -(q2_2) +(q3_2)) * IMU_ACC_Z0;
    
    
    return true;
}





void SPEW_THE_ERROR(char const * str)
{
    #if (SYSTEM_IMPLEMENTATION == SYSTEM_IMPLEMENTATION_PC)
        cout << (str) << endl;
    #elif (SYSTEM_IMPLEMENTATION == SYSTEM_IMPLEMENTATION_EMBEDDED_ARDUINO)
        Serial.println(str);
    #else
        /* Silent function */
    #endif
    while(1);
}
