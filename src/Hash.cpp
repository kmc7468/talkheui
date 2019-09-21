#include <th/Hash.hpp>

namespace th {
	std::uint32_t Hash(const std::string_view& string) noexcept {
		return detail::CRC32(string.data(), string.size() - 1) ^ 0xFFFFFFFF;
	}
}