#pragma once

namespace th {
	void WriteStdout(long long integer) noexcept;
	void WriteStdout(char32_t character) noexcept;
	
	long long ReadIntegerStdin() noexcept;
	long long ReadCharacterStdin() noexcept;
}