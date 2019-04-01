#include "motor.h"

void MotorPwmFlash(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM)
{		
	if(MOTO1_PWM>=Moto_PwmMax)	MOTO1_PWM = Moto_PwmMax;
	if(MOTO2_PWM>=Moto_PwmMax)	MOTO2_PWM = Moto_PwmMax;
	if(MOTO3_PWM>=Moto_PwmMax)	MOTO3_PWM = Moto_PwmMax;
	if(MOTO4_PWM>=Moto_PwmMax)	MOTO4_PWM = Moto_PwmMax;

	if(MOTO1_PWM<=Moto_PwmMin)	MOTO1_PWM = Moto_PwmMin;
	if(MOTO2_PWM<=Moto_PwmMin)	MOTO2_PWM = Moto_PwmMin;
	if(MOTO3_PWM<=Moto_PwmMin)	MOTO3_PWM = Moto_PwmMin;
	if(MOTO4_PWM<=Moto_PwmMin)	MOTO4_PWM = Moto_PwmMin;

	TIM2->CCR1 = MOTO1_PWM;
	TIM2->CCR2 = MOTO2_PWM;
	TIM2->CCR3 = MOTO3_PWM;
	TIM2->CCR4 = MOTO4_PWM;       
}

//TIM2 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
 
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
}

void motor_Stop_output(void)
{
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 0;
	TIM2->CCR4 = 0;
}

void motor_init(void)
{
	TIM2_PWM_Init(35999,39);
	motor_Stop_output();
	
	motor_start_output(1800);
	delay_ms(1000);
	delay_ms(1000);
}

void motor_start_output(u16 m)
{
	TIM2->CCR1 = m;
	delay_ms(100);
	TIM2->CCR2 = m;
	delay_ms(100);
	TIM2->CCR3 = m;
	delay_ms(100);
	TIM2->CCR4 = m;
	delay_ms(100);
}
