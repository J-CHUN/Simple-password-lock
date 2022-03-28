#ifndef _key_H
#define _key_H


#include "system.h"
 
#define KEY_LEFT_Pin    GPIO_Pin_0    //定义K_LEFT管脚
#define KEY_RIGHT_Pin   GPIO_Pin_1   //定义K_RIGHT管脚

#define KEY_Port (GPIOB) //定义端口


//使用位操作定义
#define K_LEFT PBin(0)
#define K_RIGHT PBin(1)



//定义各个按键值  
#define KEY_LEFT 1
#define KEY_RIGHT 2  



void KEY_Init(void);
u8 KEY_Scan(u8 mode);
 void EXTIX_Init(void);
#endif
