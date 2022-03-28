
/*
//杜洋工作室出品
//洋桃系列开发板应用程序
//关注微信公众号：洋桃电子
//洋桃开发板资料下载 www.DoYoung.net/YT 
//即可免费看所有教学视频，下载技术资料，技术疑难提问
//更多内容尽在 杜洋工作室主页 www.doyoung.net
*/

/*
《修改日志》
1-201708202312 创建。


*/

#include "NVIC.h"

u8 INT_MARK;//中断标志位

void KEYPAD4x4_INT_INIT (void){	 //按键中断初始化
	NVIC_InitTypeDef  NVIC_InitStruct;	//定义结构体变量
	EXTI_InitTypeDef  EXTI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //启动GPIO时钟 （需要与复用时钟一同启动）     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);//配置端口中断需要启用复用时钟

//第1个中断	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);  //定义 GPIO  中断 Selects the GPIO pin used as EXTI Line.
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line4;  //定义中断线
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //中断使能
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //中断模式为 中断
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //下降沿触发
	
	EXTI_Init(& EXTI_InitStruct);
	/*IRQ:interrupt request 中断请求*/
	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;   //中断线     
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //使能中断,让外部中断可以直接进入NVIC,
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //子优先级  2
	NVIC_Init(& NVIC_InitStruct);
	
//第2个中断	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);  //定义  GPIO 中断
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line5;  //定义中断线
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //中断使能
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //中断模式为 中断
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //下降沿触发
	
	EXTI_Init(& EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;   //中断线
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //使能中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //子优先级  2
	NVIC_Init(& NVIC_InitStruct);

//第3个中断	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);  //定义  GPIO 中断
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line6;  //定义中断线
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //中断使能
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //中断模式为 中断
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //下降沿触发
	
	EXTI_Init(& EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;   //中断线
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //使能中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //子优先级  2
	NVIC_Init(& NVIC_InitStruct);

//第4个中断	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);  //定义  GPIO 中断
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line7;  //定义中断线
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;              //中断使能
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;     //中断模式为 中断
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;   //下降沿触发
	
	EXTI_Init(& EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;   //中断线
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //使能中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级 2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;     //子优先级  2
	NVIC_Init(& NVIC_InitStruct);

}

void  EXTI4_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET){//判断某个线上的中断是否发生 
		INT_MARK=1;//标志位置1，表示有按键中断
		EXTI_ClearITPendingBit(EXTI_Line4);   //清除 LINE 上的中断标志位
	}     
}
void  EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET){//判断某个线上的中断是否发生 
		INT_MARK=2;//标志位置1，表示有按键中断
		EXTI_ClearITPendingBit(EXTI_Line5);   //清除 LINE 上的中断标志位
	}     
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET){//判断某个线上的中断是否发生 
		INT_MARK=3;//标志位置1，表示有按键中断
		EXTI_ClearITPendingBit(EXTI_Line6);   //清除 LINE 上的中断标志位
	}     
	if(EXTI_GetITStatus(EXTI_Line7)!=RESET){//判断某个线上的中断是否发生 
		INT_MARK=4;//标志位置1，表示有按键中断
		EXTI_ClearITPendingBit(EXTI_Line7);   //清除 LINE 上的中断标志位
	}     
}





