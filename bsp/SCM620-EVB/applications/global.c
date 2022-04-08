/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "LE_uart.h"
#include "LE_sst_flash.h"
#include "eth.h"
#include "LE_data_transfer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Crc_Init( void );
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program 
  * @param  None
  * @retval None
  */
void InitGlobalVariables(void)
{
	uint16_t i;
	uint16_t *P_power = ( uint16_t *)&(PVAC_Power[0]);
	

	
////	SSTF016B_RD(ParaFlashADD, Buffer_DataFlash , ParaFlashNum);	
	Crc_Init();		
	////��������///////////////////////////////////
	memset(&LEUER_Data,0,sizeof(LEUER_Data));				
	
	SSTF016B_RD(FLASHADD_INFO, Buffer_DataFlash , FLASHQUAN_INFO*2);	
	for(i=0;i<FLASHQUAN_INFO;i++)
	{	
		P_EMS[i] =((Buffer_DataFlash[2*i]<<8)&0xff00)+(Buffer_DataFlash[2*i+1]&0x00ff);
	}
	//��ȡ��������
	SSTF016B_RD(FLASHADD_POWER, Buffer_DataFlash , FLASHQUAN_POWER*2);	
	for(i=0;i<FLASHQUAN_POWER;i++)
	{	
		P_power[i] =((Buffer_DataFlash[2*i]<<8)&0xff00)+(Buffer_DataFlash[2*i+1]&0x00ff);
	}
	
	//��ȡehernet
	SSTF016B_RD(FLASHADD_ETH, Buffer_DataFlash , FLASHQUAN_ETH*2);	
	for(i=0;i<FLASHQUAN_ETH;i++)
	{	
		P_ethernet[i] =((Buffer_DataFlash[2*i]<<8)&0xff00)+(Buffer_DataFlash[2*i+1]&0x00ff);
	}
	
	#ifdef IP_USE_CODE
	//����������Ƿ���define����̫������
	Ethernet_Par.MAC_ADD0 =  MAC_ADDR0;
	Ethernet_Par.MAC_ADD1 =  MAC_ADDR1; 
	Ethernet_Par.MAC_ADD2 =  MAC_ADDR2; 
	Ethernet_Par.MAC_ADD3 =  MAC_ADDR3; 
	Ethernet_Par.MAC_ADD4 =  MAC_ADDR4; 
	Ethernet_Par.MAC_ADD5 =  MAC_ADDR5;  

	Ethernet_Par.IP_ADD0 =  IP_ADDR0;
	Ethernet_Par.IP_ADD1 =  IP_ADDR1; 
	Ethernet_Par.IP_ADD2 =  IP_ADDR2; 
	Ethernet_Par.IP_ADD3 =  IP_ADDR3;   

	Ethernet_Par.GW_ADD0 =  GW_ADDR0;
	Ethernet_Par.GW_ADD1 =  GW_ADDR1; 
	Ethernet_Par.GW_ADD2 =  GW_ADDR2; 
	Ethernet_Par.GW_ADD3 =  GW_ADDR3; 
	#endif
}

//============================================================
//�������� :Float32
//�������� :��32λ��floatת��Ϊ10��������(0~99999)����С��,IEEE 754��׼
//������� :NumFloat32:��ת��32λ������	
//�� �� ֵ :������ �޷������� u32 ,����-1Ϊ�������߳��ޣ�
//���Լ�¼ :
//============================================================
unsigned int Float32(unsigned int NumFloat32)
{
	
	unsigned char  sbit,ebits;
	unsigned int mbits,result;
 
	sbit = NumFloat32 >> 31;
	ebits =  ( NumFloat32 >> 23 ) & 0xff ;
	mbits =  NumFloat32 & 0x7fffff ; 
 
	if(sbit)    //����Ҫ�������֣�sbitΪ1ʱ��result*-1,ȥ���жϣ�
	{
		return 0 ;
	}
	ebits -= 127;    //�õ�����<0�������С��1������0��
	if(ebits < 0)
	{
		return 0;
	}
	result = (1 << ebits) +(mbits >> (23 - ebits));     
    /*����С��λ������Ҫ��mbits��0~(23 - ebits)λ�����ʵ������������Խ����Զ��崦��
    result = (1 << ebits) +(mbits >> (23 - ebits)); ��Ϊ result = (1 << (ebits + 10))* 1000 / 1024 + (mbits >> (13 - ebits)) * 1000 / 1024 ; 
    ���������1000����result/1000Ϊ�������֣�result%1000ΪС��*/
	if(result > 99999)
	{
//		return -1 ;
	}
//	printf("result = %d\n",result);
	
	return result;	
}


/**
  * @brief  ��ʮ��������ת����IEEE754���������ݣ���������û������
  * @param  None
  * @retval None
 �����ȸ����� float��32 λ������λ S ռ 1 bit��ָ�� E ռ 8 bit��β�� M ռ 23 bit
 β�� M �ĵ�һλ���� 1(��Ϊ 1 <= M < 2)�������� 1 ����ʡ�Բ�д�����Ǹ�����λ������������ 23 λβ�����Ա�ʾ�� 24 λ��Ч����
  */
uint32_t data2hex(uint32_t data)  
{
	uint32_t temp;  
	uint8_t i=0;  
	
	temp = data;//ת����Ҫ�������ֵ  
	while(temp)  
	{         
		temp >>= 1;  
		i++;//���㵱ǰֵ��β��ռ�е�λ��  
	}  
	i--;//����������i����һ�Σ��������    
	temp = data;  //�ٴθ�ֵ��׼���ϲ�        
	temp = temp<<(23-1-i);//����23λβ��  ����23-i���ԣ�Ӧ����22-i 
	
	temp = temp&0x003FFFFF; //��β�������λ��������β����λ����
	temp = temp<<1; //�൱��ȥ����β�������λ
	
	temp = (i+127)<<23 | temp;//����ָ��������β���ϲ����� 
	temp = temp & ~(1<<31);//ȷ������  
	
	return temp;//������Ѿ����Ը������Ĵ洢��ʽ��ʾ�Ĵ������Ĳ�����
}


void Crc_Init( void )
{
	uint16_t	mask, bit, crc, mem;
	for( mask = 0; mask < 0x100; mask++ ) 
	{
		crc = mask;
		for( bit = 0; bit < 8; bit++ ) 
		{
			mem = crc & 0x0001 ;
			crc /= 2;
			if( mem != 0 ) 
				crc ^= POLY_CRC16 ;
		}
		crc_tab.TABLE2[ mask ] = crc & 0xff;
		crc_tab.TABLE1[ mask ] = crc >> 8;
	}
}
