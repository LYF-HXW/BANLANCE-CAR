#include "usart2.h"
#include "usart.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define MAX_DATA_LEN 100
u8 Usart3_Receive;
 u8 mode_data[8];
 u8 six_data_stop[3]={0X59,0X59,0X59};  //停止数据样本
 u8 six_data_start[3]={0X58,0X58,0X58};  //启动数据样本

/**************************************************************************
函数功能：串口3初始化
入口参数： bound:波特率
返回  值：无
**************************************************************************/
void uart2_init(u32 bound)
{  	 
	  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能UGPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART3时钟
	//USART2_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  //USART2_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA.3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure);     //初始化串口2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口2 
}

void USART2_SendChar(u8 b)
{
	 USART_SendData(USART2,b);
		while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
}
/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART2_IRQHandler(void)
{		uint8_t data;
		static uint16_t checksum;
		static uint8_t  buffer[MAX_DATA_LEN] = {0};
		static int cur=0;
		static uint8_t datalen1;
		static short data_len;
		static enum Recstate								   		//状态机 
		{
		RECFF1,RECFF2,SENDID,RECLEN1,RECLEN2,RECSEL,RECCHECK
		} rs485Recstate = RECFF1;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
	{	  
		 data = USART_ReceiveData(USART2);		
		 switch (rs485Recstate) 
		 {
			case RECFF1:
				if (data == 0xff)	  
				{
					rs485Recstate = RECFF2;
					checksum=0;		
					cur = 0;								//校验位清零
				}
				break;
	
			case RECFF2:
				
				if (data == 0xff)
					rs485Recstate = RECLEN1;
				else
					rs485Recstate = RECFF1;
				break;
	
			case RECLEN1:		
				checksum += data;
				datalen1 = data;
				rs485Recstate = RECLEN2;	  
				break;
		
			case RECLEN2:	
				checksum += data;			 				
				data_len = (short)datalen1 << 8	 | data;
				rs485Recstate = RECSEL;	  
				break;
	
			case RECSEL:
				checksum += data;
				buffer[cur++] = data;
				if(cur >= data_len)
					rs485Recstate = RECCHECK;
				break;
	
			case RECCHECK:
				checksum=checksum%256;
				if(data == checksum)
				{		
					handle_data(buffer);			
					checksum=0;	
					rs485Recstate = RECFF1;	 
				}
				
				else
					rs485Recstate = RECFF1;
				break;
			 default:
					rs485Recstate = RECFF1;
			}	   	 
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}  											 
} 


