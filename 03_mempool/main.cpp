#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <map>

#include <GCC4/MemoryPool.h>
#include <GCC4/ResCache.h>
#include <GCC4/GameCodeApp.h>

// Required globals - TODO move
ResCache* g_ResCache = nullptr;
class TestGCCApp : public GameCodeApp
{
public:
    virtual const char* VGetGameTitle() { return "TestGCCApp"; }
    virtual const char* VGetGameAppDirectory() { return "./"; }
};
TestGCCApp g_TestGCCApp;
GameCodeApp* g_pApp = &g_TestGCCApp;

static const size_t CHUNK_SIZE = sizeof(uint64_t); // each allocated entry will be 8 bytes
static const size_t NUM_CHUNKS = 10; // there will be memory available for 10 chunks at a time

int main(void)
{
    MemoryPool memPool;
    std::map<void*,uint64_t> allocMap;

    srand(time(0));

    if (!memPool.Init(CHUNK_SIZE, NUM_CHUNKS)) {
        printf("Failed to init mem pool\n");
        return 1;
    }

    // allocate until memory pool is full
    void* curAlloc = nullptr;
    while ((curAlloc = memPool.Alloc()) != nullptr) {
        uint64_t* pVal = (uint64_t*)curAlloc;
        *pVal = rand() % 0xffff;
        allocMap[curAlloc] = *pVal;
    }

    // display contents
    for (auto& pair : allocMap) {
        printf("0x%08X: %lu\n", pair.first, pair.second);
    }
    printf("\n");

    // free from pool
    while (allocMap.size() > 0) {
        auto pairIter = allocMap.begin();
        uint8_t** ppChunkHeader = (uint8_t**)(
            (uint8_t*)pairIter->first - 
            sizeof(uint8_t*)
        );
        uint8_t* pNextListEntry = ppChunkHeader[0];
        printf("Freeing 0x%08X (chunk header = 0x%08X)\n", 
            pairIter->first, (void*)ppChunkHeader);
        printf("  Points to: 0x%08X\n", (void*)pNextListEntry);
            memPool.Free(pairIter->first);
        allocMap.erase(pairIter);
    }

    return 0;
}

