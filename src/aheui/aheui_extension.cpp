#include <talkheui/aheui/extension.hpp>

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
	extension_type extension::type() const noexcept
	{
		return type_;
	}
}