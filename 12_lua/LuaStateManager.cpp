#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <sstream>
#include "LuaStateManager.h"

LuaStateManager* LuaStateManager::s_pSingleton = nullptr;

bool LuaStateManager::Create()
{
    assert(s_pSingleton == nullptr);
    s_pSingleton = new LuaStateManager();
    if (!s_pSingleton) {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
                  << "failed to create lua state manager instance" << std::endl;
        return false;
    }
    return s_pSingleton->VInit();
}

void LuaStateManager::Destroy()
{
    assert(s_pSingleton != nullptr);
    delete s_pSingleton;
    s_pSingleton = nullptr;
}

LuaStateManager::LuaStateManager() :
    m_pLuaState(nullptr)
{
}

LuaStateManager::~LuaStateManager()
{
    if (m_pLuaState) {
        LuaPlus::LuaState::Destroy(m_pLuaState);
        m_pLuaState = nullptr;
    }
}

bool LuaStateManager::VInit()
{
    m_pLuaState = LuaPlus::LuaState::Create(true);
    if (m_pLuaState == nullptr) {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
                  << "Failed to create lua state" << std::endl;
        return false;
    }

    m_pLuaState->GetGlobals().RegisterDirect(
        "ExecuteFile",
        (*this),
        &LuaStateManager::VExecuteFile
    );
    m_pLuaState->GetGlobals().RegisterDirect(
        "ExecuteString",
        (*this),
        &LuaStateManager::VExecuteString
    );

    return true;
}

void LuaStateManager::VExecuteFile(const char* resource)
{
    int result = m_pLuaState->DoFile(resource);
    if (result != 0) {
        SetError(result);
    }
}

void LuaStateManager::VExecuteString(const char* str)
{
    int result = 0;

    // most strings directly passed to interpreter
    if (strlen(str) <= 1 || str[0] != '=')
    {
        result = m_pLuaState->DoString(str);
        if (result != 0) {
            SetError(result);
        }
    }

    // else if string starts with '=', wrap statement in print() func
    else
    {
        std::string buffer("print");
        buffer += (str + 1);
        buffer += ")";
        result = m_pLuaState->DoString(buffer.c_str());
        if (result != 0) {
            SetError(result);
        }
    }
}

void LuaStateManager::SetError(int errorNum)
{
    // note -luaplus throws an exception when an error occurs

    LuaPlus::LuaStackObject stackObj(m_pLuaState, -1);
    const char* errStr = stackObj.GetString();
    if (errStr) {
        m_lastError = errStr;
        ClearStack();
    } else {
        m_lastError = "Unknown Lua parse error";
    }

    // TODO - not assert; do exit or something
    std::cout << "Last error:" << m_lastError << std::endl;
    assert(false);
}

void LuaStateManager::ClearStack()
{
    m_pLuaState->SetTop(0);
}

LuaPlus::LuaObject
LuaStateManager::CreatePath(
    const char* pathString,
    bool toIgnoreLastElement)
{
    // split the input string by '.' character into array
    std::vector<std::string> splitPath;
    {
        int i = 0;
        const size_t len = strlen(pathString);
        do
        {
            std::string curToken = "";
            while (i < len && pathString[i] != '.')
            {
                curToken += pathString[i];
                ++i;
            }
            if (curToken.size() > 0) {
                splitPath.push_back(curToken);
            }
            ++i; // skip current . character
        } while (i < len);
    }
    if (toIgnoreLastElement) {
        splitPath.pop_back();
    }

    LuaPlus::LuaObject context = GetGlobalVars();
    for (auto it = splitPath.begin(); it != splitPath.end(); ++it)
    {
        if (context.IsNil()) {
            std::cout << __FILE__ << ":" << __LINE__ << ": "
                << "bailing out, element == " << *it << std::endl;
            return context;
        }

        const std::string& element = *it;
        LuaPlus::LuaObject curr = context.GetByName(element.c_str());

        if (!curr.IsTable())
        {
            if (!curr.IsNil())
            {
                std::cout << __FILE__ << ":" << __LINE__ << ": "
                    << "warning: Overwriting element " << element << std::endl;
                context.SetNil(element.c_str());
            }

            // object is either nil or was clobbered so add the new table
            context.CreateTable(element.c_str());
        }

        context = context.GetByName(element.c_str());
    }

    return context;
}

void
LuaStateManager::ConvertVec3ToTable(
    const Vec3& vec,
    LuaPlus::LuaObject& outLuaTable) const
{
    outLuaTable.AssignNewTable(GetLuaState());
    outLuaTable.SetNumber("x", vec.x);
    outLuaTable.SetNumber("y", vec.y);
    outLuaTable.SetNumber("z", vec.z);
}

void LuaStateManager::ConvertTableToVec3(
    const LuaPlus::LuaObject& luaTable,
    Vec3& outVec3) const
{
    LuaPlus::LuaObject tmp;

    // x
    tmp = luaTable.Get("x");
    if (tmp.IsNumber()) {
        outVec3.x = tmp.GetFloat();
    } else {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
            << "luaTable.x not a number" << std::endl;
    }

    // y
    tmp = luaTable.Get("y");
    if (tmp.IsNumber()) {
        outVec3.y = tmp.GetFloat();
    } else {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
            << "luaTable.y not a number" << std::endl;
    }
    
    // z
    tmp = luaTable.Get("z");
    if (tmp.IsNumber()) {
        outVec3.z = tmp.GetFloat();
    } else {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
            << "luaTable.z not a number" << std::endl;
    }
}

