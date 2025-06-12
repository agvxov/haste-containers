#ifndef AC_KLIST_H
#define AC_KLIST_H

#include <kvec.h>

// XXX
#define kv_pushp_(type, v) (((v).n == (v).m)?							\
						   ((v).m = ((v).m? (v).m<<1 : 2),				\
							(v).a = (void*)(type*)realloc((v).a, sizeof(type) * (v).m), 0)	\
						   : 0), ((v).a + ((v).n++))

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#define klnode_t(type) struct { type d; size_t p, n; }
#define kliter_t(type) klnode_t(type)*
#define klist_t(type) struct { size_t h, t; size_t n; kvec_t(klnode_t(type)) a; }

#define kl_init(l) (l.h = l.t = SIZE_MAX, l.n = 0, kv_init(l.a))
#define kl_destroy(l) kv_destroy(l.a)

#define kl_begin(l)    (void*)(l.h  != SIZE_MAX ? &kv_A(l.a, l.h)  : NULL)
#define kl_end(l)      (void*)(l.t  != SIZE_MAX ? &kv_A(l.a, l.t)  : NULL)
#define kl_next(l, i)  (void*)(i->n != SIZE_MAX ? &kv_A(l.a, i->n) : NULL)
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
