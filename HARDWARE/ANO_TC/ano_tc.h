#ifndef _ANO_TC_H
#define _ANO_TC_H

#include "sys.h"

void usart1_send_char(u8 c);
void usart1_niming_report(u8 fun,u8*data,u8 len);
void ano_send_status(short Roll,short Pitch,short Yaw);
void ano_send_senser(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);

#endif
