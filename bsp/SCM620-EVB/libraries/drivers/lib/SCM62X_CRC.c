/*<FH+>************************************************************************/
/*                                                                            */
/* ��Ȩ����: ������о΢���ӿƼ����޹�˾                                       */
/*                                                                            */
/* �ļ�����: SCM62X_CRC.c                                                     */
/* ����ժҪ: CRCģ�������������                                              */
/* ����˵��: **                                                               */
/* ��ǰ�汾: V1.0                                                             */
/* ��    ��: SCM                                                              */
/* �������: 2019-03-20                                                       */
/* �޸ļ�¼:                                                                  */
/*    �޸�����          �汾��        �޸���        �޸�����                  */
/* -------------------------------------------------------------------------- */
/*    2019-03-20        V1.0           SCM        �����ļ�                    */
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
  Function:		CRC_Init                                                                                                                                                                         
  input:     	crcInit
                
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: CRC initialization                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CRC_Init(CRC_InitTypeDef crcInit) 
{
    SGCC_CRC_P->CTRL = 0;
	SGCC_CRC_P->CTRL = crcInit.polyType | crcInit.inRev | crcInit.outRev | crcInit.xorOut | crcInit.crcMode;
    SGCC_CRC_P->DATA.DATA_WORD = crcInit.initVal;
}


/*-----------------------------------------------------------------------------
  Function:		CRC_Enable                                                                                                                                                                         
  input:     	opt - ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: CRC enable                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CRC_Enable(UINT8 opt) 
{
	if(opt == ENABLE)
    {
        SGCC_CRC_P->CTRL |= CRC_CTRL_CRCEN_EN;
    }
    else
    {
        SGCC_CRC_P->CTRL = 0;
    }
}

/*-----------------------------------------------------------------------------
  Function:		CRC_GetWord                                                                                                                                                                         
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: get CRC result                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CRC_GetWord(UINT32* buf) 
{
	while(!(SGCC_CRC_P->STAT & CRC_STAT_CRC_RDY));
    *buf = SGCC_CRC_P->RESU;
}
/*-----------------------------------------------------------------------------
  Function:		CRC_GetHalfWord                                                                                                                                                                         
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: get CRC result                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CRC_GetHalfWord(UINT16* buf) 
{
	while(!(SGCC_CRC_P->STAT & CRC_STAT_CRC_RDY));
    *buf = (UINT16)SGCC_CRC_P->RESU;
}

/*-----------------------------------------------------------------------------
  Function:		CRC_GetByte                                                                                                                                                                        
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: get CRC result                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CRC_GetByte(UINT8* buf) 
{
	while(!(SGCC_CRC_P->STAT & CRC_STAT_CRC_RDY));
    *buf = (UINT8)SGCC_CRC_P->RESU;
}

/*-----------------------------------------------------------------------------
  Function:		CRC_InputWord                                                                                                                                                                         
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: CRC                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void CRC_InputWord(UINT32* buf, UINT16 len) 
{
    UINT16 i;
	for(i = 0; i < len; i++)
    {
        while(!(SGCC_CRC_P->STAT & (1 << CRC_STAT_CRC_RDYB)));
        SGCC_CRC_P->DATA.DATA_WORD = buf[i];
    }
}

/*-----------------------------------------------------------------------------
  Function:		CRC_InputHalfWord                                                                                                                                                                         
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: CRC                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void CRC_InputHalfWord(UINT16* buf, UINT16 len) 
{
    UINT16 i;
	for(i = 0; i < len; i++)
    {
        while(!(SGCC_CRC_P->STAT & (1 << CRC_STAT_CRC_RDYB)));
        (SGCC_CRC_P->DATA.DATA_HALFW[0]) = buf[i];
        
    }
}

/*-----------------------------------------------------------------------------
  Function:		CRC_InputByte                                                                                                                                                                        
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: CRC                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void CRC_InputByte(UINT8* buf, UINT16 len) 
{
    UINT16 i;
	for(i = 0; i < len; i++)
    {
        while(!(SGCC_CRC_P->STAT & (1 << CRC_STAT_CRC_RDYB)));
        (SGCC_CRC_P->DATA.DATA_BYTE[0]) = buf[i];
        
    }
}

/*-----------------------------------------------------------------------------
  Function:		CRC_GetVerifyResult                                                                                                                                                                         
  input:     None	
  output:     None                                                                                    
  Returns:    1 - PASS
              0 - FAIL 
  Description: CRC initialization                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
BOOL CRC_GetVerifyResult(void) 
{
	if(SGCC_CRC_P->STAT & (1 << CRC_STAT_CRC_FAILB))
        return 0;
    else
        return 1;
}

