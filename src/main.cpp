#include <talkheui/aheui/extension.hpp>
#include <talkheui/encoding.hpp>

int main()
{
	/*talkheui::extension* ext = talkheui::open_extension("C:/Users/KMC/Desktop/새 폴더 (2).zip");
	auto* ext_la = static_cast<talkheui::aheui::lua_extension*>(ext);

	long long v = ext_la->receive();
	ext_la->send(10);*/

	std::string str = talkheui::read_as_utf8("C:/Users/KMC/Desktop/test.txt");

	return 0;
}