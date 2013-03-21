NAME = demo
CC = g++
CFLAGS = -std=c++0x -Wall -g -DDEBUG
INCL = -I../Dodge/include -I../Dodge/include/SDL
LIBS = -L../Dodge/lib -L/usr/lib -lDodge -lX11 -lGLESv2 -lEGL -lpnglite -lz -lBox2D
OBJS = Application.o \
	CParallaxSprite.o \
	CPhysicalEntity.o \
	CPhysicalSprite.o \
	CSprite.o \
	Item.o \
	main.o \
	Player.o \
	Soil.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LIBS)

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCL) $< -o $@

clean:
	rm -f $(NAME)
	rm -f *.o
