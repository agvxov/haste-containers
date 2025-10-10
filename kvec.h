#ifndef AC_KVEC_H
#define AC_KVEC_H
/*
  An example:

    #include "kvec.h"
    int main() {
        kvec_t(int) array;
        kv_init(array);
        kv_push(int, array, 10);  // append
        kv_a(int, array, 20) = 5; // dynamic
        kv_A(array, 20) = 4;      // static
        kv_destroy(array);
        return 0;
    }
*/
#if __STDC_VERSION__ >= 202311L
#elif __cplusplus >= 202302L
#else
# error "C/C++23 required for kvec."
#endif
#ifdef __cplusplus
# define typeof decltype
#endif

#include <stdlib.h>
#include <string.h>

#define kv_roundup32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

#define kvec_t(type)  struct { size_t n, m; type *a; }
#define kv_init(v)    ((v).n = (v).m = 0, (v).a = 0)
#define kv_destroy(v) free((v).a)
#define kv_A(v, i)    ((v).a[(i)])
#define kv_pop(v)     ((v).a[--(v).n])
#define kv_size(v)    ((v).n)
#define kv_max(v)     ((v).m)

#define kv_resize(v, s) (                                               \
    (v).m = (s),                                                        \
    (v).a = (typeof(v.a))realloc((v).a, sizeof(typeof(v.a[0])) * (v).m) \
)

#define kv_copy(vd, vs) do {                                  \
    if ((vd).m < (vs).n) kv_resize(vd, (vs).n);               \
    (vd).n = (vs).n;                                          \
    memcpy((vd).a, (vs).a, sizeof(typeof(vs.a[0])) * (vs).n); \
} while (0)


#define kv_append(v1, v0) do {                                                     \
    if ((v1).m < (v1).n + (v0).n) kv_resize(typeof(v1.a[0]), v1, (v1).n + (v0).n); \
    memcpy((v1).a + (v1).n, (v0).a, sizeof(typeof(v0.a[0]) * (v0).n);              \
    (v1).n += (v0).n;                                                              \
} while (0)

#define kv_push(v, x) do {                                                   \
    if ((v).n == (v).m) {                                                    \
        (v).m = (v).m ? (v).m<<1 : 2;                                        \
        (v).a = (typeof(v.a))realloc((v).a, sizeof(typeof(v.a[0])) * (v).m); \
    }                                                                        \
    (v).a[(v).n++] = (x);                                                    \
} while (0)

#define kv_pushp(v) (                                                                      \
    (                                                                                      \
        ((v).n == (v).m) ?                                                                 \
            (                                                                              \
                (v).m = ((v).m ? (v).m<<1 : 2),                                            \
                (v).a = (typeof((v).a))realloc((v).a, sizeof(typeof((v).a[0])) * (v).m), 0 \
            )                                                                              \
        :                                                                                  \
            0                                                                              \
    ),                                                                                     \
    (v).a + (v).n++                                                                        \
)

#define kv_a(v, i) (                                                                           \
    (                                                                                      \
        (v).m <= (size_t)(i) ?                                                             \
            (                                                                              \
                (v).m = (v).n = (i) + 1, kv_roundup32((v).m),                              \
                (v).a = (typeof((v).a))realloc((v).a, sizeof(typeof((v).a[0])) * (v).m), 0 \
            )                                                                              \
        : (v).n <= (size_t)(i) ? (v).n = (i) + 1 : 0                                       \
    ),                                                                                     \
    (v).a[(i)]                                                                             \
)

#endif
