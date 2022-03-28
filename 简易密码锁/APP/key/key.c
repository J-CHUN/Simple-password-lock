#include "key.h"
#include "SysTick.h"
#include "led.h"
#include "oled.h"
#include "stdio.h"
/*******************************************************************************
* 函 数 名         : KEY_Init
* 函数功能		   : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin=KEY_LEFT_Pin|KEY_RIGHT_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(KEY_Port,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : KEY_Scan
* 函数功能		   : 按键扫描检测
* 输    入         : mode=0:单次按下按键
					 mode=1：连续按下按键
* 输    出         : 0：未有按键按下
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	if(key==1&&(K_LEFT==0||K_RIGHT==0)) //任意一个按键按下
	{
		delay_ms(10);  //消抖
		key=0;
		
		 if(K_LEFT==0)
		{
			return KEY_LEFT; 
		}
		else
		{
			return KEY_RIGHT;
		}
	}
	else if(K_LEFT==1&&K_RIGHT==1)    //无按键按下
	{
		key=1;
	}
	if(mode==1) //连续按键按下
	{
		key=1;
	}
	return 0;
}

//外部中断初始化函数
void EXTIX_Init(void)
{ 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟

	KEY_Init();//初始化按键对应io模式

    //GPIOB.0 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOB.1	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器  
 
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
}


extern bool modifyFlag;
extern u16	inputPwd;
extern u8 INT_MARK;
extern u8 g_count;  //密码位数

//进入或退出修改密码 PB0
static int i = 0;
void EXTI0_IRQHandler(void)  //按下LED按键后的中断处理函数
{
	
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除LINE0线路挂起位
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
}

//清屏 PB1
void EXTI1_IRQHandler(void)   //按下BEEP按键后中断处理函数
{	
	EXTI_ClearITPendingBit(EXTI_Line1);    //清除LINE1上的中断标志位 
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
}

