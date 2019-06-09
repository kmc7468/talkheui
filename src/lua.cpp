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

	void lua_engine::load_script(const std::string_view& script)
	{
		luaL_loadbuffer(state_, script.data(), script.size(), script.data());
		lua_pcall(state_, 0, 0, 0);
	}
	void lua_engine::getglobal(const std::string& name)
	{
		lua_getglobal(state_, name.c_str());
	}
	void lua_engine::push(long long number)
	{
		lua_pushinteger(state_, number);
	}
	long long lua_engine::pop_integer()
	{
		const long long result = lua_tointeger(state_, -1);
		return lua_pop(state_, 1), result;
	}
	void lua_engine::call(int param_size, int ret_size)
	{
		lua_call(state_, param_size, ret_size);
	}
}