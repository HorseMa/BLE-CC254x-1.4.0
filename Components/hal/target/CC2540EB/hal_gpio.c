/**************************************************************************************************
  Filename:       hal_led.c
  Revised:        $Date: 2012-10-26 14:09:08 -0700 (Fri, 26 Oct 2012) $
  Revision:       $Revision: 31932 $

  Description:    This file contains the interface to the HAL LED Service.


  Copyright 2006-2012 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/***************************************************************************************************
 *                                             INCLUDES
 ***************************************************************************************************/

#include "hal_mcu.h"
#include "hal_defs.h"
#include "hal_types.h"
#include "hal_drivers.h"
#include "hal_gpio.h"
#include "osal.h"
#include "hal_board.h"
#include "OnBoard.h"

/***************************************************************************************************
 *                                              TYPEDEFS
 ***************************************************************************************************/



/***************************************************************************************************
 *                                           GLOBAL VARIABLES
 ***************************************************************************************************/



/***************************************************************************************************
 *                                            LOCAL FUNCTION
 ***************************************************************************************************/



/***************************************************************************************************
 *                                            FUNCTIONS - API
 ***************************************************************************************************/

/***************************************************************************************************
 * @fn      HalGpioInit
 *
 * @brief   Initialize LED Service
 *
 * @param   init - pointer to void that contains the initialized value
 *
 * @return  None
 ***************************************************************************************************/
void HalGpioInit (void)
{
#if defined _USE_ZM516X__
  GPIO_ZM516X_RESET_TURN_HIGH();
  GPIO_ZM516X_DEF_TURN_HIGH();
  GPIO_ZM516X_WAKEUP_TURN_HIGH();
  GPIO_ZM516X_SLEEP_TURN_HIGH();
  GPIO_ZM516X_MOTOR1_TURN_LOW();
  GPIO_ZM516X_MOTOR2_TURN_LOW();
  GPIO_ZM516X_MOTOR_EN_TURN_LOW();
  GPIO_ZM516X_MOTOR_PHASE_TURN_LOW();
  GPIO_ZM516X_DIR_TURN_LOW();
  GPIO_ZM516X_RESET_DDR |= GPIO_ZM516X_RESET_BV;
  GPIO_ZM516X_DEF_DDR |= GPIO_ZM516X_DEF_BV;
  GPIO_ZM516X_WAKEUP_DDR |= GPIO_ZM516X_WAKEUP_BV;
  GPIO_ZM516X_SLEEP_DDR |= GPIO_ZM516X_SLEEP_BV;
  GPIO_ZM516X_MOTOR1_DDR |= GPIO_ZM516X_MOTOR1_BV;
  GPIO_ZM516X_MOTOR2_DDR |= GPIO_ZM516X_MOTOR2_BV;
  GPIO_ZM516X_MOTOR_EN_DDR |= GPIO_ZM516X_MOTOR_EN_BV;
  GPIO_ZM516X_MOTOR_PHASE_DDR |= GPIO_ZM516X_MOTOR_PHASE_BV;
  GPIO_ZM516X_ACK_DDR &= ~GPIO_ZM516X_ACK_BV;
  GPIO_ZM516X_DIR_DDR |= GPIO_ZM516X_DIR_BV;
  //GPIO_ZM516X_KEY1_DDR &= ~GPIO_ZM516X_KEY1_BV;
  //GPIO_ZM516X_KEY2_DDR &= ~GPIO_ZM516X_KEY2_BV;
  HalGpioSet(HAL_GPIO_ZM516X_ALL,0);
#endif
#if defined _USE_XBEE__
  GPIO_ZM516X_RESET_TURN_HIGH();
  GPIO_ZM516X_DEF_TURN_HIGH();
  GPIO_ZM516X_WAKEUP_TURN_HIGH();
  GPIO_ZM516X_SLEEP_TURN_HIGH();
  GPIO_ZM516X_MOTOR1_TURN_LOW();
  GPIO_ZM516X_MOTOR2_TURN_LOW();
  GPIO_ZM516X_MOTOR_EN_TURN_LOW();
  GPIO_ZM516X_MOTOR_PHASE_TURN_LOW();
  GPIO_ZM516X_ACK_TURN_HIGH()
  GPIO_ZM516X_DIR_TURN_HIGH()
  P2SEL &= 0XFE;
  GPIO_ZM516X_ACK_DDR |= GPIO_ZM516X_ACK_BV;
  GPIO_ZM516X_RESET_DDR |= GPIO_ZM516X_RESET_BV;
  GPIO_ZM516X_DEF_DDR |= GPIO_ZM516X_DEF_BV;
  GPIO_ZM516X_WAKEUP_DDR &= ~GPIO_ZM516X_WAKEUP_BV;
  GPIO_ZM516X_SLEEP_DDR |= GPIO_ZM516X_SLEEP_BV;
  GPIO_ZM516X_MOTOR1_DDR |= GPIO_ZM516X_MOTOR1_BV;
  GPIO_ZM516X_MOTOR2_DDR |= GPIO_ZM516X_MOTOR2_BV;
  GPIO_ZM516X_MOTOR_EN_DDR |= GPIO_ZM516X_MOTOR_EN_BV;
  GPIO_ZM516X_MOTOR_PHASE_DDR |= GPIO_ZM516X_MOTOR_PHASE_BV;
  GPIO_ZM516X_DIR_DDR |= GPIO_ZM516X_DIR_BV;
  //GPIO_ZM516X_KEY1_DDR &= ~GPIO_ZM516X_KEY1_BV;
  //GPIO_ZM516X_KEY2_DDR &= ~GPIO_ZM516X_KEY2_BV;
  HalGpioSet(HAL_GPIO_ZM516X_ALL,0);
#endif
}

/***************************************************************************************************
 * @fn      HalGpioSet
 *
 * @brief   Tun ON/OFF/TOGGLE given LEDs
 *
 * @param   led - bit mask value of leds to be turned ON/OFF/TOGGLE
 *          mode - BLINK, FLASH, TOGGLE, ON, OFF
 * @return  None
 ***************************************************************************************************/
uint8 HalGpioSet (uint8 pin, uint8 level)
{
  switch(pin)
  {
    case HAL_GPIO_ZM516X_RESET:
      GPIO_ZM516X_RESET_SBIT = level;
      break;
    case HAL_GPIO_ZM516X_DEF:
      GPIO_ZM516X_DEF_SBIT = level;
      break;
    case HAL_GPIO_ZM516X_SLEEP:
      GPIO_ZM516X_SLEEP_SBIT = level;
      break;
#if defined _USE_ZM516X__
    case HAL_GPIO_ZM516X_WAKEUP:
      GPIO_ZM516X_WAKEUP_SBIT = level;
      break;
#endif
#if defined _USE_XBEE__
    case HAL_GPIO_ZM516X_ACK:
      GPIO_ZM516X_ACK_SBIT = level;
      break;
#endif
    case HAL_GPIO_ZM516X_MOTOR1:
      GPIO_ZM516X_MOTOR1_SBIT = level;
      break;
    case HAL_GPIO_ZM516X_MOTOR2:
      GPIO_ZM516X_MOTOR2_SBIT = level;
      break;
    case HAL_GPIO_ZM516X_DIR:
      GPIO_ZM516X_DIR_SBIT = level;
      break;
    case HAL_GPIO_ZM516X_MOTOR_EN:
      GPIO_ZM516X_MOTOR_EN_SBIT = level;
      break;
    case HAL_GPIO_ZM516X_MOTOR_PHASE:
      GPIO_ZM516X_MOTOR_PHASE_SBIT = level;
      break;
    case HAL_GPIO_ZM516X_ALL:
      GPIO_ZM516X_RESET_SBIT = level;
      GPIO_ZM516X_DEF_SBIT = level;
      GPIO_ZM516X_SLEEP_SBIT = level;
#if defined _USE_ZM516X__
      GPIO_ZM516X_WAKEUP_SBIT = level;
#endif
      GPIO_ZM516X_MOTOR1_SBIT = level;
      GPIO_ZM516X_MOTOR2_SBIT = level;
      GPIO_ZM516X_MOTOR_EN_SBIT = level;
      GPIO_ZM516X_MOTOR_PHASE_SBIT = level;
      GPIO_ZM516X_DIR_SBIT = level;
      break;
    default:
      break;
  }
  return 0;
}

uint8 HalGpioGet (uint8 pin)
{
  if(pin == HAL_GPIO_ZM516X_WAKEUP)
  {
    return GPIO_ZM516X_WAKEUP_SBIT;
  }
  else
  {
    return 0;
  }
}

/***************************************************************************************************
***************************************************************************************************/




