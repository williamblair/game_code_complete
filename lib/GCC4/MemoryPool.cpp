#include "MemoryPool.h"
#include <cstdlib>

// next linked list entry pointer size
const static size_t CHUNK_HEADER_SIZE = sizeof(uint8_t*);

MemoryPool::MemoryPool() :
    m_ppRawMemory(nullptr),
    m_pHead(nullptr),
    m_chunkSize(0),
    m_numChunks(0),
    m_arraySize(0)
{
    Reset();
}

MemoryPool::~MemoryPool()
{
    Destroy();
}

bool MemoryPool::Init(size_t chunkSize, size_t numChunks)
{
    if (m_ppRawMemory != nullptr) {
        Destroy();
    }

    m_chunkSize = chunkSize;
    m_numChunks = numChunks;

    if (GrowMemoryArray()) {
        return true;
    }
    return false;
}

void MemoryPool::Destroy()
{
    if (m_ppRawMemory == nullptr) {
        return;
    }
  
    for (size_t i = 0; i < m_arraySize; ++i) {
        free(m_ppRawMemory[i]);
        m_ppRawMemory[i] = nullptr;
    }
    free(m_ppRawMemory);
    m_ppRawMemory = nullptr;

    Reset();
}

void* MemoryPool::Alloc()
{
    // TODO - grow memory pool size if out of memory
    if (!m_pHead) {
        return nullptr;
    }

    // grab first chunk from list and move to next
    uint8_t* pRet = m_pHead;
    m_pHead = GetNext(m_pHead);
    return pRet + CHUNK_HEADER_SIZE; // return the data area past the linked list entry header
}

void MemoryPool::Free(void* pMem)
{
    if (pMem != nullptr)
    {
        // get the link list header before the data ptr
        uint8_t* pBlock = ((uint8_t*)pMem) - CHUNK_HEADER_SIZE;

        // push the chunk to the front of the free chunk memory list
        SetNext(pBlock, m_pHead);
        m_pHead = pBlock;
    }
}

void MemoryPool::Reset()
{
    m_ppRawMemory = nullptr;
    m_pHead = nullptr;
    m_chunkSize = 0;
    m_numChunks = 0;
    m_arraySize = 0;
}

bool MemoryPool::GrowMemoryArray()
{
    // Allocate new array
    size_t allocSize = sizeof(uint8_t*) * (m_arraySize + 1);
    uint8_t** ppNewArray = (uint8_t**)malloc( allocSize );
    if (!ppNewArray) {
        return false;
    }

    // copy existing list members over
    for (size_t i = 0; i < m_arraySize; ++i) {
        ppNewArray[i] = m_ppRawMemory[i];
    }

    // allocate new memory block
    ppNewArray[m_arraySize] = AllocateNewMemoryBlock();

    // attach block to end of memory list
    if (m_pHead) {
        uint8_t* pCur = m_pHead;
        uint8_t* pNext = GetNext( m_pHead );
        while (pNext) {
            pCur = pNext;
            pNext = GetNext(pNext);
        }
        SetNext(pCur, ppNewArray[m_arraySize]);
    } else {
        m_pHead = ppNewArray[m_arraySize];
    }

    if (m_ppRawMemory) {
        free(m_ppRawMemory);
    }

    m_ppRawMemory = ppNewArray;
    ++m_arraySize;

    return true;
}

uint8_t* MemoryPool::AllocateNewMemoryBlock()
{
    size_t blockSize = m_chunkSize + CHUNK_HEADER_SIZE;
    size_t trueSize = blockSize * m_numChunks;

    uint8_t* pNewMem = (uint8_t*)malloc(trueSize);
    if (!pNewMem) {
        return nullptr;
    }

    // fill in linked list
    uint8_t* pEnd = pNewMem + trueSize;
    uint8_t* pCur = pNewMem;
    while (pCur < pEnd) {
        uint8_t* pNext = pCur + blockSize;

        // set next list entry pointer
        uint8_t** ppChunkHeader = (uint8_t**)pCur;
        ppChunkHeader[0] = (pNext < pEnd ? pNext : nullptr);

        // move to next block
        pCur += blockSize;
    }

    return pNewMem;
}

uint8_t* MemoryPool::GetNext(uint8_t* pBlock)
{
    uint8_t** ppChunkHeader = (uint8_t**)pBlock;
    return ppChunkHeader[0]; // the address of the next chunk
}

void MemoryPool::SetNext(uint8_t* pBlockToChange, uint8_t* pNewNext)
{
    uint8_t** ppChunkHeader = (uint8_t**)pBlockToChange;
    ppChunkHeader[0] = pNewNext;
}

