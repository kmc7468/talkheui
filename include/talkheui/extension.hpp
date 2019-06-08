#pragma once

#include <talkheui/zip.hpp>

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

	class extension
	{
	public:
		virtual ~extension() = default;

	protected:
		extension() noexcept = default;
		extension(extension_info info) noexcept;
		extension(const extension& extension);
		extension(extension&& extension) noexcept;

	protected:
		extension& operator=(const extension& extension);
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

	private:
		extension_info info_;
	};
}