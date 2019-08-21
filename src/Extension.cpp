#ifdef _MSC_VER
#	pragma warning(disable: 4244)
#endif

#include <th/Extension.hpp>
#include <th/Aheui/Extension.hpp>

#include <cstddef>
#include <cstdlib>
#include <string_view>
#include <utility>

namespace th {
	template<>
	ExtensionTarget ToEnum<ExtensionTarget>(std::string string) noexcept {
		std::transform(string.begin(), string.end(), string.begin(), std::tolower);
		if (string == "aheui") {
			return ExtensionTarget::Aheui;
		} else return ExtensionTarget::None;
	}
}

namespace th {
	ExtensionResources::ExtensionResources(ExtensionResources&& resources) noexcept
		: m_Resources(std::move(resources.m_Resources)) {
	}

	ExtensionResources& ExtensionResources::operator=(ExtensionResources&& resources) noexcept {
		m_Resources = std::move(resources.m_Resources);
		return *this;
	}
	ZipReaderEntry ExtensionResources::operator[](const std::string& name) const {
		return m_Resources.at(name);
	}

	void ExtensionResources::Add(std::string name, ZipReaderEntry resource) {
		m_Resources.insert(std::make_pair(std::move(name), std::move(resource)));
	}
	std::map<std::string, ZipReaderEntry> th::ExtensionResources::Items() const {
		return m_Resources;
	}
}

namespace {
	struct Free final {
		void operator()(const void* raw) const noexcept {
			std::free(const_cast<void*>(raw));
		}
	};
}

namespace th {
	Extension::Extension(ExtensionInfo info) noexcept
		: m_Info(std::move(info)) {
	}
	Extension::Extension(Extension&& extension) noexcept
		: m_Info(std::move(m_Info)), m_Resources(std::move(extension.m_Resources)), m_Zip(std::move(extension.m_Zip)) {
	}

	Extension& Extension::operator=(Extension&& extension) noexcept {
		m_Info = std::move(extension.m_Info);
		m_Resources = std::move(extension.m_Resources);
		m_Zip = std::move(extension.m_Zip);
		return *this;
	}

	void Extension::Open(const std::string& path) {
		Close();

		ZipReader zip(path);

		// Extension.json
		const ZipReaderEntry info = zip.Find("Extension.json");
		std::size_t infoSize;
		const void* const infoRaw = info.Extract(&infoSize);
		const std::unique_ptr<const void, Free> infoRAII(infoRaw);
		const std::string_view infoString(static_cast<const char*>(infoRaw), infoSize);
		const nlohmann::json infoJson = nlohmann::json::parse(infoString);

		const auto name = infoJson.find("Name");
		const auto target = infoJson.find("Target");

		if (name == infoJson.end()) throw std::runtime_error("failed to find property \"Name\"");
		if (name->type() != nlohmann::json::value_t::string) throw std::runtime_error("failed to read property \"Name\" as string");
		if (target == infoJson.end()) throw std::runtime_error("failed to find property \"Target\"");
		if (target->type() != nlohmann::json::value_t::string) throw std::runtime_error("failed to read property \"Target\" as string");
		
		m_Info.Name = *name;
		m_Info.Target = ToEnum<ExtensionTarget>(*target);

		if (m_Info.Target == ExtensionTarget::None) throw std::runtime_error("unsupported target language");

		for (auto iter = infoJson.begin(); iter != infoJson.end(); ++iter) {
			if (iter.key() == "Developer") {
				if (iter->type() != nlohmann::json::value_t::string) throw std::runtime_error("failed to read property \"Developer\" as string");
				m_Info.Developer = *iter;
			} else if (iter.key() == "Description") {
				if (iter->type() != nlohmann::json::value_t::string) throw std::runtime_error("failed to read property \"Description\" as string");
				m_Info.Description = *iter;
			}
		}

		ExtensionResources resources;

		for (std::size_t i = 0; i < zip.Count(); ++i) {
			const ZipReaderEntry entry = zip[i];
			if (entry.IsDirectory()) continue;

			const std::string entryName = entry.Filename();
			if (std::string_view(entryName.data(), 4) != "res/") continue;

			resources.Add(entryName.substr(4), entry);
		}

		vOpen(zip, infoJson);

		m_Zip = std::move(zip);
		m_Resources = std::move(resources);
	}
	void Extension::Close() noexcept {
		if (m_Zip.IsOpen()) {
			m_Info = ExtensionInfo();
			m_Resources = ExtensionResources();
			m_Zip.Close();
		}
	}

	std::string Extension::Name() const {
		return m_Info.Name;
	}
	ExtensionTarget Extension::Target() const noexcept {
		return m_Info.Target;
	}

	std::string Extension::Developer() const {
		return m_Info.Developer;
	}
	std::string Extension::Description() const {
		return m_Info.Description;
	}

	const ExtensionResources& Extension::Resources() const {
		return m_Resources;
	}

	Extension* OpenExtension(const std::string& path) {
		ZipReader zip(path);

		const ZipReaderEntry info = zip.Find("Extension.json");
		std::size_t infoSize;
		const void* const infoRaw = info.Extract(&infoSize);
		const std::unique_ptr<const void, Free> infoRAII(infoRaw);
		const std::string_view infoString(static_cast<const char*>(infoRaw), infoSize);
		const nlohmann::json infoJson = nlohmann::json::parse(infoString);

		const auto target = infoJson.find("Target");
		if (target == infoJson.end()) throw std::runtime_error("failed to find property \"Target\"");
		if (target->type() != nlohmann::json::value_t::string) throw std::runtime_error("failed to read property \"Target\" as string");

		const ExtensionTarget targetEnum = ToEnum<ExtensionTarget>(*target);
		if (targetEnum == ExtensionTarget::None) throw std::runtime_error("unsupported target language");

		zip.Close();

		switch (targetEnum) {
		case ExtensionTarget::Aheui:
			return aheui::OpenExtension(path);
		
		default:
			return nullptr;
		}
	}
}