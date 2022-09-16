#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct vector {
    size_t size;
    size_t cap;
    void *buf;
};

struct vector vector_create(size_t t_size);
void vector_prepare(struct vector *v, size_t size, size_t t_size);
void vector_free(struct vector *v);

void vector_clear(struct vector *v);

void _vector_append_u64(struct vector *v, uint64_t x);
void _vector_append_u32(struct vector *v, uint32_t x);
void _vector_append_u16(struct vector *v, uint16_t x);
void _vector_append_default(struct vector *v, int x);
#define vector_append(v, x) _Generic((x),   \
    uint64_t: _vector_append_u64,           \
    uint32_t: _vector_append_u32,           \
    uint16_t: _vector_append_u16,           \
    default: _vector_append_default         \
)(v, x)

uint64_t _vector_pop_u64(struct vector *v);
uint32_t _vector_pop_u32(struct vector *v);
uint16_t _vector_pop_u16(struct vector *v);
int _vector_pop_default(struct vector *v);
#define vector_pop(x, hint) _Generic((hint),    \
    uint64_t: _vector_pop_u64,                  \
    uint32_t: _vector_pop_u32,                  \
    uint16_t: _vector_pop_u16,                  \
    default: _vector_pop_default                \
)(x)

bool _vector_contains_u64(struct vector *v, uint64_t x);
bool _vector_contains_u32(struct vector *v, uint32_t x);
bool _vector_contains_u16(struct vector *v, uint16_t x);
bool _vector_contains_default(struct vector *v, int x);
#define vector_contains(v, x) _Generic((x),   \
    uint64_t: _vector_contains_u64,           \
    uint32_t: _vector_contains_u32,           \
    uint16_t: _vector_contains_u16,           \
    default: _vector_contains_default         \
)(v, x)

#endif
