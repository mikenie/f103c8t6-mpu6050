#ifndef PID_H
#define PID_H
extern float rp_pid(float rp_hat,float rp_now,float w_now);
extern float rp_pid_i(float rp_hat,float rp_now,float w_now);  

extern int w_pid(float w_hat,float w_now);
extern int w_pid_i(float w_hat,float w_now);

extern int r_y_pid(float yaw_hat,float yaw_now);

extern float P1,I1,D1;  //ROLL参数
extern float P2,I2,D2;  //PITCH参数
#endif

