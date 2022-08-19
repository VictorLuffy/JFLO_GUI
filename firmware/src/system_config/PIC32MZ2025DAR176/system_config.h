/*******************************************************************************
  MPLAB Harmony System Configuration Header

  File Name:
    system_config.h

  Summary:
    Build-time configuration header for the system defined by this MPLAB Harmony
    project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options that are not instantiated
    until used by another MPLAB Harmony module or application.

    Created with MPLAB Harmony Version 2.06
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/

/*** SQI Driver Configuration ***/

#define DRV_SQI_INSTANCES_NUMBER     	1
#define DRV_SQI_CLIENTS_NUMBER        	1
#define DRV_SQI_BUFFER_OBJECT_NUMBER  	5
#define DRV_SQI_DMA_BUFFER_DESC_NUMBER  4

#define DRV_SQI_INTERRUPT_MODE        	true
#define DRV_SQI_INTERRUPT_SOURCE      	INT_SOURCE_SQI1

/*** SST26 Driver Configuration ***/
#define DRV_SST26_INSTANCES_NUMBER     	1
#define DRV_SST26_CLIENTS_NUMBER        1
#define DRV_SST26_BUFFER_OBJECT_NUMBER  5
#define DRV_SST26_SYS_FS_REGISTER

#include "bsp.h"


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Common System Service Configuration Options
*/
#define SYS_VERSION_STR           "2.06"
#define SYS_VERSION               20600

// *****************************************************************************
/* Clock System Service Configuration Options
*/
#define SYS_CLK_FREQ                        200000000ul
#define SYS_CLK_BUS_PERIPHERAL_1            100000000ul
#define SYS_CLK_BUS_PERIPHERAL_2            100000000ul
#define SYS_CLK_BUS_PERIPHERAL_3            100000000ul
#define SYS_CLK_BUS_PERIPHERAL_4            100000000ul
#define SYS_CLK_BUS_PERIPHERAL_5            100000000ul
#define SYS_CLK_BUS_PERIPHERAL_6            100000000ul
#define SYS_CLK_BUS_PERIPHERAL_7            200000000ul
#define SYS_CLK_BUS_REFERENCE_1             11289966ul
#define SYS_CLK_BUS_REFERENCE_2             200000000ul
#define SYS_CLK_BUS_REFERENCE_5             100000000ul
#define SYS_CLK_CONFIG_PRIMARY_XTAL         24000000ul
#define SYS_CLK_CONFIG_SECONDARY_XTAL       32768ul
   
/*** Ports System Service Configuration ***/
#define SYS_PORT_A_ANSEL        0x3902
#define SYS_PORT_A_TRIS         0xFBDF
#define SYS_PORT_A_LAT          0x0420
#define SYS_PORT_A_ODC          0x0000
#define SYS_PORT_A_CNPU         0xC000
#define SYS_PORT_A_CNPD         0x0000
#define SYS_PORT_A_CNEN         0x0000

#define SYS_PORT_B_ANSEL        0x0000
#define SYS_PORT_B_TRIS         0xC4F6
#define SYS_PORT_B_LAT          0x1308
#define SYS_PORT_B_ODC          0x0000
#define SYS_PORT_B_CNPU         0xC002
#define SYS_PORT_B_CNPD         0x0000
#define SYS_PORT_B_CNEN         0x0000

#define SYS_PORT_C_ANSEL        0x0FE5
#define SYS_PORT_C_TRIS         0xFFFF
#define SYS_PORT_C_LAT          0x0000
#define SYS_PORT_C_ODC          0x0000
#define SYS_PORT_C_CNPU         0x0002
#define SYS_PORT_C_CNPD         0x0000
#define SYS_PORT_C_CNEN         0x0000

#define SYS_PORT_D_ANSEL        0x8100
#define SYS_PORT_D_TRIS         0xF7FD
#define SYS_PORT_D_LAT          0x0000
#define SYS_PORT_D_ODC          0x0000
#define SYS_PORT_D_CNPU         0x0000
#define SYS_PORT_D_CNPD         0x4000
#define SYS_PORT_D_CNEN         0x0000

#define SYS_PORT_E_ANSEL        0xFC10
#define SYS_PORT_E_TRIS         0xFFFF
#define SYS_PORT_E_LAT          0x0000
#define SYS_PORT_E_ODC          0x0000
#define SYS_PORT_E_CNPU         0x0000
#define SYS_PORT_E_CNPD         0x0000
#define SYS_PORT_E_CNEN         0x0000

#define SYS_PORT_F_ANSEL        0xCEC0
#define SYS_PORT_F_TRIS         0xEFFF
#define SYS_PORT_F_LAT          0x0000
#define SYS_PORT_F_ODC          0x0000
#define SYS_PORT_F_CNPU         0x0000
#define SYS_PORT_F_CNPD         0x0000
#define SYS_PORT_F_CNEN         0x0000

#define SYS_PORT_G_ANSEL        0x8C3C
#define SYS_PORT_G_TRIS         0xFFFF
#define SYS_PORT_G_LAT          0x0000
#define SYS_PORT_G_ODC          0x0000
#define SYS_PORT_G_CNPU         0x0000
#define SYS_PORT_G_CNPD         0x0000
#define SYS_PORT_G_CNEN         0x0000

#define SYS_PORT_H_ANSEL        0x0010
#define SYS_PORT_H_TRIS         0xFDF1
#define SYS_PORT_H_LAT          0x0000
#define SYS_PORT_H_ODC          0x0000
#define SYS_PORT_H_CNPU         0x0000
#define SYS_PORT_H_CNPD         0x0000
#define SYS_PORT_H_CNEN         0x0000

#define SYS_PORT_J_ANSEL        0x0000
#define SYS_PORT_J_TRIS         0xFFFB
#define SYS_PORT_J_LAT          0x0000
#define SYS_PORT_J_ODC          0x0000
#define SYS_PORT_J_CNPU         0x0000
#define SYS_PORT_J_CNPD         0x0000
#define SYS_PORT_J_CNEN         0x0000

#define SYS_PORT_K_ANSEL        0xFF02
#define SYS_PORT_K_TRIS         0xFFFF
#define SYS_PORT_K_LAT          0x0000
#define SYS_PORT_K_ODC          0x0000
#define SYS_PORT_K_CNPU         0x0000
#define SYS_PORT_K_CNPD         0x0000
#define SYS_PORT_K_CNEN         0x0000


/*** Command Processor System Service Configuration ***/
#define SYS_CMD_ENABLE
#define SYS_CMD_DEVICE_MAX_INSTANCES    SYS_CONSOLE_DEVICE_MAX_INSTANCES
#define SYS_CMD_PRINT_BUFFER_SIZE       4096
#define SYS_CMD_BUFFER_DMA_READY        __attribute__((coherent)) __attribute__((aligned(16)))

/*** Console System Service Configuration ***/

#define SYS_CONSOLE_OVERRIDE_STDIO
#define SYS_CONSOLE_DEVICE_MAX_INSTANCES        2
#define SYS_CONSOLE_INSTANCES_NUMBER            1
#define SYS_CONSOLE_UART_IDX               DRV_USART_INDEX_0
#define SYS_CONSOLE_UART_BAUD_RATE_IDX     DRV_USART_BAUD_RATE_IDX0
#define SYS_CONSOLE_UART_RD_QUEUE_DEPTH    1
#define SYS_CONSOLE_UART_WR_QUEUE_DEPTH    64
#define SYS_CONSOLE_BUFFER_DMA_READY



/*** Debug System Service Configuration ***/
#define SYS_DEBUG_ENABLE
#define DEBUG_PRINT_BUFFER_SIZE       1024//4096 //tt_debug
#define SYS_DEBUG_BUFFER_DMA_READY
#define SYS_DEBUG_USE_CONSOLE

/*** File System Service Configuration ***/

#define SYS_FS_MEDIA_NUMBER         	2

#define SYS_FS_VOLUME_NUMBER		(1 + 1)

#define SYS_FS_AUTOMOUNT_ENABLE		true
#define SYS_FS_CLIENT_NUMBER		1
#define SYS_FS_MAX_FILES	    	10
#define SYS_FS_MAX_FILE_SYSTEM_TYPE 	1
#define SYS_FS_MEDIA_MAX_BLOCK_SIZE  	512
#define SYS_FS_MEDIA_MANAGER_BUFFER_SIZE 2048
#define SYS_FS_FILE_NAME_LEN 255
#define SYS_FS_CWD_STRING_LEN 1024


#define SYS_FS_MEDIA_TYPE_IDX0 				SYS_FS_MEDIA_TYPE_SPIFLASH
#define SYS_FS_TYPE_IDX0 					FAT







#define SYS_FS_MEDIA_TYPE_IDX1 				SYS_FS_MEDIA_TYPE_MSD
#define SYS_FS_TYPE_IDX1 					FAT








#define SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0 			"/mnt/SQIFlash"
#define SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0 			"/dev/mtda1"

#define SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0 			"/mnt/USB"
#define SYS_FS_MEDIA_IDX1_DEVICE_NAME_VOLUME_IDX0 			"/dev/sda1"

/*** Interrupt System Service Configuration ***/
#define SYS_INT                     true
// *****************************************************************************
/* Memory System Service Configuration Options
*/
#define DDR_SIZE				(32 * 1024 *1024)

/*** Timer System Service Configuration ***/
#define SYS_TMR_POWER_STATE             SYS_MODULE_POWER_RUN_FULL
#define SYS_TMR_DRIVER_INDEX            DRV_TMR_INDEX_6
#define SYS_TMR_MAX_CLIENT_OBJECTS      5
#define SYS_TMR_FREQUENCY               1000
#define SYS_TMR_FREQUENCY_TOLERANCE     10
#define SYS_TMR_UNIT_RESOLUTION         10000
#define SYS_TMR_CLIENT_TOLERANCE        10
#define SYS_TMR_INTERRUPT_NOTIFICATION  false

// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* I2C Driver Configuration Options
*/
#define DRV_I2C_INTERRUPT_MODE                    		true
#define DRV_I2C_CLIENTS_NUMBER                    		4
#define DRV_I2C_INSTANCES_NUMBER                  		4

#define DRV_I2C_PERIPHERAL_ID_IDX0                		I2C_ID_1
#define DRV_I2C_OPERATION_MODE_IDX0               		DRV_I2C_MODE_MASTER
#define DRV_SCL_PORT_IDX0                               PORT_CHANNEL_A
#define DRV_SCL_PIN_POSITION_IDX0                       PORTS_BIT_POS_14
#define DRV_SDA_PORT_IDX0                               PORT_CHANNEL_A
#define DRV_SDA_PIN_POSITION_IDX0                       PORTS_BIT_POS_15
#define DRV_I2C_BIT_BANG_IDX0                           false
#define DRV_I2C_STOP_IN_IDLE_IDX0                       false
#define DRV_I2C_SMBus_SPECIFICATION_IDX0			    false
#define DRV_I2C_BAUD_RATE_IDX0                    		100000
#define DRV_I2C_BRG_CLOCK_IDX0	                  		100000000
#define DRV_I2C_SLEW_RATE_CONTROL_IDX0      			false
#define DRV_I2C_MASTER_INT_SRC_IDX0               		INT_SOURCE_I2C_1_MASTER
#define DRV_I2C_SLAVE_INT_SRC_IDX0                		
#define DRV_I2C_ERR_MZ_INT_SRC_IDX0               		INT_SOURCE_I2C_1_BUS
#define DRV_I2C_MASTER_INT_VECTOR_IDX0            		INT_VECTOR_I2C1_MASTER
#define DRV_I2C_MASTER_ISR_VECTOR_IDX0                  _I2C1_MASTER_VECTOR
#define DRV_I2C_MASTER_INT_PRIORITY_IDX0          		INT_PRIORITY_LEVEL1
#define DRV_I2C_MASTER_INT_SUB_PRIORITY_IDX0      		INT_SUBPRIORITY_LEVEL0
#define DRV_I2C_SLAVE_INT_VECTOR_IDX0             		INT_VECTOR_I2C1_SLAVE
#define DRV_I2C_SLAVE_ISR_VECTOR_IDX0			  	    _I2C1_SLAVE_VECTOR
#define DRV_I2C_SLAVE_INT_PRIORITY_IDX0           		
#define DRV_I2C_SLAVE_INT_SUB_PRIORITY_IDX0       		
#define DRV_I2C_ERR_INT_VECTOR_IDX0               		INT_VECTOR_I2C1_BUS
#define DRV_I2C_ERR_ISR_VECTOR_IDX0                     _I2C1_BUS_VECTOR
#define DRV_I2C_ERR_INT_PRIORITY_IDX0             		INT_DISABLE_INTERRUPT
#define DRV_I2C_ERR_INT_SUB_PRIORITY_IDX0         		INT_SUBPRIORITY_LEVEL0
#define DRV_I2C_POWER_STATE_IDX0                  		SYS_MODULE_POWER_RUN_FULL

#define DRV_I2C_PERIPHERAL_ID_IDX1                      I2C_ID_2
#define DRV_I2C_OPERATION_MODE_IDX1                     DRV_I2C_MODE_MASTER
#define DRV_SCL_PORT_IDX1                               PORT_CHANNEL_A
#define DRV_SCL_PIN_POSITION_IDX1                       PORTS_BIT_POS_2
#define DRV_SDA_PORT_IDX1                               PORT_CHANNEL_A
#define DRV_SDA_PIN_POSITION_IDX1                       PORTS_BIT_POS_3
#define DRV_I2C_BIT_BANG_IDX1                           false
#define DRV_I2C_STOP_IN_IDLE_IDX1                       false
#define DRV_I2C_SMBus_SPECIFICATION_IDX1                false
#define DRV_I2C_BAUD_RATE_IDX1                    		100000
#define DRV_I2C_BRG_CLOCK_IDX1	                  		100000000
#define DRV_I2C_SLEW_RATE_CONTROL_IDX1                  false

#define DRV_I2C_MASTER_INT_SRC_IDX1                     INT_SOURCE_I2C_2_MASTER
#define DRV_I2C_SLAVE_INT_SRC_IDX1                      
#define DRV_I2C_ERR_MZ_INT_SRC_IDX1                     INT_SOURCE_I2C_2_BUS
#define DRV_I2C_MASTER_INT_VECTOR_IDX1                  INT_VECTOR_I2C2_MASTER
#define DRV_I2C_MASTER_ISR_VECTOR_IDX1                  _I2C2_MASTER_VECTOR
#define DRV_I2C_MASTER_INT_PRIORITY_IDX1                INT_PRIORITY_LEVEL1
#define DRV_I2C_MASTER_INT_SUB_PRIORITY_IDX1            INT_SUBPRIORITY_LEVEL0
#define DRV_I2C_SLAVE_INT_VECTOR_IDX1                   INT_VECTOR_I2C2_SLAVE
#define DRV_I2C_SLAVE_ISR_VECTOR_IDX1                   _I2C2_SLAVE_VECTOR
#define DRV_I2C_SLAVE_INT_PRIORITY_IDX1           		
#define DRV_I2C_SLAVE_INT_SUB_PRIORITY_IDX1       		
#define DRV_I2C_ERR_INT_VECTOR_IDX1               		INT_VECTOR_I2C2_BUS
#define DRV_I2C_ERR_ISR_VECTOR_IDX1                     _I2C2_BUS_VECTOR
#define DRV_I2C_ERR_INT_PRIORITY_IDX1                   INT_DISABLE_INTERRUPT
#define DRV_I2C_ERR_INT_SUB_PRIORITY_IDX1               INT_SUBPRIORITY_LEVEL0
#define DRV_I2C_POWER_STATE_IDX1                        SYS_MODULE_POWER_RUN_FULL

#define DRV_I2C_PERIPHERAL_ID_IDX2                      I2C_ID_3
#define DRV_I2C_OPERATION_MODE_IDX2                     DRV_I2C_MODE_MASTER
#define DRV_SCL_PORT_IDX2                               PORT_CHANNEL_F
#define DRV_SCL_PIN_POSITION_IDX2                       PORTS_BIT_POS_8
#define DRV_SDA_PORT_IDX2                               PORT_CHANNEL_F
#define DRV_SDA_PIN_POSITION_IDX2                       PORTS_BIT_POS_2
#define DRV_I2C_BIT_BANG_IDX2                           false
#define DRV_I2C_STOP_IN_IDLE_IDX2                       false
#define DRV_I2C_SMBus_SPECIFICATION_IDX2                true
#define DRV_I2C_BAUD_RATE_IDX2                          100000
#define DRV_I2C_BRG_CLOCK_IDX2                          100000000
#define DRV_I2C_SLEW_RATE_CONTROL_IDX2                  true

#define DRV_I2C_MASTER_INT_SRC_IDX2                     INT_SOURCE_I2C_3_MASTER
#define DRV_I2C_SLAVE_INT_SRC_IDX2                      
#define DRV_I2C_ERR_MZ_INT_SRC_IDX2                     INT_SOURCE_I2C_3_BUS
#define DRV_I2C_MASTER_INT_VECTOR_IDX2                  INT_VECTOR_I2C3_MASTER
#define DRV_I2C_MASTER_ISR_VECTOR_IDX2                  _I2C3_MASTER_VECTOR
#define DRV_I2C_MASTER_INT_PRIORITY_IDX2                INT_PRIORITY_LEVEL1
#define DRV_I2C_MASTER_INT_SUB_PRIORITY_IDX2            INT_SUBPRIORITY_LEVEL0
#define DRV_I2C_SLAVE_INT_VECTOR_IDX2                   INT_VECTOR_I2C3_SLAVE
#define DRV_I2C_SLAVE_ISR_VECTOR_IDX2                   _I2C3_SLAVE_VECTOR
#define DRV_I2C_SLAVE_INT_PRIORITY_IDX2           		
#define DRV_I2C_SLAVE_INT_SUB_PRIORITY_IDX2             
#define DRV_I2C_ERR_INT_VECTOR_IDX2                     INT_VECTOR_I2C3_BUS
#define DRV_I2C_ERR_ISR_VECTOR_IDX2                     _I2C3_BUS_VECTOR
#define DRV_I2C_ERR_INT_PRIORITY_IDX2                   INT_DISABLE_INTERRUPT
#define DRV_I2C_ERR_INT_SUB_PRIORITY_IDX2               INT_SUBPRIORITY_LEVEL0
#define DRV_I2C_POWER_STATE_IDX2                        SYS_MODULE_POWER_RUN_FULL

#define DRV_I2C_PERIPHERAL_ID_IDX3                      I2C_ID_4
#define DRV_I2C_OPERATION_MODE_IDX3                     DRV_I2C_MODE_MASTER
#define DRV_SCL_PORT_IDX3                               PORT_CHANNEL_A
#define DRV_SCL_PIN_POSITION_IDX3                       PORTS_BIT_POS_2
#define DRV_SDA_PORT_IDX3                               PORT_CHANNEL_A
#define DRV_SDA_PIN_POSITION_IDX3                       PORTS_BIT_POS_3

#define DRV_I2C_BIT_BANG_IDX3                           false
#define DRV_I2C_STOP_IN_IDLE_IDX3                       false
#define DRV_I2C_SMBus_SPECIFICATION_IDX3                false
#define DRV_I2C_BAUD_RATE_IDX3                          400000
#define DRV_I2C_BRG_CLOCK_IDX3                          100000000
#define DRV_I2C_SLEW_RATE_CONTROL_IDX3                  false

#define DRV_I2C_MASTER_INT_SRC_IDX3                     INT_SOURCE_I2C_4_MASTER
#define DRV_I2C_SLAVE_INT_SRC_IDX3                      
#define DRV_I2C_ERR_MZ_INT_SRC_IDX3                     INT_SOURCE_I2C_4_BUS
#define DRV_I2C_MASTER_INT_VECTOR_IDX3                  INT_VECTOR_I2C4_MASTER
#define DRV_I2C_MASTER_ISR_VECTOR_IDX3                  _I2C4_MASTER_VECTOR
#define DRV_I2C_MASTER_INT_PRIORITY_IDX3                INT_PRIORITY_LEVEL1
#define DRV_I2C_MASTER_INT_SUB_PRIORITY_IDX3            INT_SUBPRIORITY_LEVEL0
#define DRV_I2C_SLAVE_INT_VECTOR_IDX3                   INT_VECTOR_I2C4_SLAVE
#define DRV_I2C_SLAVE_ISR_VECTOR_IDX3                   _I2C4_SLAVE_VECTOR
#define DRV_I2C_SLAVE_INT_PRIORITY_IDX3                 
#define DRV_I2C_SLAVE_INT_SUB_PRIORITY_IDX3             
#define DRV_I2C_ERR_INT_VECTOR_IDX3                     INT_VECTOR_I2C4_BUS
#define DRV_I2C_ERR_ISR_VECTOR_IDX3                     _I2C4_BUS_VECTOR
#define DRV_I2C_ERR_INT_PRIORITY_IDX3                   INT_DISABLE_INTERRUPT
#define DRV_I2C_ERR_INT_SUB_PRIORITY_IDX3               INT_SUBPRIORITY_LEVEL0
#define DRV_I2C_POWER_STATE_IDX3                        SYS_MODULE_POWER_RUN_FULL
#define DRV_I2C_INTERRUPT_MODE                    		true


/*** I2S Driver Configuration ***/


#define DRV_I2S_INTERRUPT_MODE					true
#define DRV_I2S_CLIENTS_NUMBER					1
#define DRV_I2S_INSTANCES_NUMBER				1
#define DRV_I2S_STOP_IN_IDLE					false
#define DRV_I2S_PERIPHERAL_ID_IDX0				SPI_ID_4
#define DRV_I2S_USAGE_MODE_IDX0					DRV_I2S_MODE_MASTER
#define DRV_I2S_STOP_IN_IDLE_IDX0				false
#define SPI_BAUD_RATE_CLK_IDX0					SPI_BAUD_RATE_MCLK_CLOCK
#define DRV_I2S_BAUD_RATE                       44100
#define DRV_I2S_CLK_MODE_IDX0					DRV_I2S_CLOCK_MODE_IDLE_LOW_EDGE_RISE
#define SPI_AUDIO_COMM_WIDTH_IDX0				SPI_AUDIO_COMMUNICATION_16DATA_16FIFO_16CHANNEL
#define SPI_AUDIO_TRANSMIT_MODE_IDX0			SPI_AUDIO_TRANSMIT_MONO
#define SPI_INPUT_SAMPLING_PHASE_IDX0			SPI_INPUT_SAMPLING_PHASE_IN_MIDDLE
#define DRV_I2S_AUDIO_PROTOCOL_MODE_IDX0		DRV_I2S_AUDIO_I2S
#define DRV_I2S_TX_INT_SRC_IDX0					INT_SOURCE_SPI_4_TRANSMIT
#define DRV_I2S_RX_INT_SRC_IDX0					INT_SOURCE_SPI_4_RECEIVE
#define QUEUE_SIZE_TX_IDX0                      3
#define QUEUE_SIZE_RX_IDX0                      1
#define DRV_I2S_TX_DMA_CHANNEL_IDX0				DMA_CHANNEL_0
#define DRV_I2S_TX_DMA_SOURCE_IDX0				INT_SOURCE_DMA_0
#define DRV_I2S_POWER_STATE_IDX0				SYS_MODULE_POWER_RUN_FULL
#define DRV_I2S_QUEUE_DEPTH_COMBINED     		4

#define DRV_IC_DRIVER_MODE_STATIC 
#define DRV_OC_DRIVER_MODE_STATIC 

/*** SPI Driver Configuration ***/
#define DRV_SPI_NUMBER_OF_MODULES		6
/*** Driver Compilation and static configuration options. ***/
/*** Select SPI compilation units.***/
#define DRV_SPI_POLLED 				0
#define DRV_SPI_ISR 				1
#define DRV_SPI_MASTER 				1
#define DRV_SPI_SLAVE 				0
#define DRV_SPI_RM 					0
#define DRV_SPI_EBM 				1
#define DRV_SPI_8BIT 				1
#define DRV_SPI_16BIT 				0
#define DRV_SPI_32BIT 				0
#define DRV_SPI_DMA 				0

/*** SPI Driver Static Allocation Options ***/
#define DRV_SPI_INSTANCES_NUMBER 		1
#define DRV_SPI_CLIENTS_NUMBER 			1
#define DRV_SPI_ELEMENTS_PER_QUEUE 		10
/* SPI Driver Instance 0 Configuration */
#define DRV_SPI_SPI_ID_IDX0 				SPI_ID_5
#define DRV_SPI_TASK_MODE_IDX0 				DRV_SPI_TASK_MODE_ISR
#define DRV_SPI_SPI_MODE_IDX0				DRV_SPI_MODE_MASTER
#define DRV_SPI_ALLOW_IDLE_RUN_IDX0			false
#define DRV_SPI_SPI_PROTOCOL_TYPE_IDX0 		DRV_SPI_PROTOCOL_TYPE_STANDARD
#define DRV_SPI_COMM_WIDTH_IDX0 			SPI_COMMUNICATION_WIDTH_8BITS
#define DRV_SPI_CLOCK_SOURCE_IDX0 		    SPI_BAUD_RATE_PBCLK_CLOCK
#define DRV_SPI_SPI_CLOCK_IDX0 				CLK_BUS_PERIPHERAL_2
#define DRV_SPI_BAUD_RATE_IDX0 				1000000
#define DRV_SPI_BUFFER_TYPE_IDX0 			DRV_SPI_BUFFER_TYPE_ENHANCED
#define DRV_SPI_CLOCK_MODE_IDX0 			DRV_SPI_CLOCK_MODE_IDLE_LOW_EDGE_FALL
#define DRV_SPI_INPUT_PHASE_IDX0 			SPI_INPUT_SAMPLING_PHASE_IN_MIDDLE
#define DRV_SPI_TRANSMIT_DUMMY_BYTE_VALUE_IDX0      0xFF

#define DRV_SPI_TX_INT_SOURCE_IDX0 			INT_SOURCE_SPI_5_TRANSMIT
#define DRV_SPI_RX_INT_SOURCE_IDX0 			INT_SOURCE_SPI_5_RECEIVE
#define DRV_SPI_ERROR_INT_SOURCE_IDX0 		INT_SOURCE_SPI_5_ERROR
#define DRV_SPI_TX_INT_VECTOR_IDX0			INT_VECTOR_SPI5_TX
#define DRV_SPI_RX_INT_VECTOR_IDX0			INT_VECTOR_SPI5_RX
#define DRV_DRV_SPI_ERROR_INT_VECTOR_IDX0	INT_VECTOR_SPI5_FAULT
#define DRV_SPI_TX_INT_PRIORITY_IDX0 		INT_PRIORITY_LEVEL1
#define DRV_SPI_TX_INT_SUB_PRIORITY_IDX0 	INT_SUBPRIORITY_LEVEL0
#define DRV_SPI_RX_INT_PRIORITY_IDX0 		INT_PRIORITY_LEVEL1
#define DRV_SPI_RX_INT_SUB_PRIORITY_IDX0 	INT_SUBPRIORITY_LEVEL0
#define DRV_SPI_ERROR_INT_PRIORITY_IDX0 	INT_PRIORITY_LEVEL1
#define DRV_SPI_ERROR_INT_SUB_PRIORITY_IDX0 INT_SUBPRIORITY_LEVEL0
#define DRV_SPI_QUEUE_SIZE_IDX0 			10
#define DRV_SPI_RESERVED_JOB_IDX0 			1
/*** Timer Driver Configuration ***/
#define DRV_TMR_INTERRUPT_MODE             true
#define DRV_TMR_INSTANCES_NUMBER           8
#define DRV_TMR_CLIENTS_NUMBER             1

/*** Timer Driver 0 Configuration ***/
#define DRV_TMR_PERIPHERAL_ID_IDX0          TMR_ID_2
#define DRV_TMR_INTERRUPT_SOURCE_IDX0       INT_SOURCE_TIMER_2
#define DRV_TMR_INTERRUPT_VECTOR_IDX0       INT_VECTOR_T2
#define DRV_TMR_ISR_VECTOR_IDX0             _TIMER_2_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX0     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX0 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX0           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX0               TMR_PRESCALE_VALUE_8
#define DRV_TMR_OPERATION_MODE_IDX0         DRV_TMR_OPERATION_MODE_16_BIT
#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX0     false
#define DRV_TMR_POWER_STATE_IDX0            SYS_MODULE_POWER_RUN_FULL

#define DRV_TMR_PERIPHERAL_ID_IDX1          TMR_ID_3
#define DRV_TMR_INTERRUPT_SOURCE_IDX1       INT_SOURCE_TIMER_3
#define DRV_TMR_INTERRUPT_VECTOR_IDX1       INT_VECTOR_T3
#define DRV_TMR_ISR_VECTOR_IDX1             _TIMER_3_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX1     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX1 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX1           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX1               TMR_PRESCALE_VALUE_32
#define DRV_TMR_OPERATION_MODE_IDX1         DRV_TMR_OPERATION_MODE_16_BIT

#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX1     false
#define DRV_TMR_POWER_STATE_IDX1            SYS_MODULE_POWER_RUN_FULL
/*** Timer Driver 2 Configuration ***/
#define DRV_TMR_PERIPHERAL_ID_IDX2          TMR_ID_4
#define DRV_TMR_INTERRUPT_SOURCE_IDX2       INT_SOURCE_TIMER_4
#define DRV_TMR_INTERRUPT_VECTOR_IDX2       INT_VECTOR_T4
#define DRV_TMR_ISR_VECTOR_IDX2             _TIMER_4_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX2     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX2 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX2           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX2               TMR_PRESCALE_VALUE_2
#define DRV_TMR_OPERATION_MODE_IDX2         DRV_TMR_OPERATION_MODE_16_BIT
#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX2     false
#define DRV_TMR_POWER_STATE_IDX2            SYS_MODULE_POWER_RUN_FULL
/*** Timer Driver 3 Configuration ***/
#define DRV_TMR_PERIPHERAL_ID_IDX3          TMR_ID_5
#define DRV_TMR_INTERRUPT_SOURCE_IDX3       INT_SOURCE_TIMER_5
#define DRV_TMR_INTERRUPT_VECTOR_IDX3       INT_VECTOR_T5
#define DRV_TMR_ISR_VECTOR_IDX3             _TIMER_5_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX3     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX3 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX3           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX3               TMR_PRESCALE_VALUE_32
#define DRV_TMR_OPERATION_MODE_IDX3         DRV_TMR_OPERATION_MODE_16_BIT
#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX3     false
#define DRV_TMR_POWER_STATE_IDX3            SYS_MODULE_POWER_RUN_FULL

/*** Timer Driver 4 Configuration ***/
#define DRV_TMR_PERIPHERAL_ID_IDX4          TMR_ID_6
#define DRV_TMR_INTERRUPT_SOURCE_IDX4       INT_SOURCE_TIMER_6
#define DRV_TMR_INTERRUPT_VECTOR_IDX4       INT_VECTOR_T6
#define DRV_TMR_ISR_VECTOR_IDX4             _TIMER_6_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX4     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX4 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX4           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX4               TMR_PRESCALE_VALUE_256
#define DRV_TMR_OPERATION_MODE_IDX4         DRV_TMR_OPERATION_MODE_16_BIT
#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX4     false
#define DRV_TMR_POWER_STATE_IDX4            SYS_MODULE_POWER_RUN_FULL

/*** Timer Driver 5 Configuration ***/
#define DRV_TMR_PERIPHERAL_ID_IDX5          TMR_ID_7
#define DRV_TMR_INTERRUPT_SOURCE_IDX5       INT_SOURCE_TIMER_7
#define DRV_TMR_INTERRUPT_VECTOR_IDX5       INT_VECTOR_T7
#define DRV_TMR_ISR_VECTOR_IDX5             _TIMER_7_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX5     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX5 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX5           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX5               TMR_PRESCALE_VALUE_256
#define DRV_TMR_OPERATION_MODE_IDX5         DRV_TMR_OPERATION_MODE_16_BIT
#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX5     false
#define DRV_TMR_POWER_STATE_IDX5            SYS_MODULE_POWER_RUN_FULL

/*** Timer Driver 6 Configuration ***/
#define DRV_TMR_PERIPHERAL_ID_IDX6          TMR_ID_8
#define DRV_TMR_INTERRUPT_SOURCE_IDX6       INT_SOURCE_TIMER_8
#define DRV_TMR_INTERRUPT_VECTOR_IDX6       INT_VECTOR_T8
#define DRV_TMR_ISR_VECTOR_IDX6             _TIMER_8_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX6     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX6 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX6           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX6               TMR_PRESCALE_VALUE_32
#define DRV_TMR_OPERATION_MODE_IDX6         DRV_TMR_OPERATION_MODE_16_BIT
#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX6     false
#define DRV_TMR_POWER_STATE_IDX6            SYS_MODULE_POWER_RUN_FULL

/*** Timer Driver 7 Configuration ***/
#define DRV_TMR_PERIPHERAL_ID_IDX7          TMR_ID_9
#define DRV_TMR_INTERRUPT_SOURCE_IDX7       INT_SOURCE_TIMER_9
#define DRV_TMR_INTERRUPT_VECTOR_IDX7       INT_VECTOR_T9
#define DRV_TMR_ISR_VECTOR_IDX7             _TIMER_9_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX7     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX7 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX7           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX7               TMR_PRESCALE_VALUE_256
#define DRV_TMR_OPERATION_MODE_IDX7         DRV_TMR_OPERATION_MODE_16_BIT
#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX7     false
#define DRV_TMR_POWER_STATE_IDX7            SYS_MODULE_POWER_RUN_FULL


 // *****************************************************************************
/* USART Driver Configuration Options
*/
#define DRV_USART_INTERRUPT_MODE                    true

#define DRV_USART_BYTE_MODEL_SUPPORT                false

#define DRV_USART_READ_WRITE_MODEL_SUPPORT          true

#define DRV_USART_BUFFER_QUEUE_SUPPORT              true

#define DRV_USART_CLIENTS_NUMBER                    4
#define DRV_USART_INSTANCES_NUMBER                  4

#define DRV_USART_PERIPHERAL_ID_IDX0                USART_ID_1
#define DRV_USART_OPER_MODE_IDX0                    DRV_USART_OPERATION_MODE_NORMAL
#define DRV_USART_OPER_MODE_DATA_IDX0               
#define DRV_USART_INIT_FLAG_WAKE_ON_START_IDX0      false
#define DRV_USART_INIT_FLAG_AUTO_BAUD_IDX0          false
#define DRV_USART_INIT_FLAG_STOP_IN_IDLE_IDX0       false
#define DRV_USART_INIT_FLAGS_IDX0                   0
#define DRV_USART_BRG_CLOCK_IDX0                    100000000
#define DRV_USART_BAUD_RATE_IDX0                    115200
#define DRV_USART_LINE_CNTRL_IDX0                   DRV_USART_LINE_CONTROL_8NONE1
#define DRV_USART_HANDSHAKE_MODE_IDX0               DRV_USART_HANDSHAKE_NONE
#define DRV_USART_LINES_ENABLE_IDX0                 USART_ENABLE_TX_RX_USED
#define DRV_USART_XMIT_INT_SRC_IDX0                 INT_SOURCE_USART_1_TRANSMIT
#define DRV_USART_RCV_INT_SRC_IDX0                  INT_SOURCE_USART_1_RECEIVE
#define DRV_USART_ERR_INT_SRC_IDX0                  INT_SOURCE_USART_1_ERROR
#define DRV_USART_XMIT_INT_VECTOR_IDX0              INT_VECTOR_UART1_TX
#define DRV_USART_XMIT_INT_PRIORITY_IDX0            INT_PRIORITY_LEVEL1
#define DRV_USART_XMIT_INT_SUB_PRIORITY_IDX0        INT_SUBPRIORITY_LEVEL0
#define DRV_USART_RCV_INT_VECTOR_IDX0               INT_VECTOR_UART1_RX
#define DRV_USART_RCV_INT_PRIORITY_IDX0             INT_PRIORITY_LEVEL1
#define DRV_USART_RCV_INT_SUB_PRIORITY_IDX0         INT_SUBPRIORITY_LEVEL0
#define DRV_USART_ERR_INT_VECTOR_IDX0               INT_VECTOR_UART1_FAULT
#define DRV_USART_ERR_INT_PRIORITY_IDX0             INT_PRIORITY_LEVEL1
#define DRV_USART_ERR_INT_SUB_PRIORITY_IDX0         INT_SUBPRIORITY_LEVEL0

#define DRV_USART_XMIT_QUEUE_SIZE_IDX0              20
#define DRV_USART_RCV_QUEUE_SIZE_IDX0               2


#define DRV_USART_POWER_STATE_IDX0                  SYS_MODULE_POWER_RUN_FULL

#define DRV_USART_PERIPHERAL_ID_IDX1                USART_ID_2
#define DRV_USART_OPER_MODE_IDX1                    DRV_USART_OPERATION_MODE_NORMAL
#define DRV_USART_OPER_MODE_DATA_IDX1               
#define DRV_USART_INIT_FLAG_WAKE_ON_START_IDX1      false
#define DRV_USART_INIT_FLAG_AUTO_BAUD_IDX1          false
#define DRV_USART_INIT_FLAG_STOP_IN_IDLE_IDX1       false
#define DRV_USART_INIT_FLAGS_IDX1                   0
#define DRV_USART_BRG_CLOCK_IDX1                    100000000
#define DRV_USART_BAUD_RATE_IDX1                    115200
#define DRV_USART_LINE_CNTRL_IDX1                   DRV_USART_LINE_CONTROL_8NONE1
#define DRV_USART_HANDSHAKE_MODE_IDX1               DRV_USART_HANDSHAKE_SIMPLEX
#define DRV_USART_LINES_ENABLE_IDX1                 USART_ENABLE_TX_RX_CTS_RTS_USED
#define DRV_USART_XMIT_INT_SRC_IDX1                 INT_SOURCE_USART_2_TRANSMIT
#define DRV_USART_RCV_INT_SRC_IDX1                  INT_SOURCE_USART_2_RECEIVE
#define DRV_USART_ERR_INT_SRC_IDX1                  INT_SOURCE_USART_2_ERROR
#define DRV_USART_XMIT_INT_VECTOR_IDX1              INT_VECTOR_UART2_TX
#define DRV_USART_XMIT_INT_PRIORITY_IDX1            INT_PRIORITY_LEVEL1
#define DRV_USART_XMIT_INT_SUB_PRIORITY_IDX1        INT_SUBPRIORITY_LEVEL0
#define DRV_USART_RCV_INT_VECTOR_IDX1               INT_VECTOR_UART2_RX
#define DRV_USART_RCV_INT_PRIORITY_IDX1             INT_PRIORITY_LEVEL1
#define DRV_USART_RCV_INT_SUB_PRIORITY_IDX1         INT_SUBPRIORITY_LEVEL0
#define DRV_USART_ERR_INT_VECTOR_IDX1               INT_VECTOR_UART2_FAULT
#define DRV_USART_ERR_INT_PRIORITY_IDX1             INT_PRIORITY_LEVEL1
#define DRV_USART_ERR_INT_SUB_PRIORITY_IDX1         INT_SUBPRIORITY_LEVEL0

#define DRV_USART_XMIT_QUEUE_SIZE_IDX1              2
#define DRV_USART_RCV_QUEUE_SIZE_IDX1               2


#define DRV_USART_POWER_STATE_IDX1                  SYS_MODULE_POWER_RUN_FULL

#define DRV_USART_PERIPHERAL_ID_IDX2                USART_ID_4
#define DRV_USART_OPER_MODE_IDX2                    DRV_USART_OPERATION_MODE_NORMAL
#define DRV_USART_OPER_MODE_DATA_IDX2               
#define DRV_USART_INIT_FLAG_WAKE_ON_START_IDX2      false
#define DRV_USART_INIT_FLAG_AUTO_BAUD_IDX2          false
#define DRV_USART_INIT_FLAG_STOP_IN_IDLE_IDX2       false
#define DRV_USART_INIT_FLAGS_IDX2                   0
#define DRV_USART_BRG_CLOCK_IDX2                    100000000
#define DRV_USART_BAUD_RATE_IDX2                    4800
#define DRV_USART_LINE_CNTRL_IDX2                   DRV_USART_LINE_CONTROL_8NONE1
#define DRV_USART_HANDSHAKE_MODE_IDX2               DRV_USART_HANDSHAKE_NONE
#define DRV_USART_LINES_ENABLE_IDX2                 USART_ENABLE_TX_RX_USED
#define DRV_USART_XMIT_INT_SRC_IDX2                 INT_SOURCE_USART_4_TRANSMIT
#define DRV_USART_RCV_INT_SRC_IDX2                  INT_SOURCE_USART_4_RECEIVE
#define DRV_USART_ERR_INT_SRC_IDX2                  INT_SOURCE_USART_4_ERROR
#define DRV_USART_XMIT_INT_PRIORITY_IDX2            INT_PRIORITY_LEVEL1
#define DRV_USART_XMIT_INT_SUB_PRIORITY_IDX2        INT_SUBPRIORITY_LEVEL0
#define DRV_USART_RCV_INT_VECTOR_IDX2               INT_VECTOR_UART4_RX
#define DRV_USART_RCV_INT_PRIORITY_IDX2             INT_PRIORITY_LEVEL1
#define DRV_USART_RCV_INT_SUB_PRIORITY_IDX2         INT_SUBPRIORITY_LEVEL0
#define DRV_USART_ERR_INT_VECTOR_IDX2               INT_VECTOR_UART4_FAULT
#define DRV_USART_ERR_INT_PRIORITY_IDX2             INT_PRIORITY_LEVEL1
#define DRV_USART_ERR_INT_SUB_PRIORITY_IDX2         INT_SUBPRIORITY_LEVEL0

#define DRV_USART_XMIT_QUEUE_SIZE_IDX2              20
#define DRV_USART_RCV_QUEUE_SIZE_IDX2               2


#define DRV_USART_POWER_STATE_IDX2                  SYS_MODULE_POWER_RUN_FULL

#define DRV_USART_PERIPHERAL_ID_IDX3                USART_ID_6
#define DRV_USART_OPER_MODE_IDX3                    DRV_USART_OPERATION_MODE_NORMAL
#define DRV_USART_OPER_MODE_DATA_IDX3               
#define DRV_USART_INIT_FLAG_WAKE_ON_START_IDX3      false
#define DRV_USART_INIT_FLAG_AUTO_BAUD_IDX3          false
#define DRV_USART_INIT_FLAG_STOP_IN_IDLE_IDX3       false
#define DRV_USART_INIT_FLAGS_IDX3                   0
#define DRV_USART_BRG_CLOCK_IDX3                    100000000
#define DRV_USART_BAUD_RATE_IDX3                    921000
#define DRV_USART_LINE_CNTRL_IDX3                   DRV_USART_LINE_CONTROL_8NONE1
#define DRV_USART_HANDSHAKE_MODE_IDX3               DRV_USART_HANDSHAKE_NONE
#define DRV_USART_LINES_ENABLE_IDX3                 USART_ENABLE_TX_RX_USED
#define DRV_USART_XMIT_INT_SRC_IDX3                 INT_SOURCE_USART_6_TRANSMIT
#define DRV_USART_RCV_INT_SRC_IDX3                  INT_SOURCE_USART_6_RECEIVE
#define DRV_USART_ERR_INT_SRC_IDX3                  INT_SOURCE_USART_6_ERROR
#define DRV_USART_XMIT_INT_VECTOR_IDX3              INT_VECTOR_UART6_TX
#define DRV_USART_XMIT_INT_PRIORITY_IDX3            INT_PRIORITY_LEVEL1
#define DRV_USART_XMIT_INT_SUB_PRIORITY_IDX3        INT_SUBPRIORITY_LEVEL0
#define DRV_USART_RCV_INT_VECTOR_IDX3               INT_VECTOR_UART6_RX
#define DRV_USART_RCV_INT_PRIORITY_IDX3             INT_PRIORITY_LEVEL1
#define DRV_USART_RCV_INT_SUB_PRIORITY_IDX3         INT_SUBPRIORITY_LEVEL0
#define DRV_USART_ERR_INT_VECTOR_IDX3               INT_VECTOR_UART6_FAULT
#define DRV_USART_ERR_INT_PRIORITY_IDX3             INT_PRIORITY_LEVEL1
#define DRV_USART_ERR_INT_SUB_PRIORITY_IDX3         INT_SUBPRIORITY_LEVEL0

#define DRV_USART_XMIT_QUEUE_SIZE_IDX3              10
#define DRV_USART_RCV_QUEUE_SIZE_IDX3               10


#define DRV_USART_POWER_STATE_IDX3                  SYS_MODULE_POWER_RUN_FULL

#define DRV_USART_QUEUE_DEPTH_COMBINED              68
/*** GLCD Driver Configuration ***/
#define  GFX_GLCD_CONFIG_CONTROL                       0x80000000
#define  GFX_GLCD_BACKGROUND_COLOR                     0xFFFFFF00
#define  GFX_GLCD_LAYERS                               3
/*** GLCD Layer 0 Configuration ***/
#define  GFX_GLCD_LAYER0_BASEADDR                      0xA8000000
#define  GFX_GLCD_LAYER0_DBL_BASEADDR                  0xA8465000
/*** GLCD Layer 1 Configuration ***/
#define  GFX_GLCD_LAYER1_BASEADDR                      0xA8177000
#define  GFX_GLCD_LAYER1_DBL_BASEADDR                  0xA85DC000
/*** GLCD Layer 2 Configuration ***/
#define  GFX_GLCD_LAYER2_BASEADDR                      0xA82EE000
#define  GFX_GLCD_LAYER2_DBL_BASEADDR                  0xA8753000


// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// Section: Bootloader Configuration
// *****************************************************************************

#define BOOTLOADER_LIVE_UPDATE_STATE_SAVE  1

/* APP_FLASH_BASE_ADDRESS and APP_FLASH_END_ADDRESS reserves program Flash for the application*/
/* Rule:
    1)The memory regions kseg0_program_mem, kseg0_boot_mem, exception_mem and
    kseg1_boot_mem of the application linker script must fall with in APP_FLASH_BASE_ADDRESS
    and APP_FLASH_END_ADDRESS

    2)The base address and end address must align on boundaries according to the flash page size */
#define FLASH_ID_STRUCT_SIZE            0x10

#define FLASH_ID_CHECKSUM_START         0xDEADBEEF
#define FLASH_ID_CHECKSUM_END           0xBEEFDEAD
#define FLASH_ID_CHECKSUM_CLR           0xFFFFFFFF

#define LOWER_FLASH_BASE_ADDRESS        (0x9D000000)
#define LOWER_FLASH_ID_BASE_ADDRESS     (LOWER_FLASH_BASE_ADDRESS)
#define LOWER_FLASH_ID_END_ADDRESS      (LOWER_FLASH_ID_BASE_ADDRESS + FLASH_ID_STRUCT_SIZE)
#define LOWER_FLASH_APP_BASE_ADDRESS    (LOWER_FLASH_ID_END_ADDRESS)
#define LOWER_FLASH_APP_END_ADDRESS     (LOWER_FLASH_ID_BASE_ADDRESS + (0x200000 / 2) - 1)

#define UPPER_FLASH_BASE_ADDRESS        (LOWER_FLASH_APP_END_ADDRESS + 1)
#define UPPER_FLASH_ID_BASE_ADDRESS     (UPPER_FLASH_BASE_ADDRESS)
#define UPPER_FLASH_ID_END_ADDRESS      (UPPER_FLASH_ID_BASE_ADDRESS + FLASH_ID_STRUCT_SIZE)
#define UPPER_FLASH_APP_BASE_ADDRESS    (UPPER_FLASH_ID_END_ADDRESS)
#define UPPER_FLASH_APP_END_ADDRESS     (UPPER_FLASH_ID_BASE_ADDRESS + (0x200000 / 2) - 1)

#define APP_FLASH_BASE_ADDRESS          (LOWER_FLASH_APP_BASE_ADDRESS)

#define APP_FLASH_END_ADDRESS           (LOWER_FLASH_APP_END_ADDRESS)

/* Address of  the Flash from where the application starts executing */
/* Rule: Set APP_FLASH_BASE_ADDRESS to _RESET_ADDR value of application linker script*/
#define APP_RESET_ADDRESS               (APP_FLASH_BASE_ADDRESS)
#define BOOTLOADER_IMAGE_FILE_NAME      "image.hex"

// *****************************************************************************
// Section: Bootloader NVM Driver Configuration
// *****************************************************************************
/* NVM Driver Flash Memory row and page size in bytes */
#define USE_PAGE_ERASE  0
#define USE_QUAD_WORD_WRITE 1

/*** OSAL Configuration ***/
#define OSAL_USE_RTOS          9

/*** USB Driver Configuration ***/


/* Disable Device Support */
#define DRV_USBHS_DEVICE_SUPPORT      false

/* Enables Host Support */
#define DRV_USBHS_HOST_SUPPORT      true

/* Maximum USB driver instances */
#define DRV_USBHS_INSTANCES_NUMBER    1

/* Interrupt mode enabled */
#define DRV_USBHS_INTERRUPT_MODE      true


/* Number of Endpoints used */
#define DRV_USBHS_ENDPOINTS_NUMBER    1







#define DRV_USBHS_HOST_NAK_LIMIT      2000
/* Provides Host pipes number */
#define DRV_USBHS_HOST_PIPES_NUMBER    10
#define DRV_USBHS_HOST_ATTACH_DEBOUNCE_DURATION 500
#define DRV_USBHS_HOST_RESET_DURATION 100
// *****************************************************************************
// *****************************************************************************
// Section: USB Device Layer Configuration
// *****************************************************************************
// *****************************************************************************
/* Provides Host pipes number */
#define USB_HOST_PIPES_NUMBER    10

// *****************************************************************************
// *****************************************************************************
// Section: USB Host Layer Configuration
// *****************************************************************************
// **************************************************************************
 
/* Total number of devices to be supported */
#define USB_HOST_DEVICES_NUMBER         1

/* Target peripheral list entries */
#define  USB_HOST_TPL_ENTRIES           1 

/* Maximum number of configurations supported per device */
#define USB_HOST_DEVICE_INTERFACES_NUMBER     5    

#define USB_HOST_CONTROLLERS_NUMBER           1

#define USB_HOST_TRANSFERS_NUMBER             10

/* Number of Host Layer Clients */
#define USB_HOST_CLIENTS_NUMBER               1   

/* Number of MSD Function driver instances in the application */
#define USB_HOST_MSD_INSTANCES_NUMBER         1

/* Number of Logical Units */
#define USB_HOST_SCSI_INSTANCES_NUMBER        1
#define USB_HOST_MSD_LUN_NUMBERS              1






// *****************************************************************************
/* BSP Configuration Options
*/
#define BSP_OSC_FREQUENCY 24000000


// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************
/*** Application Defined Pins ***/

/*** Functions for USB_ENABLE pin ***/
#define USB_ENABLEToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_10)
#define USB_ENABLEOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_10)
#define USB_ENABLEOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_10)
#define USB_ENABLEStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_10)
#define USB_ENABLEStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_10, Value)

/*** Functions for MOTOR_BRAKE pin ***/
#define MOTOR_BRAKEToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_3)
#define MOTOR_BRAKEOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_3)
#define MOTOR_BRAKEOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_3)
#define MOTOR_BRAKEStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_3)
#define MOTOR_BRAKEStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_3, Value)

/*** Functions for MOTOR_ENABLE pin ***/
#define MOTOR_ENABLEToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13)
#define MOTOR_ENABLEOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13)
#define MOTOR_ENABLEOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13)
#define MOTOR_ENABLEStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13)
#define MOTOR_ENABLEStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13, Value)

/*** Functions for MOTOR_DIRECTION pin ***/
#define MOTOR_DIRECTIONToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_8)
#define MOTOR_DIRECTIONOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_8)
#define MOTOR_DIRECTIONOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_8)
#define MOTOR_DIRECTIONStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_8)
#define MOTOR_DIRECTIONStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_8, Value)

/*** Functions for NIMH_ON pin ***/
#define NIMH_ONToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_11)
#define NIMH_ONOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_11)
#define NIMH_ONOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_11)
#define NIMH_ONStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_11)
#define NIMH_ONStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_11, Value)

/*** Functions for MOTOR_SPI_SS pin ***/
#define MOTOR_SPI_SSToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_12)
#define MOTOR_SPI_SSOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_12)
#define MOTOR_SPI_SSOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_12)
#define MOTOR_SPI_SSStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_12)
#define MOTOR_SPI_SSStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_12, Value)

/*** Functions for IH_ENABLE pin ***/
#define IH_ENABLEToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1)
#define IH_ENABLEOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1)
#define IH_ENABLEOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1)
#define IH_ENABLEStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1)
#define IH_ENABLEStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_1, Value)

/*** Functions for POWER_ENABLE pin ***/
#define POWER_ENABLEToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_9)
#define POWER_ENABLEOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_9)
#define POWER_ENABLEOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_9)
#define POWER_ENABLEStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_9)
#define POWER_ENABLEStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_9, Value)

/*** Functions for CHAMBER_ENABLE pin ***/
#define CHAMBER_ENABLEToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_1)
#define CHAMBER_ENABLEOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_1)
#define CHAMBER_ENABLEOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_1)
#define CHAMBER_ENABLEStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_1)
#define CHAMBER_ENABLEStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_1, Value)

/*** Functions for ESP32_HS pin ***/
#define ESP32_HSToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_11)
#define ESP32_HSOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_11)
#define ESP32_HSOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_11)
#define ESP32_HSStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_11)
#define ESP32_HSStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_11, Value)

/*** Functions for VIN_BYPASS pin ***/
#define VIN_BYPASSToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_3)
#define VIN_BYPASSOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_3)
#define VIN_BYPASSOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_3)
#define VIN_BYPASSStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_3)
#define VIN_BYPASSStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_3, Value)

/*** Functions for VBQ_BYPASS pin ***/
#define VBQ_BYPASSToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_2)
#define VBQ_BYPASSOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_2)
#define VBQ_BYPASSOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_2)
#define VBQ_BYPASSStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_2)
#define VBQ_BYPASSStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_2, Value)

/*** Functions for AUDIO_RESET pin ***/
#define AUDIO_RESETToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_J, PORTS_BIT_POS_2)
#define AUDIO_RESETOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_J, PORTS_BIT_POS_2)
#define AUDIO_RESETOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_J, PORTS_BIT_POS_2)
#define AUDIO_RESETStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_J, PORTS_BIT_POS_2)
#define AUDIO_RESETStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_J, PORTS_BIT_POS_2, Value)

/*** Functions for TOUCH_RESET pin ***/
#define TOUCH_RESETToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_12)
#define TOUCH_RESETOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_12)
#define TOUCH_RESETOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_12)
#define TOUCH_RESETStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_12)
#define TOUCH_RESETStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_12, Value)

/*** Functions for SPO2_RESET pin ***/
#define SPO2_RESETToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_9)
#define SPO2_RESETOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_9)
#define SPO2_RESETOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_9)
#define SPO2_RESETStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_9)
#define SPO2_RESETStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_9, Value)

/*** Functions for SPO2_ENABLE pin ***/
#define SPO2_ENABLEToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_5)
#define SPO2_ENABLEOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_5)
#define SPO2_ENABLEOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_5)
#define SPO2_ENABLEStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_5)
#define SPO2_ENABLEStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_5, Value)

/*** Functions for USB_OC pin ***/
#define USB_OCStateGet() PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_9)

/*** Functions for MUTE_SW pin ***/
#define MUTE_SWStateGet() PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_6)

/*** Functions for POWER_SW pin ***/
#define POWER_SWStateGet() PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_14)

/*** Functions for ACOK pin ***/
#define ACOKStateGet() PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_8)

/*** Functions for CHAMBER_OC pin ***/
#define CHAMBER_OCStateGet() PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_H, PORTS_BIT_POS_0)

/*** Functions for ESP32_INT pin ***/
#define ESP32_INTStateGet() PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_J, PORTS_BIT_POS_11)

/*** Functions for BAT_DETECT pin ***/
#define BAT_DETECTStateGet() PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_14)

/*** Functions for SPO2_FLG pin ***/
#define SPO2_FLGStateGet() PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_4)


/*** Application Instance 0 Configuration ***/
/*
IC driver model constants used by the application:
   
APP_IC_INDEX 				: Identifies the IC Driver instance to use
APP_TMR_INDEX				: Identifies the Timer Driver instance to use
APP_DRV)_IC_NUM_EDGES  		: Number of ICx edges that timer value is captured on
*/
#define APP_IC_INDEX       			0
#define APP_TMR_INDEX                0
#define APP_IC_NUM_EDGES      		32

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _SYSTEM_CONFIG_H
/*******************************************************************************
 End of File
*/
