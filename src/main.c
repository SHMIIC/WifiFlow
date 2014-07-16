#include "rey_config.h"
#define suart2huart

void SendCommand(u8 *puts, u8 length);
//global value
unsigned int systick=0;
void main(void)
{
	unsigned char i=0;
	rey_usart_config();
  rey_timer_config();
	SOFT_UART_INIT();
	
	Initialize_LCD();
	ClearLine(1);
	ClearLine(2);
	
	P13 = 0;

	
	EA = 1;
	PutString(1,1,"--INFO1:123456--");
	PutString(2,1,"--INFO2:654321--");
	
//	SendCommand(start,ComanndLenth10);
//	SendCommand(getname,ComanndLenth10);
//	SendCommand(getuartmode,ComanndLenth10);
//	SendCommand(getdualup,ComanndLenth10);	
//	SendCommand(getmacaddr,ComanndLenth10);	
    while(1)
    {
// use to test soft uart send data to hard uart.  (not working!)			(内部短接硬件串口的发送和接收无效，外部跳帽短接可以收发)
#ifdef suart2huart
			if (REND)				//如果接收完,把接收到的值存入接收buff
		{
			REND = 0;
//			buf[r++ & 0x0f] = RBUF;
			TX1_write2buff(RBUF);
		}

		if(!TING)		//发送空闲
		{
	
					while(COM1.RX_Cnt != i)	//有要发送的数据
					{
						TBUF = RX1_Buffer[i++ &0x3f];
						TING = 1;
					}
					COM1.RX_Cnt = 0;
					i = 0;
		}
#endif
// use to test soft uart (sRX2sTX, already working!)
#ifdef suart2suart
		if(REND)
		{
			REND = 0;
			buf[r++ & 0x0f] =RBUF;
		}
		if(!TING)
		{
			if(r != t)
			{
				TBUF = buf[t++ &0x0f];
				TING = 1;
			}
		}
#endif
// use to test hard uart (hRX2hTX,  already working!)
#ifdef huart2huart
		if(COM1.RX_TimeOut != 0)
		{
			if(--COM1.RX_TimeOut == 0)
			{
				if(COM1.RX_Cnt != 0)
				{
					for(i=0;i<=COM1.RX_Cnt;i++) TX1_write2buff(RX1_Buffer[i]);
				}
				COM1.RX_Cnt = 0;
			}
		}
#endif
	}


				
}
void SendCommand(u8 *puts,u8 length)
{
			unsigned int i;
	    for (i=0; i< length;	i++)  
				{
					TX1_write2buff(*puts); 	//遇到停止符0结束
					puts++;
				}
			while(COM1.TX_Cnt != 0){;}
}


/********************* Timer0中断函数************************/
//#ifdef USE_TIMER0
//void timer0_int (void) interrupt TIMER0_VECTOR
//{
//    systick++;
//}
//#endif

/********************* Timer1中断函数************************/
#ifdef USE_TIMER1
void timer1_int (void) interrupt TIMER1_VECTOR
{

}
#endif
