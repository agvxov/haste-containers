#include <stdio.h>
#include "kvec.h"

signed main(void) {
    kvec_t(int) v;
    kv_init(v);

    kv_add(v, 1);
    kv_add(v, 2);
    kv_add(v, 4);

    for (size_t i = 0; i < kv_size(v); i++) {
        printf("%d, ", kv_A(v, i));
    }
    puts("");

    kv_delete(v);
    return 0;
}
