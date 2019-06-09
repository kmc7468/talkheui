#pragma once

#include <talkheui/zip.hpp>

#include <map>
#include <nlohmann/json.hpp>
#include <string>

namespace talkheui
{
	struct extension_info final
	{
		std::string name;
		std::string developer;
		std::string description;
		std::string target;
	};

	class extension_resources final
	{
	public:
		extension_resources() = default;
		extension_resources(extension_resources&& resources) noexcept;
		~extension_resources() = default;

	public:
		extension_resources& operator=(extension_resources&& resources) noexcept;
		zip_reader_entry operator[](const std::string& name) const;

	public:
		std::map<std::string, zip_reader_entry>::const_iterator find(const std::string& name) const;
		void add(std::string name, zip_reader_entry resource);
		std::map<std::string, zip_reader_entry> list() const;

	private:
		std::map<std::string, zip_reader_entry> resources_;
	};

	class extension
	{
	public:
		virtual ~extension() = default;

	protected:
		extension() = default;
		extension(extension_info info);
		extension(extension&& extension) noexcept;

	protected:
		extension& operator=(extension&& extension) noexcept;

	public:
		void open(const std::string& path);

	protected:
		virtual void open_priv(const zip_reader& extension, const nlohmann::json& extension_info) = 0;

	public:
		std::string name() const;
		std::string developer() const;
		std::string description() const;
		std::string target() const;
		const extension_resources& resources() const;

	private:
		extension_info info_;
		extension_resources resources_;
		zip_reader zip_;
	};

	extension* open_extension(const std::string& path);
}