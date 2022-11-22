#include <GCCRandom.h>
#include <ctime>

GCCRandom::GCCRandom()
{
    m_seed = 1;
    m_seedSP = 0;
    m_mti = CMATH_N + 1;
}

uint32_t GCCRandom::Random( uint32_t n )
{
    uint64_t y;
    static uint64_t mag01[2] = { 0x0, CMATH_MATRIX_A };

    if ( n == 0 ) {
        return 0;
    }

    // Generate N words at one time
    uint64_t* mt = m_stateArray;
    if ( m_mti >= int(CMATH_N) )
    {
        int kk;

        if ( m_mti == CMATH_N + 1 ) {
            SetRandomSeed( 4357 ); // default initial seed
        }

        for ( kk = 0; kk < int(CMATH_N-CMATH_M); ++kk )
        {
            y = (mt[kk] & CMATH_UPPER_MASK) | 
                (mt[kk+1] & CMATH_LOWER_MASK);
            
            mt[kk] = mt[kk + CMATH_M] ^ 
                     (y >> 1)         ^ 
                     mag01[y & 0x1];
        }

        for ( ; kk < int(CMATH_N-1); ++kk )
        {
            y = (mt[kk] & CMATH_UPPER_MASK) | 
                (mt[kk+1] & CMATH_LOWER_MASK);
            
            mt[kk] = mt[kk + (CMATH_M - CMATH_N)] ^ 
                     (y >> 1)                     ^ 
                     mag01[y & 0x1];
        } 

        y = (mt[CMATH_N-1] & CMATH_UPPER_MASK) |
            (mt[0] & CMATH_LOWER_MASK);
        
        mt[CMATH_N - 1] = mt[CMATH_M - 1] ^
                          (y >> 1)        ^
                          mag01[y & 0x1];

        m_mti = 0;
    }

    y = mt[m_mti++];
    y ^= CMATH_TEMPERING_SHIFT_U( y );
    y ^= CMATH_TEMPERING_SHIFT_S( y ) & CMATH_TEMPERING_MASK_B;
    y ^= CMATH_TEMPERING_SHIFT_T( y ) & CMATH_TEMPERING_MASK_C;
    y ^= CMATH_TEMPERING_SHIFT_L( y );
    
    return y % n;
}

float GCCRandom::Random()
{
    float r = float( Random( INT_MAX ) );
    float divisor = float( INT_MAX );
    return ( r / divisor );
}

void GCCRandom::SetRandomSeed( uint32_t n )
{
    // Setting initial seeds to m_stateArray[N] using the generator Line 25
    // of Table 1 in [KNUTH 1981, The Art of Computer Programming
    //      Vol. 2 (2nd Ed), pp102]
    m_stateArray[0] = n & 0xffffffff;
    for ( m_mti = 1; m_mti < int(CMATH_N); ++m_mti )
    {
        m_stateArray[m_mti] = (69069 * m_stateArray[m_mti - 1]) & 0xffffffff;
    }

    m_seed = n;
}

uint32_t GCCRandom::GetRandomSeed()
{
    return m_seed;
}

void GCCRandom::Randomize()
{
    SetRandomSeed( (uint32_t)time(0) );
}

