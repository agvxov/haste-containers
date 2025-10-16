.PHONY: test
.PRECIOUS: %-c.out

HHEADERS := hvec.h hlist.h hstack.h hringbuffer.h hmap.h

test: \
	test/hv_test-int-c.out test/hv_test-int-cpp.out \
	test/hv_test-str-c.out test/hv_test-str-cpp.out \
	test/hl_test-c.out     test/hl_test-cpp.out     \
	test/hrb_test-c.out    test/hrb_test-cpp.out    \
	test/hh_test-int-c.out test/hh_test-int-cpp.out \
	test/hh_test-str-c.out test/hh_test-str-cpp.out

%-c.out: %.c ${KHEADERS}
	gcc -I./ -o $@ $< -std=c23 -Wall -ggdb
	./$@

%-cpp.out: %.c ${KHEADERS}
	g++ -I./ -o $@ $< -std=c++23 -Wall -ggdb
	./$@

clean:
	-rm test/*.out
	-rm *.out
