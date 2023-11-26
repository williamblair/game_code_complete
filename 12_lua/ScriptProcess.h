#ifndef SCRIPT_PROCESS_H_INCLUDED
#define SCRIPT_PROCESS_H_INCLUDED

#include <LuaPlus/LuaPlus.h>
#include <GCC4/Process.h>

class ScriptProcess : public Process
{
public:

    // called once during application initialization
    static void RegisterScriptClass();

protected:
    
    unsigned long m_frequency;
    unsigned long m_time;
    LuaPlus::LuaObject m_scriptInitFunc, m_scriptUpdateFunc;
    LuaPlus::LuaObject m_scriptSuccessFunc, m_scriptFailFunc;
    LuaPlus::LuaObject m_scriptAbortFunc;
    LuaPlus::LuaObject m_self;

    // Process interface
    virtual void VOnInit();
    virtual void VOnUpdate(uint64_t deltaMs);
    virtual void VOnSuccess();
    virtual void VOnFail();
    virtual void VOnAbort();

private:

    static void RegisterScriptClassFunctions(
        LuaPlus::LuaObject& metaTableObj
    );
    static LuaPlus::LuaObject CreateFromScript(
        LuaPlus::LuaObject self,
        LuaPlus::LuaObject constructionData,
        LuaPlus::LuaObject originalSubClass
    );
    virtual bool VBuildCppDataFromScript(
        LuaPlus::LuaObject scriptClass,
        LuaPlus::LuaObject constructionData
    );

    // base class versions are const but LuaPlus can't use const functions
    bool ScriptIsAlive() { return IsAlive(); }
    bool ScriptIsDead() { return IsDead(); }
    bool ScriptIsPaused() { return IsPaused(); }

    void ScriptAttachChild(LuaPlus::LuaObject child);

    // disallow public construction
    explicit ScriptProcess();

    // used internally so Lua can instantiate it
    static ScriptProcess* Create(
        const char* scriptName = nullptr
    );
    static void Destroy(ScriptProcess* pObj);
};

#endif // SCRIPT_PROCESS_H_INCLUDED

