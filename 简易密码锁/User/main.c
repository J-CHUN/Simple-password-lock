/*
��������������˵����
4λ����
����ȫ�����ֺͷ�0ֵ
A����չʾ����
B�����˸�
C��������
D����������˳��޸�����
*/

#include "stm32f10x.h" //STM32ͷ�ļ�
#include "system.h"
#include "SysTick.h"
#include "oled.h"
#include "stdio.h"
#include "stm32_flash.h"
#include "KEYPAD4x4.h"
#include "NVIC.h"
#include "usart.h"	
#include "mypwd.h"

//�ѷ���system.h�ļ���
//#define STM32_FLASH_SAVE_ADDR  0X08009990		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)

char dataBuf1[16];                   //���ݻ�����
unsigned char myPwd[4]={1,2,3,4};    //��һ��Ҫд�������   
unsigned char readPwd[4]={0};       //��flash�ж�ȡ���������ڴ�
unsigned char inputPwdArr[4]={0};  //�û�������������ڴ�
unsigned char newPwdArr[4]={0};    //���������ڴ�
u8 g_count = 0;     //����λ����Ŀǰ4λ����
u16 inputPwd = 0;   //���������ֵ  
bool modifyFlag = false;  //�޸�����ı�־
//������
int main (void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	SysTick_Init(72);
	OLED_Init();
	OLED_Clear();
	INT_MARK=0;//�жϱ�־λ��0

	KEYPAD4x4_Init();//���м��̳�ʼ��
	KEYPAD4x4_INT_INIT();//���м��̵��жϳ�ʼ��
	
	//д��myPwd�����е�����
	//STM32_FLASH_Write(STM32_FLASH_SAVE_ADDR,(u16*)myPwd,sizeof(myPwd));
	//delay_ms(1000);
	
	//��ȡ���룬�������readPwd������
	STM32_FLASH_Read(STM32_FLASH_SAVE_ADDR,(u16*)readPwd,sizeof(readPwd));

	OLED_ShowString(0,16,(u8*)"input:",16);
	
	OLED_Refresh();  //�����Դ浽OLED	
	while(1)
	{	
		if(INT_MARK)  //�������£������ж�
		{
			if(modifyFlag==false) //�жϱ�־λΪ1��ʾ�а����ж�
			{ 
				INT_MARK=0;     //��־λ��0
				OLED_ShowString(0,16,(u8*)"input:",16);
				inputPwdFun();  //�������벢��ʾ
				jugePwdFun();    //�ж����������
			}

			else if(modifyFlag==true)
			{
				modifyPwdFun();  		//�޸�����
			}
		}
		//delay_ms(1000); //��ʱ��Ӱ����̵�����	
	}
}





