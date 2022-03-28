#include "KEYPAD4x4.h"
#include "oled.h"
#include "stdio.h"
#include "mypwd.h"

void KEYPAD4x4_Init(void){ //微动开关的接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构	
    GPIO_InitStructure.GPIO_Pin = KEYa | KEYb | KEYc | KEYd; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KEY1 | KEY2 | KEY3 | KEY4; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）     
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

	GPIO_ResetBits(KEYPAD4x4PORT,KEY1|KEY2|KEY3|KEY4);
	GPIO_SetBits(KEYPAD4x4PORT,KEYa|KEYb|KEYc|KEYd);
}
void KEYPAD4x4_Init2(void){ //微动开关的接口初始化2（用于IO工作方式反转）
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构	
    GPIO_InitStructure.GPIO_Pin = KEY1 | KEY2 | KEY3 | KEY4; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KEYa | KEYb | KEYc | KEYd; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式 //上拉电阻  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）     
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

	GPIO_SetBits(KEYPAD4x4PORT,KEY1|KEY2|KEY3|KEY4);
	GPIO_ResetBits(KEYPAD4x4PORT,KEYa|KEYb|KEYc|KEYd);
}

extern char dataBuf1[16];                   //数据缓冲区
extern unsigned char myPwd[4];    //第一次要写入的密码   
extern unsigned char readPwd[4];       //从flash中读取的密码存放于此
extern unsigned char inputPwdArr;  //用户输入的密码存放于此
extern unsigned char newPwdArr[4];    //新密码存放于此
extern u8 g_count ;     //密码位数，目前4位密码
extern u16 inputPwd ;   //输入的密码值  
extern bool modifyFlag ;  //修改密码的标志
extern u8 INT_MARK;
u8 KEYPAD4x4_Read (void)
{//键盘处理函
	u8 a=0,b=0;//定义变量
	static int i = 0;
	KEYPAD4x4_Init();//初始化IO
	if(!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYa) ||  	//查寻键盘口的值是否变化
		!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYb) || 
		!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYc) || 
		!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYd)){
		delay_ms (20);//延时20毫秒
		if(!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYa) ||  	//查寻键盘口的值是否变化
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYb) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYc) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYd)){
	  		a = GPIO_ReadInputData(KEYPAD4x4PORT)&0xff;//键值放入寄存器a
		}
		KEYPAD4x4_Init2();//IO工作方式反转
		b = GPIO_ReadInputData(KEYPAD4x4PORT)&0xff;//将第二次取得值放入寄存器b
		a = a|b;//将两个数据相或
		switch(a){//对比数据值
			case 0xee: b = 'D'; break;//对比得到的键值给b一个应用数据
			//case 0xed: b = '#'; break;
			//case 0xeb: b = 0; break;
			//case 0xe7: b = '*'; break;
			case 0xde: b = 'C'; break;
			case 0xdd: b = 9; break;
			case 0xdb: b = 8; break;
			case 0xd7: b = 7; break;
			case 0xbe: b = 'B'; break;
			case 0xbd: b = 6; break;
			case 0xbb: b = 5; break;
			case 0xb7: b = 4; break;
			case 0x7e: b = 'A'; break;
			case 0x7d: b = 3; break;
			case 0x7b: b = 2; break;
			case 0x77: b = 1; break;			
			default: b = 0; break;//键值错误处理
			
		}
		while(!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY1) ||  	//等待按键放开
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY2) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY3) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY4));
		delay_ms (20);//延时20毫秒
	}

	 //非数字按键处理
	//按键D:修改密码
		if(b=='D')  
		{	
			OLED_Clear();              //清屏
			modifyFlag = !modifyFlag;  //改变修改标志位
			INT_MARK = 1;              //中断标志置1，不用按下按键也能进入输入密码界面
			g_count = 0;               //密码位数清0，重新开始输入新密码
			inputPwd = 0;              //密码数值清0，重新开始输入新密码
			if(i==0)   //切换不同的显示 
			{
				i = 1;
				OLED_ShowString(0,16,(u8*)"new pwd:",16);
			}
			else 
			{
				i = 0;
				OLED_ShowString(0,16,(u8*)"input:",16);
			}
			OLED_Refresh();
		
			return 0;
		}
		//按键C:清屏
		else if(b=='C')
		{ 
			inputPwd = 0;
			g_count = 0;
			OLED_Clear();
			if(i==0)   //切换不同的显示 
			{
				OLED_ShowString(0,16,(u8*)"input:",16);
			}
			else 
			{
				OLED_ShowString(0,16,(u8*)"new pwd:",16);
			}
			OLED_Refresh();
			return 0;
		}
		else if(b=='B')
		{
			inputPwd = inputPwd/10;
			g_count = g_count>0?(g_count-1):0;
			OLED_Clear();
			if(inputPwd==0) 
			{
				sprintf(dataBuf1,"input:");  
			}
			else
			{
				sprintf(dataBuf1,"input:%d",inputPwd);  
			}
			OLED_ShowString(0,16,(u8*)dataBuf1,16);
			OLED_Refresh();
			return 0;
		}
		else if(b=='A')
		{
			//显示读取出来的密码
			OLED_Clear();
			sprintf(dataBuf1,"show pwd:%d%d%d%d",readPwd[0],readPwd[1],readPwd[2],readPwd[3]);
			OLED_ShowString(0,32,(u8*)dataBuf1,16); 
			OLED_Refresh();
			return 0;
		}
	return (b);//将b作为返回值
} 



/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/



/*
选择IO接口工作方式：
GPIO_Mode_AIN 模拟输入
GPIO_Mode_IN_FLOATING 浮空输入
GPIO_Mode_IPD 下拉输入
GPIO_Mode_IPU 上拉输入
GPIO_Mode_Out_PP 推挽输出
GPIO_Mode_Out_OD 开漏输出
GPIO_Mode_AF_PP 复用推挽输出
GPIO_Mode_AF_OD 复用开漏输出
*/
