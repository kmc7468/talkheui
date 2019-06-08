#include <talkheui/zip.hpp>

#include <cstring>
#include <stdexcept>

namespace talkheui
{
	zip_reader::zip_reader()
	{
		std::memset(&archive_, 0, sizeof(mz_zip_archive));
	}
	zip_reader::zip_reader(const std::string& path)
	{
		open(path);
	}
	zip_reader::zip_reader(zip_reader&& reader) noexcept
		: archive_(reader.archive_), is_open_(reader.is_open_)
	{}
	zip_reader::~zip_reader()
	{
		close();
	}

	zip_reader& zip_reader::operator=(zip_reader&& reader) noexcept
	{
		close();

		archive_ = reader.archive_;
		is_open_ = reader.is_open_;
		std::memset(&reader.archive_, 0, sizeof(mz_zip_archive));
		is_open_ = false;

		return *this;
	}

	void zip_reader::open(const std::string& path)
	{
		close();

		if (!mz_zip_reader_init_file(&archive_, path.c_str(), 0)) throw std::runtime_error("failed to open the zip file");
	}
	void zip_reader::close() noexcept
	{
		if (!is_open_) return;

		mz_zip_reader_end(&archive_);
		std::memset(&archive_, 0, sizeof(mz_zip_archive));
		is_open_ = false;
	}

	bool zip_reader::is_open() const noexcept
	{
		return is_open_;
	}
}