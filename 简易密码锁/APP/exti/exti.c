#include "exti.h"
#include "led.h"
#include "SysTick.h"
#include "key.h"

/*******************************************************************************
* �� �� ��         : My_EXTI_Init
* ��������		   : �ⲿ�жϳ�ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void My_EXTI_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//ѡ��GPIO�ܽ������ⲿ�ж���·
	
	
	//EXTI0 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	//EXTI2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//EXTI2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	//EXTI3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//EXTI3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//EXTI4 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//EXTI4�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line2|EXTI_Line3|EXTI_Line4; 
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}

/*******************************************************************************
* �� �� ��         : EXTI0_IRQHandler
* ��������		   : �ⲿ�ж�0����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==1)
	{
		delay_ms(10);
		if(K_UP==1)
		{
			led2=0;
		}	
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

/*******************************************************************************
* �� �� ��         : EXTI3_IRQHandler
* ��������		   : �ⲿ�ж�3����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)==1)
	{
		delay_ms(10);
		if(K_DOWN==0)
		{	
			led2=1;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

/*******************************************************************************
* �� �� ��         : EXTI2_IRQHandler
* ��������		   : �ⲿ�ж�2����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)==1)
	{
		delay_ms(10);
		if(K_LEFT==0)
		{
			led3=1;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/*******************************************************************************
* �� �� ��         : EXTI4_IRQHandler
* ��������		   : �ⲿ�ж�4����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)==1)
	{
		delay_ms(10);
		if(K_RIGHT==0)
		{
			led3=0;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}


