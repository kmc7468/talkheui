#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

#include <th/IOStream.hpp>

#include <th/Encoding.hpp>

#include <cstdio>
#include <cwctype>
#include <string>

#include <boost/lexical_cast.hpp>

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
#ifdef TH_USE_MULTIPRECISION
	void WriteStdout(boost::multiprecision::int128_t integer) noexcept {
		const std::string integerStr = boost::lexical_cast<std::string>(integer);

#ifdef _WIN32
		const TextModeRAII raii(stdout);
		for (char c : integerStr) {
			std::fputwc(c, stdout);
		}
#else
		std::fputs(integerStr.c_str(), stdout);
#endif
	}
#endif
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
#ifdef TH_USE_MULTIPRECISION
	boost::multiprecision::int128_t ReadInteger128Stdin() noexcept {
		std::string temp;
		bool isFirst = true;

#ifdef _WIN32
		const TextModeRAII raii(stdin);
		
		while (true) {
			const std::wint_t c = std::fgetwc(stdin);
			if (c == WEOF) break;
			else if (isFirst && (c == L'+') || (c == L'-')) {
				isFirst = false;
				temp += static_cast<char>(c);
			} else if (std::iswdigit(c)) {
				isFirst = false;
				temp += static_cast<char>(c);
			} else {
				std::ungetwc(c, stdin);
				break;
			}
		}
#else
		while (true) {
			const int c = std::fgetc(stdin);
			if (c == EOF) break;
			else if (isFirst && (c == '+') || (c == '-')) {
				isFirst = false;
				temp += static_cast<char>(c);
			} else if (std::iswdigit(c)) {
				isFirst = false;
				temp += static_cast<char>(c);
			} else {
				std::ungetc(c, stdin);
				break;
			}
		}
#endif

		return boost::multiprecision::int128_t(temp);
	}
#endif
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