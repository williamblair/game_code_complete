#ifndef XML_H_INCLUDED
#define XML_H_INCLUDED

#include <tinyxml2.h>

using namespace tinyxml2;

namespace XmlResourceLoader
{

XMLElement* LoadAndReturnRootXmlElement( const char* filename );

} // namespace XmlResourceLoader


#endif // XML_H_INCLUDED

