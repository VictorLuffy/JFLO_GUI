/** @file SettingScreen_SpO2AlarmLimitSetting.h
 *  @brief The interfaces for display spo2 alarm limit setting
 *  @author Trac Truong
 */

#ifndef SETTINGSCREEN_SPO2ALARMLIMITSETTING_H
#define	SETTINGSCREEN_SPO2ALARMLIMITSETTING_H

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
#endif

#include "Gui/Setting.h"
#include "Gui/LogInterface.h"
/** @brief SettingScreen_OxySourceSetting_Init
 *      This init data when open the setting
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SpO2AlarmLimitSetting_Init();

/** @brief SettingScreen_SpO2AlarmLimitSetting_Display
 *      This control for display wifi setting
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SpO2AlarmLimitSetting_Display();

/** @brief SettingScreen_SpO2AlarmLimitSetting_SetCallbackFunction
 *      This set callback function for some event that not auto generated by Harmony
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SpO2AlarmLimitSetting_SetCallbackFunction(void);

/** @brief SettingScreen_SpO2AlarmLimitSetting_NextSetting
 *      This function is switch next language setting
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SpO2AlarmLimitSetting_Inc();

/** @brief SettingScreen_SpO2AlarmLimitSetting_PrevSetting
 *      This function is switch previous language setting
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SpO2AlarmLimitSetting_Dec();

/** @brief SettingScreen_SpO2AlarmLimitSetting_SetSetting
 *      This function is use to set setting
 *  @param [in] int s
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SpO2AlarmLimitSetting_SetSetting(int s);

/** @brief SettingScreen_SpO2AlarmLimitSetting_GetSetting
 *      This function is use to get setting
 *  @param [in] None
 *  @param [out] None
 *  @return int
 */
int SettingScreen_SpO2AlarmLimitSetting_GetSetting();

/** @brief SettingScreen_SpO2AlarmLimitSetting_CheckDataChange
 *      This check if this setting is changed
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
bool SettingScreen_SpO2AlarmLimitSetting_CheckDataChange();

/** @brief SettingScreen_SpO2AlarmLimitSetting_SaveSetting
 *      This save the setting if the setting change
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SpO2AlarmLimitSetting_SaveSetting();

/** @brief SettingScreen_SpO2AlarmLimitSetting_DiscardSetting
 *      This discard the setting change
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */

void SettingScreen_SpO2AlarmLimitSetting_DiscardSetting();

#endif

/* end of file */
