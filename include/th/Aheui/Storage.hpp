#pragma once

#include <th/Memory.hpp>

#include <deque>

namespace th::aheui {
	class Storage : public Memory {
	public:
		virtual ~Storage() override = default;

	protected:
		Storage(std::string name) noexcept;
		Storage(const Storage& storage);
		Storage(Storage&& storage) noexcept;

	protected:
		Storage& operator=(const Storage& storage);
		Storage& operator=(Storage&& storage) noexcept;

	public:
		virtual void Push(long long value) = 0;
		virtual long long Pop() = 0;
		virtual void Copy() = 0;
		virtual void Swap() noexcept = 0;
	};

	class Stack final : public Storage {
	private:
		std::deque<long long> m_Data;

	public:
		Stack();
		Stack(const Stack& stack);
		Stack(Stack&& stack) noexcept;
		virtual ~Stack() override = default;

	public:
		Stack& operator=(const Stack& stack);
		Stack& operator=(Stack&& stack) noexcept;

	public:
		virtual std::size_t Count() const noexcept override;
		virtual std::size_t Bytes() const noexcept override;
		virtual void Push(long long value) override;
		virtual long long Pop() override;
		virtual void Copy() override;
		virtual void Swap() noexcept override;
	};

	class Queue final : public Storage {
	private:
		std::deque<long long> m_Data;

	public:
		Queue();
		Queue(const Queue& queue);
		Queue(Queue&& queue) noexcept;
		virtual ~Queue() override = default;

	public:
		Queue& operator=(const Queue& queue);
		Queue& operator=(Queue&& queue) noexcept;

	public:
		virtual std::size_t Count() const noexcept override;
		virtual std::size_t Bytes() const noexcept override;
		virtual void Push(long long value) override;
		virtual long long Pop() override;
		virtual void Copy() override;
		virtual void Swap() noexcept override;
	};

	class Extension;

	class Pipe final : public Storage {
	private:
		Extension* m_Extension = nullptr;
		long long m_RecentPushed = 0;

	public:
		Pipe(Extension* extension);
		Pipe(Pipe&& pipe) noexcept;
		virtual ~Pipe() override = default;

	public:
		Pipe& operator=(Pipe&& pipe) noexcept;

	public:
		virtual std::size_t Count() const noexcept override;
		virtual std::size_t Bytes() const noexcept override;
		virtual void Push(long long value) override;
		virtual long long Pop() override;
		virtual void Copy() override;
		virtual void Swap() noexcept override;
	};
}