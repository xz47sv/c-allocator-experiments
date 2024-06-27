#ifndef ASHLIB_ARENA_H
#define ASHLIB_ARENA_H

#ifndef ASHLIB_ARENA_DEFAULT_ALIGNMENT
#include <stdalign.h>
#define ASHLIB_ARENA_DEFAULT_ALIGNMENT alignof(max_align_t)
#endif // !ASHLIB_ARENA_DEFAULT_ALIGNMENT

#include "allocator.h"

typedef struct ArenaRegion ArenaRegion;

typedef struct {
    Allocator inner;
    ArenaRegion *start, *end;
} Arena;

Arena arena_init(Allocator a);

void* arena_alloc(Arena* arena, size_t alignment, size_t size);

void arena_reserve(Arena* arena, size_t capacity);

void arena_reset(Arena* arena);

void arena_free(Arena* arena);

Allocator allocator_arena(Arena* arena);

#endif // !ASHLIB_ARENA_H
