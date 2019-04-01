#ifndef _SBUS_H
#define _SBUS_H

#include "sys.h"

void sbus_analysis(void);
void romote_process(void);

#pragma pack (1)
typedef struct _SBUS_Receive_Data {
    uint8_t 	startbyte;						//Start value
    uint16_t 	Channel_received_value[16];		//Channel received value:Only 11 bit
    uint8_t 	flags;							//Flag value
    uint8_t 	endbyte;                        //End value
} _SBUS_RECEIVE_DATA, *P_SBUS_RECEIVE_DATA;
#pragma pack () 

typedef struct
{
	float roll;
	float pitch;
	float thro;
	float yaw;
}RC_ANGLE;

extern RC_ANGLE Target;

extern u8 Flag;
extern u8 mode;
extern _SBUS_RECEIVE_DATA Sbus_Receive_Data;
extern unsigned char sbus_rx_buffer[25];

#endif
