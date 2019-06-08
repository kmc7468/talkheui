#include <talkheui/extension.hpp>

#include <utility>

namespace talkheui
{
	extension::extension(extension_info info) noexcept
		: info_(std::move(info))
	{}
	extension::extension(const extension& extension)
		: info_(extension.info_)
	{}
	extension::extension(extension&& extension) noexcept
		: info_(std::move(extension.info_))
	{}

	extension& extension::operator=(const extension& extension)
	{
		info_ = extension.info_;
		return *this;
	}
	extension& extension::operator=(extension&& extension) noexcept
	{
		info_ = std::move(extension.info_);
		return *this;
	}

	std::string extension::name() const
	{
		return info_.name;
	}
	std::string extension::developer() const
	{
		return info_.developer;
	}
	std::string extension::description() const
	{
		return info_.description;
	}
}