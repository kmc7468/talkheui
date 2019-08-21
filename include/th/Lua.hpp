#pragma once

#include <lua/lua.hpp>

#include <string_view>

namespace talkheui
{
	class lua_engine final
	{
	public:
		lua_engine();
		lua_engine(lua_engine&& engine) noexcept;
		~lua_engine();

	public:
		lua_engine& operator=(lua_engine&& engine) noexcept;

	public:
		void load_script(const std::string_view& script);
		void getglobal(const std::string& name);
		void push(long long number);
		long long pop_integer();
		void call(int param_size, int ret_size);

	private:
		lua_State* state_ = nullptr;
	};
}