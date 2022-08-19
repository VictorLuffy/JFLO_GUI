/* ************************************************************************** */
/** @file [CommandProcessor.c]
 *  @brief { This file contains source code necessary to control and maintain 
 * Command Processor such as: adding a command to the Command Processor and handle
 * Callback function from Command Processor }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include "system_config.h"
#include "system_definitions.h"
#include "CommandProcessor.h"
#include "MotorTask.h"
#include "HeaterTask.h"


/** @brief local functions */
static int CMD_MotorStart (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int CMD_MotorStop (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int CMD_MotorSpeed (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);

static int CMD_SettingFlow (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int CMD_SettingTemperature (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int CMD_SettingHumidity (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);


/** @brief Command table, include command string format, the corresponding call 
 * back function for that command and the short description accordingly */
static const SYS_CMD_DESCRIPTOR    s_commandTable[]=
{
    {"motor-start",         CMD_MotorStart,                     "Start motor at minimum speed (6000 rpm) or indicated speed"},
    {"motor-stop",          CMD_MotorStop,                      "Start motor from spinning"},
    {"motor-speed",         CMD_MotorSpeed,                     "Speed of the motor, from 6000 rpm to 30,000 rpm"},
    
    {"setting-flow",        CMD_SettingFlow,                    "Flow rate setting, from 5 LPM to 40 LPM"},
    {"setting-temp",        CMD_SettingTemperature,             "Temperature setting, from 20 to 60 degreeC"},
    {"setting-hum",         CMD_SettingHumidity,                "Humidity setting, from 10% (TBD) to 100% (TBD)"},
};


/** @brief The callback function according with command "motor-start". This function
 * will be called automatically by Command Processor when a "motor-start" command
 * is detected from console terminal
 * This function may send a command o Flow Control Task to start Motor and set its 
 * speed if the command include speed information
 *  @param [in]     SYS_CMD_DEVICE_NODE* pCmdIO    device node
 *                  int argc    argument counter
 *                  char** argv argument vector, contain speed information
 *  @param [out]    None
 *  @return  int    no used
 */
static int CMD_MotorStart (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv) {
    int32_t speed = 0;
    if ((argc == 2) && (*argv[1] != NULL)) {
        //get speed information
        char* speedStr = argv[1];
        
        //convert speed from string to binary
        int num = strtol(speedStr, (char **)NULL, 10);
        if (num == HUGE_VAL) {
            /* Could not convert. */
        }
        else {
            speed = num;
        }
    }
    
    MOTOR_CTRL_EVENT_t event = {.id = eMotorStartId, .iData = speed};
    //send event to Flow Control Task to start motor
    MotorTask_SendEvent(event);
    
    SYS_CMD_MESSAGE(" --> command accepted");
}

/** @brief The callback function according with command "motor-stop". This function
 * will be called automatically by Command Processor when a "motor-stop" command
 * is detected from console terminal
 * This function may send a command o Flow Control Task to stop Motor 
 *  @param [in]     SYS_CMD_DEVICE_NODE* pCmdIO    device node
 *                  int argc    argument counter
 *                  char** argv argument vector
 *  @param [out]    None
 *  @return  int    no used
 */
static int CMD_MotorStop (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv) {

    SYS_PRINT("Stop motor\n");
    //send event to Flow Control Task to start motor
    MOTOR_CTRL_EVENT_t event = {.id = eMotorStopId};
    
    //send event to Flow Control Task to start motor
    MotorTask_SendEvent(event);

    SYS_CMD_MESSAGE(" --> command accepted");
}

/** @brief The callback function according with command "motor-speed". This function
 * will be called automatically by Command Processor when a "motor-speed" command
 * is detected from console terminal
 * This function may send a command o Flow Control Task to set Motor speed
 *  @param [in]     SYS_CMD_DEVICE_NODE* pCmdIO    device node
 *                  int argc    argument counter
 *                  char** argv argument vector, contain speed information
 *  @param [out]    None
 *  @return  int    no used
 */
static int CMD_MotorSpeed (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv) {
    int32_t speed = 0;
    if ((argc == 2) && (*argv[1] != NULL)) {
        //get speed information
        char* speedStr = argv[1];
        
        //convert speed from string to binary
        int num = strtol(speedStr, (char **)NULL, 10);
        if (num == HUGE_VAL) {
            /* Could not convert. */
        }
        else {
            speed = num;
        }
    }
    
    //send event to Flow Control Task to start motor
    MOTOR_CTRL_EVENT_t event = {.id = eMotorSpeedId, .iData = speed};
    MotorTask_SendEvent(event);
    
    SYS_CMD_MESSAGE(" --> command accepted");
}

/** @brief The callback function according with command "setting-flow". This function
 * will be called automatically by Command Processor when a "setting-flow" command
 * is detected from console terminal
 * This function may send a command o Flow Control Task to set expected flow
 *  @param [in]     SYS_CMD_DEVICE_NODE* pCmdIO    device node
 *                  int argc    argument counter
 *                  char** argv argument vector, contain flow rate information
 *  @param [out]    None
 *  @return  int    no used
 */
static int CMD_SettingFlow (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv){
    float flow = 0;
    if ((argc == 2) && (*argv[1] != NULL)) {
        //get speed information
        char* speedStr = argv[1];
        
        //convert speed from string to binary
        float num = strtof(speedStr, (char **)NULL);
        if (num == HUGE_VAL) {
            /* Could not convert. */
        }
        else {
            flow = num;
        }
    }
    
    //send event to Flow Control Task to start motor
    MOTOR_CTRL_EVENT_t event = {.id = eMotorFlowId, .fData = flow};
    MotorTask_SendEvent(event);
    
    SYS_CMD_MESSAGE(" --> command accepted");
}

/** @brief The callback function according with command "setting-hum". This function
 * will be called automatically by Command Processor when a "setting-hum" command
 * is detected from console terminal
 * This function may send a command o Heater Control Task to set expected Humidity
 *  @param [in]     SYS_CMD_DEVICE_NODE* pCmdIO    device node
 *                  int argc    argument counter
 *                  char** argv argument vector, contain humidity setting information
 *  @param [out]    None
 *  @return  int    no used
 */
static int CMD_SettingHumidity (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv){
    if (*argv[1] != 0) {
        SYS_CMD_MESSAGE(" --> command accepted");
    }
    else {
        SYS_CMD_MESSAGE(" --> command ignored");
    }
}

/** @brief The callback function according with command "setting-temp". This function
 * will be called automatically by Command Processor when a "setting-temp" command
 * is detected from console terminal
 * This function may send a command o Heater Control Task to set expected Temperature
 *  @param [in]     SYS_CMD_DEVICE_NODE* pCmdIO    device node
 *                  int argc    argument counter
 *                  char** argv argument vector, contain temperature setting information
 *  @param [out]    None
 *  @return  int    no used
 */
static int CMD_SettingTemperature (SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv){
    float temperature = 0;
    if ((argc == 2) && (*argv[1] != NULL)) {
        //get temperature information
        char* tempStr = argv[1];
        
        //convert speed from string to binary
        float num = strtof(tempStr, (char **)NULL);
        if (num == HUGE_VAL) {
            /* Could not convert. */
        }
        else {
            temperature = num;
        }
    }
    
    //send event to Flow Control Task to start motor
    HEATER_CTRL_EVENT_t event = {.id = eHeaterTempId, .fData = temperature};
    HeaterTask_SendEvent(event);
    
    SYS_CMD_MESSAGE(" --> command accepted");
}


/** @brief Initialize Command Processor by adding user defined command group to
 * Command Processor system 
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void CmdProcessor_Initialize ( void )
{
    SYS_CMD_ADDGRP(s_commandTable, sizeof(s_commandTable)/sizeof(*s_commandTable), "app", ": app commands");
}


/** @brief Maintain Command Processor system
 * This function should be called repeatedly faster than UART console baud rate 
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void CmdProcessor_Task (void) 
{
    SYS_CONSOLE_Tasks(sysObj.sysConsole0);
    /* SYS_COMMAND layer tasks routine */
    SYS_CMD_Tasks();
    SYS_CMD_READY_TO_READ();
}
 
static int _APP_Commands_EchoArg(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc < 2 || *argv[1] == 0)
    {
        SYS_CMD_MESSAGE("Nothing to echo.");
        SYS_CMD_MESSAGE("\n");
        return false;
    }

    SYS_CMD_MESSAGE(argv[1]);
    SYS_CMD_MESSAGE("\n");
    return false;
}

static int _APP_Commands_PrintMultipleLines(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    char* outputText = "A quick brown fox jumps over the lazy dog.";
    size_t count = 1;
    size_t index = 0;

    if (argc < 2 || *argv[1] == 0)
    {
        SYS_CMD_MESSAGE(outputText);
        SYS_CMD_MESSAGE("\n");
        return false;
    }

    count = (size_t)(*argv[1] - '0');

    for (index = 0; index < count; ++index)
    {
        SYS_CMD_MESSAGE(outputText);
        SYS_CMD_MESSAGE("\n");
    }
    
    return false;
}

/* *****************************************************************************
 End of File
 */
