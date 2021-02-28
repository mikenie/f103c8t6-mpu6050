#include "control.h"	
#include "PID.h"
#include "led.h"
#include "RC.h"
#include "mpu6050.h"
#include "imu.h"
#include "filter.h"


/*
pwm1 1883 -> 3381   油门通道 
pwm2 1970 -> 3473 中立值2719  横滚 
pwm3 1937 -> 3440 中立值2689  俯仰
pwm4 1936 -> 3440 中立值2693  偏航
*/

float junzhi(float w_in);
float Roll_last,Pitch_last;
float w_out_filterd;

unsigned char count;

#define pwm1_min 1975
#define pwm1_max 3475
#define pwm1_mid 2720

#define pwm2_min 1887
#define pwm2_max 3392
#define pwm2_mid 2637

#define pwm3_min 1883
#define pwm3_max 3388
#define pwm3_mid 2620

#define pwm4_min 1955
#define pwm4_max 3457
#define pwm4_mid 2715

int duty = 3600;
float w_x,w_y,w_z;
unsigned int a_out,b_out,c_out,d_out;

unsigned char jiesuo=1; //解锁标志
unsigned char stop = 0; //紧急停机 
float a_r_hat = 0;
float a_p_hat = 0;
float a_y_hat = 0;
float c_a,c_b;

//整个控制过程
int TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//5ms定时中断
	{   
			TIM1->SR&=~(1<<0);                                       //===清除定时器1中断标志位	
		  
			Prepare_Data();
			IMUupdate(&Gyr_rad,&Acc_filt,&Att_Angle); //四元数姿态解算
				  
		  if(jiesuo == 1){			
			duty = 3500 +3600.0*(pwm3-pwm3_min)/(pwm3_max-pwm3_min);//油门 				
			a_r_hat = -60.0*(pwm1-pwm1_mid)/(pwm1_max-pwm1_min);//期望roll
			a_p_hat = 60.0*(pwm2-pwm2_mid)/(pwm2_max-pwm2_min);//期望pitch
		  a_y_hat = -10.0*(pwm4-pwm4_mid)/(pwm4_max-pwm4_min);//期望yaw
							
      b_out = duty + rp_pid_i(a_p_hat,Att_Angle.rol,Gyr_rad.Y)+r_y_pid(a_y_hat,Gyr_rad.Z);//采用速率陀螺仪输出，测速反馈获得更小超调。（若是比例微分环节，则更快调节时间）。
      d_out = duty - rp_pid_i(a_p_hat,Att_Angle.rol,Gyr_rad.Y)+r_y_pid(a_y_hat,Gyr_rad.Z);			
			
			a_out = duty + rp_pid(a_r_hat,Att_Angle.pit,Gyr_rad.X)-r_y_pid(a_y_hat,Gyr_rad.Z);
			c_out = duty - rp_pid(a_r_hat,Att_Angle.pit,Gyr_rad.X)-r_y_pid(a_y_hat,Gyr_rad.Z);	
								
			limit();   //限幅
						
			if(pwm3 < 1950) stop = 1; //油门做锁，油门打到底，无视角度，先锁一次再说
			else stop =0;           
			if( (myabs(Att_Angle.rol)>30) | (myabs(Att_Angle.pit)>30) ) stop = 1; //限制角度，最终锁
			
			if(stop == 0)        //正常				
			set_duty(a_out,b_out,c_out,d_out);	
			
			if(stop == 1) 
			set_duty(3600,3600,3600,3600);//紧急 
			
			Led_Flash(); //LED闪烁，用于抓取实际频率。
		}
		
	}       	
	 return 0;	  
} 




void limit(void){
	
	  if(a_out> 7100) a_out=7100 ;
	  if(b_out> 7100) b_out=7100 ;
		if(c_out> 7100) c_out=7100 ;
		if(d_out> 7100) d_out=7100 ;
	
}




/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

void set_duty(int width1,int width2,int width3,int width4){
				
   	TIM4->CCR1=width1;
		TIM4->CCR2=width2;
		TIM4->CCR3=width3;
		TIM4->CCR4=width4;
	
}

float junzhi(float w_in){
  static float w[3];
	unsigned char i ;
	float w_out;
	
	for(i=0;i<=2;i++)
	{
		w[i]=w[i+1];
	}
  w[2] = w_in;
  
	for(i=0;i<=2;i++)
	{
	w_out+=w[i];
	}
	w_out = w_out / 3.0;
	return w_out;
}
