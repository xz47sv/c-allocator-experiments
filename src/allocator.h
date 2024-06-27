#ifndef ASHLIB_ALLOCATOR_H
#define ASHLIB_ALLOCATOR_H

#include <stdint.h>

#include "ashlib.h"

uintptr_t align_forward(uintptr_t address, size_t alignment);

typedef struct {
    void* (*alloc)(void*, size_t);
    void* (*alloc_aligned)(void*, size_t, size_t);
    void (*free)(void*, void*);
    void* (*realloc)(void*, void*, size_t);
    void (*deinit)(void* data);
    void* data;
} Allocator;

void* alloc_with(const Allocator* allocator, size_t size);

void* alloc_aligned_with(
    const Allocator* allocator,
    size_t alignment,
    size_t size
);

void free_with(const Allocator* allocator, void* ptr);

void* realloc_with(const Allocator* allocator, void* ptr, size_t size);

void allocator_deinit(Allocator* allocator);

#endif // !ASHLIB_ALLOCATOR_H
