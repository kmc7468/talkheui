#pragma once

#include <string>

namespace th {
	class ProgramOption final {
	public:
		std::string SourcePath;
		std::string ExtensionPath;

		bool ExtendedSource = true;

	private:
		std::string m_Message;

	public:
		ProgramOption() noexcept = default;
		ProgramOption(const ProgramOption& option);
		ProgramOption(ProgramOption&& option) noexcept;
		~ProgramOption() = default;

	public:
		ProgramOption& operator=(const ProgramOption& option);
		ProgramOption& operator=(ProgramOption&& option) noexcept;
		bool operator==(const ProgramOption&) = delete;
		bool operator!=(const ProgramOption&) = delete;

	public:
		void Clear() noexcept;
		bool IsEmpty() const noexcept;

		bool Parse(int argc, char* argv[]);
		std::string Message() const;
	};
}