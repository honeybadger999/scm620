/*<FH+>************************************************************************/
/*                                                                            */
/* ��Ȩ����: ������о΢����                                                   */
/*                                                                            */
/* �ļ�����: SCM62X_TIMER.c                                                   */
/* ����ժҪ: TIMERģ�������������                                            */
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
  Function:		TC_RestoreReg                                                                                                                                                                           
  input:     TCx��select which timer to configure     	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: �ָ���TC��ؼĴ���Ϊ�ϵ縴λֵ                                                                                                   	                                   
 *-----------------------------------------------------------------------------*/
void TC_RestoreReg(SGCC_TC_TypeDef *TCx)
{
    TCx->FSR = 0;    
    TCx->CR = 2;
    TCx->CR = 0;        
    TCx->PR = 0;       
    TCx->PC = 0;       
    TCx->MR = 0;       
    TCx->MCR  = 0;      
    TCx->EMR = 0;      
    TCx->EWCR = 0;  
    TCx->CCR = 0;
    TCx->CAPTR = 0;             
    TCx->INTIF = 0;     
    TCx->INTIE = 0;    
   
}
/*-----------------------------------------------------------------------------
  Function:		TC_Enable                                                                                                                                                                           
  input:     	TCx��select which timer 
                opt: ENABLE or DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: enable interrupt                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void TC_Enable(SGCC_TC_TypeDef *TCx, UINT8 opt) 
{
    TCx->CR = TC_TCCR_TMRST;
    
    if(opt == ENABLE)
    {
        TCx->CR = (~TC_TCCR_TMRST) | TC_TCCR_TMEN;
    }
    else
    {
        TCx->CR = ~(TC_TCCR_TMRST | TC_TCCR_TMEN);
    }	
}

/*-----------------------------------------------------------------------------
  Function:		TC_EnableIntr                                                                                                                                                                           
  input:     	TCx��select which timer 
             intrType:  TC_INTR_MR/TC_INTR_TCOV/TC_INTR_CAPRE/TC_INTR_CAPFE   
              opt: ENABLE or DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: enable interrupt                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void TC_EnableIntr(SGCC_TC_TypeDef *TCx, UINT8 intrType, UINT8 opt) 
{
    if(opt == ENABLE)
    {
	    TCx->INTIE |= intrType;
    }
    else
    {
        TCx->INTIE &=  ~intrType;
    }
	 
	
}
/*-----------------------------------------------------------------------------
  Function:		TC_ClearIntr                                                                                                                                                                           
  input:    TCx��select which timer 	
            intrType  TC_INTR_MR/TC_INTR_TCOV/TC_INTR_CAPRE/TC_INTR_CAPFE                         	  
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: clear interrupt                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void TC_ClearIntr(SGCC_TC_TypeDef *TCx, UINT8 intrType) 
{
    TCx->INTIF |=  intrType;	
}

/*-----------------------------------------------------------------------------
  Function:		TC_GetIntrStatus                                                                       
  input:    TCx: select the timer        
            flag:which interrupt to get, can be TC_INTR_MR/TC_INTR_TCOV/TC_INTR_CAPRE/TC_INTR_CAPFE
  Returns:  None                                                                                        
  Description:  get UART status                                                                                         
               	                                  
 *-----------------------------------------------------------------------------*/
UINT8 TC_GetIntrStatus(SGCC_TC_TypeDef *TCx, uint16_t flag)
{
    if(TCx->INTIF & flag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

/*-----------------------------------------------------------------------------
  Function:		TC_Timer16Bit                                                                                                                                                                           
  input:     	TCx��select which timer to work
              prescale--Ԥ��Ƶ
              matchEn--�Ƿ�ʹ��ƥ�䣬��ƥ��ʹ�ܣ�������ƥ��Ĵ�����ʹ��ƥ���жϲ�����ƥ�䷢��ʱ����ߵ͵�ƽ������ʹ��ƥ�䣬��ʹ������ж�
              matchVal--ƥ��Ĵ���        	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: ����TC������16λ��ʱģʽ                                                                                                    	                                   
 *-----------------------------------------------------------------------------*/
void TC_Timer16Bit(SGCC_TC_TypeDef *TCx, BOOL matchEn, TC_16BitTimerCFG tcCfg, BOOL interEn) 
{
   //��ʹ��TC
   	TCx->CR = 0;
   	//tc mode
   	TCx->FSR = 0;
   	//prescale
   	TCx->PR = 	tcCfg.prescale;  
   	if(matchEn)
   	{
   	  	//match value
   	  	TCx->MR = tcCfg.matchVal;
   	    //enable match, ����matchʱTC����  
   	    TCx->MCR = tcCfg.matchCtl | 0x1;
		if(interEn)
		{
   	        //enable match interrupt
   	        TCx->INTIE |= 0x1;
		}
        else
		{
   	        //disable match interrupt
   	        TCx->INTIE &= ~0x1;
		}	  
   	    //out control,����ߵ͵�ƽ
   	    TCx->EMR = tcCfg.outMode ;
		//��������壬80ms���
   	    TCx->EWCR = tcCfg.outPulseW;	  
   	}
   	else
   	{
		if(interEn)
		{
   	        //enable overflow interrupt
   	        TCx->INTIE = 0x2;
		}
		else
		{
   	            //disable overflow interrupt
   	        TCx->INTIE &= ~0x2;
		}	  
   	    	  
   	}
   	    
   	//enable TC
   	TCx->CR = 0x2;
   	TCx->CR = 0x1;
	
}

/*-----------------------------------------------------------------------------
  Function:		TC_Timer32Bit                                                                                                                                                                            
  input:     	TCx--select which timer to work in 32bit mode
              prescale--Ԥ��Ƶ
              matchEn--�Ƿ�ʹ��ƥ�䣬��ƥ��ʹ�ܣ�������ƥ��Ĵ�����ʹ��ƥ���жϲ�����ƥ�䷢��ʱ����ߵ͵�ƽ������ʹ��ƥ�䣬��ʹ������ж�
              matchValL--ƥ��Ĵ�����16bit 
              matchValH--ƥ��Ĵ�����16bit       	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: ����TC������32λ��ʱģʽ                                                                                                    	                                   
 *-----------------------------------------------------------------------------*/

void TC_Timer32Bit(SGCC_TC_TypeDef *TCx, UINT16 prescale, BOOL matchEn, UINT16 matchValL, UINT16 matchValH) 
{
	
	SGCC_TC_TypeDef *TCy =   (SGCC_TC_TypeDef *)((UINT32)TCx + 0x40);
  
   	//��ʹ��TC
   	TCx->CR = 0;
   	TCy->CR = 0;
   	
   	//tc mode
   	TCx->FSR = 0;
   	TCy->FSR = TC_TCFSR_CNTSRC_LOWOV | 0x1;
   		
   	//prescale
   	TCx->PR = 	prescale;  
   	
   	if(matchEn)
   	{
   	    //match value
   	    TCx->MR = matchValL;
   	    TCy->MR = matchValH;
   	  	    
					  
   	    //enable match, ����matchʱTC����  
   	    TCy->MCR = 0x3;
   	    //enable match interrupt
   	    TCy->INTIE = 0x1;
   	    //out control,����ߵ͵�ƽ
   	    TCy->EMR = 0x3;
   	}
   	else
   	{
   	    //enable overflow interrupt
   	    TCy->INTIE = 0x2;
   	}
   	    
   	//enable TC
   	TCy->CR = 0x1;
   
   		
}

/*-----------------------------------------------------------------------------
  Function:		TC_Counter16Bit��                                                                                                                                                                           
  input:     	TCx��select which timer 
              matchEn-�Ƿ�ʹ��ƥ��
              tcCfg--�����ĸ���Ա����ֵ       	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: ����TC������16λ����ģʽ                                                                                                       	                                   
 *-----------------------------------------------------------------------------*/
void TC_Counter16Bit(SGCC_TC_TypeDef *TCx, BOOL matchEn, TC_16BitCNTCFG tcCfg, BOOL interEn) 
{
	UINT32 temp ;

	//counter mode
    temp = tcCfg.cntMode;
    temp |= tcCfg.cntSrc ;
    //��ʹ��TC
   	TCx->CR = 0;
   	//counter mode
   	TCx->FSR = temp;
   	//prescale
	TCx->PR = tcCfg.prescale;
   	if(matchEn)
   	{
   	  	//match value
   	  	TCx->MR = tcCfg.matchVal;
   	    //enable match, ����matchʱTC����  
   	    TCx->MCR = 0x3;
   	    if(interEn)
		{
   	        //enable match interrupt
   	        TCx->INTIE |= 0x1;
		}
		else
		{
   	        //disable match interrupt
   	        TCx->INTIE &= ~0x1;
		}	  
   	    //out control,���80ms������
   	    TCx->EMR = tcCfg.outMode;
   	    TCx->EWCR = 2621;
   	    }
   	    else
   	    {
   	    	if(interEn)
			{
   	            //enable overflow interrupt
   	            TCx->INTIE = 0x2;
			}
			else
			{
   	            //disable overflow interrupt
   	            TCx->INTIE &= ~0x2;
			}	  
   	    }
   	    
   	    //enable TC
   	    TCx->CR = 0x2;
   	    TCx->CR = 0x1;	
   
}

/*-----------------------------------------------------------------------------
  Function:		TC_Counter32Bit��                                                                                                                                                                           
  input:     	TCx��select which timer 
              tcCfg--�����ĸ���Ա����ֵ       	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: ����TC������32λ����ģʽ                                                                                                       	                                   
 *-----------------------------------------------------------------------------*/
void TC_Counter32Bit(SGCC_TC_TypeDef *TCx, TC_32BitCNTCFG tcCfg) 
{
	UINT32 temp ;
    SGCC_TC_TypeDef *TCy =   (SGCC_TC_TypeDef *)((UINT32)TCx + 0x40);
    
	//counter mode
    temp = tcCfg.cntMode;
    temp |= tcCfg.cntSrc ;
  
 
   //��ʹ��TC0/1
   	TCx->CR = 0;
   	TCy->CR = 0;
   	
   	//counter mode
   	TCx->FSR = temp;
   	TCy->FSR =  TC_TCFSR_CNTSRC_LOWOV| 0x1;  //TC1�ļ���ԴΪTC0����TC_TCFSR_CNTSRC_LOWOV
   	
   	//prescale
	TCx->PR = tcCfg.prescale;
   	    
   	//match value
   	TCx->MR = tcCfg.matchValL;
   	TCy->MR = tcCfg.matchValH;
   	
   	//enable match, ����matchʱTC����  
   	TCy->MCR = 0x3;
   	       
   	//enable match interrupt
   	TCy->INTIE |= 0x1;

   	//out control,if������壬������80ms
   	TCy->EMR = tcCfg.outMode;
   	TCy->EWCR = 2621;
    //enable TC0/TC1
   	TCy->CR = 0x2;
   	TCy->CR = 0x1;	
}
/*-----------------------------------------------------------------------------
  Function:		TC_Capture16Bit                                                                                                                                                                         
  input:     	TCx��select which timer 
              tcCfg--capture���� 	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: ����TC�����ڶ�ʱģʽ�Ҳ�׽ģʽʹ��                                                                                                          	                                   
 *-----------------------------------------------------------------------------*/
void TC_Capture16Bit(SGCC_TC_TypeDef *TCx, TC_CAPTCFG tcCfg, BOOL interEn) 
{
    //��ʹ��TC
   	TCx->CR = 0;		
	TCx->CCR = 0x0;
	//clear interrupt
	TCx->INTIF = 0;
			  
			
   	//tc mode & capture source
   	TCx->FSR = tcCfg.capSrc ;
   	//prescale
   	TCx->PR = 	tcCfg.prescale; 
   	  	
			  
   	if(tcCfg.capEdge == TC_CCR_CAPRE)
   	{
   	  	TCx->CCR |= 0x2;
		if(interEn)
		{
   	        //enable capture interrupt
   	        TCx->INTIE |= 0x4;
		}
		else
		{
   	        //disable capture interrupt
   	        TCx->INTIE &= ~0x4;
		}	  
   	  	    
   	}
   	else if(tcCfg.capEdge == TC_CCR_CAPFE) 
   	{
   	  	TCx->CCR |= 0x4;
		if(interEn)
		{
   	        //enable capture interrupt
   	        TCx->INTIE |= 0x8;
		}
		else
		{
   	        //disable capture interrupt
   	        TCx->INTIE &= ~0x8;
		}	  
   	  		  
   	}	
   	else  //�������½��ز�׽
   	{
   	    TCx->CCR |= 0x6;
		if(interEn)
		{
   	        //enable capture interrupt
   	        TCx->INTIE |= 0xc;
		}
		else
		{
   	        //disable capture interrupt
   	        TCx->INTIE &= ~0xc;
		}	  
   	}
   	    
	//ʹ��TC
   	TCx->CR = 0x2;
   	TCx->CR = 1;
			
   	//capture control
   	TCx->CCR |= 0x1;  //enable capture
	
  
  
}

/*-----------------------------------------------------------------------------
  Function:		TC_Capture32Bit                                                                                                                                                                         
  input:     	TCx��select which timer 
              tcCfg--��׽ģʽ���� 	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: ����TC�����ڶ�ʱģʽ�Ҳ�׽ģʽʹ��                                                                                                          	                                   
 *-----------------------------------------------------------------------------*/
void TC_Capture32Bit(SGCC_TC_TypeDef *TCx, TC_CAPTCFG tcCfg) 
{
    SGCC_TC_TypeDef *TCy =   (SGCC_TC_TypeDef *)((UINT32)TCx + 0x40);
    
   	//��ʹ��TC0/1�Ķ�ʱ���ܺ�TC1�Ĳ�׽����
   	TCx->CR = 0;		
   	TCy->CR = 0;		
	TCy->CCR = 0x0;
	//clear interrupt
	TCy->INTIF = 0;
 
   	//tc mode & capture source
   	TCy->FSR = tcCfg.capSrc | TC_TCFSR_CNTSRC_LOWOV;  //TC1�ļ���ԴΪTC0�����;��׽Դ���Բ�������
   	//prescale
   	TCx->PR = 	tcCfg.prescale;   
   	if(tcCfg.capEdge == TC_CCR_CAPRE)
   	{
   	    TCy->CCR |= 0x2;
		//enable capture interrupt
        TCy->INTIE |= 0x4;
    }
   	else if(tcCfg.capEdge == TC_CCR_CAPFE) 
   	{
   		TCy->CCR |= 0x4;
	    //enable capture interrupt
   	    TCy->INTIE |= 0x8;
    }	
   	else  //�������½��ز�׽
   	{
   	   	TCy->CCR |= 0x6;
		//enable capture interrupt
   	    TCy->INTIE |= 0xc;
    }
   	    
	//ʹ��TC
   	TCy->CR = 0x2;
   	TCy->CR = 1;
   	//capture control
   	TCy->CCR |= 0x1;  //enable capture
   	  
}
/*-----------------------------------------------------------------------------
  Function:		Delay                                                                                                                                                                            
  input:     	time����λΪms                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ʹ��tc0��ʱ                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void Delay( UINT16 time )
{
	TC_16BitTimerCFG tcCfg;
	BOOL matchEn;
	
    TC_RestoreReg(SGCC_TIMERC0_P);
	tcCfg.prescale = 999;   //k
	matchEn = 1;
	tcCfg.matchVal = time;
	tcCfg.outMode = 3;
    tcCfg.outPulseW = 0;
	SGCC_TIMERC0_P->INTIF = 0;
    TC_Timer16Bit(SGCC_TIMERC0_P, matchEn, tcCfg, 0) ;
    
	//�ȴ���������ж�
	while(!(SGCC_TIMERC0_P->INTIF & TC_INTR_MR));
	
	//����ж�
    SGCC_TIMERC0_P->INTIF = 0;
	//disable timer
    SGCC_TIMERC0_P->CR = 0;
	
	TC_RestoreReg(SGCC_TIMERC0_P);
}


/*-----------------------------------------------------------------------------
  Function:		DelayUs                                                                                                                                                                            
  input:     	time����λus                  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ʹ��tc1��ʱ                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void DelayUs( UINT16 time )
{
	TC_16BitTimerCFG tcCfg;
	BOOL matchEn;
	
    TC_RestoreReg(SGCC_TIMERC0_P);
    
    matchEn = 1;
	tcCfg.prescale = 39;   //40M/250k
	tcCfg.matchVal = time;
	tcCfg.outMode = 3;
	tcCfg.outPulseW = 0;
	SGCC_TIMERC0_P->INTIF = 0;
    TC_Timer16Bit(SGCC_TIMERC0_P, matchEn, tcCfg, 0) ;
    
	//�ȴ���������ж�
	while(!(SGCC_TIMERC0_P->INTIF & TC_INTR_MR));
	
	//����ж�
    SGCC_TIMERC0_P->INTIF = 0;
	//disable timer
    SGCC_TIMERC0_P->CR = 0;
	
	TC_RestoreReg(SGCC_TIMERC0_P);
}
