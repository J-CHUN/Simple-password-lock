#ifndef _stm32_flash_H
#define _stm32_flash_H

#include "system.h"


//用户根据自己的需要设置
#define STM32_FLASH_SIZE 64 	 //所选STM32的FLASH容量大小(单位为K)

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
 
vu16 STM32_FLASH_ReadHalfWord(u32 faddr); 
void STM32_FLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void STM32_FLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据
					   


#endif
