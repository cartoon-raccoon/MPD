/*
 * Copyright (C) 2003-2012 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"
#include "DecoderAPI.hxx"
#include "InputStream.hxx"
#include "util/Error.hxx"
#include "Compiler.h"

#include <glib.h>

#include <unistd.h>

void
decoder_initialized(gcc_unused Decoder &decoder,
		    gcc_unused const AudioFormat audio_format,
		    gcc_unused bool seekable,
		    gcc_unused float total_time)
{
}

DecoderCommand
decoder_get_command(gcc_unused Decoder &decoder)
{
	return DecoderCommand::NONE;
}

void
decoder_command_finished(gcc_unused Decoder &decoder)
{
}

double
decoder_seek_where(gcc_unused Decoder &decoder)
{
	return 1.0;
}

void
decoder_seek_error(gcc_unused Decoder &decoder)
{
}

size_t
decoder_read(gcc_unused Decoder *decoder,
	     InputStream &is,
	     void *buffer, size_t length)
{
	return is.LockRead(buffer, length, IgnoreError());
}

void
decoder_timestamp(gcc_unused Decoder &decoder,
		  gcc_unused double t)
{
}

DecoderCommand
decoder_data(gcc_unused Decoder &decoder,
	     gcc_unused InputStream *is,
	     const void *data, size_t datalen,
	     gcc_unused uint16_t kbit_rate)
{
	gcc_unused ssize_t nbytes = write(1, data, datalen);
	return DecoderCommand::NONE;
}

DecoderCommand
decoder_tag(gcc_unused Decoder &decoder,
	    gcc_unused InputStream *is,
	    gcc_unused Tag &&tag)
{
	return DecoderCommand::NONE;
}

void
decoder_replay_gain(gcc_unused Decoder &decoder,
		    const ReplayGainInfo *rgi)
{
	const ReplayGainTuple *tuple = &rgi->tuples[REPLAY_GAIN_ALBUM];
	if (tuple->IsDefined())
		g_printerr("replay_gain[album]: gain=%f peak=%f\n",
			   tuple->gain, tuple->peak);

	tuple = &rgi->tuples[REPLAY_GAIN_TRACK];
	if (tuple->IsDefined())
		g_printerr("replay_gain[track]: gain=%f peak=%f\n",
			   tuple->gain, tuple->peak);
}

void
decoder_mixramp(gcc_unused Decoder &decoder, gcc_unused MixRampInfo &&mix_ramp)
{
}
