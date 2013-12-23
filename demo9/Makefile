DODGE_LOCATION = ..

NAME = demo
CC = g++
CFLAGS = -std=c++0x -Wall `sdl-config --cflags` -DLINUX -DGLEW -g -DDEBUG
INCL = -I$(DODGE_LOCATION)/Dodge/include
LIBS = -L$(DODGE_LOCATION)/Dodge/lib -L/usr/lib -lDodge -lX11 -lGL -lGLEW -lpnglite -lBox2D -lz `sdl-config --libs` -lSDLmain -lSDL -lSDL_mixer
SRC = src
OBJS = $(SRC)/Block.o \
	$(SRC)/Game.o \
	$(SRC)/main.o \
	$(SRC)/Platform.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LIBS)

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCL) $< -o $@

clean:
	rm -f $(SRC)/*.o
