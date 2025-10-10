.PHONY: test

test: \
	test/kv_test-int-c.out test/kv_test-int-cpp.out \
	test/kv_test-str-c.out test/kv_test-str-cpp.out \
	test/kl_test-c.out test/kl_test-cpp.out

%-c.out: %.c
	gcc -I./ -o $@ $+ -std=c23
	./$@

%-cpp.out: %.c
	g++ -I./ -o $@ $+ -std=c++23
	./$@

clean:
	-rm test/*.out
	-rm *.out
