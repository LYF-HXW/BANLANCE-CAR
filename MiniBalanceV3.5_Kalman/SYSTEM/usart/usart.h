#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
void uart_init(u32 bound);
void USART1_SendChar(u8 b);
void handle_data(uint8_t buf[]);
#endif


