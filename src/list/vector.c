#include <stdlib.h>

#include "vector.h"
#include "../utils/errors.h"

struct vector vector_create(size_t t_size) {
    void *buf = malloc(t_size);
    if (!buf)
        mexit();
    return (struct vector) {
        .size = 0,
        .cap = 1,
        .buf = buf,
    };
}

void vector_prepare(struct vector *v, size_t size, size_t t_size) {
    if (v->cap < v->size + size) {
        while (v->cap < v->size + size)
            v->cap *= 2;

        v->buf = realloc(v->buf, v->cap * t_size);
        if (!v->buf)
            mexit();
    }
}

void vector_free(struct vector *v) {
    free(v->buf);
}

void vector_clear(struct vector *v) {
    v->size = 0;
}

void _vector_append_u64(struct vector *v, uint64_t x) {
    size_t size = v->size;
    vector_prepare(v, 1, sizeof(x));
    v->size++;
    uint64_t *buf = (uint64_t*)v->buf;
    buf[size] = x;
}

void _vector_append_u32(struct vector *v, uint32_t x) {
    size_t size = v->size;
    vector_prepare(v, 1, sizeof(x));
    v->size++;
    uint32_t *buf = (uint32_t*)v->buf;
    buf[size] = x;
}

void _vector_append_u16(struct vector *v, uint16_t x) {
    size_t size = v->size;
    vector_prepare(v, 1, sizeof(x));
    v->size++;
    uint16_t *buf = (uint16_t*)v->buf;
    buf[size] = x;
}

void _vector_append_default(struct vector *v, int x) {
    unsuported("vector_append");
}

uint64_t _vector_pop_u64(struct vector *v) {
    v->size--;
    return ((uint64_t*)v->buf)[v->size];
}

uint32_t _vector_pop_u32(struct vector *v) {
    v->size--;
    return ((uint32_t*)v->buf)[v->size];
}

uint16_t _vector_pop_u16(struct vector *v) {
    v->size--;
    return ((uint16_t*)v->buf)[v->size];
}

int _vector_pop_default(struct vector *v) {
    unsuported("vector_pop");
    return 0;
}

bool _vector_contains_u64(struct vector *v, uint64_t x) {
    uint64_t *buf = (uint64_t*)v->buf;
    for (size_t i = 0; i < v->size; i++) {
        if (buf[i] == x)
            return true;
    }
    return false;
}

bool _vector_contains_u32(struct vector *v, uint32_t x) {
    uint32_t *buf = (uint32_t*)v->buf;
    for (size_t i = 0; i < v->size; i++) {
        if (buf[i] == x)
            return true;
    }
    return false;
}

bool _vector_contains_u16(struct vector *v, uint16_t x) {
    uint16_t *buf = (uint16_t*)v->buf;
    for (size_t i = 0; i < v->size; i++) {
        if (buf[i] == x)
            return true;
    }
    return false;
}

bool _vector_contains_default(struct vector *v, int x) {
    unsuported("vector_contains");
    return false;
}
