#include <th/lua.hpp>

#include <limits>
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
	
	bool Lua::GetGlobal(const std::string& name) {
		lua_getglobal(m_State, name.c_str());
		return lua_isnoneornil(m_State, 0);
	}
	void Lua::Push(long long number) {
		lua_pushinteger(m_State, number);
	}
	void Lua::Push(const char* string) {
		lua_pushstring(m_State, string);
	}
#ifdef TH_USE_MULTIPRECISION
	void Lua::Push(boost::multiprecision::int128_t number) {
		if (number <= std::numeric_limits<long long>::max() &&
			number >= std::numeric_limits<long long>::min()) {
			Push(static_cast<long long>(number));
		} else {
			CreateTable(2);

			Push("low");
			Push(static_cast<long long>(number & 0xFFFFFFFFFFFFFFFF));
			SetTable(-3);

			Push("high");
			Push(static_cast<long long>(number >> 64 & 0xFFFFFFFFFFFFFFFF));
			SetTable(-3);
		}
	}
#endif
	long long Lua::PopInteger() {
		const long long result = lua_tointeger(m_State, -1);
		return lua_pop(m_State, 1), result;
	}
#ifdef TH_USE_MULTIPRECISION
	boost::multiprecision::int128_t Lua::PopInteger128() {
		if (lua_isinteger(m_State, -1)) {
			return PopInteger();
		} else {
			return 0; // TODO
		}
	}
#endif
	void Lua::Call(int paramSize, int retSize) {
		lua_call(m_State, paramSize, retSize);
	}

	void Lua::CreateTable(int elements) {
		lua_createtable(m_State, 0, elements);
	}
	void Lua::SetTable(int index) {
		lua_settable(m_State, index);
	}
}