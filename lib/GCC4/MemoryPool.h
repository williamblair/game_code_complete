#ifndef GCC4_MEMORY_POOL_H_INCLUDED
#define GCC4_MEMORY_POOL_H_INCLUDED

#include <cstdint>
#include <cstddef>

#include "Logger.h"

class MemoryPool
{
public:

    MemoryPool();
    ~MemoryPool();

    /**
     * create the internal memory pool
     */
    bool Init(size_t chunkSize, size_t numChunks);

    /**
     * free the internal memory pool memory
     */
    void Destroy();

    /**
     * Get a chunk of memory (size = chunkSize)
     */
    void* Alloc();

    /**
     * release a chunk of memory previously obtained from Alloc()
     */
    void Free(void* pMem);

    /**
     * Get the configured size of each chunk
     */ 
    size_t GetChunkSize() const { return m_chunkSize; }

private:

    // no copying!
    MemoryPool(const MemoryPool& memPool);

    uint8_t** m_ppRawMemory; // array of memory blocks, eaching have a single uint8_t header
    uint8_t* m_pHead;       // front of the memory chunk linked list
    size_t m_chunkSize;   // number of bytes in each chunk
    size_t m_numChunks;   // number of chunks in the memory pool
    size_t m_arraySize;   // number of memory blocks

    void Reset();
    bool GrowMemoryArray();
    uint8_t* AllocateNewMemoryBlock();

    uint8_t* GetNext(uint8_t* pBlock); // traverse and find next linked list entry
    void SetNext(uint8_t* pBlockToChange, uint8_t* pNewNext); 
  
};

#define SAFE_DELETE(ptr) if ((ptr)) { delete (ptr); }

//---------------------------------------------------------------------------------------------------------------------
// These macros are designed to allow classes to easily take advantage of memory pools.  To use, follow this steps:
// 1) Call GCC_MEMORYPOOL_DECLARATION() in the class declaration
// 2) Call GCC_MEMORYPOOL_DEFINITION() in the cpp file
// 3) Call GCC_MEMORYPOOL_AUTOINIT() or GCC_MEMORYPOOL_AUTOINIT_DEBUGNAME() in the cpp file
// 
// That's it!  Objects of your class will now be allocated through the memory pool!  You can see an example of its 
// usage in Pathing.h and Pathing.cpp.  Check out the PathingNode class.
//---------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------
// This macro is placed inside the body of the class that you want to use a memory pool with.  It declares the 
// overloaded new and delete operators as well as the static MemoryPool object.
// 
// IMPORTANT: InitMemoryPool() and DestroyMemoryPool() must be called manually unless you use the GCC_MEMORYPOOL_AUTOINIT()
// macro below.
//---------------------------------------------------------------------------------------------------------------------
#define GCC_MEMORYPOOL_DECLARATION(__defaultNumChunks__) \
    public: \
        static MemoryPool* s_pMemoryPool; \
        static void InitMemoryPool(unsigned int numChunks = __defaultNumChunks__, const char* debugName = 0); \
        static void DestroyMemoryPool(void); \
        static void* operator new(size_t size); \
        static void operator delete(void* pPtr); \
        static void* operator new[](size_t size); \
        static void operator delete[](void* pPtr); \
    private: \


//---------------------------------------------------------------------------------------------------------------------
// This macro defines the definition for the overloaded new & delete operators on a class meant to be pooled with a
// memory pool.  It is meant to work specifically with the MemoryPool class.  To use it, call this macro from the cpp
// file where your class function definitions are.
//    - _className_:        The name of this class.
//---------------------------------------------------------------------------------------------------------------------
#define GCC_MEMORYPOOL_DEFINITION(_className_) \
    MemoryPool* _className_::s_pMemoryPool = NULL;\
    void _className_::InitMemoryPool(unsigned int numChunks, const char* debugName) \
    { \
        if (s_pMemoryPool != NULL) \
        { \
            GCC_ERROR("s_pMemoryPool is not NULL.  All data will be destroyed.  (Ignorable)"); \
            SAFE_DELETE(s_pMemoryPool); \
        } \
        s_pMemoryPool = new MemoryPool; \
        /*if (debugName)*/ \
        /*    s_pMemoryPool->SetDebugName(debugName);*/ \
        /*else*/ \
        /*    s_pMemoryPool->SetDebugName(#_className_);*/ \
        s_pMemoryPool->Init(sizeof(_className_), numChunks); \
    } \
    void _className_::DestroyMemoryPool(void) \
    { \
        GCC_ASSERT(s_pMemoryPool != NULL); \
        SAFE_DELETE(s_pMemoryPool); \
    } \
    void* _className_::operator new(size_t size) \
    { \
        GCC_ASSERT(s_pMemoryPool); \
        void* pMem = s_pMemoryPool->Alloc(); \
        return pMem; \
    } \
    void _className_::operator delete(void* pPtr) \
    { \
        GCC_ASSERT(s_pMemoryPool); \
        s_pMemoryPool->Free(pPtr); \
    } \
    void* _className_::operator new[](size_t size) \
    { \
        GCC_ASSERT(s_pMemoryPool); \
        void* pMem = s_pMemoryPool->Alloc(); \
        return pMem; \
    } \
    void _className_::operator delete[](void* pPtr) \
    { \
        GCC_ASSERT(s_pMemoryPool); \
        s_pMemoryPool->Free(pPtr); \
    } \


//---------------------------------------------------------------------------------------------------------------------
// This macro defines a static class that automatically initializes a memory pool at global startup and destroys it at
// global destruction time.  Using this gets around the requirement of manually initializing and destroying the memory
// pool yourself.
//---------------------------------------------------------------------------------------------------------------------
#define GCC_MEMORYPOOL_AUTOINIT_DEBUGNAME(_className_, _numChunks_, _debugName_) \
class _className_ ## _AutoInitializedMemoryPool \
{ \
public: \
    _className_ ## _AutoInitializedMemoryPool(void); \
    ~_className_ ## _AutoInitializedMemoryPool(void); \
}; \
_className_ ## _AutoInitializedMemoryPool::_className_ ## _AutoInitializedMemoryPool(void) \
{ \
    _className_::InitMemoryPool(_numChunks_, _debugName_); \
} \
_className_ ## _AutoInitializedMemoryPool::~_className_ ## _AutoInitializedMemoryPool(void) \
{ \
    _className_::DestroyMemoryPool(); \
} \
static _className_ ## _AutoInitializedMemoryPool s_ ## _className_ ## _AutoInitializedMemoryPool; \

#define GCC_MEMORYPOOL_AUTOINIT(_className_, _numChunks_) GCC_MEMORYPOOL_AUTOINIT_DEBUGNAME(_className_, _numChunks_, #_className_)

#endif // GCC4_MEMORY_POOL_H_INCLUDED

