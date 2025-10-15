.PHONY: test
.PRECIOUS: %-c.out

KHEADERS := kvec.h klist.h kstack.h kringbuffer.h khash.h

test: \
	test/kv_test-int-c.out test/kv_test-int-cpp.out \
	test/kv_test-str-c.out test/kv_test-str-cpp.out \
	test/kl_test-c.out test/kl_test-cpp.out \
	test/krb_test-c.out test/krb_test-cpp.out \
	test/kh_test-int-c.out test/kh_test-int-cpp.out \
	test/kh_test-str-c.out test/kh_test-str-cpp.out

%-c.out: %.c ${KHEADERS}
	gcc -I./ -o $@ $< -std=c23 -Wall -ggdb
	./$@

%-cpp.out: %.c ${KHEADERS}
	g++ -I./ -o $@ $< -std=c++23 -Wall -ggdb
	./$@

clean:
	-rm test/*.out
	-rm *.out
