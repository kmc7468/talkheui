#pragma once

#include <cstddef>
#include <string>

namespace talkheui
{
	class memory
	{
	public:
		virtual ~memory() = default;

	protected:
		memory(std::string name) noexcept;
		memory(const memory& memory);
		memory(memory&& memory) noexcept;

	protected:
		memory& operator=(const memory& memory);
		memory& operator=(memory&& memory) noexcept;

	public:
		virtual std::size_t bytes() const noexcept = 0;
		virtual std::size_t size() const noexcept = 0;

	public:
		std::string name() const;

	private:
		std::string name_;
	};
}