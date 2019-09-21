#pragma once

#include <th/uh/Program.hpp>

#include <functional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace th::uh {
	enum class FunctionType {
		Script,
		Native,
	};
	
	class Object;
	using NFResult = Object;
	using NFParameter = const std::vector<Object>&;
	using NativeFunction = std::function<NFResult(NFParameter)>;

	class Closure final {
	private:
		std::variant<std::vector<Command>, NativeFunction> m_Function;
		std::vector<int> m_Recursives;

	public:
		Closure(std::vector<Command> scriptFunction) noexcept;
		Closure(std::vector<Command> scriptFunction, std::vector<int> recursives) noexcept;
		Closure(NativeFunction nativeFunction);
		Closure(NativeFunction nativeFunction, std::vector<int> recursives);
		Closure(const Closure& closure);
		Closure(Closure&& closure);
		~Closure() = default;

	public:
		Closure& operator=(const Closure& closure);
		Closure& operator=(Closure&& closure);

	public:
		FunctionType Type() const noexcept;
		bool HasScriptFunction() const noexcept;
		bool HasNativeFunction() const noexcept;
		const std::vector<int>& GetRecursives() const noexcept;
		void SetRecursives(std::vector<int> newRecursives) noexcept;

		const std::vector<Command>& GetAsScriptFunction() const noexcept;
		const NativeFunction& GetAsNativeFunction() const noexcept;
	};
	
	enum class ObjectType {
		Nil,
		Number,
		Boolean,
		String,
		List,
		Closure,
		IO,
	};

	class Object final {
	private:
		std::variant<std::monostate, double, bool, std::string, std::vector<Object>> m_Data;

	public:
		Object() noexcept = default;
		Object(double number) noexcept;
		Object(bool boolean) noexcept;
		Object(std::string string) noexcept;
		Object(std::vector<Object> list) noexcept;
		Object(const Object& object);
		Object(Object&& object) noexcept;
		~Object() = default;

	public:
		Object& operator=(double number) noexcept;
		Object& operator=(bool boolean) noexcept;
		Object& operator=(std::string string) noexcept;
		Object& operator=(std::vector<Object> list) noexcept;
		Object& operator=(const Object& object);
		Object& operator=(Object&& object) noexcept;

	public:
		ObjectType Type() const noexcept;
		bool IsNil() const noexcept;
		bool IsNumber() const noexcept;
		bool IsBoolean() const noexcept;
		bool IsString() const noexcept;
		bool IsList() const noexcept;
		bool IsClosure() const noexcept;
		bool IsIO() const noexcept;

		double GetAsNumber() const noexcept;
		bool GetAsBoolean() const noexcept;
		const std::string& GetAsString() const noexcept;
		const std::vector<Object>& GetAsList() const noexcept;
	};
}