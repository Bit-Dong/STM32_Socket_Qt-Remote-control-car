#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "pwm.h"
#include "timer.h"
#include "car.h"
#include "usart.h"
#include "usart2.h"
#include "esp8266.h"

/*
	TIM3(高级定时器)：CH1-CH4: PWM输出控制四个电机  
		PA6    左前     
		PA7    左后     
		PB0   右前     
		PB1   右后
		
	TIM4:定时测速调速 
	
	红外输出：
	PB3：左
	PB4：中
  PB5: 右
	
	TB6612N逻辑输入：（正转  反转）
	PB12,PB13:左前	 通道1
	PB10,PB11:左后   通道2
	PB14,PB15:右前   通道3
	PB7,PB8:  右后	 通道4
	
	ESP8266的连接：USART2（PA2、PA3）
	如何判断数据接收完全？
1、出现了换行符；
2、如果超过10ms了都没有下一条数据（TIM2来进行10ms的定时）
*/


#define L GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define M GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)
#define R GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)


int main(void)
{
	unsigned char*	m=NULL;
	 int k1,k2,k3,k4,flag,f1,f2,f3;
	delay_init();	    	 			//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 			//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 				//串口初始化为115200
	USART2_init(115200);	 				//串口初始化为115200
	 
	 esp8266_quit_trans();
	esp8266_start_trans();							//esp8266进行初始化
	 
	GPIOB_Init();
	
	TIM3_PWM_Init(100-1,72-1);
	int i,j=15;
	flag=1;
	while(1)
	{
		m=WIFI_Rece_Data();	
		if(flag == 1) 
		{
			m[0]='S';
			flag = 0;
		}

		else if(m[0]=='F')  i=0;
		
		else if(m[0]=='L')  i=1;
		
		else if(m[0]=='R')  i=3;
		
		else if(m[0]=='S')  i=5;
		
		else if(m[0]=='B')  i=7;
		
		else if(m[0]=='u'&& m[1]=='p')  i=2;
		
		else if(m[0]=='u'&& m[1]=='d')  i=4;
		
		switch(i)
		{
			case 0:
				f1=1;
				setpwm(j);
				forward();
			  break;
			
			case 1:
				f2=1;
				k1=j-15;
				k2=j+15;
				if(k1<=0) 	k1=0;
				if(k2>=100) k2=100;
				setpwm1(k1,k2);
			
				if(f1==1)  turn_left();	
				if(f1==-1) turn_left1();	
			  break;

			case 3:
				f3=1;
				k3=j+15;
				k4=j-15;
				if(k3>=100) k3=100;
				if(k4<=0) 	k4=0;
				setpwm1(k3,k4);
			
				if(f1==1)  turn_right();
				if(f1==-1) turn_right1();
			  break;

			case 5:
				stop();	
				break;
			
			case 7:
				f1=-1;
				setpwm(j);
				back();
				break;
			
			case 2:
					if(f2==1) 
					{ 
							k1+=2;k2+=10; 
							if(k1>=30) k1=30; 
							if(k2>=100) k2=100; 
							setpwm1(k1,k2);
							delay_ms(20);
							f2=0;
							break;
					} 
					else if(f3==1) 
						{ k3+=10;k4+=2; 
							if(k3>=100) k3=100; 
							if(k4>=30) k4=30; 
							setpwm1(k3,k4);
							delay_ms(20);
							f3=0;
							break;
					} 
					else {
						if(j>=100) j=100;	
						setpwm(j);
						j++;
						delay_ms(50);
						break;
					}
			
			case 4:
					if(f2==1) { k1-=2;k2-=5; if(k1<=0) k1=0; if(k2<=30) k2=30; setpwm1(k1,k2); delay_ms(20);f2=0; break;} 
					else if(f3==1) { k3-=5;k4-=2; if(k3<=30) k3=30; if(k4<=0) k4=0; setpwm1(k3,k4); delay_ms(20); f3=0;break;} 
					else{
							if(j<=0) j=0;
							setpwm(j);
							j--;
							delay_ms(50);
							break;
					}
					
		}
		
		
	}
}


































/*串口中断服务函数*/
//void USART1_IRQHandler(void)
//{
//	u8 data = 0;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
//	{
//		data = USART_ReceiveData(USART1);
//		//USART_SendData(USART1,data);
//		switch(data)
//		{
//			case 0x00://0 停止
//				stop();
//				break;
//			case 0x01://1 前进
//				forward(target);
//				break;
//			case 0x02://2 后退
//				back(target);
//				break;
//			case 0x03://3 左转
//				turn_left(target_left,target_right);
//				break;
//			case 0x04://4 右转
//				turn_right(target_left,target_right);
//				break;
//			case 0x05://5 慢速
//				Change_Target(2,2);
//				target = 2;
//				break;
//			case 0x06://6 中速
//				Change_Target(8,8);
//				target = 8;
//				break;
//			case 0x07://7 快速
//				Change_Target(15,15);
//				target = 15;
//				break;
//		}
//	}
//}
