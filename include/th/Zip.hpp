#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include <miniz/miniz.h>

namespace th {
	class ZipReaderEntry final {
	private:
		std::weak_ptr<mz_zip_archive> m_Archive;
		std::size_t m_Index;

	public:
		ZipReaderEntry(std::weak_ptr<mz_zip_archive> archive, std::size_t index) noexcept;
		ZipReaderEntry(const ZipReaderEntry& entry) noexcept;
		~ZipReaderEntry() = default;

	public:
		ZipReaderEntry& operator=(const ZipReaderEntry& entry) noexcept;

	public:
		bool IsDirectory() const noexcept;
		bool IsFile() const noexcept;
		bool IsEncrypted() const noexcept;
		std::string Filename() const;

		void* Extract(std::size_t* size) const;

	public:
		std::size_t Index() const noexcept;
	};

	class ZipReader final {
	private:
		std::shared_ptr<mz_zip_archive> m_Archive;

	public:
		ZipReader() = default;
		ZipReader(const std::string& path);
		ZipReader(ZipReader&& zip) noexcept;
		~ZipReader();

	public:
		ZipReader& operator=(ZipReader&& zip) noexcept;
		ZipReaderEntry operator[](std::size_t index) const noexcept;

	public:
		void Open(const std::string& path);
		void Close() noexcept;
		bool IsOpen() const noexcept;
		std::size_t Count() const noexcept;

		ZipReaderEntry Find(const std::string& filename) const;
	};
}