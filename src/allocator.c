#include "allocator.h"

// FIX: error handling

uintptr_t align_forward(uintptr_t address, size_t alignment) {
    return ((address + (alignment - 1)) & ~(alignment - 1));
}

void* alloc_with(const Allocator* allocator, size_t size) {
    ASHLIB_ASSERT(allocator->alloc);
    ASHLIB_ASSERT(size > 0);

    return allocator->alloc(allocator->data, size);
}

void* alloc_aligned_with(
    const Allocator* allocator,
    size_t alignment,
    size_t size
) {
    ASHLIB_ASSERT(allocator->alloc_aligned);
    // NOTE: this check is not sufficient, alignment should also be power of 2
    ASHLIB_ASSERT(alignment % 2 == 0);
    ASHLIB_ASSERT(size > 0);

    if (size < alignment) {
        size = alignment;
    } else {
        size = align_forward(size, alignment);
    }

    return allocator->alloc_aligned(allocator->data, alignment, size);
}

void free_with(const Allocator* allocator, void* ptr) {
    ASHLIB_ASSERT(allocator->free);

    if (ptr) {
        allocator->free(allocator->data, ptr);
    }
}

void* realloc_with(const Allocator* allocator, void* ptr, size_t size) {
    ASHLIB_ASSERT(allocator->realloc);
    ASHLIB_ASSERT(ptr);
    ASHLIB_ASSERT(size > 0);

    return allocator->realloc(allocator->data, ptr, size);
}

void allocator_deinit(Allocator* allocator) {
    ASHLIB_ASSERT(allocator);

    if (allocator->deinit) {
        allocator->deinit(allocator->data);
    }
}
