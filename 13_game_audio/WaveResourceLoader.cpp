#include <iostream>
#include <cstring>
#include "SoundResourceExtraData.h"
#include "WaveResourceLoader.h"

// converts four characters into a 4byte integer
inline constexpr int32_t mmioFOURCC( char a, char b, char c, char d )
{
    return int32_t( (a << 24) |
                    (b << 16) |
                    (c << 8)  |
                    (d) );
}

bool WaveResourceLoader::ParseWave( char* wavStream,
                                    size_t bufferLength,
                                    std::shared_ptr<ResHandle> handle )
{
    std::shared_ptr<SoundResourceExtraData> extra = 
        std::static_pointer_cast<SoundResourceExtraData>( handle->GetExtra() );

    int32_t file = 0;
    int32_t fileEnd = 0;
    int32_t length = 0;
    int32_t type = 0;
    int32_t pos = 0; // current index into the buffered file

    type = *( (int32_t*)( wavStream + pos ));
    pos += sizeof(int32_t);
    if ( type != mmioFOURCC( 'R', 'I', 'F', 'F' ) )
    {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
                  << "type != RIFF" << std::endl;
        return false;
    }

    length = *( (int32_t*)( wavStream + pos ));
    pos += sizeof(int32_t);
    
    type = *( (int32_t*)( wavStream + pos ));
    pos += sizeof(int32_t);

    if ( type != mmioFOURCC( 'W', 'A', 'V', 'E' ) )
    {
        std::cout << __FILE__ << ":" << __LINE__ << ": "
                  << "type != WAVE" << std::endl;
        return false;
    }

    fileEnd = length - 4;

    // TODO
    //memset( &extra->m_WavFormatEx, 0, sizeof(WAVEFORMATEX) );

    bool copiedBuffer = false;

    while ( file < fileEnd )
    {
        type = *( (int32_t*)( wavStream + pos ));
        pos += sizeof(int32_t);
        file += sizeof(int32_t);

        length = *( (int32_t*)( wavStream + pos ));
        pos += sizeof(int32_t);
        file += sizeof(int32_t);

        switch ( type )
        {
        case mmioFOURCC( 'f', 'a', 'c', 't' ):
        {
            std::cout << __FILE__ << ":" << __LINE__ << ": "
                      << "don't handle compressed wav files" << std::endl;
            break;
        }
        case mmioFOURCC( 'f', 'm', 't', ' ' ):
        {
            // TODO
            //memcpy( &extra->m_WavFormatEx, wavStream+pos, length );
            //pos += length;
            //extra->m_WaveFormatEx.cbSize = (int16_t)length;
            break;
        }
        case mmioFOURCC( 'd', 'a', 't', 'a' ):
        {
            copiedBuffer = true;
            if ( length != handle->Size() )
            {
                std::cout << __FILE__ << ":" << __LINE__ << ": "
                          << "wave resource size != buffer size" << std::endl;
                return 0;
            }
            memcpy( handle->WriteableBuffer(), wavStream+pos, length );
            pos += length;
            break;
        }
        } // end switch ( type )

        file += length;

        // if both blocks have been seen, we can return true
        if ( copiedBuffer )
        {
            //extra->m_LengthMilli =
            //    (handle->GetSize() * 1000) / extra->GetFormat()->nAvgBytesPerSec;
            return true;
        }

        // increment pointer past block we just read,
        // and make sure the pointer is word aligned
        if ( length & 1 )
        {
            ++pos;
            ++file;
        }
    }

    // should not reach here; meaning .wav file didn't contain everything
    return false;
}

