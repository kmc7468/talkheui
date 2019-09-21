#include <th/uh/Object.hpp>

#include <utility>

namespace th::uh {
	Closure::Closure(std::vector<Command> scriptFunction) noexcept
		: m_Function(std::move(scriptFunction)), m_Recursives({ 0 }) {
	}
	Closure::Closure(std::vector<Command> scriptFunction, std::vector<int> recursives) noexcept
		: m_Function(std::move(scriptFunction)), m_Recursives(std::move(recursives)) {
	}
	Closure::Closure(NativeFunction nativeFunction)
		: m_Function(std::move(nativeFunction)), m_Recursives({ 0 }) {
	}
	Closure::Closure(NativeFunction nativeFunction, std::vector<int> recursives)
		: m_Function(std::move(nativeFunction)), m_Recursives(std::move(recursives)) {
	}
	Closure::Closure(const Closure& closure)
		: m_Function(closure.m_Function), m_Recursives(closure.m_Recursives) {
	}
	Closure::Closure(Closure&& closure)
		: m_Function(std::move(closure.m_Function)), m_Recursives(std::move(closure.m_Recursives)) {
	}

	Closure& Closure::operator=(const Closure& closure) {
		m_Function = closure.m_Function;
		m_Recursives = closure.m_Recursives;
		return *this;
	}
	Closure& Closure::operator=(Closure&& closure) {
		m_Function = std::move(closure.m_Function);
		m_Recursives = std::move(closure.m_Recursives);
		return *this;
	}

	FunctionType Closure::Type() const noexcept {
		return static_cast<FunctionType>(m_Function.index());
	}
	bool Closure::HasScriptFunction() const noexcept {
		return Type() == FunctionType::Script;
	}
	bool Closure::HasNativeFunction() const noexcept {
		return Type() == FunctionType::Native;
	}
	const std::vector<int>& Closure::GetRecursives() const noexcept {
		return m_Recursives;
	}
	void Closure::SetRecursives(std::vector<int> newRecursives) noexcept {
		m_Recursives = std::move(newRecursives);
	}

	const std::vector<Command>& Closure::GetAsScriptFunction() const noexcept {
		return std::get<std::vector<Command>>(m_Function);
	}
	const NativeFunction& Closure::GetAsNativeFunction() const noexcept {
		return std::get<NativeFunction>(m_Function);
	}
}

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