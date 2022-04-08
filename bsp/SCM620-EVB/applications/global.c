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

//============================================================
//函数名称 :Float32
//函数功能 :将32位的float转换为10进制正数(0~99999)舍弃小数,IEEE 754标准
//输入变量 :NumFloat32:待转换32位浮点数	
//返 回 值 :计算结果 无符号整形 u32 ,返回-1为负数或者超限；
//调试记录 :
//============================================================
unsigned int Float32(unsigned int NumFloat32)
{
	
	unsigned char  sbit,ebits;
	unsigned int mbits,result;
 
	sbit = NumFloat32 >> 31;
	ebits =  ( NumFloat32 >> 23 ) & 0xff ;
	mbits =  NumFloat32 & 0x7fffff ; 
 
	if(sbit)    //若需要负数部分，sbit为1时，result*-1,去掉判断；
	{
		return 0 ;
	}
	ebits -= 127;    //得到阶码<0，即结果小于1，返回0；
	if(ebits < 0)
	{
		return 0;
	}
	result = (1 << ebits) +(mbits >> (23 - ebits));     
    /*若有小数位数精度要求，mbits的0~(23 - ebits)位可以适当保留处理；可以进行自定义处理
    result = (1 << ebits) +(mbits >> (23 - ebits)); 改为 result = (1 << (ebits + 10))* 1000 / 1024 + (mbits >> (13 - ebits)) * 1000 / 1024 ; 
    结果扩大了1000倍，result/1000为整数部分，result%1000为小数*/
	if(result > 99999)
	{
//		return -1 ;
	}
//	printf("result = %d\n",result);
	
	return result;	
}


/**
  * @brief  将十进制数据转换成IEEE754浮点型数据，初步测试没有问题
  * @param  None
  * @retval None
 单精度浮点数 float：32 位，符号位 S 占 1 bit，指数 E 占 8 bit，尾数 M 占 23 bit
 尾数 M 的第一位总是 1(因为 1 <= M < 2)，因此这个 1 可以省略不写，它是个隐藏位，这样单精度 23 位尾数可以表示了 24 位有效数字
  */
uint32_t data2hex(uint32_t data)  
{
	uint32_t temp;  
	uint8_t i=0;  
	
	temp = data;//转存需要计算的数值  
	while(temp)  
	{         
		temp >>= 1;  
		i++;//计算当前值的尾数占有的位数  
	}  
	i--;//计算下来，i会多加一次，这儿减掉    
	temp = data;  //再次赋值，准备合并        
	temp = temp<<(23-1-i);//补足23位尾数  这里23-i不对，应该是22-i 
	
	temp = temp&0x003FFFFF; //将尾数的最高位和其它非尾数的位清零
	temp = temp<<1; //相当于去掉了尾数的最高位
	
	temp = (i+127)<<23 | temp;//计算指数，并与尾数合并起来 
	temp = temp & ~(1<<31);//确定正负  
	
	return temp;//这里就已经是以浮点数的存储方式表示的传进来的参数了
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
