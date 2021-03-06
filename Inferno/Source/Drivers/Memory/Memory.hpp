#pragma once
#include "../../../../std/Types.h"

struct MemoryDescriptor {
  uint32_t type;
  void* physAddr, *virtAddr;
  uint64_t numPages, attribs;
};

uint64_t GetMemorySize(MemoryDescriptor* Map, uint64_t MapEntries, uint64_t DescriptorSize);
void* memset(void* s, uint8_t c, uint64_t len);

class Bitmap {
  public:
    size_t Size;
    uint8_t* Buffer;
    bool operator[](uint64_t index);
    bool Get(uint64_t index);
    bool Set(uint64_t index, bool value);
};

class PageFrameAllocator {
  public:
    void ReadMemoryMap(MemoryDescriptor* Map, size_t MapSize, size_t DescriptorSize);
    Bitmap PageBitmap;
    void FreePage(void* address);
    void LockPage(void* address);
    void FreePages(void* address, uint64_t pageCount);
    void LockPages(void* address, uint64_t pageCount);
    void* RequestPage();
    uint64_t GetFreeMem();
    uint64_t GetUsedMem();
    uint64_t GetReservedMem();
  private:
    void InitBitmap(size_t bitmapSize, void* BufferAddress);
    void ReservePage(void* address);
    void UnreservePage(void* address);
    void ReservePages(void* address, uint64_t pageCount);
    void UnreservePages(void* address, uint64_t pageCount);
};


enum PT_Flag {
    Present = 0,
    ReadWrite = 1,
    UserSuper = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63
};

struct PageDirectoryEntry {
    uint64_t Value;
    void SetFlag(PT_Flag flag, bool enabled);
    bool GetFlag(PT_Flag flag);
    void SetAddress(uint64_t address);
    uint64_t GetAddress();
};

struct PageTable { 
    PageDirectoryEntry entries [512];
}__attribute__((aligned(0x1000)));

class PageMapIndexer {
  public:
    PageMapIndexer(uint64_t virtualAddress);
    uint64_t PDP_i, PD_i, PT_i, P_i;
};

class PageTableManager {
  public:
    PageTableManager(PageTable* Level4Address);
    PageTable* Level4Address;
    void MapMemory(void* virtualMemory, void* physicalMemory);
};

#define NULL 0

struct HeapSegHdr{
  size_t length;
  HeapSegHdr* next;
  HeapSegHdr* last;
  bool free;
  void CombineForward();
  void CombineBackward();
  HeapSegHdr* Split(size_t splitLength);
};

void InitializeHeap(void* heapAddress, size_t pageCount);

void* malloc(size_t size);
void free(void* address);

void ExpandHeap(size_t length); 

extern const char* MemoryTypesStrings[];
extern PageFrameAllocator Allocator;
extern PageTableManager pageTableManager;

void* memcpy(void *dst, const void *src, size_t len);

inline void* operator new(long unsigned int size) { return malloc(size); }
inline void* operator new[](long unsigned int size) { return malloc(size); }

inline void operator delete(void* p) { free(p); }