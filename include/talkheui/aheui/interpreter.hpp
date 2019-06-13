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
		virtual void reset_step() override;

	public:
		std::size_t x() const noexcept;
		std::size_t y() const noexcept;
		int dx() const noexcept;
		int dy() const noexcept;

	private:
		std::size_t x_ = 0, y_ = 0;
		int dx_ = 0, dy_ = 1;
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
		virtual void run_script() override;
		virtual void run_script_step() override;

	protected:
		virtual void load_script_priv(const std::string_view& script) override;

	public:
		const codeplane& script() const noexcept;
		long long result() const noexcept;

	private:
		codeplane script_;
		long long result_;
	};
}