OPTIONAL_CFLAGS = -Wno-unused-parameter
CFLAGS          = -O2 -march=native -Wall -Wextra -lm $(OPTIONAL_CFLAGS)

# .o
OBJFILES = build/vector3.o build/sphere.o

all:
	gcc -c src/libs/vector3.c -o build/vector3.o $(CFLAGS)
	gcc -c src/libs/shapes/sphere.c -o build/sphere.o $(CFLAGS)

	gcc $(OBJFILES) src/main.c -o main $(CFLAGS)

.PHONY: clean
clean:
	-rm build/*.o