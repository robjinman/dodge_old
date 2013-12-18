NAME = a.out
OUT_DIR = .
CC = g++
CFLAGS = -std=c++0x -Wall -DGLEW -DDEBUG -g
INCL = -I../../include -I../../include/SDL
LIBS = -L../../lib -L/usr/lib -lDodge -lX11 -lGL -lGLEW -lpnglite -lz
SRC = .
OBJS = $(SRC)/main.o \
	$(SRC)/Test.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(OUT_DIR)/$(NAME) $(LIBS)

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCL) $< -o $@

clean:
	rm -f $(SRC)/*.o
	rm -f $(OUT_DIR)/$(NAME)

