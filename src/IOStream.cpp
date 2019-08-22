#include <th/IOStream.hpp>

#include <th/Encoding.hpp>

#include <cstdio>
#include <cwctype>
#include <string>

//test
#include <iostream>

#ifdef _WIN32
#	include <fcntl.h>
#	include <io.h>

namespace {
	struct TextModeRAII final {
		std::FILE* Stream;
		int OldTextMode = 0;

		TextModeRAII(std::FILE* stream) noexcept
			: Stream(stream) {
			OldTextMode = _setmode(_fileno(stream), _O_U16TEXT);
		}
		~TextModeRAII() {
			_setmode(_fileno(Stream), OldTextMode);
		}
	};
}
#endif

namespace th {
	void WriteStdout(long long integer) noexcept {
		const std::string integerStr = std::to_string(integer);

#ifdef _WIN32
		const TextModeRAII raii(stdout);
		for (char c : integerStr) {
			std::fputwc(c, stdout);
		}
#else
		std::fputs(integerStr.c_str(), stdout);
#endif
	}
	void WriteStdout(char32_t character) noexcept {
#ifdef _WIN32
		const TextModeRAII raii(stdout);
		const std::u16string characterUTF16 = UTF32To16(std::u32string_view(&character, 1));
		std::fputws(reinterpret_cast<const wchar_t*>(characterUTF16.c_str()), stdout);
#else
		const std::string characterUTF8 = UTF32To8(std::u32string_view(&character, 1));
		std::fputs(characterUTF8.c_str(), stdout);
#endif
	}

	long long ReadIntegerStdin() noexcept {
#ifdef _WIN32
		const TextModeRAII raii(stdin);
		
		std::wstring input;
		do {
			const wchar_t read = static_cast<wchar_t>(std::fgetwc(stdin));
			if (std::iswdigit(read) ||
			   (input.empty() && (read == L'+' || read == L'-'))) {
				input += read;
			} else if (input.empty() && (std::iswspace(read) || read == 0xFEFF)) {
				continue;
			} else {
				std::ungetwc(read, stdin);
				break;
			}
		} while (!std::feof(stdin));

		return std::stoll(input);
#else
		long long input;
		std::scanf("%lld", &input);

		return input;
#endif
	}
	char32_t ReadCharacterStdin() noexcept {
#ifdef _WIN32
		const TextModeRAII raii(stdin);

		wchar_t units[2];
		units[0] = static_cast<wchar_t>(std::fgetwc(stdin));
		if (units[0] == 0xFEFF) {
			units[0] = static_cast<wchar_t>(std::fgetwc(stdin));
		}

		if ((units[0] & 0xD800) == 0xD800) {
			units[1] = static_cast<wchar_t>(std::fgetwc(stdin));
			return (units[0] - 0xD800) * 0x400 + (units[1] - 0xDC00) + 0x10000;
		} else {
			return units[0];
		}
#else
		unsigned char fb = static_cast<unsigned char>(std::fgetc(stdin));
		if (fb < 0x80) {
			return fb;
		} else if ((fb & 0xF0) == 0xF0) {
			unsigned char sb, tb, frb;
			sb = static_cast<unsigned char>(std::fgetc(stdin));
			tb = static_cast<unsigned char>(std::fgetc(stdin));
			frb = static_cast<unsigned char>(std::fgetc(stdin));
			return ((fb & 0x07) << 18) + ((sb & 0x3F) << 12) + ((tb & 0x3F) << 6) + (frb & 0x3F);
		} else if ((fb & 0xE0) == 0xE0) {
			unsigned char sb, tb;
			sb = static_cast<unsigned char>(std::fgetc(stdin));
			tb = static_cast<unsigned char>(std::fgetc(stdin));
			return ((fb & 0x0F) << 12) + ((sb & 0x3F) << 6) + (tb & 0x3F);
		} else {
			unsigned char sb = static_cast<unsigned char>(std::fgetc(stdin));
			return ((fb & 0x1F) << 6) + (sb & 0x3F);
		}
#endif
	}
}