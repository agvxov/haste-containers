#include <stdio.h>
#include "kvec.h"

signed main(void) {
    kvec_t(int) v;
    kv_init(v);

    kv_push(v, 1);
    kv_push(v, 2);
    kv_push(v, 4);

    for (int i = 0; i < kv_size(v); i++) {
        printf("%d, ", kv_A(v, i));
    }
    puts("");

    kv_destroy(v);

    return 0;
}
