/*
 * Copyright (c) 2010-2012 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

/**
	@addtogroup hal_hcs08
	@{
	@file xbee/platform_hcs08.h
	Header for Freescale HCS08 platform (XBee Programmable using CodeWarrior).

	This file is automatically included by xbee/platform.h.
*/

#ifndef __XBEE_PLATFORM_CC254X
#define __XBEE_PLATFORM_CC254X

	// HCS08 doesn't have automatic __FUNCTION__ macro.
	#define __FUNCTION__		"xbee"

	#define BYTE_ORDER		LITTLE_ENDIAN

	// CodeWarrior has a memcpy2() that is more efficient than standard memcpy().
	// It returns void instead of dest, and assumes count > 0.  Decided not to
	// use it since too many locations required explicit checks for count == 0.
	#define _f_memcpy		memcpy
	#define _f_memset		memset

	// Use these until we have an official stdint.h to ship
	typedef signed char		int8_t;
	typedef unsigned char	uint8_t;
	typedef int					int16_t;
	typedef unsigned int		uint16_t;
	typedef long				int32_t;
	typedef unsigned long	uint32_t;

	typedef unsigned char	uint_fast8_t;
	typedef signed char		int_fast8_t;

	#define INT16_C(x)		(x)
	#define UINT16_C(x)		(x ## U)
	#define INT32_C(x)		(x ## L)
	#define UINT32_C(x)		(x ## UL)

	// CodeWarrior printf doesn't like "h" modifier, so define these
	// format specifiers and don't use the defaults.
	#define PRId16				"d"
	#define PRIu16				"u"
	#define PRIx16				"x"
	#define PRIX16				"X"

	// This type isn't in stdint.h
	typedef uint8_t			bool_t;

	// the "FAR" modifier is not used
	#define FAR

	// Elements needed to keep track of serial port settings.  Must have a
	// baudrate memember, other fields are platform-specific.

	#define SERIAL_PORT_SCI1	1
	#define SERIAL_PORT_SCI2	2

	struct xbee_cbuf_t;						// defined in xbee/cbuf.h
	typedef struct xbee_serial_t {
		uint32_t					baudrate;
		uint8_t					port;
		struct xbee_cbuf_t	*rxbuf;
	} xbee_serial_t;

	// Two serial ports on Programmable XBee.
	extern xbee_serial_t HOST_SERIAL_PORT;
	extern xbee_serial_t EMBER_SERIAL_PORT;

	// We'll use 1/1/2000 as the epoch, to match ZigBee.
	#define ZCL_TIME_EPOCH_DELTA	0

	#define HAVE_SWAP_FUNCS		1
	#ifdef XBEE_CW6
		#include "rtc.h"
		// Instead of calling functions, reference global variables directly.
		#define xbee_seconds_timer()			(SEC_TIMER)
		#define xbee_millisecond_timer()		(MS_TIMER)
	#endif

	// our millisecond timer has a 4ms resolution
	#define XBEE_MS_TIMER_RESOLUTION 4

	//void xbee_reset_radio( struct xbee_dev_t *xbee, bool_t asserted);
#endif		// __XBEE_PLATFORM_HCS08

