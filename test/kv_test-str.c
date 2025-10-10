#include <stdio.h>
#include "kvec.h"

signed main(void) {
    kvec_t(char*) v;
    kv_init(v);

    kv_push(v, "a");
    kv_push(v, "b");
    kv_push(v, "c");

    for (int i = 0; i < kv_size(v); i++) {
        printf("%s, ", kv_A(v, i));
    }
    puts("");

    kv_destroy(v);

    return 0;
}
