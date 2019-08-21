#pragma once

#include <th/Extension.hpp>
#include <th/Memory.hpp>

#include <map>
#include <string>
#include <vector>

namespace th {
	class RuntimeState {
	protected:
		std::vector<Memory*> m_Memories;

	public:
		virtual ~RuntimeState();

	protected:
		RuntimeState() noexcept = default;

	public:
		RuntimeState& operator=(const RuntimeState&) = delete;

	public:
		virtual void Reset() = 0;
		virtual void ResetStep() = 0;

	public:
		std::vector<const Memory*> Memories() const;
	};

	class Interpreter {
	private:
		std::string m_Name;
		RuntimeState* m_State = nullptr;
		std::map<std::string, Extension*> m_Extensions;

	public:
		virtual ~Interpreter();

	protected:
		Interpreter(std::string name) noexcept;
		Interpreter(Interpreter&& interpreter) noexcept;

	protected:
		Interpreter& operator=(Interpreter&& interpreter) noexcept;

	public:
		void Reset();
		void ResetState();
		void ResetStep();

		virtual bool IsScriptLoaded() const = 0;
		void LoadScript(const std::string_view& script);
		virtual void UnloadScript() = 0;
		virtual void RunScript() = 0;
		virtual void RunScriptStep() = 0;

		void Run(const std::string_view& script);

		void LoadExtension(const std::string& path);
		void UnloadExtension(const std::string& path);

	protected:
		virtual void vReset() = 0;

		virtual void vLoadScript(const std::string_view& script) = 0;

	public:
		std::string Name() const;
		const RuntimeState* State() const noexcept;
		RuntimeState* State() noexcept;
		void State(RuntimeState* newState) noexcept;
		std::map<std::string, const Extension*> Extensions() const;
	};
}