#ifdef _MSC_VER
#	pragma warning(disable: 4267)
#endif

#include <th/Zip.hpp>

#include <cassert>
#include <cstring>
#include <stdexcept>

namespace th {
	ZipReaderEntry::ZipReaderEntry(std::weak_ptr<mz_zip_archive> archive, std::size_t index) noexcept
		: m_Archive(archive), m_Index(index) {
	}
	ZipReaderEntry::ZipReaderEntry(const ZipReaderEntry& entry) noexcept
		: m_Archive(entry.m_Archive), m_Index(entry.m_Index) {
	}

	ZipReaderEntry& ZipReaderEntry::operator=(const ZipReaderEntry& entry) noexcept {
		m_Archive = entry.m_Archive;
		m_Index = entry.m_Index;
		return *this;
	}

	bool ZipReaderEntry::IsDirectory() const noexcept {
		return mz_zip_reader_is_file_a_directory(m_Archive.lock().get(), m_Index);
	}
	bool ZipReaderEntry::IsFile() const noexcept {
		return !IsDirectory();
	}
	bool ZipReaderEntry::IsEncrypted() const noexcept {
		return mz_zip_reader_is_file_encrypted(m_Archive.lock().get(), m_Index);
	}
	std::string ZipReaderEntry::Filename() const {
		std::shared_ptr<mz_zip_archive> archiveLock = m_Archive.lock();
		const std::size_t size = mz_zip_reader_get_filename(archiveLock.get(), m_Index, nullptr, 0) - 1;

		std::string result(size, 0);
		mz_zip_reader_get_filename(archiveLock.get(), m_Index, result.data(), size + 1);
		return result;
	}

	void* ZipReaderEntry::Extract(std::size_t* size) const {
		std::size_t sizeInternal;
		void* const result = mz_zip_reader_extract_to_heap(m_Archive.lock().get(), m_Index, &sizeInternal, 0);

		if (!result) throw std::runtime_error("failed to extract the zip entry");
		if (size) *size = sizeInternal;

		return result;
	}

	std::size_t ZipReaderEntry::Index() const noexcept {
		return m_Index;
	}
}

namespace th {
	ZipReader::ZipReader(const std::string& path) {
		Open(path);
	}
	ZipReader::ZipReader(ZipReader&& zip) noexcept
		: m_Archive(std::move(zip.m_Archive)) {
	}
	ZipReader::~ZipReader() {
		Close();
	}

	ZipReader& ZipReader::operator=(ZipReader&& zip) noexcept {
		Close();

		m_Archive = std::move(zip.m_Archive);
		return *this;
	}
	ZipReaderEntry ZipReader::operator[](std::size_t index) const noexcept {
		assert(index < Count());
		return { m_Archive, index };
	}

	void ZipReader::Open(const std::string& path) {
		Close();

		m_Archive = std::make_shared<mz_zip_archive>();
		if (!mz_zip_reader_init_file(m_Archive.get(), path.c_str(), 0)) throw std::runtime_error("failed to open the zip file");
	}
	void ZipReader::Close() noexcept {
		if (!m_Archive) return;

		mz_zip_reader_end(m_Archive.get());
		m_Archive.reset();
	}
	bool ZipReader::IsOpen() const noexcept {
		return m_Archive != nullptr;
	}
	std::size_t ZipReader::Count() const noexcept {
		return static_cast<std::size_t>(m_Archive->m_total_files);
	}

	ZipReaderEntry ZipReader::Find(const std::string& name) const
	{
		for (std::size_t i = 0; i < Count(); ++i) {
			const ZipReaderEntry entry = (*this)[i];
			if (entry.Filename() == name) return entry;
		}
		throw std::runtime_error("failed to find the zip entry");
	}
}