#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define PWMA   TIM4->CCR3 
#define AIN1   PBout(4)
#define AIN2   PBout(5)
#define BIN1   PBout(6)
#define BIN2   PBout(7)
#define PWMB   TIM4->CCR4  
void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
#endif
