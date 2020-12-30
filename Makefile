CC=clang
CFLAGS=-Ofast

build:
	$(CC) $(CFLAGS) main.c math_lib.c rt.c -lm -o rt

clean:
	rm -f rt test.png
