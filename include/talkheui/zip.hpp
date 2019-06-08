#pragma once

#include <miniz/miniz.h>
#include <string>

namespace talkheui
{
	class zip_reader final
	{
	public:
		zip_reader();
		zip_reader(const std::string& path);
		zip_reader(zip_reader&& reader) noexcept;
		~zip_reader();

	public:
		zip_reader& operator=(zip_reader&& reader) noexcept;

	public:
		void open(const std::string& path);
		void close() noexcept;

	public:
		bool is_open() const noexcept;

	private:
		mz_zip_archive archive_;
		bool is_open_ = false;
	};
}