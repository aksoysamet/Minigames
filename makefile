# This file demonstrates how to compile the SKY project on Linux.
#
# To compile SKY do:
#
# make SKY
#

GPP = g++ -m32 -std=gnu++0x -Ilib -fno-stack-protector
GCC = gcc -m32
SKY_OUTFILE = "./minigames.so"

COMPILE_FLAGS = -g -O3 -c -fPIC -w -DLINUX

SKY = -D SKY $(COMPILE_FLAGS)

all: SKY

clean:
	-rm -f *~ *.o *.so

SKY: clean
	$(GPP) $(SKY) ./lib/SDK/*.cpp
	$(GPP) $(SKY) ./src/*.cpp
	$(GPP) $(SKY) ./src/*.c
	$(GCC) -g -nodefaultlibs -fshort-wchar -shared -o $(SKY_OUTFILE) *.o
