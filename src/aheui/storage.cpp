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
	void stack::copy()
	{
		data_.push_back(data_.back());
	}
	void stack::swap() noexcept
	{
		std::iter_swap(data_.end() - 1, data_.end() - 2);
	}
}

namespace talkheui::aheui
{
	queue::queue()
		: storage("Queue")
	{}
	queue::queue(const queue& queue)
		: storage(queue), data_(queue.data_)
	{}
	queue::queue(queue&& queue) noexcept
		: storage(std::move(queue)), data_(std::move(queue.data_))
	{}

	queue& queue::operator=(const queue& queue)
	{
		storage::operator=(queue);
		data_ = queue.data_;
		return *this;
	}
	queue& queue::operator=(queue&& queue) noexcept
	{
		storage::operator=(std::move(queue));
		data_ = std::move(queue.data_);
		return *this;
	}

	std::size_t queue::bytes() const noexcept
	{
		return data_.size() * sizeof(long long);
	}
	std::size_t queue::size() const noexcept
	{
		return data_.size();
	}
	void queue::push(long long value)
	{
		data_.push_back(value);
	}
	long long queue::pop()
	{
		const long long result = data_.front();
		return data_.pop_front(), result;
	}
	void queue::copy()
	{
		data_.push_front(data_.front());
	}
	void queue::swap() noexcept
	{
		std::iter_swap(data_.begin(), data_.begin() + 1);
	}
}

namespace talkheui::aheui
{
	pipe::pipe(extension* extension)
		: storage("Pipe"), extension_(extension)
	{}
	pipe::pipe(pipe&& pipe) noexcept
		: storage(std::move(pipe)), extension_(pipe.extension_), recent_pushed_(pipe.recent_pushed_), push_count_(pipe.push_count_)
	{
		pipe.extension_ = nullptr;
	}

	pipe& pipe::operator=(pipe&& pipe) noexcept
	{
		storage::operator=(std::move(pipe));
		extension_ = pipe.extension_;
		recent_pushed_ = pipe.recent_pushed_;
		push_count_ = pipe.push_count_;

		pipe.extension_ = nullptr;
		pipe.recent_pushed_ = 0;
		pipe.push_count_ = 0;

		return *this;
	}

	std::size_t pipe::bytes() const noexcept
	{
		return push_count_ * sizeof(long long);
	}
	std::size_t pipe::size() const noexcept
	{
		return push_count_;
	}
	void pipe::push(long long value)
	{
		extension_->send(value);
		recent_pushed_ = value;
		++push_count_;
	}
	long long pipe::pop()
	{
		return extension_->receive();
	}
	void pipe::copy()
	{
		push(recent_pushed_);
	}
	void pipe::swap() noexcept
	{}
}