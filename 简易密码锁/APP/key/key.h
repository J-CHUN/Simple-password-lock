#ifndef _key_H
#define _key_H


#include "system.h"
 
#define KEY_LEFT_Pin    GPIO_Pin_0    //����K_LEFT�ܽ�
#define KEY_RIGHT_Pin   GPIO_Pin_1   //����K_RIGHT�ܽ�

#define KEY_Port (GPIOB) //����˿�


//ʹ��λ��������
#define K_LEFT PBin(0)
#define K_RIGHT PBin(1)



//�����������ֵ  
#define KEY_LEFT 1
#define KEY_RIGHT 2  



void KEY_Init(void);
u8 KEY_Scan(u8 mode);
 void EXTIX_Init(void);
#endif
