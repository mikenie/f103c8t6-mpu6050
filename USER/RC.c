#include "RC.h"
int pwm1,pwm2,pwm3,pwm4;
int pwm1_last,pwm2_last,pwm3_last,pwm4_last;
unsigned char t1=0,t2=0,t3=0,t4=0;
unsigned char f=1;

void TIM2_2PWMin_Init(u16 arr,u16 psc)
{
RCC->APB1ENR|=1<<0;
RCC->APB2ENR|=1<<2;
GPIOA->CRL&=0XFFFF0000;
GPIOA->CRL|=0X00008888;
TIM2->ARR=arr;
TIM2->PSC=psc;
TIM2->CCMR1=0X0101;
TIM2->CCMR2=0X0101;
TIM2->CCER|=0X0001;
TIM2->DIER|=0X001f;
TIM2->CR1|=0x01;
MY_NVIC_Init(0,1,TIM2_IRQn,2);
}


void TIM2_IRQHandler(void)
{
u16 TSR;
TSR=TIM2->SR;
	
if(TSR&(1<<1)) //通道1 
{
if(t1==0)
{
t1=1;
TIM2->CCER|=1<<1;
TIM2->CNT=0;
}
else
{
t1=0;
pwm1=TIM2->CCR1;
pwm1_last=pwm1;
TIM2->CCER&=~(3<<0);
f=2;
}
}

else if(TSR&(1<<2)) //通道2
{
if(t2==0)
{
t2=1;
TIM2->CCER|=1<<5;
TIM2->CNT=0;
}
else
{
t2=0;
pwm2=TIM2->CCR2;
pwm2_last=pwm2;
TIM2->CCER&=~(3<<4);
f=3;
}
}



else if(TSR&(1<<3))  //通道3
{
if(t3==0)
{
t3=1;
TIM2->CCER|=1<<9;
TIM2->CNT=0;
}
else
{
t3=0;
pwm3=TIM2->CCR3;
pwm3_last=pwm3;
TIM2->CCER&=~(3<<8);
f=4;
}
}


else if(TSR&(1<<4))  //通道4
{
if(t4==0)
{
t4=1;
TIM2->CCER|=1<<13;
TIM2->CNT=0;
}
else
{
t4=0;
pwm4=TIM2->CCR4;
pwm4_last=pwm4;
TIM2->CCER&=~(3<<12);
f=1;
}
}

else if(TSR&(1<<0))
{
TIM2->CCER&=~(1<<((f-1)*4));
if(f==1)
pwm1=pwm1_last;
else if(f==2)
pwm2=pwm2_last;
else if(f==3)
pwm3=pwm3_last;
else if(f==4)
pwm4=pwm4_last;
f++;
}

TIM2->CCER|=1<<((f-1)*4);
TIM2->CNT=0;
TIM2->SR=0;
}

