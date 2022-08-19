/** @file SettingScreen_DeviceInformation.c
 *  @brief The interfaces for display device information
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "SettingScreen_DeviceInformation.h"

#ifdef UNIT_TEST
laLabelWidget* SC_DeviceInformationMainLabel = (laLabelWidget*)1;
laLabelWidget* SC_DeviceInformationUnderbarPanel = (laLabelWidget*)2;
laLabelWidget* SC_DeviceInformationChamberLabel = (laLabelWidget*)3;
laLabelWidget* SC_DeviceInformationCradleLabel = (laLabelWidget*)4;
laLabelWidget* SC_DeviceInformationCircuitLabel = (laLabelWidget*)5;
GFXU_FontAsset AbelRegular_S18_Bold_Internal;
laLabelWidget* SC_DeviceInformationLabel_1 = (laLabelWidget*)6;
laLabelWidget* SC_DeviceInformationLabel_2 = (laLabelWidget*)7;
laLabelWidget* SC_DeviceInformationLabel_3 = (laLabelWidget*)8;
laLabelWidget* SC_DeviceInformationLabel_4 = (laLabelWidget*)9;
laLabelWidget* SC_DeviceInformationLabel_5 = (laLabelWidget*)10;
laLabelWidget* SC_DeviceInformationLabel_6 = (laLabelWidget*)11;
#endif

extern void SettingScreen_SetSettingScreenUpdate(bool f);
static int s_deviceInformationSetting = -1;
static int s_deviceInformationSettingDisplay = -1;

void SettingScreen_DeviceInformation_Init()
{
    s_deviceInformationSetting = eMainDeviceInformationSetting;
    s_deviceInformationSettingDisplay = -1;
}

void SettingScreen_DeviceInformation_Display()
{
    if (s_deviceInformationSettingDisplay == s_deviceInformationSetting)
    {
        return;
    }
    int32_t x;
    switch (s_deviceInformationSetting)
    {
        case eMainDeviceInformationSetting:
            x = DisplayControl_GetCenterPostion((laWidget*)SC_DeviceInformationMainLabel, (laWidget*)SC_DeviceInformationUnderbarPanel);
            laWidget_SetX((laWidget*)SC_DeviceInformationUnderbarPanel, x);
            SettingScreen_DeviceInformation_DisplayMainInfo();
            break;
        case eChamberDeviceInformationSetting:
            x = DisplayControl_GetCenterPostion((laWidget*)SC_DeviceInformationChamberLabel, (laWidget*)SC_DeviceInformationUnderbarPanel);
            laWidget_SetX((laWidget*)SC_DeviceInformationUnderbarPanel, x);
            SettingScreen_DeviceInformation_DisplayChamberInfo();
            break;
        case eCradleDeviceInformationSetting:
            x = DisplayControl_GetCenterPostion((laWidget*)SC_DeviceInformationCradleLabel, (laWidget*)SC_DeviceInformationUnderbarPanel);
            laWidget_SetX((laWidget*)SC_DeviceInformationUnderbarPanel, x);
            SettingScreen_DeviceInformation_DisplayCradleInfo();
            break;
        case eCircuitDeviceInformationSetting:
            x = DisplayControl_GetCenterPostion((laWidget*)SC_DeviceInformationCircuitLabel, (laWidget*)SC_DeviceInformationUnderbarPanel);
            laWidget_SetX((laWidget*)SC_DeviceInformationUnderbarPanel, x);
            SettingScreen_DeviceInformation_DisplayCircuitInfo();
            break;
        default:
            break;            
    }
    s_deviceInformationSettingDisplay = s_deviceInformationSetting;
}

void SC_DeviceInformationMainLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;
    SettingScreen_DeviceInformation_SetSetting(eMainDeviceInformationSetting);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_DeviceInformationChamberLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;

    SettingScreen_DeviceInformation_SetSetting(eChamberDeviceInformationSetting);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_DeviceInformationCradleLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;

    SettingScreen_DeviceInformation_SetSetting(eCradleDeviceInformationSetting);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_DeviceInformationCircuitLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;

    SettingScreen_DeviceInformation_SetSetting(eCircuitDeviceInformationSetting);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SettingScreen_DeviceInformation_SetCallbackFunction(void)
{
    // Assigning the callback function to widget, these event cannot auto add by generator
    laWidget_OverrideTouchDownEvent((laWidget*)SC_DeviceInformationMainLabel, &SC_DeviceInformationMainLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_DeviceInformationChamberLabel, &SC_DeviceInformationChamberLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_DeviceInformationCradleLabel, &SC_DeviceInformationCradleLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_DeviceInformationCircuitLabel, &SC_DeviceInformationCircuitLabel_PressedEvent);

}

void SettingScreen_DeviceInformation_SetSetting(E_DeviceInformationSetting deviceInformationSetting)
{
    if (deviceInformationSetting < 0 || deviceInformationSetting >= eNoOfDeviceInformationSetting)
        return;
    s_deviceInformationSetting = deviceInformationSetting;    
    if (s_deviceInformationSettingDisplay != s_deviceInformationSetting)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}
E_DeviceInformationSetting SettingScreen_DeviceInformation_GetSetting()
{
    return s_deviceInformationSetting;
}

void SettingScreen_DeviceInformation_DisplayMainInfo()
{
    char strbuff[255];
    char strinfo[255];
    laString lastr;

    DeviceInformation_GetDeviceInfo(eMainSerialNoDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", SERIAL_NUMBER_STR, strinfo); //"JFL2104000001"
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_1, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eMainTotalRuntimeDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s %s", TOTAL_RUNTIME_STR, strinfo, "hours"); // "333"
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_2, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eMainSWVersionDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", MAIN_SOFTWARE_STR, strinfo); //"1.0.0");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_3, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eMainBootloaderVersionDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", MAIN_BOOTLOADER_STR, strinfo); //"1.0.0");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_4, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eSubSWVersionDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", SUB_SOFTWARE_STR, strinfo); //"1.0.0");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_5, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eSubBootloaderVersionDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", SUB_BOOTLOADER_STR, strinfo); //"1.0.0");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_6, lastr);
    laString_Destroy(&lastr);

}

void SettingScreen_DeviceInformation_DisplayChamberInfo()
{
    char strbuff[255];
    char strinfo[255];
    laString lastr;

    DeviceInformation_GetDeviceInfo(eChamberSerialNoDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", SERIAL_NUMBER_STR, strinfo); //"JFH2104000001");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_1, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eChamberTotalRuntimeDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s %s", TOTAL_RUNTIME_STR, strinfo, "hours");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_2, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eChamberSWVersionDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", SOFTWARE_STR, strinfo); //"1.0.0");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_3, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eChamberBootloaderVersionDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", BOOTLOADER_STR, strinfo); //"1.0.0");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_4, lastr);
    laString_Destroy(&lastr);

    sprintf(strbuff, "%s","");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_5, lastr);
    laString_Destroy(&lastr);

    sprintf(strbuff, "%s","");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_6, lastr);
    laString_Destroy(&lastr);

}

void SettingScreen_DeviceInformation_DisplayCradleInfo()
{
    char strbuff[255];
    char strinfo[255];
    laString lastr;

    DeviceInformation_GetDeviceInfo(eCradleSerialNoDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", SERIAL_NUMBER_STR, strinfo); //"JFC2104000001");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_1, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eCradleTotalRuntimeDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s %s", TOTAL_RUNTIME_STR, strinfo, "hours");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_2, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eCradleSWVersionDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", SOFTWARE_STR, strinfo); //"1.0.0");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_3, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eCradleBootloaderVersionDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", BOOTLOADER_STR, strinfo); //"1.0.0");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_4, lastr);
    laString_Destroy(&lastr);

    sprintf(strbuff, "%s","");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_5, lastr);
    laString_Destroy(&lastr);

    sprintf(strbuff, "%s","");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_6, lastr);
    laString_Destroy(&lastr);

}

void SettingScreen_DeviceInformation_DisplayCircuitInfo()
{
    char strbuff[255];
    char strinfo[255];
    laString lastr;

    DeviceInformation_GetDeviceInfo(eCircuitSerialNoDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", SERIAL_NUMBER_STR, strinfo); //"JFC2104000001");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_1, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eCircuitTypeDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", TYPE_STR, strinfo); //"Adult");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_2, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eCircuitStartOfUseNoDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s", START_OF_USE_STR, strinfo); //"2022/04/21");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_3, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eCircuitUsageTimeDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s %s", USAGE_TIME_STR, strinfo,"hours");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_4, lastr);
    laString_Destroy(&lastr);

    DeviceInformation_GetDeviceInfo(eCircuitFactoryDeviceInfoId, strinfo);
    sprintf(strbuff, "%s %s",FACTORY_STR,strinfo); //"Metran Japan");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_5, lastr);
    laString_Destroy(&lastr);

    sprintf(strbuff, "%s","");
    lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S18_Bold_Internal);
    laLabelWidget_SetText(SC_DeviceInformationLabel_6, lastr);
    laString_Destroy(&lastr);

}

// end of file
