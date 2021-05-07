#include <GCCRandom.h>
#include <cstdio>

int main(void)
{
    GCCRandom randGen;
    randGen.Randomize(); // init seed

    // gen some random values
    for ( int i = 0; i < 10; ++i )
    {
        printf("Rand val %d: %u\n", i, randGen.Random( 100 ));
    }

    return 0;
}

