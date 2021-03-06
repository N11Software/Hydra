#include "mman.h"
#include "errno.h"
#include <Inferno/Syscall.h>

extern "C" {

void* mmap(void* addr, size_t size)
{
    int rc = Syscall::invoke(Syscall::PosixMmap, (dword)addr, (dword)size);
    __RETURN_WITH_ERRNO(rc, (void*)rc, (void*)-1);
}

int munmap(void* addr, size_t size)
{
    int rc = Syscall::invoke(Syscall::PosixMunmap, (dword)addr, (dword)size);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

int set_mmap_name(void* addr, size_t size, const char* name)
{
    int rc = Syscall::invoke(Syscall::SetMmapName, (dword)addr, (dword)size, (dword)name);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

}
