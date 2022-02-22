;/*****************************************************************************
; * @file:    startup_SCM63X.s
; * @purpose: CMSIS Cortex-M4 Core Device Startup File for the 
; *           Device <Device> 
; * @version: V2.10
; * @date:    5. June 2017
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * Copyright (C) 2010 ARM Limited. All rights reserved.
; * ARM Limited (ARM) is supplying this software for use with Cortex-M 
; * processor based microcontrollers.  This file can be freely distributed 
; * within development tools that are supporting such ARM based processors. 
; *
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; *****************************************************************************/
Stack_Size      EQU     0x1000 

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


Heap_Size       EQU     0x0000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler                         ; Reserved
                DCD     BusFault_Handler                        ; Reserved
                DCD     UsageFault_Handler                        ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
; ToDo:  Add here the vectors for the device specific external interrupts handler
                DCD     SCU_IRQHandler     		; 0: Default
				DCD     WDT_IRQHandler        ; 1: Default
				DCD     IIC0_IRQHandler        	;  2: Default
                DCD     IIC1_IRQHandler        	;  3: Default
                DCD     IIC2_IRQHandler        	;  4: Default
                DCD     UART0_IRQHandler       	;  5: Default
                DCD     UART1_IRQHandler       	;  6: Default ;For Cortex-M0
                DCD     UART2_IRQHandler        	; 7: Default
				DCD     UART3_IRQHandler        	;  8: Default
				DCD     UART4_IRQHandler        	;  9: Default
				DCD     UART5_IRQHandler        	;  10: Default
				DCD     UART6_IRQHandler        	;  11: Default
				DCD     UART7_IRQHandler        	;  12: Default
				DCD     SPI0_IRQHandler        	;  13: Default
                DCD     SPI1_IRQHandler        	;  14: Default
				DCD     SPI2_IRQHandler        	;  15: Default
                DCD     SPI3_IRQHandler        	;  16: Default
				DCD     TIMERA0_IRQHandler        	;  17: Default
                DCD     TIMERA1_IRQHandler        	;  18: Default
                DCD     TIMERB0_IRQHandler      ;  19: 
                DCD     TIMERB1_IRQHandler        	;  20: Default
                DCD     TIMERC0_IRQHandler        	;  21: Default
                DCD     TIMERC1_IRQHandler        	;  22: Default
                DCD     TIMERD0_IRQHandler      ;  23: 
                DCD     TIMERD1_IRQHandler        	;  24: Default
                DCD     DMA00_IRQHandler         ; 25
				DCD     DMA01_IRQHandler        	; 26: Default
				DCD     DMA02_IRQHandler        	; 27: Default
				DCD     DMA03_IRQHandler        	;  28: Default
				DCD     DMA10_IRQHandler         ; 29
				DCD     DMA11_IRQHandler        	; 30: Default
				DCD     DMA12_IRQHandler        	; 31: Default
				DCD     DMA13_IRQHandler        	;  32: Default
				DCD     DMA20_IRQHandler         ; 33
				DCD     DMA21_IRQHandler        	; 34: Default
				DCD     DMA22_IRQHandler        	;35: Default
				DCD     DMA23_IRQHandler        	; 36: Default
				DCD     DMA30_IRQHandler         ; 37
				DCD     DMA31_IRQHandler        	;38: Default
				DCD     DMA32_IRQHandler        	;39: Default
				DCD     DMA33_IRQHandler        	; 40: Default
                DCD     ALM_IRQHandler        	;  41: Default
				DCD     PRD_IRQHandler        	;  42: Default
                DCD     ADCCTL0_IRQHandler        	;  43: Default  
                DCD     GPIO_IRQHandler         ;44
                DCD     OTG_IRQHandler          ;45
				DCD     GMAC0_IRQHandler        ;46
 				DCD     GMAC1_IRQHandler        ;47
                DCD     U7816_IRQHandler        ;48
                DCD     CAN0_IRQHandler        	;  49: Default
				DCD     CAN1_IRQHandler        	;  50: Default
                DCD     CAN2_IRQHandler        	;  51: Default  
                DCD     ADCCTL1_IRQHandler         ;52
                DCD     ADCCTL2_IRQHandler          ;53
				DCD     RESV                       ;54
 				DCD     NFC_IRQHandler        ;55
                DCD     SM4_IRQHandler        ;56
                DCD     QSPI0_IRQHandler        ;57
                DCD     QSPI1_IRQHandler        ;58

__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors


                AREA    |.text|, CODE, READONLY


; Reset Handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  InitSysClk
                IMPORT  __main

                LDR     R0, =InitSysClk
                BLX     R0
				
                LDR     R0, =__main
                BX     R0

                ENDP
			

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler	\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler  PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP
                
DEF_IRQHandler PROC
                EXPORT     SCU_IRQHandler     		[WEAK]
				        EXPORT     WDT_IRQHandler         [WEAK]
				        EXPORT     IIC0_IRQHandler        [WEAK]
                EXPORT     IIC1_IRQHandler        [WEAK]
                EXPORT     IIC2_IRQHandler        [WEAK]
                EXPORT     UART0_IRQHandler       [WEAK]
                EXPORT     UART1_IRQHandler       [WEAK]
                EXPORT     UART2_IRQHandler       [WEAK]
				        EXPORT     UART3_IRQHandler       [WEAK]
				        EXPORT     UART4_IRQHandler       [WEAK]
				        EXPORT     UART5_IRQHandler       [WEAK]
				        EXPORT     UART6_IRQHandler       [WEAK]
				        EXPORT     UART7_IRQHandler       [WEAK]
				        EXPORT     SPI0_IRQHandler        [WEAK]
                EXPORT     SPI1_IRQHandler        [WEAK]
				        EXPORT     SPI2_IRQHandler        [WEAK]
                EXPORT     SPI3_IRQHandler        [WEAK]
				        EXPORT     TIMERA0_IRQHandler        [WEAK]
                EXPORT     TIMERA1_IRQHandler        [WEAK]
                EXPORT     TIMERB0_IRQHandler        [WEAK]
                EXPORT     TIMERB1_IRQHandler        [WEAK]
                EXPORT     TIMERC0_IRQHandler        [WEAK]
                EXPORT     TIMERC1_IRQHandler        [WEAK]
                EXPORT     TIMERD0_IRQHandler        [WEAK]
                EXPORT     TIMERD1_IRQHandler        [WEAK]
                EXPORT     DMA00_IRQHandler        [WEAK]
				        EXPORT     DMA01_IRQHandler        [WEAK]
				        EXPORT     DMA02_IRQHandler        [WEAK]
				        EXPORT     DMA03_IRQHandler        [WEAK]
				        EXPORT     DMA10_IRQHandler        [WEAK]
				        EXPORT     DMA11_IRQHandler        [WEAK]
				        EXPORT     DMA12_IRQHandler        [WEAK]
				        EXPORT     DMA13_IRQHandler        [WEAK]
				        EXPORT     DMA20_IRQHandler        [WEAK]
				        EXPORT     DMA21_IRQHandler        [WEAK]
				        EXPORT     DMA22_IRQHandler       [WEAK]
				        EXPORT     DMA23_IRQHandler       [WEAK]
				        EXPORT     DMA30_IRQHandler       [WEAK]
				        EXPORT     DMA31_IRQHandler       [WEAK]
				        EXPORT     DMA32_IRQHandler       [WEAK]
				        EXPORT     DMA33_IRQHandler       [WEAK]
                EXPORT     ALM_IRQHandler         [WEAK]
				        EXPORT     PRD_IRQHandler         [WEAK]
                EXPORT     ADCCTL0_IRQHandler      [WEAK]
                EXPORT     GPIO_IRQHandler        [WEAK]
                EXPORT     OTG_IRQHandler         [WEAK]
				        EXPORT     GMAC0_IRQHandler       [WEAK]
 				        EXPORT     GMAC1_IRQHandler       [WEAK]
                EXPORT     U7816_IRQHandler       [WEAK]   
                EXPORT     CAN0_IRQHandler             [WEAK]                    
 				        EXPORT     CAN1_IRQHandler              [WEAK]        
 				        EXPORT     CAN2_IRQHandler              [WEAK]
                EXPORT     ADCCTL1_IRQHandler           [WEAK]          
                EXPORT     ADCCTL2_IRQHandler           [WEAK]        
 				        EXPORT     NFC_IRQHandler               [WEAK]
                EXPORT     SM4_IRQHandler               [WEAK]    
                EXPORT     QSPI0_IRQHandler             [WEAK]
                EXPORT     QSPI1_IRQHandler             [WEAK]             
SCU_IRQHandler     
WDT_IRQHandler     
IIC0_IRQHandler    
IIC1_IRQHandler    
IIC2_IRQHandler    
UART0_IRQHandler   
UART1_IRQHandler   
UART2_IRQHandler   
UART3_IRQHandler   
UART4_IRQHandler   
UART5_IRQHandler   
UART6_IRQHandler   
UART7_IRQHandler   
SPI0_IRQHandler    
SPI1_IRQHandler    
SPI2_IRQHandler    
SPI3_IRQHandler    
TIMERA0_IRQHandler    
TIMERA1_IRQHandler    
TIMERB0_IRQHandler    
TIMERB1_IRQHandler    
TIMERC0_IRQHandler    
TIMERC1_IRQHandler    
TIMERD0_IRQHandler    
TIMERD1_IRQHandler    
DMA00_IRQHandler    
DMA01_IRQHandler    
DMA02_IRQHandler    
DMA03_IRQHandler    
DMA10_IRQHandler    
DMA11_IRQHandler    
DMA12_IRQHandler    
DMA13_IRQHandler    
DMA20_IRQHandler    
DMA21_IRQHandler    
DMA22_IRQHandler   
DMA23_IRQHandler   
DMA30_IRQHandler   
DMA31_IRQHandler   
DMA32_IRQHandler   
DMA33_IRQHandler     
ALM_IRQHandler     
PRD_IRQHandler     
ADCCTL0_IRQHandler  
GPIO_IRQHandler    
OTG_IRQHandler     
GMAC0_IRQHandler   
GMAC1_IRQHandler   
U7816_IRQHandler      
CAN0_IRQHandler                           
CAN1_IRQHandler                    
CAN2_IRQHandler              
ADCCTL1_IRQHandler                    
ADCCTL2_IRQHandler                  
NFC_IRQHandler               
SM4_IRQHandler   
QSPI0_IRQHandler                
QSPI1_IRQHandler          
RESV


                B       .
                ENDP


                ALIGN
; User Initial Stack & Heap

               IF      :DEF:__MICROLIB
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
