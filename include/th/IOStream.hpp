#pragma once

namespace th {
	void WriteStdout(long long integer) noexcept;
	void WriteStdout(char32_t character) noexcept;
	
	long long ReadIntegerStdin() noexcept;
	char32_t ReadCharacterStdin() noexcept;
}