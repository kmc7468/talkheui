#pragma once

#include <cstddef>
#include <string>

namespace th {
	class Memory {
	private:
		std::string m_Name;

	public:
		virtual ~Memory() = default;

	protected:
		Memory(std::string name) noexcept;
		Memory(const Memory& memory);
		Memory(Memory&& memory) noexcept;

	protected:
		Memory& operator=(const Memory& memory);
		Memory& operator=(Memory&& memory) noexcept;

	public:
		virtual std::size_t Count() const noexcept = 0;
		virtual std::size_t Bytes() const noexcept = 0;

	public:
		std::string Name() const;
	};
}