#pragma once

namespace th {
	struct Jaso final {
		char32_t Chosung;
		char32_t Jungsung;
		char32_t Jongsung;
	};

	bool IsHangul(char32_t c) noexcept;
	Jaso GetJaso(char32_t hangul) noexcept;
}