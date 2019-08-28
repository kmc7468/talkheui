#pragma once

#include <cstdint>

namespace th {
	class Int128 final {
	private:
		std::uint64_t m_Low = 0;
		std::int64_t m_High = 0;

	public:
		Int128() noexcept = default;
		Int128(std::int8_t integer) noexcept;
		Int128(std::uint8_t integer) noexcept;
		Int128(std::int16_t integer) noexcept;
		Int128(std::uint16_t integer) noexcept;
		Int128(std::int32_t integer) noexcept;
		Int128(std::uint32_t integer) noexcept;
		Int128(std::int64_t integer) noexcept;
		Int128(std::uint64_t integer) noexcept;
		Int128(std::int64_t high, std::uint64_t low) noexcept;
		Int128(const Int128& integer) noexcept;
		~Int128() = default;

	public:
		Int128& operator=(const Int128& integer) noexcept;
		bool operator==(const Int128& integer) const noexcept;
		bool operator!=(const Int128& integer) const noexcept;
		bool operator>(const Int128& integer) const noexcept;
		bool operator>=(const Int128& integer) const noexcept;
		bool operator<(const Int128& integer) const noexcept;
		bool operator<=(const Int128& integer) const noexcept;
		Int128& operator+=(const Int128& integer) noexcept;
		Int128& operator-=(const Int128& integer) noexcept;
		Int128& operator*=(const Int128& integer) noexcept;
		Int128& operator/=(const Int128& integer) noexcept;
		Int128& operator%=(const Int128& integer) noexcept;

	public:
		std::uint64_t GetLowQword() const noexcept;
		std::int64_t GetHighQword() const noexcept;

	private:
		static std::uint64_t MultiplyUInt64(std::uint64_t* a, const std::uint64_t* b) noexcept;
	};
}