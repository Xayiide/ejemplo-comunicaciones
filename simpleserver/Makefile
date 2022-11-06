CC     = gcc
CFLAGS = -Wall -Wextra -O0 -g3
LFLAGS = -L../libraries -lapero -lcomun

TRGT = prueba 
SRCS = prueba.c simpleserver.c
OBJS = $(SRCS:.c=.o)
LIBS = apero

.PHONY: all debug clean re

all: $(TRGT)
	@rm -f $(OBJS)

debug: all

debug: DEBUG=-DDEBUG

$(TRGT): $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $^ $(LFLAGS) -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) -c $^ $(LFLAGS) -o $@

clean:
	@rm -f $(TRGT) $(OBJS)
	@echo "clean"

re: clean all
