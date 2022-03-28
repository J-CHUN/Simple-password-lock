#include "KEYPAD4x4.h"
#include "oled.h"
#include "stdio.h"
#include "mypwd.h"

void KEYPAD4x4_Init(void){ //΢�����صĽӿڳ�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure; //����GPIO�ĳ�ʼ��ö�ٽṹ	
    GPIO_InitStructure.GPIO_Pin = KEYa | KEYb | KEYc | KEYd; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //ѡ��IO�ӿڹ�����ʽ //��������       
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KEY1 | KEY2 | KEY3 | KEY4; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��     
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

	GPIO_ResetBits(KEYPAD4x4PORT,KEY1|KEY2|KEY3|KEY4);
	GPIO_SetBits(KEYPAD4x4PORT,KEYa|KEYb|KEYc|KEYd);
}
void KEYPAD4x4_Init2(void){ //΢�����صĽӿڳ�ʼ��2������IO������ʽ��ת��
	GPIO_InitTypeDef  GPIO_InitStructure; //����GPIO�ĳ�ʼ��ö�ٽṹ	
    GPIO_InitStructure.GPIO_Pin = KEY1 | KEY2 | KEY3 | KEY4; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //ѡ��IO�ӿڹ�����ʽ //��������       
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KEYa | KEYb | KEYc | KEYd; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ //��������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��     
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

	GPIO_SetBits(KEYPAD4x4PORT,KEY1|KEY2|KEY3|KEY4);
	GPIO_ResetBits(KEYPAD4x4PORT,KEYa|KEYb|KEYc|KEYd);
}

extern char dataBuf1[16];                   //���ݻ�����
extern unsigned char myPwd[4];    //��һ��Ҫд�������   
extern unsigned char readPwd[4];       //��flash�ж�ȡ���������ڴ�
extern unsigned char inputPwdArr;  //�û�������������ڴ�
extern unsigned char newPwdArr[4];    //���������ڴ�
extern u8 g_count ;     //����λ����Ŀǰ4λ����
extern u16 inputPwd ;   //���������ֵ  
extern bool modifyFlag ;  //�޸�����ı�־
extern u8 INT_MARK;
u8 KEYPAD4x4_Read (void)
{//���̴���
	u8 a=0,b=0;//�������
	static int i = 0;
	KEYPAD4x4_Init();//��ʼ��IO
	if(!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYa) ||  	//��Ѱ���̿ڵ�ֵ�Ƿ�仯
		!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYb) || 
		!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYc) || 
		!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYd)){
		delay_ms (20);//��ʱ20����
		if(!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYa) ||  	//��Ѱ���̿ڵ�ֵ�Ƿ�仯
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYb) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYc) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYd)){
	  		a = GPIO_ReadInputData(KEYPAD4x4PORT)&0xff;//��ֵ����Ĵ���a
		}
		KEYPAD4x4_Init2();//IO������ʽ��ת
		b = GPIO_ReadInputData(KEYPAD4x4PORT)&0xff;//���ڶ���ȡ��ֵ����Ĵ���b
		a = a|b;//�������������
		switch(a){//�Ա�����ֵ
			case 0xee: b = 'D'; break;//�Աȵõ��ļ�ֵ��bһ��Ӧ������
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
			default: b = 0; break;//��ֵ������
			
		}
		while(!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY1) ||  	//�ȴ������ſ�
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY2) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY3) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY4));
		delay_ms (20);//��ʱ20����
	}

	 //�����ְ�������
	//����D:�޸�����
		if(b=='D')  
		{	
			OLED_Clear();              //����
			modifyFlag = !modifyFlag;  //�ı��޸ı�־λ
			INT_MARK = 1;              //�жϱ�־��1�����ð��°���Ҳ�ܽ��������������
			g_count = 0;               //����λ����0�����¿�ʼ����������
			inputPwd = 0;              //������ֵ��0�����¿�ʼ����������
			if(i==0)   //�л���ͬ����ʾ 
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
		//����C:����
		else if(b=='C')
		{ 
			inputPwd = 0;
			g_count = 0;
			OLED_Clear();
			if(i==0)   //�л���ͬ����ʾ 
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
			//��ʾ��ȡ����������
			OLED_Clear();
			sprintf(dataBuf1,"show pwd:%d%d%d%d",readPwd[0],readPwd[1],readPwd[2],readPwd[3]);
			OLED_ShowString(0,32,(u8*)dataBuf1,16); 
			OLED_Refresh();
			return 0;
		}
	return (b);//��b��Ϊ����ֵ
} 



/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/



/*
ѡ��IO�ӿڹ�����ʽ��
GPIO_Mode_AIN ģ������
GPIO_Mode_IN_FLOATING ��������
GPIO_Mode_IPD ��������
GPIO_Mode_IPU ��������
GPIO_Mode_Out_PP �������
GPIO_Mode_Out_OD ��©���
GPIO_Mode_AF_PP �����������
GPIO_Mode_AF_OD ���ÿ�©���
*/
