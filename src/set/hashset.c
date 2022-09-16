#include <stdint.h>
#include <stdlib.h>

#include "hashset.h"
#include "../list/vector.h"
#include "../utils/errors.h"

struct hashset hashset_create(size_t size, size_t t_size) {
    struct vector *buckets = calloc(size, sizeof(struct vector));
    if (!buckets)
        mexit();
    return (struct hashset) {
        .buckets_s = size,
        .buckets = buckets,
    };
}

void hashset_free(struct hashset *h) {
    for (int i = 0; i < h->buckets_s; i++) {
        vector_free(&(h->buckets[i]));
    }
    free(h->buckets);
}

struct vector *_hashset_get_bucket_u64(struct hashset *h, uint64_t hash) {
    size_t i = hash % h->buckets_s;
    if (!h->buckets[i].buf)
        h->buckets[i] = vector_create(sizeof(uint64_t));
    return h->buckets + i;
}

struct vector *_hashset_get_bucket_u32(struct hashset *h, uint32_t hash) {
    size_t i = hash % h->buckets_s;
    if (!h->buckets[i].buf)
        h->buckets[i] = vector_create(sizeof(uint32_t));
    return h->buckets + i;
}

struct vector *_hashset_get_bucket_u16(struct hashset *h, uint16_t hash) {
    size_t i = hash % h->buckets_s;
    if (!h->buckets[i].buf)
        h->buckets[i] = vector_create(sizeof(uint16_t));
    return h->buckets + i;
}

struct vector *_hashset_get_bucket_default(struct hashset *h, int hash) {
    unsuported("hashset_get_bucket");
    return 0;
}

uint32_t hashset_hash_struct(void* ptr, size_t t_size) {
    if (!ptr) return 0;
    uint8_t *sized_ptr = (uint8_t*)ptr;
    uint32_t hash = 1;
    for (uint32_t i = 0; i < t_size; i++) {
        hash = 92821 * hash + sized_ptr[i];
    }
    return hash;
}

uint64_t _hashset_hash_u64(uint64_t x) {
    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
}

uint32_t _hashset_hash_u32(uint32_t x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

// TODO: could find a better suited hash for u16
uint16_t _hashset_hash_u16(uint16_t v) {
    uint32_t x = v;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

int _hashset_hash_default(int x) {
    unsuported("hashset_hash");
    return 0;
}

bool _hashset_add_u64(struct hashset *h, uint64_t x) {
    uint64_t hash = hashset_hash(x);
    struct vector *b = hashset_get_bucket(h, hash);
    if (vector_contains(b, x))
        return false;
    vector_append(b, x);
    return true;
}

bool _hashset_add_u32(struct hashset *h, uint32_t x) {
    uint32_t hash = hashset_hash(x);
    struct vector *b = hashset_get_bucket(h, hash);
    if (vector_contains(b, x))
        return false;
    vector_append(b, x);
    return true;
}

bool _hashset_add_u16(struct hashset *h, uint16_t x) {
    uint16_t hash = hashset_hash(x);
    struct vector *b = hashset_get_bucket(h, hash);
    if (vector_contains(b, x))
        return false;
    vector_append(b, x);
    return true;
}

bool _hashset_add_default(struct hashset *h, int x) {
    unsuported("hashset_add");
    return false;
}

bool _hashset_contains_u64(struct hashset *h, uint64_t x) {
    uint64_t hash = hashset_hash(x);
    struct vector *b = hashset_get_bucket(h, hash);
    return vector_contains(b, x);
}

bool _hashset_contains_u32(struct hashset *h, uint32_t x) {
    uint32_t hash = hashset_hash(x);
    struct vector *b = hashset_get_bucket(h, hash);
    return vector_contains(b, x);
}

bool _hashset_contains_u16(struct hashset *h, uint16_t x) {
    uint16_t hash = hashset_hash(x);
    struct vector *b = hashset_get_bucket(h, hash);
    return vector_contains(b, x);
}

bool _hashset_contains_default(struct hashset *h, int x) {
    unsuported("hashset_add");
    return false;
}
