#ifdef _MSC_VER
#pragma warning(disable: 4244)
#endif

#include <talkheui/extension.hpp>

#include <algorithm>
#include <cctype>
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

		info_.name = ext_info_data_json["name"];
		info_.target = ext_info_data_json["target"];

		if (info_.target.empty()) throw std::runtime_error("invalid extension");

		std::transform(info_.target.begin(), info_.target.end(), info_.target.begin(), std::tolower);
		info_.target[0] = std::toupper(info_.target.front());

		for (auto iter = ext_info_data_json.begin(); iter < ext_info_data_json.end(); ++iter)
		{
			if (iter.key() == "developer") info_.developer = *iter;
			else if (iter.key() == "description") info_.description = *iter;
		}

		open_priv(zip, ext_info_data_json);
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
}