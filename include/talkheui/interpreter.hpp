#pragma once

#include <talkheui/extension.hpp>
#include <talkheui/memory.hpp>

#include <map>
#include <string>
#include <string_view>
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
		virtual void reset_step() = 0;

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
		interpreter(interpreter&& interpreter) noexcept;

	protected:
		interpreter& operator=(interpreter&& interpreter) noexcept;

	public:
		void reset();
		void reset_state();
		void reset_step();
		virtual void unload_script() = 0;

		virtual bool is_loaded_script() const = 0;
		void load_script(const std::string_view& script);
		virtual void run_script() = 0;
		virtual void run_script_step() = 0;
		void run(const std::string_view& script);

		void load_extension(const std::string& path);
		void unload_extension(const std::string& path);

	protected:
		virtual void load_script_priv(const std::string_view& script) = 0;

	public:
		std::string name() const;
		const runtime_state* state() const noexcept;
		runtime_state* state() noexcept;
		void state(runtime_state* new_state) noexcept;
		std::map<std::string, const extension*> extensions() const;

	private:
		std::string name_;
		runtime_state* state_ = nullptr;
		std::map<std::string, extension*> extensions_;
	};
}