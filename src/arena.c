#include "arena.h"

#include "ashlib.h"

struct ArenaRegion {
    ArenaRegion* next;
    size_t capacity;
    void* ptr;
    void* buf;
};

static ArenaRegion*
region_create(const Allocator* a, size_t alignment, size_t capacity) {
    // NOTE: we cannot do this in single allocation because then the 0-th index
    //       of buf would be misaligned
    ArenaRegion* region = alloc_with(a, sizeof(*region));

    region->next = NULL;
    region->capacity = capacity;
    region->buf = alloc_aligned_with(a, alignment, capacity);
    region->ptr = region->buf;

    return region;
}

static void*
region_get_chunk(ArenaRegion* region, size_t alignment, size_t size) {
    void* aligned_ptr =
        (void*)align_forward((uintptr_t)region->ptr, alignment);

    if (aligned_ptr + size > region->ptr + region->capacity) {
        return NULL;
    } else {
        region->ptr = aligned_ptr + size;
        return aligned_ptr;
    }
}

Arena arena_init(Allocator allocator) { return (Arena){.inner = allocator}; }

void* arena_alloc(Arena* arena, size_t alignment, size_t size) {
    ASHLIB_ASSERT(arena);
    ASHLIB_ASSERT(size > 0);

    if (!arena->end) {
        arena->end = region_create(&arena->inner, alignment, size);
        arena->end->ptr += size;
        arena->start = arena->end;

        return arena->end->buf;
    }

    void* result;
    while (arena->end->next) {
        result = region_get_chunk(arena->end, alignment, size);
        if (result) {
            return result;
        }

        arena->end = arena->end->next;
    }

    result = region_get_chunk(arena->end, alignment, size);
    if (result) {
        return result;
    } else {
        arena->end->next = region_create(&arena->inner, alignment, size);
        arena->end = arena->end->next;
        arena->end->ptr += size;

        return arena->end->buf;
    }
}

void arena_reserve(Arena* arena, size_t capacity) {
    ASHLIB_ASSERT(arena);

    if (capacity == 0) {
        return;
    }

    ArenaRegion* region =
        region_create(&arena->inner, ASHLIB_ARENA_DEFAULT_ALIGNMENT, capacity);
    if (!arena->start) {
        arena->start = region;
        arena->end = arena->start;
    } else {
        region->next = arena->end->next;
        arena->end->next = region;
    }
}

void arena_reset(Arena* arena) {
    ASHLIB_ASSERT(arena);

    ArenaRegion* region = arena->start;

    while (region) {
        region->ptr = region->buf;
        region = region->next;
    }

    arena->end = arena->start;
}

void arena_free(Arena* arena) {
    ASHLIB_ASSERT(arena);
    ASHLIB_ASSERT(arena->start);

    ArenaRegion* region = arena->start;
    while (region) {
        ArenaRegion* tmp = region;
        region = region->next;

        free_with(&arena->inner, tmp->buf);
        free_with(&arena->inner, tmp);
    }

    arena->start = NULL;
    arena->end = NULL;
}

static inline void* alloc(void* data, size_t size) {
    // XXX: test different alignment sizes
    return arena_alloc(data, ASHLIB_ARENA_DEFAULT_ALIGNMENT, size);
}

static inline void* alloc_aligned(void* data, size_t alignment, size_t size) {
    return arena_alloc(data, alignment, size);
}

// dummy free
static inline void free(void* _data, void* _ptr) {
    (void)_data;
    (void)_ptr;
}

// XXX: make it actually realloc if it fits
static inline void* realloc(void* data, void* _, size_t size) {
    (void)_;
    // FIX: handle errors
    return alloc(data, size);
}

static inline void deinit(void* data) {
    Arena* arena = data;
    arena_free(arena);
    allocator_deinit(&arena->inner);
}

Allocator allocator_arena(Arena* arena) {
    return (Allocator){
        .alloc = alloc,
        .alloc_aligned = alloc_aligned,
        .free = free,
        .realloc = realloc,
        .deinit = deinit,
        .data = arena,
    };
}
