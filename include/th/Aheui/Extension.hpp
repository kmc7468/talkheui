#pragma once

#include <th/Extension.hpp>
#include <th/Lua.hpp>
#include <th/Zip.hpp>
#include <th/aheui/CodePlane.hpp>
#include <th/aheui/Interpreter.hpp>
#include <th/aheui/detail/Type.hpp>

#include <boost/multiprecision/cpp_int.hpp>
#include <nlohmann/json.hpp>

namespace th::aheui {
	enum class ExtensionType {
		Lua,
	};

	class Extension : public th::Extension {
	private:
		ExtensionType m_Type;

	public:
		virtual ~Extension() override = default;

	protected:
		Extension(ExtensionType type);
		Extension(ExtensionType type, ExtensionInfo info);
		Extension(Extension&& extension) noexcept;

	protected:
		Extension& operator=(Extension&& extension) noexcept;

	public:
		virtual void Send(detail::Value value) = 0;
		virtual detail::Value Receive() = 0;
		virtual long long Count() = 0;
		virtual long long Bytes() = 0;

	protected:
		virtual void vOpen(const ZipReader& extension, const nlohmann::json& info) override;

	public:
		ExtensionType Type() const noexcept;
	};

	class LuaExtension final : public Extension {
	private:
		Lua m_Lua;

	public:
		LuaExtension();
		LuaExtension(const std::string& path);
		LuaExtension(LuaExtension&& extension) noexcept;
		virtual ~LuaExtension() override = default;

	public:
		LuaExtension& operator=(LuaExtension&& extension) noexcept;

	public:
		virtual void Send(detail::Value value) override;
		virtual detail::Value Receive() override;
		virtual long long Count() override;
		virtual long long Bytes() override;

	protected:
		virtual void vOpen(const ZipReader& extension, const nlohmann::json& info) override;
	};

	Extension* OpenExtension(const std::string& path);
}