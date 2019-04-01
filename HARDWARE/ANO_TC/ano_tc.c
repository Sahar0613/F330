#include "ano_tc.h"
#include "stm32f10x.h"                  // Device header

//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{   	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
	USART_SendData(USART1,c);  
} 

//传送数据给匿名四轴上位机软件(V4.34版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+4]=0;	//校验数置零
	send_buf[0]=0xAA;	//帧头
	send_buf[1]=0xAA;
	send_buf[2]=fun;	//功能字
	send_buf[3]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[4+i]=data[i];			//复制数据
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+5;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}

void ano_send_status(short Roll,short Pitch,short Yaw)
{
	u8 tbuf[6];
	tbuf[0]=(Roll>>8)&0XFF;
	tbuf[1]=Roll&0XFF;
	tbuf[2]=(Pitch>>8)&0XFF;
	tbuf[3]=Pitch&0XFF;
	tbuf[4]=(Yaw>>8)&0XFF;
	tbuf[5]=Yaw&0XFF;
	usart1_niming_report(0x01,tbuf,6);
}

void ano_send_senser(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12];
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF;
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart1_niming_report(0x02,tbuf,12);
}
