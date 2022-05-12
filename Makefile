

test : test.cpp
	g++ -o $@ -O2 -Warray-bounds -Werror $<
