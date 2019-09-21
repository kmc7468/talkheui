#include <th/uh/Object.hpp>

#include <utility>

namespace th::uh {
	Object::Object(double number) noexcept
		: m_Data(number) {
	}
	Object::Object(bool boolean) noexcept
		: m_Data(boolean) {
	}
	Object::Object(std::string string) noexcept
		: m_Data(std::move(string)) {
	}
	Object::Object(std::vector<Object> list) noexcept
		: m_Data(std::move(list)) {
	}
	Object::Object(const Object& object)
		: m_Data(object.m_Data) {
	}
	Object::Object(Object&& object) noexcept
		: m_Data(std::move(object.m_Data)) {
	}
	
	Object& Object::operator=(double number) noexcept {
		m_Data = number;
		return *this;
	}
	Object& Object::operator=(bool boolean) noexcept {
		m_Data = boolean;
		return *this;
	}
	Object& Object::operator=(std::string string) noexcept {
		m_Data = std::move(string);
		return *this;
	}
	Object& Object::operator=(std::vector<Object> list) noexcept {
		m_Data = std::move(list);
		return *this;
	}
	Object& Object::operator=(const Object& object) {
		m_Data = object.m_Data;
		return *this;
	}
	Object& Object::operator=(Object&& object) noexcept {
		m_Data = std::move(object.m_Data);
		return *this;
	}

	ObjectType Object::Type() const noexcept {
		return static_cast<ObjectType>(m_Data.index());
	}
	bool Object::IsNil() const noexcept {
		return Type() == ObjectType::Nil;
	}
	bool Object::IsNumber() const noexcept {
		return Type() == ObjectType::Number;
	}
	bool Object::IsBoolean() const noexcept {
		return Type() == ObjectType::Boolean;
	}
	bool Object::IsString() const noexcept {
		return Type() == ObjectType::String;
	}
	bool Object::IsList() const noexcept {
		return Type() == ObjectType::List;
	}
	bool Object::IsClosure() const noexcept {
		return Type() == ObjectType::Closure;
	}
	bool Object::IsIO() const noexcept {
		return Type() == ObjectType::IO;
	}

	double Object::GetAsNumber() const noexcept {
		return std::get<double>(m_Data);
	}
	bool Object::GetAsBoolean() const noexcept {
		return std::get<bool>(m_Data);
	}
	const std::string& Object::GetAsString() const noexcept {
		return std::get<std::string>(m_Data);
	}
	const std::vector<Object>& Object::GetAsList() const noexcept {
		return std::get<std::vector<Object>>(m_Data);
	}
}