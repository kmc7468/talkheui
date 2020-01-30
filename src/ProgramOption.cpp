#include <th/ProgramOption.hpp>

#include <string_view>
#include <utility>

namespace th {
	ProgramOption::ProgramOption(const ProgramOption& option)
		: SourcePath(option.SourcePath), ExtensionPath(option.ExtensionPath),
		ExtendedSource(option.ExtendedSource),
		m_Message(option.m_Message) {
	}
	ProgramOption::ProgramOption(ProgramOption&& option) noexcept
		: SourcePath(std::move(option.SourcePath)), ExtensionPath(std::move(option.ExtensionPath)),
		ExtendedSource(option.ExtendedSource),
		m_Message(std::move(option.m_Message)) {
	}

	ProgramOption& ProgramOption::operator=(const ProgramOption& option) {
		SourcePath = option.SourcePath;
		ExtensionPath = option.ExtensionPath;

		ExtendedSource = option.ExtendedSource;

		m_Message = option.m_Message;

		return *this;
	}
	ProgramOption& ProgramOption::operator=(ProgramOption&& option) noexcept {
		SourcePath = std::move(option.SourcePath);
		ExtensionPath = std::move(option.ExtensionPath);

		ExtendedSource = option.ExtendedSource;

		m_Message = std::move(option.m_Message);

		return *this;
	}

	void ProgramOption::Clear() noexcept {
		SourcePath.clear();
		ExtensionPath.clear();

		ExtendedSource = true;

		m_Message.clear();
	}
	bool ProgramOption::IsEmpty() const noexcept {
		return SourcePath.empty() && ExtensionPath.empty() &&
			ExtendedSource &&
			m_Message.empty();
	}

	bool ProgramOption::Parse(int argc, char* argv[]) {
		bool extendedSource = false;

		for (int i = 1; i < argc; ++i) {
			const std::string_view arg = argv[i];
			
			if (arg[0] == '-') {
				if (arg.size() == 1) {
					m_Message = "error: unrecognized command line option '-'";
					return false;
				}

				const std::string_view option = arg.substr(1);
				
				if (option == "-e") {
					if (!ExtensionPath.empty()) {
						m_Message = "error: duplicate command line option '-e'";
						return false;
					} else if (i + 1 == argc || argv[i + 1][0] == '-') {
						m_Message = "error: missing filename after '-e'";
						return false;
					}
					
					ExtensionPath = argv[++i];
				} else if (option == "-r") {
					if (extendedSource) {
						m_Message = "error: duplicate command line option '-r'";
						return false;
					}

					ExtendedSource = false;
					extendedSource = true;
				} else if (option == "-help") {
					m_Message = "Usage: ./talkheui [options] source\n"
								"Options:\n"
								"  --help                   Display this information.\n"
								"  --version                Display version information.\n"
								"\n"
								"  -e <file>                Register <file> as an extension.\n"
								"  -r                       Disable extended source parser.\n";
					return false;
				} else {
					m_Message = "error: unrecognized command line option '" + std::string(arg) + '\'';
					return false;
				}
			} else {
				if (!SourcePath.empty()) {
					m_Message = "error: too many source files";
					return false;
				}

				SourcePath = arg;
			}
		}

		if (SourcePath.empty()) {
			m_Message = "error: no source file";
			return false;
		} else return true;
	}
	std::string ProgramOption::Message() const {
		return m_Message;
	}
}