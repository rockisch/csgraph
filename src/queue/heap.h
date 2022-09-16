#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stdlib.h>

#include "../list/vector.h"

struct heap {
    struct vector weight_vec;
    struct vector value_vec;
};

struct heap_item_u32 {
    uint32_t weight;
    uint32_t value;
};

struct heap heap_create(size_t size);
size_t heap_size(struct heap *h);
void heap_free(struct heap *h);

void _heap_insert_min_u32(struct heap *h, uint32_t weight, uint32_t value);
#define heap_insert_min(h, w, v) _Generic((v),  \
    uint32_t: _heap_insert_min_u32              \
)(h, w, v)

struct heap_item_u32 _heap_pop_min_u32(struct heap *h);
#define heap_pop_min(h, hint) _Generic((hint),  \
    uint32_t: _heap_pop_min_u32                 \
)(h)

#endif
