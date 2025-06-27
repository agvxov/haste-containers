#ifndef AC_KLIST_H
#define AC_KLIST_H

#include <kvec.h>

/* NOTE: C++ is shitware for the mentally handicapped.
 *        For this reason, it removes a bunch of useful, harmless,
 *         exciting, and holy edgecases which were put there by God himself.
 *        As a result, we have to hack around it.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#define klnode_t(type) struct { type d; size_t p, n; }
#define klist_t(type) struct { size_t h, t; size_t n; kvec_t(klnode_t(type)) a; }
/* C++ Problem 1)
 * It disallows conversions from void*, but keeps the no anonymous type folding
 *  within the same compile unit rule from C.
 * It also conveniently forgets to counter-act this with some decltype hack,
 *  e.g. this is illegal:
 *
 *      struct { int i; } a;
 *
 *      decltype( ([] constexpr { struct { int i; } a; return a; })() )* b
 *      = (decltype( ([] constexpr { struct { int i; } a; return a; })() )*)&a;
 *
 * Not to mention that the syntax has been buggy in G++ for years, but I digress.
 *
 * As a result, the original iterator:
 *      #define kliter_t(type) klnode_t(type)*
 * Proved to be unfeasible.
 * One partial solution would have been to use auto,
 *  but then it would be impossible to have uninitialized iterators,
 *  e.g. for some selection task.
 *
 * This, btw is why we depend on C23. Because either typeof,
 *  or further macro mangling to cast / not cast the iterator function return values.
 */
#ifdef __cplusplus
    #define kliter_t(l) decltype(l.a.a)
#else
    #define kliter_t(l) typeof(l.a.a)
#endif

/* C++ Problem 2)
 *  > error: types may not be defined in ‘sizeof’ expressions
 *  Truly genius, I bet Bjorn Stonesoup lies awake at night,
 *   adoring himself for inventing this.
 *  Anyways, we can fix this by lobotomy.
 */
#ifdef __cplusplus
    #define LOBOTOMIZED_TYPE_DEFINITION_SUBVERTED_INSIDE_SIZEOF(...) \
     ([] constexpr { __VA_ARGS__ i; return sizeof(i); })()
#else
    #define LOBOTOMIZED_TYPE_DEFINITION_SUBVERTED_INSIDE_SIZEOF(...) sizeof(__VA_ARGS__)
#endif

/* C++ Problem 3)
 * > error: invalid conversion from ‘void*’ to ‘[...]<unnamed struct>*’
 * Oh Severin!
 * At this time around the interface wont have to change...
 */
#ifdef __cplusplus
    #define MY_TYPE_CAST(v) (decltype((v).a))
#else
    #define MY_TYPE_CAST(v)
#endif

/* Copy of kvec's kv_pushp() with more casts.
 * A modified version is required for type safety in C,
 *  C++ just so happens to further mangle it.
 */
#define kv_pushp_(type, v) (((v).n == (v).m)?                           \
                           ((v).m = ((v).m? (v).m<<1 : 2),              \
                            (v).a = MY_TYPE_CAST(v) realloc((v).a, (LOBOTOMIZED_TYPE_DEFINITION_SUBVERTED_INSIDE_SIZEOF(type)) * (v).m), 0) \
                           : 0), ((v).a + ((v).n++))

#define kl_init(l) (l.h = l.t = SIZE_MAX, l.n = 0, kv_init(l.a))
#define kl_destroy(l) kv_destroy(l.a)

#define kl_begin(l)    (l.h  != SIZE_MAX ? &kv_A(l.a, l.h)  : NULL)
#define kl_end(l)      (l.t  != SIZE_MAX ? &kv_A(l.a, l.t)  : NULL)
#define kl_next(l, i)  (i->n != SIZE_MAX ? &kv_A(l.a, i->n) : NULL)
#define kl_size(l)     l.n
#define kl_d(i)        i->d

#define kl_add(type, l, x) ( \
    kv_pushp_(klnode_t(type), l.a), \
    kv_A(l.a, kv_size(l.a)-1).d = x, \
    (l.t != SIZE_MAX \
        ? ( \
            kv_A(l.a, l.t).n = kv_size(l.a)-1, \
            kv_A(l.a, kv_size(l.a)-1).p = l.t, \
            kv_A(l.a, kv_size(l.a)-1).n = SIZE_MAX \
        ) : ( \
            l.h = kv_size(l.a)-1, \
            kv_A(l.a, l.h).p = SIZE_MAX, \
            kv_A(l.a, l.h).n = SIZE_MAX \
    )), \
    l.t = kv_size(l.a)-1, \
    ++l.n \
)

#define kl_remove(l, i) ( \
    (i->p != SIZE_MAX) ? (kv_A(l.a, i->p).n = i->n) : (l.h = i->n), \
    (i->n != SIZE_MAX) ? (kv_A(l.a, i->n).p = i->p) : (l.t = i->p), \
    --l.n \
)

/* XXX
// Insert before iterator position
void insert(List *list, ListIterator pos, void *data) {
    if (pos == NULL) {
        push(list, data);
        return;
    }

    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->prev = pos->prev;
    node->next = pos;

    if (pos->prev)
        pos->prev->next = node;
    else
        list->head = node;

    pos->prev = node;
    list->size++;
}
*/

#endif
