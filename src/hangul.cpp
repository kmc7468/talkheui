#include <talkheui/hangul.hpp>

#include <cassert>

namespace talkheui
{
	bool ishangul(char32_t c) noexcept
	{
		return 0xAC00 <= c && c <= 0xD7A3;
	}
	jaso get_jaso(char32_t hangul) noexcept
	{
		assert(ishangul(hangul));

		static constexpr char32_t chosungs[] =
			{ U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��' };
		static constexpr char32_t jungsungs[] =
			{ U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��' };
		static constexpr char32_t jongsungs[] =
			{ 0, U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��', U'��' };
		
		hangul -= 0xAC00;
		const char32_t jongsung_index = hangul % 28;
		const char32_t jungsung_index = (hangul - jongsung_index) / 28 % 21;
		const char32_t chosung_index = (((hangul - jongsung_index) / 28) - jungsung_index) / 21;

		return { chosungs[chosung_index], jungsungs[jungsung_index], jongsungs[jongsung_index] };
	}
}