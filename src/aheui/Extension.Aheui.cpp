#ifdef _MSC_VER
#	pragma warning(disable: 4244 4996)
#endif

#include <th/Encoding.hpp>
#include <th/aheui/Extension.hpp>

#include <algorithm>
#include <cctype>
#include <utility>

namespace th::aheui {
	Extension::Extension(ExtensionType type)
		: m_Type(type) {
	}
	Extension::Extension(ExtensionType type, ExtensionInfo info)
		: th::Extension(std::move(info)), m_Type(type) {
	}
	Extension::Extension(Extension&& extension) noexcept
		: th::Extension(std::move(extension)), m_Type(extension.m_Type) {
	}

	Extension& Extension::operator=(Extension&& extension) noexcept {
		th::Extension::operator=(std::move(extension));
		m_Type = extension.m_Type;

		return *this;
	}

	void Extension::vOpen(const ZipReader&, const nlohmann::json& info) {
		const auto type = info.find("Type");

		if (type == info.end()) throw std::runtime_error("failed to find property \"Type\"");
		if (type->type() != nlohmann::json::value_t::string) throw std::runtime_error("failed to read property \"Type\" as string");

		std::string typeStr = *type;
		std::transform(typeStr.begin(), typeStr.end(), typeStr.begin(), [](char c){ return std::tolower(c); });

		if (typeStr == "lua") {
			m_Type = ExtensionType::Lua;
		} else throw std::runtime_error("unsupported type");
	}

	ExtensionType Extension::Type() const noexcept {
		return m_Type;
	}
}

namespace {
	struct Free final {
		void operator()(const void* raw) const noexcept {
			std::free(const_cast<void*>(raw));
		}
	};
}

namespace th::aheui {
	LuaExtension::LuaExtension()
		: Extension(ExtensionType::Lua) {
#ifdef TH_USE_MULTIPRECISION
		m_Lua.AddInt128Class();
#endif
	}
	LuaExtension::LuaExtension(const std::string& path)
		: LuaExtension() {
		Open(path);
	}
	LuaExtension::LuaExtension(LuaExtension&& extension) noexcept
		: Extension(std::move(extension)), m_Lua(std::move(extension.m_Lua)) {
	}
	
	LuaExtension& LuaExtension::operator=(LuaExtension&& extension) noexcept {
		Extension::operator=(std::move(extension));
		m_Lua = std::move(extension.m_Lua);

		return *this;
	}

	void LuaExtension::Send(detail::Value value) {
		m_Lua.GetGlobal("receive");
		m_Lua.Push(value);
		m_Lua.Call(1, 0);
	}
	detail::Value LuaExtension::Receive() {
		m_Lua.GetGlobal("send");
		m_Lua.Call(0, 1);
#ifdef TH_USE_MULTIPRECISION
		return m_Lua.PopInteger128();
#else
		return m_Lua.PopInteger();
#endif
	}
	long long LuaExtension::Count() {
		m_Lua.GetGlobal("send_count");
		m_Lua.Call(0, 1);
		return m_Lua.PopInteger();
	}
	long long LuaExtension::Bytes() {
		if (m_Lua.GetGlobal("send_bytes")) {
			m_Lua.Call(0, 1);
			return m_Lua.PopInteger();
		} else return Count() * sizeof(long long);
	}

	void LuaExtension::vOpen(const ZipReader& zip, const nlohmann::json& info) {
		Extension::vOpen(zip, info);

		const auto source = info.find("Source");
		if (source == info.end()) throw std::runtime_error("failed to find property \"Source\"");
		if (source->type() != nlohmann::json::value_t::string) throw std::runtime_error("failed to read property \"Source\" as string");
		const std::string sourceStr = *source;

		const ZipReaderEntry script = zip.Find(sourceStr);
		std::size_t scriptSize;
		const void* const scriptRaw = script.Extract(&scriptSize);
		const std::unique_ptr<const void, Free> scriptRAII(scriptRaw);
		const std::string_view scriptString(static_cast<const char*>(scriptRaw), scriptSize);

		m_Lua.LoadScript(scriptString);
	}
}

namespace th::aheui {
	Extension* OpenExtension(const std::string& path) {
		ZipReader zip(path);

		const ZipReaderEntry info = zip.Find("Extension.json");
		std::size_t infoSize;
		const void* const infoRaw = info.Extract(&infoSize);
		const std::unique_ptr<const void, Free> infoRAII(infoRaw);
		const std::string_view infoString(static_cast<const char*>(infoRaw), infoSize);
		const nlohmann::json infoJson = nlohmann::json::parse(infoString);

		const auto type = infoJson.find("Type");
		if (type == infoJson.end()) throw std::runtime_error("failed to find property \"Type\"");
		if (type->type() != nlohmann::json::value_t::string) throw std::runtime_error("failed to read property \"Type\" as string");

		std::string typeStr = *type;
		std::transform(typeStr.begin(), typeStr.end(), typeStr.begin(), [](char c){ return std::tolower(c); });

		if (typeStr == "lua") return new LuaExtension(path);
		else throw std::runtime_error("unsupported type");
	}
}