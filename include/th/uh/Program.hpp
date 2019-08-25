#pragma once

#include <string>
#include <vector>

namespace th::uh {
	enum class Command {
		Giyeok,
		Nieun,
		Digeut,
		Rieul,
		Mieum,
		Bieup,
		Siot,
		Ieung,
		Jieut,
		Hieut,
	};

	class Program final {
	private:
		std::vector<std::vector<Command>> m_Words;

	public:
		Program() noexcept = default;
		Program(const Program& program);
		Program(Program&& program) noexcept;
		~Program() = default;

	public:
		Program& operator=(const Program& program);
		Program& operator=(Program&& program) noexcept;

	public:
		void Clear() noexcept;
		bool IsEmpty() const noexcept;
		void Parse(const std::u32string& code);
	};
}