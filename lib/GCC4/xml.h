#ifndef GCC4_XML_H_INCLUDED
#define GCC4_XML_H_INCLUDED

#include "tinyxml2.h"
#include "IResourceExtraData.h"
#include "IResourceLoader.h"

class XmlResourceExtraData : public IResourceExtraData
{
public:
    virtual std::string VToString() { return "XmlResourceExtraData"; }
    void ParseXml(char* pRawBuffer);
    tinyxml2::XMLElement* GetRoot() { return m_xmlDocument.RootElement(); }
private:
    tinyxml2::XMLDocument m_xmlDocument;
};

class XmlResourceLoader : public IResourceLoader
{
public:
    virtual bool VUseRawFile() { return false; }
    virtual bool VDiscardRawBufferAfterLoad() { return true; }
    virtual unsigned int VGetLoadedResourceSize(char* rawBuffer, unsigned int rawSize) {
        return rawSize;
    }
    virtual bool VLoadResource(char* rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> handle);
    virtual std::string VGetPattern() { return ".*\\.xml$"; }

    // convenience function
    static tinyxml2::XMLElement* LoadAndReturnRootXmlElement(const char* resourceString);
};

std::shared_ptr<IResourceLoader> CreateXmlResourceLoader();

#endif // GCC4_XML_H_INCLUDED

