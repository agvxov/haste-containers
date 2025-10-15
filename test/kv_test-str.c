#include <stdio.h>
#include "kvec.h"

signed main(void) {
    kvec_t(const char*) v;
    kv_init(v);

    kv_add(v, "a");
    kv_add(v, "b");
    kv_add(v, "c");

    for (size_t i = 0; i < kv_size(v); i++) {
        printf("%s, ", kv_A(v, i));
    }
    puts("");

    kv_delete(v);
    return 0;
}
