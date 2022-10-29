CC     = gcc
CFLAGS = -Wall -Wextra -O0 -g3

TRGT = showip
SRCS = showip.c
OBJS = $(SRCS:.c=.o)
LIBS = apero

.PHONY: all clean re

all: $(TRGT)
	@rm -f $(OBJS)

$(TRGT): $(OBJS)
	$(CC) $(CFLAGS) $^ -L../libraries -lapero -o $@ 

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	@rm -f $(TRGT) $(OBJS)
	@echo "clean"

re: clean all
