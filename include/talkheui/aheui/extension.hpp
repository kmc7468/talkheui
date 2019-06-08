#pragma once

#include <talkheui/extension.hpp>

namespace talkheui::aheui
{
	enum class extension_type
	{
		lua,
		aheui,
	};

	class extension : public talkheui::extension
	{
	public:
		virtual ~extension() override = default;

	protected:
		extension(extension_type type) noexcept;
		extension(extension_type type, extension_info info) noexcept;
		extension(const extension& extension);
		extension(extension&& extension) noexcept;

	public:
		extension& operator=(const extension& extension);
		extension& operator=(extension&& extension) noexcept;

	public:
		extension_type type() const noexcept;

	private:
		extension_type type_;
	};
}