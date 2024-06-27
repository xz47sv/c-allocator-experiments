#include "guarded.h"

#include <threads.h>

typedef struct {
    Allocator inner;
    mtx_t lock;
} AllocatorGuarded;

// FIX: handle mutex errors
static void* alloc(void* data, size_t size) {
    AllocatorGuarded* guarded = data;

    mtx_lock(&guarded->lock);
    void* result = alloc_with(&guarded->inner, size);
    mtx_unlock(&guarded->lock);

    return result;
}

static void* alloc_aligned(void* data, size_t alignment, size_t size) {
    AllocatorGuarded* guarded = data;

    mtx_lock(&guarded->lock);
    void* result = alloc_aligned_with(&guarded->inner, alignment, size);
    mtx_unlock(&guarded->lock);

    return result;
}

static void free(void* data, void* ptr) {
    AllocatorGuarded* guarded = data;

    mtx_lock(&guarded->lock);
    free_with(&guarded->inner, ptr);
    mtx_unlock(&guarded->lock);
}

static void* realloc(void* data, void* ptr, size_t size) {
    AllocatorGuarded* guarded = data;

    mtx_lock(&guarded->lock);
    void* result = realloc_with(&guarded->inner, ptr, size);
    mtx_unlock(&guarded->lock);

    return result;
}

static void deinit(void* data) {
    AllocatorGuarded* guarded = data;
    mtx_destroy(&guarded->lock);
    allocator_deinit(&guarded->inner);
}

Allocator allocator_guarded(Allocator allocator) {
    // NOTE: this cannot be stack allocated because mutex needs ptr to itself
    AllocatorGuarded* guarded = alloc_with(&allocator, sizeof(*guarded));
    guarded->inner = allocator;
    mtx_init(&guarded->lock, mtx_plain);

    return (Allocator){
        .alloc = alloc,
        .alloc_aligned = alloc_aligned,
        .free = free,
        .realloc = realloc,
        .deinit = deinit,
        .data = guarded,
    };
}
