#include "xbee_api_opt.h"
#include "hal_types.h"
#include "npi.h"

static uint8 xbee_wbuf[256],xbee_rbuf[256];

void xbee_api_atcmd_set_led(uint8 led,uint8 state)
{
  pst_api_frame_head head = (pst_api_frame_head)xbee_wbuf;
  pst_api_atCmd atCmd;
  head->start_delimiter = 0x7e;
  head->len_msb = 0x00;
  head->len_lsb = 0x05;
  atCmd = (pst_api_atCmd)&head->fram_data;
  atCmd->frame_type = 0x08;
  atCmd->frame_id = 0x52;
  switch(led)
  {
  case XBEE_LED1:
    atCmd->atCmd[0] = 'P';
    atCmd->atCmd[1] = '1';
    break;
  case XBEE_LED2:
    atCmd->atCmd[0] = 'D';
    atCmd->atCmd[1] = '4';
    break;
  case XBEE_LED3:
    atCmd->atCmd[0] = 'D';
    atCmd->atCmd[1] = '5';
    break;
  case XBEE_LED4:
    atCmd->atCmd[0] = 'D';
    atCmd->atCmd[1] = '3';
    break;
  }
  atCmd->atPara = (state == LED_ON)?4:5;
  xbee_api_checksum((uint8*)atCmd,5);
  NPI_WriteTransport(xbee_wbuf, 9);
}

void xbee_api_atcmd_speaker_opt(uint8 state)
{
  pst_api_frame_head head = (pst_api_frame_head)xbee_wbuf;
  pst_api_atCmd atCmd;
  head->start_delimiter = 0x7e;
  head->len_msb = 0x00;
  head->len_lsb = 0x05;
  atCmd = (pst_api_atCmd)&head->fram_data;
  atCmd->frame_type = 0x08;
  atCmd->frame_id = 0x52;
  atCmd->atCmd[0] = 'P';
  atCmd->atCmd[1] = '2';
  atCmd->atPara = (state == SPEAKER_ON)?5:4;
  xbee_api_checksum((uint8*)atCmd,5);
  NPI_WriteTransport(xbee_wbuf, 9);
}

void xbee_api_atcmd_getadc(uint8 adc)
{
  /*pst_api_frame_head head = (pst_api_frame_head)xbee_wbuf;
  pst_api_atCmd atCmd;
  head->start_delimiter = 0x7e;
  head->len_msb = 0x00;
  head->len_lsb = 0x05;
  atCmd = (pst_api_atCmd)&head->fram_data;
  atCmd->frame_type = 0x08;
  atCmd->frame_id = 0x00;
  switch(led)
  {
  case XBEE_LED1:
    atCmd->atCmd[0] = 'P';
    atCmd->atCmd[1] = '1';
    break;
  case XBEE_LED2:
    atCmd->atCmd[0] = 'D';
    atCmd->atCmd[1] = '4';
    break;
  case XBEE_LED3:
    atCmd->atCmd[0] = 'D';
    atCmd->atCmd[1] = '5';
    break;
  case XBEE_LED4:
    atCmd->atCmd[0] = 'D';
    atCmd->atCmd[1] = '3';
    break;
  }
  atCmd->atPara = (state == LED_ON)?5:4;
  xbee_api_checksum((uint8*)atCmd,5);
  NPI_WriteTransport(xbee_wbuf, 9);*/
}

void xbee_api_checksum(uint8 *begin,uint16 length)
{
  uint8 sum = 0;
  for(int loop = 0;loop < length;loop ++)
  {
    sum += begin[loop];
  }
  begin[length] = 0xff - sum;
}

void xbee_api_transmit_data_request(uint8 *dest_addr,uint8 *data,uint16 len)
{
  xbee_wbuf[0] = 0x7e;
  xbee_wbuf[1] = (uint8)((len + 1) >> 8);
  xbee_wbuf[2] = (uint8)(len + 1);
  xbee_wbuf[3] = 0x10;
  osal_memcpy(&xbee_wbuf[4],data,len);
  xbee_api_checksum(&xbee_wbuf[3],len + 1);
  NPI_WriteTransport(xbee_wbuf, len + 1 + 4);
}