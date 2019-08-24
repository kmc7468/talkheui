#pragma once

#include <string_view>

#include <lua/lua.hpp>

#ifdef TH_USE_MULTIPRECISION
#	include <boost/multiprecision/cpp_int.hpp>
#endif

namespace th {
	class Lua final {
	private:
		lua_State* m_State = nullptr;

	public:
		Lua();
		Lua(Lua&& lua) noexcept;
		~Lua();

	public:
		Lua& operator=(Lua&& lua) noexcept;

	public:
		void LoadScript(const std::string_view& script);
		
		bool GetGlobal(const std::string& name);
		void SetGlobal(const std::string& name);
		void Push(long long number);
		void Push(const char* string);
#ifdef TH_USE_MULTIPRECISION
		void Push(boost::multiprecision::int128_t number);
#endif
		void Push(bool boolean);
		void Push(lua_CFunction function);
		long long PopInteger();
#ifdef TH_USE_MULTIPRECISION
		boost::multiprecision::int128_t PopInteger128();
#endif
		void Call(int paramSize, int retSize);

		void CreateTable(int elements);
		void SetTable(int index);

#ifdef TH_USE_MULTIPRECISION
		void AddInt128Class();
#endif

	private:
		static int LuaIsInt128Available(lua_State* state);
#ifdef TH_USE_MULTIPRECISION
		static boost::multiprecision::int128_t* LuaPushInt128(lua_State* state);
		static int LuaInt128New(lua_State* state);
		static int LuaInt128Print(lua_State* state);
		static int LuaInt128PrintLn(lua_State* state);
		static int LuaInt128Read(lua_State* state);
		static int LuaInt128Add(lua_State* state);
		static int LuaInt128Sub(lua_State* state);
		static int LuaInt128Mul(lua_State* state);
		static int LuaInt128Div(lua_State* state);
		static int LuaInt128Mod(lua_State* state);
		static int LuaInt128Equal(lua_State* state);
		static int LuaInt128NotEqual(lua_State* state);
		static int LuaInt128Greater(lua_State* state);
		static int LuaInt128GreaterEqual(lua_State* state);
		static int LuaInt128Less(lua_State* state);
		static int LuaInt128LessEqual(lua_State* state);
		static int LuaInt128BitAnd(lua_State* state);
		static int LuaInt128BitOr(lua_State* state);
		static int LuaInt128BitXor(lua_State* state);
		static int LuaInt128BitLeftShift(lua_State* state);
		static int LuaInt128BitRightShift(lua_State* state);
#endif
	};
}