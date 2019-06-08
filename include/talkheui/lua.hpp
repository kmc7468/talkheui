#pragma once

#include <lua/lua.hpp>

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

	private:
		lua_State* state_ = nullptr;
	};
}