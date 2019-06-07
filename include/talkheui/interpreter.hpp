#pragma once

#include <string>

namespace talkheui
{
	class interpreter
	{
	public:
		virtual ~interpreter() = default;

	protected:
		interpreter(std::string name) noexcept;
		interpreter(const interpreter& interpreter);
		interpreter(interpreter&& interpreter) noexcept;

	protected:
		interpreter& operator=(const interpreter& interpreter);
		interpreter& operator=(interpreter&& interpreter) noexcept;

	public:
		std::string name() const;

	private:
		std::string name_;
	};
}