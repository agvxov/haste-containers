#include <stdio.h>
#include "kringbuffer.h"

kringbuffer_t(int, 20) b;

signed main(void) {
    krb_init(b);

    for (int i = 0; i < 100; i++) {
        krb_push(b, i);
        printf("%d, ", krb_top(b));
    }

    puts("");

    for (int i = 0; i < krb_size(b); i++) {
        printf("%d, ", krb_A(b, i));
    }

    puts("");

    return 0;
}
