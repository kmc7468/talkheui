#pragma once

#include <string>
#include <variant>
#include <vector>

namespace th::uh {
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
	};
}