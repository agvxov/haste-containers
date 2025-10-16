#include <stdio.h>
#include "hlist.h"

signed main(void) {
    hlist_t(int) l;
    hl_init(l);

    hl_add(l, 1);
    hl_add(l, 2);
    hl_add(l, 3);
    hl_add(l, 4);

    hl_remove(l, hl_next(l, hl_begin(l)));

    for (hliter_t(l) i = hl_begin(l); i; i = hl_next(l, i)) {
        printf("%d, ", hl_d(i));
    }
    puts("");

    hl_delete(l);

    return 0;
}
