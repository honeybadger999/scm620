/**
  ******************************************************************************
  * @file    netconf.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013 
  * @brief   This file contains all the functions prototypes for the netconf.c 
  *          file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETCONF_H
#define __NETCONF_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#define DHCP_START                 1
#define DHCP_WAIT_ADDRESS          2
#define DHCP_ADDRESS_ASSIGNED      3
#define DHCP_TIMEOUT               4
#define DHCP_LINK_DOWN             5
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
typedef struct 
{
	uint8_t serial_number_H;
	uint8_t serial_number_L;
	uint8_t identifier_H;
	uint8_t identifier_L;
	uint8_t PDU_len_H;
	uint8_t PDU_len_L;
	uint8_t station_add;
	uint8_t function_code; //标识符
	uint8_t add_H; 
	uint8_t add_L; 
	uint8_t len_or_data_H;
	uint8_t len_or_data_L;
}ModbusTCP_Struct;

//slave发送报文标准结构体
typedef struct 
{
	uint8_t serial_number_H;
	uint8_t serial_number_L;
	uint8_t identifier_H;
	uint8_t identifier_L;
	uint8_t PDU_len_H;
	uint8_t PDU_len_L;
	uint8_t station_add;
	uint8_t function_code; //标识符
	uint8_t  quant; 
	uint8_t  data[256];
}ModbusTCP_Send_Struct;

struct struct_Ethernet_Par
{
	uint16_t MAC_ADD0;
	uint16_t MAC_ADD1;
	uint16_t MAC_ADD2;
	uint16_t MAC_ADD3;
	uint16_t MAC_ADD4;
	uint16_t MAC_ADD5;
	uint16_t IP_ADD0;
	uint16_t IP_ADD1;
	uint16_t IP_ADD2;
	uint16_t IP_ADD3;
	uint16_t GW_ADD0;
	uint16_t GW_ADD1;
	uint16_t GW_ADD2;
	uint16_t GW_ADD3;
	
};
extern uint16_t *P_ethernet;
/* Exported functions ------------------------------------------------------- */
void LwIP_Init(void);
void LwIP_DHCP_task(void * pvParameters);



#ifdef __cplusplus
}
#endif

#endif /* __NETCONF_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


