#ifdef _MSC_VER
#pragma warning(disable: 4244)
#endif

#include <talkheui/aheui/extension.hpp>

#include <algorithm>
#include <cctype>

namespace
{
	struct free_
	{
		void operator()(const void* memory)
		{
			std::free(const_cast<void*>(memory));
		}
	};
}

namespace talkheui::aheui
{
	extension::extension(extension_type type)
		: type_(type)
	{}
	extension::extension(extension_type type, extension_info info)
		: talkheui::extension(info), type_(type)
	{}
	extension::extension(extension&& extension) noexcept
		: talkheui::extension(std::move(extension)), type_(extension.type_)
	{}

	extension& extension::operator=(extension&& extension) noexcept
	{
		talkheui::extension::operator=(std::move(extension));
		type_ = extension.type_;
		return *this;
	}

	void extension::open_priv(const zip_reader& extension, const nlohmann::json& extension_info)
	{
		std::string ext_type_str = extension_info["type"];
		std::transform(ext_type_str.begin(), ext_type_str.end(), ext_type_str.begin(), std::tolower);
		ext_type_str[0] = std::toupper(ext_type_str.front());

		if (ext_type_str == "Lua") type_ = extension_type::lua;
		else if (ext_type_str == "Aheui") type_ = extension_type::aheui;
		else throw std::runtime_error("invalid type");
	}

	extension_type extension::type() const noexcept
	{
		return type_;
	}
}

namespace talkheui::aheui
{
	lua_extension::lua_extension()
		: extension(extension_type::lua)
	{}
	lua_extension::lua_extension(const std::string& path)
		: lua_extension()
	{
		open(path);
	}
	lua_extension::lua_extension(lua_extension&& extension) noexcept
		: extension(std::move(extension)), lua_(std::move(extension.lua_))
	{}

	lua_extension& lua_extension::operator=(lua_extension&& extension) noexcept
	{
		extension::operator=(std::move(extension));
		lua_ = std::move(extension.lua_);
		return *this;
	}

	void lua_extension::send(long long value)
	{
		lua_.getglobal("receive");
		lua_.push(value);
		lua_.call(1, 0);
	}
	long long lua_extension::receive()
	{
		lua_.getglobal("send");
		lua_.call(0, 1);
		return lua_.pop_integer();
	}

	void lua_extension::open_priv(const zip_reader& extension, const nlohmann::json& extension_info)
	{
		extension::open_priv(extension, extension_info);
		
		const std::string source_name = extension_info["source"];
		const zip_reader_entry source = extension.find(source_name);
		std::size_t source_data_size;
		const void* source_data = source.extract(&source_data_size);
		const std::unique_ptr<const void, free_> source_data_raii(source_data);
		const std::string_view source_data_str(static_cast<const char*>(source_data), source_data_size);

		lua_.load_script(source_data_str);
	}
}

namespace talkheui::aheui
{
	aheui_extension::aheui_extension()
		: extension(extension_type::aheui)
	{}
	aheui_extension::aheui_extension(const std::string& path)
		: aheui_extension()
	{
		open(path);
	}
	aheui_extension::aheui_extension(aheui_extension&& extension) noexcept
		: extension(std::move(extension))
	{}

	aheui_extension& aheui_extension::operator=(aheui_extension&& extension) noexcept
	{
		extension::operator=(std::move(extension));
		return *this;
	}

	void aheui_extension::send(long long value)
	{
		// TODO
	}
	long long aheui_extension::receive()
	{
		// TODO
		return 0;
	}

	void aheui_extension::open_priv(const zip_reader& extension, const nlohmann::json& extension_info)
	{
		extension::open_priv(extension, extension_info);
	}
}

namespace talkheui::aheui
{
	extension* open_extension(const std::string& path)
	{
		const zip_reader zip(path);

		const zip_reader_entry ext_info = zip.find("extension.json");
		std::size_t ext_info_data_size;
		const void* ext_info_data = ext_info.extract(&ext_info_data_size);
		const std::unique_ptr<const void, free_> ext_info_data_raii(ext_info_data);
		const std::string_view ext_info_data_str(static_cast<const char*>(ext_info_data), ext_info_data_size);
		const nlohmann::json ext_info_data_json = nlohmann::json::parse(ext_info_data_str);

		return open_extension(path, zip, ext_info_data_json);
	}
	extension* open_extension(const std::string& path, const zip_reader& extension, const nlohmann::json& extension_info)
	{
		std::string ext_type_str = extension_info["type"];
		std::transform(ext_type_str.begin(), ext_type_str.end(), ext_type_str.begin(), std::tolower);
		ext_type_str[0] = std::toupper(ext_type_str.front());

		if (ext_type_str == "Lua") return new lua_extension(path);
		else if (ext_type_str == "Aheui") return new aheui_extension(path);
		else throw std::runtime_error("invalid type");
	}
}