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
	uint16_t *p,i;
	uint16_t *P_power = ( uint16_t *)&(PVAC_Power[0]);
	

	
////	SSTF016B_RD(ParaFlashADD, Buffer_DataFlash , ParaFlashNum);	
	Crc_Init();		
	////数据清零///////////////////////////////////
	memset(&LEUER_Data,0,sizeof(LEUER_Data));				
	
	SSTF016B_RD(FLASHADD_INFO, Buffer_DataFlash , FLASHQUAN_INFO*2);	
	for(i=0;i<FLASHQUAN_INFO;i++)
	{	
		P_EMS[i] =((Buffer_DataFlash[2*i]<<8)&0xff00)+(Buffer_DataFlash[2*i+1]&0x00ff);
	}
	//读取三个电能
	SSTF016B_RD(FLASHADD_POWER, Buffer_DataFlash , FLASHQUAN_POWER*2);	
	for(i=0;i<FLASHQUAN_POWER;i++)
	{	
		P_power[i] =((Buffer_DataFlash[2*i]<<8)&0xff00)+(Buffer_DataFlash[2*i+1]&0x00ff);
	}
	
	//读取ehernet
	SSTF016B_RD(FLASHADD_ETH, Buffer_DataFlash , FLASHQUAN_ETH*2);	
	for(i=0;i<FLASHQUAN_ETH;i++)
	{	
		P_ethernet[i] =((Buffer_DataFlash[2*i]<<8)&0xff00)+(Buffer_DataFlash[2*i+1]&0x00ff);
	}
	
	#ifdef IP_USE_CODE
	//在这里决定是否用define的以太网参数
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
