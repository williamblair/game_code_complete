#ifndef WAVE_RESOURCE_LOADER_H_INCLUDED
#define WAVE_RESOURCE_LOADER_H_INCLUDED

#include <cstdint>
#include <IResourceLoader.h>

class WaveResourceLoader : public IResourceLoader
{
public:

    virtual std::string VGetPattern() { return "*.wav"; }
    virtual bool VUseRawFile() { return false; }
    virtual unsigned int VGetLoadedResourceSize( char* pRawBuffer,
                                                 unsigned int rawSize );

    virtual bool VLoadResource( char* pRawBuffer,
                                unsigned int rawSize,
                                std::shared_ptr<ResHandle> handle );

protected:

    bool ParseWave( char* wavStream,
                    size_t bufferLength,
                    std::shared_ptr<ResHandle> handle );
};

#endif // WAVE_RESOURCE_LOADER_H_INCLUDED

