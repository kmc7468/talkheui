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

	class IO final {
	private:
		std::vector<Object> m_Objects;

	public:
		IO() noexcept = default;
		IO(Object object) noexcept;
		IO(std::vector<Object> objects) noexcept;
		IO(const IO& io);
		IO(IO&& io) noexcept;
		~IO() = default;

	public:
		IO& operator=(const IO& io);
		IO& operator=(IO&& io) noexcept;

	public:
		void AddObject(Object object);
		const std::vector<Object>& GetObjects() const noexcept;
	};

	class Object final {
	private:
		std::variant<std::monostate, double, bool, std::string, std::vector<Object>, Closure, IO> m_Data;

	public:
		Object() noexcept = default;
		Object(double number) noexcept;
		Object(bool boolean) noexcept;
		Object(std::string string) noexcept;
		Object(std::vector<Object> list) noexcept;
		Object(Closure closure);
		Object(IO io) noexcept;
		Object(const Object& object);
		Object(Object&& object) noexcept;
		~Object() = default;

	public:
		Object& operator=(double number) noexcept;
		Object& operator=(bool boolean) noexcept;
		Object& operator=(std::string string) noexcept;
		Object& operator=(std::vector<Object> list) noexcept;
		Object& operator=(Closure closure);
		Object& operator=(IO io) noexcept;
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
		const Closure& GetAsClosure() const noexcept;
		const IO& GetAsIO() const noexcept;
	};
}