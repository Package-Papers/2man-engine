#pragma once
#ifndef TME_LUA_CONTEXT 
#define TME_LUA_CONTEXT

#include <string>
#include <optional>

#include <lua.hpp>

class LuaContext
{
public:
  explicit LuaContext()
  : m_lua_state(luaL_newstate())
  {
  }

  ~LuaContext()
  {
    lua_close(m_lua_state);
  }

  std::optional<std::string> get_string_global(const std::string& name)
  {
    auto _ = fetch_global(name);
    if (!lua_isstring(m_lua_state, -1))
    {
      set_error(true);
      return {};
    }

    return std::string(lua_tostring(m_lua_state, -1));
  }
  std::optional<double> get_double_global(const std::string& name)
  {
    auto _ = fetch_global(name);
    if (!lua_isnumber(m_lua_state, -1))
    {
      set_error(true);
      return {};
    }

    return static_cast<double>(lua_tonumber(m_lua_state, -1));
  }

  std::optional<float> get_float_global(const std::string& name)
  {
    auto _ = fetch_global(name);
    if (!lua_isnumber(m_lua_state, -1))
    {
      set_error(true);
      return {};
    }

    return static_cast<float>(lua_tonumber(m_lua_state, -1));
  }

  int fetch_global(const std::string& name)
  {
    return lua_getglobal(m_lua_state, name.c_str());
  }
  

  bool execute(const std::string& string)
  {
    int result = luaL_dostring(m_lua_state, string.c_str());
    auto ok = result != LUA_OK;
    set_error(ok);
    return ok;
  }

  void set_error(bool result)
  {
    m_error_occured = result;
  }

  std::optional<std::string> get_last_error()
  {
    if (m_error_occured) return std::string(lua_tostring(m_lua_state, -1));
    return {};
  }

private:
  lua_State* m_lua_state;
  bool m_error_occured;
};

#endif /* TME_LUA_CONTEXT */