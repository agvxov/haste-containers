#include <stdio.h>
#include "hringbuffer.h"

hringbuffer_t(int, 20) b;

signed main(void) {
    hrb_init(b);

    for (int i = 0; i < 100; i++) {
        hrb_push(b, i);
        printf("%d, ", hrb_top(b));
    }

    puts("");

    for (size_t i = 0; i < hrb_size(b); i++) {
        printf("%d, ", hrb_A(b, i));
    }

    puts("");

    return 0;
}
