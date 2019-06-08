#include <talkheui/interpreter.hpp>

#include <utility>

namespace talkheui
{
	runtime_state::~runtime_state()
	{
		for (memory* mem : memories_)
		{
			delete mem;
		}
	}

	std::vector<const memory*> runtime_state::memories() const
	{
		return std::vector<const memory*>(memories_.begin(), memories_.end());
	}

	std::vector<memory*>& runtime_state::memories() noexcept
	{
		return memories_;
	}
	void runtime_state::memories(std::vector<memory*> new_memories) noexcept
	{
		memories_ = std::move(new_memories);
	}
}

namespace talkheui
{
	interpreter::~interpreter()
	{
		delete state_;
	}

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
	const runtime_state* interpreter::state() const noexcept
	{
		return state_;
	}
	runtime_state* interpreter::state() noexcept
	{
		return state_;
	}
	void interpreter::status(runtime_state* new_state) noexcept
	{
		delete state_;
		state_ = new_state;
	}
}