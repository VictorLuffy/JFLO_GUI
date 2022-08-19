/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _AUDIO_H    /* Guard against multiple inclusion */
#define _AUDIO_H


/* This section lists the other files that are included in this file.
 */

#include "stdint.h"




/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    
/** @brief define command for Play alarm sound */
typedef enum {
  ePlayHighAlarm,
  ePlayMediumAlarm,
  ePlayLow1Alarm,
  ePlayStartupSound,
  eStopPlay,
  eMute,  
  eTestSound,        
} E_AudioCommand;


    bool _i2s_TLV320Init (void);
    bool Audio_Initialize();
    void Audio_HandleEvent(E_AudioCommand cmd, uint16_t data);
    void Audio_HandleLowAlarm(void);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _AUDIO_H */

/* *****************************************************************************
 End of File
 */
