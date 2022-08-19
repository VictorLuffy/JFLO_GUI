/*******************************************************************************
 System Interrupt Source File

  File Name:
    sys_interrupt_a.S

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the 
    interrupt sub-system.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2014 released Microchip Technology Inc.  All rights reserved.

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
/*
*********************************************************************************************************
*                                           INCLUDES
*********************************************************************************************************
*/
#include <xc.h>



#include "ISR_Support.h"

/* ADC Interrupt */
   .extern  IntHandlerDrvAdcCommonData

   .section	.vector_44,code, keep
   .equ     __vector_dispatch_44, IntVectorDrvAdc
   .global  __vector_dispatch_44
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvAdc

IntVectorDrvAdc:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvAdcCommonData
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvAdc




/* External Interrupt Instance 0 Interrupt */
   .extern  IntHandlerExternalInterruptInstance0

   .section	.vector_23,code, keep
   .equ     __vector_dispatch_23, IntVectorExternalInterruptInstance0
   .global  __vector_dispatch_23
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorExternalInterruptInstance0

IntVectorExternalInterruptInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerExternalInterruptInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorExternalInterruptInstance0


/* External Interrupt Instance 1 Interrupt */
   .extern  IntHandlerExternalInterruptInstance1
 
   .section	.vector_3,code, keep
   .equ     __vector_dispatch_3, IntVectorExternalInterruptInstance1
   .global  __vector_dispatch_3
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorExternalInterruptInstance1

IntVectorExternalInterruptInstance1:
    portSAVE_CONTEXT
    la    s6,  IntHandlerExternalInterruptInstance1
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorExternalInterruptInstance1


 

/* TMR Instance 0 Interrupt */
   .extern  IntHandlerDrvTmrInstance0

   .section	.vector_9,code, keep
   .equ     __vector_dispatch_9, IntVectorDrvTmrInstance0
   .global  __vector_dispatch_9
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance0

IntVectorDrvTmrInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvTmrInstance0


/* TMR Instance 1 Interrupt */
   .extern  IntHandlerDrvTmrInstance1

   .section	.vector_14,code, keep
   .equ     __vector_dispatch_14, IntVectorDrvTmrInstance1
   .global  __vector_dispatch_14
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance1

IntVectorDrvTmrInstance1:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance1
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvTmrInstance1


/* TMR Instance 2 Interrupt */
   .extern  IntHandlerDrvTmrInstance2

   .section	.vector_19,code, keep
   .equ     __vector_dispatch_19, IntVectorDrvTmrInstance2
   .global  __vector_dispatch_19
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance2

IntVectorDrvTmrInstance2:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance2
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvTmrInstance2


/* TMR Instance 3 Interrupt */
   .extern  IntHandlerDrvTmrInstance3

   .section	.vector_24,code, keep
   .equ     __vector_dispatch_24, IntVectorDrvTmrInstance3
   .global  __vector_dispatch_24
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance3

IntVectorDrvTmrInstance3:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance3
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvTmrInstance3


/* TMR Instance 4 Interrupt */
   .extern  IntHandlerDrvTmrInstance4

   .section	.vector_28,code, keep
   .equ     __vector_dispatch_28, IntVectorDrvTmrInstance4
   .global  __vector_dispatch_28
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance4

IntVectorDrvTmrInstance4:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance4
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvTmrInstance4


/* TMR Instance 5 Interrupt */
   .extern  IntHandlerDrvTmrInstance5

   .section	.vector_32,code, keep
   .equ     __vector_dispatch_32, IntVectorDrvTmrInstance5
   .global  __vector_dispatch_32
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance5

IntVectorDrvTmrInstance5:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance5
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvTmrInstance5


/* TMR Instance 6 Interrupt */
   .extern  IntHandlerDrvTmrInstance6

   .section	.vector_36,code, keep
   .equ     __vector_dispatch_36, IntVectorDrvTmrInstance6
   .global  __vector_dispatch_36
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance6

IntVectorDrvTmrInstance6:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance6
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvTmrInstance6


/* TMR Instance 7 Interrupt */
   .extern  IntHandlerDrvTmrInstance7

   .section	.vector_40,code, keep
   .equ     __vector_dispatch_40, IntVectorDrvTmrInstance7
   .global  __vector_dispatch_40
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance7

IntVectorDrvTmrInstance7:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance7
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvTmrInstance7




/* USART Instance 0 Interrupt */

   .extern  IntHandlerDrvUsartReceiveInstance0

   .section	.vector_113,code, keep
   .equ     __vector_dispatch_113, IntVectorDrvUsartReceiveInstance0
   .global  __vector_dispatch_113
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartReceiveInstance0

IntVectorDrvUsartReceiveInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartReceiveInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartReceiveInstance0


   .extern  IntHandlerDrvUsartTransmitInstance0

   .section	.vector_114,code, keep
   .equ     __vector_dispatch_114, IntVectorDrvUsartTransmitInstance0
   .global  __vector_dispatch_114
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartTransmitInstance0

IntVectorDrvUsartTransmitInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartTransmitInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartTransmitInstance0


   .extern  IntHandlerDrvUsartErrorInstance0

   .section	.vector_112,code, keep
   .equ     __vector_dispatch_112, IntVectorDrvUsartErrorInstance0
   .global  __vector_dispatch_112
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartErrorInstance0

IntVectorDrvUsartErrorInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartErrorInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartErrorInstance0



/* USART Instance 1 Interrupt */

   .extern  IntHandlerDrvUsartReceiveInstance1

   .section	.vector_146,code, keep
   .equ     __vector_dispatch_146, IntVectorDrvUsartReceiveInstance1
   .global  __vector_dispatch_146
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartReceiveInstance1

IntVectorDrvUsartReceiveInstance1:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartReceiveInstance1
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartReceiveInstance1


   .extern  IntHandlerDrvUsartTransmitInstance1

   .section	.vector_147,code, keep
   .equ     __vector_dispatch_147, IntVectorDrvUsartTransmitInstance1
   .global  __vector_dispatch_147
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartTransmitInstance1

IntVectorDrvUsartTransmitInstance1:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartTransmitInstance1
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartTransmitInstance1


   .extern  IntHandlerDrvUsartErrorInstance1

   .section	.vector_145,code, keep
   .equ     __vector_dispatch_145, IntVectorDrvUsartErrorInstance1
   .global  __vector_dispatch_145
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartErrorInstance1

IntVectorDrvUsartErrorInstance1:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartErrorInstance1
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartErrorInstance1



/* USART Instance 2 Interrupt */

   .extern  IntHandlerDrvUsartReceiveInstance2

   .section	.vector_171,code, keep
   .equ     __vector_dispatch_171, IntVectorDrvUsartReceiveInstance2
   .global  __vector_dispatch_171
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartReceiveInstance2

IntVectorDrvUsartReceiveInstance2:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartReceiveInstance2
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartReceiveInstance2


   .extern  IntHandlerDrvUsartTransmitInstance2

   .section	.vector_172,code, keep
   .equ     __vector_dispatch_172, IntVectorDrvUsartTransmitInstance2
   .global  __vector_dispatch_172
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartTransmitInstance2

IntVectorDrvUsartTransmitInstance2:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartTransmitInstance2
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartTransmitInstance2


   .extern  IntHandlerDrvUsartErrorInstance2

   .section	.vector_170,code, keep
   .equ     __vector_dispatch_170, IntVectorDrvUsartErrorInstance2
   .global  __vector_dispatch_170
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartErrorInstance2

IntVectorDrvUsartErrorInstance2:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartErrorInstance2
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartErrorInstance2



/* USART Instance 3 Interrupt */

   .extern  IntHandlerDrvUsartReceiveInstance3

   .section	.vector_189,code, keep
   .equ     __vector_dispatch_189, IntVectorDrvUsartReceiveInstance3
   .global  __vector_dispatch_189
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartReceiveInstance3

IntVectorDrvUsartReceiveInstance3:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartReceiveInstance3
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartReceiveInstance3


   .extern  IntHandlerDrvUsartTransmitInstance3

   .section	.vector_190,code, keep
   .equ     __vector_dispatch_190, IntVectorDrvUsartTransmitInstance3
   .global  __vector_dispatch_190
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartTransmitInstance3

IntVectorDrvUsartTransmitInstance3:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartTransmitInstance3
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartTransmitInstance3


   .extern  IntHandlerDrvUsartErrorInstance3

   .section	.vector_188,code, keep
   .equ     __vector_dispatch_188, IntVectorDrvUsartErrorInstance3
   .global  __vector_dispatch_188
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartErrorInstance3

IntVectorDrvUsartErrorInstance3:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartErrorInstance3
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvUsartErrorInstance3



/* SPI Instance 0 Interrupt */
   .extern  IntHandlerSPIRxInstance0

   .section	.vector_155,code, keep
   .equ     __vector_dispatch_155, IntVectorSPIRxInstance0
   .global  __vector_dispatch_155
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorSPIRxInstance0

IntVectorSPIRxInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerSPIRxInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorSPIRxInstance0


   .extern  IntHandlerSPITxInstance0

   .section	.vector_156,code, keep
   .equ     __vector_dispatch_156, IntVectorSPITxInstance0
   .global  __vector_dispatch_156
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorSPITxInstance0

IntVectorSPITxInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerSPITxInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorSPITxInstance0


   .extern  IntHandlerSPIFaultInstance0

   .section	.vector_154,code, keep
   .equ     __vector_dispatch_154, IntVectorSPIFaultInstance0
   .global  __vector_dispatch_154
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorSPIFaultInstance0

IntVectorSPIFaultInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerSPIFaultInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorSPIFaultInstance0


/* SQI Interrupt */
   .extern  IntHandlerDrvSqi

   .section	.vector_169,code, keep
   .equ     __vector_dispatch_169, IntVectorDrvSqi
   .global  __vector_dispatch_169
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvSqi

IntVectorDrvSqi:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvSqi
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvSqi



/* Sytem DMA Instance 0 Interrupt */
   .extern  IntHandlerSysDmaInstance0

   .section	.vector_134,code, keep
   .equ     __vector_dispatch_134, IntVectorSysDmaInstance0
   .global  __vector_dispatch_134
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorSysDmaInstance0

IntVectorSysDmaInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerSysDmaInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorSysDmaInstance0



/* I2C Instance 0 Interrupt */

   .extern  IntHandlerDrvI2CMasterInstance0

   .section	.vector_117,code, keep
   .equ     __vector_dispatch_117, IntVectorDrvI2CMasterInstance0
   .global  __vector_dispatch_117
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvI2CMasterInstance0

IntVectorDrvI2CMasterInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvI2CMasterInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvI2CMasterInstance0


   .extern  IntHandlerDrvI2CErrorInstance0

   .section	.vector_115,code, keep
   .equ     __vector_dispatch_115, IntVectorDrvI2CErrorInstance0
   .global  __vector_dispatch_115
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvI2CErrorInstance0

IntVectorDrvI2CErrorInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvI2CErrorInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvI2CErrorInstance0



/* I2C Instance 1 Interrupt */

   .extern  IntHandlerDrvI2CMasterInstance1

   .section	.vector_150,code, keep
   .equ     __vector_dispatch_150, IntVectorDrvI2CMasterInstance1
   .global  __vector_dispatch_150
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvI2CMasterInstance1

IntVectorDrvI2CMasterInstance1:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvI2CMasterInstance1
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvI2CMasterInstance1


   .extern  IntHandlerDrvI2CErrorInstance1

   .section	.vector_148,code, keep
   .equ     __vector_dispatch_148, IntVectorDrvI2CErrorInstance1
   .global  __vector_dispatch_148
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvI2CErrorInstance1

IntVectorDrvI2CErrorInstance1:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvI2CErrorInstance1
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvI2CErrorInstance1



/* I2C Instance 2 Interrupt */

   .extern  IntHandlerDrvI2CMasterInstance2

   .section	.vector_162,code, keep
   .equ     __vector_dispatch_162, IntVectorDrvI2CMasterInstance2
   .global  __vector_dispatch_162
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvI2CMasterInstance2

IntVectorDrvI2CMasterInstance2:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvI2CMasterInstance2
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvI2CMasterInstance2


   .extern  IntHandlerDrvI2CErrorInstance2

   .section	.vector_160,code, keep
   .equ     __vector_dispatch_160, IntVectorDrvI2CErrorInstance2
   .global  __vector_dispatch_160
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvI2CErrorInstance2

IntVectorDrvI2CErrorInstance2:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvI2CErrorInstance2
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvI2CErrorInstance2



/* I2C Instance 3 Interrupt */

   .extern  IntHandlerDrvI2CMasterInstance3

   .section	.vector_175,code, keep
   .equ     __vector_dispatch_175, IntVectorDrvI2CMasterInstance3
   .global  __vector_dispatch_175
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvI2CMasterInstance3

IntVectorDrvI2CMasterInstance3:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvI2CMasterInstance3
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvI2CMasterInstance3


   .extern  IntHandlerDrvI2CErrorInstance3

   .section	.vector_173,code, keep
   .equ     __vector_dispatch_173, IntVectorDrvI2CErrorInstance3
   .global  __vector_dispatch_173
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvI2CErrorInstance3

IntVectorDrvI2CErrorInstance3:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvI2CErrorInstance3
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorDrvI2CErrorInstance3



/* USB Device Interrupt */
   .extern  IntHandler_USB_stub

   .section	.vector_132,code, keep
   .equ     __vector_dispatch_132, IntVector_USB_stub
   .global  __vector_dispatch_132
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVector_USB_stub

IntVector_USB_stub:
    portSAVE_CONTEXT
    la    s6,  IntHandler_USB_stub
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVector_USB_stub


   .extern  IntHandlerUSBInstance0_USBDMA

   .section	.vector_133,code, keep
   .equ     __vector_dispatch_133, IntVectorUSBInstance0_USBDMA
   .global  __vector_dispatch_133
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorUSBInstance0_USBDMA

IntVectorUSBInstance0_USBDMA:
    portSAVE_CONTEXT
    la    s6,  IntHandlerUSBInstance0_USBDMA
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	IntVectorUSBInstance0_USBDMA



/*******************************************************************************
 End of File
 */

