#include "SEND.H"
#include "filter.h"
#include "control.h"
#include "usart.h"
#include "RC.h"
#include "control.h"

char mdata[30];
char tail[4] = {0x00, 0x00, 0x80, 0x7f};


void send_data()
{ 
	unsigned char i;
	float val;
	char *p;
		  p=(char *)&Att_Angle.rol;
		mdata[3]=*(p+3);
		mdata[2]=*(p+2);
		mdata[1]=*(p+1);
		mdata[0]=*(p);		
		  p=(char *)&Att_Angle.pit;
		mdata[7]=*(p+3);
		mdata[6]=*(p+2);
		mdata[5]=*(p+1);
		mdata[4]=*(p);
			p=(char *)&Att_Angle.yaw;
		mdata[11]=*(p+3);
		mdata[10]=*(p+2);
		mdata[9]=*(p+1);
		mdata[8]=*(p);
	
			p=(char *)&Gyr_rad.Y;
	  mdata[12]=*(p);	
		mdata[13]=*(p+1);
	  mdata[14]=*(p+2);
		mdata[15]=*(p+3);
			p=(char *)&Gyr_rad.Z;
	  mdata[16]=*(p);	
		mdata[17]=*(p+1);
	  mdata[18]=*(p+2);
		mdata[19]=*(p+3);
		   
			val = a_r_hat;
			p=(char *)&val;
	  mdata[20]=*(p);	
		mdata[21]=*(p+1);
	  mdata[22]=*(p+2);
		mdata[23]=*(p+3); 
		
			val = a_p_hat;
			p=(char *)&val;
	  mdata[24]=*(p);	
		mdata[25]=*(p+1);
	  mdata[26]=*(p+2);
		mdata[27]=*(p+3);
		
		
		
				
		for(i=0;i<=27;i++)
		{
		usart1_send(mdata[i]);
		}
		usart1_send(tail[0]);
		usart1_send(tail[1]);
		usart1_send(tail[2]);
		usart1_send(tail[3]);

}



