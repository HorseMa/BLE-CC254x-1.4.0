#include "xbee_at_cmd.h"
#include "npi.h"

void xbee_enter_at_mode(void)
{
  NPI_WriteTransport("+++", 3);
}

void xbee_exit_at_mode(void)
{
  
}

void xbee_keep_at_mode(void)
{
  
}
/*      Address cmd     */
void xbee_get_local_addr(void)
{
  
}

void xbee_get_device_type(void)
{
  
}
/*      Network cmd     */
void xbee_disassociat(void)
{
  
}

/*      Security cmd     */

/*      IO opt cmd     */
void xbee_dio3_set(unsigned char level)
{
  // low or high
}

void xbee_dio4_set(unsigned char level)
{
  // low or high
}

void xbee_dio5_set(unsigned char level)
{
  // low or high
}

void xbee_dio11_set(unsigned char level)
{
  // low or high
}
/*      Address cmd     */

/*      Diagnostics cmd     */
void xbee_associat_indicat(void)
{
  
}

void xbee_get_firmware_ver(void)
{
  
}

/*      xbee response     */
void xbee_response(void)
{
  
}