#pragma once

#include <th/Memory.hpp>
#include <th/uh/Object.hpp>

#include <deque>

namespace th::uh {
	class Stack final : public Memory {
	private:
		std::deque<Object> m_Data;
		
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
		Object Pop();
		void Push(Object object);
	};
}