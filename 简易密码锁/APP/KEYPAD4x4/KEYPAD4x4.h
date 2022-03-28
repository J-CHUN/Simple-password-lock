#ifndef __KEYPAD4x4_H
#define __KEYPAD4x4_H	 
#include "system.h"
#include "SysTick.h"


#define KEYPAD4x4PORT	GPIOA	//����IO�ӿ���
#define KEY1	GPIO_Pin_0	//����IO�ӿ�
#define KEY2	GPIO_Pin_1	//����IO�ӿ�
#define KEY3	GPIO_Pin_2	//����IO�ӿ�
#define KEY4	GPIO_Pin_3	//����IO�ӿ�
#define KEYa	GPIO_Pin_4	//����IO�ӿ�
#define KEYb	GPIO_Pin_5	//����IO�ӿ�
#define KEYc	GPIO_Pin_6	//����IO�ӿ�
#define KEYd	GPIO_Pin_7	//����IO�ӿ�


void KEYPAD4x4_Init(void);//��ʼ��
void KEYPAD4x4_Init2(void);//��ʼ��2������IO������ʽ��ת��
u8 KEYPAD4x4_Read (void);//�����м���
		 				    
#endif
