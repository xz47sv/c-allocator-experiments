#include "malloc.h"

#include <stdlib.h>

static void* alloc_impl(void* _, size_t size) {
    (void)_;
    return malloc(size);
}

static void*
alloc_aligned_impl(void* _, size_t alignment, size_t size) {
    (void)_;
    return aligned_alloc(alignment, size);
}

static void free_impl(void* _, void* ptr) {
    (void)_;
    free(ptr);
}

static void* realloc_impl(void* _, void* ptr, size_t size) {
    (void)_;
    return realloc(ptr, size);
}

Allocator allocator_malloc() {
    return (Allocator){
        .alloc = alloc_impl,
        .alloc_aligned = alloc_aligned_impl,
        .free = free_impl,
        .realloc = realloc_impl,
    };
}
