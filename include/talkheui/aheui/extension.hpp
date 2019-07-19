#pragma once

#include <talkheui/aheui/codeplane.hpp>
#include <talkheui/aheui/interpreter.hpp>
#include <talkheui/extension.hpp>
#include <talkheui/lua.hpp>
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

	public:
		virtual void send(long long value) = 0;
		virtual long long receive() = 0;
		virtual long long size() = 0;
		virtual long long bytes() = 0;

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

	public:
		virtual void send(long long value) override;
		virtual long long receive() override;
		virtual long long size() override;
		virtual long long bytes() override;

	protected:
		virtual void open_priv(const zip_reader& extension, const nlohmann::json& extension_info) override;

	private:
		lua_engine lua_;
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

	public:
		virtual void send(long long value) override;
		virtual long long receive() override;
		virtual long long size() override;
		virtual long long bytes() override;

	protected:
		virtual void open_priv(const zip_reader& extension, const nlohmann::json& extension_info) override;

	private:
		codeplane evt_send_, evt_receive_;
		interpreter interpreter_;
	};
	
	extension* open_extension(const std::string& path);
	extension* open_extension(const std::string& path, const zip_reader& extension, const nlohmann::json& extension_info);
}