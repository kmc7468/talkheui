#include <th/ah/Storage.hpp>

#include <th/ah/Extension.hpp>

#include <utility>

namespace th::ah {
	Storage::Storage(std::string name) noexcept
		: Memory(std::move(name)) {
	}
	Storage::Storage(const Storage& storage)
		: Memory(storage) {
	}
	Storage::Storage(Storage&& storage) noexcept
		: Memory(std::move(storage)) {
	}

	Storage& Storage::operator=(const Storage& storage) {
		return Memory::operator=(storage), *this;
	}
	Storage& Storage::operator=(Storage&& storage) noexcept {
		return Memory::operator=(std::move(storage)), *this;
	}
}

namespace th::ah {
	Stack::Stack()
		: Storage("Stack") {
	}
	Stack::Stack(const Stack& stack)
		: Storage(stack), m_Data(stack.m_Data) {
	}
	Stack::Stack(Stack&& stack) noexcept
		: Storage(std::move(stack)), m_Data(std::move(stack.m_Data)) {
	}

	Stack& Stack::operator=(const Stack& stack) {
		Storage::operator=(stack);

		m_Data = stack.m_Data;
		return *this;
	}
	Stack& Stack::operator=(Stack&& stack) noexcept {
		Storage::operator=(std::move(stack));

		m_Data = std::move(stack.m_Data);
		return *this;
	}

	std::size_t Stack::Count() const noexcept {
		return m_Data.size();
	}
	std::size_t Stack::Bytes() const noexcept {
		return Count() * sizeof(detail::Value);
	}
	void Stack::Push(detail::Value value) {
		m_Data.push_back(value);
	}
	detail::Value Stack::Pop() {
		const detail::Value result = m_Data.back();
		return m_Data.pop_back(), result;
	}
	void Stack::Copy() {
		m_Data.push_back(m_Data.back());
	}
	void Stack::Swap() noexcept {
		std::iter_swap(m_Data.end() - 1, m_Data.end() - 2);
	}
}

namespace th::ah {
	Queue::Queue()
		: Storage("Queue") {
	}
	Queue::Queue(const Queue& queue)
		: Storage(queue), m_Data(queue.m_Data) {
	}
	Queue::Queue(Queue&& queue) noexcept
		: Storage(std::move(queue)), m_Data(std::move(queue.m_Data)) {
	}

	Queue& Queue::operator=(const Queue& queue) {
		Storage::operator=(queue);

		m_Data = queue.m_Data;
		return *this;
	}
	Queue& Queue::operator=(Queue&& queue) noexcept {
		Storage::operator=(std::move(queue));

		m_Data = std::move(queue.m_Data);
		return *this;
	}

	std::size_t Queue::Count() const noexcept {
		return m_Data.size();
	}
	std::size_t Queue::Bytes() const noexcept {
		return Count() * sizeof(detail::Value);
	}
	void Queue::Push(detail::Value value) {
		m_Data.push_back(value);
	}
	detail::Value Queue::Pop() {
		const detail::Value result = m_Data.front();
		return m_Data.pop_front(), result;
	}
	void Queue::Copy() {
		m_Data.push_front(m_Data.front());
	}
	void Queue::Swap() noexcept {
		std::iter_swap(m_Data.begin(), m_Data.begin() + 1);
	}
}

namespace th::ah {
	Pipe::Pipe(Extension* extension)
		: Storage("Pipe"), m_Extension(extension)
	{}
	Pipe::Pipe(Pipe&& pipe) noexcept
		: Storage(std::move(pipe)), m_Extension(pipe.m_Extension), m_RecentPushed(pipe.m_RecentPushed) {
		pipe.m_Extension = nullptr;
	}

	Pipe& Pipe::operator=(Pipe&& pipe) noexcept {
		Storage::operator=(std::move(pipe));
		m_Extension = pipe.m_Extension;
		m_RecentPushed = pipe.m_RecentPushed;

		pipe.m_Extension = nullptr;
		pipe.m_RecentPushed = 0;

		return *this;
	}

	std::size_t Pipe::Count() const noexcept {
		return static_cast<std::size_t>(m_Extension->Count());
	}
	std::size_t Pipe::Bytes() const noexcept {
		return static_cast<std::size_t>(m_Extension->Bytes());
	}
	void Pipe::Push(detail::Value value) {
		m_Extension->Send(value);
		m_RecentPushed = value;
	}
	detail::Value Pipe::Pop() {
		return m_Extension->Receive();
	}
	void Pipe::Copy() {
		Push(m_RecentPushed);
	}
	void Pipe::Swap() noexcept {
	}
}