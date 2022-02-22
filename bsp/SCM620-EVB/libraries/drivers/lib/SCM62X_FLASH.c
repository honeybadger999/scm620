/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: 北京智芯微电子                                                   */
/*                                                                            */
/* 文件名称: SCM62X_FLASH.c                                                   */
/* 内容摘要: FLASH模块基础函数定义                                            */
/* 其它说明: **                                                               */
/* 当前版本: V1.0                                                             */
/* 作    者: SCM                                                              */
/* 完成日期: 2019-03-20                                                       */
/* 修改记录:                                                                  */
/*    修改日期          版本号        修改人        修改内容                  */
/* -------------------------------------------------------------------------- */
/*    2019-03-20        V1.0          SCM        创建文件                     */
/*<FH->************************************************************************/


/******************************************************************************/
/*               #include（依次为标准库头文件、非标准库头文件）               */
/******************************************************************************/
#include "SCM62X.h"

/******************************************************************************/
/*                                外部引用定义                                */
/******************************************************************************/


/******************************************************************************/
/*                                  全局变量                                  */
/******************************************************************************/

/*-----------------------------------------------------------------------------
  Function:		FLASH_EnableSupervisor                                                                                                                                                                           
  input:     	option --ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 打开或关闭超级权限                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void FLASH_EnableSupervisor(UINT8 option ) 
{
    if(option == ENABLE)
    {
        SGCC_FMC_P->FMC_PROTECT = 0xacce5509;
        SGCC_FMC_P->FMC_PROTECT = 0xacce55e0;
    }    
    else
    {
        SGCC_FMC_P->FMC_PROTECT = 0xacce55c1;
        SGCC_FMC_P->FMC_PROTECT = 0xacce5505;
    }
}
/*-----------------------------------------------------------------------------
  Function:		FLASH_SetNvrKey                                                                                                                                                                           
  input:     	None                 	  
  output:     None                                                                                    
  Returns:    1--success  0--fail                                                                                      
  Description: 设置information block保护字                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_SetNvrKey() 
{
	  
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;   //clear program fail bit
    
	  SGCC_FMC_P->PROG_START = 1;  
    
    Write32(NVR_KEY_ADDR, NVR_KEY);
    
    while(1)
    {
    	if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	{
    		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
    		{
    			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
    			 return 0;
    		}
    		else
    			 return 1;
    	}
    }
    
	  
}
/*-----------------------------------------------------------------------------
  Function:		FLASH_SetUpsKey                                                                                                                                                                           
  input:     	None                 	  
  output:     None                                                                                    
  Returns:    1--success  0--fail                                                                                  
  Description: 设置main block保护字                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_SetUpsKey() 
{
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;   //clear program fail bit
	  SGCC_FMC_P->PROG_START = 1;  
    Write32(UPS_KEY_ADDR, UPS_KEY);
    while(1)
    {
    	if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	{
    		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
    		{
    			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
    			 return 0;
    		}
    		else
    			 return 1;
    	}
    } 
}

/*-----------------------------------------------------------------------------
  Function:		FLASH_ConfigTimPara                                                                                                                                                                           
  input:     	                	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 配置时序参数                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void FLASH_ConfigTimPara(FLASH_TIMPARA timPara ) 
{
    SGCC_FMC_P->T_1US_REF = FLASH_CLK * timPara.t_1usRef / 1000; 
    SGCC_FMC_P->T_RW_CNT = FLASH_CLK * timPara.t_rwCnt / 1000; 
    SGCC_FMC_P->T_AEPH_CNT = FLASH_CLK * timPara.t_aephCnt / 1000; 
    SGCC_FMC_P->T_AA_CNT = FLASH_CLK * timPara.t_aaCnt / 1000; 
    SGCC_FMC_P->T_APH_CNT = FLASH_CLK * timPara.t_aphCnt / 1000; 
    SGCC_FMC_P->T_DS_CNT = FLASH_CLK * timPara.t_dsCnt / 1000; 
    
    SGCC_FMC_P->T_HV_US_CNT = timPara.t_hvUsCnt ;  
    SGCC_FMC_P->T_PROG_US_CNT = timPara.t_progUsCnt; 
    SGCC_FMC_P->T_NVSP_US_CNT = timPara.t_nvspUsCnt; 
    SGCC_FMC_P->T_PGS_US_CNT =  timPara.t_pgsUsCnt ; 
    SGCC_FMC_P->T_RCVP_US_CNT = timPara.t_rcvpUsCnt ; 
    SGCC_FMC_P->T_SERASE_US_CNT = timPara.t_seraseUsCnt ; 
    SGCC_FMC_P->T_CERASE_US_CNT = timPara.t_ceraseUsCnt ; 
    SGCC_FMC_P->T_NVSC_US_CNT =  timPara.t_nvscUsCnt ; 
    SGCC_FMC_P->T_RCVE_US_CNT =  timPara.t_rcveUsCnt ; 
 
}

void FLASH_ConfigTimPara1(FLASH_TIMPARA timPara ) 
{
    

    SGCC_FMC_P->T_NVSP_US_CNT = 5; 

 
}
/*-----------------------------------------------------------------------------
  Function:		FLASH_ProgSingleWord                                                                                                                                                                          
  input:     	addr: flash address
                buf:  
                len:                
  output:     None                                                                                    
  Returns:   1--success  0--fail                                                                                   
  Description: program 单个word                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_ProgSingleWord(UINT32 *addr, UINT32 dataWord) 
{
  /* */ while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
    
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;   //clear program fail bit
    
   
	  SGCC_FMC_P->PROG_START = 1;  
    
    *addr = dataWord;
    
     while(1)
    {
    	if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	{
    		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
    		{
    			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
    			 return 0;
    		}
    		else
    			 return 1;
    	}
    }
    
    /*
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
    SGCC_FMC_P->PROG_START = 1;  
    *addr++ = dataWord;
    */
}

/*-----------------------------------------------------------------------------
  Function:		FLASH_ProgWordsInPage                                                                                                                                                                          
  input:     	addr: flash address
                buf:  
                len:                
  output:     None                                                                                    
  Returns:    1--success  0--fail                                                                                   
  Description: program multiple words                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_ProgWordsInPage(UINT32 *addr, UINT32 *buf, UINT32 len) 
{
    UINT32 i;
    
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
    
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;   //clear program fail bit
    
	  SGCC_FMC_P->PROG_START = 1;  
    
    for(i = 0; i < len; i++)
    {
        addr[i] = buf[i];
    }
    while(1)
    {
    	if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	{
    		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
    		{
    			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
    			 return 0;
    		}
    		else
    			 return 1;
    	}
    }  
}

/*-----------------------------------------------------------------------------
  Function:		FLASH_ProgMultiWords                                                                                                                                                                          
  input:     	addr: flash address
                buf:  
                len: number of words                 
  output:     None                                                                                    
  Returns:    1--success  0--fail                                                                                   
  Description: program multiple words                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_ProgMultiWords(UINT32 *addr, UINT32 *buf, UINT32 len) 
{
    UINT32 i;
    UINT32  pageNum, curPage;
    
    pageNum = len/32;
    curPage = 0;
   
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;   //clear program fail bit
    
    if(pageNum > 0)        //if more than one page need to be written
	{
		while(pageNum>0)
		{
			while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
			 SGCC_FMC_P->PROG_START = 1; 
			 for(i = 0; i < 32; i++)
			{
				*(addr+i+curPage*32) = buf[i+curPage*32];
			}
            while(1)
            {
    	        if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	        {
    		        if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
    		        {
    			        SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
    			        return 0;
    		        }
    		        else
    			        break;
    	        }
            }  
			pageNum--;
			curPage++;
		}
	}		
	if(len%32)     
	{
		addr = addr+4*32*curPage;
		while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
		SGCC_FMC_P->PROG_START = 1; 
		for(i=0;i<len%32;i++)
		{
			*(addr+i) = buf[i+32*curPage];
		}
		while(1)
        {
    	    if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	    {
    	        if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
    	        {
    		        SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
    		        return 0;
    	        }
    	        else
    		        return 1;
    	        }
        }  
	}
	return 1;
}

/*-----------------------------------------------------------------------------
  Function:		FLASH_SectorErase                                                                                                                                                                          
  input:     eraseAdr-address to erase	                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: start sector erase                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_SectorErase(UINT32 *eraseAdr) 
{
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x4;   //clear erase fail bit
	  SGCC_FMC_P->SECTOR_ERASE_START = 1;  
    *eraseAdr = 0xffffffff;
    while(1)
    {
    	if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	{
    		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x4)
    		{
    			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x4;
    			 return 0;
    		}
    		else
    			 return 1;
    	}
    }  
}

/*-----------------------------------------------------------------------------
  Function:		FLASH_ChipErase                                                                                                                                                                          
  input:     	None                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 启动chip erase                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_ChipErase(UINT32 *eraseAdr) 
{
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x4;   //clear erase fail bit
	  SGCC_FMC_P->CHIP_ERASE_START = 1;  
    *eraseAdr = 0xffffffff;
    while(1)
    {
    	if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	{
    		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x4)
    		{
    			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x4;
    			 return 0;
    		}
    		else
    			 return 1;
    	}
    }  
}

/*-----------------------------------------------------------------------------
  Function:		EFLASH_Read                                                                                                                                                                            
  input:     
            len, WORD as unit
  output:     buf                                                                                    
  Returns:    None                                                                                     
  Description: 读FLASH                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void EFLASH_Read(UINT32 *addr,  UINT32 *buf, UINT32 len)
{
	UINT32 i=0;
	
    for(i = 0; i < len; i++)
    {
        buf[i] = addr[i];
    }
}

/*-----------------------------------------------------------------------------
  Function:		EFLASH_EnStdby                                                                                                                                                                            
  input:    opt: ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                     
  Description: enable flash standby                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void EFLASH_EnStdby(UINT8 opt)
{
    if(opt == ENABLE)
	    SGCC_FMC_P->STANDBY_EN |= STANDBY_EN_STANDBY_EN;
    else
        SGCC_FMC_P->STANDBY_EN &= ~STANDBY_EN_STANDBY_EN;
}

/*-----------------------------------------------------------------------------
  Function:		EFLASH_EnDeepPowerDown                                                                                                                                                                            
  input:    opt: ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                     
  Description: enable flash deep power down                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void EFLASH_EnDeepPowerDown(UINT8 opt)
{
    if(opt == ENABLE)
	    SGCC_FMC_P->STANDBY_EN |= STANDBY_EN_DPD_EN;
    else
        SGCC_FMC_P->STANDBY_EN &= ~STANDBY_EN_DPD_EN;
}
