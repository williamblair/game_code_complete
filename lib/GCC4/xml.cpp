#include <memory>
#include <iostream>
#include <vector>
#include "xml.h"
#include "GameCodeApp.h"
#include "Logger.h"

using namespace tinyxml2;

void XmlResourceExtraData::ParseXml(char* pRawBuffer)
{
    assert(pRawBuffer);
    m_xmlDocument.Parse(pRawBuffer);
}


bool XmlResourceLoader::VLoadResource(
    char* rawBuffer,
    unsigned int rawSize,
    std::shared_ptr<ResHandle> handle)
{
    if (rawSize == 0) {
        GCC_ERROR("Resource raw size is zero");
        return false;
    }

    std::shared_ptr<XmlResourceExtraData> pExtraData =
        std::shared_ptr<XmlResourceExtraData>(new XmlResourceExtraData);
    pExtraData->ParseXml(rawBuffer);
    handle->SetExtra(pExtraData);
    return true;
}

XMLElement* XmlResourceLoader::LoadAndReturnRootXmlElement(const char* filename)
{
    Resource resource(filename);
    // actually loads xml from the zip file
    std::shared_ptr<ResHandle> pResourceHandle =
        g_pApp->m_pResCache->GetHandle(&resource);
    std::shared_ptr<XmlResourceExtraData> pExtraData =
        std::static_pointer_cast<XmlResourceExtraData>(
            pResourceHandle->GetExtra()
        );
    return pExtraData->GetRoot();
}

std::shared_ptr<IResourceLoader> CreateXmlResourceLoader()
{
    return std::shared_ptr<IResourceLoader>(new XmlResourceLoader);
}

