#pragma once

#include <talkheui/memory.hpp>

#include <string>
#include <vector>

namespace talkheui
{
	class runtime_state
	{
	public:
		virtual ~runtime_state();

	protected:
		runtime_state() noexcept = default;

	public:
		runtime_state& operator=(const runtime_state&) = delete;

	public:
		virtual void reset() = 0;

	public:
		std::vector<const memory*> memories() const;
		
	protected:
		std::vector<memory*>& memories() noexcept;
		void memories(std::vector<memory*> new_memories) noexcept;

	private:
		std::vector<memory*> memories_;
	};

	class interpreter
	{
	public:
		virtual ~interpreter();

	protected:
		interpreter(std::string name) noexcept;
		interpreter(const interpreter& interpreter);
		interpreter(interpreter&& interpreter) noexcept;

	protected:
		interpreter& operator=(const interpreter& interpreter);
		interpreter& operator=(interpreter&& interpreter) noexcept;

	public:
		virtual void reset() = 0;

	public:
		std::string name() const;
		const runtime_state* state() const noexcept;
		runtime_state* state() noexcept;
		void status(runtime_state* new_state) noexcept;

	private:
		std::string name_;
		runtime_state* state_ = nullptr;
	};
}