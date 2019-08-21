#pragma once

#include <string>
#include <vector>

#include <u5e/basic_grapheme.hpp>

namespace th::aheui {
	class CodePlane final {
	private:
		std::vector<std::u32string> m_Lines;
		std::vector<std::vector<u5e::basic_grapheme<std::u32string>>> m_Graphemes;

	public:
		CodePlane() noexcept = default;
		CodePlane(const std::u32string& code);
		CodePlane(CodePlane&& code) noexcept;
		~CodePlane() = default;

	public:
		CodePlane& operator=(CodePlane&& code) noexcept;

	public:
		void Clear() noexcept;
		bool IsEmpty() const noexcept;
		void Parse(const std::u32string& code);

		u5e::basic_grapheme<std::u32string> At(int x, int y) const noexcept;
		const std::vector<u5e::basic_grapheme<std::u32string>>& At(int y) const noexcept;

		std::size_t MaxLines() const noexcept;
	};
}