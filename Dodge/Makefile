PLATFORM = linux
BASE_DIR = src
INCL = -I/usr/include -Iinclude
include src/Makefile.inc
CC = g++
FLAGS = -std=c++0x -g -Wall -DDEBUG
OUT = lib/libDodge.a

all: $(OBJS)
	ar -rc $(OUT) $(OBJS)

$(OBJS): %.o: %.cpp
	$(CC) $(FLAGS) -c $(INCL) $< -o $@

clean:
	find . -name *.o -exec rm {} \;
	rm -f $(OUT)
