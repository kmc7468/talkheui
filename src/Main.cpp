#include <th/Encoding.hpp>
#include <th/ProgramOption.hpp>
#include <th/ah/Interpreter.hpp>

#include <cstdio>
#include <iostream>
#include <string>

#ifdef _WIN32
#	include <io.h>
#endif

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		std::cout << "Usage: ./talkheui --help\n";
		return 1;
	}

#ifdef _WIN32
	if (!_isatty(_fileno(stdout))) {
		const std::uint16_t temp = 0xFEFF;
		std::fwrite(&temp, sizeof(temp), 1, stdout);
	}
#endif

	th::ProgramOption option;
	if (!option.Parse(argc, argv)) {
		std::cout << option.Message() << '\n';
		return 1;
	}
	
	th::ah::Interpreter i;
	std::string sc;

	try {
		sc = th::ReadAsUTF8(option.SourcePath);
	} catch (...) {
		std::cout << "error: " << option.SourcePath << ": failed to read this source.\n";
		return 1;
	}

	if (option.ExtendedSource) {
		// TODO
	}

	if (!option.ExtensionPath.empty()) {
		try {
			i.LoadExtension(option.ExtensionPath);
			i.ConstructPipe(option.ExtensionPath);
		} catch (...) {
			std::cout << "error: " << option.ExtensionPath << ": failed to load this extension.\n";
			return 1;
		}
	}

	i.Run(sc);
	return static_cast<int>(i.Result());
}