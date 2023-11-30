#include "ScriptResourceLoader.h"

std::shared_ptr<IResourceLoader> CreateScriptResourceLoader()
{
    return std::make_shared<ScriptResourceLoader>();
}

