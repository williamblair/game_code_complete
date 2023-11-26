#ifndef LUA_STATE_MANAGER_H_INCLUDED
#define LUA_STATE_MANAGER_H_INCLUDED

#include <string>
#include <cassert>

#include <LuaPlus/LuaPlus.h>
//#include "Vec3.h"
#include <GCC4/GCCMath.h>

class IScriptManager
{
public:
    virtual bool VInit() = 0;
    virtual void VExecuteFile(const char* resource) = 0;
    virtual void VExecuteString(const char* str) = 0;
};

class LuaStateManager : public IScriptManager
{
public:

    static bool Create();
    static void Destroy();
    static LuaStateManager* GetInstance() {
        assert(s_pSingleton != nullptr);
        return s_pSingleton;
    }

    // IScriptManager interface
    virtual bool VInit() override;
    virtual void VExecuteFile(const char* resource) override;
    virtual void VExecuteString(const char* str) override;

    LuaPlus::LuaObject GetGlobalVars() { return m_pLuaState->GetGlobals(); }
    LuaPlus::LuaState* GetLuaState() const { return m_pLuaState; }

    // public helpers
    LuaPlus::LuaObject CreatePath(
        const char* pathString,
        bool toIgnoreLastElement = false
    );
    void ConvertVec3ToTable(
        const Vec3& vec,
        LuaPlus::LuaObject& outLuaTable
    ) const;
    void ConvertTableToVec3(
        const LuaPlus::LuaObject& luaTable,
        Vec3& outVec3
    ) const;

private:

    static LuaStateManager* s_pSingleton; // singleton instance
    LuaPlus::LuaState* m_pLuaState;
    std::string m_lastError;

    void SetError(int errorNum);
    void ClearStack();

    // private singleton constructor/destructor; use Create() and Destroy() instead
    explicit LuaStateManager();
    virtual ~LuaStateManager();
};

#endif // LUA_STATE_MANAGER_H_INCLUDED

