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
		const char32_t chosungIndex = (((hangul - jongsungIndex) / 28) - jungsungIndex) / 21;

		return { Chosungs[chosungIndex], Jungsungs[jungsungIndex], Jongsungs[jongsungIndex] };
	}

	int IsConsonant(char32_t c) noexcept {
		if (0x1100 <= c && c <= 0x1112) return 0x1100;
		else if (0x3131 <= c && c <= 0x314E) return 0x3131;
		else if (0xFFA1 <= c && c <= 0xFFBE) return 0xFFA1;
		else return 0;
	}
	char32_t NormalizeConsonant(char32_t consonant) noexcept {
		assert(IsConsonant(consonant));

		static constexpr char32_t Consonants[] =
			{ U'ㄱ', U'ㄲ', U'ㄳ', U'ㄴ', U'ㄵ', U'ㄶ', U'ㄷ', U'ㄸ', U'ㄹ', U'ㄺ', U'ㄻ', U'ㄼ', U'ㄽ', U'ㄾ', U'ㄿ', U'ㅀ', U'ㅁ', U'ㅂ', U'ㅃ', U'ㅄ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅉ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ' };
		
		return Consonants[consonant - IsConsonant(consonant)];
	}
}