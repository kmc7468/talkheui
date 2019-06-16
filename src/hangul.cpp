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
			{ U'ㄱ', U'ㄲ', U'ㄴ', U'ㄷ', U'ㄸ', U'ㄹ', U'ㅁ', U'ㅂ', U'ㅃ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅉ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ' };
		static constexpr char32_t jungsungs[] =
			{ U'ㅏ', U'ㅐ', U'ㅑ', U'ㅒ', U'ㅓ', U'ㅔ', U'ㅕ', U'ㅖ', U'ㅗ', U'ㅘ', U'ㅙ', U'ㅚ', U'ㅛ', U'ㅜ', U'ㅝ', U'ㅞ', U'ㅟ', U'ㅠ', U'ㅡ', U'ㅢ', U'ㅣ' };
		static constexpr char32_t jongsungs[] =
			{ 0, U'ㄱ', U'ㄲ', U'ㄳ', U'ㄴ', U'ㄵ', U'ㄶ', U'ㄷ', U'ㄹ', U'ㄺ', U'ㄻ', U'ㄼ', U'ㄽ', U'ㄾ', U'ㄿ', U'ㅀ', U'ㅁ', U'ㅂ', U'ㅄ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ' };
		
		hangul -= 0xAC00;
		const char32_t jongsung_index = hangul % 28;
		const char32_t jungsung_index = (hangul - jongsung_index) / 28 % 21;
		const char32_t chosung_index = (((hangul - jongsung_index) / 28) - jungsung_index) / 21;

		return { chosungs[chosung_index], jungsungs[jungsung_index], jongsungs[jongsung_index] };
	}
}