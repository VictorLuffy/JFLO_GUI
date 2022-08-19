/** @file DeviceInformation.h
 *  @brief 
 *  @author Trac Truong
 */

#include "AlarmNotificationList.h"

#ifdef UNIT_TEST
bool logInterface_WriteAlarmLog(uint8_t dataLen, void* logData, E_AlarmId id){}
#endif

static unsigned int size = 0;
static AlarmNotification AlarmNotificationList[MAX_SIZE_LIST] __attribute__((section(".ddr_data"), space(prog)));

const char *AlarmStatusStr[4] = {
    "Inactive",
    "Paused",
    "WaitingToActive",
    "Active",
};
const char *AlarmPriorityStr[4] = {
    "None",
    "Low",
    "Medium",
    "High"
};

void AlarmNotificationList_Init()
{
    size = 0;
    int i = 0;
    for (i = 0 ; i < MAX_SIZE_LIST; i ++)
    {
        memset(&AlarmNotificationList[i], 0, sizeof(AlarmNotification));
    }
}

AlarmNotificationError AlarmNotificationList_Remove(E_AlarmId alarmId)
{
    if (size > MAX_SIZE_LIST || size <= 0 )
    {
        DEBUG_MSG("eSizeInvalid \n");
        return eSizeInvalid;
    }
    if (size == 1)
    {
        if (alarmId == AlarmNotificationList[0].alarmId)
        {
            memset(&AlarmNotificationList[0], 0, sizeof(AlarmNotification));
            size = 0;
            return eNoError;
        }
        else
        {
            return eProcessError;
        }
    }
    if (alarmId == AlarmNotificationList[size -1].alarmId)
    {
        // remove last item
        memset(&AlarmNotificationList[size - 1], 0, sizeof(AlarmNotification));
        size--;
        return eNoError;
    }

    unsigned int i = 0;
    bool remove = false;
    for (i = 0 ; i < size - 1; i ++)
    {
        if (alarmId == AlarmNotificationList[i].alarmId)
        {
            remove = true;
        }
        if (remove)
        {
            memcpy(&AlarmNotificationList[i], &AlarmNotificationList[i+1], sizeof(AlarmNotification));
        }
    }
    if (remove)
    {
        size--;
        return eNoError;
    }
    DEBUG_MSG("eUpdateError \n");
    return eProcessError;
}

AlarmNotificationError AlarmNotificationList_GetItem(unsigned int index, AlarmNotification *alarmNotification)
{
    if (index >= size)
    {
        return eIndexInvalid;
    }
    if (alarmNotification == 0)
    {
        return eProcessError;
    }
    memcpy(alarmNotification, &AlarmNotificationList[index], sizeof(AlarmNotification));
    return eNoError;

}

// this used for debug only
#ifndef UNIT_TEST
void AlarmNotificationList_PrintList()
{
    if (size > MAX_SIZE_LIST || size <= 0)
    {
        DEBUG_MSG("[id: - | sts: - | pri: -] \n");
        return;
    }
    unsigned int i = 0;
    for (i = 0 ; i < size; i ++)
    {
        DEBUG_MSG("[id: %d | sts: %s | pri: %s] \n",
            AlarmNotificationList[i].alarmId,
            AlarmStatusStr[AlarmNotificationList[i].alarmStatus],
            AlarmPriorityStr[AlarmNotificationList[i].alarmPriority]
            );
        // DEBUG_MSG("AlarmNotification id %d  / %d \n", i, size);
        // DEBUG_MSG(" - alarmId %d \n", AlarmNotificationList[i].alarmId);
        // DEBUG_MSG(" - alarmStatus %d \n", AlarmNotificationList[i].alarmStatus);
        // DEBUG_MSG(" - alarmPriority %d \n", AlarmNotificationList[i].alarmPriority);
        // DEBUG_MSG(" - alarmData[0] %d \n", AlarmNotificationList[i].alarmData[0]);
        // DEBUG_MSG(" - alarmData[1] %d \n", AlarmNotificationList[i].alarmData[1]);
        // DEBUG_MSG(" - alarmData[2] %d \n", AlarmNotificationList[i].alarmData[2]);
        // DEBUG_MSG(" - alarmData[3] %d \n", AlarmNotificationList[i].alarmData[3]);
        // DEBUG_MSG(" - alarmData[4] %d \n", AlarmNotificationList[i].alarmData[4]);
    }
    return;
}
#endif
int AlarmNotificationList_GetSize()
{
    if (size > MAX_SIZE_LIST)
    {
        return -1;
    }
    return size;
}

int AlarmNotificationList_GetItemIndex(E_AlarmId alarmId)
{
    if (size > MAX_SIZE_LIST || size <=0 )
    {
        return -1;
    }
    unsigned int i = 0;
    for (i = 0; i < size; i++)
    {
        if (alarmId == AlarmNotificationList[i].alarmId)
        {
            return i;
        }
    }
    return -1;    
}

int AlarmNotificationList_GetActiveIndex()
{
    if (size > MAX_SIZE_LIST)
    {
        return -1;
    }
    unsigned int i = 0;
    for (i = 0 ; i < size; i ++)
    {
        if (AlarmNotificationList[i].alarmStatus == eActive)
        {
            return i;
        }
    }
    return -1;
}

AlarmNotificationError AlarmNotificationList_ProcessAlarmNotificationList()
{
    if (size > MAX_SIZE_LIST || size <= 0 )
    {
        DEBUG_MSG("eSizeInvalid \n");
        return eSizeInvalid;
    }

    unsigned int i = 0;
    int next_active_id = -1;
    E_AlarmPriority next_active_pri = -1;
    E_AlarmStatus next_active_status = -1;
    for (i = 0 ; i < size; i ++)
    {
        // find the first active / waiting to active, with highest priority
        if (AlarmNotificationList[i].alarmStatus == eActive || AlarmNotificationList[i].alarmStatus == eWaitingToActive)
        {
            if (next_active_id < 0) 
            {
                //set first id
                next_active_id = i;
                next_active_pri = AlarmNotificationList[i].alarmPriority;
                next_active_status = AlarmNotificationList[i].alarmStatus;
            }
            else if (AlarmNotificationList[i].alarmPriority > next_active_pri)
            {
                // set the highest priority with waiting/active status,
                next_active_id = i;
                next_active_pri = AlarmNotificationList[i].alarmPriority;
                next_active_status = AlarmNotificationList[i].alarmStatus;
            }
            else if (AlarmNotificationList[i].alarmPriority == next_active_pri && 
                    AlarmNotificationList[i].alarmStatus > next_active_status)
            {
                // set the highest status with same priority
                next_active_id = i;
                next_active_pri = AlarmNotificationList[i].alarmPriority;
                next_active_status = AlarmNotificationList[i].alarmStatus;
            }
            else {}
        }
    }

    if (next_active_id >= 0)
    {
        // if there is next alarm active id, set status it to active
        DEBUG_MSG("ACTIVE ALARM ID %d \n", AlarmNotificationList[next_active_id].alarmId);
        AlarmNotificationList[next_active_id].alarmStatus = eActive;

        // all active remain -> waiting to active status
        for (i = 0; i < size; i ++)
        {
            if (i != (unsigned int)next_active_id && AlarmNotificationList[i].alarmStatus == eActive)
            {
                AlarmNotificationList[i].alarmStatus = eWaitingToActive;
            }
        }
        return eNoError;
    }
    else
    {
        DEBUG_MSG("NO ACTIVE ALARM \n");
        return eNoActiveAlarm;
    }
}
AlarmNotificationError AlarmNotificationList_UpdateAlarm(AlarmNotification alarmNotification)
{
    // behavious depend on the alaram status
    switch(alarmNotification.alarmStatus)
    {
        case eActive:
        case eWaitingToActive:
        case ePaused:
        {
            if (alarmNotification.alarmStatus == eActive)
            {
                // always append to list with waiting to active status
                // call AlarmNotificationList_UpdateAllStatus() to decide which alarm is active
                alarmNotification.alarmStatus = eWaitingToActive;
            }

            //add new alarm when array is empty
            if (size == 0) 
            {
                DEBUG_MSG("active new alarm id %d \n", alarmNotification.alarmId);
                memcpy(&AlarmNotificationList[size], &alarmNotification, sizeof(AlarmNotification));
                size++;
                
                //logging
                uint8_t logData[2] = {(uint8_t)alarmNotification.alarmStatus, (uint8_t)alarmNotification.alarmPriority};
                logInterface_WriteAlarmLog(2, logData , alarmNotification.alarmId);
                return eNoError;        
            }

            //update existing alarm
            int index = AlarmNotificationList_GetItemIndex(alarmNotification.alarmId);
            if (index >= 0) // found item to update
            {
                //logging
                if (alarmNotification.alarmStatus != AlarmNotificationList[index].alarmStatus)
                {
                    // only logging if status change in paused
                    uint8_t logData[2] = {(uint8_t)alarmNotification.alarmStatus, (uint8_t)alarmNotification.alarmPriority};
                    logInterface_WriteAlarmLog(2, logData , alarmNotification.alarmId);
                }
                
                DEBUG_MSG("update alarm id %d \n", alarmNotification.alarmId);
                memcpy(&AlarmNotificationList[index], &alarmNotification, sizeof(AlarmNotification));
                return eNoError;
            }
            else
            {}
            
            //add new alarm
            if (size < MAX_SIZE_LIST) // list not full
            {
                DEBUG_MSG("active new alarm id %d \n", alarmNotification.alarmId);
                memcpy(&AlarmNotificationList[size], &alarmNotification, sizeof(AlarmNotification));
                size++;
                //logging
                uint8_t logData[2] = {(uint8_t)alarmNotification.alarmStatus, (uint8_t)alarmNotification.alarmPriority};
                logInterface_WriteAlarmLog(2, logData , alarmNotification.alarmId);

                return eNoError;
            }
            else
            {
                DEBUG_MSG("alarm list full \n");
                return eArrayFull;
            }
        }
        case eInactive:
        {
            DEBUG_MSG("inactive alarm id %d \n", alarmNotification.alarmId);
            //logging
            uint8_t logData[2] = {(uint8_t)alarmNotification.alarmStatus, (uint8_t)alarmNotification.alarmPriority};
            logInterface_WriteAlarmLog(2, logData , alarmNotification.alarmId);
            return AlarmNotificationList_Remove(alarmNotification.alarmId);
        }
        default:
            return eProcessError;
    }
}

/* end of file */
