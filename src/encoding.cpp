#include <talkheui/encoding.hpp>

#include <utf8/utf8.h>

namespace talkheui
{
	std::u32string utf8to32(const std::string_view& utf8)
	{
		std::u32string result;
		utf8::utf8to32(utf8.begin(), utf8.end(), std::back_inserter(result));
		return result;
	}
	std::string utf32to8(const std::u32string_view& utf32)
	{
		std::string result;
		utf8::utf32to8(utf32.begin(), utf32.end(), std::back_inserter(result));
		return result;
	}
	std::u16string utf8to16(const std::string_view& utf8)
	{
		std::u16string result;
		utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(result));
		return result;
	}
	std::string utf16to8(const std::u16string_view& utf16)
	{
		std::string result;
		utf8::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(result));
		return result;
	}
	std::u16string utf32to16(const std::u32string_view& utf32)
	{
		return utf8to16(utf32to8(utf32));
	}
	std::u32string utf16to32(const std::u16string_view& utf16)
	{
		return utf8to32(utf16to8(utf16));
	}
}