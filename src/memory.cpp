#include <talkheui/memory.hpp>

#include <utility>

namespace talkheui
{
	memory::memory(std::string name) noexcept
		: name_(std::move(name))
	{}
	memory::memory(const memory& memory)
		: name_(memory.name_)
	{}
	memory::memory(memory&& memory) noexcept
		: name_(std::move(memory.name_))
	{}

	memory& memory::operator=(const memory& memory)
	{
		name_ = memory.name_;
		return *this;
	}
	memory& memory::operator=(memory&& memory) noexcept
	{
		name_ = std::move(memory.name_);
		return *this;
	}

	std::string memory::name() const
	{
		return name_;
	}
}