/** @file GuiDefine.h
 *  @brief Contains define of GUI
 *  @author Viet Le
 */

#ifndef GUIDEFINE_H
#define	GUIDEFINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#ifndef UNIT_TEST
#include "system_config.h"
#include "system_definitions.h"
#include "gfx/hal/inc/gfx_context.h"
#endif

#include "mm.h"
#include "System/USBInterface.h"
#include "../Device/AudioDefine.h"

/** @brief Get tick */
#define GET_TICKS() __builtin_mfc0(9, 0)
   
///** @brief 2 CRC bytes  */
#define    CRC_BYTE                             (2)

/** @brief Size of image file */
#define     SIZE_GRAPHIC_IMAGE                      (50152)

/** @brief Size of font file */
#define     SIZE_GRAPHIC_ABELFONT                   (46080)

/** @brief Size of font file */
#define     SIZE_GRAPHIC_BEBASFONT                  (95687)

//File Name on SQI Flash
#define    FILE_NAME_GRAPHIC_IMAGE                              "SQI_Images_CRC.bin"
#define    FILE_NAME_GRAPHIC_ABELFONT                           "SQI_AbelFont_CRC.bin"
#define    FILE_NAME_GRAPHIC_BEBASFONT                          "SQI_BebasFont_CRC.bin"

#define FONT_IMAGE_TOTAL_FILES 3

// Intro video file define
#define INTRO_VIDEO_START_INDEX FONT_IMAGE_TOTAL_FILES
#define INTRO_VIDEO_TOTAL_FILES 50
// Alarm video file define
#define ALARM_E001_VIDEO_DATA_START_ID 0
#define ALARM_E001_VIDEO_FILELIST_START_ID INTRO_VIDEO_START_INDEX+INTRO_VIDEO_TOTAL_FILES
#define ALARM_E001_TOTAL_FILES 55
#define ALARM_E001_FRAME_DATA_PTR(i) alarmVideoInputData[ALARM_E001_VIDEO_DATA_START_ID+i]
#define ALARM_E001_FRAME_DATA_SIZE(i) g_fileList[ALARM_E001_VIDEO_FILELIST_START_ID+i].fileSize

#define ALARM_E002_VIDEO_DATA_START_ID ALARM_E001_TOTAL_FILES
#define ALARM_E002_VIDEO_FILELIST_START_ID ALARM_E001_VIDEO_FILELIST_START_ID+ALARM_E001_TOTAL_FILES
#define ALARM_E002_TOTAL_FILES 44
#define ALARM_E002_FRAME_DATA_PTR(i) alarmVideoInputData[ALARM_E002_VIDEO_DATA_START_ID+i]
#define ALARM_E002_FRAME_DATA_SIZE(i) g_fileList[ALARM_E002_VIDEO_FILELIST_START_ID+i].fileSize

#define ALARM_E003_VIDEO_DATA_START_ID ALARM_E001_TOTAL_FILES+ALARM_E002_TOTAL_FILES
#define ALARM_E003_VIDEO_FILELIST_START_ID ALARM_E002_VIDEO_FILELIST_START_ID+ALARM_E002_TOTAL_FILES
#define ALARM_E003_TOTAL_FILES 18
#define ALARM_E003_FRAME_DATA_PTR(i) alarmVideoInputData[ALARM_E003_VIDEO_DATA_START_ID+i]
#define ALARM_E003_FRAME_DATA_SIZE(i) g_fileList[ALARM_E003_VIDEO_FILELIST_START_ID+i].fileSize

#define ALARM_E007_VIDEO_DATA_START_ID ALARM_E001_TOTAL_FILES+ALARM_E002_TOTAL_FILES+ALARM_E003_TOTAL_FILES
#define ALARM_E007_VIDEO_FILELIST_START_ID ALARM_E003_VIDEO_FILELIST_START_ID+ALARM_E003_TOTAL_FILES
#define ALARM_E007_TOTAL_FILES 55
#define ALARM_E007_FRAME_DATA_PTR(i) alarmVideoInputData[ALARM_E007_VIDEO_DATA_START_ID+i]
#define ALARM_E007_FRAME_DATA_SIZE(i) g_fileList[ALARM_E007_VIDEO_FILELIST_START_ID+i].fileSize

#define ALARM_E008_VIDEO_DATA_START_ID ALARM_E001_TOTAL_FILES+ALARM_E002_TOTAL_FILES+ALARM_E003_TOTAL_FILES+ALARM_E007_TOTAL_FILES
#define ALARM_E008_VIDEO_FILELIST_START_ID ALARM_E007_VIDEO_FILELIST_START_ID+ALARM_E007_TOTAL_FILES
#define ALARM_E008_TOTAL_FILES 23
#define ALARM_E008_FRAME_DATA_PTR(i) alarmVideoInputData[ALARM_E008_VIDEO_DATA_START_ID+i]
#define ALARM_E008_FRAME_DATA_SIZE(i) g_fileList[ALARM_E008_VIDEO_FILELIST_START_ID+i].fileSize

#define ALARM_E009_VIDEO_DATA_START_ID ALARM_E001_TOTAL_FILES+ALARM_E002_TOTAL_FILES+ALARM_E003_TOTAL_FILES+ALARM_E007_TOTAL_FILES+ALARM_E008_TOTAL_FILES
#define ALARM_E009_VIDEO_FILELIST_START_ID ALARM_E008_VIDEO_FILELIST_START_ID+ALARM_E008_TOTAL_FILES
#define ALARM_E009_TOTAL_FILES 44
#define ALARM_E009_FRAME_DATA_PTR(i) alarmVideoInputData[ALARM_E009_VIDEO_DATA_START_ID+i]
#define ALARM_E009_FRAME_DATA_SIZE(i) g_fileList[ALARM_E009_VIDEO_FILELIST_START_ID+i].fileSize

/** @brief Total media files in UI */
#define    NUMBER_FILE_IMAGE_FONT               FONT_IMAGE_TOTAL_FILES \
                                                + AUDIO_TOTAL_FILE \
                                                +INTRO_VIDEO_TOTAL_FILES \
                                                +ALARM_E001_TOTAL_FILES \
                                                +ALARM_E002_TOTAL_FILES \
                                                +ALARM_E003_TOTAL_FILES \
//                                                +ALARM_E007_TOTAL_FILES \
//                                                +ALARM_E008_TOTAL_FILES \
//                                                +ALARM_E009_TOTAL_FILES 






/** @brief define struct file read */
typedef struct {
    uint8_t id;
    const char* fileName;       /**< File name in SQI*/
    uint8_t* data; /**<data */
    int32_t fileSize;
} FILE_ITEM_t;

enum
{
    eImageAssetId,
    eFontAssetId,
    eIntroVideoAssetId,
    eAlarmVideoAssetId,
    eAudioLow260msAssetId,
    eAudioMedium260msAssetId,
    eAudioHigh210msAssetId
};

// Storage of image on UI 
extern uint8_t g_graphicImage[] ;

// Storage of font on UI
extern uint8_t g_graphicAbelFont[];

// Storage of font on UI
extern uint8_t g_graphicBebasFont[];

// Storage of media properties in UI
uint8_t* introVideoInputData[INTRO_VIDEO_TOTAL_FILES]; // this can be free after boot up finish

// Storage of media properties in UI
uint8_t* alarmVideoInputData[NUMBER_FILE_IMAGE_FONT - FONT_IMAGE_TOTAL_FILES - INTRO_VIDEO_TOTAL_FILES];

// Storage of asset list on UI
// size without CRC
const FILE_ITEM_t g_fileList[NUMBER_FILE_IMAGE_FONT] = {
    {eImageAssetId, FILE_NAME_GRAPHIC_IMAGE, g_graphicImage, SIZE_GRAPHIC_IMAGE},
    {eFontAssetId, FILE_NAME_GRAPHIC_ABELFONT, g_graphicAbelFont, SIZE_GRAPHIC_ABELFONT},
    {eFontAssetId, FILE_NAME_GRAPHIC_BEBASFONT, g_graphicBebasFont, SIZE_GRAPHIC_BEBASFONT},
    // data will contain the index
    {eIntroVideoAssetId,"I_01", (uint8_t*)0 ,572},
    {eIntroVideoAssetId,"I_02", (uint8_t*)1 ,427},
    {eIntroVideoAssetId,"I_03", (uint8_t*)2 ,849},
    {eIntroVideoAssetId,"I_04", (uint8_t*)3 ,989},
    {eIntroVideoAssetId,"I_05", (uint8_t*)4 ,1337},
    {eIntroVideoAssetId,"I_06", (uint8_t*)5 ,1888},
    {eIntroVideoAssetId,"I_07", (uint8_t*)6 ,1996},
    {eIntroVideoAssetId,"I_08", (uint8_t*)7 ,570},
    {eIntroVideoAssetId,"I_09", (uint8_t*)8 ,710},
    {eIntroVideoAssetId,"I_10", (uint8_t*)9 ,1169},
    {eIntroVideoAssetId,"I_11", (uint8_t*)10 ,2163},
    {eIntroVideoAssetId,"I_12", (uint8_t*)11 ,3212},
    {eIntroVideoAssetId,"I_13", (uint8_t*)12 ,4265},
    {eIntroVideoAssetId,"I_14", (uint8_t*)13 ,5870},
    {eIntroVideoAssetId,"I_15", (uint8_t*)14 ,7735},
    {eIntroVideoAssetId,"I_16", (uint8_t*)15 ,8743},
    {eIntroVideoAssetId,"I_17", (uint8_t*)16 ,9664},
    {eIntroVideoAssetId,"I_18", (uint8_t*)17 ,10679},
    {eIntroVideoAssetId,"I_19", (uint8_t*)18 ,11805},
    {eIntroVideoAssetId,"I_20", (uint8_t*)19 ,14143},
    {eIntroVideoAssetId,"I_21", (uint8_t*)20 ,17526},
    {eIntroVideoAssetId,"I_22", (uint8_t*)21 ,19140},
    {eIntroVideoAssetId,"I_23", (uint8_t*)22 ,18318},
    {eIntroVideoAssetId,"I_24", (uint8_t*)23 ,17503},
    {eIntroVideoAssetId,"I_25", (uint8_t*)24 ,17543},
    {eIntroVideoAssetId,"I_26", (uint8_t*)25 ,17965},
    {eIntroVideoAssetId,"I_27", (uint8_t*)26 ,16953},
    {eIntroVideoAssetId,"I_28", (uint8_t*)27 ,14669},
    {eIntroVideoAssetId,"I_29", (uint8_t*)28 ,11491},
    {eIntroVideoAssetId,"I_30", (uint8_t*)29 ,11342},
    {eIntroVideoAssetId,"I_31", (uint8_t*)30 ,12094},
    {eIntroVideoAssetId,"I_32", (uint8_t*)31 ,12344},
    {eIntroVideoAssetId,"I_33", (uint8_t*)32 ,12356},
    {eIntroVideoAssetId,"I_34", (uint8_t*)33 ,12395},
    {eIntroVideoAssetId,"I_35", (uint8_t*)34 ,11978},
    {eIntroVideoAssetId,"I_36", (uint8_t*)35 ,12508},
    {eIntroVideoAssetId,"I_37", (uint8_t*)36 ,12320},
    {eIntroVideoAssetId,"I_38", (uint8_t*)37 ,14033},
    {eIntroVideoAssetId,"I_39", (uint8_t*)38 ,13225},
    {eIntroVideoAssetId,"I_40", (uint8_t*)39 ,9262},
    {eIntroVideoAssetId,"I_41", (uint8_t*)40 ,7740},
    {eIntroVideoAssetId,"I_42", (uint8_t*)41 ,8304},
    {eIntroVideoAssetId,"I_43", (uint8_t*)42 ,8513},
    {eIntroVideoAssetId,"I_44", (uint8_t*)43 ,8568},
    {eIntroVideoAssetId,"I_45", (uint8_t*)44 ,8600},
    {eIntroVideoAssetId,"I_46", (uint8_t*)45 ,8579},
    {eIntroVideoAssetId,"I_47", (uint8_t*)46 ,8742},
    {eIntroVideoAssetId,"I_48", (uint8_t*)47 ,8596},
    {eIntroVideoAssetId,"I_49", (uint8_t*)48 ,8594},
    {eIntroVideoAssetId,"I_50", (uint8_t*)49 ,8720},
    //
    {eAlarmVideoAssetId, "A1_01", (uint8_t*)0, 9824},
    {eAlarmVideoAssetId, "A1_02", (uint8_t*)1, 9822},
    {eAlarmVideoAssetId, "A1_03", (uint8_t*)2, 9822},
    {eAlarmVideoAssetId, "A1_04", (uint8_t*)3, 9827},
    {eAlarmVideoAssetId, "A1_05", (uint8_t*)4, 9822},
    {eAlarmVideoAssetId, "A1_06", (uint8_t*)5, 9822},
    {eAlarmVideoAssetId, "A1_07", (uint8_t*)6, 9822},
    {eAlarmVideoAssetId, "A1_08", (uint8_t*)7, 9822},
    {eAlarmVideoAssetId, "A1_09", (uint8_t*)8, 9822},
    {eAlarmVideoAssetId, "A1_10", (uint8_t*)9, 9817},
    {eAlarmVideoAssetId, "A1_11", (uint8_t*)10, 9817},
    {eAlarmVideoAssetId, "A1_12", (uint8_t*)11, 9818},
    {eAlarmVideoAssetId, "A1_13", (uint8_t*)12, 9821},
    {eAlarmVideoAssetId, "A1_14", (uint8_t*)13, 9821},
    {eAlarmVideoAssetId, "A1_15", (uint8_t*)14, 9821},
    {eAlarmVideoAssetId, "A1_16", (uint8_t*)15, 9906},
    {eAlarmVideoAssetId, "A1_17", (uint8_t*)16, 10479},
    {eAlarmVideoAssetId, "A1_18", (uint8_t*)17, 11151},
    {eAlarmVideoAssetId, "A1_19", (uint8_t*)18, 11932},
    {eAlarmVideoAssetId, "A1_20", (uint8_t*)19, 12758},
    {eAlarmVideoAssetId, "A1_21", (uint8_t*)20, 13123},
    {eAlarmVideoAssetId, "A1_22", (uint8_t*)21, 13473},
    {eAlarmVideoAssetId, "A1_23", (uint8_t*)22, 13653},
    {eAlarmVideoAssetId, "A1_24", (uint8_t*)23, 13741},
    {eAlarmVideoAssetId, "A1_25", (uint8_t*)24, 13752},
    {eAlarmVideoAssetId, "A1_26", (uint8_t*)25, 13746},
    {eAlarmVideoAssetId, "A1_27", (uint8_t*)26, 13656},
    {eAlarmVideoAssetId, "A1_28", (uint8_t*)27, 13554},
    {eAlarmVideoAssetId, "A1_29", (uint8_t*)28, 13465},
    {eAlarmVideoAssetId, "A1_30", (uint8_t*)29, 13437},
    {eAlarmVideoAssetId, "A1_31", (uint8_t*)30, 13483},
    {eAlarmVideoAssetId, "A1_32", (uint8_t*)31, 13464},
    {eAlarmVideoAssetId, "A1_33", (uint8_t*)32, 13358},
    {eAlarmVideoAssetId, "A1_34", (uint8_t*)33, 13387},
    {eAlarmVideoAssetId, "A1_35", (uint8_t*)34, 13422},
    {eAlarmVideoAssetId, "A1_36", (uint8_t*)35, 13425},
    {eAlarmVideoAssetId, "A1_37", (uint8_t*)36, 13407},
    {eAlarmVideoAssetId, "A1_38", (uint8_t*)37, 13422},
    {eAlarmVideoAssetId, "A1_39", (uint8_t*)38, 13421},
    {eAlarmVideoAssetId, "A1_40", (uint8_t*)39, 13428},
    {eAlarmVideoAssetId, "A1_41", (uint8_t*)40, 13428},
    {eAlarmVideoAssetId, "A1_42", (uint8_t*)41, 13399},
    {eAlarmVideoAssetId, "A1_43", (uint8_t*)42, 13386},
    {eAlarmVideoAssetId, "A1_44", (uint8_t*)43, 13409},
    {eAlarmVideoAssetId, "A1_45", (uint8_t*)44, 13405},
    {eAlarmVideoAssetId, "A1_46", (uint8_t*)45, 13423},
    {eAlarmVideoAssetId, "A1_47", (uint8_t*)46, 13404},
    {eAlarmVideoAssetId, "A1_48", (uint8_t*)47, 13404},
    {eAlarmVideoAssetId, "A1_49", (uint8_t*)48, 13404},
    {eAlarmVideoAssetId, "A1_50", (uint8_t*)49, 13423},
    {eAlarmVideoAssetId, "A1_51", (uint8_t*)50, 13404},
    {eAlarmVideoAssetId, "A1_52", (uint8_t*)51, 13404},
    {eAlarmVideoAssetId, "A1_53", (uint8_t*)52, 13404},
    {eAlarmVideoAssetId, "A1_54", (uint8_t*)53, 13423},
    {eAlarmVideoAssetId, "A1_55", (uint8_t*)54, 13423},
    //
    {eAlarmVideoAssetId, "A2_01", (uint8_t*)55, 3267},
    {eAlarmVideoAssetId, "A2_02", (uint8_t*)56, 3376},
    {eAlarmVideoAssetId, "A2_03", (uint8_t*)57, 3383},
    {eAlarmVideoAssetId, "A2_04", (uint8_t*)58, 3443},
    {eAlarmVideoAssetId, "A2_05", (uint8_t*)59, 3475},
    {eAlarmVideoAssetId, "A2_06", (uint8_t*)60, 3649},
    {eAlarmVideoAssetId, "A2_07", (uint8_t*)61, 3769},
    {eAlarmVideoAssetId, "A2_08", (uint8_t*)62, 3934},
    {eAlarmVideoAssetId, "A2_09", (uint8_t*)63, 4020},
    {eAlarmVideoAssetId, "A2_10", (uint8_t*)64, 4270},
    {eAlarmVideoAssetId, "A2_11", (uint8_t*)65, 4573},
    {eAlarmVideoAssetId, "A2_12", (uint8_t*)66, 4921},
    {eAlarmVideoAssetId, "A2_13", (uint8_t*)67, 5343},
    {eAlarmVideoAssetId, "A2_14", (uint8_t*)68, 5881},
    {eAlarmVideoAssetId, "A2_15", (uint8_t*)69, 6375},
    {eAlarmVideoAssetId, "A2_16", (uint8_t*)70, 7003},
    {eAlarmVideoAssetId, "A2_17", (uint8_t*)71, 7295},
    {eAlarmVideoAssetId, "A2_18", (uint8_t*)72, 7535},
    {eAlarmVideoAssetId, "A2_19", (uint8_t*)73, 7640},
    {eAlarmVideoAssetId, "A2_20", (uint8_t*)74, 7533},
    {eAlarmVideoAssetId, "A2_21", (uint8_t*)75, 7571},
    {eAlarmVideoAssetId, "A2_22", (uint8_t*)76, 7552},
    {eAlarmVideoAssetId, "A2_23", (uint8_t*)77, 7543},
    {eAlarmVideoAssetId, "A2_24", (uint8_t*)78, 7560},
    {eAlarmVideoAssetId, "A2_25", (uint8_t*)79, 7585},
    {eAlarmVideoAssetId, "A2_26", (uint8_t*)80, 7582},
    {eAlarmVideoAssetId, "A2_27", (uint8_t*)81, 7595},
    {eAlarmVideoAssetId, "A2_28", (uint8_t*)82, 7617},
    {eAlarmVideoAssetId, "A2_29", (uint8_t*)83, 7574},
    {eAlarmVideoAssetId, "A2_30", (uint8_t*)84, 7546},
    {eAlarmVideoAssetId, "A2_31", (uint8_t*)85, 7518},
    {eAlarmVideoAssetId, "A2_32", (uint8_t*)86, 6981},
    {eAlarmVideoAssetId, "A2_33", (uint8_t*)87, 6861},
    {eAlarmVideoAssetId, "A2_34", (uint8_t*)88, 7419},
    {eAlarmVideoAssetId, "A2_35", (uint8_t*)89, 9332},
    {eAlarmVideoAssetId, "A2_36", (uint8_t*)90, 10759},
    {eAlarmVideoAssetId, "A2_37", (uint8_t*)91, 14578},
    {eAlarmVideoAssetId, "A2_38", (uint8_t*)92, 16365},
    {eAlarmVideoAssetId, "A2_39", (uint8_t*)93, 18667},
    {eAlarmVideoAssetId, "A2_40", (uint8_t*)94, 20366},
    {eAlarmVideoAssetId, "A2_41", (uint8_t*)95, 22789},
    {eAlarmVideoAssetId, "A2_42", (uint8_t*)96, 22349},
    {eAlarmVideoAssetId, "A2_43", (uint8_t*)97, 22547},
    {eAlarmVideoAssetId, "A2_44", (uint8_t*)98, 22454},
    //
    {eAlarmVideoAssetId, "A3_01", (uint8_t*)99, 24768},
    {eAlarmVideoAssetId, "A3_02", (uint8_t*)100, 24888},
    {eAlarmVideoAssetId, "A3_03", (uint8_t*)101, 24456},
    {eAlarmVideoAssetId, "A3_04", (uint8_t*)102, 24361},
    {eAlarmVideoAssetId, "A3_05", (uint8_t*)103, 24349},
    {eAlarmVideoAssetId, "A3_06", (uint8_t*)104, 24209},
    {eAlarmVideoAssetId, "A3_07", (uint8_t*)105, 24236},
    {eAlarmVideoAssetId, "A3_08", (uint8_t*)106, 24737},
    {eAlarmVideoAssetId, "A3_09", (uint8_t*)107, 24701},
    {eAlarmVideoAssetId, "A3_10", (uint8_t*)108, 24336},
    {eAlarmVideoAssetId, "A3_11", (uint8_t*)109, 24176},
    {eAlarmVideoAssetId, "A3_12", (uint8_t*)110, 24340},
    {eAlarmVideoAssetId, "A3_13", (uint8_t*)111, 24328},
    {eAlarmVideoAssetId, "A3_14", (uint8_t*)112, 24604},
    {eAlarmVideoAssetId, "A3_15", (uint8_t*)113, 24568},
    {eAlarmVideoAssetId, "A3_16", (uint8_t*)114, 24389},
    {eAlarmVideoAssetId, "A3_17", (uint8_t*)115, 24132},
    {eAlarmVideoAssetId, "A3_18", (uint8_t*)116, 24117},
    {eAudioLow260msAssetId, FILE_SOUND_800HZ_260MS_LOW, (uint8_t *)0, SIZE_SOUND_800HZ_260MS_LOW},
    {eAudioMedium260msAssetId,  FILE_SOUND_800HZ_260MS_MEDIUM, (uint8_t *)0, SIZE_SOUND_800HZ_260MS_MEDIUM},
    {eAudioHigh210msAssetId,    FILE_SOUND_800HZ_210MS_HIGH, (uint8_t *)0, SIZE_SOUND_800HZ_210MS_HIGH},
    //
//    {eAlarmVideoAssetId, "A7_01", (uint8_t*)117, 10495},
//    {eAlarmVideoAssetId, "A7_02", (uint8_t*)118, 10508},
//    {eAlarmVideoAssetId, "A7_03", (uint8_t*)119, 10509},
//    {eAlarmVideoAssetId, "A7_04", (uint8_t*)120, 10509},
//    {eAlarmVideoAssetId, "A7_05", (uint8_t*)121, 10510},
//    {eAlarmVideoAssetId, "A7_06", (uint8_t*)122, 10510},
//    {eAlarmVideoAssetId, "A7_07", (uint8_t*)123, 10510},
//    {eAlarmVideoAssetId, "A7_08", (uint8_t*)124, 10510},
//    {eAlarmVideoAssetId, "A7_09", (uint8_t*)125, 10510},
//    {eAlarmVideoAssetId, "A7_10", (uint8_t*)126, 10516},
//    {eAlarmVideoAssetId, "A7_11", (uint8_t*)127, 10516},
//    {eAlarmVideoAssetId, "A7_12", (uint8_t*)128, 10516},
//    {eAlarmVideoAssetId, "A7_13", (uint8_t*)129, 10524},
//    {eAlarmVideoAssetId, "A7_14", (uint8_t*)130, 10538},
//    {eAlarmVideoAssetId, "A7_15", (uint8_t*)131, 10506},
//    {eAlarmVideoAssetId, "A7_16", (uint8_t*)132, 10506},
//    {eAlarmVideoAssetId, "A7_17", (uint8_t*)133, 10553},
//    {eAlarmVideoAssetId, "A7_18", (uint8_t*)134, 10651},
//    {eAlarmVideoAssetId, "A7_19", (uint8_t*)135, 10730},
//    {eAlarmVideoAssetId, "A7_20", (uint8_t*)136, 10810},
//    {eAlarmVideoAssetId, "A7_21", (uint8_t*)137, 10924},
//    {eAlarmVideoAssetId, "A7_22", (uint8_t*)138, 11253},
//    {eAlarmVideoAssetId, "A7_23", (uint8_t*)139, 11691},
//    {eAlarmVideoAssetId, "A7_24", (uint8_t*)140, 12018},
//    {eAlarmVideoAssetId, "A7_25", (uint8_t*)141, 12461},
//    {eAlarmVideoAssetId, "A7_26", (uint8_t*)142, 12745},
//    {eAlarmVideoAssetId, "A7_27", (uint8_t*)143, 12495},
//    {eAlarmVideoAssetId, "A7_28", (uint8_t*)144, 12131},
//    {eAlarmVideoAssetId, "A7_29", (uint8_t*)145, 11736},
//    {eAlarmVideoAssetId, "A7_30", (uint8_t*)146, 11385},
//    {eAlarmVideoAssetId, "A7_31", (uint8_t*)147, 10739},
//    {eAlarmVideoAssetId, "A7_32", (uint8_t*)148, 10362},
//    {eAlarmVideoAssetId, "A7_33", (uint8_t*)149, 10169},
//    {eAlarmVideoAssetId, "A7_34", (uint8_t*)150, 10068},
//    {eAlarmVideoAssetId, "A7_35", (uint8_t*)151, 9871},
//    {eAlarmVideoAssetId, "A7_36", (uint8_t*)152, 9723},
//    {eAlarmVideoAssetId, "A7_37", (uint8_t*)153, 9685},
//    {eAlarmVideoAssetId, "A7_38", (uint8_t*)154, 9684},
//    {eAlarmVideoAssetId, "A7_39", (uint8_t*)155, 9675},
//    {eAlarmVideoAssetId, "A7_40", (uint8_t*)156, 9727},
//    {eAlarmVideoAssetId, "A7_41", (uint8_t*)157, 9739},
//    {eAlarmVideoAssetId, "A7_42", (uint8_t*)158, 9704},
//    {eAlarmVideoAssetId, "A7_43", (uint8_t*)159, 9729},
//    {eAlarmVideoAssetId, "A7_44", (uint8_t*)160, 9729},
//    {eAlarmVideoAssetId, "A7_45", (uint8_t*)161, 9738},
//    {eAlarmVideoAssetId, "A7_46", (uint8_t*)162, 9739},
//    {eAlarmVideoAssetId, "A7_47", (uint8_t*)163, 9739},
//    {eAlarmVideoAssetId, "A7_48", (uint8_t*)164, 9739},
//    {eAlarmVideoAssetId, "A7_49", (uint8_t*)165, 9738},
//    {eAlarmVideoAssetId, "A7_50", (uint8_t*)166, 9739},
//    {eAlarmVideoAssetId, "A7_51", (uint8_t*)167, 9739},
//    {eAlarmVideoAssetId, "A7_52", (uint8_t*)168, 9739},
//    {eAlarmVideoAssetId, "A7_53", (uint8_t*)169, 9738},
//    {eAlarmVideoAssetId, "A7_54", (uint8_t*)170, 9739},
//    {eAlarmVideoAssetId, "A7_55", (uint8_t*)171, 9738},
//    //
//    {eAlarmVideoAssetId, "A8_01", (uint8_t*)172, 15369},
//    {eAlarmVideoAssetId, "A8_02", (uint8_t*)173, 15381},
//    {eAlarmVideoAssetId, "A8_03", (uint8_t*)174, 15480},
//    {eAlarmVideoAssetId, "A8_04", (uint8_t*)175, 15620},
//    {eAlarmVideoAssetId, "A8_05", (uint8_t*)176, 15701},
//    {eAlarmVideoAssetId, "A8_06", (uint8_t*)177, 15500},
//    {eAlarmVideoAssetId, "A8_07", (uint8_t*)178, 15363},
//    {eAlarmVideoAssetId, "A8_08", (uint8_t*)179, 15055},
//    {eAlarmVideoAssetId, "A8_09", (uint8_t*)180, 14977},
//    {eAlarmVideoAssetId, "A8_10", (uint8_t*)181, 14641},
//    {eAlarmVideoAssetId, "A8_11", (uint8_t*)182, 14456},
//    {eAlarmVideoAssetId, "A8_12", (uint8_t*)183, 13999},
//    {eAlarmVideoAssetId, "A8_13", (uint8_t*)184, 13927},
//    {eAlarmVideoAssetId, "A8_14", (uint8_t*)185, 13409},
//    {eAlarmVideoAssetId, "A8_15", (uint8_t*)186, 13171},
//    {eAlarmVideoAssetId, "A8_16", (uint8_t*)187, 12825},
//    {eAlarmVideoAssetId, "A8_17", (uint8_t*)188, 12763},
//    {eAlarmVideoAssetId, "A8_18", (uint8_t*)189, 12678},
//    {eAlarmVideoAssetId, "A8_19", (uint8_t*)190, 12664},
//    {eAlarmVideoAssetId, "A8_20", (uint8_t*)191, 12680},
//    {eAlarmVideoAssetId, "A8_21", (uint8_t*)192, 12721},
//    {eAlarmVideoAssetId, "A8_22", (uint8_t*)193, 12688},
//    {eAlarmVideoAssetId, "A8_23", (uint8_t*)194, 12727},
//    //
//    {eAlarmVideoAssetId, "A9_01", (uint8_t*)195, 11534},
//    {eAlarmVideoAssetId, "A9_02", (uint8_t*)196, 11522},
//    {eAlarmVideoAssetId, "A9_03", (uint8_t*)197, 11515},
//    {eAlarmVideoAssetId, "A9_04", (uint8_t*)198, 11515},
//    {eAlarmVideoAssetId, "A9_05", (uint8_t*)199, 11521},
//    {eAlarmVideoAssetId, "A9_06", (uint8_t*)200, 11553},
//    {eAlarmVideoAssetId, "A9_07", (uint8_t*)201, 11554},
//    {eAlarmVideoAssetId, "A9_08", (uint8_t*)202, 11576},
//    {eAlarmVideoAssetId, "A9_09", (uint8_t*)203, 11580},
//    {eAlarmVideoAssetId, "A9_10", (uint8_t*)204, 11621},
//    {eAlarmVideoAssetId, "A9_11", (uint8_t*)205, 11695},
//    {eAlarmVideoAssetId, "A9_12", (uint8_t*)206, 11892},
//    {eAlarmVideoAssetId, "A9_13", (uint8_t*)207, 11948},
//    {eAlarmVideoAssetId, "A9_14", (uint8_t*)208, 12120},
//    {eAlarmVideoAssetId, "A9_15", (uint8_t*)209, 12027},
//    {eAlarmVideoAssetId, "A9_16", (uint8_t*)210, 12024},
//    {eAlarmVideoAssetId, "A9_17", (uint8_t*)211, 11874},
//    {eAlarmVideoAssetId, "A9_18", (uint8_t*)212, 11710},
//    {eAlarmVideoAssetId, "A9_19", (uint8_t*)213, 11431},
//    {eAlarmVideoAssetId, "A9_20", (uint8_t*)214, 11192},
//    {eAlarmVideoAssetId, "A9_21", (uint8_t*)215, 10916},
//    {eAlarmVideoAssetId, "A9_22", (uint8_t*)216, 10542},
//    {eAlarmVideoAssetId, "A9_23", (uint8_t*)217, 10128},
//    {eAlarmVideoAssetId, "A9_24", (uint8_t*)218, 9724},
//    {eAlarmVideoAssetId, "A9_25", (uint8_t*)219, 9629},
//    {eAlarmVideoAssetId, "A9_26", (uint8_t*)220, 9510},
//    {eAlarmVideoAssetId, "A9_27", (uint8_t*)221, 9279},
//    {eAlarmVideoAssetId, "A9_28", (uint8_t*)222, 9054},
//    {eAlarmVideoAssetId, "A9_29", (uint8_t*)223, 8984},
//    {eAlarmVideoAssetId, "A9_30", (uint8_t*)224, 8882},
//    {eAlarmVideoAssetId, "A9_31", (uint8_t*)225, 8836},
//    {eAlarmVideoAssetId, "A9_32", (uint8_t*)226, 8857},
//    {eAlarmVideoAssetId, "A9_33", (uint8_t*)227, 8888},
//    {eAlarmVideoAssetId, "A9_34", (uint8_t*)228, 8889},
//    {eAlarmVideoAssetId, "A9_35", (uint8_t*)229, 8887},
//    {eAlarmVideoAssetId, "A9_36", (uint8_t*)230, 8888},
//    {eAlarmVideoAssetId, "A9_37", (uint8_t*)231, 8900},
//    {eAlarmVideoAssetId, "A9_38", (uint8_t*)232, 8874},
//    {eAlarmVideoAssetId, "A9_39", (uint8_t*)233, 8863},
//    {eAlarmVideoAssetId, "A9_40", (uint8_t*)234, 8866},
//    {eAlarmVideoAssetId, "A9_41", (uint8_t*)235, 8865},
//    {eAlarmVideoAssetId, "A9_42", (uint8_t*)236, 8874},
//    {eAlarmVideoAssetId, "A9_43", (uint8_t*)237, 8861},
//    {eAlarmVideoAssetId, "A9_44", (uint8_t*)238, 8869},
    //
//    {eAlarmVideoAssetId, "A20_01", (uint8_t*)239, 7994},
//    {eAlarmVideoAssetId, "A20_02", (uint8_t*)240, 8145},
//    {eAlarmVideoAssetId, "A20_03", (uint8_t*)241, 8191},
//    {eAlarmVideoAssetId, "A20_04", (uint8_t*)242, 8400},
//    {eAlarmVideoAssetId, "A20_05", (uint8_t*)243, 8574},
//    {eAlarmVideoAssetId, "A20_06", (uint8_t*)244, 8623},
//    {eAlarmVideoAssetId, "A20_07", (uint8_t*)245, 8524},
//    {eAlarmVideoAssetId, "A20_08", (uint8_t*)246, 8683},
//    {eAlarmVideoAssetId, "A20_09", (uint8_t*)247, 8525},
//    {eAlarmVideoAssetId, "A20_10", (uint8_t*)248, 8552},
//    {eAlarmVideoAssetId, "A20_11", (uint8_t*)249, 8470},
//    {eAlarmVideoAssetId, "A20_12", (uint8_t*)250, 8557},
//    {eAlarmVideoAssetId, "A20_13", (uint8_t*)251, 8438},
//    {eAlarmVideoAssetId, "A20_14", (uint8_t*)252, 8534},
//    {eAlarmVideoAssetId, "A20_15", (uint8_t*)253, 8567},
//    {eAlarmVideoAssetId, "A20_16", (uint8_t*)254, 8435},
//    {eAlarmVideoAssetId, "A20_17", (uint8_t*)255, 8519},
//    {eAlarmVideoAssetId, "A20_18", (uint8_t*)256, 8546},
//    {eAlarmVideoAssetId, "A20_19", (uint8_t*)257, 8533},
//    {eAlarmVideoAssetId, "A20_20", (uint8_t*)258, 8507},
//    {eAlarmVideoAssetId, "A20_21", (uint8_t*)259, 8503},
//    {eAlarmVideoAssetId, "A20_22", (uint8_t*)260, 8519},
//    {eAlarmVideoAssetId, "A20_23", (uint8_t*)261, 8520},
//    {eAlarmVideoAssetId, "A20_24", (uint8_t*)262, 8513},
//    {eAlarmVideoAssetId, "A20_25", (uint8_t*)263, 8510},
//    {eAlarmVideoAssetId, "A20_26", (uint8_t*)264, 8521},
//    {eAlarmVideoAssetId, "A20_27", (uint8_t*)265, 8548},
//    {eAlarmVideoAssetId, "A20_28", (uint8_t*)266, 8548},
//    {eAlarmVideoAssetId, "A21_01", (uint8_t*)267, 7994},
//    {eAlarmVideoAssetId, "A21_02", (uint8_t*)268, 8145},
//    {eAlarmVideoAssetId, "A21_03", (uint8_t*)269, 8191},
//    {eAlarmVideoAssetId, "A21_04", (uint8_t*)270, 8400},
//    {eAlarmVideoAssetId, "A21_05", (uint8_t*)271, 8574},
//    {eAlarmVideoAssetId, "A21_06", (uint8_t*)272, 8623},
//    {eAlarmVideoAssetId, "A21_07", (uint8_t*)273, 8524},
//    {eAlarmVideoAssetId, "A21_08", (uint8_t*)274, 8683},
//    {eAlarmVideoAssetId, "A21_09", (uint8_t*)275, 8525},
//    {eAlarmVideoAssetId, "A21_10", (uint8_t*)276, 8552},
//    {eAlarmVideoAssetId, "A21_11", (uint8_t*)277, 8470},
//    {eAlarmVideoAssetId, "A21_12", (uint8_t*)278, 8557},
//    {eAlarmVideoAssetId, "A21_13", (uint8_t*)279, 8438},
//    {eAlarmVideoAssetId, "A21_14", (uint8_t*)280, 8534},
//    {eAlarmVideoAssetId, "A21_15", (uint8_t*)281, 8567},
//    {eAlarmVideoAssetId, "A21_16", (uint8_t*)282, 8435},
//    {eAlarmVideoAssetId, "A21_17", (uint8_t*)283, 8519},
//    {eAlarmVideoAssetId, "A21_18", (uint8_t*)284, 8546},
//    {eAlarmVideoAssetId, "A21_19", (uint8_t*)285, 8533},
//    {eAlarmVideoAssetId, "A21_20", (uint8_t*)286, 8507},
//    {eAlarmVideoAssetId, "A21_21", (uint8_t*)287, 8503},
//    {eAlarmVideoAssetId, "A21_22", (uint8_t*)288, 8519},
//    {eAlarmVideoAssetId, "A21_23", (uint8_t*)289, 8520},
//    {eAlarmVideoAssetId, "A21_24", (uint8_t*)290, 8513},
//    {eAlarmVideoAssetId, "A21_25", (uint8_t*)291, 8510},
//    {eAlarmVideoAssetId, "A21_26", (uint8_t*)292, 8521},
//    {eAlarmVideoAssetId, "A21_27", (uint8_t*)293, 8548},
//    {eAlarmVideoAssetId, "A21_28", (uint8_t*)294, 8548},
//    {eAlarmVideoAssetId, "A22_01", (uint8_t*)295, 15369},
//    {eAlarmVideoAssetId, "A22_02", (uint8_t*)296, 15381},
//    {eAlarmVideoAssetId, "A22_03", (uint8_t*)297, 15480},
//    {eAlarmVideoAssetId, "A22_04", (uint8_t*)298, 15620},
//    {eAlarmVideoAssetId, "A22_05", (uint8_t*)299, 15701},
//    {eAlarmVideoAssetId, "A22_06", (uint8_t*)300, 15500},
//    {eAlarmVideoAssetId, "A22_07", (uint8_t*)301, 15363},
//    {eAlarmVideoAssetId, "A22_08", (uint8_t*)302, 15055},
//    {eAlarmVideoAssetId, "A22_09", (uint8_t*)303, 14977},
//    {eAlarmVideoAssetId, "A22_10", (uint8_t*)304, 14641},
//    {eAlarmVideoAssetId, "A22_11", (uint8_t*)305, 14456},
//    {eAlarmVideoAssetId, "A22_12", (uint8_t*)306, 13999},
//    {eAlarmVideoAssetId, "A22_13", (uint8_t*)307, 13927},
//    {eAlarmVideoAssetId, "A22_14", (uint8_t*)308, 13409},
//    {eAlarmVideoAssetId, "A22_15", (uint8_t*)309, 13171},
//    {eAlarmVideoAssetId, "A22_16", (uint8_t*)310, 12825},
//    {eAlarmVideoAssetId, "A22_17", (uint8_t*)311, 12763},
//    {eAlarmVideoAssetId, "A22_18", (uint8_t*)312, 12678},
//    {eAlarmVideoAssetId, "A22_19", (uint8_t*)313, 12664},
//    {eAlarmVideoAssetId, "A22_20", (uint8_t*)314, 12680},
//    {eAlarmVideoAssetId, "A22_21", (uint8_t*)315, 12721},
//    {eAlarmVideoAssetId, "A22_22", (uint8_t*)316, 12688},
//    {eAlarmVideoAssetId, "A22_23", (uint8_t*)317, 12727},
//    {eAlarmVideoAssetId, "A22_24", (uint8_t*)318, 8311},
//    {eAlarmVideoAssetId, "A22_25", (uint8_t*)319, 8380},
//    {eAlarmVideoAssetId, "A22_26", (uint8_t*)320, 8529},
//    {eAlarmVideoAssetId, "A22_27", (uint8_t*)321, 8622},
//    {eAlarmVideoAssetId, "A22_28", (uint8_t*)322, 8615},
//    {eAlarmVideoAssetId, "A22_29", (uint8_t*)323, 8722},
//    {eAlarmVideoAssetId, "A22_30", (uint8_t*)324, 8631},
//    {eAlarmVideoAssetId, "A22_31", (uint8_t*)325, 8640},
//    {eAlarmVideoAssetId, "A22_32", (uint8_t*)326, 8605},
//    {eAlarmVideoAssetId, "A22_33", (uint8_t*)327, 8638},
//    {eAlarmVideoAssetId, "A22_34", (uint8_t*)328, 8442},
//    {eAlarmVideoAssetId, "A22_35", (uint8_t*)329, 8497},
//    {eAlarmVideoAssetId, "A22_36", (uint8_t*)330, 8360},
//    {eAlarmVideoAssetId, "A22_37", (uint8_t*)331, 8373},
//    {eAlarmVideoAssetId, "A22_38", (uint8_t*)332, 8376},
//    {eAlarmVideoAssetId, "A22_39", (uint8_t*)333, 8411},
//    {eAlarmVideoAssetId, "A22_40", (uint8_t*)334, 8384},
//    {eAlarmVideoAssetId, "A22_41", (uint8_t*)335, 8395},
//    {eAlarmVideoAssetId, "A22_42", (uint8_t*)336, 8402},
//    {eAlarmVideoAssetId, "A22_43", (uint8_t*)337, 8387},
//    {eAlarmVideoAssetId, "A22_44", (uint8_t*)338, 8390},
//    {eAlarmVideoAssetId, "A22_45", (uint8_t*)339, 8424},
//    {eAlarmVideoAssetId, "A22_46", (uint8_t*)340, 8432},
//    {eAlarmVideoAssetId, "A22_47", (uint8_t*)341, 8416},
//    {eAlarmVideoAssetId, "A22_48", (uint8_t*)342, 8396},
//    {eAlarmVideoAssetId, "A22_49", (uint8_t*)343, 8434},
//    {eAlarmVideoAssetId, "A22_50", (uint8_t*)344, 8396},
//    {eAlarmVideoAssetId, "A27_01", (uint8_t*)345, 14867},
//    {eAlarmVideoAssetId, "A27_02", (uint8_t*)346, 14963},
//    {eAlarmVideoAssetId, "A27_03", (uint8_t*)347, 14924},
//    {eAlarmVideoAssetId, "A27_04", (uint8_t*)348, 14907},
//    {eAlarmVideoAssetId, "A27_05", (uint8_t*)349, 15073},
//    {eAlarmVideoAssetId, "A27_06", (uint8_t*)350, 14905},
//    {eAlarmVideoAssetId, "A27_07", (uint8_t*)351, 14584},
//    {eAlarmVideoAssetId, "A27_08", (uint8_t*)352, 14127},
//    {eAlarmVideoAssetId, "A27_09", (uint8_t*)353, 14106},
//    {eAlarmVideoAssetId, "A27_10", (uint8_t*)354, 13419},
//    {eAlarmVideoAssetId, "A27_11", (uint8_t*)355, 13237},
//    {eAlarmVideoAssetId, "A27_12", (uint8_t*)356, 13078},
//    {eAlarmVideoAssetId, "A27_13", (uint8_t*)357, 13054},
//    {eAlarmVideoAssetId, "A27_14", (uint8_t*)358, 13067},
//    {eAlarmVideoAssetId, "A27_15", (uint8_t*)359, 13079},
//    {eAlarmVideoAssetId, "A27_16", (uint8_t*)360, 13087},
//    {eAlarmVideoAssetId, "A27_17", (uint8_t*)361, 13130},
//    {eAlarmVideoAssetId, "A27_18", (uint8_t*)362, 13009},
//    {eAlarmVideoAssetId, "A27_19", (uint8_t*)363, 13101},
//    {eAlarmVideoAssetId, "A27_20", (uint8_t*)364, 13242},
//    {eAlarmVideoAssetId, "A27_21", (uint8_t*)365, 13514},
//    {eAlarmVideoAssetId, "A27_22", (uint8_t*)366, 13657},
//    {eAlarmVideoAssetId, "A27_23", (uint8_t*)367, 14135},
//    {eAlarmVideoAssetId, "A27_24", (uint8_t*)368, 14391},
//    {eAlarmVideoAssetId, "A27_25", (uint8_t*)369, 14326},
//    {eAlarmVideoAssetId, "A27_26", (uint8_t*)370, 14928},
//    {eAlarmVideoAssetId, "A27_27", (uint8_t*)371, 15243},
//    {eAlarmVideoAssetId, "A27_28", (uint8_t*)372, 15577},
//    {eAlarmVideoAssetId, "A27_29", (uint8_t*)373, 15787},
//    {eAlarmVideoAssetId, "A27_30", (uint8_t*)374, 15806},
//    {eAlarmVideoAssetId, "A27_31", (uint8_t*)375, 15799},
//    {eAlarmVideoAssetId, "A27_32", (uint8_t*)376, 15807},
//    {eAlarmVideoAssetId, "A27_33", (uint8_t*)377, 15822},
//    {eAlarmVideoAssetId, "A27_34", (uint8_t*)378, 15825},
//    {eAlarmVideoAssetId, "A27_35", (uint8_t*)379, 15850},
//    {eAlarmVideoAssetId, "A27_36", (uint8_t*)380, 15853},
//    {eAlarmVideoAssetId, "A27_37", (uint8_t*)381, 15867},
//    {eAlarmVideoAssetId, "A27_38", (uint8_t*)382, 15848},
//    {eAlarmVideoAssetId, "A27_39", (uint8_t*)383, 15859},
//    {eAlarmVideoAssetId, "A27_40", (uint8_t*)384, 15853},
//    {eAlarmVideoAssetId, "A27_41", (uint8_t*)385, 15863},
//    {eAlarmVideoAssetId, "A28_01", (uint8_t*)386, 14867},
//    {eAlarmVideoAssetId, "A28_02", (uint8_t*)387, 14963},
//    {eAlarmVideoAssetId, "A28_03", (uint8_t*)388, 14924},
//    {eAlarmVideoAssetId, "A28_04", (uint8_t*)389, 14907},
//    {eAlarmVideoAssetId, "A28_05", (uint8_t*)390, 15073},
//    {eAlarmVideoAssetId, "A28_06", (uint8_t*)391, 14905},
//    {eAlarmVideoAssetId, "A28_07", (uint8_t*)392, 14584},
//    {eAlarmVideoAssetId, "A28_08", (uint8_t*)393, 14127},
//    {eAlarmVideoAssetId, "A28_09", (uint8_t*)394, 14106},
//    {eAlarmVideoAssetId, "A28_10", (uint8_t*)395, 13419},
//    {eAlarmVideoAssetId, "A28_11", (uint8_t*)396, 13237},
//    {eAlarmVideoAssetId, "A28_12", (uint8_t*)397, 13078},
//    {eAlarmVideoAssetId, "A28_13", (uint8_t*)398, 13054},
//    {eAlarmVideoAssetId, "A28_14", (uint8_t*)399, 13067},
//    {eAlarmVideoAssetId, "A28_15", (uint8_t*)400, 13079},
//    {eAlarmVideoAssetId, "A28_16", (uint8_t*)401, 13087},
//    {eAlarmVideoAssetId, "A28_17", (uint8_t*)402, 13130},
//    {eAlarmVideoAssetId, "A28_18", (uint8_t*)403, 13009},
//    {eAlarmVideoAssetId, "A28_19", (uint8_t*)404, 13101},
//    {eAlarmVideoAssetId, "A28_20", (uint8_t*)405, 13242},
//    {eAlarmVideoAssetId, "A28_21", (uint8_t*)406, 13514},
//    {eAlarmVideoAssetId, "A28_22", (uint8_t*)407, 13657},
//    {eAlarmVideoAssetId, "A28_23", (uint8_t*)408, 14135},
//    {eAlarmVideoAssetId, "A28_24", (uint8_t*)409, 14391},
//    {eAlarmVideoAssetId, "A28_25", (uint8_t*)410, 14326},
//    {eAlarmVideoAssetId, "A28_26", (uint8_t*)411, 14928},
//    {eAlarmVideoAssetId, "A28_27", (uint8_t*)412, 15243},
//    {eAlarmVideoAssetId, "A28_28", (uint8_t*)413, 15577},
//    {eAlarmVideoAssetId, "A28_29", (uint8_t*)414, 15787},
//    {eAlarmVideoAssetId, "A28_30", (uint8_t*)415, 15806},
//    {eAlarmVideoAssetId, "A28_31", (uint8_t*)416, 15799},
//    {eAlarmVideoAssetId, "A28_32", (uint8_t*)417, 15807},
//    {eAlarmVideoAssetId, "A28_33", (uint8_t*)418, 15822},
//    {eAlarmVideoAssetId, "A28_34", (uint8_t*)419, 15825},
//    {eAlarmVideoAssetId, "A28_35", (uint8_t*)420, 15850},
//    {eAlarmVideoAssetId, "A28_36", (uint8_t*)421, 15853},
//    {eAlarmVideoAssetId, "A28_37", (uint8_t*)422, 15867},
//    {eAlarmVideoAssetId, "A28_38", (uint8_t*)423, 15848},
//    {eAlarmVideoAssetId, "A28_39", (uint8_t*)424, 15859},
//    {eAlarmVideoAssetId, "A28_40", (uint8_t*)425, 15853},
//    {eAlarmVideoAssetId, "A28_41", (uint8_t*)426, 15863},
//    {eAlarmVideoAssetId, "A29_01", (uint8_t*)427, 12381},
//    {eAlarmVideoAssetId, "A29_02", (uint8_t*)428, 12395},
//    {eAlarmVideoAssetId, "A29_03", (uint8_t*)429, 12600},
//    {eAlarmVideoAssetId, "A29_04", (uint8_t*)430, 12636},
//    {eAlarmVideoAssetId, "A29_05", (uint8_t*)431, 12637},
//    {eAlarmVideoAssetId, "A29_06", (uint8_t*)432, 12876},
//    {eAlarmVideoAssetId, "A29_07", (uint8_t*)433, 12852},
//    {eAlarmVideoAssetId, "A29_08", (uint8_t*)434, 13096},
//    {eAlarmVideoAssetId, "A29_09", (uint8_t*)435, 13368},
//    {eAlarmVideoAssetId, "A29_10", (uint8_t*)436, 13529},
//    {eAlarmVideoAssetId, "A29_11", (uint8_t*)437, 13641},
//    {eAlarmVideoAssetId, "A29_12", (uint8_t*)438, 13911},
//    {eAlarmVideoAssetId, "A29_13", (uint8_t*)439, 14063},
//    {eAlarmVideoAssetId, "A29_14", (uint8_t*)440, 14111},
//    {eAlarmVideoAssetId, "A29_15", (uint8_t*)441, 14375},
//    {eAlarmVideoAssetId, "A29_16", (uint8_t*)442, 14477},
//    {eAlarmVideoAssetId, "A29_17", (uint8_t*)443, 14650},
//    {eAlarmVideoAssetId, "A29_18", (uint8_t*)444, 14678},
//    {eAlarmVideoAssetId, "A29_19", (uint8_t*)445, 14685},
//    {eAlarmVideoAssetId, "A29_20", (uint8_t*)446, 14726},
//    {eAlarmVideoAssetId, "A29_21", (uint8_t*)447, 14652},
//    {eAlarmVideoAssetId, "A29_22", (uint8_t*)448, 14630},
//    {eAlarmVideoAssetId, "A29_23", (uint8_t*)449, 14632},
//    {eAlarmVideoAssetId, "A29_24", (uint8_t*)450, 14612},
//    {eAlarmVideoAssetId, "A29_25", (uint8_t*)451, 14614},
//    {eAlarmVideoAssetId, "A29_26", (uint8_t*)452, 14642},
//    {eAlarmVideoAssetId, "A29_27", (uint8_t*)453, 14616},
//    {eAlarmVideoAssetId, "A29_28", (uint8_t*)454, 14626},    
    
};

#endif	/* GUIDEFINE_H */

/* end of file */

