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
	void interpreter::load_script(const std::string_view& script)
	{
		const std::u32string script_utf32 = utf8to32(script);
		script_.parse(script_utf32);
	}

	const codeplane& interpreter::script() const noexcept
	{
		return script_;
	}
}