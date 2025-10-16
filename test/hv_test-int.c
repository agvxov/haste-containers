#include <stdio.h>
#include "hvec.h"

signed main(void) {
    hvec_t(int) v;
    hv_init(v);

    hv_add(v, 1);
    hv_add(v, 2);
    hv_add(v, 4);

    for (size_t i = 0; i < hv_size(v); i++) {
        printf("%d, ", hv_A(v, i));
    }
    puts("");

    hv_delete(v);
    return 0;
}
