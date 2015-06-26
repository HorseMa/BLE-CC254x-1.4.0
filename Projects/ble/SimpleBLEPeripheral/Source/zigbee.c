/*********************************************************************
 * INCLUDES
 */

#include "bcomdef.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"

#include "OnBoard.h"
#include "hal_adc.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_lcd.h"
#include "zlgAtCmd.h"
#include "gatt.h"

#include "hci.h"

#include "gapgattserver.h"
#include "gattservapp.h"
#include "devinfoservice.h"
#include "simpleGATTprofile.h"

#if defined( CC2540_MINIDK )
  #include "simplekeys.h"
#endif

#include "peripheral.h"

#include "gapbondmgr.h"

#include "zigbee.h"
#include "xbee_at_cmd.h"
#include "xbee_api_opt.h"

#if defined FEATURE_OAD
  #include "oad.h"
  #include "oad_target.h"
#endif
#include "npi.h"
#include "hal_gpio.h"
#include "hal_i2c.h"
#include "hal_sensor.h"
#include <math.h>
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */



/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
uint8 zigbee_TaskID;   // Task ID for internal task/event processing


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void zigbee_ProcessOSALMsg( osal_event_hdr_t *pMsg );

#if defined( CC2540_MINIDK )
static void zigbee_HandleKeys( uint8 shift, uint8 keys );
#endif


/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Zigbee_Init
 *
 * @brief   Initialization function for the Simple BLE Peripheral App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void Zigbee_Init( uint8 task_id )
{
  zigbee_TaskID = task_id;

#if defined( CC2540_MINIDK )

  // Register for all key events - This app will handle all key events
  RegisterForKeys( zigbee_TaskID );

#endif // #if defined( CC2540_MINIDK )

  // Setup a delayed profile startup
  osal_set_event( zigbee_TaskID, ZIGBEE_START_DEVICE_EVT );

}

/*********************************************************************
 * @fn      Zigbee_ProcessEvent
 *
 * @brief   Simple BLE Peripheral Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16 Zigbee_ProcessEvent( uint8 task_id, uint16 events )
{

  VOID task_id; // OSAL required parameter that isn't used in this function
  static unsigned char led = 0;
  if ( events & SYS_EVENT_MSG )
  {
    uint8 *pMsg;

    if ( (pMsg = osal_msg_receive( zigbee_TaskID )) != NULL )
    {
      zigbee_ProcessOSALMsg( (osal_event_hdr_t *)pMsg );

      // Release the OSAL message
      VOID osal_msg_deallocate( pMsg );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
#if defined _USE_ZM516X__
  if(events & ZIGBEE_READ_ZM516X_INFO_EVT)
  {
    static unsigned char state = 0;
    unsigned char len;
    unsigned int i;
    unsigned char sum;
    static unsigned char wbuf[255],rbuf[255];
    static unsigned char cnt = 0;
    HalGpioSet(HAL_GPIO_ZM516X_ALL,1);
    

    switch(state)
    {
      case 0:
        HalGpioSet(HAL_GPIO_ZM516X_ALL,1);
        HalGpioSet(HAL_GPIO_ZM516X_DEF,0);
        HalGpioSet(HAL_GPIO_ZM516X_RESET,0);
        //HalGpioSet(HAL_GPIO_ZM516X_SLEEP,1);
        osal_start_timerEx( task_id, ZIGBEE_READ_ZM516X_INFO_EVT, 100 );
        state = 1;
        break;
      case 1:
        HalGpioSet(HAL_GPIO_ZM516X_RESET,1);
        osal_start_timerEx( task_id, ZIGBEE_READ_ZM516X_INFO_EVT, 100 );
        state = 2;
        break;
      case 2:
readloacalcfg:
        HalGpioSet(HAL_GPIO_ZM516X_DEF,1);
        //HalGpioSet(HAL_GPIO_ZM516X_SLEEP,0);
        NPI_ReadTransport(rbuf,NPI_RxBufLen());  // clear uart buffer
        wbuf[0] = 0xab;
        wbuf[1] = 0xbc;
        wbuf[2] = 0xcd;
        wbuf[3] = enReadLoacalCfg;
        wbuf[4] = wbuf[0] + wbuf[1] + wbuf[2] + wbuf[3];
        NPI_WriteTransport(wbuf, 5);
        state = 3;
        break;
      case 3:
        len = NPI_RxBufLen();
        cnt ++;
        if(len >= 74)
        {
          osal_memset(rbuf,0,128);
          NPI_ReadTransport(rbuf,74);
          if((rbuf[0] == 0xab)
          &&(rbuf[1] == 0xbc)
          &&(rbuf[2] == 0xcd)
          &&(rbuf[3] == enReadLoacalCfg))
          {
            osal_memcpy(&stDevInfo,&rbuf[4],sizeof(struct dev_info));
            rbuf[0] = 0xab;
            rbuf[1] = 0xbc;
            rbuf[2] = 0xcd;
            rbuf[3] = enModifyCfg;
            rbuf[4] = stDevInfo.devLoacalNetAddr[0];
            rbuf[5] = stDevInfo.devLoacalNetAddr[1];
            stDevInfo.devLoacalNetAddr[0] = 0x00;
            stDevInfo.devLoacalNetAddr[1] = 0x01;
            stDevInfo.devChannel = 0x19;
            stDevInfo.devPanid[0] = 0x10;
            stDevInfo.devPanid[1] = 0x01;
            osal_memcpy(&rbuf[6],&stDevInfo,65);
            sum = 0;
            for(i = 0;i < (6 + 65);i++)
            {
              sum += rbuf[i];
            }
            rbuf[6 + 65] = sum;
            NPI_WriteTransport(rbuf, 6 + 65 + 1);
            state = 4;
          }
          else
          {
            goto readloacalcfg;
          }
        }
        break;
      case 4:
        len = NPI_RxBufLen();
        
        if(len == 7)
        {
          NPI_ReadTransport(rbuf,7);
          //osal_start_timerEx( task_id, SBP_READ_ZM516X_INFO_EVT, 100 );
          //break;
          HalGpioSet(HAL_GPIO_ZM516X_RESET,0);
          osal_start_timerEx( task_id, ZIGBEE_READ_ZM516X_INFO_EVT, 100 );
          state = 5;
        }
        break;
      case 5:
        HalGpioSet(HAL_GPIO_ZM516X_RESET,1);
        osal_set_event( zigbee_TaskID, BOARD_TEST_EVT );
        state = 6;
        break;
      case 6:
        len = NPI_RxBufLen();
        if(len >= 13)
        {
          NPI_ReadTransport(rbuf,13);
          led = !led;
          if(led)
          {
            HalGpioSet(HAL_GPIO_ZM516X_MOTOR1,1);
            HalGpioSet(HAL_GPIO_ZM516X_MOTOR2,0);
          }
          else
          {
            HalGpioSet(HAL_GPIO_ZM516X_MOTOR1,0);
            HalGpioSet(HAL_GPIO_ZM516X_MOTOR2,1);
          }
        }
    }

    return ( events ^ ZIGBEE_READ_ZM516X_INFO_EVT );
  }
#endif 
#if defined _USE_XBEE__
  if(events & XBEE_INIT_EVT)
  {
    static uint8 state = 0;
    uint8 len;
    static uint8 rbuf[128];
    switch(state)
    {
      case 0:
      HalGpioSet(HAL_GPIO_XBEE_RESET,0);
      HalGpioSet(HAL_GPIO_XBEE_RTS,1);
      osal_start_timerEx( task_id, XBEE_INIT_EVT, 100 );
      state = 1;
      break;
      case 1:
      HalGpioSet(HAL_GPIO_XBEE_RESET,1);
      //osal_start_timerEx( task_id, XBEE_INIT_EVT, 2000 );
      state = 2;
      break;
      case 2:
      len = NPI_RxBufLen();
      NPI_ReadTransport(rbuf,len);
      osal_memcpy(rbuf,&stDevInfo,65);
      xbee_api_atcmd_set_led(XBEE_LED1,LED_OFF);
      xbee_api_atcmd_set_led(XBEE_LED2,LED_ON);
      xbee_api_atcmd_set_led(XBEE_LED3,LED_ON);
      xbee_api_atcmd_set_led(XBEE_LED4,LED_ON);
      xbee_api_atcmd_speaker_opt(SPEAKER_ON);
      HalGpioSet(HAL_GPIO_ZM516X_MOTOR_EN,0);
      
      //xbee_api_transmit_data_request("hello",6);
      state = 3;
      break;
      case 3:
      len = NPI_RxBufLen();
      NPI_ReadTransport(rbuf,len);
      state = 4;
      break;
      case 4:
      osal_set_event( zigbee_TaskID, BOARD_TEST_EVT );
      state = 5;
      break;
      case 5:
      break;
      default:
      break;
    }
    

    return ( events ^ XBEE_INIT_EVT );
  }
#endif
  if(events & BOARD_TEST_EVT)
  {
#if defined _USE_ZM516X__
    const unsigned char setiodir[] = {0xDE, 0xDF, 0xEF, 0xD4, 0x20, 0x01, 0x7C};
    const unsigned char setio[] = {0xDE, 0xDF, 0xEF, 0xD6, 0x20, 0x01, 0x7C};
    const unsigned char readadc[] = {0xDE, 0xDF, 0xEF, 0xD7, 0x20, 0x01, 0x01};
    static unsigned char wbuf[255];
#endif
    static unsigned char state = 0;

    switch(state)
    {
      case 0:
        
        HalSensorReadReg(0x0a,wbuf,3);
        wbuf[0] = 0x00;
        //wbuf[1] = 0x00;
        HalSensorWriteReg(0x02, wbuf, 1);
        HalSensorWriteReg(0x01, wbuf, 1);
        HalSensorReadReg(0x03,wbuf,6);
        int16 x,y,z,angle;
        x=wbuf[0] << 8 | wbuf[1];
        y=wbuf[2] << 8 | wbuf[3];
        z=wbuf[4] << 8 | wbuf[5];
        angle= atan2((double)y,(double)x) * (180 / 3.14159265) + 180; // angle in degrees
        angle*=10;
        angle*=10;
        /*wan=temp_data/10000+0x30 ;
        temp_data=temp_data%10000;   //取余运算
	qian=temp_data/1000+0x30 ;
        temp_data=temp_data%1000;    //取余运算
        bai=temp_data/100+0x30   ;
        temp_data=temp_data%100;     //取余运算
        shi=temp_data/10+0x30    ;
        temp_data=temp_data%10;      //取余运算
        ge=temp_data+0x30; 	*/
        
#if defined _USE_ZM516X__
      osal_memcpy(wbuf,setiodir,7);
      //osal_memcpy(&wbuf[4],stDevInfo.devLoacalNetAddr,2);
      NPI_WriteTransport(wbuf, 7);
#endif
      osal_start_timerEx( zigbee_TaskID, BOARD_TEST_EVT, 200 );
#if defined _USE_XBEE__
      HalGpioSet(HAL_GPIO_XBEE_RTS,1);
      HalGpioSet(HAL_GPIO_XBEE_RESET,1);
#endif
      HalGpioSet(HAL_GPIO_ZM516X_MOTOR_EN,0);
      HalGpioSet(HAL_GPIO_ZM516X_DIR,1);
      state = 1;
      break;
      case 1:
#if defined _USE_ZM516X__
      osal_memcpy(wbuf,setio,7);
      //osal_memcpy(&wbuf[4],stDevInfo.devLoacalNetAddr,2);
      NPI_WriteTransport(wbuf, 7);
#endif
      HalGpioSet(HAL_GPIO_ZM516X_MOTOR1,1);
      HalGpioSet(HAL_GPIO_ZM516X_MOTOR2,0);
#if defined _USE_XBEE__
      xbee_api_atcmd_set_led(XBEE_LED1,LED_ON);
      xbee_api_atcmd_set_led(XBEE_LED2,LED_OFF);
      xbee_api_atcmd_set_led(XBEE_LED3,LED_OFF);
      xbee_api_atcmd_set_led(XBEE_LED4,LED_OFF);
      xbee_api_atcmd_speaker_opt(SPEAKER_OFF);
#endif
      state = 2;
      osal_start_timerEx( zigbee_TaskID, BOARD_TEST_EVT, 1000 );
      break;
      case 2:
      HalGpioSet(HAL_GPIO_ZM516X_MOTOR1,1);
      HalGpioSet(HAL_GPIO_ZM516X_MOTOR2,1);
      state = 3;
      osal_start_timerEx( zigbee_TaskID, BOARD_TEST_EVT, 1000 );
      break;
      case 3:
      HalGpioSet(HAL_GPIO_ZM516X_MOTOR1,0);
      HalGpioSet(HAL_GPIO_ZM516X_MOTOR2,1);
      state = 4;
      osal_start_timerEx( zigbee_TaskID, BOARD_TEST_EVT, 1000 );
      break;
      case 4:
      HalGpioSet(HAL_GPIO_ZM516X_MOTOR1,1);
      HalGpioSet(HAL_GPIO_ZM516X_MOTOR2,1);
#if defined _USE_ZM516X__
      osal_memcpy(wbuf,setio,7);
      wbuf[6] = ~0x7C;
      //osal_memcpy(&wbuf[4],stDevInfo.devLoacalNetAddr,2);
      NPI_WriteTransport(wbuf, 7);
#endif
#if defined _USE_XBEE__
      xbee_api_atcmd_set_led(XBEE_LED1,LED_OFF);
      xbee_api_atcmd_set_led(XBEE_LED2,LED_ON);
      xbee_api_atcmd_set_led(XBEE_LED3,LED_ON);
      xbee_api_atcmd_set_led(XBEE_LED4,LED_ON);
      xbee_api_atcmd_speaker_opt(SPEAKER_ON);
#endif
      state = 1;
      osal_start_timerEx( zigbee_TaskID, BOARD_TEST_EVT, 1000 );
      break;
      default:
      break;
    }
    
    return ( events ^ BOARD_TEST_EVT );
  }

  if(events & UART_RECEIVE_EVT)
  {
    static unsigned char lenold = 0;
    unsigned char len = 0;
    //; NPI_ReadTransport(buf,len);
    if((len = NPI_RxBufLen()) > 0)
    {
      if(lenold == len)
      {
#if defined _USE_ZM516X__
        osal_set_event( task_id, ZIGBEE_READ_ZM516X_INFO_EVT );
#endif
#if defined _USE_XBEE__
        osal_set_event( task_id, XBEE_INIT_EVT );
#endif
        lenold = 0;
      }
      else
      {
        lenold = len;
        osal_stop_timerEx( task_id, UART_RECEIVE_EVT );
#if defined _USE_ZM516X__
        osal_start_timerEx( task_id, UART_RECEIVE_EVT, 3 );
#endif
#if defined _USE_XBEE__
        osal_start_timerEx( task_id, UART_RECEIVE_EVT, 7 );
#endif
      }
      return ( events ^ UART_RECEIVE_EVT );
    }
    else
    {
      //if(lenold > 0)
      //osal_set_event( task_id, SBP_READ_ZM516X_INFO_EVT );
      //lenold = len;
      //osal_start_timerEx( task_id, UART_RECEIVE_EVT, 10 );
      //osal_set_event( task_id, SBP_READ_ZM516X_INFO_EVT );
      return ( events ^ UART_RECEIVE_EVT );
    }
    
    
  }

  if ( events & ZIGBEE_START_DEVICE_EVT )
  {
#if defined _USE_ZM516X__
    // Get zm516x info
    osal_set_event( zigbee_TaskID, ZIGBEE_READ_ZM516X_INFO_EVT );
#endif
    
#if defined _USE_XBEE__
    osal_set_event( zigbee_TaskID, XBEE_INIT_EVT );
#endif
    return ( events ^ ZIGBEE_START_DEVICE_EVT );
  }

  // Discard unknown events
  return 0;
}

/*********************************************************************
 * @fn      zigbee_ProcessOSALMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void zigbee_ProcessOSALMsg( osal_event_hdr_t *pMsg )
{
  switch ( pMsg->event )
  {
  #if defined( CC2540_MINIDK )
    case KEY_CHANGE:
      zigbee_HandleKeys( ((keyChange_t *)pMsg)->state, ((keyChange_t *)pMsg)->keys );
      break;
  #endif // #if defined( CC2540_MINIDK )

  default:
    // do nothing
    break;
  }
}

#if defined( CC2540_MINIDK )
/*********************************************************************
 * @fn      zigbee_HandleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 HAL_KEY_SW_2
 *                 HAL_KEY_SW_1
 *
 * @return  none
 */
static void zigbee_HandleKeys( uint8 shift, uint8 keys )
{
  uint8 SK_Keys = 0;

  VOID shift;  // Intentionally unreferenced parameter

  if ( keys & HAL_KEY_SW_1 )
  {
    SK_Keys |= SK_KEY_LEFT;
    HalGpioSet(HAL_GPIO_ZM516X_MOTOR1,0);
    HalGpioSet(HAL_GPIO_ZM516X_MOTOR2,0);
  }

  if ( keys & HAL_KEY_SW_2 )
  {
    HalGpioSet(HAL_GPIO_ZM516X_MOTOR1,0);
    HalGpioSet(HAL_GPIO_ZM516X_MOTOR2,0);
    SK_Keys |= SK_KEY_RIGHT;
  }
}
#endif // #if defined( CC2540_MINIDK )
