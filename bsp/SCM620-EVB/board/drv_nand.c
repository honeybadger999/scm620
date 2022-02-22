#include <rtdevice.h>
#include <string.h>
#include <board.h>
#include <rtthread.h>
#include <drivers/mtd_nand.h>

#ifdef RT_USING_MTD_NAND

#define NAND_DEBUG    rt_kprintf
/* nandflash confg */
#define PAGES_PER_BLOCK         64
#define PAGE_DATA_SIZE          2048
#define PAGE_OOB_SIZE           64
#define ECC_SIZE                4

//S34ML01G2 芯片：为128MByte+4MByte， 分为1024个blocks, 每个block分为64个pages,每个page为2K+64 bytes
//                                    1024*64*(2K+64)

//用于设置NFC_FIFO值的基本函数
#define SetRandValFlag	0x20182018
#define KB1							0x400
#define B512						0x200
#define KB1_5						0x600
#define KB2							0x800
#define KB4 						0x1000
#define KB16						0x4000
#define KB32						0x8000

#define ColumnAddr0    0       //某个page上的第0个地址
#define ColumnAddr256    256   //某个page上的第256个地址

#define Page0          0x0     //the 1st block the 1st page
#define Page1					 0x1     //the 1st block the 2nd page
#define Page32					 32     //the 2nd block the 1st page

#define FDMALEN1_DMA0   1      //模式0时，FDMALength的值为1，此时实际长度512
#define FDMALEN2_DMA0   2      //模式0时，FDMALength的值为2，此时实际长度1K
#define FDMALEN3_DMA0   3      //模式0时，FDMALength的值为3，此时实际长度1.5K
#define FDMALEN4_DMA0   4      //模式0时，FDMALength的值为3，此时实际长度2K

#define MMUAddr0				0			// 用于FDMA命令，NFC FIFO 第几块SRAM   0-31块，每块512
#define MMUAddr1				1
#define MMUAddr2				2
#define MMUAddr3				3
#define MMUAddr4				4
#define MMUAddr8				8

#define BLOCK0          0x0     //the 1st block number
#define BLOCK1					0x1     //the 2nd block number

#define FDMAMODE0				FC_CMD_FDMA_MODE0  
#define FDMAMODE1				FC_CMD_FDMA_MODE1

 struct scm630_nand
{
    rt_uint8_t id[5];
    struct rt_mutex lock;
    struct rt_completion comp;
};

static struct scm630_nand _device;

void FlashReset(void)
{
	  fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_COMMAND,0,0xfF);  //reset
    fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_WAITR_B,0,0x40000);  //wait R/B
    fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_RUN    ,0,0x1);   //run
    wait_cmd_success();
}

//初始化NAND FLASH
void NAND_Init(void)
{
//		NFC_InitPad(NFC_NCE2_ENABLE);

    SGCC_NFC_REG_P->ch0_config |= (1<<8); //关WP 
		SGCC_NFC_REG_P->ctrl |= 0x1;
		SGCC_NFC_REG_P->flash_timing0 = 0x008;

 	  FlashReset();	
}

void getFlashID(void)
{
      SGCC_NFC_FIFO_REG_P->fulempsel =1;//软件管理空满
			SGCC_NFC_FIFO_REG_P->fifofulemp =0;//置满信号，NFC可读MEMORY
      fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_COMMAND,0,0x90);  //cmd
      fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE,0x0);  //addr
      fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_FDMA   ,FC_CMD_FDMA_DATA_READ/*FC_CMD_FDMA_DATA_WRITE*/ | FC_CMD_FDMA_MODE1,5<<7);  //FDMA1 bit0-6为MMU ADDR  bit7-21为DMALen
      fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_RUN    ,0,0x1);   //run
      wait_cmd_success(); 
}

static rt_err_t nand_S34ML01G2_readid(struct rt_mtd_nand_device *device)
{
    rt_uint32_t id;  
		
		getFlashID();
	
		_device.id[0] = *(UINT8*)(0X50750580);
		_device.id[1] = *(UINT8*)(0X50750581);
		_device.id[2] = *(UINT8*)(0X50750582);
		_device.id[3] = *(UINT8*)(0X50750583);
		_device.id[4] = *(UINT8*)(0X50750584);

    id=((rt_uint32_t)_device.id[1])<<24|((rt_uint32_t)_device.id[2])<<16|((rt_uint32_t)_device.id[3])<<8|_device.id[4];   
    rt_kprintf("NAND ID: 0x%08X\n", id);
    return RT_EOK;
    
}

void PageRead_DMA(UINT32 FlashColumnAddr,UINT32 FlashPageNum,UINT32 FlashBlockNum,UINT32 FDMALength,UINT8 MMUAddr,UINT8 mode)
{
	UINT32 FlashPageAddr = 0;
	UINT32 DELAY = 0X2000;
	UINT32 WAITRB= 0X4FFFF;
	
	FlashPageAddr = ((FlashPageNum&0x03F) | ((FlashBlockNum&0x3FF)<<6));//
	
	FlashColumnAddr &= 0xfff; //A0~A11
		SGCC_NFC_FIFO_REG_P->fulempsel = 1;//软件管理空满
	SGCC_NFC_FIFO_REG_P->fifofulemp =0; //置空
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_COMMAND,0,0x00);  //cmd
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE ,(UINT8)(FlashColumnAddr&0xFF));    //column addr A0~A7
  fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE ,(UINT8)((FlashColumnAddr>>8)&0xFF));      //column addr A8~A11
  fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE ,(UINT8)(FlashPageAddr)&0xFF);   //page Addr: A12~A17 Block addr A18-A19
  fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE ,(UINT8)(FlashPageAddr>>8)&0xFF);  //Block addr A20~A27
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE,(UINT8)0);  //Row Add.3 :s34ML01G2没有RowAdd.3,用0代替
	
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_COMMAND,0,0x30);  //cmd
	 
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_WAITR_B,0,WAITRB);  //wait R/B
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_DELAY,0,DELAY);  //delay
	
	if(mode == FDMAMODE1)
			fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_FDMA   ,FC_CMD_FDMA_DATA_READ|mode,(FDMALength<<7)|(MMUAddr&0x7F));  //FDMA1
	else if(mode == FDMAMODE0)
			fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_FDMA   ,FC_CMD_FDMA_DATA_READ|mode,(FDMALength<<8)|MMUAddr);  //FDMA

    fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_RUN    ,0,0x1);   //run
    wait_cmd_success();
}

void ReadFlash_DMA(UINT32 FlashColumnAddr,UINT32 FlashPageNum,UINT32 FlashBlockNum,UINT32 FDMALength,UINT8 MMUAddr,UINT8 mode)
{
	PageRead_DMA(FlashColumnAddr,FlashPageNum,FlashBlockNum,FDMALength,MMUAddr,mode);	
}


static rt_err_t nand_S34ML01G2_readpage(struct rt_mtd_nand_device *device,
                                     rt_off_t                   page,
                                     rt_uint8_t                *data,
                                     rt_uint32_t                data_len,
                                     rt_uint8_t                *spare,
                                     rt_uint32_t                spare_len)
{
    rt_uint32_t i;
		rt_uint8_t* readData = (rt_uint8_t*)(0x50750580);
	
    page = page + device->block_start * device->pages_per_block;
    if (page/device->pages_per_block > device->block_end)
    {
        return -RT_MTD_EIO;
    }
		
    rt_mutex_take(&_device.lock, RT_WAITING_FOREVER);

    if (data && data_len)
    {
			ReadFlash_DMA(ColumnAddr0,page%64,page/64,FDMALEN4_DMA0,MMUAddr0,FDMAMODE0);//通过MODE0 读取2KB
        
			for (i = 0; i < data_len; i ++)
			{
					data[i] = readData[i];
			}
    }
		
		if (spare && spare_len)
    {
			ReadFlash_DMA(2048,page%64,page/64,spare_len,MMUAddr0,FDMAMODE1);//通过MODE0 读取2KB
        
			for (i = 0; i < spare_len; i ++)
			{
					spare[i] = readData[i];//0xFF;
			}
    }
		
    rt_mutex_release(&_device.lock);

    return RT_MTD_EOK;
}

void PageProgram_DMA(UINT32 FlashColumnAddr,UINT32 FlashPageNum,UINT32 FlashBlockNum,UINT32 FDMALength,UINT8 MMUAddr,UINT8 mode)
{
	UINT32 FlashPageAddr = 0;
	
	FlashPageAddr = ((FlashPageNum&0x03F) | ((FlashBlockNum&0x3FF)<<6));
	
	FlashColumnAddr &= 0xfff; //A0~A11
	SGCC_NFC_FIFO_REG_P->fulempsel = 3;//软件管理空满
	SGCC_NFC_FIFO_REG_P->fifofulemp =3; //置满
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_COMMAND,0,0x80);  //cmd
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE ,(UINT8)(FlashColumnAddr&0xFF));    //column addr A0~A7
  fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE ,(UINT8)((FlashColumnAddr>>8)&0xFF));      //column addr A8~A11
  fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE ,(UINT8)(FlashPageAddr)&0xFF);   //page Addr: A12~A17 Block addr A18-A19
  fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE ,(UINT8)(FlashPageAddr>>8)&0xFF);  //Block addr A20~A27
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE,(UINT8)0);  //Row Add.3 :s34ML01G2没有RowAdd.3,用0代替

	if(mode == FDMAMODE1)
		fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_FDMA   ,FC_CMD_FDMA_DATA_WRITE|FC_CMD_FDMA_MODE1,(FDMALength<<7)|(MMUAddr&0x7F));  //FDMA1
  else if(mode == FDMAMODE0)
		fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_FDMA   ,FC_CMD_FDMA_DATA_WRITE|FC_CMD_FDMA_MODE0,(FDMALength<<8)|MMUAddr);  //FDMA
	
  fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_COMMAND,0,0x10);  //cmd
  fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_WAITR_B,0,0xffffff);  //wait R/B  
		fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_DELAY,0,0x3ffff);  //delay
  fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_RUN    ,0,0x1);   //run
  wait_cmd_success();
}

void WriteFlash_DMA(UINT32 FlashColumnAddr,UINT32 FlashPageNum,UINT32 FlashBlockNum,UINT32 FDMALength,UINT8 MMUAddr,UINT8 mode) //
{
	PageProgram_DMA(FlashColumnAddr,FlashPageNum,FlashBlockNum,FDMALength,MMUAddr,mode);
}

static rt_err_t nand_S34ML01G2_writepage(struct rt_mtd_nand_device *device,
                                      rt_off_t                   page,
                                      const rt_uint8_t          *data,
                                      rt_uint32_t                data_len,
                                      const rt_uint8_t          *spare,
                                      rt_uint32_t                spare_len)
{
    rt_uint32_t i;
		rt_uint32_t* writeData = (rt_uint32_t*)(0x50750580);
  
    page = page + device->block_start * device->pages_per_block;
    if (page/device->pages_per_block > device->block_end)
    {
        return -RT_MTD_EIO;
    }

    rt_mutex_take(&_device.lock, RT_WAITING_FOREVER);

    if (data && data_len)
    {
    
        for (i = 0; i < data_len/4; i ++)
        {
            writeData[i] = (data[i*4 + 3]<<24)+(data[i*4 + 2]<<16)+(data[i*4 + 1]<<8)+(data[i*4 + 0]<<0);
        }
				WriteFlash_DMA(ColumnAddr0,page%64,page/64,FDMALEN4_DMA0,MMUAddr0,FDMAMODE0);//通过MODE0写入2KB
    }

		if (spare && spare_len)
    {
			for (i = 0; i < spare_len/4; i ++)
			{
					writeData[i] = (spare[i*4 + 3]<<24)+(spare[i*4 + 2]<<16)+(spare[i*4 + 1]<<8)+(spare[i*4 + 0]<<0);
			}
			WriteFlash_DMA(2048,page%64,page/64,spare_len,MMUAddr0,FDMAMODE1);//通过MODE0写入2KB
    }
		
    rt_mutex_release(&_device.lock);

    return RT_MTD_EOK;

}


void blockEarseFlash(UINT32 blocknum) //blocknum : 0-1024
{
	UINT32 BLOCKEARSERB =0x8fffff;
	UINT32 BLOCKEARSEDEL=0x7fff;
	
	blocknum &= 0x3FF;
	 SGCC_NFC_FIFO_REG_P->fulempsel =1;//软件管理空满
	SGCC_NFC_FIFO_REG_P->fifofulemp =1;  
	
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_COMMAND,0,0x60);  //cmd
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE,(UINT8)((blocknum&0x03)<<6));  //Row Add.1 : BA0~BA1
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE,(UINT8)(blocknum>>2)&0xFF);  //Row Add.2 : BA2~BA9              | 
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_ADDRESS,FC_CMD_ADDR_LOW_1BYTE,(UINT8)0);  //Row Add.3 :s34ML01G2没有RowAdd.3,用0代替

	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_COMMAND,0,0xd0);  //cmd
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_WAITR_B,0,BLOCKEARSERB);  //wait R/B
	SGCC_NFC_FIFO_REG_P->fifofulemp =0;  
		fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_DELAY,0,BLOCKEARSEDEL);  //delay
	 
	fc_send_cmd(FC_CE_CHIP0,FC_CMD_TYPE_RUN    ,0,0x1);   //run
      wait_cmd_success(); 
}


static rt_err_t nand_S34ML01G2_eraseblock(struct rt_mtd_nand_device *device,
                                       rt_uint32_t block)
{
//    unsigned int blockPage;
    /* add the start blocks */
    block = block + device->block_start;
//    blockPage = (block << 6);

    rt_mutex_take(&_device.lock, RT_WAITING_FOREVER);

		blockEarseFlash(block);
	
    rt_mutex_release(&_device.lock);
    return RT_MTD_EOK;
}

static rt_err_t nand_S34ML01G2_pagecopy(struct rt_mtd_nand_device *device,
                                     rt_off_t                   src_page,
                                     rt_off_t                   dst_page)
{
    rt_uint32_t source_block=0,dest_block=0; 
    src_page = src_page + device->block_start * device->pages_per_block;
    dst_page = dst_page + device->block_start * device->pages_per_block;
    //判断源页和目的页是否在同一个plane中
    source_block=src_page/device->pages_per_block;
    dest_block=dst_page/device->pages_per_block;
    if((source_block%2)!=(dest_block%2))return RT_MTD_ESRC;	//不在同一个plane内 
	
		
		ReadFlash_DMA(ColumnAddr0,src_page%64,src_page/64,FDMALEN4_DMA0,MMUAddr0,FDMAMODE0);//通过MODE0 读取2KB
		WriteFlash_DMA(ColumnAddr0,dst_page%64,dst_page/64,FDMALEN4_DMA0,MMUAddr0,FDMAMODE0);//通过MODE0写入2KB

    return RT_MTD_EOK;


}

static rt_err_t nand_S34ML01G2_checkblock(struct rt_mtd_nand_device* device, rt_uint32_t block)
{
    return (RT_MTD_EOK);
}

static rt_err_t nand_S34ML01G2_markbad(struct rt_mtd_nand_device* device, rt_uint32_t block)
{
    return (RT_MTD_EOK);
}

static const struct rt_mtd_nand_driver_ops ops =
{
    nand_S34ML01G2_readid,
    nand_S34ML01G2_readpage,
    nand_S34ML01G2_writepage,
    nand_S34ML01G2_pagecopy,
    nand_S34ML01G2_eraseblock,
    nand_S34ML01G2_checkblock,
    nand_S34ML01G2_markbad,
};
static struct rt_mtd_nand_device _partition[1];

int rt_hw_nand_init(void)
{
		NAND_Init();
    rt_mutex_init(&_device.lock, "nand", RT_IPC_FLAG_FIFO);
    /* register nand0 */
    _partition[0].page_size       = PAGE_DATA_SIZE;
    _partition[0].pages_per_block = PAGES_PER_BLOCK;
    _partition[0].plane_num       = 2;
    _partition[0].oob_size        = PAGE_OOB_SIZE;
    _partition[0].oob_free        = PAGE_OOB_SIZE - ((PAGE_DATA_SIZE) * 3 / 256);
    _partition[0].block_start     = 0;
    _partition[0].block_end       = 1023;

    _partition[0].block_total     = _partition[0].block_end - _partition[0].block_start;
    _partition[0].ops             = &ops;

    rt_mtd_nand_register_device("nand0", &_partition[0]);
    nand_S34ML01G2_readid(&_partition[0]);

    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_nand_init);


#endif

