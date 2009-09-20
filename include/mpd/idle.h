/* libmpdclient
   (c) 2003-2009 The Music Player Daemon Project
   This project's homepage is: http://www.musicpd.org

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   - Neither the name of the Music Player Daemon nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef MPD_IDLE_H
#define MPD_IDLE_H

#include <mpd/compiler.h>

#include <stdbool.h>

struct mpd_pair;
struct mpd_connection;

/**
 * This enum contains bit masks for all idle events.
 */
enum mpd_idle {
	/** song database has been updated*/
	MPD_IDLE_DATABASE = 0x1,

	/** a stored playlist has been modified, created, deleted or
	    renamed */
	MPD_IDLE_STORED_PLAYLIST = 0x2,

	/** the current playlist (queue) has been modified */
	MPD_IDLE_PLAYLIST = 0x4,

	/** the player state has changed: play, stop, pause, seek, ... */
	MPD_IDLE_PLAYER = 0x8,

	/** the volume has been modified */
	MPD_IDLE_MIXER = 0x10,

	/** an audio output device has been enabled or disabled */
	MPD_IDLE_OUTPUT = 0x20,

	/** options have changed: crossfade, random, repeat, ... */
	MPD_IDLE_OPTIONS = 0x40,

	/** a database update has started or finished. */
	MPD_IDLE_UPDATE = 0x100,
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns the name of the specified idle event.
 *
 * @param idle an idle event id
 * @return the name, or NULL if that event is not known
 */
mpd_const
const char *
mpd_idle_name(enum mpd_idle idle);

/**
 * Parses the name of an idle eveent.
 *
 * @param name an idle event name
 * @return the id, or 0 if that event is not known
 */
mpd_pure
enum mpd_idle
mpd_idle_name_parse(const char *name);

/**
 * Enters "idle" mode: MPD will stall the response until an event has
 * occured.  Call mpd_send_noidle() to abort the idle mode, or
 * mpd_recv_idle() to read the event mask (or synchronously wait for
 * events).
 */
bool
mpd_send_idle(struct mpd_connection *connection);

/**
 * Same as mpd_send_idle(), but listen only on specific events.
 *
 * @param connection the connection to MPD
 * @param mask a bit mask of idle events; must not be 0
 * @return a positive job id on success, 0 on error
 */
bool
mpd_send_idle_mask(struct mpd_connection *connection, enum mpd_idle mask);

/**
 * Tells MPD to leave the "idle" mode.  MPD will then respond with a
 * list of events which have occured (which may be empty).  Call
 * mpd_recv_idle() after that.
 */
bool
mpd_send_noidle(struct mpd_connection *connection);

/**
 * Parses a "changed" pair, which is part of MPD's response to the
 * "idle" command.
 *
 * @return an idle code, or 0 if the pair was not understood
 */
mpd_pure
enum mpd_idle
mpd_idle_parse_pair(const struct mpd_pair *pair);

/**
 * Waits until MPD sends the list of idle events and returns it in a
 * bit mask.
 *
 * During this function, the configured timeout (see
 * mpd_connection_set_timeout()) is disabled.  This function blcoks
 * forever, until either MPD sends a response, or an error occurs.
 */
enum mpd_idle
mpd_recv_idle(struct mpd_connection *connection);

/**
 * Shortcut for mpd_send_idle() and mpd_recv_idle().
 *
 * @param connection the connection to MPD
 * @return the event bit mask, or 0 on error
 */
enum mpd_idle
mpd_run_idle(struct mpd_connection *connection);

/**
 * Shortcut for mpd_send_idle_mask() and mpd_recv_idle().
 *
 * @param connection the connection to MPD
 * @param mask a bit mask of idle events; must not be 0
 * @return the event bit mask, or 0 on error
 */
enum mpd_idle
mpd_run_idle_mask(struct mpd_connection *connection, enum mpd_idle mask);

#ifdef __cplusplus
}
#endif

#endif
