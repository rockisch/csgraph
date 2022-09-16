#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>

#include "../list/vector.h"

struct hashset {
    size_t buckets_s;
    struct vector *buckets;
};

struct hashset hashset_create(size_t size, size_t t_size);
void hashset_free(struct hashset *h);

struct vector *_hashset_get_bucket_u64(struct hashset *h, uint64_t hash);
struct vector *_hashset_get_bucket_u32(struct hashset *h, uint32_t hash);
struct vector *_hashset_get_bucket_u16(struct hashset *h, uint16_t hash);
struct vector *_hashset_get_bucket_default(struct hashset *h, int hash);
#define hashset_get_bucket(h, x) _Generic((x),  \
    uint64_t: _hashset_get_bucket_u64,          \
    uint32_t: _hashset_get_bucket_u32,          \
    uint16_t: _hashset_get_bucket_u16,          \
    default: _hashset_get_bucket_default        \
)(h, x)

uint32_t hashset_hash_struct(void* ptr, size_t t_size);

uint64_t _hashset_hash_u64(uint64_t x);
uint32_t _hashset_hash_u32(uint32_t x);
uint16_t _hashset_hash_u16(uint16_t x);
int _hashset_hash_default(int x);
#define hashset_hash(x) _Generic((x),   \
    uint64_t: _hashset_hash_u64,        \
    uint32_t: _hashset_hash_u32,        \
    uint16_t: _hashset_hash_u16,        \
    default: _hashset_hash_default      \
)(x)

bool _hashset_add_u64(struct hashset *h, uint64_t x);
bool _hashset_add_u32(struct hashset *h, uint32_t x);
bool _hashset_add_u16(struct hashset *h, uint16_t x);
bool _hashset_add_default(struct hashset *h, int x);
#define hashset_add(h, x) _Generic((x), \
    uint64_t: _hashset_add_u64,         \
    uint32_t: _hashset_add_u32,         \
    uint16_t: _hashset_add_u16,         \
    default: _hashset_add_default       \
)(h, x)

bool _hashset_contains_u64(struct hashset *h, uint64_t x);
bool _hashset_contains_u32(struct hashset *h, uint32_t x);
bool _hashset_contains_u16(struct hashset *h, uint16_t x);
bool _hashset_contains_default(struct hashset *h, int x);
#define hashset_contains(h, x) _Generic((x),    \
    uint64_t: _hashset_contains_u64,            \
    uint32_t: _hashset_contains_u32,            \
    uint16_t: _hashset_contains_u16,            \
    default: _hashset_contains_default          \
)(h, x)
#endif
