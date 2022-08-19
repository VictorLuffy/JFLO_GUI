/** @file SettingScreen_DataLog.h
 *  @brief The interfaces for display data log
 *  @author Trac Truong
 */

#ifndef SETTINGSCREEN_DATALOG_H
#define	SETTINGSCREEN_DATALOG_H

/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

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
    typedef laWidget laImageWidget;
    typedef laWidget GFXU_ImageAsset;
    typedef laWidget laTextFieldWidget;

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
    laResult laImageWidget_SetImage(laImageWidget* img,
                                               GFXU_ImageAsset* imgAst);
    laResult laTextFieldWidget_SetText(laTextFieldWidget* txt, laString str);

    typedef uint32_t laScheme;
    void laWidget_SetScheme(laWidget* wgt, laScheme* scheme);
    laString laString_CreateFromID(uint32_t id);
    void laString_Initialize(laString* str);
    void laString_Destroy(laString* str);
    typedef uint32_t GFXU_StringTableAsset;
    GFXU_FontAsset* GFXU_StringFontIndexLookup(GFXU_StringTableAsset* table,
                                                          uint32_t stringID,
                                                          uint32_t languageID);
    void laString_Append(laString* dst, const laString* src);
    typedef uint32_t TickType_t;
    TickType_t xTaskGetTickCount( void );

    void SettingScreen_SetSettingScreenUpdate(bool f);
    int32_t DisplayControl_GetCenterPostion(laWidget* widgetA, laWidget* widgetB);
    TickType_t DisplayControl_CalculateDeltaTick(TickType_t current, TickType_t last);
    uint32_t DisplayControl_GetRunningTime(void);
    uint8_t DisplayControl_GetActiveScreenIndex();
    void StatusBar_SetAlarmInfo(bool f);

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

    #define string_text_Running 1
    #define MainScreen_ID    2
    #define SettingScreen_ID    3
    extern laWidget* panelNotice ;
    extern laLabelWidget* SC_RunningTimeLabel;
    extern laLabelWidget* lbInfo_NoticeArea;
    extern uint32_t string_Nums_S18_Nor ;
    extern uint32_t string_Nums_BebasNeueBook_S20_Bold;
    extern uint32_t stringTable ;
    extern uint32_t JFLO_NoticeRunningArea_Scheme ;
    extern laImageWidget* imgEXTBattery;
    extern laImageWidget* SC_ExternalBatteryIcon;
    extern uint32_t stringTable ;
    extern laImageWidget* imgEXTBattery ;
    extern laImageWidget* SC_ExternalBatteryIcon ;
    extern laImageWidget* imgINTBattery ;
    extern laImageWidget* SC_InternalBatteryIcon  ;
    extern laImageWidget* imgSocket  ;
    extern laWidget* SC_AlarmInfoButton  ;
    extern laWidget *imgWifi ;
    extern laWidget *SC_WifiIcon;
    extern laWidget *SC_PowerACIcon ;
    extern laWidget *btnInfo;
    extern laWidget *tfCurrentTime;
    extern laWidget *SC_DateTimeTextField;
    extern GFXU_ImageAsset iconExtBattery_0;
    extern GFXU_ImageAsset iconExtBattery_1;
    extern GFXU_ImageAsset iconExtBattery_2;
    extern GFXU_ImageAsset iconExtBattery_3;
    extern GFXU_ImageAsset iconExtBattery_4;
    extern GFXU_ImageAsset iconIntBattery_0;
    extern GFXU_ImageAsset iconIntBattery_1;
    extern GFXU_ImageAsset iconIntBattery_2;
    extern GFXU_ImageAsset iconIntBattery_3;
    extern GFXU_ImageAsset iconIntBattery_4;
    extern GFXU_ImageAsset Icon_InternalBatteryNoInserted;
    extern GFXU_ImageAsset iconWifi_0;
    extern GFXU_ImageAsset iconWifi_1;
    extern GFXU_ImageAsset iconWifi_2;
    extern GFXU_ImageAsset iconWifi_3;
    extern GFXU_ImageAsset iconWifi_4;
    extern GFXU_ImageAsset iconSocket;
    extern GFXU_ImageAsset Icon_SocketDisconnected;
    extern uint32_t string_Nums_BebasNeueBook_S20_Bold;
    extern laLabelWidget* SC_DataLogSettingAlarmLabel ;
    extern laLabelWidget* SC_DataLogSettingEventLabel ;
    extern laLabelWidget* SC_DataLogSettingTimeLabel_1 ;
    extern laLabelWidget* SC_DataLogSettingIndicator_1 ;
    extern laLabelWidget* SC_DataLogSettingTitleLabel_1 ;
    extern laLabelWidget* SC_DataLogSettingStateLabel_1 ;
    extern laLabelWidget* SC_DataLogSettingTimeLabel_2 ;
    extern laLabelWidget* SC_DataLogSettingIndicator_2 ;
    extern laLabelWidget* SC_DataLogSettingTitleLabel_2 ;
    extern laLabelWidget* SC_DataLogSettingStateLabel_2 ;
    extern laLabelWidget* SC_DataLogSettingTimeLabel_3 ;
    extern laLabelWidget* SC_DataLogSettingIndicator_3 ;
    extern laLabelWidget* SC_DataLogSettingTitleLabel_3 ;
    extern laLabelWidget* SC_DataLogSettingStateLabel_3 ;
    extern laLabelWidget* SC_DataLogSettingTimeLabel_4 ;
    extern laLabelWidget* SC_DataLogSettingIndicator_4 ;
    extern laLabelWidget* SC_DataLogSettingTitleLabel_4 ;
    extern laLabelWidget* SC_DataLogSettingStateLabel_4 ;
    extern laLabelWidget* SC_DataLogSettingTimeLabel_5 ;
    extern laLabelWidget* SC_DataLogSettingIndicator_5 ;
    extern laLabelWidget* SC_DataLogSettingTitleLabel_5 ;
    extern laLabelWidget* SC_DataLogSettingStateLabel_5 ;
    extern laWidget* SC_DataLogSettingUnderbarPanel ;
    extern laWidget* SC_DataLogSettingTitleLabel ;
    extern laWidget* SC_DataLogSettingStateLabel ;
    extern uint32_t string_text_SettingScreen_DataLog_AlarmTitle ;
    extern uint32_t string_text_SettingScreen_DataLog_State ;
    extern GFXU_FontAsset AbelRegular_S20_Bold_Internal;
    extern GFXU_FontAsset AbelRegular_S12_Bold_Internal;
    extern laWidget* SC_DataLogSettingPageNumberLabel ;
    extern uint32_t string_text_Nullstring ;
    extern laScheme JFLO_AlarmIndicatorLow1Priority_Scheme;
    extern laScheme JFLO_AlarmIndicatorMediumPriority_Scheme;
    extern laScheme JFLO_AlarmIndicatorHighPriority_Scheme;
    extern uint32_t string_text_SettingScreen_DataLog_EventName ;
    extern uint32_t string_text_SettingScreen_DataLog_Refer ;
#endif

#include "Gui/LogInterface.h"
#include "Gui/LogMgr.h"

#define MAX_LOG_IN_PAGE             5
#define MAX_LOG_DISPLAY             1000

#define MAX_ALARM_IN_PAGE           MAX_LOG_IN_PAGE
#define MAX_ALARM_DISPLAY           100

#define ALARM_TITLE_ITEM_POS_X      88
#define EVENT_NAME_ITEM_POS_X      85

#define ALARM_LOG_DATA_STATUS 0
#define ALARM_LOG_DATA_PRIORITY 1

typedef enum {
    eAlarmDataLogSetting, /**< alarm data log screen */  
    eEventDataLogSetting, /**< event data log screen */  
    eNoOfDataLogSetting, /**< no of enum */  
} E_DataLogSetting;

typedef struct {
    laLabelWidget* timeWidget; /**< time widget */  
    laWidget* indicatorWidget;  /**< indicator widget */  
    laLabelWidget* nameWidget; /**< alarm log / event log content widget */  
    laLabelWidget* dataWidget; /**< reference data widget */  
} LogItem_Struct;


/** @brief SettingScreen_DataLog_Init
 *      This init data
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_Init();

/** @brief SettingScreen_DataLog_Display
 *      This control for display wifi setting
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_Display();

/** @brief SettingScreen_DataLog_SetCallbackFunction
 *      This set callback function for some event that not auto generated by Harmony
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_SetCallbackFunction(void);

/** @brief SettingScreen_DataLog_SetSetting
 *  @param [in] E_DataLogSetting s
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_SetSetting(E_DataLogSetting s);

/** @brief SettingScreen_DataLog_GetSetting
 *  @param [in] None
 *  @param [out] None
 *  @return E_DataLogSetting
 */
E_DataLogSetting SettingScreen_DataLog_GetSetting();

/** @brief SettingScreen_DataLog_DisplayAlarm
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_DisplayAlarm();

/** @brief SettingScreen_DataLog_DisplayEvent
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_DisplayEvent();

/** @brief SettingScreen_DataLog_InitAlarmData
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_InitAlarmData();

/** @brief SettingScreen_DataLog_InitEventData
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_InitEventData();

/** @brief SettingScreen_DataLog_SetAlarmIndicator
 *  @param [in] laWidget* w
 *  @param [in] uint8_t data : priority
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_SetAlarmIndicator(laWidget* w, uint8_t data);

/** @brief SettingScreen_DataLog_SetInitDisplayData
 *      This is use for notify re-loading the display data
 *  @param [in] bool f
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_SetInitDisplayData(bool f);

/** @brief SettingScreen_DataLog_GetSetting
 *  @param [in] None
 *  @param [out] None
 *  @return E_DataLogSetting
 */
bool SettingScreen_DataLog_GetInitDisplayData();

/** @brief SettingScreen_DataLog_NextPage
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_NextPage();

/** @brief SettingScreen_DataLog_PrevPage
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DataLog_PrevPage();

#endif

/* end of file */
