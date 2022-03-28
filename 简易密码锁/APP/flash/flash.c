#include "flash.h"
#include "spi.h"
#include "SysTick.h"   
#include "usart.h"


u16 EN25QXX_TYPE=EN25Q64;	//Ĭ����EN25Q64


//4KbytesΪһ��Sector
//16������Ϊ1��Block
//EN25Q64
//����Ϊ8M�ֽ�,����128��Block,4096��Sector 
													 
//��ʼ��SPI FLASH��IO��
void EN25QXX_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOF, ENABLE);
	
	/* FLASH_CS PG13 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOG,GPIO_Pin_13);
	
	/* SD_CS PG14 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOG,GPIO_Pin_14);
	
	/* ENC28J60_CS PB12 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOB, GPIO_Pin_12); 
	
	/* NRF24L01_CS PF9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PF7
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOF,GPIO_Pin_9);//PF9���1,��ֹNRF����SPI FLASH��ͨ��
	
	EN25QXX_CS=1;			//SPI FLASH��ѡ��
	SPI2_Init();		   			//��ʼ��SPI		
	EN25QXX_TYPE=EN25QXX_ReadID();	//��ȡFLASH ID.

}

//��ȡEN25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
u8 EN25QXX_ReadSR(void)   
{  
	u8 byte=0;   
	EN25QXX_CS=0;                            //ʹ������   
	SPI2_ReadWriteByte(EN25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI2_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     
	return byte;   
} 

//дEN25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void EN25QXX_Write_SR(u8 sr)   
{   
	EN25QXX_CS=0;                            //ʹ������   
	SPI2_ReadWriteByte(EN25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPI2_ReadWriteByte(sr);               //д��һ���ֽ�  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
}  

//EN25QXXдʹ��	
//��WEL��λ   
void EN25QXX_Write_Enable(void)   
{
	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_WriteEnable);      //����дʹ��  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
} 

//EN25QXXд��ֹ	
//��WEL����  
void EN25QXX_Write_Disable(void)   
{  
	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_WriteDisable);     //����д��ָֹ��    
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
} 

//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪEN25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪEN25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪEN25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪEN25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪEN25Q128 	  
u16 EN25QXX_ReadID(void)
{
	u16 Temp = 0;	  
	EN25QXX_CS=0;				    
	SPI2_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	SPI2_ReadWriteByte(0x00); 	    
	SPI2_ReadWriteByte(0x00); 	    
	SPI2_ReadWriteByte(0x00); 	 			   
	Temp|=SPI2_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI2_ReadWriteByte(0xFF);	 
	EN25QXX_CS=1;				    
	return Temp;
}  

//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void EN25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_ReadData);         //���Ͷ�ȡ����   
    SPI2_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
    SPI2_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI2_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI2_ReadWriteByte(0XFF);   //ѭ������  
    }
	EN25QXX_CS=1;  				    	      
}  

//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void EN25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    EN25QXX_Write_Enable();                  //SET WEL 
	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_PageProgram);      //����дҳ����   
    SPI2_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
    SPI2_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI2_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI2_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ 
	EN25QXX_Wait_Busy();					   //�ȴ�д�����
}

//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void EN25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		EN25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	}	    
} 

//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
u8 EN25QXX_BUFFER[4096];		 
void EN25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * EN25QXX_BUF;	  
   	EN25QXX_BUF=EN25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		EN25QXX_Read(EN25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(EN25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			EN25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				EN25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			EN25QXX_Write_NoCheck(EN25QXX_BUF,secpos*4096,4096);//д����������  

		}else EN25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	}	 
}

//��������оƬ		  
//�ȴ�ʱ�䳬��...
void EN25QXX_Erase_Chip(void)   
{                                   
    EN25QXX_Write_Enable();                  //SET WEL 
    EN25QXX_Wait_Busy();   
  	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_ChipErase);        //����Ƭ��������  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
	EN25QXX_Wait_Busy();   				   //�ȴ�оƬ��������
}   

//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void EN25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//����falsh�������,������   
 	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    EN25QXX_Write_Enable();                  //SET WEL 	 
    EN25QXX_Wait_Busy();   
  	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_SectorErase);      //������������ָ�� 
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI2_ReadWriteByte((u8)Dst_Addr);  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    EN25QXX_Wait_Busy();   				   //�ȴ��������
}

//�ȴ�����
void EN25QXX_Wait_Busy(void)   
{   
	while((EN25QXX_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}

//�������ģʽ
void EN25QXX_PowerDown(void)   
{ 
  	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_PowerDown);        //���͵�������  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TPD  
}   

//����
void EN25QXX_WAKEUP(void)   
{  
  	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_ReleasePowerDown);   //  send EN25X_PowerDown command 0xAB    
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TRES1
}   



