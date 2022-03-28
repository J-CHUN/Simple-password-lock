
/*

*/

#include "mypwd.h"
#include "KEYPAD4x4.h"
#include "stdbool.h"
#include <stdio.h>
#include "key.h"
#include "stm32_flash.h"

extern char dataBuf1[16];
extern unsigned char myPwd[4];
extern unsigned char readPwd[4];
extern unsigned char inputPwdArr[4];
extern unsigned char newPwdArr[4];
extern u8 g_count ;  //����λ��
extern bool modifyFlag ;
extern u16 inputPwd;
extern u8 INT_MARK;//�жϱ�־λ

void inputPwdFun()
{
	u8 s=KEYPAD4x4_Read();//��������ֵ				
	if(s!=0)//�簴��ֵ����0��Ҳ����˵�а������������ж�Ϊ��
	{ 
		//��������
		inputPwd = s+inputPwd*10;  //��ȡ4λ����
		inputPwdArr[g_count] = s;  //������������
		if(g_count<=3)  //ֻ��ʾ4λ����
		{
			g_count++;
			sprintf(dataBuf1,"input:%d",inputPwd);  
			OLED_ShowString(0,16,(u8*)dataBuf1,16);
			OLED_Refresh();
		}
		else
		{
			inputPwd = 0;
		}
	}
}

//�ж�����
int jugePwdFun(void)
{
	//�����ж�
	if(g_count==4)//�Ѿ�������4λ����
	{
		if(readPwd[0]==inputPwdArr[0]&&readPwd[1]==inputPwdArr[1]&&readPwd[2]==inputPwdArr[2]&&readPwd[3]==inputPwdArr[3])
		{
			OLED_ShowString(0,48,(u8*)"pwd ok ",16);
			OLED_Refresh();
			return 1;
		}
		else 
		{
			OLED_ShowString(0,48,(u8*)"pwd err",16);
			sprintf(dataBuf1,"input:"); 
			modifyFlag = false;			
			OLED_Refresh();
			return 0;
		}
	}
	return 0;
}

//�޸�����
void modifyPwdFun(void)
{
	//�޸�����
	u8 s;
	INT_MARK=0;//��־λ��0
	s = KEYPAD4x4_Read();//��������ֵ
	if(s!=0)
	{
		inputPwd = s+inputPwd*10;  //��ȡ4λ����
		newPwdArr[g_count] = s;
		if(g_count<=3)  //ֻ��ʾ4λ����
		{
			sprintf(dataBuf1,"new pwd:%d",inputPwd); 
			OLED_ShowString(0,16,(u8*)dataBuf1,16);
			g_count++;
			if(g_count==4) //���������һλ����ʾ�޸ĳɹ�
			{
				OLED_ShowString(0,32,(u8*)"modify ok",16);
				STM32_FLASH_Write(STM32_FLASH_SAVE_ADDR,(u16*)newPwdArr,sizeof(newPwdArr));
				inputPwd = 0;
			}
			OLED_Refresh();
		}
	}
	//��ȡ������
	STM32_FLASH_Read(STM32_FLASH_SAVE_ADDR,(u16*)readPwd,sizeof(readPwd));
}
//չʾĬ�Ͻ���
void show_default()
{
	OLED_Clear();	
	OLED_ShowString(0,16,(u8*)"input:",16);	
	OLED_Refresh();		//������ʾ��OLED 
}
