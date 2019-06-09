#pragma once

#include <string>
#include <string_view>

namespace talkheui
{
	std::u32string utf8to32(const std::string_view& utf8);
	std::string utf32to8(const std::u32string_view& utf32);
	std::u16string utf8to16(const std::string_view& utf8);
	std::string utf16to8(const std::u16string_view& utf16);
	std::u16string utf32to16(const std::u32string_view& utf32);
	std::u32string utf16to32(const std::u16string_view& utf16);

	std::string read_as_utf8(const std::string& path);
}