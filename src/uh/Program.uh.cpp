#include <th/uh/Program.hpp>

#include <th/Hangul.hpp>

#include <cassert>
#include <utility>

namespace th::uh {
	Program::Program(const Program& program)
		: m_Words(program.m_Words) {
	}
	Program::Program(Program&& program) noexcept
		: m_Words(std::move(program.m_Words)) {
	}

	Program& Program::operator=(const Program& program) {
		m_Words = program.m_Words;
		return *this;
	}
	Program& Program::operator=(Program&& program) noexcept {
		m_Words = std::move(program.m_Words);
		return *this;
	}

	void Program::Clear() noexcept {
		m_Words.clear();
	}
	bool Program::IsEmpty() const noexcept {
		return m_Words.empty();
	}
	void Program::Parse(const std::u32string& code) {
		Clear();
		
		std::vector<Command>* currentWord = &m_Words.emplace_back();

		for (char32_t c : code) {
			if ((c == U' ' || c == U'\t' || c == U'\r' || c == U'\n') && !currentWord->empty()) {
				currentWord = &m_Words.emplace_back();
			}

			char32_t con;
			if (IsHangul(c)) {
				con = GetJaso(c).Chosung;
			} else if (IsConsonant(c)) {
				con = NormalizeConsonant(c);
			} else continue;

			switch (con) {
#define W(consonant) currentWord->push_back(Command:: consonant); break
			case U'ㄱ': case U'ㄲ': case U'ㅋ': W(Giyeok);
			case U'ㄴ': W(Nieun);
			case U'ㄷ': case U'ㄸ': case U'ㅌ': W(Digeut);
			case U'ㄹ': W(Rieul);
			case U'ㅁ': W(Mieum);
			case U'ㅂ': case U'ㅃ': case U'ㅍ': W(Bieup);
			case U'ㅅ': case 'ㅆ': W(Siot);
			case U'ㅈ': case U'ㅉ': case U'ㅊ': W(Jieut);
#undef W
#define W(a, b) currentWord->push_back(Command:: a); currentWord->push_back(Command:: b); break
			case U'ㄳ': W(Giyeok, Siot);
			case U'ㄵ': W(Nieun, Jieut);
			case U'ㄺ': W(Rieul, Giyeok);
			case U'ㄻ': W(Rieul, Mieum);
			case U'ㄼ': case U'ㄿ': W(Rieul, Bieup);
			case U'ㄽ': W(Rieul, Siot);
			case U'ㄾ': W(Rieul, Digeut);
			case U'ㅄ': W(Bieup, Siot);
#undef W
			case U'ㅇ':
				if (!currentWord->empty()) {
					currentWord = &m_Words.emplace_back();
				}
				currentWord->push_back(Command::Ieung);
				break;
			case U'ㅎ':
				if (!currentWord->empty()) {
					currentWord = &m_Words.emplace_back();
				}
				currentWord->push_back(Command::Hieut);
				break;
			case U'ㄶ':
				currentWord->push_back(Command::Nieun);
				if (!currentWord->empty()) {
					currentWord = &m_Words.emplace_back();
				}
				currentWord->push_back(Command::Hieut);
				break;
			case U'ㅀ':
				currentWord->push_back(Command::Rieul);
				if (!currentWord->empty()) {
					currentWord = &m_Words.emplace_back();
				}
				currentWord->push_back(Command::Hieut);
				break;
			default: assert(false); break;
			}
		}

		if (currentWord->empty()) {
			m_Words.erase(m_Words.end() - 1);
		}
	}
}