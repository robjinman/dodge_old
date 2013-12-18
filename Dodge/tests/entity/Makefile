NAME = test
CC = g++
CFLAGS = -std=c++0x -Wall `sdl-config --cflags` -DLINUX -DGLEW -g -DDEBUG
INCL = -I../../include -I../../include/SDL
LIBS = -L../../lib -L/usr/lib -lDodge -lX11 -lGL -lGLEW -lz
OBJS = main.o \
	Test.o \
	TestCase0.o \
	TestCase1.o \
	TestCase2.o \
	TestCase3.o \
	TestCase4.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LIBS)

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCL) $< -o $@

clean:
	rm -f $(NAME)
	rm -f *.o
