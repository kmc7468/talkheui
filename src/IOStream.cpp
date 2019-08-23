#include <th/IOStream.hpp>

#include <th/Encoding.hpp>

#include <cstdio>
#include <cwctype>
#include <string>

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
		
		long long input;
		if (std::wscanf(L"%lld", &input) == EOF) return -1;
		else return input;
#else
		long long input;
		if (std::scanf("%lld", &input) == EOF) return -1;
		else return input;
#endif
	}
	long long ReadCharacterStdin() noexcept {
#ifdef _WIN32
		const TextModeRAII raii(stdin);

		std::wint_t units[2];
		if ((units[0] = std::fgetwc(stdin)) == WEOF ||
			(units[0] == 0xFEFF && (units[0] = std::fgetwc(stdin)) == WEOF)) return -1;
		else if ((units[0] & 0xD800) == 0xD800) {
			units[1] = std::fgetwc(stdin);
			if (units[1] == WEOF) return -1;
			else return (units[0] - 0xD800) * 0x400 + (units[1] - 0xDC00) + 0x10000;
		} else return units[0];
#else
		int units[4];
		if ((units[0] = std::fgetc(stdin)) == EOF) return -1;
		else if (units[0] < 0x80) return units[0];
		else if ((units[0] & 0xF0) == 0xF0) {
			for (int i = 1; i < 4; ++i) {
				if ((units[i] = std::fgetc(stdin)) == EOF) return -1;
			}
			return ((units[0] & 0x07) << 18) + ((units[1] & 0x3F) << 12) + ((units[2] & 0x3F) << 6) + (units[3] & 0x3F);
		} else if ((units[0] & 0xE0) == 0xE0) {
			for (int i = 1; i < 3; ++i) {
				if ((units[i] = std::fgetc(stdin)) == EOF) return -1;
			}
			return ((units[0] & 0x0F) << 12) + ((units[1] & 0x3F) << 6) + (units[2] & 0x3F);
		} else {
			if ((units[1] = std::fgetc(stdin)) == EOF) return -1;
			else return ((units[0] & 0x1F) << 6) + (units[1] & 0x3F);
		}
#endif
	}
}