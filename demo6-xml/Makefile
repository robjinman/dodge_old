NAME = demo
CC = g++
CFLAGS = -std=c++0x -Wall -O3 -g -DDEBUG
INCL = -I../Dodge/include
LIBS = -L../Dodge/lib -L/usr/lib -lDodge -lX11 -lGLESv2 -lEGL -lpnglite -lz
OBJS = main.o \
	Game.o \
	Item.o \
	Player.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LIBS)

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCL) $< -o $@

clean:
	rm -f $(NAME)
	rm -f *.o
