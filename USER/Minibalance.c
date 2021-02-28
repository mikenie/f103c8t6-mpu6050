#include "sys.h"
#include "control.h"
#include "RC.h"
#include "led.h"
#include "iic.h"
#include "mpu6050.h"
#include "filter.h"
#include "SEND.H"
u8 Way_Angle=2;                             //获取角度的算法，1：四元数  2：卡尔曼  3：互补滤波 默认搭载卡尔曼滤波
u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu=2; //蓝牙遥控相关的变量
u8 Flag_Stop=0,Flag_Show=1;                 //停止标志位和 显示标志位 默认停止 显示打开
int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数
int Moto1,Moto2;                            //电机PWM变量 应是Motor的 向Moto致敬	
int Temperature;                            //显示温度
int Voltage;                                //电池电压采样相关的变量
float Angle_Balance,Gyro_Balance,Gyro_Turn; //平衡倾角 平衡陀螺仪 转向陀螺仪
float Show_Data_Mb;                         //全局显示变量，用于显示需要查看的数据
unsigned char ig=1;

int main(void)
{ 
	Stm32_Clock_Init(9);            //系统时钟设置
	delay_init(72);                 //延时初始化
	JTAG_Set(JTAG_SWD_DISABLE);     //=====关闭JTAG接口
	JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试
	LED_Init(); //板载 pc13 led
	
	uart_init(72,115200);          
	//uart2_init(36,9600);          
	MiniBalance_PWM_Init(7199,19);   //=====初始化PWM 500HZ,电调允许600最高来着 
	
	IIC_GPIO_Init();
	MPU6050_Init();
	
	
  Timer1_Init(99,7199);           //=====10MS进一次中断服务函数
	
	TIM2_2PWMin_Init(7200*4-1,20*2-1);  //调节捕获的时间。。。
	
	while(1)
		{		
			send_data();
			delay_ms(1);
		} 
}
