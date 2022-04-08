/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-19     SummerGift   first version
 * 2018-12-25     zylx         fix some bugs
 */

#include "board.h"
#include <netif/ethernetif.h>
#include "lwipopts.h"
#include "drv_eth.h"
#include "misc.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

#ifdef RT_USING_MAC

#ifdef MAC_DEBUG
    #define MAC_TRACE         rt_kprintf
#else
    #define MAC_TRACE(...)
#endif /* #ifdef FLASH_DEBUG */

//#define ETH_RX_DUMP
//#define ETH_TX_DUMP


/** @defgroup ETH_Private_Defines
  * @{
  */
/* Global pointers on Tx and Rx descriptor used to track transmit and receive descriptors */
ETH_DMADESCTypeDef  *DMATxDescToSet;
ETH_DMADESCTypeDef  *DMARxDescToGet;
ETH_DMADESCTypeDef  *DMAPTPTxDescToSet;
ETH_DMADESCTypeDef  *DMAPTPRxDescToGet;


struct netif  DP83848_netif;    //定义一个全局的网络接口，注册网卡函数要用到

/*
* Emac driver uses CubeMX tool to generate emac and phy's configuration,
* the configuration files can be found in CubeMX_Config floder.
*/

#define LOG_TAG             "drv.emac"
#include <drv_log.h>

#define MAX_ADDR_LEN 6

struct rt_scm62x_eth
{
    /* inherit from ethernet device */
    struct eth_device parent;

    /* interface address info, hw address */
    rt_uint8_t  dev_addr[MAX_ADDR_LEN];
    /* ETH_Speed */
    uint32_t    ETH_Speed;
    /* ETH_Duplex_Mode */
    uint32_t    ETH_Mode;
};

static struct rt_scm62x_eth scm62x_eth_device;

#if defined(ETH_RX_DUMP) || defined(ETH_TX_DUMP)
#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
static void dump_hex(const rt_uint8_t *ptr, rt_size_t buflen)
{
    unsigned char *buf = (unsigned char *)ptr;
    int i, j;

    for (i = 0; i < buflen; i += 16)
    {
        rt_kprintf("%08X: ", i);

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                rt_kprintf("%02X ", buf[i + j]);
            else
                rt_kprintf("   ");

        rt_kprintf(" ");

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                rt_kprintf("%c", __is_print(buf[i + j]) ? buf[i + j] : '.');

        rt_kprintf("\n");
    }
}
#endif


ETH_TypeDef *ETH;

void ETH_DeInit(ETH_TypeDef *ETH)
{
    if(ETH == SGCC_GMAC0_P)
    {
        // RMU_SoftRstSingMod(GMAC0) ;
        SGCC_RESET_P->SOFT_RST0 = SOFT_RST0_GMAC0;

        while(SGCC_RESET_P->SOFT_RST0 & SOFT_RST0_GMAC0);
    }
    else
    {
        // RMU_SoftRstSingMod(GMAC1) ;
        SGCC_RESET_P->SOFT_RST0 = SOFT_RST0_GMAC1;

        while(SGCC_RESET_P->SOFT_RST0 & SOFT_RST0_GMAC1);
    }
}

void ETH_SoftwareReset(ETH_TypeDef *ETH)
{
    /* Set the SWR bit: resets all MAC subsystem internal registers and logic */
    /* After reset all the registers holds their respective reset values */
    ETH->DMABMR |= ETH_DMABMR_SR;
}

FlagStatus ETH_GetSoftwareResetStatus(ETH_TypeDef *ETH)
{
    FlagStatus bitstatus = RESET;

    if((ETH->DMABMR & ETH_DMABMR_SR) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

void ETH_StructInit(ETH_InitTypeDef* ETH_InitStruct)
{
    /* ETH_InitStruct members default value */
    /*------------------------   MAC   -----------------------------------*/
    ETH_InitStruct->ETH_CrcStip = ETH_MACCR_CST_DISABLE;
    ETH_InitStruct->ETH_AutoNegotiation = ETH_AutoNegotiation_Disable;
    ETH_InitStruct->ETH_AutoNegotiation = ETH_AutoNegotiation_Disable;
    ETH_InitStruct->ETH_Watchdog = ETH_Watchdog_Enable;
    ETH_InitStruct->ETH_Jabber = ETH_Jabber_Enable;
    ETH_InitStruct->ETH_InterFrameGap = ETH_InterFrameGap_96Bit;
    ETH_InitStruct->ETH_CarrierSense = ETH_CarrierSense_Enable;
    ETH_InitStruct->ETH_Speed = ETH_Speed_10M;
    ETH_InitStruct->ETH_ReceiveOwn = ETH_ReceiveOwn_Enable;
    ETH_InitStruct->ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;//ETH_Mode_HalfDuplex;
    ETH_InitStruct->ETH_ChecksumOffload = ETH_ChecksumOffload_Disable;
    ETH_InitStruct->ETH_RetryTransmission = ETH_RetryTransmission_Enable;
    ETH_InitStruct->ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStruct->ETH_BackOffLimit = ETH_BackOffLimit_10;
    ETH_InitStruct->ETH_DeferralCheck = ETH_DeferralCheck_Disable;
    ETH_InitStruct->ETH_ReceiveAll = ETH_ReceiveAll_Disable;
    ETH_InitStruct->ETH_SourceAddrFilter = ETH_SourceAddrFilter_Disable;
    ETH_InitStruct->ETH_PassControlFrames = ETH_PassControlFrames_BlockAll;    ///////////////
    ETH_InitStruct->ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Disable;
    ETH_InitStruct->ETH_DestinationAddrFilter = ETH_DestinationAddrFilter_Normal;
    ETH_InitStruct->ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    ETH_InitStruct->ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStruct->ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
    ETH_InitStruct->ETH_HashTableHigh = 0x0;
    ETH_InitStruct->ETH_HashTableLow = 0x0;
    ETH_InitStruct->ETH_PauseTime = 0x0;
    ETH_InitStruct->ETH_ZeroQuantaPause = ETH_ZeroQuantaPause_Disable;
    ETH_InitStruct->ETH_PauseLowThreshold = ETH_PauseLowThreshold_Minus4;
    ETH_InitStruct->ETH_UnicastPauseFrameDetect = ETH_UnicastPauseFrameDetect_Disable;
    ETH_InitStruct->ETH_ReceiveFlowControl = ETH_ReceiveFlowControl_Disable;
    ETH_InitStruct->ETH_TransmitFlowControl = ETH_TransmitFlowControl_Disable;
    ETH_InitStruct->ETH_VLANTagComparison = ETH_VLANTagComparison_16Bit;
    ETH_InitStruct->ETH_VLANTagIdentifier = 0x0;
    /*------------------------   DMA   -----------------------------------*/
    ETH_InitStruct->ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Disable;
    ETH_InitStruct->ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
    ETH_InitStruct->ETH_FlushReceivedFrame = ETH_FlushReceivedFrame_Disable;
    ETH_InitStruct->ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;
    ETH_InitStruct->ETH_TransmitThresholdControl = ETH_TransmitThresholdControl_64Bytes;
    ETH_InitStruct->ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
    ETH_InitStruct->ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
    ETH_InitStruct->ETH_ReceiveThresholdControl = ETH_ReceiveThresholdControl_64Bytes;
    ETH_InitStruct->ETH_SecondFrameOperate = ETH_SecondFrameOperate_Disable;
    ETH_InitStruct->ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
    ETH_InitStruct->ETH_FixedBurst = ETH_FixedBurst_Disable;
    ETH_InitStruct->ETH_RxDMABurstLength = ETH_RxDMABurstLength_1Beat;
    ETH_InitStruct->ETH_TxDMABurstLength = ETH_TxDMABurstLength_1Beat;
    ETH_InitStruct->ETH_DescriptorSkipLength = 0x0;
    ETH_InitStruct->ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_1_1;
}

static void ETH_Delay(__IO uint32_t nCount)
{
    __IO uint32_t index = 0;

    for(index = nCount; index != 0; index--)
    {
    }
}
uint32_t ETH_WritePHYRegister(ETH_TypeDef *ETH, uint16_t PHYAddress, uint16_t PHYReg, uint16_t PHYValue)
{
    uint32_t tmpreg = 0;
    __IO uint32_t timeout = 0;
    /* Check the parameters */
    assert_param(IS_ETH_PHY_ADDRESS(PHYAddress));
    assert_param(IS_ETH_PHY_REG(PHYReg));

    /* Get the ETHERNET MACMIIAR value */
    tmpreg = ETH->MACMIIAR;
    /* Keep only the CSR Clock Range CR[2:0] bits value */
    tmpreg &= ~MACMIIAR_CR_MASK;
    /* Prepare the MII register address value */
    tmpreg |= (((uint32_t)PHYAddress << 11) & ETH_MACMIIAR_PA); /* Set the PHY device address */
    tmpreg |= (((uint32_t)PHYReg << 6) & ETH_MACMIIAR_MR);   /* Set the PHY register address */
    tmpreg |= ETH_MACMIIAR_MW;                               /* Set the write mode */
    tmpreg |= ETH_MACMIIAR_MB;                               /* Set the MII Busy bit */
    /* Give the value to the MII data register */
    ETH->MACMIIDR = PHYValue;
    /* Write the result value into the MII Address register */
    ETH->MACMIIAR = tmpreg;

    /* Check for the Busy flag */
    do
    {
        timeout++;
        tmpreg = ETH->MACMIIAR;
    }
    while ((tmpreg & ETH_MACMIIAR_MB) && (timeout < (uint32_t)PHY_WRITE_TO));

    /* Return ERROR in case of timeout */
    if(timeout == PHY_WRITE_TO)
    {
        return ETH_ERROR;
    }

    /* Return SUCCESS */
    return ETH_SUCCESS;
}

uint16_t ETH_ReadPHYRegister(ETH_TypeDef *ETH, uint16_t PHYAddress, uint16_t PHYReg)
{
    uint32_t tmpreg = 0;
    __IO uint32_t timeout = 0;
    /* Check the parameters */
    assert_param(IS_ETH_PHY_ADDRESS(PHYAddress));
    assert_param(IS_ETH_PHY_REG(PHYReg));

    /* Get the ETHERNET MACMIIAR value */
    tmpreg = ETH->MACMIIAR;
    /* Keep only the CSR Clock Range CR[2:0] bits value */
    tmpreg &= ~MACMIIAR_CR_MASK;
    /* Prepare the MII address register value */
    tmpreg |= (((uint32_t)PHYAddress << 11) & ETH_MACMIIAR_PA); /* Set the PHY device address */
    tmpreg |= (((uint32_t)PHYReg << 6) & ETH_MACMIIAR_MR);   /* Set the PHY register address */
    tmpreg &= ~ETH_MACMIIAR_MW;                              /* Set the read mode */
    tmpreg |= ETH_MACMIIAR_MB;                               /* Set the MII Busy bit */
    /* Write the result value into the MII Address register */
    ETH->MACMIIAR = tmpreg;

    /* Check for the Busy flag */
    do
    {
        timeout++;
        tmpreg = ETH->MACMIIAR;
    }
    while ((tmpreg & ETH_MACMIIAR_MB) && (timeout < (uint32_t)PHY_READ_TO));

    /* Return ERROR in case of timeout */
    if(timeout == PHY_READ_TO)
    {
        return (uint16_t)ETH_ERROR;
    }

    /* Return data register value */
    return (uint16_t)(ETH->MACMIIDR);
}

uint32_t ETH_Init(ETH_TypeDef *ETH, ETH_InitTypeDef* ETH_InitStruct, uint16_t PHYAddress)
{
    uint32_t RegValue = 0, tmpreg = 0;
    __IO uint32_t i = 0;
    //RCC_ClocksTypeDef  rcc_clocks;
    uint32_t hclk = 46000000;
    __IO uint32_t timeout = 0;
    /* Check the parameters */
    /* MAC --------------------------*/
    assert_param(IS_ETH_AUTONEGOTIATION(ETH_InitStruct->ETH_AutoNegotiation));
    assert_param(IS_ETH_WATCHDOG(ETH_InitStruct->ETH_Watchdog));
    assert_param(IS_ETH_JABBER(ETH_InitStruct->ETH_Jabber));
    assert_param(IS_ETH_INTER_FRAME_GAP(ETH_InitStruct->ETH_InterFrameGap));
    assert_param(IS_ETH_CARRIER_SENSE(ETH_InitStruct->ETH_CarrierSense));
    assert_param(IS_ETH_SPEED(ETH_InitStruct->ETH_Speed));
    assert_param(IS_ETH_RECEIVE_OWN(ETH_InitStruct->ETH_ReceiveOwn));
    assert_param(IS_ETH_LOOPBACK_MODE(ETH_InitStruct->ETH_LoopbackMode));
    assert_param(IS_ETH_DUPLEX_MODE(ETH_InitStruct->ETH_Mode));
    assert_param(IS_ETH_CHECKSUM_OFFLOAD(ETH_InitStruct->ETH_ChecksumOffload));
    assert_param(IS_ETH_RETRY_TRANSMISSION(ETH_InitStruct->ETH_RetryTransmission));
    assert_param(IS_ETH_AUTOMATIC_PADCRC_STRIP(ETH_InitStruct->ETH_AutomaticPadCRCStrip));
    assert_param(IS_ETH_BACKOFF_LIMIT(ETH_InitStruct->ETH_BackOffLimit));
    assert_param(IS_ETH_DEFERRAL_CHECK(ETH_InitStruct->ETH_DeferralCheck));
    assert_param(IS_ETH_RECEIVE_ALL(ETH_InitStruct->ETH_ReceiveAll));
    assert_param(IS_ETH_SOURCE_ADDR_FILTER(ETH_InitStruct->ETH_SourceAddrFilter));
    assert_param(IS_ETH_CONTROL_FRAMES(ETH_InitStruct->ETH_PassControlFrames));
    assert_param(IS_ETH_BROADCAST_FRAMES_RECEPTION(ETH_InitStruct->ETH_BroadcastFramesReception));
    assert_param(IS_ETH_DESTINATION_ADDR_FILTER(ETH_InitStruct->ETH_DestinationAddrFilter));
    assert_param(IS_ETH_PROMISCUOUS_MODE(ETH_InitStruct->ETH_PromiscuousMode));
    assert_param(IS_ETH_MULTICAST_FRAMES_FILTER(ETH_InitStruct->ETH_MulticastFramesFilter));
    assert_param(IS_ETH_UNICAST_FRAMES_FILTER(ETH_InitStruct->ETH_UnicastFramesFilter));
    assert_param(IS_ETH_PAUSE_TIME(ETH_InitStruct->ETH_PauseTime));
    assert_param(IS_ETH_ZEROQUANTA_PAUSE(ETH_InitStruct->ETH_ZeroQuantaPause));
    assert_param(IS_ETH_PAUSE_LOW_THRESHOLD(ETH_InitStruct->ETH_PauseLowThreshold));
    assert_param(IS_ETH_UNICAST_PAUSE_FRAME_DETECT(ETH_InitStruct->ETH_UnicastPauseFrameDetect));
    assert_param(IS_ETH_RECEIVE_FLOWCONTROL(ETH_InitStruct->ETH_ReceiveFlowControl));
    assert_param(IS_ETH_TRANSMIT_FLOWCONTROL(ETH_InitStruct->ETH_TransmitFlowControl));
    assert_param(IS_ETH_VLAN_TAG_COMPARISON(ETH_InitStruct->ETH_VLANTagComparison));
    assert_param(IS_ETH_VLAN_TAG_IDENTIFIER(ETH_InitStruct->ETH_VLANTagIdentifier));
    /* DMA --------------------------*/
    assert_param(IS_ETH_DROP_TCPIP_CHECKSUM_FRAME(ETH_InitStruct->ETH_DropTCPIPChecksumErrorFrame));
    assert_param(IS_ETH_RECEIVE_STORE_FORWARD(ETH_InitStruct->ETH_ReceiveStoreForward));
    assert_param(IS_ETH_FLUSH_RECEIVE_FRAME(ETH_InitStruct->ETH_FlushReceivedFrame));
    assert_param(IS_ETH_TRANSMIT_STORE_FORWARD(ETH_InitStruct->ETH_TransmitStoreForward));
    assert_param(IS_ETH_TRANSMIT_THRESHOLD_CONTROL(ETH_InitStruct->ETH_TransmitThresholdControl));
    assert_param(IS_ETH_FORWARD_ERROR_FRAMES(ETH_InitStruct->ETH_ForwardErrorFrames));
    assert_param(IS_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES(ETH_InitStruct->ETH_ForwardUndersizedGoodFrames));
    assert_param(IS_ETH_RECEIVE_THRESHOLD_CONTROL(ETH_InitStruct->ETH_ReceiveThresholdControl));
    assert_param(IS_ETH_SECOND_FRAME_OPERATE(ETH_InitStruct->ETH_SecondFrameOperate));
    assert_param(IS_ETH_ADDRESS_ALIGNED_BEATS(ETH_InitStruct->ETH_AddressAlignedBeats));
    assert_param(IS_ETH_FIXED_BURST(ETH_InitStruct->ETH_FixedBurst));
    assert_param(IS_ETH_RXDMA_BURST_LENGTH(ETH_InitStruct->ETH_RxDMABurstLength));
    assert_param(IS_ETH_TXDMA_BURST_LENGTH(ETH_InitStruct->ETH_TxDMABurstLength));
    assert_param(IS_ETH_DMA_DESC_SKIP_LENGTH(ETH_InitStruct->ETH_DescriptorSkipLength));
    assert_param(IS_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX(ETH_InitStruct->ETH_DMAArbitration));
    /*-------------------------------- MAC Config ------------------------------*/
    /*---------------------- ETHERNET MACMIIAR Configuration -------------------*/

    /* Get the ETHERNET MACMIIAR value */
    tmpreg = ETH->MACMIIAR;
    /* Clear CSR Clock Range CR[2:0] bits */
    tmpreg &= MACMIIAR_CR_MASK;

    /* Get hclk frequency value
    RCC_GetClocksFreq(&rcc_clocks);
    hclk = rcc_clocks.HCLK_Frequency;*/
    /* Set CR bits depending on hclk value */
    if((hclk >= 20000000) && (hclk < 35000000))
    {
        /* CSR Clock Range between 20-35 MHz */
        tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div16;
    }
    else if((hclk >= 35000000) && (hclk < 60000000))
    {
        /* CSR Clock Range between 35-60 MHz */
        tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div26;
    }
    else /* ((hclk >= 60000000)&&(hclk <= 72000000)) */
    {
        /* CSR Clock Range between 60-72 MHz */
        tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div42;
    }

    /* Write to ETHERNET MAC MIIAR: Configure the ETHERNET CSR Clock Range */
    ETH->MACMIIAR = (uint32_t)tmpreg;


    RegValue = ETH_ReadPHYRegister(ETH, PHYAddress, PHY_BCR);

    /*-------------------- PHY initialization and configuration ----------------*/
    /* Put the PHY in reset mode */
    if(!(ETH_WritePHYRegister(ETH, PHYAddress, PHY_BCR, PHY_Reset)))
    {
        /* Return ERROR in case of write timeout */
        return ETH_ERROR;
    }

    /* Delay to assure PHY reset */
    _eth_delay_(PHY_ResetDelay);
    RegValue = ETH_ReadPHYRegister(ETH, PHYAddress, PHY_BCR);

    if(ETH_InitStruct->ETH_AutoNegotiation != ETH_AutoNegotiation_Disable)
    {
        /* We wait for linked satus... */
        do
        {
            timeout++;
        }
        while (!(ETH_ReadPHYRegister(ETH, PHYAddress, PHY_BSR) & PHY_Linked_Status) && (timeout < PHY_READ_TO));

        /* Return ERROR in case of timeout */
        if(timeout == PHY_READ_TO)
        {
            return ETH_ERROR;
        }

        /* Reset Timeout counter */
        timeout = 0;

        /* Enable Auto-Negotiation */
        if(!(ETH_WritePHYRegister(ETH, PHYAddress, PHY_BCR, PHY_AutoNegotiation)))
        {
            /* Return ERROR in case of write timeout */
            return ETH_ERROR;
        }

        /* Wait until the autonegotiation will be completed */
        do
        {
            timeout++;
        }
        while (!(ETH_ReadPHYRegister(ETH, PHYAddress, PHY_BSR) & PHY_AutoNego_Complete) && (timeout < (uint32_t)PHY_READ_TO));

        /* Return ERROR in case of timeout */
        if(timeout == PHY_READ_TO)
        {
            return ETH_ERROR;
        }

        /* Reset Timeout counter */
        timeout = 0;


        /* Read the result of the autonegotiation */
        RegValue = ETH_ReadPHYRegister(ETH, PHYAddress, PHY_SR);

        /* Configure the MAC with the Duplex Mode fixed by the autonegotiation process */
        if((RegValue & PHY_Duplex_Status) != (uint32_t)RESET)
        {
            /* Set Ethernet duplex mode to FullDuplex following the autonegotiation */
            ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;

        }
        else
        {
            /* Set Ethernet duplex mode to HalfDuplex following the autonegotiation */
            ETH_InitStruct->ETH_Mode = ETH_Mode_HalfDuplex;
        }

        /* Configure the MAC with the speed fixed by the autonegotiation process */
        if(RegValue & PHY_Speed_Status)
        {
            /* Set Ethernet speed to 10M following the autonegotiation */
            ETH_InitStruct->ETH_Speed = ETH_Speed_10M;
        }
        else
        {
            /* Set Ethernet speed to 100M following the autonegotiation */
            ETH_InitStruct->ETH_Speed = ETH_Speed_100M;
        }
    }
    else
    {
        if(!ETH_WritePHYRegister(ETH, PHYAddress, PHY_BCR, ((uint16_t)(ETH_InitStruct->ETH_Mode >> 3) |
                                 (uint16_t)(ETH_InitStruct->ETH_Speed >> 1))))
        {
            /* Return ERROR in case of write timeout */
            return ETH_ERROR;
        }

        /* Delay to assure PHY configuration */
        _eth_delay_(PHY_ConfigDelay);

    }

    /* write and Read RMII and bypass register
    ETH_WritePHYRegister(ETH, PHYAddress, 0x17, 0x22);
    RegValue = ETH_ReadPHYRegister(ETH, PHYAddress, 0x17);
    */
    /*------------------------ ETHERNET MACCR Configuration --------------------*/
    /* Get the ETHERNET MACCR value */
    tmpreg = ETH->MACCR;
    /* Clear WD, PCE, PS, TE and RE bits */
    tmpreg &= MACCR_CLEAR_MASK;
    /* Set the WD bit according to ETH_Watchdog value */
    /* Set the JD: bit according to ETH_Jabber value */
    /* Set the IFG bit according to ETH_InterFrameGap value */
    /* Set the DCRS bit according to ETH_CarrierSense value */
    /* Set the FES bit according to ETH_Speed value */
    /* Set the DO bit according to ETH_ReceiveOwn value */
    /* Set the LM bit according to ETH_LoopbackMode value */
    /* Set the DM bit according to ETH_Mode value */
    /* Set the IPC bit according to ETH_ChecksumOffload value */
    /* Set the DR bit according to ETH_RetryTransmission value */
    /* Set the ACS bit according to ETH_AutomaticPadCRCStrip value */
    /* Set the BL bit according to ETH_BackOffLimit value */
    /* Set the DC bit according to ETH_DeferralCheck value */
    tmpreg |= (uint32_t)(ETH_InitStruct->ETH_CrcStip |
                         ETH_InitStruct->ETH_Watchdog |
                         ETH_InitStruct->ETH_Jabber |
                         ETH_InitStruct->ETH_InterFrameGap |
                         ETH_InitStruct->ETH_CarrierSense |
                         ETH_InitStruct->ETH_Speed |
                         ETH_InitStruct->ETH_ReceiveOwn |
                         ETH_InitStruct->ETH_LoopbackMode |
                         ETH_InitStruct->ETH_Mode |
                         ETH_InitStruct->ETH_ChecksumOffload |
                         ETH_InitStruct->ETH_RetryTransmission |
                         ETH_InitStruct->ETH_AutomaticPadCRCStrip |
                         ETH_InitStruct->ETH_BackOffLimit |
                         ETH_InitStruct->ETH_DeferralCheck);
    /* Write to ETHERNET MACCR */
    ETH->MACCR = (uint32_t)tmpreg;

    /*----------------------- ETHERNET MACFFR Configuration --------------------*/
    /* Set the RA bit according to ETH_ReceiveAll value */
    /* Set the SAF and SAIF bits according to ETH_SourceAddrFilter value */
    /* Set the PCF bit according to ETH_PassControlFrames value */
    /* Set the DBF bit according to ETH_BroadcastFramesReception value */
    /* Set the DAIF bit according to ETH_DestinationAddrFilter value */
    /* Set the PR bit according to ETH_PromiscuousMode value */
    /* Set the PM, HMC and HPF bits according to ETH_MulticastFramesFilter value */
    /* Set the HUC and HPF bits according to ETH_UnicastFramesFilter value */
    /* Write to ETHERNET MACFFR */
    ETH->MACFFR = (uint32_t)(ETH_InitStruct->ETH_ReceiveAll |
                             ETH_InitStruct->ETH_SourceAddrFilter |
                             ETH_InitStruct->ETH_PassControlFrames |
                             ETH_InitStruct->ETH_BroadcastFramesReception |
                             ETH_InitStruct->ETH_DestinationAddrFilter |
                             ETH_InitStruct->ETH_PromiscuousMode |
                             ETH_InitStruct->ETH_MulticastFramesFilter |
                             ETH_InitStruct->ETH_UnicastFramesFilter);
    /*--------------- ETHERNET MACHTHR and MACHTLR Configuration ---------------*/
    /* Write to ETHERNET MACHTHR */
    ETH->MACHTHR = (uint32_t)ETH_InitStruct->ETH_HashTableHigh;
    /* Write to ETHERNET MACHTLR */
    ETH->MACHTLR = (uint32_t)ETH_InitStruct->ETH_HashTableLow;
    /*----------------------- ETHERNET MACFCR Configuration --------------------*/
    /* Get the ETHERNET MACFCR value */
    tmpreg = ETH->MACFCR;
    /* Clear xx bits */
    tmpreg &= MACFCR_CLEAR_MASK;

    /* Set the PT bit according to ETH_PauseTime value */
    /* Set the DZPQ bit according to ETH_ZeroQuantaPause value */
    /* Set the PLT bit according to ETH_PauseLowThreshold value */
    /* Set the UP bit according to ETH_UnicastPauseFrameDetect value */
    /* Set the RFE bit according to ETH_ReceiveFlowControl value */
    /* Set the TFE bit according to ETH_TransmitFlowControl value */
    tmpreg |= (uint32_t)((ETH_InitStruct->ETH_PauseTime << 16) |
                         ETH_InitStruct->ETH_ZeroQuantaPause |
                         ETH_InitStruct->ETH_PauseLowThreshold |
                         ETH_InitStruct->ETH_UnicastPauseFrameDetect |
                         ETH_InitStruct->ETH_ReceiveFlowControl |
                         ETH_InitStruct->ETH_TransmitFlowControl);
    /* Write to ETHERNET MACFCR */
    ETH->MACFCR = (uint32_t)tmpreg;
    /*----------------------- ETHERNET MACVLANTR Configuration -----------------*/
    /* Set the ETV bit according to ETH_VLANTagComparison value */
    /* Set the VL bit according to ETH_VLANTagIdentifier value */
    ETH->MACVLANTR = (uint32_t)(ETH_InitStruct->ETH_VLANTagComparison |
                                ETH_InitStruct->ETH_VLANTagIdentifier);

    /*-------------------------------- DMA Config ------------------------------*/
    /*----------------------- ETHERNET DMAOMR Configuration --------------------*/
    /* Get the ETHERNET DMAOMR value */
    tmpreg = ETH->DMAOMR;
    /* Clear xx bits */
    tmpreg &= DMAOMR_CLEAR_MASK;

    /* Set the DT bit according to ETH_DropTCPIPChecksumErrorFrame value */
    /* Set the RSF bit according to ETH_ReceiveStoreForward value */
    /* Set the DFF bit according to ETH_FlushReceivedFrame value */
    /* Set the TSF bit according to ETH_TransmitStoreForward value */
    /* Set the TTC bit according to ETH_TransmitThresholdControl value */
    /* Set the FEF bit according to ETH_ForwardErrorFrames value */
    /* Set the FUF bit according to ETH_ForwardUndersizedGoodFrames value */
    /* Set the RTC bit according to ETH_ReceiveThresholdControl value */
    /* Set the OSF bit according to ETH_SecondFrameOperate value */
    tmpreg |= (uint32_t)(ETH_InitStruct->ETH_DropTCPIPChecksumErrorFrame |
                         ETH_InitStruct->ETH_ReceiveStoreForward |
                         ETH_InitStruct->ETH_FlushReceivedFrame |
                         ETH_InitStruct->ETH_TransmitStoreForward |
                         ETH_InitStruct->ETH_TransmitThresholdControl |
                         ETH_InitStruct->ETH_ForwardErrorFrames |
                         ETH_InitStruct->ETH_ForwardUndersizedGoodFrames |
                         ETH_InitStruct->ETH_ReceiveThresholdControl |
                         ETH_InitStruct->ETH_SecondFrameOperate);
    /* Write to ETHERNET DMAOMR */
    ETH->DMAOMR = (uint32_t)tmpreg;

    /*----------------------- ETHERNET DMABMR Configuration --------------------*/
    /* Set the AAL bit according to ETH_AddressAlignedBeats value */
    /* Set the FB bit according to ETH_FixedBurst value */
    /* Set the RPBL and 4*PBL bits according to ETH_RxDMABurstLength value */
    /* Set the PBL and 4*PBL bits according to ETH_TxDMABurstLength value */
    /* Set the DSL bit according to ETH_DesciptorSkipLength value */
    /* Set the PR and DA bits according to ETH_DMAArbitration value */
    ETH->DMABMR = (uint32_t)(ETH_InitStruct->ETH_AddressAlignedBeats |
                             ETH_InitStruct->ETH_FixedBurst |
                             ETH_InitStruct->ETH_RxDMABurstLength | /* !! if 4xPBL is selected for Tx or Rx it is applied for the other */
                             ETH_InitStruct->ETH_TxDMABurstLength |
                             (ETH_InitStruct->ETH_DescriptorSkipLength << 2) |
                             ETH_InitStruct->ETH_DMAArbitration |
                             ETH_DMABMR_USP); /* Enable use of separate PBL for Rx and Tx */
    /* Return Ethernet configuration success */
    return ETH_SUCCESS;
}


void ETH_DMAITConfig(ETH_TypeDef *ETH, uint32_t ETH_DMA_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ETH_DMA_IT(ETH_DMA_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected ETHERNET DMA interrupts */
        ETH->DMAIER |= ETH_DMA_IT;
    }
    else
    {
        /* Disable the selected ETHERNET DMA interrupts */
        ETH->DMAIER &= (~(uint32_t)ETH_DMA_IT);
    }
}



ETH_InitTypeDef ETH_InitStructure;  //库文件，初始化以太网用到的结构体

/* EMAC initialization function */
static rt_err_t rt_scm62x_eth_init(rt_device_t dev)
{
    #ifdef RT_USING_MAC_MII
    SCU_ConfigGmacMode(SYS_CTRL_GMAC_MODE_MII) ;//该函数设置以太网接口为MII/RMII  SYS_CTRL_GMAC_MODE_RMII/SYS_CTRL_GMAC_MODE_MII

    if(ETH == SGCC_GMAC0_P)
    {
        MII0_InitPad();
    }
    else if(ETH == SGCC_GMAC1_P)
    {
        MII1_InitPad();
    }

    IO_ConfigPullDown(100, DISABLE);
    MCO_InitPad();
    #else
    SCU_ConfigGmacMode(SYS_CTRL_GMAC_MODE_RMII) ;//该函数设置以太网接口为MII/RMII  SYS_CTRL_GMAC_MODE_RMII/SYS_CTRL_GMAC_MODE_MII

    if(ETH == SGCC_GMAC0_P)
    {
        ////ff  RMII0_InitPad();
		MII0_InitPad();
    }
    else if(ETH == SGCC_GMAC1_P)
    {
        RMII1_InitPad();
    }

    #endif
//		IO_ConfigPullDown(PIN100,DISABLE);
//		MCO_InitPad();
//		IO_ConfigDS(PIN100,IO_DS_3);
//		CLK_ConfigMacDivCntMax(SYSCLK_CTRL2_MAC_DIV_CNT_MAX_(5));

    return RT_EOK;
}

static rt_err_t rt_scm62x_eth_open(rt_device_t dev, rt_uint16_t oflag)
{
    MAC_TRACE("emac open\n");
    return RT_EOK;
}

static rt_err_t rt_scm62x_eth_close(rt_device_t dev)
{
    MAC_TRACE("emac close\n");
    return RT_EOK;
}

static rt_size_t rt_scm62x_eth_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    MAC_TRACE("emac read\n");
    rt_set_errno(-RT_ENOSYS);
    return 0;
}

static rt_size_t rt_scm62x_eth_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    MAC_TRACE("emac write\n");
    rt_set_errno(-RT_ENOSYS);
    return 0;
}

static rt_err_t rt_scm62x_eth_control(rt_device_t dev, int cmd, void *args)
{
    switch (cmd)
    {
        case NIOCTL_GADDR:

            /* get mac address */
            if (args) rt_memcpy(args, scm62x_eth_device.dev_addr, 6);
            else return -RT_ERROR;

            break;

        default :
            break;
    }

    return RT_EOK;
}

//获取发送DMA描述符下的缓冲区
u32_t ETH_GetCurrentTxBuffer(void)
{
    return (DMATxDescToSet->Buffer1Addr);   //得到DMA描述符内Buffer1Addr地址。
}

//网卡发送数据函数
u32_t ETH_TxPkt_ChainMode(u16 FrameLength)
{
    if((DMATxDescToSet->Status & ETH_DMATxDesc_OWN) != (u32)RESET)//如果DMA占用描述符成立
    {
        return ETH_ERROR;//返回错误代码
    }

    //如果if不成立，表示CPU占用描述符
    DMATxDescToSet->ControlBufferSize |= ETH_DMATxDesc_IC; //zhangyx

    DMATxDescToSet->ControlBufferSize &= ~ETH_DMATxDesc_TBS1;
    DMATxDescToSet->ControlBufferSize |= (FrameLength & ETH_DMATxDesc_TBS1);//设置发送帧长度

    DMATxDescToSet->ControlBufferSize |= ETH_DMATxDesc_LS | ETH_DMATxDesc_FS;//ETH_DMATxDesc_LS和ETH_DMATxDesc_FS置1，表示帧中存放了，第一个和最后一个分块

    DMATxDescToSet->Status |= ETH_DMATxDesc_OWN;//把描述符给DMA使用


    if ((ETH->DMASR & ETH_DMASR_TBUS) != (u32)RESET)//如果发送缓存不可用，if成立
    {
        ETH->DMASR = ETH_DMASR_TBUS;//清除发送缓存不可用标志
        ETH->DMATPDR = 0;//写ETH->DMATPDR寄存器，以求回复发送流程
    }

    DMATxDescToSet = (ETH_DMADESCTypeDef*) (DMATxDescToSet->Buffer2NextDescAddr);//将当前发送DMA描述符指针，指向下一个发送DMA链表中的DMA描述符


    return ETH_SUCCESS;   //返回成功代码
}

//发送数据函数
//看一看简单的框架和DMA描述符的结构
//整理思路如下
//要发送的数据存放在最为参数传进来的pubf下
//DMA发送描述符内有指向缓冲器的指针，而且我们也设置了缓冲区
//我们首先要得到描述符的DMA缓冲区指针，所以我们要实现一个ETH_GetCurrentTxBuffer函数
//接下来我们将pbuf的数据拷贝到缓冲区
//根据使用的网卡，写一个网卡发送数据的函数ETH_TxPkt_ChainMode
//这几个函数ST官方都给了基于DP83848的例程
static err_t low_level_output(struct pbuf *p)
{
    struct pbuf *q; //顶一个pbuf缓冲区，暂存拷贝中间数据
    int l = 0;      //长度
    u8 *buffer =  (u8 *)ETH_GetCurrentTxBuffer();//获取当前DMA内缓冲区指针，将要发送的数据，拷贝到缓冲区

    for(q = p; q != NULL; q = q->next)   //利用for循环，拷贝数据
    {
        memcpy((u8_t*)&buffer[l], q->payload, q->len);
        l = l + q->len;
    }

    ETH_TxPkt_ChainMode(l);//发送数据

    #ifdef ETH_TX_DUMP

    if(p->tot_len > 0)
    {
        MAC_TRACE("TXD:%d\n", p->tot_len);
        dump_hex(buffer, p->tot_len);
    }

    #endif

    return ERR_OK;
}

/* ethernet device interface */
/* transmit data*/
rt_err_t rt_scm62x_eth_tx(rt_device_t dev, struct pbuf *p)
{
    rt_err_t ret = RT_ERROR;

    ret = low_level_output(p);

    return ret;
}

/**
  * @brief  Returns the specified DMA Rx Desc frame length.
  * @param  DMARxDesc: pointer on a DMA Rx descriptor
  * @retval The Rx descriptor received frame length.
  */
uint32_t ETH_GetDMARxDescFrameLength(ETH_DMADESCTypeDef *DMARxDesc)
{
    /* Return the Receive descriptor frame length */
    return ((DMARxDesc->Status & ETH_DMARxDesc_FL) >> ETH_DMARXDESC_FRAME_LENGTHSHIFT);
}



/**
  * @brief  Get the size of received the received packet.
  * @param  None
  * @retval framelength: received packet size
  */
uint32_t ETH_GetRxPktSize(void)
{
    uint32_t frameLength = 0;

    if(((DMARxDescToGet->Status & ETH_DMARxDesc_OWN) == (uint32_t)RESET) &&
            ((DMARxDescToGet->Status & ETH_DMARxDesc_ES) == (uint32_t)RESET) &&
            ((DMARxDescToGet->Status & ETH_DMARxDesc_LS) != (uint32_t)RESET) &&
            ((DMARxDescToGet->Status & ETH_DMARxDesc_FS) != (uint32_t)RESET))
    {
        /* Get the size of the packet: including 4 bytes of the CRC */
        frameLength = ETH_GetDMARxDescFrameLength(DMARxDescToGet);
    }

    /* Return Frame Length */
    return frameLength;
}

static struct pbuf * low_level_input(ETH_TypeDef *ETH);
/* receive data*/
struct pbuf *rt_scm62x_eth_rx(rt_device_t dev)
{
    struct pbuf *p = NULL;

    if(ETH_GetRxPktSize() != 0)
    {
        p = low_level_input(ETH);//调用low_level_input函数接收数据
    }

    return p;
}




//网卡接收数据函数
FrameTypeDef ETH_RxPkt_ChainMode(void)
{
    u32 framelength = 0;             //变量待用
    FrameTypeDef frame = {0, 0};     //帧结构待用

    if((DMARxDescToGet->Status & ETH_DMARxDesc_OWN) != (u32)RESET)//如果DMA占用描述符成立
    {
        frame.length = ETH_ERROR;   //存放错误代码

        if ((ETH->DMASR & ETH_DMASR_RBUS) != (u32)RESET)  //如果发送缓存不可用，if成立
        {
            ETH->DMASR = ETH_DMASR_RBUS; //清除接收缓冲区不可用标志
            ETH->DMARPDR = 0;//通过写ETH->DMARPDR寄存器，恢复DMA接收
        }

        return frame; //返回帧结构
    }

    //如果上步if不成立，标志描述符由CPU占用
    //又要进行3个判断
    //ETH_DMARxDesc_ES判断接收中是否出错，成立表示没有错误发生
    //ETH_DMARxDesc_LS判断是否到了最后一个缓冲区
    //ETH_DMARxDesc_FS判断是否包含了帧的第一个缓冲区
    if(((DMARxDescToGet->Status & ETH_DMARxDesc_ES) == (u32)RESET) &&
            ((DMARxDescToGet->Status & ETH_DMARxDesc_LS) != (u32)RESET) &&
            ((DMARxDescToGet->Status & ETH_DMARxDesc_FS) != (u32)RESET))
    {
        //都成立的话，得到帧长度值，
        //DMA接收描述符RDES0软件寄存器位16-位29存放帧长度值
        //右移16位，然后还要减去4个自己的CRC校验
        framelength = ((DMARxDescToGet->Status & ETH_DMARxDesc_FL) >> ETH_DMARxDesc_FrameLengthShift) - 4;

        frame.buffer = DMARxDescToGet->Buffer1Addr;	//得到接收描述符下Buffer1Addr地址，它指向了数据缓冲区
    }
    else//如果上步if不成立
    {
        framelength = ETH_ERROR;//记录错误代码
    }

    frame.length = framelength; //将帧长度值，记录在frame结构体中的length成员


    frame.descriptor = DMARxDescToGet;//frame结构体中的descriptor成员指向当前的DMA接收描述符

    DMARxDescToGet = (ETH_DMADESCTypeDef*) (DMARxDescToGet->Buffer2NextDescAddr);//将当前接收DMA描述符指针，指向下一个接收DMA链表中的DMA描述符

    return (frame);  //返回帧结构
}

//接收数据函数
//看一看简单的框架和DMA描述符的结构
//整理思路如下
//当网卡接收到数据，会存放在接收缓冲区，接收DMA描述符下有指向其的指针
//我们还要实现一个网卡接收数据的函数ETH_TxPkt_ChainMode，同发送一样ST提供了例程
//得到缓冲区的数据后，我们要将其拷贝到pbuf结构中，供LWip使用
//所以我们最后将数据拷贝到pbuf后，将它作为函数返回值，返回
static struct pbuf * low_level_input(ETH_TypeDef *ETH)
{
    struct pbuf *p, *q; //p要返回的数据，q拷贝数据时用于暂存数据
    u16_t len;          //保存接收到数据帧的长度
    int l = 0;          //长度，for时暂存中间值
    FrameTypeDef frame; //接受侦
    u8 *buffer;         //接收到数据的地址

    p = NULL; //p向指向空，待用
    frame = ETH_RxPkt_ChainMode();//接收数据帧

    len = frame.length;//将数据帧长度存放在len内待用

    buffer = (u8 *)frame.buffer; //得到数据区地址

    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);//内存池分配空间

    if (p != NULL)//分配成功
    {
        for (q = p; q != NULL; q = q->next)//利用for循环拷贝数据
        {
            memcpy((u8_t*)q->payload, (u8_t*)&buffer[l], q->len);
            l = l + q->len;
        }
    }

    frame.descriptor->Status = ETH_DMARxDesc_OWN; //设置DMA占用描述符

    if ((ETH->DMASR & ETH_DMASR_RBUS) != (u32)RESET)  //通过判断ETH->DMASR寄存器位7，判断接收缓冲区可不可用
    {
        //接收缓冲区不可用，if成立
        ETH->DMASR = ETH_DMASR_RBUS; //清除接收缓冲区不可用标志
        ETH->DMARPDR = 0;//通过写ETH->DMARPDR寄存器，恢复DMA接收
    }

    if(p->tot_len > 0)
    {
//	MAC_TRACE("RXD:%d\n",p->tot_len);
    }
    else
    {
        return NULL;
    }

    #ifdef ETH_RX_DUMP

    if(p->tot_len > 0)
    {
        MAC_TRACE("RXD:%d\n", p->tot_len);
        dump_hex(buffer, p->tot_len);
    }

    #endif

    return p;//返回数据
}


/**
  * @brief  Clears the ETHERNET抯 DMA IT pending bit.
  * @param  ETH_DMA_IT: specifies the interrupt pending bit to clear.
  *   This parameter can be any combination of the following values:
  *     @arg ETH_DMA_IT_NIS : Normal interrupt summary
  *     @arg ETH_DMA_IT_AIS : Abnormal interrupt summary
  *     @arg ETH_DMA_IT_ER  : Early receive interrupt
  *     @arg ETH_DMA_IT_FBE : Fatal bus error interrupt
  *     @arg ETH_DMA_IT_ETI : Early transmit interrupt
  *     @arg ETH_DMA_IT_RWT : Receive watchdog timeout interrupt
  *     @arg ETH_DMA_IT_RPS : Receive process stopped interrupt
  *     @arg ETH_DMA_IT_RBU : Receive buffer unavailable interrupt
  *     @arg ETH_DMA_IT_R   : Receive interrupt
  *     @arg ETH_DMA_IT_TU  : Transmit Underflow interrupt
  *     @arg ETH_DMA_IT_RO  : Receive Overflow interrupt
  *     @arg ETH_DMA_IT_TJT : Transmit jabber timeout interrupt
  *     @arg ETH_DMA_IT_TBU : Transmit buffer unavailable interrupt
  *     @arg ETH_DMA_IT_TPS : Transmit process stopped interrupt
  *     @arg ETH_DMA_IT_T   : Transmit interrupt
  * @retval None
  */
void ETH_DMAClearITPendingBit(ETH_TypeDef *ETH, uint32_t ETH_DMA_IT)
{
    /* Check the parameters */
    assert_param(IS_ETH_DMA_IT(ETH_DMA_IT));

    /* Clear the selected ETHERNET DMA IT */
    ETH->DMASR = (uint32_t) ETH_DMA_IT;
}

ITStatus ETH_GetDMAITStatus(ETH_TypeDef *ETH, uint32_t ETH_DMA_IT)
{
    ITStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_ETH_DMA_GET_IT(ETH_DMA_IT));

    if ((ETH->DMASR & ETH_DMA_IT) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

void GMAC0_IRQHandler()
{
    rt_interrupt_enter();
//	MAC_TRACE("INT\n");
//	if(ETH_GetDMAITStatus(ETH,ETH_DMA_IT_R) == SET)
//	{
//		MAC_TRACE("RX\n");
    eth_device_ready(&(scm62x_eth_device.parent));
//	}
    ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_R);     //清除DMA中断标志位
    ETH_DMAClearITPendingBit(SGCC_GMAC0_P, ETH_DMA_IT_NIS);   //清除DMA接收中断标志位
    rt_interrupt_leave();
}

void NVIC_Configuration(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;            //库文件内，NVIC结构体

    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);     //库函数，设置向量表的位置和偏移，将向量表设置在FLASH区，无偏移。

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //库函数，设置优先级分组：先占优先级和从优先级

    //NVIC_PriorityGroup_2：先占优先级2位 从优先级2位
    if(ETH == SGCC_GMAC0_P)
        NVIC_InitStructure.NVIC_IRQChannel = GMAC0_IRQn;           //设置中断通道=以太网中断
    else
        NVIC_InitStructure.NVIC_IRQChannel = GMAC1_IRQn;           //设置中断通道=以太网中断

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//设置先占优先级=2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //设置从优先级=0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //使能中断
    NVIC_Init(&NVIC_InitStructure);                          //库函数，根据NVIC_InitStructure结构体内的参数初始化NVIC寄存器
}

/**
  * @brief  Initializes the DMA Tx descriptors in chain mode.
  * @param  DMATxDescTab: Pointer on the first Tx desc list
  * @param  TxBuff: Pointer on the first TxBuffer list
  * @param  TxBuffCount: Number of the used Tx desc in the list
  * @retval None
  */
void ETH_DMATxDescChainInit(ETH_TypeDef *ETH, ETH_DMADESCTypeDef *DMATxDescTab, uint8_t* TxBuff, uint32_t TxBuffCount)
{
    uint32_t i = 0;
    ETH_DMADESCTypeDef *DMATxDesc;

    /* Set the DMATxDescToSet pointer with the first one of the DMATxDescTab list */
    DMATxDescToSet = DMATxDescTab;

    /* Fill each DMATxDesc descriptor with the right values */
    for(i = 0; i < TxBuffCount; i++)
    {
        /* Get the pointer on the ith member of the Tx Desc list */
        DMATxDesc = DMATxDescTab + i;


        /* Set Second Address Chained bit */
        DMATxDesc->ControlBufferSize = ETH_DMATxDesc_TCH;

        /* Set Buffer1 address pointer */
        DMATxDesc->Buffer1Addr = (uint32_t)(&TxBuff[i * ETH_MAX_PACKET_SIZE]);

        /* Initialize the next descriptor with the Next Desciptor Polling Enable */
        if(i < (TxBuffCount - 1))
        {
            /* Set next descriptor address register with next descriptor base address */
            DMATxDesc->Buffer2NextDescAddr = (uint32_t)(DMATxDescTab + i + 1);
        }
        else
        {
            /* For last descriptor, set next descriptor address register equal to the first descriptor base address */
            DMATxDesc->Buffer2NextDescAddr = (uint32_t) DMATxDescTab;
        }
    }

    /* Set Transmit Desciptor List Address Register */
    ETH->DMATDLAR = (uint32_t) DMATxDescTab;
}

/**
  * @brief  Initializes the DMA Rx descriptors in chain mode.
  * @param  DMARxDescTab: Pointer on the first Rx desc list
  * @param  RxBuff: Pointer on the first RxBuffer list
  * @param  RxBuffCount: Number of the used Rx desc in the list
  * @retval None
  */
void ETH_DMARxDescChainInit(ETH_TypeDef *ETH, ETH_DMADESCTypeDef *DMARxDescTab, uint8_t *RxBuff, uint32_t RxBuffCount)
{
    uint32_t i = 0;
    ETH_DMADESCTypeDef *DMARxDesc;

    /* Set the DMARxDescToGet pointer with the first one of the DMARxDescTab list */
    DMARxDescToGet = DMARxDescTab;

    /* Fill each DMARxDesc descriptor with the right values */
    for(i = 0; i < RxBuffCount; i++)
    {
        /* Get the pointer on the ith member of the Rx Desc list */
        DMARxDesc = DMARxDescTab + i;
        /* Set Own bit of the Rx descriptor Status */
        DMARxDesc->Status = ETH_DMARxDesc_OWN;

        /* Set Buffer1 size and Second Address Chained bit */
        DMARxDesc->ControlBufferSize = ETH_DMARxDesc_RCH | (uint32_t)ETH_MAX_PACKET_SIZE;
        /* Set Buffer1 address pointer */
        DMARxDesc->Buffer1Addr = (uint32_t)(&RxBuff[i * ETH_MAX_PACKET_SIZE]);

        /* Initialize the next descriptor with the Next Desciptor Polling Enable */
        if(i < (RxBuffCount - 1))
        {
            /* Set next descriptor address register with next descriptor base address */
            DMARxDesc->Buffer2NextDescAddr = (uint32_t)(DMARxDescTab + i + 1);
        }
        else
        {
            /* For last descriptor, set next descriptor address register equal to the first descriptor base address */
            DMARxDesc->Buffer2NextDescAddr = (uint32_t)(DMARxDescTab);
        }
    }

    /* Set Receive Desciptor List Address Register */
    ETH->DMARDLAR = (uint32_t) DMARxDescTab;
}

/**
  * @brief  Enables or disables the specified DMA Rx Desc receive interrupt.
  * @param  DMARxDesc: Pointer on a Rx desc
  * @param  NewState: new state of the specified DMA Rx Desc interrupt.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ETH_DMARxDescReceiveITConfig(ETH_DMADESCTypeDef *DMARxDesc, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the DMA Rx Desc receive interrupt */
        DMARxDesc->ControlBufferSize &= (~(uint32_t)ETH_DMARxDesc_DIC);
    }
    else
    {
        /* Disable the DMA Rx Desc receive interrupt */
        DMARxDesc->ControlBufferSize |= ETH_DMARxDesc_DIC;
    }
}

/**
  * @brief  Selects the specified ETHERNET DMA Tx Desc Checksum Insertion.
  * @param  DMATxDesc: pointer on a DMA Tx descriptor
  * @param  DMATxDesc_Checksum: specifies is the DMA Tx desc checksum insertion.
  *   This parameter can be one of the following values:
  *     @arg ETH_DMATxDesc_ChecksumByPass : Checksum bypass
  *     @arg ETH_DMATxDesc_ChecksumIPV4Header : IPv4 header checksum
  *     @arg ETH_DMATxDesc_ChecksumTCPUDPICMPSegment : TCP/UDP/ICMP checksum. Pseudo header checksum is assumed to be present
  *     @arg ETH_DMATxDesc_ChecksumTCPUDPICMPFull : TCP/UDP/ICMP checksum fully in hardware including pseudo header
  * @retval None
  */
void ETH_DMATxDescChecksumInsertionConfig(ETH_DMADESCTypeDef *DMATxDesc, uint32_t DMATxDesc_Checksum)
{
    /* Check the parameters */
    assert_param(IS_ETH_DMA_TXDESC_CHECKSUM(DMATxDesc_Checksum));

    /* Set the selected DMA Tx desc checksum insertion control */
    DMATxDesc->ControlBufferSize |= DMATxDesc_Checksum;
}

/*---------------------------------  MAC  ------------------------------------*/
/**
  * @brief  Enables or disables the MAC transmission.
  * @param  NewState: new state of the MAC transmission.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ETH_MACTransmissionCmd(ETH_TypeDef *ETH, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the MAC transmission */
        ETH->MACCR |= ETH_MACCR_TE;
    }
    else
    {
        /* Disable the MAC transmission */
        ETH->MACCR &= ~ETH_MACCR_TE;
    }
}

/**
  * @brief  Clears the ETHERNET transmit FIFO.
  * @param  None
  * @retval None
  */
void ETH_FlushTransmitFIFO(ETH_TypeDef *ETH)
{
    /* Set the Flush Transmit FIFO bit */
    ETH->DMAOMR |= ETH_DMAOMR_FTF;
}

/**
  * @brief  Enables or disables the MAC reception.
  * @param  NewState: new state of the MAC reception.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ETH_MACReceptionCmd(ETH_TypeDef *ETH, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the MAC reception */
        ETH->MACCR |= ETH_MACCR_RE;
    }
    else
    {
        /* Disable the MAC reception */
        ETH->MACCR &= ~ETH_MACCR_RE;
    }
}

/**
  * @brief  Enables or disables the DMA transmission.
  * @param  NewState: new state of the DMA transmission.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ETH_DMATransmissionCmd(ETH_TypeDef *ETH, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the DMA transmission */
        ETH->DMAOMR |= ETH_DMAOMR_ST;
    }
    else
    {
        /* Disable the DMA transmission */
        ETH->DMAOMR &= ~ETH_DMAOMR_ST;
    }
}

/**
  * @brief  Enables or disables the DMA reception.
  * @param  NewState: new state of the DMA reception.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ETH_DMAReceptionCmd(ETH_TypeDef *ETH, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the DMA reception */
        ETH->DMAOMR |= ETH_DMAOMR_SR;
    }
    else
    {
        /* Disable the DMA reception */
        ETH->DMAOMR &= ~ETH_DMAOMR_SR;
    }
}

/**
  * @brief  Enables ENET MAC and DMA reception/transmission
  * @param  None
  * @retval None
  */
void ETH_Start(ETH_TypeDef *ETH)
{
    /* Enable transmit state machine of the MAC for transmission on the MII */
    ETH_MACTransmissionCmd(ETH, ENABLE);
    /* Flush Transmit FIFO */
    ETH_FlushTransmitFIFO(ETH);
    /* Enable receive state machine of the MAC for reception from the MII */
    ETH_MACReceptionCmd(ETH, ENABLE);

    /* Start DMA transmission */
    ETH_DMATransmissionCmd(ETH, ENABLE);
    /* Start DMA reception */
    ETH_DMAReceptionCmd(ETH, ENABLE);
}

uint8_t Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE], Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];//发送和接收缓冲区
ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB];

//初始化函数
static void low_level_init(ETH_TypeDef *ETH)
{
    uint8_t i;

    //接下来我们要初始化发送和接收DMA描述符链表
    //107VCT6采用链式结构
    //我们要先创建DMA描述符数组
    //DMA描述符内包含了一个指向接收和发送缓冲区的指针，我们还要创建接收和发送缓冲区，两个数组
    ETH_DMATxDescChainInit(ETH, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);//初始化发送DMA描述符链表
    ETH_DMARxDescChainInit(ETH, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);//初始化接收DMA描述符链表

    //开启DMA描述符接收中断
    for(i = 0; i < ETH_RXBUFNB; i++)
    {
        ETH_DMARxDescReceiveITConfig(&DMARxDscrTab[i], ENABLE);
    }

    #if  !CHECKSUM_GEN_ICMP    //判断是否开启硬件校验,关闭软件校验

    //开启发送帧校验
    for(i = 0; i < ETH_TXBUFNB; i++)
    {
        ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
    }

    #endif

    ETH_Start(ETH);//开启MAC和DMA
}

static uint8_t phy_speed = 0;
#define PHY_LINK_MASK       (1<<0)
static void phy_monitor_thread_entry(void *parameter)
{
    uint8_t phy_addr = 0xFF;
    uint8_t phy_speed_new = 0;
    rt_uint32_t status = 0;
    uint8_t detected_count = 0;

    while(phy_addr == 0xFF)
    {
        /* phy search */
        rt_uint32_t i, temp;

        for (i = 0; i <= 0x1F; i++)
        {
            temp = ETH_ReadPHYRegister(ETH, i, PHY_ID1_REG);

            if (temp != 0xFFFF && temp != 0x00)
            {
                phy_addr = i;
                break;
            }
        }

        detected_count++;
        rt_thread_mdelay(1000);

        if (detected_count > 10)
        {
            MAC_TRACE("No PHY device was detected, please check hardware!\n");
        }
    }

    MAC_TRACE("Found a phy, address:0x%02X\n", phy_addr);
    NVIC_Configuration();
    /* RESET PHY */
    MAC_TRACE("RESET PHY!\n");

    ETH_DeInit(ETH);       //复位以太网外设寄存器到默认值，先使能，在禁止，已恢复默认值

    ETH_SoftwareReset(ETH);//软件复位，通过设置ETH->DMABMR寄存器的位0实现

    while (ETH_GetSoftwareResetStatus(ETH) == SET);//等待软复位成功

    ETH_StructInit(&ETH_InitStructure);//初始化结构体ETH_InitStructure各个成员为默认值

    //ETH_InitStructure结构体内成员较多，填充外默认值后，修改一些关键成员，以适应当前的需要
    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;   			//开启网络自适应功能
    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;					//关闭反馈
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable; 		//关闭重传功能
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable; 	//关闭自动去除PDA/CRC功能
    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;						//关闭接收所有的帧
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;//允许接收所有广播帧
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;			//关闭混合模式的地址过滤
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;//对于组播地址使用完美地址过滤
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;	//对单播地址使用完美地址过滤

    #if  !CHECKSUM_GEN_ICMP    //判断是否开启硬件校验,关闭软件校验
    ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable; 			//开启ipv4和TCP/UDP/ICMP的帧校验和卸载
    #endif

    //当我们使用帧校验和卸载功能的时候，一定要使能存储转发模式,存储转发模式中要保证整个帧存储在FIFO中,
    //这样MAC能插入/识别出帧校验值,当真校验正确的时候DMA就可以处理帧,否则就丢弃掉该帧
    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; //开启丢弃TCP/IP错误帧
    ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;     //开启接收数据的存储转发模式
    ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;   //开启发送数据的存储转发模式

    ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;     	//禁止转发错误帧
    ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;	//不转发过小的好帧
    ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;  		//打开处理第二帧功能
    ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;  	//开启DMA传输的地址对齐功能
    ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;            			//开启固定突发功能
    ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;     		//DMA发送的最大突发长度为32个节拍
    ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;			//DMA接收的最大突发长度为32个节拍
    ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

    if(ETH == SGCC_GMAC0_P)
    {
        (*(__IO uint32_t *) (SGCC_GMAC0_BASE + 0x40 + ETH_MAC_Address0)) = ((uint32_t)scm62x_eth_device.dev_addr[5] << 8) | (uint32_t)scm62x_eth_device.dev_addr[4];
    }
    else
    {
        (*(__IO uint32_t *) (SGCC_GMAC1_BASE + 0x40 + ETH_MAC_Address0)) = ((uint32_t)scm62x_eth_device.dev_addr[5] << 8) | (uint32_t)scm62x_eth_device.dev_addr[4];
    }

    /* Load the selectecd MAC address low register */
    if(ETH == SGCC_GMAC0_P)
    {
        (*(__IO uint32_t *) (SGCC_GMAC0_BASE + 0x44 + ETH_MAC_Address0)) = ((uint32_t)scm62x_eth_device.dev_addr[3] << 24) | ((uint32_t)scm62x_eth_device.dev_addr[2] << 16) | ((uint32_t)scm62x_eth_device.dev_addr[1] << 8) | scm62x_eth_device.dev_addr[0];
    }
    else
    {
        (*(__IO uint32_t *) (SGCC_GMAC1_BASE + 0x44 + ETH_MAC_Address0)) = ((uint32_t)scm62x_eth_device.dev_addr[3] << 24) | ((uint32_t)scm62x_eth_device.dev_addr[2] << 16) | ((uint32_t)scm62x_eth_device.dev_addr[1] << 8) | scm62x_eth_device.dev_addr[0];
    }


    ETH_Init(ETH, &ETH_InitStructure, PHY_ADDRESS);		       //配置ETH

    ETH_DMAITConfig(ETH, ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);  	//使能以太网接收中断

    low_level_init(ETH);

    while (1)
    {
        status = ETH_ReadPHYRegister(ETH, phy_addr, PHY_BASIC_STATUS_REG);
//        MAC_TRACE("PHY BASIC STATUS REG:0x%04X\n", status);

        phy_speed_new = 0;

        if (status & (PHY_AUTONEGO_COMPLETE_MASK | PHY_LINKED_STATUS_MASK))
        {
            rt_uint32_t SR;

            phy_speed_new = PHY_LINK_MASK;

            SR = ETH_ReadPHYRegister(ETH, phy_addr, PHY_SR);
//            MAC_TRACE("PHY Control/Status REG:0x%04X\n", SR);

            if (SR & PHY_Speed_Status)
            {
                phy_speed_new |= PHY_10M_MASK;
            }
            else
            {
                phy_speed_new |= PHY_100M_MASK;
            }

            if (SR & PHY_FULL_DUPLEX_MASK)
            {
                phy_speed_new |= PHY_FULL_DUPLEX_MASK;
            }
        }

        /* linkchange */
        if (phy_speed_new != phy_speed)
        {
            if (phy_speed_new & PHY_LINK_MASK)
            {
                MAC_TRACE("link up \n");

                if (phy_speed_new & PHY_100M_MASK)
                {
                    MAC_TRACE("100Mbps\n");
                    scm62x_eth_device.ETH_Speed = ETH_Speed_100M;
                }
                else
                {
                    scm62x_eth_device.ETH_Speed = ETH_Speed_10M;
                    MAC_TRACE("10Mbps\n");
                }

                if (phy_speed_new & PHY_FULL_DUPLEX_MASK)
                {
                    MAC_TRACE("full-duplex\n");
                    scm62x_eth_device.ETH_Mode = ETH_MODE_FULLDUPLEX;
                }
                else
                {
                    MAC_TRACE("half-duplex\n");
                    scm62x_eth_device.ETH_Mode = ETH_MODE_HALFDUPLEX;
                }


                /* send link up. */
                eth_device_linkchange(&scm62x_eth_device.parent, RT_TRUE);
            } /* link up. */
            else
            {
                MAC_TRACE("link down\n");
                /* send link down. */
                eth_device_linkchange(&scm62x_eth_device.parent, RT_FALSE);
            }

            phy_speed = phy_speed_new;
        }

        rt_thread_delay(RT_TICK_PER_SECOND);
    }
}




#define RT_USING_MAC0
/* Register the EMAC device */
static int rt_hw_scm62x_eth_init(void)
{
    rt_err_t state = RT_EOK;

    scm62x_eth_device.ETH_Speed = ETH_Speed_100M;
    scm62x_eth_device.ETH_Mode  = ETH_Mode_FullDuplex;

    /* OUI 00-80-E1 STMICROELECTRONICS. */
    scm62x_eth_device.dev_addr[0] = 0x00;
    scm62x_eth_device.dev_addr[1] = 0x80;
    scm62x_eth_device.dev_addr[2] = 0xE1;
    /* generate MAC addr from 96bit unique ID (only for test). */
    #ifdef RT_USING_MAC0
    ETH = SGCC_GMAC0_P;
    scm62x_eth_device.dev_addr[3] = 0x44 + ETH_MAC_Address0;
    scm62x_eth_device.dev_addr[4] = 0x55 + ETH_MAC_Address0;
    scm62x_eth_device.dev_addr[5] = 0x66 + ETH_MAC_Address0;
    #endif
    #ifdef RT_USING_MAC1
    ETH = SGCC_GMAC1_P;
    scm62x_eth_device.dev_addr[3] = 0x44 + ETH_MAC_Address0;
    scm62x_eth_device.dev_addr[4] = 0x55 + ETH_MAC_Address0;
    scm62x_eth_device.dev_addr[5] = 0x66 + ETH_MAC_Address0;
    #endif


    scm62x_eth_device.parent.parent.init       = rt_scm62x_eth_init;
    scm62x_eth_device.parent.parent.open       = rt_scm62x_eth_open;
    scm62x_eth_device.parent.parent.close      = rt_scm62x_eth_close;
    scm62x_eth_device.parent.parent.read       = rt_scm62x_eth_read;
    scm62x_eth_device.parent.parent.write      = rt_scm62x_eth_write;
    scm62x_eth_device.parent.parent.control    = rt_scm62x_eth_control;
    scm62x_eth_device.parent.parent.user_data  = RT_NULL;

    scm62x_eth_device.parent.eth_rx     = rt_scm62x_eth_rx;
    scm62x_eth_device.parent.eth_tx     = rt_scm62x_eth_tx;

    /* register eth device */
    state = eth_device_init(&(scm62x_eth_device.parent), "eth0");

    if (RT_EOK == state)
    {
        MAC_TRACE("emac device init success\n");
    }
    else
    {
        MAC_TRACE("emac device init faild: %d\n", state);
        state = -RT_ERROR;
        goto __exit;
    }

    /* start phy monitor */
    rt_thread_t tid;
    tid = rt_thread_create("phy",
                           phy_monitor_thread_entry,
                           RT_NULL,
                           1024,
                           RT_THREAD_PRIORITY_MAX - 2,
                           2);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        state = -RT_ERROR;
    }

__exit:

    return state;
}
INIT_DEVICE_EXPORT(rt_hw_scm62x_eth_init);

#define BUFSZ  4096
void tcpclient(int argc, char **argv)
{
    int ret;
    char *recv_data;
    struct hostent *host;
    int sock, bytes_received;
    struct sockaddr_in server_addr;
    const char *url;
    int port;
    const char send_data[] = "This is scm62x TCP Test!\n";

    /* 接收到的参数小于 3 个 */
    if (argc < 3)
    {
        rt_kprintf("Usage: tcpclient URL PORT\n");
        rt_kprintf("Like: tcpclient 192.168.12.44 5000\n");
        return ;
    }

    url = argv[1];
    port = strtoul(argv[2], 0, 10);

    /* 通过函数入口参数 url 获得 host 地址（如果是域名，会做域名解析） */
    host = gethostbyname(url);

    /* 分配用于存放接收数据的缓冲 */
    recv_data = rt_malloc(BUFSZ);

    if (recv_data == RT_NULL)
    {
        rt_kprintf("No memory\n");
        return;
    }

    /* 创建一个 socket，类型是 SOCKET_STREAM，TCP 类型 */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        /* 创建 socket 失败 */
        rt_kprintf("Socket error\n");

        /* 释放接收缓冲 */
        rt_free(recv_data);
        return;
    }

    /* 初始化预连接的服务端地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    /* 连接到服务端 */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        /* 连接失败 */
        rt_kprintf("Connect fail!\n");
        closesocket(sock);

        /* 释放接收缓冲 */
        rt_free(recv_data);
        return;
    }

    while (1)
    {
        /* 从 sock 连接中接收最大 BUFSZ - 1 字节数据 */
        bytes_received = recv(sock, recv_data, BUFSZ - 1, 0);

        if (bytes_received < 0)
        {
            /* 接收失败，关闭这个连接 */
            closesocket(sock);
            rt_kprintf("\nreceived error,close the socket.\r\n");

            /* 释放接收缓冲 */
            rt_free(recv_data);
            break;
        }
        else if (bytes_received == 0)
        {
            /* 打印 recv 函数返回值为 0 的警告信息 */
            rt_kprintf("\nReceived warning,recv function return 0.\r\n");

            continue;
        }

        /* 有接收到数据，把末端清零 */
        recv_data[bytes_received] = '\0';

        if (strncmp(recv_data, "q", 1) == 0 || strncmp(recv_data, "Q", 1) == 0)
        {
            /* 如果是首字母是 q 或 Q，关闭这个连接 */
            closesocket(sock);
            rt_kprintf("\n got a'q'or'Q',close the socket.\r\n");

            /* 释放接收缓冲 */
            rt_free(recv_data);
            break;
        }
        else
        {
            /* 在控制终端显示收到的数据 */
            rt_kprintf("\nReceived data = %s", recv_data);
        }

        /* 发送数据到 sock 连接 */
        ret = send(sock, send_data, strlen(send_data), 0);

        if (ret < 0)
        {
            /* 接收失败，关闭这个连接 */
            closesocket(sock);
            rt_kprintf("\nsend error,close the socket.\r\n");

            rt_free(recv_data);
            break;
        }
        else if (ret == 0)
        {
            /* 打印 send 函数返回值为 0 的警告信息 */
            rt_kprintf("\n Send warning,send function return 0.\r\n");
        }
    }

    return;
}

MSH_CMD_EXPORT(tcpclient, tcpclient test input IP PORT);

#endif /* RT_USING_MAC */
