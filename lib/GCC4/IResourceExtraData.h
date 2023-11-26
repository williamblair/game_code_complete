#ifndef GCC4_IRESOURCE_EXTRA_DATA_H_INCLUDED
#define GCC4_IRESOURCE_EXTRA_DATA_H_INCLUDED

#include <string>

class IResourceExtraData
{
public:
    virtual std::string VToString() = 0;
};

#endif // GCC4_IRESOURCE_EXTRA_DATA_H_INCLUDED

