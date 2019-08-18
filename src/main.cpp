#include <talkheui/aheui/interpreter.hpp>
#include <talkheui/encoding.hpp>

#include <iostream>
#include <string>

#ifdef _WIN32
#	include <fcntl.h>
#	include <io.h>
#	include <Windows.h>
#endif

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cout << "Usage: talkheui <Aheui code path> [Extension path]\n";
		return 0;
	}
	else if (argc > 3)
	{
		std::cout << "There are too many arguments\n";
		return 0;
	}

#ifdef _WIN32
	std::setlocale(LC_ALL, "");
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	talkheui::aheui::interpreter i;
	std::string sc;

	try
	{
		sc = talkheui::read_as_utf8(argv[1]);
	}
	catch (...)
	{
		std::cout << "Failed to open the aheui code '" << argv[1] << "'\n";
		return 0;
	}

	if (argc == 3)
	{
		std::string ex = argv[2];
		try
		{
			i.load_extension(ex);
			i.construct_pipe(ex);
		}
		catch (...)
		{
			std::cout << "Failed to open the extension '" << argv[1] << "'\n";
			return 0;
		}
	}

	i.run(sc);

	return 0;
}