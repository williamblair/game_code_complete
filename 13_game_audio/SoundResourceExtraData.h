#ifndef SOUND_RESOURCE_EXTRA_DATA_H_INCLUDED
#define SOUND_RESOURCE_EXTRA_DATA_H_INCLUDED

#include <string>

#include <GCC4/ResHandle.h>

#include "SoundType.h"


class SoundResourceExtraData : public IResourceExtraData
{

friend class WaveResourceLoader;
friend class OggResourceLoader;

public:

    SoundResourceExtraData();
    virtual ~SoundResourceExtraData();

    virtual std::string VToString() { return "SoundResourceExtraData"; }

    SoundType GetSoundType() const { return m_SoundType; }

    // TODO - this is directX specific
    //WAVEFORMATEX const* GetFormat() { return &m_WaveFormatEx; }

private:

    SoundType m_SoundType; // OGG, WAVE, etc.
    bool m_bInitialized;
    //WAVEFORMATEX m_WaveFormatEx;
    int m_LengthMilli; // how long the sound is in milliseconds
};

#endif // SOUND_RESOURCE_EXTRA_DATA_H_INCLUDED

