
/*
//�������ҳ�Ʒ
//����ϵ�п�����Ӧ�ó���
//��ע΢�Ź��ںţ����ҵ���
//���ҿ������������� www.DoYoung.net/YT 
//������ѿ����н�ѧ��Ƶ�����ؼ������ϣ�������������
//�������ݾ��� ����������ҳ www.doyoung.net
*/

/*
���޸���־��
1-201708202312 ������


*/

#include "NVIC.h"

u8 INT_MARK;//�жϱ�־λ

void KEYPAD4x4_INT_INIT (void){	 //�����жϳ�ʼ��
	NVIC_InitTypeDef  NVIC_InitStruct;	//����ṹ�����
	EXTI_InitTypeDef  EXTI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //����GPIOʱ�� ����Ҫ�븴��ʱ��һͬ������     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);//���ö˿��ж���Ҫ���ø���ʱ��

//��1���ж�	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);  //���� GPIO  �ж� Selects the GPIO pin used as EXTI Line.
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line4;  //�����ж���
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //�ж�ʹ��
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //�ж�ģʽΪ �ж�
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //�½��ش���
	
	EXTI_Init(& EXTI_InitStruct);
	/*IRQ:interrupt request �ж�����*/
	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;   //�ж���     
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ʹ���ж�,���ⲿ�жϿ���ֱ�ӽ���NVIC,
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //��ռ���ȼ� 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //�����ȼ�  2
	NVIC_Init(& NVIC_InitStruct);
	
//��2���ж�	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);  //����  GPIO �ж�
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line5;  //�����ж���
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //�ж�ʹ��
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //�ж�ģʽΪ �ж�
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //�½��ش���
	
	EXTI_Init(& EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;   //�ж���
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ʹ���ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //��ռ���ȼ� 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //�����ȼ�  2
	NVIC_Init(& NVIC_InitStruct);

//��3���ж�	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);  //����  GPIO �ж�
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line6;  //�����ж���
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //�ж�ʹ��
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //�ж�ģʽΪ �ж�
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //�½��ش���
	
	EXTI_Init(& EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;   //�ж���
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ʹ���ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //��ռ���ȼ� 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //�����ȼ�  2
	NVIC_Init(& NVIC_InitStruct);

//��4���ж�	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);  //����  GPIO �ж�
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line7;  //�����ж���
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //�ж�ʹ��
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //�ж�ģʽΪ �ж�
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //�½��ش���
	
	EXTI_Init(& EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;   //�ж���
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ʹ���ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //��ռ���ȼ� 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //�����ȼ�  2
	NVIC_Init(& NVIC_InitStruct);

}

void  EXTI4_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET){//�ж�ĳ�����ϵ��ж��Ƿ��� 
		INT_MARK=1;//��־λ��1����ʾ�а����ж�
		EXTI_ClearITPendingBit(EXTI_Line4);   //��� LINE �ϵ��жϱ�־λ
	}     
}
void  EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET){//�ж�ĳ�����ϵ��ж��Ƿ��� 
		INT_MARK=2;//��־λ��1����ʾ�а����ж�
		EXTI_ClearITPendingBit(EXTI_Line5);   //��� LINE �ϵ��жϱ�־λ
	}     
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET){//�ж�ĳ�����ϵ��ж��Ƿ��� 
		INT_MARK=3;//��־λ��1����ʾ�а����ж�
		EXTI_ClearITPendingBit(EXTI_Line6);   //��� LINE �ϵ��жϱ�־λ
	}     
	if(EXTI_GetITStatus(EXTI_Line7)!=RESET){//�ж�ĳ�����ϵ��ж��Ƿ��� 
		INT_MARK=4;//��־λ��1����ʾ�а����ж�
		EXTI_ClearITPendingBit(EXTI_Line7);   //��� LINE �ϵ��жϱ�־λ
	}     
}





