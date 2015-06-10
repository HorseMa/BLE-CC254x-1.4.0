#ifndef _ZIGBEE_H__
#define _ZIGBEE_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */


// Simple BLE Peripheral Task Events
#define ZIGBEE_START_DEVICE_EVT                              0x0001
#define ZIGBEE_PERIODIC_EVT                                  0x0002
#define ZIGBEE_READ_ZM516X_INFO_EVT                          0x0004
#define UART_RECEIVE_EVT                                     0x0008
#define BOARD_TEST_EVT                                       0x0010

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the BLE Application
 */
extern void Zigbee_Init( uint8 task_id );

/*
 * Task Event Processor for the BLE Application
 */
extern uint16 Zigbee_ProcessEvent( uint8 task_id, uint16 events );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEBLEPERIPHERAL_H */