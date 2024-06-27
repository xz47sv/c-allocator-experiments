#ifndef ASHLIB_H
#define ASHLIB_H

#ifdef ASHLIB_NO_ASSERT
#define ASHLIB_ASSERT(predicate) (void)(predicate)
#else
#include <assert.h>
#define ASHLIB_ASSERT(predicate) assert(predicate)
#endif // ASHLIB_NO_ASSERT

// we should use the types from here everywhere anyway
#include <stddef.h>

#endif // !ASHLIB_H
