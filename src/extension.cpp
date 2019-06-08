#include <talkheui/extension.hpp>

#include <nlohmann/json.hpp>

#include <cstdlib>
#include <memory>
#include <utility>
#include <string_view>

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

	extension::extension(extension_info info) noexcept
		: info_(std::move(info))
	{}
	extension::extension(const extension& extension)
		: info_(extension.info_)
	{}
	extension::extension(extension&& extension) noexcept
		: info_(std::move(extension.info_))
	{}

	extension& extension::operator=(const extension& extension)
	{
		info_ = extension.info_;
		return *this;
	}
	extension& extension::operator=(extension&& extension) noexcept
	{
		info_ = std::move(extension.info_);
		return *this;
	}

	void extension::open(const std::string& path)
	{
		const zip_reader zip(path);

		const zip_reader_entry ext_info = zip.find("extension.json");
		std::size_t ext_info_data_size;
		const void* ext_info_data = ext_info.extract(&ext_info_data_size);
		const std::unique_ptr<const void, talkheui::free> ext_info_data_raii(ext_info_data);
		const std::string_view ext_info_data_str(static_cast<const char*>(ext_info_data), ext_info_data_size);
		const nlohmann::json ext_info_data_json = nlohmann::json::parse(ext_info_data_str);

		for (auto iter = ext_info_data_json.begin(); iter < ext_info_data_json.end(); ++iter)
		{
			if (iter.key() == "name") info_.name = *iter;
			else if (iter.key() == "developer") info_.developer = *iter;
			else if (iter.key() == "description") info_.description = *iter;
		}

		open_priv(zip);
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
}