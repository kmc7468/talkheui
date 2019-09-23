#include <th/uh/Interpreter.hpp>

#include <th/Encoding.hpp>
#include <th/Hash.hpp>
#include <th/uh/Stack.hpp>

#include <cmath>
#include <string>

namespace th::uh {
	RuntimeState::RuntimeState() {
		Reset();
	}

	void RuntimeState::Reset() {
		m_Memories.clear();
		ResetStep();

		m_Memories.push_back(new Stack());
	}
	void RuntimeState::ResetStep() {
		Word = -1;
		CallStack.clear();
	}
}

namespace th::uh {
	Interpreter::Interpreter()
		: th::Interpreter("Unsuspected Hangeul") {
		State(new RuntimeState());
	}
	Interpreter::Interpreter(Interpreter&& interpreter) noexcept
		: th::Interpreter(std::move(interpreter)), m_Script(std::move(interpreter.m_Script)), m_Result(std::move(interpreter.m_Result)) {
	}
	
	Interpreter& Interpreter::operator=(Interpreter&& interpreter) noexcept {
		th::Interpreter::operator=(std::move(interpreter));

		m_Script = std::move(interpreter.m_Script);
		m_Result = std::move(interpreter.m_Result);
		return *this;
	}

	bool Interpreter::IsScriptLoaded() const {
		return !m_Script.IsEmpty();
	}
	void Interpreter::UnloadScript() {
		m_Script.Clear();
		m_Result.reset();
	}
	void Interpreter::RunScript() {
		m_Result.reset();

		while (!HasResult()) {
			RunScriptStep();
		}
	}
	void Interpreter::RunScriptStep() {
		// TODO
	}

	Object Interpreter::Result() const {
		return m_Result.value();
	}
	bool Interpreter::HasResult() const noexcept {
		return m_Result.has_value();
	}

	void Interpreter::vReset() {
	}

	void Interpreter::vLoadScript(const std::string_view& script) {
		const std::u32string scriptUTF32 = UTF8To32(script);
		m_Script.Parse(scriptUTF32);
	}
}