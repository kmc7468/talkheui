#include <th/Int128.hpp>

namespace th {
	Int128::Int128(std::int8_t integer) noexcept
		: m_Low(integer), m_High(integer >= 0 ? 0 : -1) {
	}
	Int128::Int128(std::uint8_t integer) noexcept
		: m_Low(integer) {
	}
	Int128::Int128(std::int16_t integer) noexcept
		: m_Low(integer), m_High(integer >= 0 ? 0 : -1) {
	}
	Int128::Int128(std::uint16_t integer) noexcept
		: m_Low(integer) {
	}
	Int128::Int128(std::int32_t integer) noexcept
		: m_Low(integer), m_High(integer >= 0 ? 0 : -1) {
	}
	Int128::Int128(std::uint32_t integer) noexcept
		: m_Low(integer) {
	}
	Int128::Int128(std::int64_t integer) noexcept
		: m_Low(integer), m_High(integer >= 0 ? 0 : -1) {
	}
	Int128::Int128(std::uint64_t integer) noexcept
		: m_Low(integer) {
	}
	Int128::Int128(std::int64_t high, std::uint64_t low) noexcept
		: m_Low(low), m_High(high) {
	}
	Int128::Int128(const Int128& integer) noexcept
		: m_Low(integer.m_Low), m_High(integer.m_High) {
	}

	Int128& Int128::operator=(const Int128& integer) noexcept {
		m_Low = integer.m_Low;
		m_High = integer.m_High;
		return *this;
	}
	bool Int128::operator==(const Int128& integer) const noexcept {
		return m_Low == integer.m_Low && m_High == integer.m_High;
	}
	bool Int128::operator!=(const Int128& integer) const noexcept {
		return m_Low != integer.m_Low || m_High != integer.m_High;
	}
	bool Int128::operator>(const Int128& integer) const noexcept {
		if (m_High > integer.m_High) return true;
		else if (m_High < integer.m_High ||
				 m_Low <= integer.m_Low) return false;
		else return true;
	}
	bool Int128::operator>=(const Int128& integer) const noexcept {
		if (m_High > integer.m_High) return true;
		else if (m_High < integer.m_High ||
				 m_Low < integer.m_Low) return false;
		else return true;
	}
	bool Int128::operator<(const Int128& integer) const noexcept {
		if (m_High < integer.m_High) return true;
		else if (m_High > integer.m_High ||
				 m_Low >= integer.m_Low) return false;
		else return true;
	}
	bool Int128::operator<=(const Int128& integer) const noexcept {
		if (m_High < integer.m_High) return true;
		else if (m_High > integer.m_High ||
				 m_Low > integer.m_Low) return false;
		else return true;
	}

	std::uint64_t Int128::GetLowQword() const noexcept {
		return m_Low;
	}
	std::int64_t Int128::GetHighQword() const noexcept {
		return m_High;
	}
}