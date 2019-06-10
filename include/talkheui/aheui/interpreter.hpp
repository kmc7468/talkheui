#pragma once

#include <talkheui/interpreter.hpp>
#include <talkheui/aheui/codeplane.hpp>
#include <talkheui/aheui/storage.hpp>

#include <string_view>

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

	public:
		virtual void unload_script() override;

		virtual bool is_loaded_script() const override;
		virtual void load_script(const std::string_view& script);

	public:
		const codeplane& script() const noexcept;

	private:
		codeplane script_;
	};
}