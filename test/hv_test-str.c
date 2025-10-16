#include <stdio.h>
#include "hvec.h"

signed main(void) {
    hvec_t(const char*) v;
    hv_init(v);

    hv_add(v, "a");
    hv_add(v, "b");
    hv_add(v, "c");

    for (size_t i = 0; i < hv_size(v); i++) {
        printf("%s, ", hv_A(v, i));
    }
    puts("");

    hv_delete(v);
    return 0;
}
