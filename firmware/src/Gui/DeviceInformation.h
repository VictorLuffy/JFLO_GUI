/** @file DeviceInformation.h
 *  @brief 
 *  @author Trac Truong
 */

#ifndef DEVICEINFORMATION_H
#define	DEVICEINFORMATION_H

#include "stdint.h"
#include "stdbool.h"

#define MAIN_FW_VERSION "0.7.0"

#define MAX_OPERATION_TIME 1000000
#define CRC_SIZE_BYTES 2

typedef struct {
    uint32_t total_operation_time_adult;
    uint32_t total_operation_time_child;
    uint32_t operation_time_after_service_adult;
    uint32_t operation_time_after_service_child;
    
    char esp_fw_version[16]; /*  xx.yy.zz */
    char spo2_fw_version[16]; /*  xx.yy.zz */
    
    char mainSerialNo[16];
    char mainTotalRuntime[16];
    char mainSWVer[16];
    char mainBootloaderVer[16];
    char subSWVer[16];
    char subBootloaderVer[16];
    char chamberSerialNo[16];
    char chamberTotalRuntime[16];
    char chamberSWVer[16];
    char chamberBootloaderVer[16];
    char cradleSerialNo[16];
    char cradleTotalRuntime[16];
    char cradleSWVer[16];
    char cradleBootloaderVer[16];
    char circuitSerialNo[16];
    char circuitType[16];
    char circuitStartOfUse[16];
    char circuitUsageTime[16];
    char circuitFactory[16];
    
} DeviceInformation;

typedef enum {
    eMainSerialNoDeviceInfoId,
    eMainTotalRuntimeDeviceInfoId,
    eMainSWVersionDeviceInfoId,
    eMainBootloaderVersionDeviceInfoId,
    eSubSWVersionDeviceInfoId,
    eSubBootloaderVersionDeviceInfoId,

    eChamberSerialNoDeviceInfoId,
    eChamberTotalRuntimeDeviceInfoId,
    eChamberSWVersionDeviceInfoId,
    eChamberBootloaderVersionDeviceInfoId,

    eCradleSerialNoDeviceInfoId,
    eCradleTotalRuntimeDeviceInfoId,
    eCradleSWVersionDeviceInfoId,
    eCradleBootloaderVersionDeviceInfoId,

    eCircuitSerialNoDeviceInfoId,
    eCircuitTypeDeviceInfoId,
    eCircuitStartOfUseNoDeviceInfoId,
    eCircuitUsageTimeDeviceInfoId,
    eCircuitFactoryDeviceInfoId,

    eNoOfDeviceInfoId
} E_DeviceInfoId;

/** @brief DeviceInformation_Init
 *  @param [in] None
 *  @return None
 */
void DeviceInformation_Init(void);

/** @brief DeviceInformation_GetAllInfo
 *  @param [Out] devInfo
 *  @return None
 */
void DeviceInformation_GetAllInfo(DeviceInformation *devInfo);

/** @brief DeviceInformation_SetAllInfo
 *  @param [in] devInfo
 *  @return None
 */
void DeviceInformation_SetAllInfo(DeviceInformation devInfo);

/** @brief DeviceInformation_AddOperationTimeAdult
 *  @param [in] op_time
 *  @return None
 */
void DeviceInformation_AddOperationTimeAdult(uint32_t op_time);

/** @brief DeviceInformation_AddOperationTimeChild
 *  @param [in] op_time
 *  @return None
 */
void DeviceInformation_AddOperationTimeChild(uint32_t op_time);

/** @brief DeviceInformation_GetTotalOperationTime
 *  @param [in] None
 *  @return None
 */
uint32_t DeviceInformation_GetTotalOperationTime(void);

/** @brief DeviceInformation_GetOperationTimeAfterService
 *  @param [in] None
 *  @return None
 */
uint32_t DeviceInformation_GetOperationTimeAfterService(void);

/** @brief DeviceInformation_Save
 *  @param [in] None
 *  @return None
 */
void DeviceInformation_Save(void);

/** @brief DeviceInformation_Restore
 *  @param [in] None
 *  @return None
 */
bool DeviceInformation_Restore(void);

/** @brief DeviceInformation_SetDefault
 *  @param [in] None
 *  @return None
 */
void DeviceInformation_SetDefault(void);

/** @brief DeviceInformation_PrintToConsole
 *  @param [in] None
 *  @return None
 */
void DeviceInformation_PrintToConsole(void );

/** @brief Get device info string from id
 *  @param [in] E_DeviceInfoId id
 *  @param [out] char *str
 *  @return None
 */
void DeviceInformation_GetDeviceInfo(E_DeviceInfoId id, char *str);

/** @brief Set device info string from id
 *  @param [in] E_DeviceInfoId id
 *  @param [in] char *str
 *  @return bool: true if success, false if fail
 */
bool DeviceInformation_SetDeviceInfo(E_DeviceInfoId id, char *str);


#endif	/* DEVICEINFORMATION_H */

/* end of file */
