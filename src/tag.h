/* the Music Player Daemon (MPD)
 * Copyright (C) 2003-2007 by Warren Dukes (warren.dukes@gmail.com)
 * This project's homepage is: http://www.musicpd.org
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef MPD_TAG_H
#define MPD_TAG_H

#include "gcc.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

enum tag_type {
	TAG_ITEM_ARTIST,
	TAG_ITEM_ALBUM,
	TAG_ITEM_ALBUM_ARTIST,
	TAG_ITEM_TITLE,
	TAG_ITEM_TRACK,
	TAG_ITEM_NAME,
	TAG_ITEM_GENRE,
	TAG_ITEM_DATE,
	TAG_ITEM_COMPOSER,
	TAG_ITEM_PERFORMER,
	TAG_ITEM_COMMENT,
	TAG_ITEM_DISC,
	TAG_NUM_OF_ITEM_TYPES
};

extern const char *mpdTagItemKeys[];

struct tag_item {
	enum tag_type type;
	char value[sizeof(long)];
} mpd_packed;

struct tag {
	int time;
	struct tag_item **items;
	uint8_t numOfItems;
};

struct tag *tag_ape_load(const char *file);

struct tag *tag_new(void);

void tag_lib_init(void);

void tag_clear_items_by_type(struct tag *tag, enum tag_type itemType);

void tag_free(struct tag *tag);

/**
 * Gives an optional hint to the tag library that we will now add
 * several tag items; this is used by the library to optimize memory
 * allocation.  Only one tag may be in this state, and this tag must
 * not have any items yet.  You must call tag_end_add() when you are
 * done.
 */
void tag_begin_add(struct tag *tag);

/**
 * Finishes the operation started with tag_begin_add().
 */
void tag_end_add(struct tag *tag);

void tag_add_item_n(struct tag *tag, enum tag_type itemType,
		    const char *value, size_t len);

static inline void tag_add_item(struct tag *tag, enum tag_type itemType,
				const char *value)
{
	tag_add_item_n(tag, itemType, value, strlen(value));
}

struct tag *tag_dup(const struct tag *tag);

/**
 * Merges the data from two tags.  If both tags share data for the
 * same tag_type, only data from "add" is used.
 *
 * @return a newly allocated tag, which must be freed with tag_free()
 */
struct tag *
tag_merge(const struct tag *base, const struct tag *add);

/**
 * Returns true if the tag contains no items.  This ignores the "time"
 * attribute.
 */
static inline bool
tag_is_empty(const struct tag *tag)
{
	return tag->numOfItems == 0;
}

/**
 * Checks whether the tag contains one or more items with
 * the specified type.
 */
bool tag_has_type(const struct tag *tag, enum tag_type type);

int tag_equal(const struct tag *tag1, const struct tag *tag2);

#endif
