BIN=CRectangle
CC=gcc
CXX=g++
CFLAGS=-Wall
CXXFLAGS=-Wall
RM=rm -Rf
SRCS=class_ex1_class.c class_main.c
OBJS=$(SRCS:.c=.o)

all: clean build

clean: 
	$(RM) *.o $(BIN)

build: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(BIN)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
