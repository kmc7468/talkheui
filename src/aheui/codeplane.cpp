#include <talkheui/aheui/codeplane.hpp>

#include <utility>
#include <u5e/basic_grapheme_iterator.hpp>

namespace talkheui::aheui
{
	codeplane::codeplane(const std::u32string& code)
	{
		parse(code);
	}
	codeplane::codeplane(codeplane&& code) noexcept
		: lines_(std::move(code.lines_)), graphemes_(std::move(code.graphemes_))
	{}

	codeplane& codeplane::operator=(codeplane&& code) noexcept
	{
		lines_ = std::move(code.lines_);
		graphemes_ = std::move(code.graphemes_);
		return *this;
	}

	void codeplane::clear() noexcept
	{
		lines_.clear();
		graphemes_.clear();
	}
	bool codeplane::empty() const noexcept
	{
		return lines_.empty();
	}
	void codeplane::parse(const std::u32string& code)
	{
		clear();

		using giter = u5e::basic_grapheme_iterator<std::u32string>;
		
		std::size_t pos = 0, line = 0;
		while ((line = code.find(U'\n', pos)) != std::u32string::npos)
		{
			std::u32string& line_str = lines_.emplace_back(code.substr(pos, line - pos));
			pos = line + 1;

			if (!line_str.empty() && line_str.back() == U'\r') line_str.erase(line_str.end() - 1);

			const giter end = giter(line_str.begin(), line_str.end(), line_str.end());
			std::vector<u5e::basic_grapheme<std::u32string>>& line_graphemes = graphemes_.emplace_back();

			for (giter iter(line_str.begin(), line_str.end()); iter != end; ++iter)
			{
				line_graphemes.push_back(*iter);
			}
		}

		if (pos < code.size())
		{
			std::u32string& line_str = lines_.emplace_back(code.substr(pos));
			pos = line + 1;

			const giter end = giter(line_str.begin(), line_str.end(), line_str.end());
			std::vector<u5e::basic_grapheme<std::u32string>>& line_graphemes = graphemes_.emplace_back();

			for (giter iter(line_str.begin(), line_str.end()); iter != end; ++iter)
			{
				line_graphemes.push_back(*iter);
			}
		}
	}
}