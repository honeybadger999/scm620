#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
//#include "LE_uart.h"
//#include "LE_can.h"

#include "SCM62X.h"
//#include "hal_w25q64.h"
/* Includes ------------------------------------------------------------------*/

#include "LE_sst_flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t	Buffer_DataFlash[256];
uint16_t wbuf[255];
uint16_t rbuf[255];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/****************************************************************
  * @brief  SPI初始化，芯片为sstv25 flash存储芯片

  * @param  无
  * @retval 无
  ***************************************************************/


void hal_w25q64_send_byte(uint8_t val)
{
    SPI_WriteByte(SGCC_SPI1_P, val);
    SPI_RdByte(SGCC_SPI1_P);
}

uint8_t hal_w25q64_rec_byte(void)
{
    uint8_t result;
    SPI_WriteByte(SGCC_SPI1_P, 0xff);
    result = SPI_RdByte(SGCC_SPI1_P);
    return result;
}

void hal_w25q64_start(void)
{
    SPI_MASTER initMaster;
    initMaster.rxEn = SPI_CR1_R_EN;
    initMaster.txEn = SPI_CR1_T_EN;
    initMaster.sckDiv = 25;
    initMaster.mode = SPI_CR1_SCK_MOD3;
    initMaster.txFifoLvl = SPI_CR2_TTHD_8;
    initMaster.rxFifoLvl = SPI_CR2_RTHD_0;
    initMaster.DataNumEn = SPI_CR3_DNUM_DIS;
    initMaster.DataNum = 0;
    initMaster.waitNum = 0;
    SPI_InitMaster(SGCC_SPI1_P, &initMaster );
	SPI_StartTransferAsMaster(SGCC_SPI1_P);
}

void SPI_FLASH_Init(void)
{


    /*	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // 双线全双工
    	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                     //主机模式
    	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                 // 通讯帧8位
    	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                      //时钟极性 低电平
    	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                    // SCK奇数边沿采集数据
    	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                       //使能信号 手动控制
    	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;     //波特率64分频
    	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;              // 高位数据先行
    	SPI_InitStructure.SPI_CRCPolynomial = 7;
    	SPI_Init(FLASH_SPI, &SPI_InitStructure);
    	SPI_Cmd(FLASH_SPI, ENABLE);
    */
    SPI_MASTER initMaster;
	
	SPI1_InitPad();
    SPI_ResetReg(SGCC_SPI1_P);
	SPI_StopTransferAsMaster(SGCC_SPI1_P);
	
    initMaster.rxEn = SPI_CR1_R_EN;
    initMaster.txEn = SPI_CR1_T_EN;
    initMaster.sckDiv = 25;
    initMaster.mode = SPI_CR1_SCK_MOD3;
    initMaster.txFifoLvl = SPI_CR2_TTHD_8;
    initMaster.rxFifoLvl = SPI_CR2_RTHD_0;
    initMaster.DataNumEn = SPI_CR3_DNUM_DIS;
    initMaster.DataNum = 0;
    initMaster.waitNum = 0;
    SPI_InitMaster(SGCC_SPI1_P, &initMaster );
	//hal_w25q64_start();

}

/*******************************************************************************
* Function Name  : Flash_ReadWriteByte
* Description    : 通过硬件SPI发送一个字节到SST25VF016B
* Input          : - data: 发送的数据
* Output         : None
* Return         : SST25VF016B 返回的数据
* Attention		 : None
*******************************************************************************/
/*
uint8_t Flash_ReadWriteByte(uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);
    //while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_BSY) == SET);
	SPI_I2S_SendData(FLASH_SPI, data);
    while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);
   // while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_BSY) == SET);
	return SPI_I2S_ReceiveData(FLASH_SPI);
}
*/
/*******************************************************************************
* Function Name  : SSTF016B_RD
* Description    : SST25VF016B的读函数,可选择读ID和读数据操作
* Input          : - Dst: 目标地址,范围 0x0 - MAX_ADDR（MAX_ADDR = 0x1FFFFF）
*                  - RcvBufPt: 接收缓存的指针
*                  - NByte: 要读取的数据字节数
* Output         : 操作成功则返回OK,失败则返回ERROR
* Return         : SST25VF016B 返回的数据
* Attention		 : 若某功能下,某一入口参数无效,可在该入口参数处填Invalid，该参数将被忽略
*******************************************************************************/
uint8_t SSTF016B_RD(uint32_t Dst, uint8_t* RcvBufPt, uint32_t NByte)
{
    uint16_t i = 0;

    if ((Dst + NByte > MAX_ADDR) || (NByte == 0))	return (ERROR);	 /*	检查入口参数 */

    hal_w25q64_start();
    hal_w25q64_send_byte(0x0B); 											/* 发送读命令 */
    hal_w25q64_send_byte(((Dst & 0xFFFFFF) >> 16));	/* 发送地址信息:该地址由3个字节组成	*/
    hal_w25q64_send_byte(((Dst & 0xFFFF) >> 8));
    hal_w25q64_send_byte(Dst & 0xFF);
    hal_w25q64_send_byte(0xFF);											/* 发送一个哑字节以读取数据	*/

    for (i = 0; i < NByte; i++)
    {
        RcvBufPt[i] = hal_w25q64_rec_byte();
    }

    SPI_StopTransferAsMaster(SGCC_SPI1_P);
    return (ENABLE);
}

/*******************************************************************************
* Function Name  : SSTF016B_RdID
* Description    : SST25VF016B的读ID函数,可选择读ID和读数据操作
* Input          : - IDType: ID类型。用户可在Jedec_ID,Dev_ID,Manu_ID三者里选择
* Output         : - RcvbufPt: 存储ID变量的指针
* Return         : 操作成功则返回OK,失败则返回ERROR
* Attention		 : 若填入的参数不符合要求，则返回ERROR
*******************************************************************************/
/*uint8_t SSTF016B_RdID(idtype IDType,uint32_t* RcvbufPt)
{
	uint32_t temp = 0;

	if (IDType == Jedec_ID)
	{
	//	hal_w25q64_start();
		Flash_ReadWriteByte(0x9F);
		temp = (temp | Flash_ReadWriteByte(0x00)) << 8;
		temp = (temp | Flash_ReadWriteByte(0x00)) << 8;
		temp = (temp | Flash_ReadWriteByte(0x00));
	//	SPI_FLASH_CS_HIGH();
		*RcvbufPt = temp;
		return (ENABLE);
	}

	if ((IDType == Manu_ID) || (IDType == Dev_ID) )
	{
	  hal_w25q64_start();
		Flash_ReadWriteByte(0x90);
    Flash_ReadWriteByte(0x00);
		Flash_ReadWriteByte(0x00);
		Flash_ReadWriteByte(IDType);
		temp = Flash_ReadWriteByte(0x00);
		SPI_FLASH_CS_HIGH();
		*RcvbufPt = temp;
		return (ENABLE);
	}
	else
	{
		return (ERROR);
	}
}
*/
/*******************************************************************************
* Function Name  : SSTF016B_WR
* Description    : SST25VF016B的写函数，可写1个和多个数据到指定地址
* Input          : - Dst: 目标地址,范围 0x0 - MAX_ADDR（MAX_ADDR = 0x1FFFFF）
*                  - SndbufPt: 发送缓存区指针
*                  - NByte: 要写的数据字节数
* Output         : None
* Return         : 操作成功则返回OK,失败则返回ERROR
* Attention		 : 若某功能下,某一入口参数无效,可在该入口参数处填Invalid，该参数将被忽略
*******************************************************************************/
uint8_t SSTF016B_WR(uint32_t Dst, uint8_t* SndbufPt, uint32_t NByte)
{
    uint8_t temp = 0, StatRgVal = 0;
    uint32_t i = 0;

    if (( (Dst + NByte - 1 > MAX_ADDR) || (NByte == 0) ))		return (ERROR);	 /*	检查入口参数 */

	hal_w25q64_start();
    hal_w25q64_send_byte(0x05);				/* 发送读状态寄存器命令	*/
    temp = hal_w25q64_rec_byte();	/* 保存读得的状态寄存器值 */
	SPI_StopTransferAsMaster(SGCC_SPI1_P);

	hal_w25q64_start();
    hal_w25q64_send_byte(0x50);				/* 使状态寄存器可写	*/
	SPI_StopTransferAsMaster(SGCC_SPI1_P);

	hal_w25q64_start();
    hal_w25q64_send_byte(0x01);				/* 发送写状态寄存器指令 */
    hal_w25q64_send_byte(0);						/* 清0BPx位，使Flash芯片全区可写 */
	SPI_StopTransferAsMaster(SGCC_SPI1_P);

    for(i = 0; i < NByte; i++)
    {
		hal_w25q64_start();
        hal_w25q64_send_byte(0x06);			/* 发送写使能命令 */
		SPI_StopTransferAsMaster(SGCC_SPI1_P);

		hal_w25q64_start();
        hal_w25q64_send_byte(0x02); 						    						/* 发送字节数据烧写命令	*/
        hal_w25q64_send_byte((((Dst + i) & 0xFFFFFF) >> 16));  	/* 发送3个字节的地址信息 */
        hal_w25q64_send_byte((((Dst + i) & 0xFFFF) >> 8));
        hal_w25q64_send_byte((Dst + i) & 0xFF);
        hal_w25q64_send_byte(SndbufPt[i]);											/* 发送被烧写的数据	*/
		SPI_StopTransferAsMaster(SGCC_SPI1_P);


        do
        {
            hal_w25q64_start();
            hal_w25q64_send_byte(0x05);					    		/* 发送读状态寄存器命令 */
            StatRgVal = hal_w25q64_rec_byte();			/* 保存读得的状态寄存器值 */
            SPI_StopTransferAsMaster(SGCC_SPI1_P);
        }
        while (StatRgVal == 0x03);					          /* 一直等待，直到芯片空闲	*/
    }

	hal_w25q64_start();
    hal_w25q64_send_byte(0x06);	/* 发送写使能命令 */
	SPI_StopTransferAsMaster(SGCC_SPI1_P);

	hal_w25q64_start();
    hal_w25q64_send_byte(0x50);	/* 使状态寄存器可写	*/
	SPI_StopTransferAsMaster(SGCC_SPI1_P);

	hal_w25q64_start();
    hal_w25q64_send_byte(0x01);	/* 发送写状态寄存器指令 */
    hal_w25q64_send_byte(temp);	/* 恢复状态寄存器设置信息 */
	SPI_StopTransferAsMaster(SGCC_SPI1_P);

    return (ENABLE);
}

/*******************************************************************************
* Function Name  : SSTF016B_Erase
* Description    : 根据指定的扇区号选取最高效的算法擦除
* Input          : - sec1: 起始扇区号,范围(0~511)
*                  - sec2: 终止扇区号,范围(0~511)
* Output         : None
* Return         : 操作成功则返回OK,失败则返回ERROR
* Attention		 : None
*******************************************************************************/
uint8_t SSTF016B_Erase(uint32_t sec1, uint32_t sec2)
{
    uint8_t  temp1 = 0, temp2 = 0, StatRgVal = 0;
    uint32_t SecnHdAddr = 0;
    uint32_t no_SecsToEr = 0;				   			    /* 要擦除的扇区数目 */
    uint32_t CurSecToEr = 0;	  						    /* 当前要擦除的扇区号 */

    if ((sec1 > SEC_MAX) || (sec2 > SEC_MAX))			return (ERROR);			/*  检查入口参数 */

    hal_w25q64_start();
    hal_w25q64_send_byte(0x05);						/* 发送读状态寄存器命令	*/
    temp1 = hal_w25q64_rec_byte();		/* 保存读得的状态寄存器值 */
	SPI_StopTransferAsMaster(SGCC_SPI1_P);

	hal_w25q64_start();
    hal_w25q64_send_byte(0x50);						/* 使状态寄存器可写	*/
	SPI_StopTransferAsMaster(SGCC_SPI1_P);

	hal_w25q64_start();
    hal_w25q64_send_byte(0x01);						/* 发送写状态寄存器指令	*/
    hal_w25q64_send_byte(0);								/* 清0BPx位，使Flash芯片全区可写 */
	SPI_StopTransferAsMaster(SGCC_SPI1_P);

	hal_w25q64_start();
    hal_w25q64_send_byte(0x06);							/* 发送写使能命令 */
	SPI_StopTransferAsMaster(SGCC_SPI1_P);

    if (sec1 > sec2)	/* 如果用户输入的起始扇区号大于终止扇区号，则在内部作出调整 */
    {
        temp2 = sec1;
        sec1  = sec2;
        sec2  = temp2;
    }

    if (sec1 == sec2)		/* 若起止扇区号相等则擦除单个扇区 */
    {
		hal_w25q64_start();
        hal_w25q64_send_byte(0x06);						    /* 发送写使能命令 */
		SPI_StopTransferAsMaster(SGCC_SPI1_P);

        SecnHdAddr = SEC_SIZE * sec1;				      /* 计算扇区的起始地址	*/
	    hal_w25q64_start();
        hal_w25q64_send_byte(0x20);							  /* 发送扇区擦除指令 */
        hal_w25q64_send_byte(((SecnHdAddr & 0xFFFFFF) >> 16)); /* 发送3个字节的地址信息 */
        hal_w25q64_send_byte(((SecnHdAddr & 0xFFFF) >> 8));
        hal_w25q64_send_byte(SecnHdAddr & 0xFF);

		SPI_StopTransferAsMaster(SGCC_SPI1_P);
        do
        {
		    hal_w25q64_start();
            hal_w25q64_send_byte(0x05);						  /* 发送读状态寄存器命令 */
            StatRgVal = hal_w25q64_rec_byte();	/* 保存读得的状态寄存器值	*/
			SPI_StopTransferAsMaster(SGCC_SPI1_P);
        }
        while (StatRgVal == 0x03);				         /* 一直等待，直到芯片空闲	*/

        return (ENABLE);
    }

    /* 根据起始扇区和终止扇区间距调用最快速的擦除功能 */

    if (sec2 - sec1 == SEC_MAX)
    {
		hal_w25q64_start();
        hal_w25q64_send_byte(0x60);							  /* 发送芯片擦除指令(60h or C7h) */

		SPI_StopTransferAsMaster(SGCC_SPI1_P);
        do
        {
		  hal_w25q64_start();
            hal_w25q64_send_byte(0x05);						  /* 发送读状态寄存器命令 */
            StatRgVal = hal_w25q64_rec_byte();  /* 保存读得的状态寄存器值	*/
			SPI_StopTransferAsMaster(SGCC_SPI1_P);
        }
        while (StatRgVal == 0x03);					       /* 一直等待，直到芯片空闲	*/

        return (ENABLE);
    }

    no_SecsToEr = sec2 - sec1 + 1;					      /* 获取要擦除的扇区数目 */
    CurSecToEr  = sec1;								           /* 从起始扇区开始擦除	*/

    /* 若两个扇区之间的间隔够大，则采取8扇区擦除算法 */
    while (no_SecsToEr >= 8)
    {
		hal_w25q64_start();
        hal_w25q64_send_byte(0x06);						    /* 发送写使能命令 */
		SPI_StopTransferAsMaster(SGCC_SPI1_P);

        SecnHdAddr = SEC_SIZE * CurSecToEr;			  /* 计算扇区的起始地址 */
	  hal_w25q64_start();
        hal_w25q64_send_byte(0x52);							  /* 发送32KB擦除指令 */
        hal_w25q64_send_byte(((SecnHdAddr & 0xFFFFFF) >> 16)); /* 发送3个字节的地址信息 */
        hal_w25q64_send_byte(((SecnHdAddr & 0xFFFF) >> 8));
        hal_w25q64_send_byte(SecnHdAddr & 0xFF);

  	SPI_StopTransferAsMaster(SGCC_SPI1_P);
        do
        {
		  hal_w25q64_start();
            hal_w25q64_send_byte(0x05);						  /* 发送读状态寄存器命令 */
            StatRgVal = hal_w25q64_rec_byte();  /* 保存读得的状态寄存器值	*/
			SPI_StopTransferAsMaster(SGCC_SPI1_P);
        }
        while (StatRgVal == 0x03);					     /* 一直等待，直到芯片空闲	*/

        CurSecToEr  += 8;
        no_SecsToEr -=  8;
    }

    /* 采用扇区擦除算法擦除剩余的扇区 */
    while (no_SecsToEr >= 1)
    {
		hal_w25q64_start();
        hal_w25q64_send_byte(0x06);						    /* 发送写使能命令 */
		SPI_StopTransferAsMaster(SGCC_SPI1_P);

        SecnHdAddr = SEC_SIZE * CurSecToEr;			          /* 计算扇区的起始地址 */
	  hal_w25q64_start();
        hal_w25q64_send_byte(0x20);							  /* 发送扇区擦除指令 */
        hal_w25q64_send_byte(((SecnHdAddr & 0xFFFFFF) >> 16)); /* 发送3个字节的地址信息 */
        hal_w25q64_send_byte(((SecnHdAddr & 0xFFFF) >> 8));
        hal_w25q64_send_byte(SecnHdAddr & 0xFF);

  	SPI_StopTransferAsMaster(SGCC_SPI1_P);
        do
        {
		  hal_w25q64_start();
            hal_w25q64_send_byte(0x05);						  /* 发送读状态寄存器命令 */
            StatRgVal = hal_w25q64_rec_byte();			  /* 保存读得的状态寄存器值	*/
			SPI_StopTransferAsMaster(SGCC_SPI1_P);
        }
        while (StatRgVal == 0x03);					          /* 一直等待，直到芯片空闲 */

        CurSecToEr  += 1;
        no_SecsToEr -=  1;
    }

    /* 擦除结束,恢复状态寄存器信息 */
	hal_w25q64_start();
    hal_w25q64_send_byte(0x06);								  /* 发送写使能命令 */
	SPI_StopTransferAsMaster(SGCC_SPI1_P);

	hal_w25q64_start();
    hal_w25q64_send_byte(0x50);								  /* 使状态寄存器可写 */
	SPI_StopTransferAsMaster(SGCC_SPI1_P);
	hal_w25q64_start();
    hal_w25q64_send_byte(0x01);								  /* 发送写状态寄存器指令 */
    hal_w25q64_send_byte(temp1);								  /* 恢复状态寄存器设置信息 */
	SPI_StopTransferAsMaster(SGCC_SPI1_P);
    return (ENABLE);
}


/* SPI*/
/*
static uint32_t CurReadAddr;
static uint32_t CurWriteAddr;
void df_read_open(uint32_t addr)	{  CurReadAddr=addr;	}
void df_write_open(uint32_t addr)	{	 CurWriteAddr=addr;	}

void df_read(uint8_t *buf,uint16_t size)
{
  if( CurReadAddr + size <= MAX_ADDR )
  {
    SSTF016B_RD(CurReadAddr,buf,size);
    CurReadAddr+=size;
  }
}

void df_write(uint8_t *buf,uint16_t size)
{
  if( CurWriteAddr + size <= MAX_ADDR )
  {
    SSTF016B_WR(CurWriteAddr,buf,size);
    CurWriteAddr+=size;
  }
}

void df_read_close(void)		{	}
void df_write_close(void)		{	}

void df_read_seek(uint32_t addr)
{
 df_read_close();
 df_read_open(addr);
}
void df_write_seek(uint32_t addr)
{
 df_write_close();
 df_write_open(addr);
}

void SPI_FLASH_Check(void)
{
	uint16_t i = 0, flash_start_addr = 0x0;
	unsigned char WriteBuffer[256], ReadBuffer[256];
	for(i = 0;i < 256;i ++)		WriteBuffer[i] = i;
	SSTF016B_Erase( flash_start_addr/SEC_SIZE , ( flash_start_addr + sizeof(WriteBuffer) )/SEC_SIZE +1 );
	df_write_open( flash_start_addr );
	df_write( (unsigned char*)WriteBuffer,256);
	df_write_close();
	df_read_open(flash_start_addr);
	df_read(ReadBuffer, 256);
	for(i = 0;i < 256;i ++)
	{
		if(ReadBuffer[i] != i)
		{
			break;
		}
	}
}
*/
void Store_par_DataFlash(uint16_t *P_DATA_PAR, uint16_t len, uint16_t sector, uint32_t ParaFlashADD)
{
    uint16_t i, temp;


    for(i = 0; i < len; i++)
    {
        temp =  *P_DATA_PAR++;

        Buffer_DataFlash[i * 2] = (temp >> 8) & 0x00ff;
        Buffer_DataFlash[i * 2 + 1] = temp & 0x00ff;

    }

    SSTF016B_Erase( sector,  sector );  //擦sector扇区 ，擦除完了才能写 一个扇区大小=0x1000
    SSTF016B_WR( ParaFlashADD, Buffer_DataFlash, len * 2);

}


void flash_thread_entry(void *parameter)
{
    struct rt_spi_device *spi_dev_sst25;
	uint16_t i;

	uint16_t aaa[10];
	uint8_t bbb[20];
    /* 查 找 spi 设 备 获 取 设 备 句 柄 */
    spi_dev_sst25 = (struct rt_spi_device *)rt_device_find("spi1");

    if (!spi_dev_sst25)
    {
        rt_kprintf("spi sample run failed! can't find %s device!\n", "spi1");
    }

    SPI_FLASH_Init();

    for(;;)
    {
		
		rt_thread_delay(3000/ portTICK_RATE_MS);
		for (i = 0; i < sizeof(wbuf); i++)
		{
	//		wbuf[i] = i;
		}
		
//		Store_par_DataFlash(wbuf,FLASHQUAN_INFO,FLASHSECTOR_INFO,FLASHADD_INFO);
//		SSTF016B_RD(FLASHADD_INFO, Buffer_DataFlash , FLASHQUAN_INFO*2);
    }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
