#pragma once

#include <string>

namespace talkheui
{
	struct extension_info final
	{
		std::string name;
		std::string developer;
		std::string description;
	};

	class extension
	{
	public:
		virtual ~extension() = default;

	protected:
		extension() noexcept = default;
		extension(extension_info info) noexcept;
		extension(const extension& extension);
		extension(extension&& extension) noexcept;

	protected:
		extension& operator=(const extension& extension);
		extension& operator=(extension&& extension) noexcept;

	public:
		std::string name() const;
		std::string developer() const;
		std::string description() const;

	private:
		extension_info info_;
	};
}