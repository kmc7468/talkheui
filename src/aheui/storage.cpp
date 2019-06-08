#include <talkheui/aheui/storage.hpp>

#include <utility>

namespace talkheui::aheui
{
	storage::storage(std::string name) noexcept
		: memory(std::move(name))
	{}
	storage::storage(const storage& storage)
		: memory(storage)
	{}
	storage::storage(storage&& storage) noexcept
		: memory(std::move(storage))
	{}

	storage& storage::operator=(const storage& storage)
	{
		return memory::operator=(storage), *this;
	}
	storage& storage::operator=(storage&& storage) noexcept
	{
		return memory::operator=(std::move(storage)), *this;
	}
}

namespace talkheui::aheui
{
	stack::stack()
		: storage("Stack")
	{}
	stack::stack(const stack& stack)
		: storage(stack), data_(stack.data_)
	{}
	stack::stack(stack&& stack) noexcept
		: storage(std::move(stack)), data_(std::move(stack.data_))
	{}

	stack& stack::operator=(const stack& stack)
	{
		storage::operator=(stack);
		data_ = stack.data_;
		return *this;
	}
	stack& stack::operator=(stack&& stack) noexcept
	{
		storage::operator=(std::move(stack));
		data_ = std::move(stack.data_);
		return *this;
	}

	std::size_t stack::bytes() const noexcept
	{
		return data_.size() * sizeof(long long);
	}
	std::size_t stack::size() const noexcept
	{
		return data_.size();
	}
	void stack::push(long long value)
	{
		data_.push_back(value);
	}
	long long stack::pop()
	{
		const long long result = data_.back();
		return data_.pop_back(), result;
	}
	void stack::unpop(long long value)
	{
		data_.push_back(value);
	}
	void stack::copy()
	{
		data_.push_back(data_.back());
	}
	void stack::move(long long value)
	{
		data_.push_back(value);
	}
	void stack::swap() noexcept
	{
		std::iter_swap(data_.begin(), data_.end() - 1);
	}
}