// SPDX-License-Identifier: BSD-2-Clause
// author: Max Kellermann <max.kellermann@gmail.com>

#pragma once

#include "util/DynamicFifoBuffer.hxx"
#include "util/SpanCast.hxx"

#include <fmt/core.h>

#include <cstddef>
#include <string_view>

#ifdef _UNICODE
#include <wchar.h>
#endif

class OutputStream;

/**
 * An #OutputStream wrapper that buffers its output to reduce the
 * number of OutputStream::Write() calls.
 *
 * All wchar_t based strings are converted to UTF-8.
 *
 * To make sure everything is written to the underlying #OutputStream,
 * call Flush() before destructing this object.
 */
class BufferedOutputStream {
	OutputStream &os;

	DynamicFifoBuffer<std::byte> buffer;

public:
	explicit BufferedOutputStream(OutputStream &_os,
				      size_t buffer_size=32768) noexcept
		:os(_os), buffer(buffer_size) {}

	/**
	 * Write the contents of a buffer.
	 */
	void Write(std::span<const std::byte> src);

	/**
	 * Write the given object.  Note that this is only safe with
	 * POD types.  Types with padding can expose sensitive data.
	 */
	template<typename T>
	void WriteT(const T &value) {
		Write(ReferenceAsBytes(value));
	}

	/**
	 * Write one narrow character.
	 */
	void Write(const char &ch) {
		WriteT(ch);
	}

	/**
	 * Write a string.
	 */
	void Write(std::string_view src) {
		Write(AsBytes(src));
	}

	void VFmt(fmt::string_view format_str, fmt::format_args args);

	template<typename S, typename... Args>
	void Fmt(const S &format_str, Args&&... args) {
		VFmt(format_str,
		     fmt::make_format_args(args...));
	}

#ifdef _UNICODE
	/**
	 * Write one wide character.
	 */
	void Write(const wchar_t &ch) {
		WriteWideToUTF8({&ch, 1});
	}

	/**
	 * Write a wide string.
	 */
	void Write(std::wstring_view src) {
		WriteWideToUTF8(src);
	}
#endif

	/**
	 * Write buffer contents to the #OutputStream.
	 */
	void Flush();

	/**
	 * Discard buffer contents.
	 */
	void Discard() noexcept {
		buffer.Clear();
	}

private:
	bool AppendToBuffer(std::span<const std::byte> src) noexcept;

#ifdef _UNICODE
	void WriteWideToUTF8(std::wstring_view src);
#endif
};

/**
 * Helper function which constructs a #BufferedOutputStream, calls the
 * given function and flushes the #BufferedOutputStream.
 */
template<typename F>
void
WithBufferedOutputStream(OutputStream &os, F &&f)
{
	BufferedOutputStream bos(os);
	f(bos);
	bos.Flush();
}
