#include <th/Hangul.hpp>

#include <cassert>

namespace th {
	bool IsHangul(char32_t c) noexcept {
		return 0xAC00 <= c && c <= 0xD7A3;
	}
	Jaso GetJaso(char32_t hangul) noexcept {
		assert(IsHangul(hangul));

		static constexpr char32_t Chosungs[] =
			{ U'ㄱ', U'ㄲ', U'ㄴ', U'ㄷ', U'ㄸ', U'ㄹ', U'ㅁ', U'ㅂ', U'ㅃ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅉ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ' };
		static constexpr char32_t Jungsungs[] =
			{ U'ㅏ', U'ㅐ', U'ㅑ', U'ㅒ', U'ㅓ', U'ㅔ', U'ㅕ', U'ㅖ', U'ㅗ', U'ㅘ', U'ㅙ', U'ㅚ', U'ㅛ', U'ㅜ', U'ㅝ', U'ㅞ', U'ㅟ', U'ㅠ', U'ㅡ', U'ㅢ', U'ㅣ' };
		static constexpr char32_t Jongsungs[] =
			{ 0, U'ㄱ', U'ㄲ', U'ㄳ', U'ㄴ', U'ㄵ', U'ㄶ', U'ㄷ', U'ㄹ', U'ㄺ', U'ㄻ', U'ㄼ', U'ㄽ', U'ㄾ', U'ㄿ', U'ㅀ', U'ㅁ', U'ㅂ', U'ㅄ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ' };

		hangul -= 0xAC00;
		const char32_t jongsungIndex = hangul % 28;
		const char32_t jungsungIndex = (hangul - jongsungIndex) / 28 % 21;
		const char32_t chosungIndex = (((hangul - jongsungIndex) / 28) - chosungIndex) / 21;

		return { Chosungs[chosungIndex], Jungsungs[jungsungIndex], Jongsungs[jongsungIndex] };
	}
}