#pragma once

#include <talkheui/extension.hpp>
#include <talkheui/zip.hpp>

#include <nlohmann/json.hpp>

namespace talkheui::aheui
{
	enum class extension_type
	{
		lua,
		aheui,
	};

	class extension : public talkheui::extension
	{
	public:
		virtual ~extension() override = default;

	protected:
		extension(extension_type type);
		extension(extension_type type, extension_info info);
		extension(extension&& extension) noexcept;

	protected:
		extension& operator=(extension&& extension) noexcept;

	protected:
		virtual void open_priv(const zip_reader& extension, const nlohmann::json& extension_info) override;

	public:
		extension_type type() const noexcept;

	private:
		extension_type type_;
	};

	class lua_extension final : public extension
	{
	public:
		lua_extension();
		lua_extension(const std::string& path);
		lua_extension(lua_extension&& extension) noexcept;
		virtual ~lua_extension() override = default;

	public:
		lua_extension& operator=(lua_extension&& extension) noexcept;

	protected:
		virtual void open_priv(const zip_reader& extension, const nlohmann::json& extension_info) override;
	};

	class aheui_extension final : public extension
	{
	public:
		aheui_extension();
		aheui_extension(const std::string& path);
		aheui_extension(aheui_extension&& extension) noexcept;
		virtual ~aheui_extension() override = default;

	public:
		aheui_extension& operator=(aheui_extension&& extension) noexcept;

	protected:
		virtual void open_priv(const zip_reader& extension, const nlohmann::json& extension_info) override;
	};
	
	extension* open_extension(const std::string& path);
	extension* open_extension(const std::string& path, const zip_reader& extension, const nlohmann::json& extension_info);
}