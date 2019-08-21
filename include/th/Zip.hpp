#pragma once

#include <memory>
#include <miniz/miniz.h>
#include <string>

namespace talkheui
{
	class zip_reader_entry final
	{
		friend class zip_reader;

	public:
		zip_reader_entry(const zip_reader_entry& entry) noexcept;
		~zip_reader_entry() = default;

	private:
		zip_reader_entry(std::weak_ptr<mz_zip_archive> archive, std::size_t index) noexcept;

	public:
		zip_reader_entry& operator=(const zip_reader_entry& entry) noexcept;

	public:
		bool is_directory() const noexcept;
		bool is_file() const noexcept;
		bool is_encrypted() const noexcept;
		void* extract(std::size_t* size) const;
		std::string filename() const;

	public:
		std::size_t index() const noexcept;

	private:
		std::weak_ptr<mz_zip_archive> archive_;
		std::size_t index_;
	};

	class zip_reader final
	{
	public:
		zip_reader() = default;
		zip_reader(const std::string& path);
		zip_reader(zip_reader&& reader) noexcept;
		~zip_reader();

	public:
		zip_reader& operator=(zip_reader&& reader) noexcept;
		zip_reader_entry operator[](std::size_t index) const noexcept;

	public:
		void open(const std::string& path);
		void close() noexcept;
		
		zip_reader_entry find(const std::string& name) const;
		zip_reader_entry at(std::size_t index) const;
		std::size_t size() const noexcept;

	public:
		bool is_open() const noexcept;

	private:
		std::shared_ptr<mz_zip_archive> archive_;
	};
}