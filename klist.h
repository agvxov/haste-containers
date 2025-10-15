#ifndef AC_KLIST_H
#define AC_KLIST_H

#include <kvec.h>

#include <stdlib.h>
#include <stdint.h>

#define klnode_t(type) struct { type d; size_t p, n; }
#define klist_t(type)  struct { size_t h, t; size_t n; kvec_t(klnode_t(type)) a; }
#define kliter_t(l) typeof(l.a.a)

#define kl_init(l)   (l.h = l.t = SIZE_MAX, l.n = 0, kv_init(l.a))
#define kl_delete(l) kv_delete(l.a)

#define kl_begin(l)    (l.h  != SIZE_MAX ? &kv_A(l.a, l.h)  : nullptr)
#define kl_end(l)      (l.t  != SIZE_MAX ? &kv_A(l.a, l.t)  : nullptr)
#define kl_next(l, i)  (i->n != SIZE_MAX ? &kv_A(l.a, i->n) : nullptr) /*XXX: maybe warn unsed?*/
#define kl_size(l)     l.n
#define kl_d(i)        i->d

#define kl_add(l, x) (                             \
    kv_addp(l.a),                                  \
    kv_A(l.a, kv_size(l.a)-1).d = x,               \
    (l.t != SIZE_MAX                               \
        ? (                                        \
            kv_A(l.a, l.t).n = kv_size(l.a)-1,     \
            kv_A(l.a, kv_size(l.a)-1).p = l.t,     \
            kv_A(l.a, kv_size(l.a)-1).n = SIZE_MAX \
        ) : (                                      \
            l.h = kv_size(l.a)-1,                  \
            kv_A(l.a, l.h).p = SIZE_MAX,           \
            kv_A(l.a, l.h).n = SIZE_MAX            \
    )),                                            \
    l.t = kv_size(l.a)-1,                          \
    ++l.n                                          \
)

#define kl_remove(l, i) (                                           \
    (i->p != SIZE_MAX) ? (kv_A(l.a, i->p).n = i->n) : (l.h = i->n), \
    (i->n != SIZE_MAX) ? (kv_A(l.a, i->n).p = i->p) : (l.t = i->p), \
    --l.n                                                           \
)

#define kl_foreach(it, l) \
    for (kliter_t(l) _i = kl_begin(l), _flag = (typeof(_flag))0x1l; _flag && _i; _i = kl_next(l, _i), _flag = (typeof(_flag))(uintptr_t)!_flag) \
        for ([[ maybe_unused ]] it = _i; _flag; _flag = (typeof(_flag))(uintptr_t)!_flag)

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
