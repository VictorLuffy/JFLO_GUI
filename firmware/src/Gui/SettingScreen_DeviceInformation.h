/** @file SettingScreen_DeviceInformation.h
 *  @brief The interfaces for display device information
 *  @author Trac Truong
 */

#ifndef SETTINGSCREEN_DEVICEINFORMATIONSETTING_H
#define	SETTINGSCREEN_DEVICEINFORMATIONSETTING_H

/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef UNIT_TEST
#include "system_config.h"
#include "system_definitions.h"
#endif

#ifdef UNIT_TEST
    #define SYS_PRINT(...)
    typedef enum laResult_t
    {
        LA_FAILURE = -1,
        LA_SUCCESS = 0
    } laResult;
    typedef enum laBool_t
    {
        LA_FALSE = 0,
        LA_TRUE
    } laBool;
    typedef int laWidget;

    typedef enum laEventID_t
    {
        // internal events
        LA_EVENT_NONE,
        LA_EVENT_SCREEN_CHANGE,
        LA_EVENT_TOUCH_DOWN,
        LA_EVENT_TOUCH_UP,
        LA_EVENT_TOUCH_MOVED
    } laEventID;
    typedef struct laWidgetEvent_t
    {
        laEventID id;
        laWidget* source;
        laWidget* target;
        laBool accepted;
    } laWidgetEvent;

    typedef struct laInput_TouchDownEvent_t
    {
        laWidgetEvent event;

        int32_t touchID;
        int32_t x;
        int32_t y;
    } laInput_TouchDownEvent;

    typedef uint32_t GFXU_CHAR;
    typedef uint32_t GFXU_FontAsset;

    typedef struct laString_t
    {
        GFXU_CHAR*   data; // local string data storage
        uint32_t    capacity; // actual memory capacity of the string
        uint32_t    length; // actual length of the string, typically this is
                            // capacity - 1, but can be less.

        GFXU_FontAsset*  font;  // the font that contains the glyph raster data
                                // for this string

        int32_t     table_index; // if this is not LA_STRING_NULLIDX then this string
                                 // is referencing an index in the string table.  string
                                 // table references are read-only but can be extracted
                                 // to local modifiable versions
    } laString;

    typedef enum
    {
        LA_HALIGN_LEFT,
        LA_HALIGN_CENTER,
        LA_HALIGN_RIGHT
    } laHAlignment;

    typedef enum
    {
        LA_VALIGN_TOP,
        LA_VALIGN_MIDDLE,
        LA_VALIGN_BOTTOM
    } laVAlignment;
    typedef uint32_t GFXU_ExternalAssetReader;

    typedef struct laLabelWidget_t
    {
        laWidget widget; // widget base class

        laString text; // string to draw

        laHAlignment halign; // horizontal alignment of string
        laVAlignment valign; // vertical alignment of string

        GFXU_ExternalAssetReader* reader; // asset reader
    } laLabelWidget;

    typedef void (*laWidget_TouchDownEvent_FnPtr)(laWidget*, laInput_TouchDownEvent*);
    typedef uintptr_t       SYS_FS_HANDLE;
    #define SYS_FS_HANDLE_INVALID ((SYS_FS_HANDLE)(-1))
    typedef enum
    {
        /* Set file offset to input number of bytes from the start of file */
        SYS_FS_SEEK_SET,
        /* Set file offset to its current location plus input number of bytes */
        SYS_FS_SEEK_CUR,
        /* Set file offset to size of the file plus input number of bytes */
        SYS_FS_SEEK_END,

    } SYS_FS_FILE_SEEK_CONTROL;

    void laWidget_SetX(laWidget* w, int32_t x);
    void laWidget_SetY(laWidget* w, int32_t y);
    laResult laWidget_SetVisible(laWidget* wgt, laBool visible);
    laResult laWidget_OverrideTouchDownEvent(laWidget* wgt, laWidget_TouchDownEvent_FnPtr ptr);
    laString laString_CreateFromCharBuffer(const GFXU_CHAR* chr, GFXU_FontAsset* fnt);
    laResult laLabelWidget_SetText(laLabelWidget* lbl, laString str);
    void laString_Destroy(laString* str);

    void SettingScreen_SetSettingScreenUpdate(bool f);
    int32_t DisplayControl_GetCenterPostion(laWidget* widgetA, laWidget* widgetB);

    extern laWidget* SC_MenuSetting_SettingBrightness_PrevButton;
    extern laWidget* SC_MenuSetting_SettingBrightness_NextButton;
    extern laLabelWidget* SC_MenuSetting_SettingBrightness_ValueLabel;
    extern laWidget* SC_MenuSetting_SettingBrightness_UnderBar;
    extern GFXU_FontAsset BebasNeueBook_S60_Bold_Internal;
    extern laLabelWidget* SC_MenuSetting_SettingBrightness_OffLabel;
    extern laLabelWidget* SC_MenuSetting_SettingBrightness_OnLabel;
    extern laWidget* SC_MenuSetting_SettingOxySource_90Label;
    extern laWidget* SC_MenuSetting_SettingOxySource_UnderBarPanel;
    extern laWidget* SC_MenuSetting_SettingOxySource_100Label;
    extern laLabelWidget* SC_MenuSetting_SettingSpo2Alarm_ValueLabel;
    extern laLabelWidget* SC_MenuSetting_SettingOxyAlarm_LoLimitLabel;
    extern laLabelWidget* SC_MenuSetting_SettingOxyAlarm_UpLimitLabel;
    extern laLabelWidget* SC_DeviceInformationMainLabel ;
    extern laLabelWidget* SC_DeviceInformationUnderbarPanel ;
    extern laLabelWidget* SC_DeviceInformationChamberLabel ;
    extern laLabelWidget* SC_DeviceInformationCradleLabel ;
    extern laLabelWidget* SC_DeviceInformationCircuitLabel ;
    extern GFXU_FontAsset AbelRegular_S18_Bold_Internal;
    extern laLabelWidget* SC_DeviceInformationLabel_1 ;
    extern laLabelWidget* SC_DeviceInformationLabel_2 ;
    extern laLabelWidget* SC_DeviceInformationLabel_3 ;
    extern laLabelWidget* SC_DeviceInformationLabel_4 ;
    extern laLabelWidget* SC_DeviceInformationLabel_5 ;
    extern laLabelWidget* SC_DeviceInformationLabel_6 ;

#endif

#include "Gui/DeviceInformation.h"

#define SERIAL_NUMBER_STR "- Serial number : "
#define TOTAL_RUNTIME_STR "- Total runtime : "
#define MAIN_SOFTWARE_STR "- Main software : "
#define MAIN_BOOTLOADER_STR "- Main Bootloader : "
#define SUB_SOFTWARE_STR "- Sub software : "
#define SUB_BOOTLOADER_STR "- Sub Bootloader : "

#define SOFTWARE_STR "- Software : "
#define BOOTLOADER_STR "- Bootloader : "

#define TYPE_STR "- Type : "
#define START_OF_USE_STR "- Start of use : "
#define USAGE_TIME_STR "- Usage time : "
#define FACTORY_STR "- Factory : "

typedef enum {
    eMainDeviceInformationSetting, /**< main device information screen */  
    eChamberDeviceInformationSetting, /**< chamber device information screen */  
    eCradleDeviceInformationSetting, /**< cradle device information screen */  
    eCircuitDeviceInformationSetting, /**< circuit device information screen */  
    eNoOfDeviceInformationSetting, /**< no of enum */  
} E_DeviceInformationSetting;

/** @brief SettingScreen_DeviceInformation_Init
 *      This init data
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DeviceInformation_Init();

/** @brief SettingScreen_DeviceInformation_Display
 *      This control for display wifi setting
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DeviceInformation_Display();

/** @brief SettingScreen_DeviceInformation_SetCallbackFunction
 *      This set callback function for some event that not auto generated by Harmony
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DeviceInformation_SetCallbackFunction(void);

/** @brief SettingScreen_DeviceInformation_SetSetting
 *  @param [in] E_DeviceInformationSetting deviceInformationSetting
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DeviceInformation_SetSetting(E_DeviceInformationSetting deviceInformationSetting);

/** @brief SettingScreen_DeviceInformation_GetSetting
 *  @param [in] None
 *  @param [out] None
 *  @return E_DeviceInformationSetting
 */
E_DeviceInformationSetting SettingScreen_DeviceInformation_GetSetting();

/** @brief SettingScreen_DeviceInformation_DisplayMainInfo
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DeviceInformation_DisplayMainInfo();

/** @brief SettingScreen_DeviceInformation_DisplayMainInfo
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DeviceInformation_DisplayChamberInfo();

/** @brief SettingScreen_DeviceInformation_DisplayMainInfo
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DeviceInformation_DisplayCradleInfo();

/** @brief SettingScreen_DeviceInformation_DisplayMainInfo
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DeviceInformation_DisplayCircuitInfo();

#endif

/* end of file */
