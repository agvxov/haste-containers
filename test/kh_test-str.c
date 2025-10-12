// @BAKE gcc -o $*.out $@ -std=c23 -ggdb
#include <stdio.h>
#include "khash.h"

int main(void) {
    khash(char *, char *) h;
    kh_init(h);

    kh_A(h, "10") = "1";
    kh_A(h, "20") = "2";
    kh_A(h, "40") = "4";

    if (kh_exists(h, "30")) {
        printf("found key '30'\n");
    }


    kh_foreach(auto i, h) {
        printf("%s : %s, ", kh_key(h, i), kh_val(h, i));
    }

    puts("");

    kh_destroy(h);
    return 0;
}
