#include <th/lua.hpp>

#include <th/Config.hpp>
#include <th/IOStream.hpp>

#include <limits>
#include <stdexcept>

#ifdef TH_USE_MULTIPRECISION
#	include <boost/lexical_cast.hpp>
#endif

namespace th {
	Lua::Lua()
		: m_State(luaL_newstate()) {
		if (!m_State) throw std::runtime_error("failed to start a lua interpreter");
		luaL_openlibs(m_State);

		Push(LuaGetTalkheuiVersion);
		SetGlobal("get_talkheui_version");

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

#ifdef TH_USE_MULTIPRECISION
	void Lua::AddInt128Class() {
		static const luaL_Reg int128Methods[] = {
			{ "new", LuaInt128New },
			{ "print", LuaInt128Print },
			{ "println", LuaInt128PrintLn },
			{ "read", LuaInt128Read },
			{ "get_low_qword", LuaInt128GetLowQWord },
			{ "get_high_qword", LuaInt128GetHighQWord },
			{ nullptr, nullptr },
		};
		static const luaL_Reg int128MetaTable[] = {
			{ "__add", LuaInt128Add },
			{ "__sub", LuaInt128Sub },
			{ "__mul", LuaInt128Mul },
			{ "__pow", LuaInt128Pow },
			{ "__div", LuaInt128Div },
			{ "__idiv", LuaInt128Div },
			{ "__mod", LuaInt128Mod },
			{ "__unm", LuaInt128Neg },
			{ "__eq", LuaInt128Equal },
			{ "__lt", LuaInt128Less },
			{ "__le", LuaInt128LessEqual },
			{ "__band", LuaInt128BitAnd },
			{ "__bor", LuaInt128BitOr },
			{ "__bxor", LuaInt128BitXor },
			{ "__shl", LuaInt128BitLeftShift },
			{ "__shr", LuaInt128BitRightShift },
			{ "__bnot", LuaInt128BitNot },
			{ "__tostring", LuaInt128ToString },
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

		Push(LuaInt128ToInt128);
		SetGlobal("to_int128");
	}
#endif

	int Lua::LuaGetTalkheuiVersion(lua_State* state) {
		const int argNum = lua_gettop(state);
		if (argNum == 0) {
			lua_pushstring(state, TH_VERSION);
		} else {
			luaL_error(state, "get_talkheui_version too many arguments");
		}
		return 1;
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
			} else if (lua_isstring(state, -1)) {
				*LuaPushInt128(state) = boost::multiprecision::int128_t(luaL_checkstring(state, -2));
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
			luaL_error(state, "Int128.print needs argument");
		} else if (argNum == 1) {
			WriteStdout(*static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128")));
		} else {
			luaL_error(state, "Int128.print too many arguments");
		}
		return 0;
	}
	int Lua::LuaInt128PrintLn(lua_State* state) {
		const int argNum = lua_gettop(state);
		if (argNum == 0) {
			luaL_error(state, "Int128.println needs argument");
		} else if (argNum == 1) {
			WriteStdout(*static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128")));
			WriteStdout(U'\n');
		} else {
			luaL_error(state, "Int128.println too many arguments");
		}
		return 0;
	}
	int Lua::LuaInt128Read(lua_State* state) {
		const int argNum = lua_gettop(state);
		if (argNum == 0) {
			*LuaPushInt128(state) = ReadInteger128Stdin();
		} else {
			luaL_error(state, "Int128.read too many arguments");
		}
		return 1;
	}
	int Lua::LuaInt128GetLowQWord(lua_State* state) {
		const int argNum = lua_gettop(state);
		if (argNum == 0) {
			luaL_error(state, "Int128.get_low_qword needs argument");
		} else if (argNum == 1) {
			lua_pushinteger(state, static_cast<long long>(*static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128")) & 0xFFFFFFFFFFFFFFFF));
		} else {
			luaL_error(state, "Int128.get_low_qword too many arguments");
		}
		return 1;
	}
	int Lua::LuaInt128GetHighQWord(lua_State* state) {
		const int argNum = lua_gettop(state);
		if (argNum == 0) {
			luaL_error(state, "Int128.get_high_qword needs argument");
		} else if (argNum == 1) {
			lua_pushinteger(state, static_cast<long long>(*static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128")) >> 64 & 0xFFFFFFFFFFFFFFFF));
		} else {
			luaL_error(state, "Int128.get_high_qword too many arguments");
		}
		return 1;
	}
	int Lua::LuaInt128Add(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = *left + *right;
		return 1;
	}
	int Lua::LuaInt128Sub(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = *left - *right;
		return 1;
	}
	int Lua::LuaInt128Mul(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = *left * *right;
		return 1;
	}
	int Lua::LuaInt128Pow(lua_State* state) {
		const long long right = luaL_checkinteger(state, -1);
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = boost::multiprecision::pow(*left, static_cast<unsigned int>(right));
		return 1;
	}
	int Lua::LuaInt128Div(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = *left / *right;
		return 1;
	}
	int Lua::LuaInt128Mod(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = *left % *right;
		return 1;
	}
	int Lua::LuaInt128Neg(lua_State* state) {
		boost::multiprecision::int128_t* const value = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		*LuaPushInt128(state) = -*value;
		return 1;
	}
	int Lua::LuaInt128Equal(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		lua_pushboolean(state, *left == *right);
		return 1;
	}
	int Lua::LuaInt128Less(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		lua_pushboolean(state, *left < *right);
		return 1;
	}
	int Lua::LuaInt128LessEqual(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		lua_pushboolean(state, *left <= *right);
		return 1;
	}
	int Lua::LuaInt128BitAnd(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = *left & *right;
		return 1;
	}
	int Lua::LuaInt128BitOr(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = *left | *right;
		return 1;
	}
	int Lua::LuaInt128BitXor(lua_State* state) {
		boost::multiprecision::int128_t* const right = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = *left ^ *right;
		return 1;
	}
	int Lua::LuaInt128BitLeftShift(lua_State* state) {
		const long long right = luaL_checkinteger(state, -1);
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = *left << right;
		return 1;
	}
	int Lua::LuaInt128BitRightShift(lua_State* state) {
		const long long right = luaL_checkinteger(state, -1);
		boost::multiprecision::int128_t* const left = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -2, "Int128"));
		*LuaPushInt128(state) = *left >> right;
		return 1;
	}
	int Lua::LuaInt128BitNot(lua_State* state) {
		boost::multiprecision::int128_t* const value = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		*LuaPushInt128(state) = ~*value;
		return 1;
	}
	int Lua::LuaInt128ToString(lua_State* state) {
		boost::multiprecision::int128_t* const value = static_cast<boost::multiprecision::int128_t*>(luaL_checkudata(state, -1, "Int128"));
		lua_pushstring(state, boost::lexical_cast<std::string>(*value).c_str());
		return 1;
	}
	int Lua::LuaInt128ToInt128(lua_State* state) {
		const boost::multiprecision::int128_t value(luaL_checkstring(state, -1));
		*LuaPushInt128(state) = std::move(value);
		return 1;
	}
#endif
}