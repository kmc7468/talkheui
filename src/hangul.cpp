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
			{ U'ぁ', U'あ', U'い', U'ぇ', U'え', U'ぉ', U'け', U'げ', U'こ', U'さ', U'ざ', U'し', U'じ', U'す', U'ず', U'せ', U'ぜ', U'そ', U'ぞ' };
		static constexpr char32_t jungsungs[] =
			{ U'た', U'だ', U'ち', U'ぢ', U'っ', U'つ', U'づ', U'て', U'で', U'と', U'ど', U'な', U'に', U'ぬ', U'ね', U'の', U'は', U'ば', U'ぱ', U'ひ', U'び' };
		static constexpr char32_t jongsungs[] =
			{ 0, U'ぁ', U'あ', U'ぃ', U'い', U'ぅ', U'う', U'ぇ', U'ぉ', U'お', U'か', U'が', U'き', U'ぎ', U'く', U'ぐ', U'け', U'げ', U'ご', U'さ', U'ざ', U'し', U'じ', U'ず', U'せ', U'ぜ', U'そ', U'ぞ' };
		
		hangul -= 0xAC00;
		const char32_t jongsung_index = hangul % 28;
		const char32_t jungsung_index = (hangul - jongsung_index) / 28 % 21;
		const char32_t chosung_index = (((hangul - jongsung_index) / 28) - jungsung_index) / 21;

		return { chosungs[chosung_index], jungsungs[jungsung_index], jongsungs[jongsung_index] };
	}
}