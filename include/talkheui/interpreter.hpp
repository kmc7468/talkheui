#pragma once

#include <talkheui/memory.hpp>

#include <string>
#include <vector>

namespace talkheui
{
	class runtime_status
	{
	public:
		virtual ~runtime_status();

	protected:
		runtime_status() noexcept = default;

	public:
		runtime_status& operator=(const runtime_status&) = delete;

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
		const runtime_status* status() const noexcept;
		runtime_status* status() noexcept;
		void status(runtime_status* new_status) noexcept;

	private:
		std::string name_;
		runtime_status* status_ = nullptr;
	};
}