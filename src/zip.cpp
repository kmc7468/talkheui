#include <talkheui/zip.hpp>

#include <cassert>
#include <cstring>
#include <stdexcept>

namespace talkheui
{
	zip_reader_entry::zip_reader_entry(const zip_reader_entry& entry) noexcept
		: archive_(entry.archive_), index_(entry.index_)
	{}

	zip_reader_entry::zip_reader_entry(std::weak_ptr<mz_zip_archive> archive, std::size_t index) noexcept
		: archive_(archive), index_(index)
	{}

	zip_reader_entry& zip_reader_entry::operator=(const zip_reader_entry& entry) noexcept
	{
		archive_ = entry.archive_;
		index_ = entry.index_;
		return *this;
	}

	bool zip_reader_entry::is_directory() const noexcept
	{
		return mz_zip_reader_is_file_a_directory(archive_.lock().get(), index_);
	}
	bool zip_reader_entry::is_file() const noexcept
	{
		return !is_directory();
	}
	bool zip_reader_entry::is_encrypted() const noexcept
	{
		return mz_zip_reader_is_file_encrypted(archive_.lock().get(), index_);
	}
	void* zip_reader_entry::extract(std::size_t* size) const
	{
		std::size_t size_internal;
		void* result = mz_zip_reader_extract_to_heap(archive_.lock().get(), index_, &size_internal, 0);

		if (!result) throw std::runtime_error("failed to extract the zip entry");
		if (size)* size = size_internal;

		return result;
	}
	std::string zip_reader_entry::filename() const
	{
		std::shared_ptr<mz_zip_archive> archive_lock = archive_.lock();
		const std::size_t size = mz_zip_reader_get_filename(archive_lock.get(), index_, nullptr, 0) - 1;

		std::string result(size, 0);
		mz_zip_reader_get_filename(archive_lock.get(), index_, result.data(), size + 1);

		return result;
	}

	std::size_t talkheui::zip_reader_entry::index() const noexcept
	{
		return index_;
	}
}

namespace talkheui
{
	zip_reader::zip_reader(const std::string& path)
	{
		open(path);
	}
	zip_reader::zip_reader(zip_reader&& reader) noexcept
		: archive_(std::move(reader.archive_))
	{}
	zip_reader::~zip_reader()
	{
		close();
	}

	zip_reader& zip_reader::operator=(zip_reader&& reader) noexcept
	{
		close();

		archive_ = std::move(reader.archive_);
		return *this;
	}
	zip_reader_entry zip_reader::operator[](std::size_t index) noexcept
	{
		assert(index < size());
		return zip_reader_entry(archive_, index);
	}

	void zip_reader::open(const std::string& path)
	{
		close();

		archive_ = std::make_shared<mz_zip_archive>();
		if (!mz_zip_reader_init_file(archive_.get(), path.c_str(), 0)) throw std::runtime_error("failed to open the zip file");
	}
	void zip_reader::close() noexcept
	{
		if (!archive_) return;

		mz_zip_reader_end(archive_.get());
		archive_.reset();
	}

	zip_reader_entry zip_reader::at(std::size_t index)
	{
		if (index >= size()) throw std::out_of_range("the argument 'index' is out of range");
		return zip_reader_entry(archive_, index);
	}
	std::size_t zip_reader::size() const noexcept
	{
		return static_cast<std::size_t>(archive_->m_total_files);
	}

	bool zip_reader::is_open() const noexcept
	{
		return archive_ != nullptr;
	}
}