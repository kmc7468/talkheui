#pragma once

#include <th/Interpreter.hpp>
#include <th/uh/Object.hpp>
#include <th/uh/Program.hpp>

#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

namespace th::uh {
	class Interpreter;

	class RuntimeState final : public th::RuntimeState {
		friend class uh::Interpreter;

	public:
		int Word = -1;
		std::vector<std::pair<Closure, std::vector<Object>>> CallStack;

	public:
		virtual ~RuntimeState() override = default;

	private:
		RuntimeState();

	public:
		RuntimeState& operator=(const RuntimeState&) = delete;

	public:
		virtual void Reset() override;
		virtual void ResetStep() override;
	};

	class Interpreter final : public th::Interpreter {
	private:
		Program m_Script;
		std::optional<Object> m_Result;

	public:
		Interpreter();
		Interpreter(Interpreter&& interpreter) noexcept;
		virtual ~Interpreter() override = default;

	public:
		Interpreter& operator=(Interpreter&& interpreter) noexcept;

	public:
		virtual bool IsScriptLoaded() const override;
		virtual void UnloadScript() override;
		virtual void RunScript() override;
		virtual void RunScriptStep() override;

		Object Result() const;
		bool HasResult() const noexcept;

	protected:
		virtual void vReset() override;

		virtual void vLoadScript(const std::string_view& script) override;
	};
}