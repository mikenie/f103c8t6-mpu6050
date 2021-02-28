
#include "usart2.h"

 u8 mode_data[8];
 u8 six_data_stop[3]={0X59,0X59,0X59};  //停止数据样本
 u8 six_data_start[3]={0X58,0X58,0X58};  //启动数据样本

/**************************************************************************
函数功能：串口2初始化
入口参数：pclk2:PCLK2 时钟频率(Mhz)    bound:波特率
返回  值：无
**************************************************************************/
void uart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;  //使能串口时钟 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO状态设置
	GPIOA->ODR|=1<<2;	  
	RCC->APB1RSTR|=1<<17;   //复位串口1
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; // 波特率设置	 
	USART2->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断
	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART2_IRQn,2);//组2，最低优先级 
}

/**************************************************************************
函数功能：串口2接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART2_IRQHandler(void)
{	
if(USART2->SR&(1<<5))//接收到数据
	{	  
	 
	 static	int uart_receive=0;//蓝牙接收相关变量
	 uart_receive=USART2->DR; 
		if(uart_receive==0x59)  Flag_sudu=2;  //低速挡（默认值）
		if(uart_receive==0x58)  Flag_sudu=1;  //高速档
		
		if(uart_receive==0x00)	
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
		else if(uart_receive==0x01)
		{
			
			//while(1){PWMA=6000;PWMB=6000;}
			Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////前
		}
			else if(uart_receive==0x05)
			Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////后
		else if(uart_receive==0x02||uart_receive==0x03||uart_receive==0x04)	
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;  //左
		else if(uart_receive==0x06||uart_receive==0x07||uart_receive==0x08)	
			Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0; //右
		else 
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
	}  											 				 
} 

