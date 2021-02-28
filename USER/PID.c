#include "PID.H"
#include "control.h"
float P1 = 9.0;
float I1 = 0.03;
float D1 = 350.0;

float P2 = 9.0;//12
float I2 = 0.03;//0.1
float D2 = 350.0;

float P3 = 0.0;
float I3 = 0.0;
float D3 = 0.0; 

float P4 = 0.0;
float I4 = 0;
float D4 = 0.0; 

float P5 = 210.0;
float D5 = 0.0; 

float rp_pid(float rp_hat,float rp_now,float w_now)  //角度环pid ROLL通道
{
	float output; //输出
	static float err_rp;  //累计偏差
	float err_now;				//当前偏差
	static float err_last;
	
	err_now = rp_hat-rp_now;
	
	if((stop == 0)&(err_now<20)&(err_now>-20)) err_rp+=err_now;  //小角度积分
	if(err_rp>1500) err_rp=1500;                       //积分限幅 
	if(err_rp<(-1500)) err_rp=-1500;
		
	output =  P1*err_now + I1*err_rp + D1*(err_now-err_last);        
	err_last = err_now;
	return output; 
}

float rp_pid_i(float rp_hat,float rp_now,float w_now)  //角度环pid PITCH通道
{
	float output; //输出
	static float err_rp_i;  //累计偏差
	float err_now;				//当前偏差
	static float err_last;
	
	err_now = rp_hat-rp_now;
	
	if((stop == 0)&(err_now<20)&(err_now>-20)) err_rp_i+=err_now;  //小角度积分
	if(err_rp_i>1500) err_rp_i=1500;                       //积分限幅 
	if(err_rp_i<(-1500)) err_rp_i=-1500;
		
	output =  P2*err_now + I2*err_rp_i + D2*(err_now-err_last);        
	
	err_last = err_now;
	
	return output; 
}

int w_pid(float w_hat,float w_now)
{
	int output;
	static float err_i;
	static float err_last;
	float err_now;
	
	err_now = w_hat - w_now;
	
	if((stop==0)&(err_now<2)&(err_now>-2)) err_i+=err_now;  //小角速度积分
	if(err_i>1500) err_i=1500;                       //积分限幅 
	if(err_i<(-1500)) err_i=-1500;
	
	output = P3*err_now + I3*err_i + D3*(err_now-err_last);
  err_last = err_now; 
  
	return output;
}

int w_pid_i(float w_hat,float w_now)
{
	int output;
	static float err_i;
	static float err_last;
	float err_now;
	
	err_now = w_hat - w_now;
	
	if((stop==0 )&(err_now<10)&(err_now>-10)) err_i+=err_now;  //小角速度积分
	if(err_i>1500) err_i=1500;                       //积分限幅 
	if(err_i<(-1500)) err_i=-1500;
	
	output = P4*err_now + I4*err_i + D4*(err_now-err_last);
  err_last = err_now;
   
	return output;
}

int r_y_pid(float yaw_hat,float yaw_now)
{
  int output;
	float err;
	static float err_last;
	
	err = yaw_hat-yaw_now;
	
	output = err*P5 + (err - err_last )*D5;
	
	err_last  = err;
	return output ;
}



