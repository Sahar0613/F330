#include "timer.h"

volatile u16 loop1ms,loop2ms=0;
u8 loop1msFlag,loop2msFlag=0;


void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  


	TIM_Cmd(TIM3, ENABLE); 
}

void TIM3_IRQHandler(void)  
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
		{
			loop1ms++;
			loop2ms++;
			if(loop1ms>1)
			{
				loop1ms = 0;
				loop1msFlag = 1;
			}
			if(loop2ms>2)
			{
				loop2ms = 0;
				loop2msFlag = 1;
			}
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  
		}
}
