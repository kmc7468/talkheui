#pragma once

namespace th {
	struct Jaso final {
		char32_t Chosung;
		char32_t Jungsung;
		char32_t Jongsung;
	};

	bool IsHangul(char32_t c) noexcept;
	Jaso GetJaso(char32_t hangul) noexcept;

	int IsConsonant(char32_t c) noexcept;
	char32_t NormalizeConsonant(char32_t consonant) noexcept;
}