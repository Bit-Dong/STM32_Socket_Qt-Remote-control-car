//#include "stm32f10x.h"                  // Device header
//#include "encoder.h"
//#include "stdlib.h"
///*
//	配置定时中断流程：
//		1.RCC开启时钟（定时器基准时钟和外设工作时钟）
//		2.选择时基单元的时钟源（对于定时中断，选择内部时钟源）
//		3.配置时基单元（包括PSC预分频器，ARR自动重装器，CNT计数器）
//		4.配置输出中断控制，使能更新中断，允许更新中断输出到NVIC
//		5.配置NVIC，打开定时器通道，并分配优先级
//		6.运行控制，使能计数器
//*/

//void TIM4_Init(u16 per,u16 psc)
//{
//	/*1、开启通用定时器TIM4时钟*/
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
//	
//	/*2、选择时钟源为内部时钟*/
//	//TIM_InternalClockConfig(TIM4);//默认为内部时钟
//	
//	/*3、配置时基单元*/
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频为1分频
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//选择计数模式为向上计数
//	/*
//	定时频率 = 72M / (PSC+1) / (ARR+1)
//	*/
//	TIM_TimeBaseInitStructure.TIM_Period = per;//配置周期（ARR自动重装器的值）
//	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;//配置PSC预分频器的值
//	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
//	
//	/*4、使能更新中断*/
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
//	
//	/*5、配置NVIC*/
//	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//选择中断通道
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//开启中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级
//	NVIC_Init(&NVIC_InitStructure);	
//	
//	/*6、启动定时器*/
//	TIM_Cmd(TIM4,ENABLE);
//}

//float Kp=0.5;
//float Ki=0.4;
//float Kd= 0.1;
//int target_left=0;
//int target_right=0;

//int err_now1=0;
//int err_now2=0;
//int err_last1=0;
//int err_last2=0;
//int err_last_last1=0;
//int err_last_last2=0;
//int spd_now1=0;
//int spd_now2=0;
//int jisuan1=0;
//int jisuan2=0;
//int out_left=0;
//int out_right=0;

//void Change_Target(int t_l,int t_r)
//{
//	target_left = t_l;
//	target_right = t_r;
//}

//void TIM4_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
//	{
//		int cnt_left,cnt_right;
//		cnt_left = abs((TIM2->CNT)-0X7FFF);
//		cnt_right = abs((TIM3->CNT)-0X7FFF);
//		
//		TIM2->CNT = 0X7FFF;
//		TIM3->CNT = 0X7FFF;

//		
//		//左电机	
//		spd_now1 = cnt_left;
//		err_now1 = target_left - spd_now1;
//		jisuan1 = Kp*(err_now1-err_last1) + Ki*err_now1 + Kd*(err_now1+err_last_last1-2*err_last1);
//		out_left += jisuan1;
//		if(out_left<0)
//			out_left = 0;
//		if(out_left>100) 
//			out_left = 100; 
//		TIM_SetCompare1(TIM1,out_left);	   //左前
//		TIM_SetCompare2(TIM1,out_left);	  
//		err_last_last1 = err_last1;
//		err_last1 = err_now1;
//		
//		//右电机
//		spd_now2 = cnt_right;
//		err_now2 = target_right - spd_now2;
//		jisuan2 = Kp*(err_now2-err_last2) + Ki*err_now2 + Kd*(err_now2+err_last_last2-2*err_last2);
//		out_right += jisuan2;
//		if(out_right<0)
//			out_right = 0;
//		if(out_right>100) 
//			out_right = 100; 	
//		TIM_SetCompare3(TIM1,out_right);	 //右前轮
//		TIM_SetCompare4(TIM1,out_right);
//		err_last_last2 = err_last2;
//		err_last2 = err_now2; 
//		
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//	}
// 
//}

