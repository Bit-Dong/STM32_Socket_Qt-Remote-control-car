#include "stm32f10x.h"                  // Device header
#include "pwm.h"

void TIM3_PWM_Init(u16 per,u16 psc)
{
	/*使能TIM4时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	/*使能GPIO*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	/*使能AFIO*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	/*配置GPIO*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	/*设置重映射*/
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//部分重映射	
	
	/*初始化定时器参数*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频为1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//选择计数模式为向上计数
	TIM_TimeBaseInitStructure.TIM_Period = per;//配置周期（ARR自动重装器的值）
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;//配置PSC预分频器的值
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器的值，高级计数器才需配置
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	//TIM_ClearFlag(TIM4,TIM_FLAG_Update);//先清除标志位，避免刚初始化就进入中断
	
	/*初始化PWM参数*/
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;   //选择空闲状态下的非工作状态 低电平
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;  //选择互补空闲状态下的非工作状态 低电平
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//选择PWM1模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性：高电平有效
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //输出比较使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;  //互补输出比较使能
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
	
	/*使能TIMX在CCRX上的预装载寄存器*/
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	
	/*使能TIMX在ARR上的预装载寄存器允许位*/
	//TIM_ARRPreloadConfig(TIM4,ENABLE);
	
	/*开启定时器*/
	TIM_Cmd(TIM3,ENABLE);
}	


