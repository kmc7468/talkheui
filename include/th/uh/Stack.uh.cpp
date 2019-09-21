#include <th/uh/Stack.hpp>

#include <utility>

namespace th::uh {
	Stack::Stack()
		: Memory("Stack") {
	}
	Stack::Stack(const Stack& stack)
		: Memory(stack), m_Data(stack.m_Data) {
	}
	Stack::Stack(Stack&& stack) noexcept
		: Memory(std::move(stack)), m_Data(std::move(stack.m_Data)) {
	}

	Stack& Stack::operator=(const Stack& stack) {
		Memory::operator=(stack);

		m_Data = stack.m_Data;
		return *this;
	}
	Stack& Stack::operator=(Stack&& stack) noexcept {
		Memory::operator=(std::move(stack));

		m_Data = std::move(stack.m_Data);
		return *this;
	}

	std::size_t Stack::Count() const noexcept {
		return m_Data.size();
	}
	std::size_t Stack::Bytes() const noexcept {
		return m_Data.size() * sizeof(Object);
	}
	Object Stack::Pop() {
		const Object result = std::move(m_Data.back());
		return m_Data.pop_back(), result;
	}
	void Stack::Push(Object object) {
		m_Data.push_back(std::move(object));
	}
}