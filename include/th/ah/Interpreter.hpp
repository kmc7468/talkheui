#pragma once

#include <th/interpreter.hpp>
#include <th/ah/CodePlane.hpp>
#include <th/ah/detail/Type.hpp>

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

namespace th::ah {
	class Extension;
	class Interpreter;

	class RuntimeState final : public th::RuntimeState
	{
		friend class ah::Interpreter;

	public:
		int X = 0, Y = -1;
		int DX = 0, DY = 1;
		std::size_t SelectedStorage = 0;

	private:
		ah::Extension* m_Extension = nullptr;

	public:
		virtual ~RuntimeState() override = default;

	private:
		RuntimeState();
		
	public:
		RuntimeState& operator=(const RuntimeState&) = delete;

	public:
		virtual void Reset() override;
		virtual void ResetStep() override;

	public:
		ah::Extension* GetConstructedExtension() const noexcept;
		void ConstructPipe(ah::Extension* extension);
	};

	class Interpreter final : public th::Interpreter {
	private:
		CodePlane m_Script;
		std::optional<detail::Value> m_Result;

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

		void ConstructPipe(const std::string& path);
		void DeconstructPipe();

	protected:
		virtual void vReset() override;

		virtual void vLoadScript(const std::string_view& script) override;

	private:
		void MoveCursor();
		void ReverseCursor();
		void UpdateCursor(char32_t jungsung);

	public:
		const CodePlane& Script() const noexcept;
		detail::Value Result() const noexcept;
		bool HasResult() const noexcept;
	};
}