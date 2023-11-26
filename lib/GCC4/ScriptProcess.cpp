#include <memory>
#include "ScriptProcess.h"
#include "LuaStateManager.h"

#define SCRIPT_PROCESS_NAME "ScriptProcess"


ScriptProcess::ScriptProcess()
{

    LuaPlus::LuaState* pState =
        LuaStateManager::GetInstance()->GetLuaState();

    m_frequency = 0;
    m_time = 0;
    m_scriptInitFunc.AssignNil(pState);
    m_scriptUpdateFunc.AssignNil(pState);
    m_scriptSuccessFunc.AssignNil(pState);
    m_scriptFailFunc.AssignNil(pState);
    m_scriptAbortFunc.AssignNil(pState);
}

void ScriptProcess::RegisterScriptClass()
{
    LuaPlus::LuaObject metaTableObj =
        LuaStateManager::GetInstance()->GetGlobalVars()
            .CreateTable(SCRIPT_PROCESS_NAME);

    metaTableObj.SetObject("__index", metaTableObj);
    metaTableObj.SetObject("base", metaTableObj);
    metaTableObj.SetBoolean("cpp", true);

    RegisterScriptClassFunctions(metaTableObj);

    metaTableObj.RegisterDirect("Create", &ScriptProcess::CreateFromScript);
}

void
ScriptProcess::RegisterScriptClassFunctions( 
    LuaPlus::LuaObject& metaTableObj)
{
    metaTableObj.RegisterObjectDirect("Succeed", (Process*)0, &Process::Succeed);
    metaTableObj.RegisterObjectDirect("Fail", (Process*)0, &Process::Fail);
    metaTableObj.RegisterObjectDirect("Pause", (Process*)0, &Process::Pause);
    metaTableObj.RegisterObjectDirect("UnPause", (Process*)0, &Process::UnPause);

    metaTableObj.RegisterObjectDirect("IsAlive", (ScriptProcess*)0, &ScriptProcess::ScriptIsAlive);
    metaTableObj.RegisterObjectDirect("IsDead", (ScriptProcess*)0, &ScriptProcess::ScriptIsDead);
    metaTableObj.RegisterObjectDirect("IsPaused", (ScriptProcess*)0, &ScriptProcess::ScriptIsPaused);
    metaTableObj.RegisterObjectDirect("AttachChild", (ScriptProcess*)0, &ScriptProcess::ScriptAttachChild);
}

void ScriptProcess::ScriptAttachChild(LuaPlus::LuaObject child)
{
    if (child.IsTable())
    {
        LuaPlus::LuaObject obj = child.GetByName("__object");
        if (!obj.IsNil())
        {
            // Lua doesn't know what a smart pointer is so we have to cast
            // a raw pointer
            std::shared_ptr<Process> pProcess(
                static_cast<Process*>(obj.GetLightUserdata())
            );
            assert(pProcess);
            AttachChild(pProcess);
        }
    }
    else
    {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
            << "Invalid object type: " << child.TypeName() << std::endl; 
        // TODO - not assert
        assert(false);
    }
}

LuaPlus::LuaObject 
ScriptProcess::CreateFromScript(
    LuaPlus::LuaObject self,
    LuaPlus::LuaObject constructionData,
    LuaPlus::LuaObject originalSubClass)
{
    // the self parameter isn't used in this function but is used in the Lua
    // version of the function
    ScriptProcess* pObj = new ScriptProcess;

    pObj->m_self.AssignNewTable(LuaStateManager::GetInstance()->GetLuaState());
    if (pObj->VBuildCppDataFromScript(originalSubClass, constructionData))
    {
        LuaPlus::LuaObject metaTableObj =
            LuaStateManager::GetInstance()->GetGlobalVars().Lookup(SCRIPT_PROCESS_NAME);
        assert(!metaTableObj.IsNil());

        pObj->m_self.SetLightUserdata("__object", pObj);
        pObj->m_self.SetMetatable(metaTableObj);
    }
    else
    {
        pObj->m_self.AssignNil(LuaStateManager::GetInstance()->GetLuaState());
        delete pObj;
    }

    return pObj->m_self;
}

bool ScriptProcess::VBuildCppDataFromScript(
    LuaPlus::LuaObject scriptClass,
    LuaPlus::LuaObject constructionData)
{
    if (scriptClass.IsTable())
    {
        // OnInit()
        LuaPlus::LuaObject tmp = scriptClass.GetByName("OnInit");
        if (tmp.IsFunction()) {
            m_scriptInitFunc = tmp;
        }

        // OnUpdate()
        tmp = scriptClass.GetByName("OnUpdate");
        if (tmp.IsFunction()) {
            m_scriptUpdateFunc = tmp;
        } else {
            std::cout << __FILE__ << ":" << __LINE__ << ": "
                << "No OnUpdate() found in script process" << std::endl;
            return false;
        }

        // OnSuccess
        tmp = scriptClass.GetByName("OnSuccess");
        if (tmp.IsFunction()) {
            m_scriptSuccessFunc = tmp;
        }

        // OnAbort
        tmp = scriptClass.GetByName("OnAbort");
        if (tmp.IsFunction()) {
            m_scriptAbortFunc = tmp;
        }
    }
    else
    {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
            << "scriptClass is not a table" << std::endl;
        return false;
    }

    if (constructionData.IsTable())
    {
        for (LuaPlus::LuaTableIterator it(constructionData);
              it;
              it.Next())
        {
            const char* key = it.GetKey().GetString();
            LuaPlus::LuaObject val = it.GetValue();

            if (strcmp(key, "frequency") == 0 && val.IsInteger()) {
                m_frequency = val.GetInteger();
            } else {
                m_self.SetObject(key, val);
            }
        }
    }

    return true;
}

void ScriptProcess::VOnInit()
{
    Process::VOnInit();
    if (!m_scriptInitFunc.IsNil())
    {
        //LuaPlus::LuaFunction<void> func( m_scriptInitFunc );
        LuaPlus::LuaFunctionVoid func(m_scriptInitFunc);
        func(m_self);
    }
}

void ScriptProcess::VOnSuccess()
{
    if (!m_scriptSuccessFunc.IsNil())
    {
        LuaPlus::LuaFunctionVoid func(m_scriptSuccessFunc);
        func(m_self);
    }
}

void ScriptProcess::VOnFail()
{
    if (!m_scriptFailFunc.IsNil())
    {
        LuaPlus::LuaFunctionVoid func(m_scriptFailFunc);
        func(m_self);
    }
}

void ScriptProcess::VOnAbort()
{
    if (!m_scriptAbortFunc.IsNil())
    {
        LuaPlus::LuaFunctionVoid func(m_scriptAbortFunc);
        func(m_self);
    }
}

void ScriptProcess::VOnUpdate(uint64_t deltaMs)
{
    m_time += deltaMs;
    if (m_time >= m_frequency)
    {
        LuaPlus::LuaFunctionVoid func(m_scriptUpdateFunc);
        func(m_self, m_time);
        m_time = 0;
    }
}


