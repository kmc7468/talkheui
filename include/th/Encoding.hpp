#pragma once

#include <istream>
#include <string>
#include <string_view>

namespace th {
	std::u32string UTF8To32(const std::string_view& utf8);
	std::string UTF32To8(const std::u32string_view& utf32);
	std::u16string UTF8To16(const std::string_view& utf8);
	std::string UTF16To8(const std::u16string_view& utf16);
	std::u16string UTF32To16(const std::u32string_view& utf32);
	std::u32string UTF16To32(const std::u16string_view& utf16);

	std::string	ReadAsUTF8(const std::string& path);
	std::string ReadAsUTF8(std::istream& stream);
}