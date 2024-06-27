#ifndef ASHLIB_SLICE_H
#define ASHLIB_SLICE_H

#define ASHLIB_SLICE_TYPE(T)                                                  \
    struct {                                                                  \
        T* buf;                                                               \
        size_t len;                                                           \
    }

#define ASHLIB_SLICE(b, l)                                                    \
    { .buf = b, .len = l }

#endif // !ASHLIB_SLICE_H
