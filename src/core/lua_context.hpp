#pragma once
#ifndef TME_LUA_CONTEXT
#define TME_LUA_CONTEXT

#include <optional>
#include <string>

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

    template <typename T>
    std::optional<T> read(const int index = -1);

    int fetch_global(const std::string& name)
    {
        return lua_getglobal(m_lua_state, name.c_str());
    }

    bool execute(const std::string& string)
    {
        int  result = luaL_dostring(m_lua_state, string.c_str());
        auto ok     = result != LUA_OK;
        set_error(ok);
        return ok;
    }

    void set_error(bool result)
    {
        m_error_occured = result;
    }

    std::optional<std::string> get_last_error()
    {
        if (m_error_occured)
            return std::string(lua_tostring(m_lua_state, -1));
        return {};
    }

  private:
    lua_State* m_lua_state;
    bool       m_error_occured;
};

template <>
inline std::optional<float> LuaContext::read<float>(const int index)
{
    if (!lua_isnumber(m_lua_state, index))
        return {};
    return static_cast<float>(lua_tonumber(m_lua_state, index));
}

template <>
inline std::optional<double> LuaContext::read<double>(const int index)
{
    if (!lua_isnumber(m_lua_state, index))
        return {};
    return static_cast<double>(lua_tonumber(m_lua_state, index));
}

template <>
inline std::optional<std::string> LuaContext::read<std::string>(const int index)
{
    if (!lua_isstring(m_lua_state, index))
        return {};
    return static_cast<std::string>(lua_tostring(m_lua_state, index));
}

#endif /* TME_LUA_CONTEXT */