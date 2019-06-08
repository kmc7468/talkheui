#ifdef _MSC_VER
#pragma warning(disable: 4244)
#endif

#include <talkheui/aheui/extension.hpp>

#include <algorithm>
#include <cctype>

namespace talkheui::aheui
{
	extension::extension(extension_type type) noexcept
		: type_(type)
	{}
	extension::extension(extension_type type, extension_info info) noexcept
		: talkheui::extension(info), type_(type)
	{}
	extension::extension(const extension& extension)
		: talkheui::extension(extension), type_(extension.type_)
	{}
	extension::extension(extension&& extension) noexcept
		: talkheui::extension(std::move(extension)), type_(extension.type_)
	{}

	extension& extension::operator=(const extension& extension)
	{
		talkheui::extension::operator=(extension);
		type_ = extension.type_;
		return *this;
	}
	extension& extension::operator=(extension&& extension) noexcept
	{
		talkheui::extension::operator=(std::move(extension));
		type_ = extension.type_;
		return *this;
	}

	void extension::open_priv(const zip_reader& extension, const nlohmann::json& extension_info)
	{
		std::string ext_type_str = extension_info["type"];
		std::transform(ext_type_str.begin(), ext_type_str.end(), ext_type_str.begin(), std::tolower);
		ext_type_str[0] = std::toupper(ext_type_str.front());

		if (ext_type_str == "Lua") type_ = extension_type::lua;
		else if (ext_type_str == "Aheui") type_ = extension_type::aheui;
		else throw std::runtime_error("invalid extension");
	}

	extension_type extension::type() const noexcept
	{
		return type_;
	}
}