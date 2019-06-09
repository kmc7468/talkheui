#pragma once

#include <talkheui/memory.hpp>

#include <deque>

namespace talkheui::aheui
{
	class storage : public memory
	{
	public:
		virtual ~storage() override = default;

	protected:
		storage(std::string name) noexcept;
		storage(const storage& storage);
		storage(storage&& storage) noexcept;

	protected:
		storage& operator=(const storage& storage);
		storage& operator=(storage&& storage) noexcept;

	public:
		virtual void push(long long value) = 0;
		virtual long long pop() = 0;
		virtual void copy() = 0;
		virtual void swap() noexcept = 0;
	};

	class stack final : public storage
	{
	public:
		stack();
		stack(const stack& stack);
		stack(stack&& stack) noexcept;
		virtual ~stack() override = default;

	public:
		stack& operator=(const stack& stack);
		stack& operator=(stack&& stack) noexcept;

	public:
		virtual std::size_t bytes() const noexcept override;
		virtual std::size_t size() const noexcept override;
		virtual void push(long long value) override;
		virtual long long pop() override;
		virtual void copy() override;
		virtual void swap() noexcept override;

	private:
		std::deque<long long> data_;
	};

	class queue final : public storage
	{
	public:
		queue();
		queue(const queue& queue);
		queue(queue&& queue) noexcept;
		virtual ~queue() override = default;

	public:
		queue& operator=(const queue& queue);
		queue& operator=(queue&& queue) noexcept;

	public:
		virtual std::size_t bytes() const noexcept override;
		virtual std::size_t size() const noexcept override;
		virtual void push(long long value) override;
		virtual long long pop() override;
		virtual void copy() override;
		virtual void swap() noexcept override;

	private:
		std::deque<long long> data_;
	};

	class extension;

	class pipe final : public storage
	{
	public:
		pipe(extension* extension);
		pipe(pipe&& pipe) noexcept;
		virtual ~pipe() override = default;

	public:
		pipe& operator=(pipe&& pipe) noexcept;

	public:
		virtual std::size_t bytes() const noexcept override;
		virtual std::size_t size() const noexcept override;
		virtual void push(long long value) override;
		virtual long long pop() override;
		virtual void copy() override;
		virtual void swap() noexcept override;

	private:
		extension* extension_;
		long long recent_pushed_ = 0;
		std::size_t push_count_ = 0;
	};
}