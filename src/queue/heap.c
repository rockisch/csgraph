#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"
#include "../list/vector.h"

struct heap heap_create(size_t size) {
    struct vector w_vec = vector_create(size);
    struct vector v_vec = vector_create(size);
    return (struct heap) {
        .weight_vec=w_vec,
        .value_vec=v_vec,
    };
}

size_t heap_size(struct heap *h) {
    return h->weight_vec.size;
}

void heap_free(struct heap *h) {
    vector_free(&(h->weight_vec));
    vector_free(&(h->value_vec));
}

void heapify_min_u32(struct heap*h) {
    uint32_t *buf = (uint32_t*)h->weight_vec.buf;
    uint32_t *v_buf = (uint32_t*)h->value_vec.buf;
    size_t n = (h->weight_vec.size / 2);
    if (n > 0)
        n--;
    for (int i = n; i >= 0; i--) {
        size_t m = i;
        size_t li = (2*i)+1;
        if (li < h->weight_vec.size) {
            if (buf[m] > buf[li])
                m = li;
            size_t ri = (2*i)+2;
            if (ri < h->weight_vec.size) {
                if (buf[m] > buf[ri])
                    m = ri;
            }
            uint32_t w = buf[i];
            buf[i] = buf[m];
            buf[m] = w;

            uint32_t v = v_buf[i];
            v_buf[i] = v_buf[m];
            v_buf[m] = v;
        }
    }
}

void _heap_insert_min_u32(struct heap *h, uint32_t weight, uint32_t value) {
    vector_append(&(h->weight_vec), weight);
    vector_append(&(h->value_vec), value);
    heapify_min_u32(h);
}

struct heap_item_u32 _heap_pop_min_u32(struct heap *h) {
    assert(h->weight_vec.size);
    uint32_t head_w = ((uint32_t*)h->weight_vec.buf)[0];
    uint32_t head_v = ((uint32_t*)h->value_vec.buf)[0];
    uint32_t tail_w = vector_pop(&(h->weight_vec), (uint32_t)0);
    uint32_t tail_v = vector_pop(&(h->value_vec), (uint32_t)0);
    ((uint32_t*)h->weight_vec.buf)[0] = tail_w;
    ((uint32_t*)h->value_vec.buf)[0] = tail_v;
    heapify_min_u32(h);
    return (struct heap_item_u32) {.weight=head_w, .value=head_v};
}
