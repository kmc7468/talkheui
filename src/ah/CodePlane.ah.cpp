#ifdef _MSC_VER
#	pragma warning(disable: 4456)
#endif

#include <th/ah/CodePlane.hpp>

#include <utility>

#include <u5e/basic_grapheme_iterator.hpp>

namespace th::ah {
	CodePlane::CodePlane(const std::u32string& code) {
		Parse(code);
	}
	CodePlane::CodePlane(CodePlane&& code) noexcept
		: m_Lines(std::move(code.m_Lines)), m_Graphemes(std::move(code.m_Graphemes)) {
	}

	CodePlane& CodePlane::operator=(CodePlane&& code) noexcept {
		m_Lines = std::move(code.m_Lines);
		m_Graphemes = std::move(code.m_Graphemes);
		return *this;
	}

	void CodePlane::Clear() noexcept {
		m_Lines.clear();
		m_Graphemes.clear();
	}
	bool CodePlane::IsEmpty() const noexcept {
		return m_Lines.empty();
	}
	void CodePlane::Parse(const std::u32string& code) {
		Clear();

		using GIter = u5e::basic_grapheme_iterator<std::u32string>;

		std::size_t pos = 0, line = 0;
		while ((line = code.find(U'\n', pos)) != std::u32string::npos) {
			std::u32string& lineStr = m_Lines.emplace_back(code.substr(pos, line - pos));
			pos = line + 1;

			if (!lineStr.empty() && lineStr.back() == U'\r') {
				lineStr.erase(lineStr.end() - 1);
			}
		}

		if (pos < code.size()) {
			m_Lines.emplace_back(code.substr(pos));
		}

		for (std::u32string& line : m_Lines) {
			const GIter end(line.begin(), line.end(), line.end());
			std::vector<u5e::basic_grapheme<std::u32string>>& lineGraphemes = m_Graphemes.emplace_back();

			for (GIter iter(line.begin(), line.end()); iter != end; ++iter) {
				lineGraphemes.push_back(*iter);
			}
		}
	}

	u5e::basic_grapheme<std::u32string> CodePlane::At(int x, int y) const noexcept {
		if (x >= At(y).size()) {
			static const std::u32string dummy_str = U" ";
			return { dummy_str.begin(), dummy_str.begin() + 1 };
		} else return m_Graphemes[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
	}
	const std::vector<u5e::basic_grapheme<std::u32string>>& CodePlane::At(int y) const noexcept {
		return m_Graphemes[static_cast<std::size_t>(y)];
	}

	std::size_t CodePlane::MaxLines() const noexcept {
		return m_Lines.size();
	}
}