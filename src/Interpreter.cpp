#include <th/Interpreter.hpp>

#include <utility>

namespace th {
	RuntimeState::~RuntimeState() {
		for (Memory* memory : m_Memories) {
			delete memory;
		}
	}

	std::vector<const Memory*> RuntimeState::Memories() const {
		return { m_Memories.begin(), m_Memories.end() };
	}
}

namespace th {
	Interpreter::~Interpreter() {
		delete m_State;
	}
	
	Interpreter::Interpreter(std::string name) noexcept
		: m_Name(std::move(name)) {
	}
	Interpreter::Interpreter(Interpreter&& interpreter) noexcept
		: m_Name(std::move(interpreter.m_Name)), m_State(interpreter.m_State), m_Extensions(std::move(interpreter.m_Extensions)) {
		interpreter.m_State = nullptr;
	}

	Interpreter& Interpreter::operator=(Interpreter&& interpreter) noexcept {
		m_Name = std::move(interpreter.m_Name);
		m_State = interpreter.m_State;
		m_Extensions = std::move(interpreter.m_Extensions);

		interpreter.m_State = nullptr;
		return *this;
	}
	
	void Interpreter::Reset() {
		ResetState();
		UnloadScript();
		vReset();
	}
	void Interpreter::ResetState() {
		m_State->Reset();
	}
	void Interpreter::ResetStep() {
		m_State->ResetStep();
	}

	void Interpreter::LoadScript(const std::string_view& script) {
		UnloadScript();
		vLoadScript(script);
	}

	void Interpreter::Run(const std::string_view& script) {
		LoadScript(script);
		RunScript();
	}

	void Interpreter::LoadExtension(const std::string& path) {
		if (m_Extensions.find(path) != m_Extensions.end()) throw std::runtime_error("already loaded extension");
		m_Extensions.insert(std::make_pair(path, OpenExtension(path)));
	}
	void Interpreter::UnloadExtension(const std::string& path) {
		const auto iter = m_Extensions.find(path);
		if (iter == m_Extensions.end()) throw std::runtime_error("failed to find the extension");
		m_Extensions.erase(iter);
	}

	std::string Interpreter::Name() const {
		return m_Name;
	}
	const RuntimeState* Interpreter::State() const noexcept {
		return m_State;
	}
	RuntimeState* Interpreter::State() noexcept {
		return m_State;
	}
	void Interpreter::State(RuntimeState* newState) noexcept {
		if (m_State == newState) return;

		delete m_State;
		m_State = newState;
	}

	std::map<std::string, const Extension*> Interpreter::Extensions() const {
		return { m_Extensions.begin(), m_Extensions.end() };
	}
}