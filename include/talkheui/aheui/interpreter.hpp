#pragma once

#include <talkheui/interpreter.hpp>
#include <talkheui/aheui/storage.hpp>

namespace talkheui::aheui
{
	class interpreter;

	class runtime_state final : public talkheui::runtime_state
	{
		friend class aheui::interpreter;

	public:
		virtual ~runtime_state() override = default;

	private:
		runtime_state();
		
	public:
		runtime_state& operator=(const runtime_state&) = delete;

	public:
		virtual void reset() override;
	};

	class interpreter final : public talkheui::interpreter
	{
	public:
		interpreter() noexcept;
		interpreter(interpreter&& interpreter) noexcept;
		virtual ~interpreter() override = default;

	public:
		interpreter& operator=(interpreter&& interpreter) noexcept;
	};
}