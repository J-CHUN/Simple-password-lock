/*
简易密码锁功能说明：
4位密码
密码全是数字和非0值
A按键展示密码
B按键退格
C按键清屏
D按键进入和退出修改密码
*/

#include "stm32f10x.h" //STM32头文件
#include "system.h"
#include "SysTick.h"
#include "oled.h"
#include "stdio.h"
#include "stm32_flash.h"
#include "KEYPAD4x4.h"
#include "NVIC.h"
#include "usart.h"	
#include "mypwd.h"

//已放在system.h文件中
//#define STM32_FLASH_SAVE_ADDR  0X08009990		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

char dataBuf1[16];                   //数据缓冲区
unsigned char myPwd[4]={1,2,3,4};    //第一次要写入的密码   
unsigned char readPwd[4]={0};       //从flash中读取的密码存放于此
unsigned char inputPwdArr[4]={0};  //用户输入的密码存放于此
unsigned char newPwdArr[4]={0};    //新密码存放于此
u8 g_count = 0;     //密码位数，目前4位密码
u16 inputPwd = 0;   //输入的密码值  
bool modifyFlag = false;  //修改密码的标志
//主程序
int main (void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	SysTick_Init(72);
	OLED_Init();
	OLED_Clear();
	INT_MARK=0;//中断标志位清0

	KEYPAD4x4_Init();//阵列键盘初始化
	KEYPAD4x4_INT_INIT();//阵列键盘的中断初始化
	
	//写入myPwd数组中的密码
	//STM32_FLASH_Write(STM32_FLASH_SAVE_ADDR,(u16*)myPwd,sizeof(myPwd));
	//delay_ms(1000);
	
	//读取密码，并存放在readPwd数组中
	STM32_FLASH_Read(STM32_FLASH_SAVE_ADDR,(u16*)readPwd,sizeof(readPwd));

	OLED_ShowString(0,16,(u8*)"input:",16);
	
	OLED_Refresh();  //更新显存到OLED	
	while(1)
	{	
		if(INT_MARK)  //按键按下，发生中断
		{
			if(modifyFlag==false) //中断标志位为1表示有按键中断
			{ 
				INT_MARK=0;     //标志位清0
				OLED_ShowString(0,16,(u8*)"input:",16);
				inputPwdFun();  //输入密码并显示
				jugePwdFun();    //判断输入的密码
			}

			else if(modifyFlag==true)
			{
				modifyPwdFun();  		//修改密码
			}
		}
		//delay_ms(1000); //延时会影响键盘的输入	
	}
}





