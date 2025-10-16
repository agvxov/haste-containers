#include <stdio.h>
#include "klist.h"

signed main(void) {
    klist_t(int) l;
    kl_init(l);

    kl_add(l, 1);
    kl_add(l, 2);
    kl_add(l, 3);
    kl_add(l, 4);

    kl_remove(l, kl_next(l, kl_begin(l)));

    for (kliter_t(l) i = kl_begin(l); i; i = kl_next(l, i)) {
        printf("%d, ", kl_d(i));
    }
    puts("");

    kl_delete(l);

    return 0;
}
