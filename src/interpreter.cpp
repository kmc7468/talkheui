#include <talkheui/interpreter.hpp>

#include <stdexcept>
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
	interpreter::interpreter(interpreter&& interpreter) noexcept
		: name_(std::move(interpreter.name_)), state_(interpreter.state_), extensions_(std::move(interpreter.extensions_))
	{
		interpreter.state_ = nullptr;
	}

	interpreter& interpreter::operator=(interpreter&& interpreter) noexcept
	{
		name_ = std::move(interpreter.name_);
		state_ = interpreter.state_;
		extensions_ = std::move(interpreter.extensions_);
		interpreter.state_ = nullptr;
		return *this;
	}

	void interpreter::reset()
	{
		reset_state();
		unload_script();
	}
	void interpreter::reset_state()
	{
		state_->reset();
	}
	void interpreter::reset_step()
	{
		state_->reset_step();
	}

	void interpreter::load_script(const std::string_view& script)
	{
		if (is_loaded_script()) unload_script();
		load_script_priv(script);
	}
	void interpreter::run(const std::string_view& script)
	{
		load_script(script);
		run_script();
	}

	void interpreter::load_extension(const std::string& path)
	{
		if (extensions_.find(path) != extensions_.end()) throw std::runtime_error("already loaded extension");

		extensions_.insert(std::make_pair(path, open_extension(path)));
	}
	void interpreter::unload_extension(const std::string& path)
	{
		auto iter = extensions_.find(path);
		if (iter == extensions_.end()) throw std::runtime_error("failed to find the extension");

		extensions_.erase(iter);
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
	void interpreter::state(runtime_state* new_state) noexcept
	{
		delete state_;
		state_ = new_state;
	}
	std::map<std::string, const extension*> interpreter::extensions() const
	{
		return std::map<std::string, const extension*>(extensions_.begin(), extensions_.end());
	}
}