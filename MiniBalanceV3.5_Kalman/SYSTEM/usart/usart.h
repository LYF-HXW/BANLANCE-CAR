#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
void uart_init(u32 bound);
void USART1_SendChar(u8 b);
void handle_data(uint8_t buf[]);
#endif


