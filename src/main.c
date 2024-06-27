#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "allocator.h"
#include "arena.h"
#include "guarded.h"
#include "malloc.h"

int main(void) {
    printf("%lu\n", ASHLIB_ARENA_DEFAULT_ALIGNMENT);
    Arena arena = arena_init(allocator_malloc());
    Allocator a = allocator_guarded(allocator_arena(&arena));

    arena_reserve(&arena, 2048);

    char* test = alloc_with(&a, 1024);

    (void)alloc_with(&a, 512);
    (void)alloc_with(&a, 104);

    printf("%lu\n", (uintptr_t)alloc_aligned_with(&a, 4096, 1011) % 4096);

    arena_reset(&arena);

    test = alloc_with(&a, 1024);

    (void)alloc_with(&a, 512);
    (void)alloc_with(&a, 104);

    printf("%lu\n", (uintptr_t)alloc_aligned_with(&a, 4096, 1011) % 4096);

    memset(test, 'a', 1024);
    test[1023] = 0;

    printf("%s\n", test);

	allocator_deinit(&a);

    return 0;
}
