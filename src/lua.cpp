#include <talkheui/lua.hpp>

#include <stdexcept>

namespace talkheui
{
	lua_engine::lua_engine()
		: state_(luaL_newstate())
	{
		if (!state_) throw std::runtime_error("failed to create a new lua state");

		luaL_openlibs(state_);
	}
	lua_engine::lua_engine(lua_engine&& engine) noexcept
		: state_(engine.state_)
	{
		engine.state_ = nullptr;
	}
	lua_engine::~lua_engine()
	{
		if (state_) lua_close(state_);
	}

	lua_engine& lua_engine::operator=(lua_engine&& engine) noexcept
	{
		state_ = engine.state_;
		engine.state_ = nullptr;
		return *this;
	}
}