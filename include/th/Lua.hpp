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
		void Push(long long number);
		void Push(const char* string);
#ifdef TH_USE_MULTIPRECISION
		void Push(boost::multiprecision::int128_t number);
#endif
		long long PopInteger();
#ifdef TH_USE_MULTIPRECISION
		boost::multiprecision::int128_t PopInteger128();
#endif
		void Call(int paramSize, int retSize);

		void CreateTable(int elements);
		void SetTable(int index);
	};
}