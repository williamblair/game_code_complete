#include <memory>
#include <iostream>
#include <vector>
#include <xml.h>

#define DBG_PRINT 1

using namespace tinyxml2;

namespace XmlResourceLoader
{

static std::vector<std::shared_ptr<XMLDocument>> docs;

XMLElement* LoadAndReturnRootXmlElement( const char* filename )
{
    //std::shared_ptr<XMLDocument> xmlDoc( new XMLDocument );
    docs.push_back(std::shared_ptr<XMLDocument>( new XMLDocument ));
    XMLDocument* xmlDoc = docs[ docs.size()-1 ].get();
    xmlDoc->LoadFile( filename );
#if DBG_PRINT
    {
        std::cout << __func__ << ": loaded xml file: " 
                  << filename << std::endl;
        xmlDoc->Print();
    }
#endif
    return xmlDoc->RootElement();
}

} // namespace XmlResourceLoader
