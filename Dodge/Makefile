PLATFORM = linux
GL_MODE = 
BASE_DIR = src
INCL = -Iinclude -Iinclude/dodge
include src/Makefile.inc
CC = g++
FLAGS = -std=c++0x `sdl-config --cflags` -O3 -Wall -DLINUX -DGLEW -g -DDEBUG
OUT = lib/libDodge.a

all: $(OBJS)
	ar -rc $(OUT) $(OBJS)

$(OBJS): %.o: %.cpp
	$(CC) $(FLAGS) -c $(INCL) $< -o $@

clean:
	find . -name *.o -exec rm {} \;
	rm -f $(OUT)
