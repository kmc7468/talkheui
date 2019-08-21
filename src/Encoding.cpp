#include <th/Encoding.hpp>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iterator>
#include <stdexcept>

#include <utf8/utf8.h>

namespace {
	enum class Encoding {
		UTF8,
		UTF16,
		UTF16BE,
		UTF32,
		UTF32BE,
	};
	Encoding DetectEncoding(std::ifstream& stream) {
		static constexpr std::uint8_t UTF8Bom[] = { 0xEF, 0xBB, 0xBF };
		static constexpr std::uint8_t UTF16Bom[] = { 0xFF, 0xFE };
		static constexpr std::uint8_t UTF16BEBom[] = { 0xFE, 0xFF };
		static constexpr std::uint8_t UTF32Bom[] = { 0xFF, 0xFE, 0, 0 };
		static constexpr std::uint8_t UTF32BEBom[] = { 0, 0, 0xFE, 0xFF };

		const std::streampos pos = stream.tellg();

		std::uint8_t bomBuffer[4];
		stream.read(reinterpret_cast<char*>(bomBuffer), 4);
		const std::streamsize readCount = stream.gcount();

		if (readCount == 4) {
			if (std::equal(bomBuffer, bomBuffer + 4, UTF32Bom)) return Encoding::UTF32;
			else if (std::equal(bomBuffer, bomBuffer + 4, UTF32BEBom)) return Encoding::UTF32BE;
		}
		if (readCount == 3) {
			if (std::equal(bomBuffer, bomBuffer + 3, UTF8Bom)) {
				stream.clear();
				stream.seekg(pos + static_cast<std::streamoff>(3), std::ios::beg);
				return Encoding::UTF8;
			}
		}
		if (readCount == 2) {
			if (std::equal(bomBuffer, bomBuffer + 2, UTF16Bom)) {
				stream.clear();
				stream.seekg(pos + static_cast<std::streamoff>(2), std::ios::beg);
				return Encoding::UTF16;
			} else if (std::equal(bomBuffer, bomBuffer + 2, UTF16BEBom)) {
				stream.clear();
				stream.seekg(pos + static_cast<std::streamoff>(2), std::ios::beg);
				return Encoding::UTF16BE;
			}
		}

		stream.clear();
		stream.seekg(static_cast<std::streamoff>(pos), std::ios::beg);
		return Encoding::UTF8;
	}
}

namespace {
	enum class Endian {
		Little,
		Big,
	};
	Endian DetectEndian() {
		const std::uint32_t temp = 0x12345678;
		if (*reinterpret_cast<const std::uint8_t*>(&temp) == 0x78) return Endian::Little;
		else if (*reinterpret_cast<const std::uint8_t*>(&temp) == 0x12) return Endian::Big;
		else throw std::runtime_error("unsupported platform");
	}
}

namespace th {
	std::u32string UTF8To32(const std::string_view& utf8) {
		std::u32string result;
		return utf8::utf8to32(utf8.begin(), utf8.end(), std::back_inserter(result)), result;
	}
	std::string UTF32To8(const std::u32string_view& utf32) {
		std::string result;
		return utf8::utf32to8(utf32.begin(), utf32.end(), std::back_inserter(result)), result;
	}
	std::u16string UTF8To16(const std::string_view& utf8) {
		std::u16string result;
		return utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(result)), result;
	}
	std::string UTF16To8(const std::u16string_view& utf16) {
		std::string result;
		return utf8::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(result)), result;
	}
	std::u16string UTF32To16(const std::u32string_view& utf32) {
		return UTF8To16(UTF32To8(utf32));
	}
	std::u32string UTF16To32(const std::u16string_view& utf16) {
		return UTF8To32(UTF16To8(utf16));
	}

	std::string ReadAsUTF8(const std::string& path) {
		std::ifstream file(path);
		if (!file) throw std::runtime_error("failed to open the file");
		return ReadAsUTF8(file);
	}
	std::string ReadAsUTF8(std::istream& stream) {
		const Encoding encoding = DetectEncoding(stream);
		const Endian endian = DetectEndian();

		const std::streampos pos = stream.tellg();
		stream.seekg(0, std::ios::end);
		const std::size_t length = static_cast<std::size_t>(stream.tellg() - pos);
		stream.seekg(static_cast<std::streamoff>(pos), std::ios::beg);

		switch (encoding) {
		case Encoding::UTF8: {
			std::string	result(length, 0);
			stream.read(result.data(), static_cast<std::streamsize>(result.size()));
			const std::string::iterator invalidIter = utf8::find_invalid(result.begin(), result.end());

			if (invalidIter != result.end()) throw std::runtime_error("invalid encoding");
			return result;
		}

		case Encoding::UTF16:
		case Encoding::UTF16BE: {
			if (length % 2) throw std::runtime_error("invalid encoding");
			
			std::u16string result(length / 2, 0);
			stream.read(reinterpret_cast<char*>(result.data()), static_cast<std::streamsize>(length));

			if (static_cast<int>(endian) != static_cast<int>(encoding) - static_cast<int>(Encoding::UTF16)) {
				std::transform(result.begin(), result.end(), result.begin(), [](char16_t c) {
					return static_cast<char16_t>(((c & 0xFF) << 8) | ((c & 0xFF00) >> 8));
				});
			}

			return UTF16To8(result);
		}

		case Encoding::UTF32:
		case Encoding::UTF32BE: {
			if (length % 4) throw std::runtime_error("invalid encoding");
			
			std::u32string result(length / 4, 0);
			stream.read(reinterpret_cast<char*>(result.data()), static_cast<std::streamsize>(length));

			if (static_cast<int>(endian) != static_cast<int>(encoding) - static_cast<int>(Encoding::UTF32)) {
				std::transform(result.begin(), result.end(), result.begin(), [](char16_t c) {
					return static_cast<char32_t>(((c & 0xFF000000) >> 24) | ((c & 0xFF0000) >> 8) | ((c & 0xFF00) << 8) | ((c & 0xFF) << 24));
				});
			}

			return UTF32To8(result);
		}

		default:
			return "";
		}
	}
}