#include <th/Encoding.hpp>
#include <th/Aheui/Interpreter.hpp>

#include <cstdio>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		std::cout << "Usage: ./talkheui <Aheui code path> [Extension path]\n";
		return 1;
	} else if (argc >= 4) {
		std::cout << "There are too many arguments\n";
		return 1;
	}

#ifdef _WIN32
	std::fpos_t pos;
	std::fgetpos(stdout, &pos);
	if (pos != -1) {
		const std::uint16_t bom = 0xFEFF;
		std::fwrite(&bom, sizeof(bom), 1, stdout);
	}
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
	return static_cast<int>(i.Result());
}