#pragma once

#include <th/Zip.hpp>

#include <algorithm>
#include <cctype>
#include <map>
#include <string>

#include <nlohmann/json.hpp>

namespace th {
	template<typename E>
	E ToEnum(std::string string) noexcept;
}

namespace th {
	enum class ExtensionTarget {
		None,

		Aheui,
	};

	template<>
	ExtensionTarget ToEnum<ExtensionTarget>(std::string string) noexcept {
		std::transform(string.begin(), string.end(), string.begin(), std::tolower);
		if (string == "aheui") {
			return ExtensionTarget::Aheui;
		} else ExtensionTarget::None;
	}

	struct ExtensionInfo final {
		std::string Name;
		ExtensionTarget Target;

		std::string Developer;
		std::string Description;
	};

	class ExtensionResources final {
	private:
		std::map<std::string, ZipReaderEntry> m_Resources;

	public:
		ExtensionResources() = default;
		ExtensionResources(ExtensionResources&& resources) noexcept;
		~ExtensionResources() = default;

	public:
		ExtensionResources& operator=(ExtensionResources&& resources) noexcept;
		ZipReaderEntry operator[](const std::string& name) const;

	public:
		void Add(std::string name, ZipReaderEntry resource);
		std::map<std::string, ZipReaderEntry> Items() const;
	};

	class Extension {
	private:
		ExtensionInfo m_Info;
		ExtensionResources m_Resources;
		ZipReader m_Zip;

	public:
		virtual ~Extension() = default;

	protected:
		Extension() = default;
		Extension(ExtensionInfo info) noexcept;
		Extension(Extension&& extension) noexcept;

	protected:
		Extension& operator=(Extension&& extension) noexcept;

	public:
		void Open(const std::string& path);
		void Close() noexcept;

	protected:
		virtual void vOpen(const ZipReader& zip, const nlohmann::json& info) = 0;

	public:
		std::string Name() const;
		ExtensionTarget Target() const noexcept;

		std::string Developer() const;
		std::string Description() const;

		const ExtensionResources& Resources() const;
	};

	Extension* OpenExtension(const std::string& path);
}