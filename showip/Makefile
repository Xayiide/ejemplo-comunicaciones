CC     = gcc
CFLAGS = -Wall -Wextra -O0 -g3

TRGT = showip
SRCS = showip.c
OBJS = $(SRCS:.c=.o)
LIBS = apero

.PHONY: all debug clean re

all: $(TRGT)
	@rm -f $(OBJS)

debug: all

debug: DEBUG=-DDEBUG

$(TRGT): $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $^ -L../libraries -lapero -o $@ 

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(DEBUG) -c $^ -o $@

clean:
	@rm -f $(TRGT) $(OBJS)
	@echo "clean"

re: clean all
