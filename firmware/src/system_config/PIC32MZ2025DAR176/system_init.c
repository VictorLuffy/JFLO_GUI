/*******************************************************************************
  System Initialization File

  File Name:
    system_init.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources, such as the
    sysObj structure that contains the object handles to all the MPLAB Harmony
    module objects in the system.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"
#include "USB_Power.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
// <editor-fold defaultstate="collapsed" desc="Configuration Bits">

/*** DEVCFG0 ***/

#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx2
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   GAIN_LEVEL_3
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   GAIN_LEVEL_3
#pragma config POSCBOOST =  ON
#pragma config POSCFGAIN =  GAIN_G3
#pragma config POSCAGCDLY =  POSCAGCDLY1
#pragma config POSCAGCRNG =  RANGE_1X
#pragma config POSCAGC =  OFF
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/

#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    ON
#pragma config IESO =       ON
#pragma config POSCMOD =    EC
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS8192//1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF
/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_3
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_POSC
#pragma config FPLLMULT =   MUL_50
#pragma config FPLLODIV =   DIV_2
#pragma config VBATBOREN =  ON
#pragma config DSBOREN =    ON
#pragma config DSWDTPS =    DSPS32
#pragma config DSWDTOSC =   LPRC
#pragma config DSWDTEN =    OFF
#pragma config FDSEN =      ON
#pragma config UPLLFSEL =   FREQ_24MHZ
/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config EXTDDRSIZE = DDR_SIZE_32MB
#pragma config FMIIEN =     OFF
#pragma config FETHIO =     ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON

/*** DEVCFG4 ***/

#pragma config SWDTPS = SPS1048576

/*** BF1SEQ0 ***/

#pragma config TSEQ =       0x0000
#pragma config CSEQ =       0xffff
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="DRV_I2C Initialization Data">
// *****************************************************************************
/* I2C Driver Initialization Data
*/

const DRV_I2C_INIT drvI2C0InitData =
{
    .i2cId = DRV_I2C_PERIPHERAL_ID_IDX0,
    .i2cMode = DRV_I2C_OPERATION_MODE_IDX0,
    .portSCL = DRV_SCL_PORT_IDX0,
	.pinSCL  = DRV_SCL_PIN_POSITION_IDX0,
	.portSDA = DRV_SDA_PORT_IDX0,
	.pinSDA  = DRV_SDA_PIN_POSITION_IDX0,
    .baudRate = DRV_I2C_BAUD_RATE_IDX0,
    .busspeed = DRV_I2C_SLEW_RATE_CONTROL_IDX0,
    .buslevel = DRV_I2C_SMBus_SPECIFICATION_IDX0,
    .mstrInterruptSource = DRV_I2C_MASTER_INT_SRC_IDX0,
    .errInterruptSource = DRV_I2C_ERR_MZ_INT_SRC_IDX0,

};


const DRV_I2C_INIT drvI2C1InitData =
{
    .i2cId = DRV_I2C_PERIPHERAL_ID_IDX1,
    .i2cMode = DRV_I2C_OPERATION_MODE_IDX1,
    .portSCL = DRV_SCL_PORT_IDX1,
	.pinSCL  = DRV_SCL_PIN_POSITION_IDX1,
	.portSDA = DRV_SDA_PORT_IDX1,
	.pinSDA  = DRV_SDA_PIN_POSITION_IDX1,
    .buslevel = DRV_I2C_SMBus_SPECIFICATION_IDX1,
    .baudRate = DRV_I2C_BAUD_RATE_IDX1,
    .busspeed = DRV_I2C_SLEW_RATE_CONTROL_IDX1,
    .mstrInterruptSource = DRV_I2C_MASTER_INT_SRC_IDX1,
    .errInterruptSource = DRV_I2C_ERR_MZ_INT_SRC_IDX1,

};


const DRV_I2C_INIT drvI2C2InitData =
{
    .i2cId = DRV_I2C_PERIPHERAL_ID_IDX2,
    .i2cMode = DRV_I2C_OPERATION_MODE_IDX2,
    .portSCL = DRV_SCL_PORT_IDX2,
	.pinSCL  = DRV_SCL_PIN_POSITION_IDX2,
	.portSDA = DRV_SDA_PORT_IDX2,
	.pinSDA  = DRV_SDA_PIN_POSITION_IDX2,
    .baudRate = DRV_I2C_BAUD_RATE_IDX2,
    .busspeed = DRV_I2C_SLEW_RATE_CONTROL_IDX2,
    .buslevel = DRV_I2C_SMBus_SPECIFICATION_IDX2,
    .mstrInterruptSource = DRV_I2C_MASTER_INT_SRC_IDX2,
    .errInterruptSource = DRV_I2C_ERR_MZ_INT_SRC_IDX2,

};


const DRV_I2C_INIT drvI2C3InitData =
{
    .i2cId = DRV_I2C_PERIPHERAL_ID_IDX3,
    .i2cMode = DRV_I2C_OPERATION_MODE_IDX3,
    .portSCL = DRV_SCL_PORT_IDX3,
	.pinSCL  = DRV_SCL_PIN_POSITION_IDX3,
	.portSDA = DRV_SDA_PORT_IDX3,
	.pinSDA  = DRV_SDA_PIN_POSITION_IDX3,
    .baudRate = DRV_I2C_BAUD_RATE_IDX3,
    .busspeed = DRV_I2C_SLEW_RATE_CONTROL_IDX3,
    .buslevel = DRV_I2C_SMBus_SPECIFICATION_IDX3,
    .mstrInterruptSource = DRV_I2C_MASTER_INT_SRC_IDX3,
    .errInterruptSource = DRV_I2C_ERR_MZ_INT_SRC_IDX3,

};

// </editor-fold>
 // <editor-fold defaultstate="collapsed" desc="DRV_I2S Initialization Data">
/*** I2S Driver Initialization Data ***/
const DRV_I2S_INIT drvI2S0InitData =
{
    .moduleInit.value = DRV_I2S_POWER_STATE_IDX0,
    .spiID = DRV_I2S_PERIPHERAL_ID_IDX0, 
    .usageMode = DRV_I2S_USAGE_MODE_IDX0,
    .baudClock = SPI_BAUD_RATE_CLK_IDX0,
    .baud = DRV_I2S_BAUD_RATE,
    .clockMode = DRV_I2S_CLK_MODE_IDX0,
    .audioCommWidth = SPI_AUDIO_COMM_WIDTH_IDX0,
    .audioTransmitMode = SPI_AUDIO_TRANSMIT_MODE_IDX0,
    .inputSamplePhase = SPI_INPUT_SAMPLING_PHASE_IDX0,
    .protocolMode = DRV_I2S_AUDIO_PROTOCOL_MODE_IDX0,
    .queueSizeTransmit = QUEUE_SIZE_TX_IDX0,
    .queueSizeReceive = QUEUE_SIZE_RX_IDX0,
    .dmaChannelTransmit = DRV_I2S_TX_DMA_CHANNEL_IDX0,
    .txInterruptSource = DRV_I2S_TX_INT_SRC_IDX0,    
    .dmaInterruptTransmitSource = DRV_I2S_TX_DMA_SOURCE_IDX0,    
    .dmaChannelReceive = DMA_CHANNEL_NONE,
};





// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_SPI Initialization Data"> 
 /*** SPI Driver Initialization Data ***/
  /*** Index 0  ***/
 DRV_SPI_INIT drvSpi0InitData =
 {
    .spiId = DRV_SPI_SPI_ID_IDX0,
    .taskMode = DRV_SPI_TASK_MODE_IDX0,
    .spiMode = DRV_SPI_SPI_MODE_IDX0,
    .allowIdleRun = DRV_SPI_ALLOW_IDLE_RUN_IDX0,
    .spiProtocolType = DRV_SPI_SPI_PROTOCOL_TYPE_IDX0,
    .commWidth = DRV_SPI_COMM_WIDTH_IDX0,
    .baudClockSource = DRV_SPI_CLOCK_SOURCE_IDX0,
    .spiClk = DRV_SPI_SPI_CLOCK_IDX0,
    .baudRate = DRV_SPI_BAUD_RATE_IDX0,
    .bufferType = DRV_SPI_BUFFER_TYPE_IDX0,
    .clockMode = DRV_SPI_CLOCK_MODE_IDX0,
    .inputSamplePhase = DRV_SPI_INPUT_PHASE_IDX0,
    .txInterruptSource = DRV_SPI_TX_INT_SOURCE_IDX0,
    .rxInterruptSource = DRV_SPI_RX_INT_SOURCE_IDX0,
    .errInterruptSource = DRV_SPI_ERROR_INT_SOURCE_IDX0,
    .dummyByteValue = DRV_SPI_TRANSMIT_DUMMY_BYTE_VALUE_IDX0,
    .queueSize = DRV_SPI_QUEUE_SIZE_IDX0,
    .jobQueueReserveSize = DRV_SPI_RESERVED_JOB_IDX0,
 };
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_SQI Initialization Data">
/*** SQI Driver Initialization Data ***/
const DRV_SQI_INIT drvSqiInit =
{
    .sqiId = SQI_ID_0,
    .interruptSource = INT_SOURCE_SQI1,
    .enabledDevices = DRV_SQI_ENABLE_DEVICE_0,
    .clockDivider = DRV_SQI_CLK_DIV_4,
    .devCfg[0].spiMode = DRV_SQI_SPI_MODE_0,
    .devCfg[0].lsbFirst = false,
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_SST26 Initialization Data">
/*** SST26 FLASH Driver Initialization Data ***/
const DRV_SST26_INIT drvSst26InitData0 =
{
    .sqiDevice = 0,
};
// </editor-fold>
const DRV_TMR_INIT drvTmr6InitData =
{
    .moduleInit.sys.powerState = DRV_TMR_POWER_STATE_IDX6,
    .tmrId = DRV_TMR_PERIPHERAL_ID_IDX6,
    .clockSource = DRV_TMR_CLOCK_SOURCE_IDX6,
    .prescale = DRV_TMR_PRESCALE_IDX6,
    .mode = DRV_TMR_OPERATION_MODE_IDX6,
    .interruptSource = DRV_TMR_INTERRUPT_SOURCE_IDX6,
    .asyncWriteEnable = false,
};

// <editor-fold defaultstate="collapsed" desc="DRV_USART Initialization Data">

const DRV_USART_INIT drvUsart0InitData =
{
    .moduleInit.value = DRV_USART_POWER_STATE_IDX0,
    .usartID = DRV_USART_PERIPHERAL_ID_IDX0, 
    .mode = DRV_USART_OPER_MODE_IDX0,
    .flags = DRV_USART_INIT_FLAGS_IDX0,
    .brgClock = DRV_USART_BRG_CLOCK_IDX0,
    .lineControl = DRV_USART_LINE_CNTRL_IDX0,
    .baud = DRV_USART_BAUD_RATE_IDX0,
    .handshake = DRV_USART_HANDSHAKE_MODE_IDX0,
    .linesEnable = DRV_USART_LINES_ENABLE_IDX0,
    .interruptTransmit = DRV_USART_XMIT_INT_SRC_IDX0,
    .interruptReceive = DRV_USART_RCV_INT_SRC_IDX0,
    .interruptError = DRV_USART_ERR_INT_SRC_IDX0,
    .queueSizeTransmit = DRV_USART_XMIT_QUEUE_SIZE_IDX0,
    .queueSizeReceive = DRV_USART_RCV_QUEUE_SIZE_IDX0,
    .dmaChannelTransmit = DMA_CHANNEL_NONE,
    .dmaInterruptTransmit = DRV_USART_XMIT_INT_SRC_IDX0,    
    .dmaChannelReceive = DMA_CHANNEL_NONE,
    .dmaInterruptReceive = DRV_USART_RCV_INT_SRC_IDX0,    
};

const DRV_USART_INIT drvUsart1InitData =
{
    .moduleInit.value = DRV_USART_POWER_STATE_IDX1,
    .usartID = DRV_USART_PERIPHERAL_ID_IDX1, 
    .mode = DRV_USART_OPER_MODE_IDX1,
    .flags = DRV_USART_INIT_FLAGS_IDX1,
    .brgClock = DRV_USART_BRG_CLOCK_IDX1,
    .lineControl = DRV_USART_LINE_CNTRL_IDX1,
    .baud = DRV_USART_BAUD_RATE_IDX1,
    .handshake = DRV_USART_HANDSHAKE_MODE_IDX1,
    .linesEnable = DRV_USART_LINES_ENABLE_IDX1,
    .interruptTransmit = DRV_USART_XMIT_INT_SRC_IDX1,
    .interruptReceive = DRV_USART_RCV_INT_SRC_IDX1,
    .interruptError = DRV_USART_ERR_INT_SRC_IDX1,
    .queueSizeTransmit = DRV_USART_XMIT_QUEUE_SIZE_IDX1,
    .queueSizeReceive = DRV_USART_RCV_QUEUE_SIZE_IDX1,
    .dmaChannelTransmit = DMA_CHANNEL_NONE,
    .dmaInterruptTransmit = DRV_USART_XMIT_INT_SRC_IDX1,
    .dmaChannelReceive = DMA_CHANNEL_NONE,
    .dmaInterruptReceive= DRV_USART_RCV_INT_SRC_IDX1,
};

const DRV_USART_INIT drvUsart2InitData =
{
    .moduleInit.value = DRV_USART_POWER_STATE_IDX2,
    .usartID = DRV_USART_PERIPHERAL_ID_IDX2, 
    .mode = DRV_USART_OPER_MODE_IDX2,
    .flags = DRV_USART_INIT_FLAGS_IDX2,
    .brgClock = DRV_USART_BRG_CLOCK_IDX2,
    .lineControl = DRV_USART_LINE_CNTRL_IDX2,
    .baud = DRV_USART_BAUD_RATE_IDX2,
    .handshake = DRV_USART_HANDSHAKE_MODE_IDX2,
    .linesEnable = DRV_USART_LINES_ENABLE_IDX2,
    .interruptTransmit = DRV_USART_XMIT_INT_SRC_IDX2,
    .interruptReceive = DRV_USART_RCV_INT_SRC_IDX2,
    .interruptError = DRV_USART_ERR_INT_SRC_IDX2,
    .queueSizeTransmit = DRV_USART_XMIT_QUEUE_SIZE_IDX2,
    .queueSizeReceive = DRV_USART_RCV_QUEUE_SIZE_IDX2,
    .dmaChannelTransmit = DMA_CHANNEL_NONE,
    .dmaInterruptTransmit = DRV_USART_XMIT_INT_SRC_IDX2,
    .dmaChannelReceive = DMA_CHANNEL_NONE,
    .dmaInterruptReceive = DRV_USART_RCV_INT_SRC_IDX2,
};

const DRV_USART_INIT drvUsart3InitData =
{
    .moduleInit.value = DRV_USART_POWER_STATE_IDX3,
    .usartID = DRV_USART_PERIPHERAL_ID_IDX3, 
    .mode = DRV_USART_OPER_MODE_IDX3,
    .flags = DRV_USART_INIT_FLAGS_IDX3,
    .brgClock = DRV_USART_BRG_CLOCK_IDX3,
    .lineControl = DRV_USART_LINE_CNTRL_IDX3,
    .baud = DRV_USART_BAUD_RATE_IDX3,
    .handshake = DRV_USART_HANDSHAKE_MODE_IDX3,
    .linesEnable = DRV_USART_LINES_ENABLE_IDX3,
    .interruptTransmit = DRV_USART_XMIT_INT_SRC_IDX3,
    .interruptReceive = DRV_USART_RCV_INT_SRC_IDX3,
    .interruptError = DRV_USART_ERR_INT_SRC_IDX3,
    .queueSizeTransmit = DRV_USART_XMIT_QUEUE_SIZE_IDX3,
    .queueSizeReceive = DRV_USART_RCV_QUEUE_SIZE_IDX3,
    .dmaChannelTransmit = DMA_CHANNEL_NONE,
    .dmaInterruptTransmit = DRV_USART_XMIT_INT_SRC_IDX3,
    .dmaChannelReceive = DMA_CHANNEL_NONE,
    .dmaInterruptReceive = DRV_USART_RCV_INT_SRC_IDX3,
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USB Initialization Data">
/******************************************************
 * USB Driver Initialization
 ******************************************************/
const DRV_USBHS_INIT drvUSBHSInit =
{
    /* Interrupt Source for USB module */
    .interruptSource = INT_SOURCE_USB_1,

    /* Interrupt Source for USB module */
    .interruptSourceUSBDma = INT_SOURCE_USB_1_DMA,

    /* System module initialization */
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},

    /* Operation Mode */
    .operationMode = DRV_USBHS_OPMODE_HOST,

    /* Operation Speed */ 
    .operationSpeed = USB_SPEED_HIGH,
    
    /* Stop in idle */
    .stopInIdle = false,

    /* Suspend in sleep */
    .suspendInSleep = false,

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USBHS_ID_0,
    
    /* Root Hub Port indication */
    .portIndication = NULL,
        
     /* Power Enable */ 
    .portPowerEnable = USB_Power_Enable,
    
    /* Over Current detection */ 
    .portOverCurrentDetect = USB_Power_IsOverCurrent,
     
     /* Available Current */
    .rootHubAvailableCurrent = 500,
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************

/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Module Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="Bootloader Initialization Data">
/*** Bootloader Initialization Data ***/
 const BOOTLOADER_INIT BootloaderInitData =
{
    .drvType          = TYPE_USB_HOST,
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_COMMAND Initialization Data">
/*** System Command Initialization Data ***/

SYS_CMD_INIT sysCmdInit =
{
    .moduleInit = {0},
    .consoleCmdIOParam = SYS_CMD_SINGLE_CHARACTER_READ_CONSOLE_IO_PARAM,
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_CONSOLE Initialization Data">
/*** System Console Initialization Data ***/

SYS_MODULE_OBJ sysConsoleObjects[] = { SYS_MODULE_OBJ_INVALID };

/* Declared in console device implementation (sys_console_uart.c) */
extern SYS_CONSOLE_DEV_DESC consUsartDevDesc;
SYS_CONSOLE_INIT consUsartInit0 =
{
    .moduleInit = {0},
    .consDevDesc = &consUsartDevDesc,
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_DEBUG Initialization Data">
/*** System Debug Initialization Data ***/

SYS_DEBUG_INIT debugInit =
{
    .moduleInit = {0},
    .errorLevel = SYS_ERROR_FATAL
};
// </editor-fold>
//<editor-fold defaultstate="collapsed" desc="SYS_DMA Initialization Data">
/*** System DMA Initialization Data ***/

const SYS_DMA_INIT sysDmaInit =
{
	.sidl = SYS_DMA_SIDL_DISABLE,

};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_FS Initialization Data">
/*** File System Initialization Data ***/


const SYS_FS_MEDIA_MOUNT_DATA sysfsMountTable[SYS_FS_VOLUME_NUMBER] = 
{
    
    {
		.mountName = SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0,
		.devName   = SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0, 
		.mediaType = SYS_FS_MEDIA_TYPE_IDX0,
		.fsType   = SYS_FS_TYPE_IDX0   
    },
    
    {
		.mountName = SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0,
		.devName   = SYS_FS_MEDIA_IDX1_DEVICE_NAME_VOLUME_IDX0, 
		.mediaType = SYS_FS_MEDIA_TYPE_IDX1,
		.fsType   = SYS_FS_TYPE_IDX1   
    },



     
};




const SYS_FS_REGISTRATION_TABLE sysFSInit [ SYS_FS_MAX_FILE_SYSTEM_TYPE ] =
{
    {
        .nativeFileSystemType = FAT,
        .nativeFileSystemFunctions = &FatFsFunctions
    }
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_INPUT Initialization Data">
// *****************************************************************************
/* Input System Initialization Data
*/

const SYS_INPUT_Init sysInputInit =
{
    0
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_TMR Initialization Data">
/*** TMR Service Initialization Data ***/
const SYS_TMR_INIT sysTmrInitData =
{
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},
    .drvIndex = DRV_TMR_INDEX_6,
    .tmrFreq = 1000, 
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="USB Host Initialization Data"> 



const USB_HOST_TPL_ENTRY USBTPList[ 1 ] =
{
	
    TPL_INTERFACE_CLASS_SUBCLASS_PROTOCOL(0x08, 0x06, 0x50, NULL,  USB_HOST_MSD_INTERFACE) ,






};


const USB_HOST_HCD hcdTable = 
{
    .drvIndex = DRV_USBHS_INDEX_0,
    .hcdInterface = DRV_USBHS_HOST_INTERFACE
};

const USB_HOST_INIT usbHostInitData = 
{
    .nTPLEntries = 1 ,
    .tplList = (USB_HOST_TPL_ENTRY *)USBTPList,
    .hostControllerDrivers = (USB_HOST_HCD *)&hcdTable
    
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
/*******************************************************************************
  Function:
    void __pic32_tlb_init_ebi_sqi ( void )

  Summary:
    Override the default function in crt0.s to skip TLB initialization from
    application.
 */

void __pic32_tlb_init_ebi_sqi ( void ) {}

void SYS_ReinitializeI2C3()
{
    DRV_I2C_Deinitialize(sysObj.drvI2C2);
    sysObj.drvI2C2 = DRV_I2C_Initialize(DRV_I2C_INDEX_2, (SYS_MODULE_INIT *)&drvI2C2InitData);
}

/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
    See prototype in system/common/sys_module.h.
 */

void SYS_Initialize ( void* data )
{
    /* Core Processor Initialization */
    SYS_CLK_Initialize( NULL );
    SYS_DEVCON_Initialize(SYS_DEVCON_INDEX_0, (SYS_MODULE_INIT*)NULL);
    SYS_DEVCON_PerformanceConfig(SYS_CLK_SystemFrequencyGet());

    /* Board Support Package Initialization */
    BSP_Initialize();        

    /* Initialize Drivers */
    sysObj.drvI2C0 = DRV_I2C_Initialize(DRV_I2C_INDEX_0, (SYS_MODULE_INIT *)&drvI2C0InitData);
    sysObj.drvI2C1 = DRV_I2C_Initialize(DRV_I2C_INDEX_1, (SYS_MODULE_INIT *)&drvI2C1InitData);
    sysObj.drvI2C2 = DRV_I2C_Initialize(DRV_I2C_INDEX_2, (SYS_MODULE_INIT *)&drvI2C2InitData);
    sysObj.drvI2C3 = DRV_I2C_Initialize(DRV_I2C_INDEX_3, (SYS_MODULE_INIT *)&drvI2C3InitData);


    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C1_MASTER, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C1_MASTER, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C1_BUS, INT_DISABLE_INTERRUPT);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C1_BUS, INT_SUBPRIORITY_LEVEL0);

    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C2_MASTER, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C2_MASTER, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C2_BUS, INT_DISABLE_INTERRUPT);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C2_BUS, INT_SUBPRIORITY_LEVEL0);

    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C3_MASTER, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C3_MASTER, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C3_BUS, INT_DISABLE_INTERRUPT);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C3_BUS, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C4_MASTER, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C4_MASTER, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C4_BUS, INT_DISABLE_INTERRUPT);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C4_BUS, INT_SUBPRIORITY_LEVEL0);
    sysObj.drvI2S0 = DRV_I2S_Initialize(DRV_I2S_INDEX_0, (SYS_MODULE_INIT *)&drvI2S0InitData);


    sysObj.sysDma = SYS_DMA_Initialize((SYS_MODULE_INIT *)&sysDmaInit);
    SYS_INT_VectorPrioritySet(INT_VECTOR_DMA0, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_DMA0, INT_SUBPRIORITY_LEVEL0);

    SYS_INT_SourceEnable(INT_SOURCE_DMA_0);



    /* Initialize ADC */
    DRV_ADC_Initialize();

    /* Initialize the IC Driver */
    DRV_IC0_Initialize();
    /* Initialize the OC Driver */
    DRV_OC0_Initialize();
    DRV_OC1_Initialize();
    DRV_OC2_Initialize();
    DRV_OC3_Initialize();
    DRV_OC4_Initialize();
    /* Disable SDHC module */
	PLIB_POWER_PeripheralModuleDisable(POWER_ID_0, POWER_MODULE_SDHC);

    /*** SPI Driver Index 0 initialization***/

    SYS_INT_VectorPrioritySet(DRV_SPI_TX_INT_VECTOR_IDX0, DRV_SPI_TX_INT_PRIORITY_IDX0);
    SYS_INT_VectorSubprioritySet(DRV_SPI_TX_INT_VECTOR_IDX0, DRV_SPI_TX_INT_SUB_PRIORITY_IDX0);
    SYS_INT_VectorPrioritySet(DRV_SPI_RX_INT_VECTOR_IDX0, DRV_SPI_RX_INT_PRIORITY_IDX0);
    SYS_INT_VectorSubprioritySet(DRV_SPI_RX_INT_VECTOR_IDX0, DRV_SPI_RX_INT_SUB_PRIORITY_IDX0);
    SYS_INT_VectorPrioritySet(DRV_DRV_SPI_ERROR_INT_VECTOR_IDX0, DRV_SPI_ERROR_INT_PRIORITY_IDX0);
    SYS_INT_VectorSubprioritySet(DRV_DRV_SPI_ERROR_INT_VECTOR_IDX0, DRV_SPI_ERROR_INT_SUB_PRIORITY_IDX0);
    sysObj.spiObjectIdx0 = DRV_SPI_Initialize(DRV_SPI_INDEX_0, (const SYS_MODULE_INIT  * const)&drvSpi0InitData);
    /* Configure the Flash Controller Interrupt Priority */
    SYS_INT_VectorPrioritySet(INT_VECTOR_SQI1, INT_PRIORITY_LEVEL3);

    /* Configure the Flash Controller Interrupt Sub Priority */
    SYS_INT_VectorSubprioritySet(INT_VECTOR_SQI1, INT_SUBPRIORITY_LEVEL0);

    /* Initialize the SQI Driver */
    sysObj.drvSqi = DRV_SQI_Initialize(DRV_SQI_INDEX_0, (SYS_MODULE_INIT *)&drvSqiInit);
    /* Initialize the SST26 Driver */
    sysObj.drvSst26Obj0 = DRV_SST26_Initialize(DRV_SST26_INDEX_0, (SYS_MODULE_INIT *)&drvSst26InitData0);
    /*Initialize TMR0 */
    DRV_TMR0_Initialize();
    /*Initialize TMR1 */
    DRV_TMR1_Initialize();
    /*Initialize TMR2 */
    DRV_TMR2_Initialize();
    /*Initialize TMR3 */
    DRV_TMR3_Initialize();
    /*Initialize TMR4 */
    DRV_TMR4_Initialize();
    /*Initialize TMR5 */
    DRV_TMR5_Initialize();
    /*Initialize TMR6 */
//    DRV_TMR6_Initialize();
    /*Initialize TMR7 */
    DRV_TMR7_Initialize();
 
    sysObj.drvTmr6 = DRV_TMR_Initialize(DRV_TMR_INDEX_6, (SYS_MODULE_INIT *)&drvTmr6InitData);
    SYS_INT_VectorPrioritySet(INT_VECTOR_T8, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_T8, INT_SUBPRIORITY_LEVEL0);
    
 
     sysObj.drvUsart0 = DRV_USART_Initialize(DRV_USART_INDEX_0, (SYS_MODULE_INIT *)&drvUsart0InitData);
    sysObj.drvUsart1 = DRV_USART_Initialize(DRV_USART_INDEX_1, (SYS_MODULE_INIT *)&drvUsart1InitData);
    sysObj.drvUsart2 = DRV_USART_Initialize(DRV_USART_INDEX_2, (SYS_MODULE_INIT *)&drvUsart2InitData);
    sysObj.drvUsart3 = DRV_USART_Initialize(DRV_USART_INDEX_3, (SYS_MODULE_INIT *)&drvUsart3InitData);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART1_TX, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART1_TX, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART1_RX, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART1_RX, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART1_FAULT, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART1_FAULT, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART2_TX, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART2_TX, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART2_RX, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART2_RX, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART2_FAULT, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART2_FAULT, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART4_TX, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART4_TX, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART4_RX, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART4_RX, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART4_FAULT, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART4_FAULT, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART6_TX, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART6_TX, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART6_RX, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART6_RX, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART6_FAULT, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART6_FAULT, INT_SUBPRIORITY_LEVEL0);
 
    // initialize the GFX hardware abstraction layer
    GFX_Initialize();

    /* RTCC System Service Initialization Call */
    sysObj.sysRtcc = SYS_RTCC_Initialize( );
    /* Initialize USB Driver */ 
    sysObj.drvUSBObject = DRV_USBHS_Initialize(DRV_USBHS_INDEX_0, (SYS_MODULE_INIT *) &drvUSBHSInit);

    /* Set priority of USB interrupt source */
    SYS_INT_VectorPrioritySet(INT_VECTOR_USB1, INT_PRIORITY_LEVEL4);

    /* Set Sub-priority of USB interrupt source */
    SYS_INT_VectorSubprioritySet(INT_VECTOR_USB1, INT_SUBPRIORITY_LEVEL0);
    
    /* Set the priority of the USB DMA Interrupt */
    SYS_INT_VectorPrioritySet(INT_VECTOR_USB1_DMA, INT_PRIORITY_LEVEL4);

    /* Set Sub-priority of the USB DMA Interrupt */
    SYS_INT_VectorSubprioritySet(INT_VECTOR_USB1_DMA, INT_SUBPRIORITY_LEVEL0);
    

    /* Initialize System Services */
    SYS_PORTS_Initialize();

    /*** Command Service Initialization Code ***/
    SYS_CMD_Initialize((SYS_MODULE_INIT*)&sysCmdInit);
    sysObj.sysConsole0 = SYS_CONSOLE_Initialize(SYS_CONSOLE_INDEX_0, (SYS_MODULE_INIT *)&consUsartInit0);


    /*** Debug Service Initialization Code ***/
    sysObj.sysDebug = SYS_DEBUG_Initialize(SYS_DEBUG_INDEX_0, (SYS_MODULE_INIT*)&debugInit);

    /*** File System Service Initialization Code ***/
    SYS_FS_Initialize( (const void *) sysFSInit );
    SYS_INP_Init((SYS_MODULE_INIT*)&sysInputInit);


    /*** Interrupt Service Initialization Code ***/
    SYS_INT_Initialize();

    /*Setup the INT_SOURCE_EXTERNAL_3 and Enable it*/
    SYS_INT_VectorPrioritySet(INT_VECTOR_INT3, INT_PRIORITY_LEVEL5);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_INT3, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_ExternalInterruptTriggerSet(INT_EXTERNAL_INT_SOURCE3,INT_EDGE_TRIGGER_RISING);
    SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_3);

    /*Setup the INT_SOURCE_EXTERNAL_0 and Enable it*/
    SYS_INT_VectorPrioritySet(INT_VECTOR_INT0, INT_PRIORITY_LEVEL5);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_INT0, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_ExternalInterruptTriggerSet(INT_EXTERNAL_INT_SOURCE0,INT_EDGE_TRIGGER_FALLING);
    SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_0);




    SYS_MEMORY_Initialize();

    /*** TMR Service Initialization Code ***/
    sysObj.sysTmr  = SYS_TMR_Initialize(SYS_TMR_INDEX_0, (const SYS_MODULE_INIT  * const)&sysTmrInitData);

    /* Initialize Middleware */
     /* Initialize the Bootloader */
     Bootloader_Initialize(&BootloaderInitData);
 
    // initialize UI library
    LibAria_Initialize();

 
    /* Initialize the USB Host layer */
    sysObj.usbHostObject0 = USB_HOST_Initialize (( SYS_MODULE_INIT *)& usbHostInitData );

    /* Initialize the Application */
    APP_Initialize();
}


/*******************************************************************************
 End of File
*/

