#ifndef GCC4_MEMORY_POOL_H_INCLUDED
#define GCC4_MEMORY_POOL_H_INCLUDED

#include <cstdint>
#include <cstddef>

class MemoryPool
{
public:

  MemoryPool();
  ~MemoryPool();

  /**
   * create the internal memory pool
   */
  bool Init( size_t chunkSize, size_t numChunks );

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
  void Free( void* pMem );

  /**
   * Get the configured size of each chunk
   */ 
  size_t GetChunkSize() const { return m_chunkSize; }

private:

  // no copying!
  MemoryPool( const MemoryPool& memPool );

  uint8_t** m_ppRawMemory; // array of memory blocks, eaching have a single uint8_t header
  uint8_t*  m_pHead;       // front of the memory chunk linked list
  size_t    m_chunkSize;   // number of bytes in each chunk
  size_t    m_numChunks;   // number of chunks in the memory pool
  size_t    m_arraySize;   // number of memory blocks

  void  Reset();
  bool  GrowMemoryArray();
  uint8_t* AllocateNewMemoryBlock();

  uint8_t* GetNext( uint8_t* pBlock ); // traverse and find next linked list entry
  void     SetNext( uint8_t* pBlockToChange, uint8_t* pNewNext ); 
  
};

#endif // GCC4_MEMORY_POOL_H_INCLUDED

