#pragma once

#ifdef TH_USE_MULTIPRECISION
#	include <boost/multiprecision/cpp_int.hpp>
#endif

namespace th::aheui::detail {
	using Value =
#ifdef TH_USE_MULTIPRECISION
		boost::multiprecision::int128_t
#else
		long long
#endif
	;
}