#pragma once

#include <string_view>

#include <lua/lua.hpp>

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
		
		void GetGlobal(const std::string& name);
		void Push(long long number);
		long long PopInteger();
		void Call(int paramSize, int retSize);
	};
}