all:
	gcc -c -DGLEW_STATIC -Iinclude src/glew.c -o glew.o
	ar rc lib/libglew.a glew.o

clean:
	rm -f glew.o libglew.a

install:

distclean: clean

install-strip:

.PHONY: distclean
