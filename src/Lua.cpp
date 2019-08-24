#include <th/lua.hpp>

#include <th/IOStream.hpp>

#include <limits>
#include <stdexcept>

namespace th {
	Lua::Lua()
		: m_State(luaL_newstate()) {
		if (!m_State) throw std::runtime_error("failed to start a lua interpreter");
		luaL_openlibs(m_State);

		Push(LuaIsInt128Available);
		SetGlobal("is_int128_available");
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
	void Lua::SetGlobal(const std::string& name) {
		lua_setglobal(m_State, name.c_str());
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
			*LuaPushInt128(m_State) = number;
		}
	}
#endif
	void Lua::Push(bool boolean) {
		lua_pushboolean(m_State, boolean);
	}
	void Lua::Push(lua_CFunction function) {
		lua_pushcfunction(m_State, function);
	}
	long long Lua::PopInteger() {
		const long long result = lua_tointeger(m_State, -1);
		return lua_pop(m_State, 1), result;
	}
#ifdef TH_USE_MULTIPRECISION
	boost::multiprecision::int128_t Lua::PopInteger128() {
		if (lua_isinteger(m_State, -1)) {
			return PopInteger();
		} else {
			boost::multiprecision::int128_t* const v =
				static_cast<boost::multiprecision::int128_t*>(lua_touserdata(m_State, -1));
			return lua_pop(m_State, 1), *v;
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

	void Lua::AddInt128Class() {
		static const luaL_Reg int128Methods[] = {
			{ "new", LuaInt128New },
			{ "print", LuaInt128Print },
			{ "println", LuaInt128PrintLn },
			{ "read", LuaInt128Read },
			{ nullptr, nullptr },
		};
		static const luaL_Reg int128MetaTable[] = {
			{ "__add", LuaInt128Add },
			{ nullptr, nullptr },
		};

		lua_createtable(m_State, 0, 0);
		const int tableId = lua_gettop(m_State);

		luaL_newmetatable(m_State, "Int128");
		const int metaId = lua_gettop(m_State);
		luaL_setfuncs(m_State, int128MetaTable, 0);

		luaL_newlib(m_State, int128Methods);
		lua_setfield(m_State, metaId, "__index");
		luaL_newlib(m_State, int128MetaTable);
		lua_setfield(m_State, metaId, "__metatable");

		lua_setmetatable(m_State, tableId);

		lua_setglobal(m_State, "Int128");
	}
	int Lua::LuaIsInt128Available(lua_State* state) {
#ifdef TH_USE_MULTIPRECISION
		lua_pushboolean(state, true);
#else
		lua_pushboolean(state, false);
#endif
		return 1;
	}
#ifdef TH_USE_MULTIPRECISION
	boost::multiprecision::int128_t* Lua::LuaPushInt128(lua_State* state) {
		boost::multiprecision::int128_t* const result =
			static_cast<boost::multiprecision::int128_t*>(lua_newuserdata(state, sizeof(boost::multiprecision::int128_t)));
		luaL_getmetatable(state, "Int128");
		lua_setmetatable(state, -2);
		return result;
	}
	int Lua::LuaInt128New(lua_State* state) {
		const int argNum = lua_gettop(state);
		if (argNum == 0) {
			*LuaPushInt128(state) = 0;
		} else if (argNum == 1) {
			if (lua_isinteger(state, -1)) {
				*LuaPushInt128(state) = luaL_checkinteger(state, -2);
			} else if(lua_isuserdata(state, -1)) {
				*LuaPushInt128(state) = *static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
			} else {
				luaL_error(state, "Int128.new invalid argument type");
			}
		} else if (argNum == 2) {
			if (lua_isinteger(state, -1) && lua_isinteger(state, -2)) {
				*LuaPushInt128(state) = luaL_checkinteger(state, -2) | (boost::multiprecision::int128_t(luaL_checkinteger(state, -3)) << 64);
			} else {
				luaL_error(state, "Int128.new invalid argument type");
			}
		} else {
			luaL_error(state, "Int128.new too many arguments");
		}
		return 1;
	}
	int Lua::LuaInt128Print(lua_State* state) {
		const int argNum = lua_gettop(state);
		if (argNum == 0) {
			luaL_error(state, "Int128.new needs argument");
		} else if (argNum == 1) {
			WriteStdout(*static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128")));
		} else {
			luaL_error(state, "Int128.new too many arguments");
		}
		return 0;
	}
	int Lua::LuaInt128PrintLn(lua_State* state) {
		const int argNum = lua_gettop(state);
		if (argNum == 0) {
			luaL_error(state, "Int128.new needs argument");
		} else if (argNum == 1) {
			WriteStdout(*static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128")));
			WriteStdout(U'\n');
		} else {
			luaL_error(state, "Int128.new too many arguments");
		}
		return 0;
	}
	int Lua::LuaInt128Read(lua_State* state) {
		const int argNum = lua_gettop(state);
		if (argNum == 0) {
			*LuaPushInt128(state) = ReadInteger128Stdin();
		} else {
			luaL_error(state, "Int128.new too many arguments");
		}
		return 1;
	}
	int Lua::LuaInt128Add(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = *left + *right;
		return 1;
	}
#endif
}