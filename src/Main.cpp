#include <th/Encoding.hpp>
#include <th/Aheui/Interpreter.hpp>

#include <iostream>
#include <string>

#ifdef _WIN32
#	include <clocale>

#	include <fcntl.h>
#	include <io.h>
#	include <Windows.h>
#endif

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		std::cout << "Usage: ./talkheui <Aheui code path> [Extension path]\n";
		return 1;
	} else if (argc >= 4) {
		std::cout << "There are too many arguments\n";
		return 1;
	}
	
#ifdef _WIN32
	std::setlocale(LC_ALL, "");
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	th::aheui::Interpreter i;
	std::string sc;

	try {
		sc = th::ReadAsUTF8(argv[1]);
	} catch (...) {
		std::cout << "Failed to open the aheui code '" << argv[1] << "'\n";
		return 1;
	}

	if (argc == 3) {
		const std::string ex = argv[2];
		try {
			i.LoadExtension(ex);
			i.ConstructPipe(ex);
		} catch (...) {
			std::cout << "Failed to open the extension '" << argv[2] << "'\n";
			return 1;
		}
	}

	i.Run(sc);
	return i.Result();
}