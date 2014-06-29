vpath %.h apple/include
vpath %.c apple/src

OUT=out

$(OUT)/apple.o:apple.c apple.h
	cc -c $< -Iapple/include -o $@
