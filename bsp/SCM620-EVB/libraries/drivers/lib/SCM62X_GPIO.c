/*<FH+>************************************************************************/
/*                                                                            */
/* ��Ȩ����: ������о΢����                                                   */
/*                                                                            */
/* �ļ�����: SCM62X_GPIO.c                                                    */
/* ����ժҪ: GPIOģ�������������                                             */
/* ����˵��: **                                                               */
/* ��ǰ�汾: V1.0                                                             */
/* ��    ��: SCM                                                              */
/* �������: 2019-03-20                                                       */
/* �޸ļ�¼:                                                                  */
/*    �޸�����          �汾��        �޸���        �޸�����                  */
/* -------------------------------------------------------------------------- */
/*    2019-03-20        V1.0          SCM        �����ļ�                     */
/*<FH->************************************************************************/


/******************************************************************************/
/*               #include������Ϊ��׼��ͷ�ļ����Ǳ�׼��ͷ�ļ���               */
/******************************************************************************/
#include "SCM62X.h"

/******************************************************************************/
/*                                �ⲿ���ö���                                */
/******************************************************************************/


/******************************************************************************/
/*                                  ȫ�ֱ���                                  */
/******************************************************************************/



/*-----------------------------------------------------------------------------
  Function:		GPIO_RestoreVal                                                                                                                                                                            
  input:      None          	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ����GPIO�ܽŵķ�������or���                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_RestoreVal() 
{
    SGCC_GPIO_P->DR = 0;
	  SGCC_GPIO_P->DIR = 0;
	  SGCC_GPIO_P->INTER = 0;
	  SGCC_GPIO_P->INTMR = 0;

	  SGCC_GPIO_P->DBSYR = 0;
	  SGCC_GPIO_P->INTCR = 0xffffffff;
	  SGCC_GPIO_P->PULLEN = 0;
}

/*-----------------------------------------------------------------------------
  Function:		GPIO_ConfigDir                                                                                                                                                                            
  input:    portNum: port number 
            dir��
            	GPIO_INPUT:����
              GPIO_OUTPUT:���                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ����GPIO�ܽŵķ�������or���                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_ConfigDir(UINT8 portNum, UINT8 dir) 
{
	  UINT32 temp;
	  SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
		regnum = portNum / 32;
		 
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
	 	temp = GPIO_P->DIR;
	  
	if(regnum)
		portNum = portNum%32;

		temp &= ~(1 << portNum);
	  temp |= (dir << portNum);

	
	  GPIO_P->DIR = temp ;
	  
}


/*-----------------------------------------------------------------------------
  Function:		GPIO_WrDataOut                                                                                                                                                                            
  input:    portNum: port number 
            level��GPIO_HIGH/GPIO_LOW            	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ʹGPIO����ߵ͵�ƽ                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_WrDataOut(UINT8 portNum, UINT8 level) 
{
	  UINT32 temp;
	  SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
		regnum = portNum / 32;
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	  temp = GPIO_P->DR;
	
		if(regnum)
			portNum = portNum%32;
		
		temp &= ~(1 << portNum);
		temp |= (level << portNum);

    GPIO_P->DR = temp ;
	
}

/*-----------------------------------------------------------------------------
  Function:		GPIO_RdDataIn                                                                                                                                                                            
  input:    portNum: port number     	                   	  
  output:     None                                                                                    
  Returns:    gpio level                                                                                      
  Description: ����GPIO port�ĵ�ƽ                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
UINT8 GPIO_RdDataIn(UINT8 portNum) 
{
	  UINT32 temp;
	  SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = portNum / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			portNum = portNum%32;
	
	  temp = GPIO_P->ETPR;
	
	  temp = (temp >> portNum) & 0x1;
	  
	  return((UINT8)temp);
}	  


/*-----------------------------------------------------------------------------
  Function:		GPIO_EnIntr                                                                                                                                                                            
  input:    portNum: port number 
            opt: ENABLE/DISABLE          	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ʹ�ܻ�ʹ���ж�                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_EnIntr(UINT8 portNum, UINT8 opt) 
{
	  SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = portNum / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			portNum = portNum%32;
		
    if(opt == ENABLE)
    {
        GPIO_P->INTER |= (1 << portNum);
    }
    else
    {
        GPIO_P->INTER &= ~(1 << portNum);
    }

}	  
	
/*-----------------------------------------------------------------------------
  Function:		GPIO_MaskIntr                                                                                                                                                                            
  input:    portNum: port number 
            intrEn: GPIO_INTR_MASK/GPIO_INTR_UNMASK          	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ���λ�������ж�                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_MaskIntr(UINT8 portNum, UINT8 intrMask) 
{
	  UINT32 temp;
	   SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = portNum / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			portNum = portNum%32;
		
	  	temp = GPIO_P->INTMR;
	  	temp &= ~(1 << portNum);
	    temp |= (intrMask << portNum);
	    GPIO_P->INTMR= temp ;

}	  

/*-----------------------------------------------------------------------------
  Function:		GPIO_ClrIntr                                                                                                                                                                            
  input:    portNum: port number           	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ����ж�                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_ClrIntr(UINT8 portNum) 
{
		SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = portNum / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			portNum = portNum%32;

	    GPIO_P->INTCR = (1 << portNum) ;

}	  

/*-----------------------------------------------------------------------------
  Function:		GPIO_ConfigIntrMode                                                                                                                                                                           
  input:    portNum: port number 
            trigMode: 
            GPIO_INTR_RISINGEDGE  
            GPIO_INTR_FALLINGEDGE  
            GPIO_INTR_HIGHLVEL   
            GPIO_INTR_LOWLEVEL  
            GPIO_INTR_BOTHEDGE       	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: �ж�ģʽ����                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_ConfigIntrMode(UINT8 portNum, UINT8 trigMode) 
{
	  UINT32 temp;
	  UINT8 div;
    UINT8 offSet;
	
	
		SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = portNum / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			portNum = portNum%32;
	
	
    
    div = portNum / 8;
    offSet = div * 8 + 4;
    
    temp = GPIO_P->DBSYR;
    
    switch(div)
    {
    	case 0:
    		temp &= ~GPIO_DBSYR_INTER_MODE1;
    		break;
    	case 1:
    		temp &= ~GPIO_DBSYR_INTER_MODE2;
    		break;	
    	case 2:
    		temp &= ~GPIO_DBSYR_INTER_MODE3;
    		break;	
    	case 3:
    		temp &= ~GPIO_DBSYR_INTER_MODE4;
    		break;
    	default:
    		break;
    }
    
    temp |= ((UINT32)trigMode << offSet);
	GPIO_P->DBSYR = temp;
	     	    
	  
}	  

/*-----------------------------------------------------------------------------
  Function:		GPIO_RdIntrMode                                                                                                                                                                           
  input:    portNum: port number     	                   	  
  output:     None                                                                                    
  Returns:    gpio level                                                                                      
  Description: return GPIO port's trigger mode                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
UINT8 GPIO_RdIntrMode(UINT8 portNum) 
{
	UINT32 temp;
	UINT8 div;
    UINT8 offSet;
	
	SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = portNum / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			portNum = portNum%32;
	
	
    
    div = portNum / 8;
    offSet = div * 8 + 4;
	
    temp = GPIO_P->DBSYR;
	temp = (temp >> offSet) & 0x7;
	  
	  
	return((UINT8)temp);
}	  

/*-----------------------------------------------------------------------------
  Function:		GPIO_ConfigDebounce                                                                                                                                                                            
  input:     portNum- port number 
             debounceMode- debounce mode
             debounceCycle- debounce cycle       	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ȥë��ʹ��                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_ConfigDebounce(UINT8 portNum, UINT8 debounceMode, UINT8 debounceCycle) 
{
	  UINT32 temp;

	  UINT8 div;
    UINT8 offSet;
    
		
	SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = portNum / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			portNum = portNum%32;
	
	
    div = portNum / 8;
    offSet = div * 8 ;
    
    temp = GPIO_P->DBSYR;
    
    switch(div)
    {
    	case 0:
    		temp &= ~(GPIO_DBSYR_DEBOUNCE1 | GPIO_DBSYR_DEBOUNCE_CYCLE1);
    		break;
    	case 1:
    		temp &= ~(GPIO_DBSYR_DEBOUNCE2 | GPIO_DBSYR_DEBOUNCE_CYCLE2);
    		break;	
    	case 2:
    		temp &= ~(GPIO_DBSYR_DEBOUNCE3 | GPIO_DBSYR_DEBOUNCE_CYCLE3);
    		break;	
    	case 3:
    		temp &= ~(GPIO_DBSYR_DEBOUNCE4 | GPIO_DBSYR_DEBOUNCE_CYCLE4);
    		break;
    	default:
    		break;
    }
    
    temp |= ((UINT32)debounceMode << offSet);
    temp |= ((UINT32)debounceCycle << (offSet + 2));
    
	  GPIO_P->DBSYR = temp;
	  
}


/*-----------------------------------------------------------------------------
  Function:		GPIO_EnSyn                                                                                                                                                                            
  input:     portNum: port number 
             synEn:enable or disable synchronization        	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ͬ��ʹ��                                                                                          	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_EnSyn(UINT8 portNum, UINT8 synEn) 
{
	  UINT32 temp;
	  UINT8 offSet = (portNum / 8) * 8 + 7;
	
		SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = portNum / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			portNum = portNum%32;
	

	  temp = GPIO_P->DBSYR;
	  
	  if(synEn)
	      temp |= ((UINT32)1 << offSet);
	  else
	  	  temp &= ~((UINT32)1 << offSet);
	  	  
	  GPIO_P->DBSYR = temp; 
}
