#include "sys.h"
#include "usart.h"	
#include "control.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//Flag_Show!=0  ʹ�ô���1   
	USART1->DR = (u8) ch;      	
	return ch;
}
#endif 

//#if EN_USART1_RX   //���ʹ���˽���

void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}
void USART1_SendChar(u8 b)
{
	 USART_SendData(USART1,b);
		while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
		
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{	
			
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		}
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
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
