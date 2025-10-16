#ifndef HMAP_H
#define HMAP_H

/* TODO:
 *  - proper set interface; making sure the two dont mix
 *  - *_core could be called directly, probably(?)
 */

#if __STDC_VERSION__ >= 202311L
#elif __cplusplus >= 202302L
#else
# error "C/C++23 required for hmap."
#endif
#ifdef __cplusplus
# define typeof decltype
# define register
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int32_t bits;
    int32_t size;
    int32_t * used;
    // map -> (key, value)[]
    // set -> key[]
    char * payloads;
} hm_meta_t;

/***************************
 * Must be defined up high *
 ***************************/
#pragma region
#define SCOPE [[maybe_unused]] static

#ifndef hm_max_load_factor
# define hm_max_load_factor(cap) (((cap)>>1) + ((cap)>>2)) /* 75% */
#endif

#define _type_check_key(h, k) 1 ? (typeof(k+0)*)k : &h.type_guard.key_type_guard
#define _type_check_val(h, v) 1 ? (typeof(v+0)*)v : &h.type_guard.value_type_guard

typedef int32_t (*hash_fn_t)(...);
typedef bool    (*bool_fn_t)(...);

static inline int32_t fmix32(int32_t x);
static inline int32_t issue19(int32_t x);
static inline int32_t splitmix64(int64_t x);
static inline int32_t FNV1a32(const char * s);

#define hm_hash_uint32 fmix32
#define hm_hash_uint64 splitmix64
#define hm_hash_str    FNV1a32

#define hm_eq_intN(N)\
    static inline bool hm_eq_int##N (void * a, int##N##_t b) { return *(int##N##_t*)a  == b; }
hm_eq_intN(8) hm_eq_intN(16) hm_eq_intN(32) hm_eq_intN(64)
static inline bool hm_eq_str(const char * a, const char * b) { return !strcmp(a, b); }

#ifndef __cplusplus
    #define hash_generic(x) (hash_fn_t)_Generic(x \
        , int32_t       : hm_hash_uint32 \
        , int64_t       : hm_hash_uint64 \
        , char *        : hm_hash_str    \
        , const char *  : hm_hash_str    \
    )
    #define eq_generic(x) (bool_fn_t)_Generic(x \
        , int8_t        : hm_eq_int8  \
        , int16_t       : hm_eq_int16 \
        , int32_t       : hm_eq_int32 \
        , int64_t       : hm_eq_int64 \
        , char *        : hm_eq_str   \
        , const char *  : hm_eq_str   \
    )
#else
    hash_fn_t hash_generic(int32_t i) { return (hash_fn_t)hm_hash_uint32; }
    hash_fn_t hash_generic(int64_t i) { return (hash_fn_t)hm_hash_uint64; }
    hash_fn_t hash_generic(const char * i) { return (hash_fn_t)hm_hash_str; }
    #define eq_genericN(N) bool_fn_t eq_generic(int##N##_t i) { return (bool_fn_t)hm_eq_int##N; }
    eq_genericN(8) eq_genericN(16) eq_genericN(32) eq_genericN(64)
    bool_fn_t eq_generic(const char * i) { return (bool_fn_t)hm_eq_str; }
#endif

static inline bool _hm_is_used(int32_t * flag, int32_t i) { return (flag[i>>5] >> (i&0x1fU) & 1U); }
static inline void _hm_set_used(int32_t * flag, int32_t i) { flag[i>>5] |= 1U<<(i&0x1fU); }
static inline void _hm_set_unused(int32_t * flag, int32_t i) { flag[i>>5] &= ~(1U<<(i&0x1fU)); }

#define _hm_sizeof_used(n_buckets) ((n_buckets) < 32 ? 1 : (n_buckets)>>5)
#define _hm_get_n_buckets(h) ((int32_t)1U << (h)->bits)
#pragma endregion

/**************************
 * Public macro functions *
 **************************/
#pragma region
#define hmap(key_type, value_type)       \
    union {                              \
        hm_meta_t meta;                  \
        struct {                         \
            key_type key_type_guard;     \
            value_type value_type_guard; \
        } type_guard;                    \
    }
#define hmiter_t(h) int32_t

#define hm_init(h)    h.meta = (hm_meta_t) { .bits = 0, .size = 0, .used = nullptr, .payloads = nullptr }
#define hm_clear(h)   _hm_clear(&h.meta)
#define hm_delete(h)  _hm_destroy(&h.meta)

#define hm_size(h)    ((h).meta->size)
#define hm_cap(h)     ((h).meta.payloads ? 1U<<(h).meta.bits : 0U)
#define hm_begin(h)   0
#define hm_end(h)     (int32_t)hm_cap(h)

#define hm_key(h, x) (( (typeof((h).type_guard)*)((h).meta.payloads) )[x].key_type_guard)
#define hm_val(h, x) (( (typeof((h).type_guard)*)((h).meta.payloads) )[x].value_type_guard)
#define hm_a(h, k)    ( (typeof((h).type_guard)*)_hm_a(&h.meta, _type_check_key(h, k), sizeof(h.type_guard), hash_generic(k), eq_generic(k)) )->value_type_guard

#define hm_get(h, k)        _hm_get(&h.meta, _type_check_key(h, k), sizeof(h.type_guard), hash_generic(k), eq_generic(k))
#define hm_ensure_key(h, v) _hm_ensure_key(&h.meta, _type_check_val(h, v), sizeof(h.type_guard), hash_generic(k), eq_generic(k))
#define hm_del(h, it)       _hm_del(&h.meta, sizeof(h.type_guard), it)
#define hm_resize(h, n)     _hm_resize(h, sizeof(h.type_guard), new_n_buckets, hash_generic(k))
#define hm_exists(h, k)     (hm_get(h, k) != hm_end(h)) // XXX buggy

#define hm_foreach(it, h) \
    for (int _i = 0, _flag = 1; _flag && _i != hm_end(h); ++_i, _flag = !_flag) \
        for ([[ maybe_unused ]] it = _i; _flag; _flag = !_flag) \
            if (_hm_is_used((h).meta.used, _i))
#pragma endregion

/*************************
 * Common hash functions *
 *************************/
#pragma region
static inline int32_t _hm_h2b(int32_t hash, int32_t bits) { return hash * 2654435769U >> (32 - bits); } /* Fibonacci hashing */

/* Source: https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp */
static inline
int32_t fmix32(int32_t x) {
    x ^= x >> 16;
    x *= 0x85ebca6bU;
    x ^= x >> 13;
    x *= 0xc2b2ae35U;
    x ^= x >> 16;
    return x;
}

/* Source: https://github.com/skeeto/hash-prospector/issues/19 */
static inline
int32_t issue19(int32_t x) {
    x ^= x >> 16;
    x *= 0x21f0aaad;
    x ^= x >> 15;
    x *= 0xd35a2d97;
    x ^= x >> 15;
    return x;
}

/* Source: https://xoshiro.di.unimi.it/splitmix64.c */
static inline
int32_t splitmix64(int64_t x) {
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9ULL;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebULL;
    x ^= x >> 31;
    return (int32_t)x;
}

// XXX: try cityhash (https://github.com/google/cityhash/blob/master/src/city.cc)
static inline
int32_t FNV1a32(const char * s) {
    int32_t h = 2166136261U;
    const unsigned char * t = (const unsigned char*)s;
    for (; *t; ++t) {
        h ^= *t;
        h *= 16777619;
    }
    return h;
}
#pragma endregion

/****************************
 * Implementation internals *
 ****************************/
#pragma region
SCOPE
void _hm_destroy(hm_meta_t * meta) {
    free(meta->used);
    free(meta->payloads);
}

SCOPE
void _hm_clear(hm_meta_t * h) {
    if (h && h->used) {
        int32_t n_buckets = _hm_get_n_buckets(h);
        memset(h->used, 0, _hm_sizeof_used(n_buckets) * sizeof(int32_t));
        h->size = 0;
    }
}

SCOPE
int _hm_resize(hm_meta_t *h, int payload_size, int32_t new_n_buckets, hash_fn_t hash_fn) {
    int32_t * new_used = nullptr;
    int32_t new_bits = 0;

    while ((new_n_buckets >>= 1) != 0) { ++new_bits; }
    if (new_n_buckets & (new_n_buckets - 1)) { ++new_bits; }
    new_bits = new_bits > 2 ? new_bits : 2;
    new_n_buckets = (int32_t)1U << new_bits;

    // requested size is too small
    if (h->size > hm_max_load_factor(new_n_buckets)) { return 0; }

    new_used = (typeof(new_used))malloc(sizeof(int32_t) * _hm_sizeof_used(new_n_buckets));
    if (!new_used) { return -1; }

    memset(new_used, 0, _hm_sizeof_used(new_n_buckets) * sizeof(int32_t));
    int32_t n_buckets = h->payloads ? _hm_get_n_buckets(h) : 0U;
    if (n_buckets < new_n_buckets) { /* expand */
        char * new_payloads = (typeof(new_payloads))realloc(h->payloads, payload_size * new_n_buckets);
        if (!new_payloads) {
            free(new_used);
            return -1;
        }
        h->payloads = new_payloads;
    }
    for (int32_t j = 0; j != n_buckets; ++j) {
        if (!_hm_is_used(h->used, j)) { continue; }

        char * key = h->payloads + payload_size*j;
        _hm_set_unused(h->used, j);
        // Kick-out process (sort of like in Cuckoo hashing)
        for (int32_t i; true;) {
            i = _hm_h2b(hash_fn(key), new_bits);
            int32_t new_mask = new_n_buckets - 1;
            while (_hm_is_used(new_used, i)) { i = (i + 1) & new_mask; }
            _hm_set_used(new_used, i);
            // kick out the existing element
            if (i < n_buckets && _hm_is_used(h->used, i)) {
                {
                    char tmp[payload_size];
                    memcpy(tmp, h->payloads + payload_size*i, payload_size);
                    memcpy(h->payloads,  key, payload_size);
                    memcpy(key,  tmp, payload_size);
                    key = tmp;
                }
                _hm_set_unused(h->used, i);
            } else {
                memcpy(h->payloads + payload_size*i, key, payload_size);
                break;
            }
        }
    }

    // Shrink
    if (n_buckets > new_n_buckets) {
        char * new_keys = (typeof(new_keys))realloc(h->payloads, payload_size * new_n_buckets);
        if (!new_keys) {
            free(new_used);
            return -1;
        }
        h->payloads = new_keys;
    }

    free(h->used);
    h->used = new_used;
    h->bits = new_bits;

    return 0;
}

SCOPE
int _hm_del(hm_meta_t *h, int payload_size, int32_t i, hash_fn_t hash_fn) {
    if (h->payloads == nullptr) { return 0; }

    int32_t j = i;
    int32_t n_buckets = _hm_get_n_buckets(h);
    int32_t mask = n_buckets - 1U;
    while (true) {
        j = (j + 1U) & mask;
        /* j==i only when the table is completely full */
        if (j == i || !_hm_is_used(h->used, j)) { break; }
        int32_t k = _hm_h2b(hash_fn(h->payloads + j*payload_size), h->bits);
        if ((j > i && (k <= i || k > j)) || (j < i && (k <= i && k > j))) {
            memcpy(h->payloads + i*payload_size, h->payloads + j*payload_size, payload_size);
            i = j;
        }
    }
    _hm_set_unused(h->used, i);
    --h->size;

    return 1;
}

SCOPE
int32_t _hm_get_core(const hm_meta_t * h, const void * key, int payload_size, int32_t hash, bool_fn_t eq) {
    if (h->payloads == nullptr) { return 0; }

    int32_t n_buckets = _hm_get_n_buckets(h);
    int32_t mask = n_buckets - 1U;
    int32_t last, i = last = _hm_h2b(hash, h->bits);
    while (_hm_is_used(h->used, i) && !eq(h->payloads + i*payload_size, key, payload_size)) {
        i = (i + 1U) & mask;
        if (i == last) { return n_buckets; }
    }

    return !_hm_is_used(h->used, i) ? n_buckets : i;
}

SCOPE
int32_t _hm_get(const hm_meta_t *h, const void * key, int payload_size, hash_fn_t hash_fn, bool_fn_t eq) {
    return _hm_get_core(h, key, payload_size, hash_fn(key), eq);
}

SCOPE
int32_t _hm_ensure_key_core(hm_meta_t *h, const void * key, int payload_size, int32_t hash, hash_fn_t hash_fn, bool_fn_t eq) {
    int32_t n_buckets = h->payloads ? _hm_get_n_buckets(h) : 0U;

    // Rehashing
    // TODO: to implement automatically shrinking; resize() already support shrinking
    if (h->size >= hm_max_load_factor(n_buckets)) {
        if (_hm_resize(h, payload_size, n_buckets + 1U, hash_fn) < 0) {
            return n_buckets;
        }
        n_buckets = _hm_get_n_buckets(h);
    }

    // Look for key
    int32_t mask = n_buckets - 1U;
    int32_t last, i = last = _hm_h2b(hash, h->bits);
    while (_hm_is_used(h->used, i) && !eq(h->payloads + i*payload_size, key, payload_size)) {
        i = (i + 1U) & mask;
        if (i == last) { break; }
    }

    // Insert when not present
    if (!_hm_is_used(h->used, i)) {
        memcpy(h->payloads + i*payload_size, key, payload_size);
        _hm_set_used(h->used, i);
        ++h->size;
    }

    return i;
}

SCOPE
int32_t _hm_ensure_key(hm_meta_t *h, const void * key, int payload_size, hash_fn_t hash_fn, bool_fn_t eq) {
    // NOTE: we take `&key` so that we avoid `&"ls"`
    return _hm_ensure_key_core(h, &key, payload_size, hash_fn(key), hash_fn, eq);
}

SCOPE
void * _hm_a(hm_meta_t * h, const void * key, int payload_size, hash_fn_t hash_fn, bool_fn_t eq) {
    register int i = _hm_ensure_key(h, key, payload_size, hash_fn, eq);
    return h->payloads + i*payload_size;
}
#pragma endregion

#endif
