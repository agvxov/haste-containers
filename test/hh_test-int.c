#include <stdio.h>
#include "hmap.h"

int main(void) {
    hmap(int32_t, int32_t) h;
    hm_init(h);

    hm_a(h, 10) = 1;
    hm_a(h, 20) = 2;
    hm_a(h, 40) = 4;

    if (hm_exists(h, 30)) {
        printf("found key '30'\n");
    }

    hm_foreach(auto i, h) {
        printf("%u : %d, ", hm_key(h, i), hm_val(h, i));
    }

    puts("");

    hm_delete(h);
    return 0;
}
