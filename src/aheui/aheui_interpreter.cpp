#include <talkheui/aheui/interpreter.hpp>

#include <talkheui/encoding.hpp>

#include <utility>

namespace talkheui::aheui
{
	runtime_state::runtime_state()
	{
		reset();
	}

	void runtime_state::reset()
	{
		memories().clear();

		for (int i = 0; i < 27; ++i)
		{
			memories().push_back(new stack());
		}

		memories().insert(memories().begin() + 21, new queue());

		reset_step();
	}
	void runtime_state::reset_step()
	{
		x_ = y_ = 0;
		dx_ = 0;
		dy_ = 1;
	}

	std::size_t runtime_state::x() const noexcept
	{
		return x_;
	}
	std::size_t runtime_state::y() const noexcept
	{
		return y_;
	}
	int runtime_state::dx() const noexcept
	{
		return dx_;
	}
	int runtime_state::dy() const noexcept
	{
		return dy_;
	}
}

namespace talkheui::aheui
{
	interpreter::interpreter() noexcept
		: talkheui::interpreter("Aheui")
	{
		state(new runtime_state());
	}
	interpreter::interpreter(interpreter&& interpreter) noexcept
		: talkheui::interpreter(std::move(interpreter))
	{}

	interpreter& interpreter::operator=(interpreter&& interpreter) noexcept
	{
		talkheui::interpreter::operator=(std::move(interpreter));
		return *this;
	}

	void interpreter::unload_script()
	{
		script_.clear();
	}

	bool interpreter::is_loaded_script() const
	{
		return !script_.empty();
	}
	void interpreter::run_script()
	{
		// TODO
	}
	void interpreter::run_script_step()
	{
		// TODO
	}

	void interpreter::load_script_priv(const std::string_view& script)
	{
		const std::u32string script_utf32 = utf8to32(script);
		script_.parse(script_utf32);
	}

	const codeplane& interpreter::script() const noexcept
	{
		return script_;
	}
	long long interpreter::result() const noexcept
	{
		return result_;
	}
}