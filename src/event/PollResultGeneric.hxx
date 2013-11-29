/*
 * Copyright (C) 2003-2013 The Music Player Daemon Project
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

#ifndef MPD_EVENT_POLLRESULT_GENERIC_HXX
#define MPD_EVENT_POLLRESULT_GENERIC_HXX

#include "check.h"

#include <vector>

class PollResultGeneric
{
	struct Item
	{
		unsigned events;
		void *obj;

		Item() = default;
		Item(unsigned _events, void *_obj)
			: events(_events), obj(_obj) { }
	};

	std::vector<Item> items;
public:
	int GetSize() const { return items.size(); }
	unsigned GetEvents(int i) const { return items[i].events; }
	void *GetObject(int i) const { return items[i].obj; }
	void Reset() { items.clear(); }

	void Clear(void *obj) {
		for (auto i = items.begin(); i != items.end(); ++i)
			if (i->obj == obj)
				i->events = 0;
	}

	void Add(unsigned events, void *obj) {
		items.emplace_back(events, obj);
	}
};

#endif
