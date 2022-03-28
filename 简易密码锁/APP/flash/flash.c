#include "flash.h"
#include "spi.h"
#include "SysTick.h"   
#include "usart.h"


u16 EN25QXX_TYPE=EN25Q64;	//默认是EN25Q64


//4Kbytes为一个Sector
//16个扇区为1个Block
//EN25Q64
//容量为8M字节,共有128个Block,4096个Sector 
													 
//初始化SPI FLASH的IO口
void EN25QXX_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOF, ENABLE);
	
	/* FLASH_CS PG13 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOG,GPIO_Pin_13);
	
	/* SD_CS PG14 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOG,GPIO_Pin_14);
	
	/* ENC28J60_CS PB12 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOB, GPIO_Pin_12); 
	
	/* NRF24L01_CS PF9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PF7
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOF,GPIO_Pin_9);//PF9输出1,防止NRF干扰SPI FLASH的通信
	
	EN25QXX_CS=1;			//SPI FLASH不选中
	SPI2_Init();		   			//初始化SPI		
	EN25QXX_TYPE=EN25QXX_ReadID();	//读取FLASH ID.

}

//读取EN25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
u8 EN25QXX_ReadSR(void)   
{  
	u8 byte=0;   
	EN25QXX_CS=0;                            //使能器件   
	SPI2_ReadWriteByte(EN25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=SPI2_ReadWriteByte(0Xff);             //读取一个字节  
	EN25QXX_CS=1;                            //取消片选     
	return byte;   
} 

//写EN25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void EN25QXX_Write_SR(u8 sr)   
{   
	EN25QXX_CS=0;                            //使能器件   
	SPI2_ReadWriteByte(EN25X_WriteStatusReg);   //发送写取状态寄存器命令    
	SPI2_ReadWriteByte(sr);               //写入一个字节  
	EN25QXX_CS=1;                            //取消片选     	      
}  

//EN25QXX写使能	
//将WEL置位   
void EN25QXX_Write_Enable(void)   
{
	EN25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(EN25X_WriteEnable);      //发送写使能  
	EN25QXX_CS=1;                            //取消片选     	      
} 

//EN25QXX写禁止	
//将WEL清零  
void EN25QXX_Write_Disable(void)   
{  
	EN25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(EN25X_WriteDisable);     //发送写禁止指令    
	EN25QXX_CS=1;                            //取消片选     	      
} 

//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为EN25Q80  
//0XEF14,表示芯片型号为EN25Q16    
//0XEF15,表示芯片型号为EN25Q32  
//0XEF16,表示芯片型号为EN25Q64 
//0XEF17,表示芯片型号为EN25Q128 	  
u16 EN25QXX_ReadID(void)
{
	u16 Temp = 0;	  
	EN25QXX_CS=0;				    
	SPI2_ReadWriteByte(0x90);//发送读取ID命令	    
	SPI2_ReadWriteByte(0x00); 	    
	SPI2_ReadWriteByte(0x00); 	    
	SPI2_ReadWriteByte(0x00); 	 			   
	Temp|=SPI2_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI2_ReadWriteByte(0xFF);	 
	EN25QXX_CS=1;				    
	return Temp;
}  

//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void EN25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	EN25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(EN25X_ReadData);         //发送读取命令   
    SPI2_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址    
    SPI2_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI2_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI2_ReadWriteByte(0XFF);   //循环读数  
    }
	EN25QXX_CS=1;  				    	      
}  

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void EN25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    EN25QXX_Write_Enable();                  //SET WEL 
	EN25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(EN25X_PageProgram);      //发送写页命令   
    SPI2_ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址    
    SPI2_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI2_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI2_ReadWriteByte(pBuffer[i]);//循环写数  
	EN25QXX_CS=1;                            //取消片选 
	EN25QXX_Wait_Busy();					   //等待写入结束
}

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void EN25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		EN25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	}	    
} 

//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
u8 EN25QXX_BUFFER[4096];		 
void EN25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * EN25QXX_BUF;	  
   	EN25QXX_BUF=EN25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		EN25QXX_Read(EN25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(EN25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			EN25QXX_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				EN25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			EN25QXX_Write_NoCheck(EN25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}else EN25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	}	 
}

//擦除整个芯片		  
//等待时间超长...
void EN25QXX_Erase_Chip(void)   
{                                   
    EN25QXX_Write_Enable();                  //SET WEL 
    EN25QXX_Wait_Busy();   
  	EN25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(EN25X_ChipErase);        //发送片擦除命令  
	EN25QXX_CS=1;                            //取消片选     	      
	EN25QXX_Wait_Busy();   				   //等待芯片擦除结束
}   

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void EN25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//监视falsh擦除情况,测试用   
 	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    EN25QXX_Write_Enable();                  //SET WEL 	 
    EN25QXX_Wait_Busy();   
  	EN25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(EN25X_SectorErase);      //发送扇区擦除指令 
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>16));  //发送24bit地址    
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI2_ReadWriteByte((u8)Dst_Addr);  
	EN25QXX_CS=1;                            //取消片选     	      
    EN25QXX_Wait_Busy();   				   //等待擦除完成
}

//等待空闲
void EN25QXX_Wait_Busy(void)   
{   
	while((EN25QXX_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}

//进入掉电模式
void EN25QXX_PowerDown(void)   
{ 
  	EN25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(EN25X_PowerDown);        //发送掉电命令  
	EN25QXX_CS=1;                            //取消片选     	      
    delay_us(3);                               //等待TPD  
}   

//唤醒
void EN25QXX_WAKEUP(void)   
{  
  	EN25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(EN25X_ReleasePowerDown);   //  send EN25X_PowerDown command 0xAB    
	EN25QXX_CS=1;                            //取消片选     	      
    delay_us(3);                               //等待TRES1
}   



