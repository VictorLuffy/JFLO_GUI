/*******************************************************************************
 System Interrupts File

  File Name:
    system_interrupt.c

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the
    interrupt sub-system.

  Summary:
    This file contains source code for the interrupt vector functions in the
    system.

  Description:
    This file contains source code for the interrupt vector functions in the
    system.  It implements the system and part specific vector "stub" functions
    from which the individual "Tasks" functions are called for any modules
    executing interrupt-driven in the MPLAB Harmony system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    interrupt-driven in the system.  These handles are passed into the individual
    module "Tasks" functions to identify the instance of the module to maintain.
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

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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

#include "system/common/sys_common.h"
#include "app.h"
#include "system_definitions.h"
#include "../../Device/GT911.h"
#include "../../Device/ADC.h"
#include "../../Device/UART_2.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************


 
void IntHandlerDrvI2CMasterInstance0(void)
{
    DRV_I2C_Tasks(sysObj.drvI2C0);
}


void IntHandlerDrvI2CErrorInstance0(void)
{
    //SYS_ASSERT(false, "I2C Driver Instance 0 Error");
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_I2C_1_BUS);
}

     
   

 
void IntHandlerDrvI2CMasterInstance1(void)
{
    DRV_I2C_Tasks(sysObj.drvI2C1);
}


void IntHandlerDrvI2CErrorInstance1(void)
{
    //SYS_ASSERT(false, "I2C Driver Instance 1 Error");
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_I2C_2_BUS);
}


     
   
 
void IntHandlerDrvI2CMasterInstance2(void)
{
    DRV_I2C_Tasks(sysObj.drvI2C2);
}


void IntHandlerDrvI2CErrorInstance2(void)
{
    //SYS_ASSERT(false, "I2C Driver Instance 2 Error");
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_I2C_3_BUS);
}


     
   
 
void IntHandlerDrvI2CMasterInstance3(void)
{
    DRV_I2C_Tasks(sysObj.drvI2C3);
}


void IntHandlerDrvI2CErrorInstance3(void)
{
    //SYS_ASSERT(false, "I2C Driver Instance 3 Error");
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_I2C_4_BUS);
}

     
   

 




 void IntHandlerDrvSqi (void)
{
    DRV_SQI_Tasks(sysObj.drvSqi);
}
  
void IntHandlerDrvUsartTransmitInstance0(void)
{
    DRV_USART_TasksTransmit(sysObj.drvUsart0);
}
void IntHandlerDrvUsartReceiveInstance0(void)
{
    DRV_USART_TasksReceive(sysObj.drvUsart0);
}
void IntHandlerDrvUsartErrorInstance0(void)
{
    DRV_USART_TasksError(sysObj.drvUsart0);
}




void IntHandlerDrvUsartTransmitInstance1(void)
{
    DRV_USART_TasksTransmit(sysObj.drvUsart1);
}
void IntHandlerDrvUsartReceiveInstance1(void)
{
    DRV_USART_TasksReceive(sysObj.drvUsart1);
}
void IntHandlerDrvUsartErrorInstance1(void)
{
    DRV_USART_TasksError(sysObj.drvUsart1);
}




void IntHandlerDrvUsartTransmitInstance2(void)
{
    DRV_USART_TasksTransmit(sysObj.drvUsart2);
}
void IntHandlerDrvUsartReceiveInstance2(void)
{
    DRV_USART_TasksReceive(sysObj.drvUsart2);
}
void IntHandlerDrvUsartErrorInstance2(void)
{
    DRV_USART_TasksError(sysObj.drvUsart2);
}




void IntHandlerDrvUsartTransmitInstance3(void)
{
    DRV_USART_TasksTransmit(sysObj.drvUsart3);
}
void IntHandlerDrvUsartReceiveInstance3(void)
{
    DRV_USART_TasksReceive(sysObj.drvUsart3);
}
void IntHandlerDrvUsartErrorInstance3(void)
{
    DRV_USART_TasksError(sysObj.drvUsart3);

}
 

 
 

 


void IntHandlerSysDmaInstance0(void)
{          
    SYS_DMA_Tasks(sysObj.sysDma, DMA_CHANNEL_0);
}

void __ISR(_RTCC_VECTOR, ipl1AUTO) _IntHandlerSysRtcc (void)
{
    SYS_RTCC_Tasks(sysObj.sysRtcc);
}
 
/* If individual interrupts are not enabled, all data interrupts are handled in this ISR */
void IntHandlerDrvAdcCommonData(void) {
    //disable interrupt
    PLIB_INT_SourceDisable(INT_ID_0, INT_SOURCE_ADC_1);
    uint16_t data;
    
    //enter critical code
    UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    
    if (true == DRV_ADC_SamplesAvailable(ADC_CURRENT_SENSOR_1)) {
        data = (uint16_t)DRV_ADC_SamplesRead(ADC_CURRENT_SENSOR_1);
        ADC_UpdateData(ADC_CURRENT_SENSOR_1, data);
    }
    if (true == DRV_ADC_SamplesAvailable(ADC_CURRENT_SENSOR_2)) {
        data = (uint16_t)DRV_ADC_SamplesRead(ADC_CURRENT_SENSOR_2);
        ADC_UpdateData(ADC_CURRENT_SENSOR_2, data);
    }
    if (true == DRV_ADC_SamplesAvailable(ADC_LIGHT_SENSOR)) {
        data = (uint16_t)DRV_ADC_SamplesRead(ADC_LIGHT_SENSOR);
        ADC_UpdateData(ADC_LIGHT_SENSOR, data);
    }
    if (true == DRV_ADC_SamplesAvailable(ADC_VOLT_IH_SENSOR)) {
        data = (uint16_t)DRV_ADC_SamplesRead(ADC_VOLT_IH_SENSOR);
        ADC_UpdateData(ADC_VOLT_IH_SENSOR, data);
    }
    if (true == DRV_ADC_SamplesAvailable(ADC_VOLT_NIMH_MONITOR)) {
        data = (uint16_t)DRV_ADC_SamplesRead(ADC_VOLT_NIMH_MONITOR);
        ADC_UpdateData(ADC_VOLT_NIMH_MONITOR, data);
    }
    if (true == DRV_ADC_SamplesAvailable(ADC_VOLT_INPUT_MONITOR)) {
        data = (uint16_t)DRV_ADC_SamplesRead(ADC_VOLT_INPUT_MONITOR);
        ADC_UpdateData(ADC_VOLT_INPUT_MONITOR, data);
    }
    
    //exit critical code
    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
    
    /* Clear ADC Interrupt Flag of INT_SOURCE_ADC_1*/
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_ADC_1);
    
    //re-enable interrupt
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_ADC_1);
}
void IntHandlerExternalInterruptInstance0(void)
{
    GT911_RequestRead();
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_EXTERNAL_3);
}
void IntHandlerExternalInterruptInstance1(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_EXTERNAL_0);
}


void IntHandlerDrvTmrInstance0(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
}
void IntHandlerDrvTmrInstance1(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_3);
}
void IntHandlerDrvTmrInstance2(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_4);
}
void IntHandlerDrvTmrInstance3(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_5);
}
void IntHandlerDrvTmrInstance4(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_6);
}
void IntHandlerDrvTmrInstance5(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_7);
}
void IntHandlerDrvTmrInstance6(void)
{
    DRV_TMR_Tasks(sysObj.drvTmr6);
}
void IntHandlerDrvTmrInstance7(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_9);
}

void IntHandlerSPIRxInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}
void IntHandlerSPITxInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}
void IntHandlerSPIFaultInstance0(void)
{
    DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}


void IntHandler_USB_stub ( void )
{
    DRV_USBHS_Tasks_ISR(sysObj.drvUSBObject);
}

void IntHandlerUSBInstance0_USBDMA ( void )
{
     DRV_USBHS_Tasks_ISR_USBDMA(sysObj.drvUSBObject);
}
/*******************************************************************************
 End of File
 */
