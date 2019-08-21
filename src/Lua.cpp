#include <th/lua.hpp>

#include <stdexcept>

namespace th {
	Lua::Lua()
		: m_State(luaL_newstate()) {
		if (!m_State) throw std::runtime_error("failed to start a lua interpreter");
		luaL_openlibs(m_State);
	}
	Lua::Lua(Lua&& lua) noexcept
		: m_State(lua.m_State) {
		lua.m_State = nullptr;
	}
	Lua::~Lua() {
		if (m_State) {
			lua_close(m_State);
		}
	}

	Lua& Lua::operator=(Lua&& lua) noexcept {
		m_State = lua.m_State;
		lua.m_State = nullptr;
		return *this;
	}

	void Lua::LoadScript(const std::string_view& script) {
		luaL_loadbuffer(m_State, script.data(), script.size(), script.data());
		lua_pcall(m_State, 0, 0, 0);
	}
	
	void Lua::GetGlobal(const std::string& name) {
		lua_getglobal(m_State, name.c_str());
	}
	void Lua::Push(long long number) {
		lua_pushinteger(m_State, number);
	}
	long long Lua::PopInteger() {
		const long long result = lua_tointeger(m_State, -1);
		return lua_pop(m_State, 1), result;
	}
	void Lua::Call(int paramSize, int retSize) {
		lua_call(m_State, paramSize, retSize);
	}
}