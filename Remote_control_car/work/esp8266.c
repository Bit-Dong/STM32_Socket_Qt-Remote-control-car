#include "esp8266.h"
#include "string.h"
#include "usart.h"
#include "usart2.h"
#include "stm32f10x.h"
#include "sys.h" 
#include "Delay.h"


//ESP8266模块和PC进入透传模式
void esp8266_start_trans(void)
{
	//printf("等待初始化\r\n");
	while(esp8266_send_cmd((u8 *)"AT",(u8 *)"OK",20));

	
	//设置工作模式 1：station模式   2：AP模式  3：兼容 AP+station模式
	while(esp8266_send_cmd((u8*)"AT+CWMODE=1",(u8*)"OK",20));
	//printf("设置工作模式成功\r\n");
	
	delay_ms(1000);

	//让模块连接上自己的路由
	//while(esp8266_send_cmd((u8*)"AT+CWJAP=\"聂冬的iPhone\",\"1810079793\"",(u8*)"WIFI GOT IP",500));
	while(esp8266_send_cmd((u8*)"AT+CWJAP=\"226\",\"226226226\"",(u8*)"WIFI GOT IP",200));
	//printf("连接路由器成功\r\n");
	delay_ms(1000);
	
	//=0：单路连接模式     =1：多路连接模式
	while(esp8266_send_cmd((u8*)"AT+CIPMUX=0",(u8*)"OK",200)){printf("设置单路连接模式失败\r\n");}
	//printf("设置单路连接模式成功\r\n");
	delay_ms(1000);

	//建立TCP连接  这四项分别代表了 要连接的ID号0~4   连接类型  远程服务器IP地址   远程服务器端口号
	while(esp8266_send_cmd((u8*)"AT+CIPSTART=\"TCP\",\"192.168.124.66\",8080",(u8*)"OK",500));
	//printf("TCP连接成功\r\n");
	delay_ms(1000);
	
	//是否开启透传模式  0：表示关闭 1：表示开启透传
	esp8266_send_cmd((u8*)"AT+CIPMODE=1",(u8*)"OK",200);
	//printf("开启透传模式\r\n");
	
	//透传模式下 开始发送数据的指令 这个指令之后就可以直接发数据了
	esp8266_send_cmd((u8*)"AT+CIPSEND",(u8*)"OK",50);
	//printf("开启透传成功\r\n");
}

//ESP8266退出透传模式   返回值:0,退出成功;1,退出失败
//配置wifi模块，通过想wifi模块连续发送3个+（每个+号之间 超过10ms,这样认为是连续三次发送+）
u8 esp8266_quit_trans(void)
{
	u8 result=1;
	u3_printf("+++");
	delay_ms(1000);					//等待500ms太少 要1000ms才可以退出
	result=esp8266_send_cmd((u8*)"AT",(u8*)"OK",20);//退出透传判断.
	if(result)
		printf("退出透传失败!\r\n");
	else
		printf("退出透传成功!\r\n");
	return result;
}


//向ESP8266发送命令
//cmd:发送的命令字符串;ack:期待的应答结果,如果为空,则表示不需要等待应答;waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果);1,发送失败
u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				
				if(esp8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 


//ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)
u8* esp8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


//向ESP8266发送数据
//cmd:发送的命令字符串;waittime:等待时间(单位:10ms)
//返回值:发送数据后，服务器的返回验证码
u8* esp8266_send_data(u8 *cmd,u16 waittime)
{
	char temp[5];
	char *ack=temp;
	USART2_RX_STA=0;
	u3_printf("%s",cmd);	//发送命令
	if(waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
				ack=(char*)USART2_RX_BUF;
				printf("ack:%s\r\n",(u8*)ack);
				USART2_RX_STA=0;
				break;//得到有效数据 
			} 
		}
	}
	return (u8*)ack;
} 

u8* WIFI_Rece_Data(void)
{
	u16 len=0;
	if( USART2_RX_STA & 0X8000 )
	{
		memset((char*)USART2_RX_BUF,0x00,len);//串口接收缓冲区清0
		len = USART2_RX_STA & 0X7FFF;
		USART2_RX_BUF[len]=0;//添加结束符
		USART2_RX_STA=0;
	}
	return USART2_RX_BUF;
}

