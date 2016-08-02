#include "stm32f10x.h"
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
u8 Way_Angle=2;                             //��ȡ�Ƕȵ��㷨��1����Ԫ��  2��������  3�������˲� 
u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu=2; //����ң����صı���
u8 Flag_Stop=1;                 //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Encoder_Left,Encoder_Right;             //���ұ��������������
int Moto1,Moto2;                            //���PWM���� Ӧ��Motor�� ��Moto�¾�	
float Angle_Balance,Gyro_Balance,Gyro_Turn; //ƽ����� ƽ�������� ת��������
float Show_Data_Mb;                         //ȫ����ʾ������������ʾ��Ҫ�鿴������
u8 delay_50,delay_flag;         //Ĭ������£����������Ϲ��ܣ������û�����2s���Ͽ��Խ������ģʽ
float Acceleration_Z;                      //Z����ٶȼ�  
int main(void)
  { 
		delay_init();	    	            //=====��ʱ������ʼ��	
		uart_init(115200);	            //=====���ڳ�ʼ��Ϊ
		JTAG_Set(JTAG_SWD_DISABLE);     //=====�ر�JTAG�ӿ�
		JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
		LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
	  KEY_Init();                     //=====������ʼ��
    MiniBalance_PWM_Init(7199,0);   //=====��ʼ��PWM 10KHZ������������� �����ʼ������ӿ� 
		uart2_init(9600);               //=====����3��ʼ��
    Encoder_Init_TIM2();            //=====�������ӿ�
    Encoder_Init_TIM3();            //=====��ʼ��������2
    IIC_Init();                     //=====IIC��ʼ��
    MPU6050_initialize();           //=====MPU6050��ʼ��	  
	  DMP_Init();                     //=====��ʼ��DMP 
	  MiniBalance_EXTI_Init();        //=====MPU6050 5ms��ʱ�жϳ�ʼ��
		
    while(1)
	   {
				  delay_flag=1;	
					delay_50=0;
					while(delay_flag);	     //ͨ��MPU6050��INT�ж�ʵ�ֵ�50ms��׼��ʱ	
	   } 
}

