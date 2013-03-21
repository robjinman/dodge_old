DODGE_LOCATION = ..

NAME = demo
CC = g++
CFLAGS = -std=c++0x -O3 -Wall `sdl-config --cflags` -DLINUX -g -DDEBUG
INCL = -I$(DODGE_LOCATION)/Dodge/include
LIBS = -L$(DODGE_LOCATION)/Dodge/lib -L/usr/lib -lDodge -lX11 -lGLESv2 -lEGL -lpnglite -lz -lBox2D `sdl-config --libs` -lSDLmain -lSDL -lSDL_mixer
SRC = .
OBJS = $(SRC)/main.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LIBS)

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCL) $< -o $@

clean:
	rm -f $(SRC)/*.o
