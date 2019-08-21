#pragma once

#include <talkheui/interpreter.hpp>
#include <talkheui/aheui/codeplane.hpp>

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

namespace talkheui::aheui
{
	class extension;
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
		int x() const noexcept;
		void x(int new_x) noexcept;
		int y() const noexcept;
		void y(int new_y) noexcept;
		int dx() const noexcept;
		void dx(int new_dx) noexcept;
		int dy() const noexcept;
		void dy(int new_dy) noexcept;
		aheui::extension* extension() const noexcept;
		void extension(aheui::extension* new_extension);
		std::size_t selected_memory() const noexcept;
		void selected_memory(std::size_t new_selected_memory) noexcept;

	private:
		int x_ = 0, y_ = -1;
		int dx_ = 0, dy_ = 1;
		aheui::extension* extension_ = nullptr;
		std::size_t selected_memory_ = 0;
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

	public:
		void construct_pipe(const std::string& path);
		void deconstruct_pipe();

	protected:
		virtual void reset_priv() override;
		
		virtual void load_script_priv(const std::string_view& script) override;

	private:
		void move_cursor();
		void reverse_cursor();
		void update_cursor(runtime_state* state, char32_t jungsung);

	public:
		const codeplane& script() const noexcept;
		long long result() const noexcept;
		bool has_result() const noexcept;

	private:
		codeplane script_;
		std::optional<long long> result_;
	};
}