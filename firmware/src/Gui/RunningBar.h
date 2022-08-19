/** @file RunningBar.h
 *  @brief The interfaces for display running bar
 *  @author Trac Truong
 */

#ifndef RUNNINGBAR_H
#define	RUNNINGBAR_H

/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef UNIT_TEST
#include "system_config.h"
#include "system_definitions.h"
#include "gfx/hal/inc/gfx_context.h"
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
    extern uint32_t stringTable ;
    extern uint32_t JFLO_NoticeRunningArea_Scheme ;

#endif

#include "Gui/Setting.h"

#define RUNNINGBAR_UPDATE_CYCLE_MS 1000

/** @brief RunningBar_DisplayRunningTime
 *      This control for display running time
  *  @param [in] bool isForceUpdate : update without checking if current tick over the DATETIME_UPDATE_CYCLE_MS
 *  @param [out] None
 *  @return None
 */
void RunningBar_DisplayRunningTime(bool isForceUpdate);


#endif

/* end of file */
