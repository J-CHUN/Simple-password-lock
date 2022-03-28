
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
extern u8 g_count ;  //密码位数
extern bool modifyFlag ;
extern u16 inputPwd;
extern u8 INT_MARK;//中断标志位

void inputPwdFun()
{
	u8 s=KEYPAD4x4_Read();//读出按键值				
	if(s!=0)//如按键值不是0，也就是说有按键操作，则判断为真
	{ 
		//输入密码
		inputPwd = s+inputPwd*10;  //读取4位密码
		inputPwdArr[g_count] = s;  //存放输入的密码
		if(g_count<=3)  //只显示4位密码
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

//判断密码
int jugePwdFun(void)
{
	//密码判断
	if(g_count==4)//已经输入了4位密码
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

//修改密码
void modifyPwdFun(void)
{
	//修改密码
	u8 s;
	INT_MARK=0;//标志位清0
	s = KEYPAD4x4_Read();//读出按键值
	if(s!=0)
	{
		inputPwd = s+inputPwd*10;  //读取4位密码
		newPwdArr[g_count] = s;
		if(g_count<=3)  //只显示4位密码
		{
			sprintf(dataBuf1,"new pwd:%d",inputPwd); 
			OLED_ShowString(0,16,(u8*)dataBuf1,16);
			g_count++;
			if(g_count==4) //输入完最后一位后显示修改成功
			{
				OLED_ShowString(0,32,(u8*)"modify ok",16);
				STM32_FLASH_Write(STM32_FLASH_SAVE_ADDR,(u16*)newPwdArr,sizeof(newPwdArr));
				inputPwd = 0;
			}
			OLED_Refresh();
		}
	}
	//读取新密码
	STM32_FLASH_Read(STM32_FLASH_SAVE_ADDR,(u16*)readPwd,sizeof(readPwd));
}
//展示默认界面
void show_default()
{
	OLED_Clear();	
	OLED_ShowString(0,16,(u8*)"input:",16);	
	OLED_Refresh();		//更新显示到OLED 
}
