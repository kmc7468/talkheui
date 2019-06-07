#include <talkheui/interpreter.hpp>

#include <utility>

namespace talkheui
{
	interpreter::interpreter(std::string name) noexcept
		: name_(std::move(name))
	{}
	interpreter::interpreter(const interpreter& interpreter)
		: name_(interpreter.name_)
	{}
	interpreter::interpreter(interpreter&& interpreter) noexcept
		: name_(std::move(interpreter.name_))
	{}

	interpreter& interpreter::operator=(const interpreter& interpreter)
	{
		name_ = interpreter.name_;
		return *this;
	}
	interpreter& interpreter::operator=(interpreter&& interpreter) noexcept
	{
		name_ = std::move(interpreter.name_);
		return *this;
	}

	std::string interpreter::name() const
	{
		return name_;
	}
}