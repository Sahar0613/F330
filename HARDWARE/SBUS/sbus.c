#include "sbus.h"
#include "usart.h"

u8 flag;
u8 mode;

void sbus_analysis(void)
{
	u8 t=0;
	Sbus_Receive_Data.startbyte = sbus_rx_buffer[0];
  Sbus_Receive_Data.endbyte = sbus_rx_buffer[24];
	
	if(Flag==1)
	{
		flag = 1;
		if (Sbus_Receive_Data.startbyte == 0x0f && Sbus_Receive_Data.endbyte == 0x00)
		{
			Sbus_Receive_Data.Channel_received_value[ 0] = (sbus_rx_buffer[ 1] | (sbus_rx_buffer[2] << 8)) &0x07ff;                                     //!< Channel 0
			Sbus_Receive_Data.Channel_received_value[ 1] = ((sbus_rx_buffer[ 2] >> 3) | (sbus_rx_buffer[3] << 5)) &0x07ff;                              //!< Channel 1
			Sbus_Receive_Data.Channel_received_value[ 2] = ((sbus_rx_buffer[ 3] >> 6) | (sbus_rx_buffer[4] << 2) | (sbus_rx_buffer[5] << 10)) &0x07ff;  //!< Channel 2 			????
			Sbus_Receive_Data.Channel_received_value[ 3] = ((sbus_rx_buffer[ 5] >> 1) | (sbus_rx_buffer[6] << 7)) &0x07ff;    							//!< Channel 3
			Sbus_Receive_Data.Channel_received_value[ 4] = ((sbus_rx_buffer[ 6] >> 4) | (sbus_rx_buffer[7] << 4)) &0x07ff;    							//!< Channel 4
			Sbus_Receive_Data.Channel_received_value[ 5] = ((sbus_rx_buffer[ 7] >> 7) | (sbus_rx_buffer[8] << 1) | (sbus_rx_buffer[9] << 9)) &0x07ff;	//!< Channel 5
			Sbus_Receive_Data.Channel_received_value[ 6] = ((sbus_rx_buffer[ 9] >> 2) | (sbus_rx_buffer[10] << 6)) &0x07ff;								//!< Channel 6
			Sbus_Receive_Data.Channel_received_value[ 7] = ((sbus_rx_buffer[10] >> 5) | (sbus_rx_buffer[11] << 3)) &0x07ff;								//!< Channel 7
			Sbus_Receive_Data.Channel_received_value[ 8] = (sbus_rx_buffer[12] | (sbus_rx_buffer[13] << 8)) &0x07ff;									//!< Channel 8
			Sbus_Receive_Data.Channel_received_value[ 9] = ((sbus_rx_buffer[13] >> 3) | (sbus_rx_buffer[14] << 5)) &0x07ff;								//!< Channel 9
			Sbus_Receive_Data.Channel_received_value[10] = ((sbus_rx_buffer[14] >> 6) | (sbus_rx_buffer[15] << 2) | (sbus_rx_buffer[16] << 10)) &0x07ff;//!< Channel 10
			Sbus_Receive_Data.Channel_received_value[11] = ((sbus_rx_buffer[16] >> 1) | (sbus_rx_buffer[17] << 7)) &0x07ff;								//!< Channel 11
			Sbus_Receive_Data.Channel_received_value[12] = ((sbus_rx_buffer[17] >> 4) | (sbus_rx_buffer[18] << 4)) &0x07ff;								//!< Channel 12
			Sbus_Receive_Data.Channel_received_value[13] = ((sbus_rx_buffer[18] >> 7) | (sbus_rx_buffer[19] << 1) | (sbus_rx_buffer[20] << 9)) &0x07ff;	//!< Channel 13
			Sbus_Receive_Data.Channel_received_value[14] = ((sbus_rx_buffer[20] >> 2) | (sbus_rx_buffer[21] << 6)) &0x07ff;								//!< Channel 14
			Sbus_Receive_Data.Channel_received_value[15] = ((sbus_rx_buffer[21] >> 5) | (sbus_rx_buffer[22] << 3)) &0x07ff;								//!< Channel 15

			Sbus_Receive_Data.flags = sbus_rx_buffer[23];
			for(t=0;t<4;t++)
			{
				printf("%d ",Sbus_Receive_Data.Channel_received_value[t]);
			}
//			printf("\r\n\r\n");
		}
		Flag = 0;  //标志位清零
	}
	else
	{
		Sbus_Receive_Data.Channel_received_value[0] = 0;	//!< Channel 0
		Sbus_Receive_Data.Channel_received_value[1] = 0;	//!< Channel 1
		Sbus_Receive_Data.Channel_received_value[2] = 0;	//!< Channel 2
		Sbus_Receive_Data.Channel_received_value[3] = 0;	//!< Channel 3
		Sbus_Receive_Data.Channel_received_value[4] = 0;	//!< Channel 4
		Sbus_Receive_Data.Channel_received_value[5] = 0;	//!< Channel 5
		Sbus_Receive_Data.Channel_received_value[6] = 0;	//!< Channel 6
		Sbus_Receive_Data.Channel_received_value[7] = 0;	//!< Channel 7

		Sbus_Receive_Data.Channel_received_value[8] = 0;	//!< Channel 8
		Sbus_Receive_Data.Channel_received_value[9] = 0;	//!< Channel 9
		Sbus_Receive_Data.Channel_received_value[10] = 0;	//!< Channel 10
		Sbus_Receive_Data.Channel_received_value[11] = 0;	//!< Channel 11
		Sbus_Receive_Data.Channel_received_value[12] = 0;	//!< Channel 12
		Sbus_Receive_Data.Channel_received_value[13] = 0;	//!< Channel 13
		Sbus_Receive_Data.Channel_received_value[14] = 0;	//!< Channel 14
		Sbus_Receive_Data.Channel_received_value[15] = 0;	//!< Channel 15

		Sbus_Receive_Data.flags = 0;
	}
}

void romote_process(void)
{
	
	if(flag == 1)
	{
		mode = 1;
		Target.roll = (((Sbus_Receive_Data.Channel_received_value[0]-179.0)/1419.0*1000.0-500.0)/500.0*30.0);
		if(Target.roll<=1&&Target.roll>=-1)Target.roll = 0;
		Target.pitch = (((Sbus_Receive_Data.Channel_received_value[1]-353.0)/1342.0*1000.0-500.0)/500.0*30.0);
		Target.thro = ((Sbus_Receive_Data.Channel_received_value[2]-175.0)/1520.0*1800.0+1800.0);
		Target.yaw = (((Sbus_Receive_Data.Channel_received_value[3]-179.0)/1419.0*1000.0-500.0)/500.0*100.0);
		if(Target.yaw<=5&&Target.yaw>=-5)Target.yaw = 0;
	}
	flag = 0;
}
