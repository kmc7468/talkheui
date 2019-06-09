#ifdef _MSC_VER
#pragma warning(disable: 4244)
#endif

#include <talkheui/aheui/extension.hpp>
#include <talkheui/extension.hpp>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <memory>
#include <utility>
#include <string_view>

namespace talkheui
{
	extension_resources::extension_resources(extension_resources&& resources) noexcept
		: resources_(std::move(resources.resources_))
	{}

	extension_resources& extension_resources::operator=(extension_resources&& resources) noexcept
	{
		resources_ = std::move(resources.resources_);
		return *this;
	}
	zip_reader_entry extension_resources::operator[](const std::string& name) const
	{
		return resources_.at(name);
	}

	std::map<std::string, zip_reader_entry>::const_iterator extension_resources::find(const std::string& name) const
	{
		return resources_.find(name);
	}
	void extension_resources::add(std::string name, zip_reader_entry resource)
	{
		resources_.insert(std::make_pair(std::move(name), std::move(resource)));
	}
	std::map<std::string, zip_reader_entry> extension_resources::list() const
	{
		return resources_;
	}
}

namespace talkheui
{
	namespace
	{
		struct free
		{
			void operator()(const void* memory)
			{
				std::free(const_cast<void*>(memory));
			}
		};
	}

	extension::extension(extension_info info)
		: info_(std::move(info))
	{}
	extension::extension(extension&& extension) noexcept
		: info_(std::move(extension.info_)), resources_(std::move(extension.resources_))
	{}

	extension& extension::operator=(extension&& extension) noexcept
	{
		info_ = std::move(extension.info_);
		resources_ = std::move(extension.resources_);
		return *this;
	}

	void extension::open(const std::string& path)
	{
		zip_reader zip(path);

		// extension.json
		const zip_reader_entry ext_info = zip.find("extension.json");
		std::size_t ext_info_data_size;
		const void* ext_info_data = ext_info.extract(&ext_info_data_size);
		const std::unique_ptr<const void, talkheui::free> ext_info_data_raii(ext_info_data);
		const std::string_view ext_info_data_str(static_cast<const char*>(ext_info_data), ext_info_data_size);
		const nlohmann::json ext_info_data_json = nlohmann::json::parse(ext_info_data_str);

		info_.name = ext_info_data_json["name"];
		info_.target = ext_info_data_json["target"];

		if (info_.target.empty()) throw std::runtime_error("invalid extension");

		std::transform(info_.target.begin(), info_.target.end(), info_.target.begin(), std::tolower);
		info_.target[0] = std::toupper(info_.target.front());

		for (auto iter = ext_info_data_json.begin(); iter != ext_info_data_json.end(); ++iter)
		{
			if (iter.key() == "developer") info_.developer = *iter;
			else if (iter.key() == "description") info_.description = *iter;
		}

		// res
		for (std::size_t i = 0; i < zip.size(); ++i)
		{
			const zip_reader_entry entry = zip[i];
			if (entry.is_directory()) continue;

			const std::string entry_name = entry.filename();
			if (std::string_view(entry_name.data(), 4) != "res/") continue;

			resources_.add(entry_name.substr(4), entry);
		}

		open_priv(zip, ext_info_data_json);

		zip_ = std::move(zip);
	}

	std::string extension::name() const
	{
		return info_.name;
	}
	std::string extension::developer() const
	{
		return info_.developer;
	}
	std::string extension::description() const
	{
		return info_.description;
	}
	std::string talkheui::extension::target() const
	{
		return info_.target;
	}
	const extension_resources& extension::resources() const
	{
		return resources_;
	}

	extension* open_extension(const std::string& path)
	{
		const zip_reader zip(path);

		const zip_reader_entry ext_info = zip.find("extension.json");
		std::size_t ext_info_data_size;
		const void* ext_info_data = ext_info.extract(&ext_info_data_size);
		const std::unique_ptr<const void, talkheui::free> ext_info_data_raii(ext_info_data);
		const std::string_view ext_info_data_str(static_cast<const char*>(ext_info_data), ext_info_data_size);
		const nlohmann::json ext_info_data_json = nlohmann::json::parse(ext_info_data_str);

		std::string target = ext_info_data_json["target"];
		std::transform(target.begin(), target.end(), target.begin(), std::tolower);
		target[0] = std::toupper(target.front());

		if (target == "Aheui") return aheui::open_extension(path, zip, ext_info_data_json);
		else throw std::runtime_error("invalid target");
	}
}