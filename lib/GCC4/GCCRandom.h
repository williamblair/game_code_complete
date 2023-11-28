#ifndef GCC4_RANDOM_H_INCLUDED
#define GCC4_RANDOM_H_INCLUDED

#include <cstdint>
#include <climits>

class GCCRandom
{
public:

    GCCRandom();

    /**
     * Returns a number from [0,n) exclusive
     */
    uint32_t Random(uint32_t n);

    /**
     * Return a number from [0,1) exclusive 
     */
    float Random();

    
    /**
     * Seed the value generation with a specified value
     */
    void SetRandomSeed(uint32_t n);

    /**
     * Returns the seed that was used to initialize the generation
     */
    uint32_t GetRandomSeed();

    /**
     * Default seed generation with time(0)
     */ 
    void Randomize();

private:

    static const uint64_t CMATH_M = 397;
    static const uint64_t CMATH_N = 624;
    static const uint64_t CMATH_MATRIX_A = 0x9908b0df;   // constant vector a
    static const uint64_t CMATH_UPPER_MASK = 0x80000000; // MSB w-r bits
    static const uint64_t CMATH_LOWER_MASK = 0x7FFFFFFF; // LSB r bits
    static const uint64_t CMATH_TEMPERING_MASK_B = 0x9d2c5680;
    static const uint64_t CMATH_TEMPERING_MASK_C = 0xefc60000;

    inline uint64_t CMATH_TEMPERING_SHIFT_U(uint64_t y) { return y >> 11; }
    inline uint64_t CMATH_TEMPERING_SHIFT_S(uint64_t y) { return y << 7; }
    inline uint64_t CMATH_TEMPERING_SHIFT_T(uint64_t y) { return y >> 15; }
    inline uint64_t CMATH_TEMPERING_SHIFT_L(uint64_t y) { return y >> 18; }

    uint32_t m_seed;
    uint32_t m_seedSP;
    uint64_t m_stateArray[CMATH_N];
    int m_mti; // m_mti == N+1 means stateArray[N] is uninitialized
};

#endif // GCC4_RANDOM_H_INCLUDED

