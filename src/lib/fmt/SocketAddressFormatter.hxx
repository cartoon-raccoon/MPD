// SPDX-License-Identifier: BSD-2-Clause
// author: Max Kellermann <max.kellermann@gmail.com>

#pragma once

#include "net/SocketAddress.hxx"
#include "net/ToString.hxx"

#include <fmt/format.h>

#include <concepts>

template<std::convertible_to<SocketAddress> T>
struct fmt::formatter<T> : formatter<string_view>
{
	template<typename FormatContext>
	auto format(SocketAddress address, FormatContext &ctx) {
		return formatter<string_view>::format(ToString(address), ctx);
	}
};
