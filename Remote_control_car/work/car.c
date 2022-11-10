#include "stm32f10x.h"                  // Device header
#include "pwm.h"
#include "car.h"


void GPIOB_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/*红外out引脚：PB3,PB4,PB5*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*初始为低电平*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

}


void forward()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_7| GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_15);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8| GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14);
}

void back()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_8| GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7| GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_15);
}

//  PB7,PB8:  右后	 
//	PB14,PB15:右前   
//	PB12,PB13:左前	 
//	PB10,PB11:左后   

void turn_left()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_7| GPIO_Pin_10 | GPIO_Pin_13 | GPIO_Pin_15);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8| GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14);
}

void turn_left1()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_8| GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7| GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15);
}

void turn_right()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_8| GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_15);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7| GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14);
}

void turn_right1()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_8| GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7| GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14);
}

void stop(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}


void setpwm(int i)
{
	TIM_SetCompare1(TIM3,i);
	TIM_SetCompare2(TIM3,i);
	TIM_SetCompare3(TIM3,i);
	TIM_SetCompare4(TIM3,i);
}
void setpwm1(int i,int j)
{
	TIM_SetCompare1(TIM3,i);
	TIM_SetCompare2(TIM3,i);
	TIM_SetCompare3(TIM3,j);
	TIM_SetCompare4(TIM3,j);
}

