#include <talkheui/encoding.hpp>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <utf8/utf8.h>

namespace
{
	enum class encoding
	{
		utf8,
		utf16,
		utf16be,
		utf32,
		utf32be,
	};

	encoding detect_encoding(std::istream& stream)
	{
		static constexpr std::uint8_t utf8_bom[] = { 0xEF, 0xBB, 0xBF };
		static constexpr std::uint8_t utf16_bom[] = { 0xFF, 0xFE };
		static constexpr std::uint8_t utf16be_bom[] = { 0xFE, 0xFF };
		static constexpr std::uint8_t utf32_bom[] = { 0xFF, 0xFE, 0, 0 };
		static constexpr std::uint8_t utf32be_bom[] = { 0, 0, 0xFE, 0xFF };
		
		const std::streampos pos = stream.tellg();

		std::uint8_t bom_buf[4];
		stream.read(reinterpret_cast<char*>(bom_buf), 4);

		const std::streamsize read_count = stream.gcount();
		if (read_count == 0) return encoding::utf8;

		if (read_count == 4)
		{
			if (std::equal(bom_buf, bom_buf + 4, utf32_bom)) return encoding::utf32;
			else if (std::equal(bom_buf, bom_buf + 4, utf32be_bom)) return encoding::utf32be;
		}
		if (read_count >= 3 && std::equal(bom_buf, bom_buf + 3, utf8_bom))
		{
			stream.clear();
			stream.seekg(pos + static_cast<std::streamoff>(3), std::ios::beg);
			return encoding::utf8;
		}
		if (read_count >= 2)
		{
			if (std::equal(bom_buf, bom_buf + 2, utf16_bom))
			{
				stream.clear();
				stream.seekg(pos + static_cast<std::streamoff>(2), std::ios::beg);
				return encoding::utf16;
			}
			else if (std::equal(bom_buf, bom_buf + 2, utf16be_bom))
			{
				stream.clear();
				stream.seekg(pos + static_cast<std::streamoff>(2), std::ios::beg);
				return encoding::utf16be;
			}
		}

		stream.clear();
		stream.seekg(pos, std::ios::beg);
		return encoding::utf8;
	}
}

namespace
{
	enum class endian
	{
		little,
		big,
	};

	endian detect_endian()
	{
		const std::uint32_t temp = 0x12345678;
		if (*reinterpret_cast<const std::uint8_t*>(&temp) == 0x78) return endian::little;
		else if (*reinterpret_cast<const std::uint8_t*>(&temp) == 0x12) return endian::big;
		else throw std::runtime_error("unsupported platform");
	}
}

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

	std::string read_as_utf8(const std::string& path)
	{
		std::ifstream file(path);
		if (!file) throw std::runtime_error("failed to open the file");

		return read_as_utf8(file);
	}
	std::string talkheui::read_as_utf8(std::istream& stream)
	{
		const encoding enc = detect_encoding(stream);

		const std::streampos pos = stream.tellg();
		stream.seekg(0, std::ios::end);
		const std::streamoff length = stream.tellg() - pos;
		stream.seekg(pos, std::ios::beg);

		static const endian end = detect_endian();

		switch (enc)
		{
		case encoding::utf8:
		{
			std::string result(static_cast<std::size_t>(length), 0);
			stream.read(result.data(), static_cast<std::streamsize>(result.size()));
			const std::string::iterator invalid_iter = utf8::find_invalid(result.begin(), result.end());

			if (invalid_iter != result.end()) throw std::runtime_error("invalid encoding");
			return result;
		}

		case encoding::utf16:
		case encoding::utf16be:
		{
			if (static_cast<std::size_t>(length - pos) % 2) throw std::runtime_error("invalid encoding");

			std::u16string result(static_cast<std::size_t>(length) / 2, 0);
			stream.read(reinterpret_cast<char*>(result.data()), static_cast<std::streamsize>(result.size()) * 2);

			if ((end == endian::little && enc == encoding::utf16be) || (end == endian::big && enc == encoding::utf16))
			{
				std::transform(result.begin(), result.end(), result.begin(), [](char16_t c)
					{
						return static_cast<char16_t>(((c & 0xFF) << 8) | ((c & 0xFF00) >> 8));
					});
			}

			return utf16to8(result);
		}

		case encoding::utf32:
		case encoding::utf32be:
		{
			if (static_cast<std::size_t>(length - pos) % 4) throw std::runtime_error("invalid encoding");

			std::u32string result(static_cast<std::size_t>(length) / 4, 0);
			stream.read(reinterpret_cast<char*>(result.data()), static_cast<std::streamsize>(result.size()) * 4);

			if ((end == endian::little && enc == encoding::utf32be) || (end == endian::big && enc == encoding::utf32))
			{
				std::transform(result.begin(), result.end(), result.begin(), [](char32_t c)
					{
						return static_cast<char32_t>(((c & 0xFF000000) >> 24) | ((c & 0xFF0000) >> 8) | ((c & 0xFF00) << 8) | ((c & 0xFF) << 24));
					});
			}

			return utf32to8(result);
		}

		default:
			return "";
		}
	}
}