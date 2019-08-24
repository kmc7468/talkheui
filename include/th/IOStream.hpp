#pragma once

#ifdef TH_USE_MULTIPRECISION
#	include <boost/multiprecision/cpp_int.hpp>
#endif

namespace th {
	void WriteStdout(long long integer) noexcept;
#ifdef TH_USE_MULTIPRECISION
	void WriteStdout(boost::multiprecision::int128_t integer) noexcept;
#endif
	void WriteStdout(char32_t character) noexcept;
	
	long long ReadIntegerStdin() noexcept;
#ifdef TH_USE_MULTIPRECISION
	boost::multiprecision::int128_t ReadInteger128Stdin() noexcept;
#endif
	long long ReadCharacterStdin() noexcept;
}