#pragma once

namespace talkheui
{
	struct jaso
	{
		char32_t chosung;
		char32_t jungsung;
		char32_t jongsung;
	};

	bool ishangul(char32_t c) noexcept;
	jaso get_jaso(char32_t hangul) noexcept;
}