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
	Int128& Int128::operator+=(const Int128& integer) noexcept {
		const std::uint64_t newLowQword = m_Low + integer.m_Low;
		m_High += integer.m_High + (newLowQword < m_Low);
		m_Low = newLowQword;

		return *this;
	}
	Int128& Int128::operator-=(const Int128& integer) noexcept {
		m_Low -= integer.m_Low;
		m_High -= integer.m_High + (m_Low < integer.m_Low);
		
		return *this;
	}
	Int128& Int128::operator*=(const Int128& integer) noexcept {
		if (m_High == 0 && integer.m_High == 0) {
			m_High = MultiplyUInt64(&m_Low, &integer.m_Low);
		} else {
			std::uint64_t abLowLowLowQword = m_Low;
			std::uint64_t abLowLowHighQword = MultiplyUInt64(&abLowLowLowQword, &integer.m_Low);
			std::uint64_t abLowHighLowQword = m_Low;
			MultiplyUInt64(&abLowHighLowQword, reinterpret_cast<const std::uint64_t*>(&integer.m_High));
			std::uint64_t abHighLowLowQword = m_High;
			MultiplyUInt64(&abHighLowLowQword, reinterpret_cast<const std::uint64_t*>(&integer.m_Low));

			m_Low = abLowLowLowQword;
			m_High = abLowLowHighQword + abLowHighLowQword + abHighLowLowQword;
		}
		
		return *this;
	}

	std::uint64_t Int128::GetLowQword() const noexcept {
		return m_Low;
	}
	std::int64_t Int128::GetHighQword() const noexcept {
		return m_High;
	}

	std::uint64_t Int128::MultiplyUInt64(std::uint64_t* a, const std::uint64_t* b) noexcept {
		const std::uint64_t aLowQword = *a & 0xFFFFFFFF;
		const std::uint64_t aHighQword = *a >> 32;
		const std::uint64_t bLowQword = *b & 0xFFFFFFFF;
		const std::uint64_t bHighQword = *b >> 32;

		const std::uint64_t abLowLow = aLowQword * bLowQword;
		const std::uint64_t abLowHigh = aLowQword * bHighQword;
		const std::uint64_t abHighLow = aHighQword * bLowQword;

		*a = abLowLow + (abLowHigh << 32) + (abHighLow << 32);
		return (aHighQword * bHighQword) + (abLowHigh >> 32) + (abHighLow >> 32) +
			   static_cast<std::uint32_t>((abLowLow >> 32) + static_cast<std::uint32_t>(abLowHigh) + static_cast<std::uint32_t>(abHighLow) >> 32);
	}
}