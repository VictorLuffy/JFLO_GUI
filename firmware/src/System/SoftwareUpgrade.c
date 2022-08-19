/** @file SoftwareUpgrade.c
 *  @brief Update software
 *  @author Viet Le
 */
#include "system_config.h"
#include "system_definitions.h"

#include "SoftwareUpgrade.h"
#include "bootloader/src/bootloader.h"
#include "peripheral/nvm/plib_nvm.h"
#include "peripheral/int/plib_int.h"
#include "system/devcon/sys_devcon.h"
#include "system/debug/sys_debug.h"
#include "GuiDefine.h"
#include "Gui/AlarmScreen.h"
#include "Gui/GuiInterface.h"
#include "SQIInterface.h"
#include "Cradle.h"
//#include "Device/Cradle.h"
//#include "ChamberUnit.h"

#include "MotorTask.h"
#include "HeaterTask.h"
#include "Delay.h"
#include "mm.h"

#include "SystemInterface.h"
#include "Watchdog.h"
#include "AlarmTask.h"

/** @brief Define flag upgrade */
#define UPGRADE_DIRNAME      "Upgrade"

/** @brief Define flag upgrade */
#define UPGRADE_DIR      "/mnt/USB/Upgrade"

/** @brief Define flag upgrade */
#define FILE_MAINBOARD_FIRMWARE_NAME      "image.hex"

/** @brief Define flag upgrade */
#define FILE_MAINBOARD_FIRMWARE_PATH      "Upgrade/image.hex"

/** @brief Define flag upgrade */
#define FILE_CHAMBER_FIRMWARE_NAME      "Chamber.hex"

/** @brief Define flag upgrade */
#define FILE_CHAMBER_FIRMWARE_PATH      "Upgrade/Chamber.hex"


/** @brief Define flag upgrade */
#define FILE_CRADLE_FIRMWARE_NAME       "Cradle.hex"
/** @brief Define flag upgrade */
#define FILE_CRADLE_FIRMWARE_PATH       "Upgrade/Cradle.hex"
/** @brief Define flag upgrade */
#define FILE_ESP32_FIRMWARE_NAME        "ESP32_10.bin"
/** @brief Define flag upgrade */
#define FILE_ESP32_FIRMWARE_PATH        "Upgrade/ESP32_10.bin"
/** @brief Define flag upgrade */
bool g_isUpgradeFirmware = false;

/** @brief Define flag upgrade */
SYS_FS_HANDLE g_firmwareFile;

//Declare flag mount USB
extern bool g_isMountUSB;

bool g_isUpgradeCopying = false;
bool g_isUpgradeProcess = false;

const char FILE_LIST_OF_UPDATE[] =  "/mnt/USB/Upgrade/update_list.jflo";
// limit 1024 files at once, file name not over 50 chars
__attribute__((section(".ddr_data"), space(prog))) char listOfUpdateFiles[1024][50] = {[ 0 ... 1023 ] = ""};
uint16_t noOfUpdateFiles = 0;

// Just for test
uint32_t tick;
uint32_t UpdateTime;
uint32_t lastTick;


/** @brief The function bootloader force event
 *  @param [in] None
 *  @param [out] None
 *  @return int : 1 is force to bootloder, 0 : force to application
 */
int softwareUpgrade_BootloaderForceEvent(void)
{
    if (g_isMountUSB == true)
    {
        if (g_isUpgradeFirmware == true)
        {
            //SYS_PRINT("bootforceUp\n");
            return (1);
        }
    }
    return (0);
}

/** @brief The function reset after upgrade success
 *  @param [in] None
 *  @param [out] None
 *  @return int
 */
int softwareUpgrade_ForceResetAfterProgram(void)
{
    SYS_PRINT("\nupdate firmware success\n");
//    PLIB_INT_Disable(INT_ID_0);
    PLIB_NVM_MemoryModifyInhibit(NVM_ID_0);
    PLIB_NVM_MemoryOperationSelect(NVM_ID_0, NO_OPERATION);
    PLIB_NVM_FlashSwapLockSelect(NVM_ID_0, NVM_FLASH_SWAP_UNLOCKED);
    if (NVMCONbits.PFSWAP)
    {
        PLIB_NVM_FlashWriteKeySequence(NVM_ID_0, NVM_PROGRAM_UNLOCK_KEY1);
        PLIB_NVM_FlashWriteKeySequence(NVM_ID_0, NVM_PROGRAM_UNLOCK_KEY2);
        PLIB_NVM_ProgramFlashBank1LowerRegion(NVM_ID_0);
    }
    //delay_MS(100);
    guiInterface_SendEvent(eGuiUpdateScreenMessageUpdateSuccessTurnOffToComplete, 0);
    //SYS_RESET_SoftwareReset();
}

/** @brief The function to initialize Software upgrade module
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void softwareUpgrade_Init()
{
    // Register the bootloader callbacks
    BOOTLOADER_ForceBootloadRegister(softwareUpgrade_BootloaderForceEvent);
    BOOTLOADER_ProgramCompleteRegister(softwareUpgrade_ForceResetAfterProgram);

    return;
}

/** @brief The function process software upgrade 
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void softwareUpgrade_Process(void)
{
  
    int i = 0;
    bool copySuccess = true;
    
    TickType_t xGuiTick = xTaskGetTickCount();
    if (!FileSystemMgr_IsUSBMounted())
    {
        guiInterface_SendEvent(eGuiUpdateScreenMessageUSBNotFound, 0);
        SYS_PRINT("USB is not mount \n");
        return;
    }    
   
    
    if(!softwareUpgrade_GetFileUpdate())
    {
        guiInterface_SendEvent(eGuiUpdateScreenMessageFileNotFound, 0);
        SYS_PRINT("Failed to check update files \n");
        return;
    }
    else
    {
        /*stop system to update*/
        MOTOR_CTRL_EVENT_t mEvent = {.id = eMotorStopId};
        MotorTask_SendEvent(mEvent);
        //stop heater
        HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId};
        HeaterTask_SendEvent(hEvent);
        vTaskDelay(100);

        //suspend Task
        SYS_PRINT("\nSuspend device Task\n");
        DeviceTask_Suspend();
        alarmTask_Suspend();
    
        //guiInterface_SendEvent(eGuiUpdateScreenMessageUpdatingAssets, 0);
        SYS_PRINT("Number Of Update Files: %d \n", noOfUpdateFiles);
        if(noOfUpdateFiles > 0)
        {
            g_isUpgradeCopying = true;
            g_isUpgradeProcess = true;
            
            //TODO: consider backup settings/log file before format SQI flash (ex: backup to USB flash, the restore to SQI flash after format)
            SYS_PRINT("Backup to USB \n");
            logMgr_BackupToUSB();
            file_CloseFileOnSQIFlash();
            LogInterface_DeInitDebugLogFile();
            
            SQIInterface_Format();
            
            SYS_PRINT("Restore frome USB \n");
            
            LogInterface_InitDebugLogFile();
            file_OpenFileOnSQIFlash();
            logMgr_RestoreFromUSB();
            
            while (i < noOfUpdateFiles && copySuccess)
            {
                SYS_PRINT("Copying %s \n", listOfUpdateFiles[i]);
                char filePath[255];
                strcpy(filePath, UPGRADE_DIRNAME);
                strcat(filePath, "/");
                strcat(filePath, listOfUpdateFiles[i]);

                if (FILECOPY_SUCCESS == SQIInterface_CopyFile(filePath, SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0,
                                                              listOfUpdateFiles[i], SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0))
                {
//                    guiInterface_SendEvent(eGuiUpdateScreenMessageUpdatingAssetsStatus, (long)i << 16 | (long) noOfUpdateFiles ); // TODO : Update status with no of file
                    if (i % 5 == 0 ) 
                    {
                        guiInterface_SendEvent(eGuiUpdateScreenMessageUpdatingAssetsStatus, (long)( i * 100 / noOfUpdateFiles)); // TODO : Update status with percent
                        vTaskDelay(50);                        
                    }
                }
                else
                {
                    copySuccess = false;
                    guiInterface_SendEvent(eGuiUpdateScreenMessageUpdateFailed, 0);
                    SYS_PRINT("\n Error : copyFailed %d \n", i);
                    break;
                }
                i++;
            }
                       
            uint32_t totalSector, freeSector;
            if (SYS_FS_DriveSectorGet("mnt/SQIFlash", &totalSector, &freeSector) != SYS_FS_RES_SUCCESS){
                SYS_PRINT("SYS_FS_DriveSectorGet Failed \n");
            }
            else{
                SYS_PRINT("totalSector = %u, freeSector = %u \n", totalSector, freeSector);
            }

            xGuiTick = xTaskGetTickCount() - xGuiTick;
            SYS_PRINT("softwareUpgrade_Process %d\n",xGuiTick); 
            
            if (copySuccess == true)
            {
                SYS_PRINT("\ncopy assets to SQI success\n");
            }
            else
            {
                g_isUpgradeCopying = false;
                return;
            }
        }
        else
        {
            SYS_PRINT("\not update assets files for GUI \n");
        }
        g_isUpgradeCopying = false;
    }
    // update firmware
    if ((copySuccess == true) || (noOfUpdateFiles == 0))
    {
        if (USBInterface_Search("/mnt/USB/Upgrade", "image.hex") == SYS_FS_RES_SUCCESS)
        {
            SYS_FS_HANDLE upgradeFile = SYS_FS_FileOpen("/mnt/USB/Upgrade/image.hex", SYS_FS_FILE_OPEN_READ);
            if (upgradeFile != SYS_FS_HANDLE_INVALID)
            {
                sizeOfMainboardFirmware = SYS_FS_FileSize(upgradeFile);
                SYS_FS_FileClose(upgradeFile);
                guiInterface_SendEvent(eGuiUpdateScreenMessageUpdatingMainboardFirmware, 0);
                SYS_PRINT("\nstart updating firmware\n");
                //set update firmware flag
                g_isUpgradeFirmware = true;           
            }
        }
        else
        {
            SYS_PRINT("\n new firmware does not exist on USB >> only update assets for GUI, not update firmware \n");
            guiInterface_SendEvent(eGuiUpdateScreenMessageUpdateSuccessTurnOffToComplete, 0);
        }
    }
    
    return;
}



void softwareUpgrade_NewSoftwareForCradle(void)
{

    SYS_PRINT("\n softwareUpgrade_NewSoftwareForCradle\n");

    uint8_t* firmwareBuffer;

    if (!FileSystemMgr_IsUSBMounted())
    {
        guiInterface_SendEvent(eGuiUpdateScreenMessageUSBNotFound, 0);
        SYS_PRINT("USB is not mount \n");
        return;
    }  

    // Disable watchdog 
    Watchdog_Disable();

    if (SYS_FS_RES_SUCCESS != SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0))
    {
        SYS_PRINT("\n SYS_FS_CurrentDriveSet USB in softwareUpgrade_NewSoftwareForCharger Fail");
        guiInterface_SendEvent(eGuiUpdateScreenMessageFileNotFound, 0);
        return;
    }

    if (USBInterface_Search("/mnt/USB/Upgrade", FILE_CRADLE_FIRMWARE_NAME) == SYS_FS_RES_SUCCESS)
    {
        SYS_PRINT("\n Update Cradle firmware");
        guiInterface_SendEvent(eGuiUpdateScreenMessageUpdatingCradleFirmware, 0);
        /*stop system to update*/
        MOTOR_CTRL_EVENT_t mEvent = {.id = eMotorStopId};
        MotorTask_SendEvent(mEvent);
        //stop heater
        HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId};
        HeaterTask_SendEvent(hEvent);  

        //wait for device task stop motor and heater
        vTaskDelay(100);
        //suspend Task
        SYS_PRINT("\nSuspend device Task\n");
        DeviceTask_Suspend();
        alarmTask_Suspend();
        //wait for cradle can receive command
        vTaskDelay(10);

        // Handle the hex file    
        g_firmwareFile = SYS_FS_FileOpen(FILE_CRADLE_FIRMWARE_PATH, SYS_FS_FILE_OPEN_READ);

        uint32_t fileSize = SYS_FS_FileSize(g_firmwareFile);

        SYS_FS_FileSeek(g_firmwareFile, 0, SYS_FS_SEEK_SET);

        // Alloc data memory for buffer
        firmwareBuffer = mm_malloc(fileSize);

        // Read data to temporary buffer
        uint32_t readBytes = SYS_FS_FileRead(g_firmwareFile, firmwareBuffer, fileSize);
        SYS_PRINT("\n readBytes = %u", readBytes);      

        // Start update
        if (cradleFirmwareUpdate_InitFirmware()) 
        {
            SYS_PRINT("\nUpgrade cradle firmware");
            g_isUpgradeProcess = true;
//                Cradle_TestFunction();
//                cradle_IsCommunicationFailure();
//                Cradle_UpgradeFirmware(firmwareBuffer, readBytes);
            while (Cradle_UpgradeFirmware(firmwareBuffer, readBytes) == false) { SYS_PRINT("\nUpgrade"); }
            SYS_PRINT("\nUpgrade Done");
        }
        else 
            return;

        mm_free(firmwareBuffer);

        // Close file
        SYS_FS_FileClose(g_firmwareFile);

        if(Cradle_IsUpdateSuccess()){
            guiInterface_SendEvent(eGuiUpdateScreenMessageUpdateSuccessTurnOffToComplete, 0);
        }
        else
        {
            guiInterface_SendEvent(eGuiUpdateScreenMessageUpdateFailed, 0);
        }
        
//        delay_MS(100);
//        SYS_PRINT("\nSystem Reset\n");
//        vTaskSuspendAll();
//        //vTaskEnterCritical();
//        SYS_RESET_SoftwareReset();
//        POWER_ENABLEOff();      
    }    
    else 
    {
        SYS_PRINT("\nCRADLE Firmware File not available in USB\n");
        guiInterface_SendEvent(eGuiUpdateScreenMessageFileNotFound, 0);
        Watchdog_Init();
    }
    //guiInterface_SendEvent(eGuiChangeToOperScreenId, 1);

    //Watchdog_Init();

    return;
}
void softwareUpgrade_NewSoftwareForChamber(void)
{
    SYS_PRINT("\n softwareUpgrade_NewSoftwareForChamber");

    uint8_t* firmwareBuffer;
    
    if (!FileSystemMgr_IsUSBMounted())
    {
        guiInterface_SendEvent(eGuiUpdateScreenMessageUSBNotFound, 0);
        SYS_PRINT("USB is not mount \n");
        return;
    }  
       
    // Disable watchdog 
    Watchdog_Disable();

    if (SYS_FS_RES_SUCCESS != SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0))
    {
        SYS_PRINT("\n SYS_FS_CurrentDriveSet USB in softwareUpgrade_NewSoftwareForChamber Fail\n");
        guiInterface_SendEvent(eGuiUpdateScreenMessageFileNotFound, 0);
        return;
    }
    
    if (USBInterface_Search("/mnt/USB/Upgrade", FILE_CHAMBER_FIRMWARE_NAME) == SYS_FS_RES_SUCCESS)
    {
        SYS_PRINT("\n Update chamber firmware\n");
        guiInterface_SendEvent(eGuiUpdateScreenMessageUpdatingChamberFirmware, 0);
        /*stop system to update*/
        MOTOR_CTRL_EVENT_t mEvent = {.id = eMotorStopId};
        MotorTask_SendEvent(mEvent);
        //stop heater
        HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId};
        HeaterTask_SendEvent(hEvent);

        //wait for device task stop motor and heater
        vTaskDelay(100);
        //suspend Task
        SYS_PRINT("\nSuspend device Task\n");
        DeviceTask_Suspend();
        alarmTask_Suspend();
        //wait for chamber can receive command
        vTaskDelay(10);
        
        // Handle the hex file 
        g_firmwareFile = SYS_FS_FileOpen(FILE_CHAMBER_FIRMWARE_PATH, SYS_FS_FILE_OPEN_READ);
        
        uint32_t fileSize = SYS_FS_FileSize(g_firmwareFile);
        
        SYS_FS_FileSeek(g_firmwareFile, 0, SYS_FS_SEEK_SET);

        // Alloc data memory for buffer
        firmwareBuffer = mm_malloc(fileSize);

        // Read data to temporary buffer
        uint32_t readBytes = SYS_FS_FileRead(g_firmwareFile, firmwareBuffer, fileSize);
        SYS_PRINT("\n readBytes = %u\n", readBytes);      

        // Start update
        if(Chamber_InitFirmware())
        {
            g_isUpgradeProcess = true;
            while (Chamber_UpgradeFirmware(firmwareBuffer, readBytes) == false) {}
        }
        //resume Task
        //DeviceTask_Resume(); 
        //SYS_PRINT("\nResume device Task\n");
        
        mm_free(firmwareBuffer);
        
        // Close file
        SYS_FS_FileClose(g_firmwareFile);  
        if(Chamber_IsUpdateSuccess()){
            guiInterface_SendEvent(eGuiUpdateScreenMessageUpdateSuccessTurnOffToComplete, 0);
        }
        else
        {
            guiInterface_SendEvent(eGuiUpdateScreenMessageUpdateFailed, 0);
        }
//        delay_MS(100);
//        SYS_PRINT("\nSystem Reset\n");
//        vTaskSuspendAll();
//        //vTaskEnterCritical();
//        SYS_RESET_SoftwareReset();
//        POWER_ENABLEOff();
    }    
    else 
    {
        SYS_PRINT("\n Chamber Firmware File not available in USB\n");
        guiInterface_SendEvent(eGuiUpdateScreenMessageFileNotFound, 0);
        Watchdog_Init();
    }

//    guiInterface_SendEvent(eGuiChangeToOperScreenId, 1);
    
//    Watchdog_Init();
    
    return;
}

//extern TaskHandle_t TaskHandle_1;
void softwareUpgrade_NewSoftwareForEsp32(void)
{
    SYS_PRINT("\n softwareUpgrade_NewSoftwareForEsp32\n");

    uint8_t* firmwareBuffer;
    //vTaskPrioritySet(TaskHandle_1, tskIDLE_PRIORITY + 4);
    // Disable watchdog 
    Watchdog_Disable();

    if (SYS_FS_RES_SUCCESS != SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0))
    {
//        SYS_PRINT("\n SYS_FS_CurrentDriveSet USB in softwareUpgrade_NewSoftwareForEsp32 Fail");
        return;
    }
    
    if (USBInterface_Search("/mnt/USB/Upgrade", FILE_ESP32_FIRMWARE_NAME) == SYS_FS_RES_SUCCESS)
    {
        SYS_PRINT("\n Update Esp32 firmware\n");
        /*stop system to update*/
        MOTOR_CTRL_EVENT_t mEvent = {.id = eMotorStopId};
        MotorTask_SendEvent(mEvent);
        //stop heater
        HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId};
        HeaterTask_SendEvent(hEvent);

        //wait for device task stop motor and heater
        vTaskDelay(100);
        //suspend Task
        SYS_PRINT("\nSuspend device Task\n");
        DeviceTask_Suspend();
        alarmTask_Suspend();
        //wait for chamber can receive command
        vTaskDelay(10);      
        
        // Handle the hex file 
        g_firmwareFile = SYS_FS_FileOpen(FILE_ESP32_FIRMWARE_PATH, SYS_FS_FILE_OPEN_READ);
        
        uint32_t fileSize = SYS_FS_FileSize(g_firmwareFile);
        
        SYS_FS_FileSeek(g_firmwareFile, 0, SYS_FS_SEEK_SET);

        // Alloc data memory for buffer
        firmwareBuffer = mm_malloc(fileSize);

        // Read data to temporary buffer
        uint32_t readBytes = SYS_FS_FileRead(g_firmwareFile, firmwareBuffer, fileSize);
//        SYS_PRINT("\n readBytes = %u", readBytes);      


//         Start update
        lastTick = xTaskGetTickCount();

        
        if(esp32_InitFirmware(firmwareBuffer, readBytes))
        {
            g_isUpgradeProcess = true;
            while (esp32_UpgradeFirmware(firmwareBuffer) == false) {}
            SYS_PRINT("\nESP UPDATE FINISHED.");
        }
            
        tick = xTaskGetTickCount();
//        uint32_t UpdateTime = 0;
        UpdateTime = tick - lastTick;
//        SYS_PRINT("tick_ %d\n",tick - lastTick);        
        mm_free(firmwareBuffer);
        
        // Close file
        SYS_FS_FileClose(g_firmwareFile);

        delay_MS(100);
        SYS_PRINT("\nSystem Reset\n");
        vTaskSuspendAll();
        //vTaskEnterCritical();
        SYS_RESET_SoftwareReset();
        POWER_ENABLEOff();      
    }    
    else 
    {
//        SYS_PRINT("\n Firmware File not available in USB");
    }

    guiInterface_SendEvent(eGuiChangeToOperScreenId, 1);
    
    Watchdog_Init();
    
    return;
}

bool softwareUpgrade_GetFileUpdate(void)
{          
    bool ret = true;
    /* Open file */
    SYS_FS_HANDLE fileHandle;
    uint32_t fileSize, readBytes;
    uint8_t *buffer;
    fileHandle = SYS_FS_FileOpen(FILE_LIST_OF_UPDATE,
            (SYS_FS_FILE_OPEN_READ));

    if(fileHandle != SYS_FS_HANDLE_INVALID)
    {
        fileSize = SYS_FS_FileSize(fileHandle);
        SYS_FS_FileSeek(fileHandle, 0, SYS_FS_SEEK_SET);

        guiInterface_SendEvent(eGuiUpdateScreenMessageCheckingAsset, 0);
        vTaskDelay(100);

        if (fileSize > 0)
        {
            buffer = (uint8_t*)mm_malloc(fileSize);
            if (buffer > 0)
            {
                readBytes = SYS_FS_FileRead(fileHandle, buffer, fileSize);
                if (readBytes != fileSize)
                {
                    SYS_PRINT("Error : readBytes != fileSize %d %d \n", readBytes, fileSize);
                    ret = false;
                }
                else
                {
                    // parse file by delimiter ','
                    int i = 0;
                    char *tmp = strtok (buffer,"\r\n");
                    while (tmp != NULL)
                    {
//                      SYS_PRINT("[Filename1[%d]: %s ] \n",i, tmp );
                      strcpy(listOfUpdateFiles[i], tmp);
                      i++;
                      tmp = strtok (NULL, "\r\n");
                    }
                    noOfUpdateFiles = i - 1; //TODO : this because last line contains abnormal character, so skip the last line
                    int j = 0;
                    for (j = 0 ; j < noOfUpdateFiles ; j++ )
                    {
                        SYS_PRINT("[Filename[%d]: %s ] \n",j, listOfUpdateFiles[j] );
                        if (USBInterface_Search(UPGRADE_DIR, listOfUpdateFiles[j]) != SYS_FS_RES_SUCCESS)
                        {
                            SYS_PRINT("Error : File not found [%d] %s \n",i, listOfUpdateFiles[j]);
                            ret = false;
                            break;
                        }
                    }
                }
            }
            mm_free(buffer);
        }
    }
    else
    {
        SYS_PRINT("Error %d : failed to open file \n", fileHandle);
        ret = false;
    }
    SYS_FS_FileClose(fileHandle);
        
    return ret;
}

bool SoftwareUpgrade_IsCopying(void)
{
    return g_isUpgradeCopying;
}

bool SoftwareUpgrade_IsUpgrade(void)
{
    return g_isUpgradeProcess;
}

/* *****************************************************************************
 End of File
 */
