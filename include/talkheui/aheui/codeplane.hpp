#pragma once

#include <string>
#include <u5e/basic_grapheme.hpp>
#include <vector>

namespace talkheui::aheui
{
	class codeplane final
	{
	public:
		codeplane() noexcept = default;
		codeplane(const std::u32string& code);
		codeplane(codeplane&& code) noexcept;
		~codeplane() = default;

	public:
		codeplane& operator=(codeplane&& code) noexcept;

	public:
		void clear() noexcept;
		void parse(const std::u32string& code);

	private:
		std::vector<std::u32string> lines_;
		std::vector<std::vector<u5e::basic_grapheme<std::u32string>>> graphemes_;
	};
}