#include "sys.h"
#include "usart.h"	
#include "control.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//Flag_Show!=0  使用串口1   
	USART1->DR = (u8) ch;      	
	return ch;
}
#endif 

//#if EN_USART1_RX   //如果使能了接收

void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}
void USART1_SendChar(u8 b)
{
	 USART_SendData(USART1,b);
		while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
		
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{	
			
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		}
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}

void handle_data(uint8_t buf[])
{		
		u8 byte1[4],byte2[4];
		jianyan=1;
		byte1[0] = buf[0];
		byte1[1] = buf[1];
		byte1[2] = buf[2];
		byte1[3] = buf[3];	
    memcpy(&Target_Velocity,byte1,sizeof(int));	
	
		byte2[0] = buf[4];
		byte2[1] = buf[5];
		byte2[2] = buf[6];
		byte2[3] = buf[7];	
    memcpy(&Turn_Target,byte2,sizeof(int));	 

		if(Turn_Target>12&&Target_Velocity<=8&&Target_Velocity>=-8)
		{
			Flag_Left=1;
			Flag_Right=0;
			Flag_Qian=0;
			Flag_Hou=0;
		}
		else if(Turn_Target<-12&&Target_Velocity<=8&&Target_Velocity>=-8)
		{	
			Flag_Left=0;
			Flag_Right=1;
			Flag_Qian=0;
			Flag_Hou=0;
		}
		else if(Target_Velocity>8&&Turn_Target>=-12&&Turn_Target<=12)
		{
			Flag_Left=0;
			Flag_Right=0;
			Flag_Qian=1;
			Flag_Hou=0;
		}
		else if(Target_Velocity<-8&&Turn_Target>=-12&&Turn_Target<=12)
		{
			Flag_Left=0;
			Flag_Right=0;
			Flag_Qian=0;
			Flag_Hou=1;
		}
		else 
		{
			Flag_Left=0;
			Flag_Right=0;
			Flag_Qian=0;
			Flag_Hou=0;
		}
}	

//#endif	
