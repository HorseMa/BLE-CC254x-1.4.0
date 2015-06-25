#ifndef __XBEE_API_OPT_H__
#define __XBEE_API_OPT_H__

#include "hal_types.h"

#define XBEE_LED1       0
#define XBEE_LED2       1
#define XBEE_LED3       2
#define XBEE_LED4       3

#ifndef LED_ON
#define LED_ON          1
#endif
#ifndef LED_OFF
#define LED_OFF         !LED_ON
#endif

#ifndef SPEAKER_ON
#define SPEAKER_ON      1
#endif
#ifndef SPEAKER_OFF
#define SPEAKER_OFF     !SPEAKER_ON
#endif

/*typedef enum
{
  enDisable,
  enSpecial,
  enAnalog,
  enDigitalInput,
  enDigitalOutPutLow,
  enDigitalOutPutHigh,
}*/

typedef struct
{
  uint8 start_delimiter;
  uint8 len_msb;
  uint8 len_lsb;
  uint8 *fram_data;
  //uint8 checksum;
}st_api_frame_head,*pst_api_frame_head;

typedef struct
{
  uint8 frame_type;
  uint8 frame_id;
  uint8 atCmd[2];
  uint8 atPara;
}st_api_atCmd,*pst_api_atCmd;

typedef struct
{
  uint8 frame_type;
  uint8 frame_id;
  uint8 dest_long_addr[8];
  uint8 dest_short_addr[2];
  uint8 radius;
  uint8 options;
  uint8 *data;
}st_api_trasmit_data,*pst_api_trasmit_data;

void xbee_api_checksum(uint8 *begin,uint16 length);
void xbee_api_atcmd_set_led(uint8,uint8);
void xbee_api_atcmd_speaker_opt(uint8 state);
void xbee_api_transmit_data_request(uint8 *dest_addr,uint8 *data,uint16 len);
#endif