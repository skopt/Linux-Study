vpath %.c src
vpath %.h include
apple:apple.o
	cc -o $@ apple.o
apple.o:apple.c apple.h
	cc -c $< -Iinclude
clean:
	rm apple apple.o
