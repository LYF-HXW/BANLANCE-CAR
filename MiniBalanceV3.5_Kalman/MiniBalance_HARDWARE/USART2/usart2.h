#ifndef __USRAT3_H
#define __USRAT3_H 
#include "sys.h"	  	
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
extern u8 Usart2_Receive,jianyan;
void uart2_init(u32 bound);
void USART2_SendChar(u8 b);
void USART2_IRQHandler(void);
#endif

