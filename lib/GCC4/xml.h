#ifndef GCC4_XML_H_INCLUDED
#define GCC4_XML_H_INCLUDED

#include "tinyxml2.h"

//using namespace tinyxml2;

namespace XmlResourceLoader
{

tinyxml2::XMLElement* LoadAndReturnRootXmlElement( const char* filename );

} // namespace XmlResourceLoader


#endif // GCC4_XML_H_INCLUDED

