/* buffer.h

   Definitions for the object management API protocol buffering... */

/*
 * Copyright (C) 2004-2022 Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1996-2003 by Internet Software Consortium
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *   Internet Systems Consortium, Inc.
 *   PO Box 360
 *   Newmarket, NH 03857 USA
 *   <info@isc.org>
 *   https://www.isc.org/
 *
 */

/* OMAPI buffers are ring buffers, which means that the beginning of the
   buffer and the end of the buffer chase each other around.   As long as
   the tail never catches up to the head, there's room in the buffer for
   data.

	- If the tail and the head are equal, the buffer is empty.

	- If the tail is less than the head, the contents of the buffer
	  are the bytes from the head to the end of buffer, and in addition,
	  the bytes between the beginning of the buffer and the tail, not
	  including the byte addressed by the tail.

	- If the tail is greater than the head, then the buffer contains
	  valid bytes starting with the byte addressed by the head, and
	  ending with the byte before the byte addressed by the tail.

   There will always be at least one byte of waste, because the tail can't
   increase so that it's equal to the head (that would represent an empty
   buffer. */
#define OMAPI_BUF_SIZE 4048
typedef struct _omapi_buffer {
	struct _omapi_buffer *next;	/* Buffers can be chained. */
	u_int32_t refcnt;		/* Buffers are reference counted. */
	u_int16_t head, tail;		/* Buffers are organized in a ring. */
	char buf [OMAPI_BUF_SIZE];	/* The actual buffer is included in
					   the buffer data structure. */
} omapi_buffer_t;

#define BUFFER_BYTES_FREE(x)	\
	((x) -> tail > (x) -> head \
	  ? sizeof ((x) -> buf) - ((x) -> tail - (x) -> head) \
	  : (x) -> head - (x) -> tail)

#define BYTES_IN_BUFFER(x)	\
	((x) -> tail > (x) -> head \
	 ? (x) -> tail - (x) -> head - 1 \
	 : sizeof ((x) -> buf) - ((x) -> head - (x) -> tail) - 1)

isc_result_t omapi_connection_require (omapi_object_t *, unsigned);
isc_result_t omapi_connection_copyout (unsigned char *,
				       omapi_object_t *, unsigned);
isc_result_t omapi_connection_copyin (omapi_object_t *,
				      const unsigned char *, unsigned);
isc_result_t omapi_connection_flush (omapi_object_t *);
isc_result_t omapi_connection_get_uint32 (omapi_object_t *, u_int32_t *);
isc_result_t omapi_connection_put_uint32 (omapi_object_t *, u_int32_t);
isc_result_t omapi_connection_get_uint16 (omapi_object_t *, u_int16_t *);
isc_result_t omapi_connection_put_uint16 (omapi_object_t *, u_int32_t);
