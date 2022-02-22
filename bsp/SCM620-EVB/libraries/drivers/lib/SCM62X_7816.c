/*<FH+>************************************************************************/
/*                                                                            */
/* ��Ȩ����: ������о΢����                                                   */
/*                                                                            */
/* �ļ�����: SCM62X_7816.c                                                    */
/* ����ժҪ: 7816ģ�������������                                             */
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
  Function:		U7816_RestoreReg                                                                      
  input:    None                         
  Returns:  None                                                                                        
  Description:  restore 7816 related registers                                                                                                       	                                  
 *-----------------------------------------------------------------------------*/
void U7816_RestoreReg(void)
{
      SGCC_U7816_P->FIFO = 0x1b;
	   __asm("nop");
	  SGCC_U7816_P->FIFO = 0x18;  //reset FIFO
	
	  SGCC_U7816_P->CHNL = 5;
	/*
	   //7816DATA0--GPIO11
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 16);
	 
	   //7816CLK0--GPIO12
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 18);
	 
	   //7816RST0--GPIO13
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 20);
	 
	   //7816DATA1--GPIO14
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 22);	 
	   //7816CLK1--GPIO15
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 24);
	   //7816RST1--GPIO16
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 26);
       */
    SGCC_U7816_P->CHNL = 0;
}

/*-----------------------------------------------------------------------------
  Function:		U7816_ConfigBaudRate                                                                                                                                                                           
  input:     	baudRate  -- baud rate configuration                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: configure U7816_BDR and U7816_DFRM                                                                                                   	                                   
 *-----------------------------------------------------------------------------*/
void U7816_ConfigBaudRate(UINT16 baudRate) 
{
  SGCC_U7816_P->BDR = baudRate;
}

/*-----------------------------------------------------------------------------
  Function:		U7816_ConfigDataFrame                                                                                                                                                                           
  input:     	tmod--U7816_DFRM_TMOD_T1(T=1)/U7816_DFRM_TMOD_T0(T=0)
              diconv--U7816_DFRM_DICONV_INV��������룩/U7816_DFRM_DICONV_NOR��������룩
              par--U7816_DFRM_PAR_ODD/U7816_DFRM_PAR_EVEN
              txegt--extended delay between two characters when transmit
                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: configure U7816_BDR and U7816_DFRM                                                                                                   	                                   
 *-----------------------------------------------------------------------------*/
void U7816_ConfigDataFrame(UINT8 tmod, UINT8 diconv, UINT8 par, UINT8 txegt) 
{
	UINT32 temp = 0;
	
	temp |= tmod  | diconv | par | txegt;
  SGCC_U7816_P->DFRM = temp;
}


/*-----------------------------------------------------------------------------
  Function:		U7816_Init                                                                                                                                                                            
  input:     	baudRate  -- baud rate configuration                  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: configure U7816_BDR and U7816_DFRM                                                                                                   	                                   
 *-----------------------------------------------------------------------------*/
void U7816_Init(UINT16 baudRate) 
{
	
  SGCC_U7816_P->BDR = baudRate;
  //����ʱguard timeΪ2etu��error signal���Ϊ1.5etu��żУ�飬������룬��LSB��T=0��
  //�Զ��ط������շ�����ʱ����
	SGCC_U7816_P->DFRM = 0x140; 
}


/*-----------------------------------------------------------------------------
  Function:		U7816_PrepareRx                                                                                                                                                                            
  input:     	rxLvl -- RXFIFO trigger level: U7816_FIFOCTRL_RTRIG_1 ~ U7816_FIFOCTRL_RTRIG_8
              errWid -- error width, can be U7816_DFRM_ERRWID_1etu, U7816_DFRM_ERRWID_1nhalfetu, U7816_DFRM_ERRWID_2etu
              ch  -- channel 0 or 1
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: prepare to receive                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void U7816_PrepareRx(UINT8 rxLvl, UINT8 errWid, UINT8 ch) 
{
  //���ý�����ֵ
	SGCC_U7816_P->FIFO = rxLvl  ;
	SGCC_U7816_P->DFRM |= errWid ;

	if( ch == 0x00 )
	{
		 SGCC_U7816_P->CHNL = U7816_CHNL_EN | U7816_CHNL_CSEL_0 | U7816_CHNL0_SCLK_EN;	//ѡ��ͨ��0��ʹ��7816ģ�顢��ʱ��
	}
	else
	{
		 SGCC_U7816_P->CHNL = U7816_CHNL_EN | U7816_CHNL_CSEL_1 |  U7816_CHNL1_SCLK_EN;	//ѡ��ͨ��1��ʹ��7816ģ�鼰ʱ��
	}
}

/*-----------------------------------------------------------------------------
  Function:		U7816_GetChars                                                                                                                                                                            
  input:      len	                      	  
  output:     data-- received data                                                                                      
  Returns:    None                                                                                    
  Description: get received data                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void U7816_GetChars(UINT8 *data, UINT16 len)
{
	UINT8 rxTrig;
  UINT8 i;
  UINT8 *rxData = data;
  
  rxTrig = ((SGCC_U7816_P->FIFO & U7816_FIFOCTRL_RTRIG) >> U7816_FIFOCTRL_RTRIGBF) + 1;
  while(len >= rxTrig)  //��Ҫ���յ����ݳ���RXFIFO��ֵ
  {
  	  while(!(SGCC_U7816_P->SR & U7816_SR_RXMTRG)); //���յ������ݴﵽtriggerֵ
  	  
  	  for(i = 0; i < rxTrig; i++)
  	  {
  	  	 *rxData++ = SGCC_U7816_P->DATA;
  	  	 len--;
  	  }
  }
  
	
	for(i = 0; i < len; i++)
	{
		  while((SGCC_U7816_P->SR & U7816_SR_RXEMPT)); //����FIFO�ǿ�
		  *rxData++ = SGCC_U7816_P->DATA;
	}
	
}

/*-----------------------------------------------------------------------------
  Function:		U7816_GetChar                                                                                                                                                                            
  input:     	None                      	  
  output:     data-- received data                                                                                      
  Returns:    1 -- success; 2 -- error; 3 -- timeout                                                                                      
  Description: get 1 byte data  when RXFIFO trigger level is 0, i.e. interrupt when received 1  byte                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
UINT8 U7816_GetChar(UINT8 *data)
{

  TC_16BitTimerCFG tcCfg;
	
	//configure TM01
	//��ʹ��TC
	TC_Enable(SGCC_TIMERA0_P, DISABLE) ;
    
  //����ж�
	TC_ClearIntr(SGCC_TIMERA0_P, TC_INTR_MR) ;
       
  //confiugure timer
  tcCfg.matchCtl = 2;
  tcCfg.outMode = 0;
  tcCfg.outPulseW = 0;
  tcCfg.prescale = 31999;
  tcCfg.matchVal = 3000 * (PCLK/(tcCfg.prescale + 1))  -1;
  
  TC_Timer16Bit(SGCC_TIMERA0_P, 1, tcCfg, 0) ;
	
	//�ж��Ƿ��ڽ���״ֱ̬��������������ж�
	while(!TC_GetIntrStatus(SGCC_TIMERA0_P, TC_INTR_MR))
	{
	    if(SGCC_U7816_P->SR & U7816_SR_RXMTRG) //���յ��������Ѵﵽtriggerֵ
	    {
				  *data = SGCC_U7816_P->DATA;
			    //disable timer
			    TC_Enable(SGCC_TIMERA0_P, DISABLE) ;
			    return 1;
	    }
		if( SGCC_U7816_P->SR & U7816_SR_RXERR ) //���մ���
		{
			    //disable timer
			    TC_Enable(SGCC_TIMERA0_P, DISABLE) ;
				//clear flag
				//SGCC_U7816_P->SR &= ~U7816_SR_RXERR;
				return 2; //error
		}
  }
	
  //disable timer
	TC_Enable(SGCC_TIMERA0_P, DISABLE) ;
	//����ж�
	TC_ClearIntr(SGCC_TIMERA0_P, TC_INTR_MR) ;	
	return 3;
	
	/*
	while( !(SGCC_U7816_P->SR & U7816_SR_RXMTRG) );//���յ��������Ѵﵽtriggerֵ
	*data = SGCC_U7816_P->DATA;
	return 1;*/
}

/*-----------------------------------------------------------------------------
  Function:		U7816_PrepareTx                                                                                                                                                                            
  input:     	rpt -- repeat times
              ch  -- channel 0 or 1
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: prepare to transmit                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void U7816_PrepareTx(UINT8 rpt, UINT8 ch) 
{
  ///�����ط�����
	SGCC_U7816_P->DFRM &= ~(0x7 << U7816_DFRM_RTXBF); 
    SGCC_U7816_P->DFRM |= (rpt << U7816_DFRM_RTXBF);

    if( ch == 0x00 )
	{
		SGCC_U7816_P->CHNL = U7816_CHNL_EN | U7816_CHNL_CSEL_0 |  U7816_CHNL0_SCLK_EN;	//ѡ��ͨ��0��ʹ��7816ģ�顢���ͼ�ʱ��
	}
	else
	{
		SGCC_U7816_P->CHNL = U7816_CHNL_EN | U7816_CHNL_CSEL_1  | U7816_CHNL1_SCLK_EN;	//ѡ��ͨ��1��ʹ��7816ģ�顢���ͼ�ʱ��
	}
}

/*-----------------------------------------------------------------------------
  Function:		U7816_SendByte                                                                                                                                                                            
  input:    data -- data to send     	  
  output:   None                                                                                    
  Returns:    0 -- success; 1 -- error; 2 -- timeout                                                                                     
  Description: send 1 byte data                                                                                                   	                                   
 *-----------------------------------------------------------------------------*/
UINT8  U7816_SendByte(UINT8 data )
{
	UINT16 i;
  
	//����FIFO������������д��һ�ֽ�����
	if(!(SGCC_U7816_P->SR & U7816_SR_TXFULL))
	{
	  SGCC_U7816_P->DATA = data;
	}

  for( i = 0x00; i < 9000*6; i++ ) //36ms
	{
		if(!(SGCC_U7816_P->SR & U7816_SR_TXBUSY))	//TX���
		{
			return 0; //�ɹ�
		}

		if( (SGCC_U7816_P->SR & U7816_SR_TXERR))	//TX error
		{
  		return 1; //error
		}
	}
	return 2;
}



/*-----------------------------------------------------------------------------
  Function:		U7816_EnableIntr                                                                                                                                                                                
  input:    UARTx: select the UART 
            IntMask:      U7816_RXFULL_INTR/U7816_RXMTRG_INTR/U7816_TXERR_INTR/U7816_TXEMPT_INTR                  
  Returns:    None                                                                                      
  Description: U7816 enable INTR                                                                                         
               	                                  
 *-----------------------------------------------------------------------------*/
void U7816_EnableIntr(UINT32 intMask)
{
	SGCC_U7816_P->INTEN |= intMask;
}

/*-----------------------------------------------------------------------------
  Function:		U7816_ClearIntr                                                                                                                                                                                
  input:    intType:      U7816_RXFULL_INTR/U7816_RXMTRG_INTR/U7816_TXERR_INTR/U7816_TXEMPT_INTR                 
  Returns:    None                                                                                      
  Description: U7816 clear INTR                                                                                   
               	                                  
 *-----------------------------------------------------------------------------*/
void U7816_ClearIntr(UINT32 intType)
{
	SGCC_U7816_P->INTFLAG &= ~intType;
}



