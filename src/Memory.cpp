#include <th/Memory.hpp>

#include <utility>

namespace th {
	Memory::Memory(std::string name) noexcept
		: m_Name(std::move(name)) {
	}
	Memory::Memory(const Memory& memory)
		: m_Name(memory.m_Name) {
	}
	Memory::Memory(Memory&& memory) noexcept
		: m_Name(std::move(memory.m_Name)) {
	}

	Memory& Memory::operator=(const Memory& memory) {
		m_Name = memory.m_Name;
		return *this;
	}
	Memory& Memory::operator=(Memory&& memory) noexcept {
		m_Name = std::move(memory.m_Name);
		return *this;
	}

	std::string Memory::Name() const {
		return m_Name;
	}
}