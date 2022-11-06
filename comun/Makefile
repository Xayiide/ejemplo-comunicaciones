CC     = gcc
CFLAGS = -Wall -Wextra -O0 -g3

AR     = ar
AFLAGS = rvs

TRGT = libcomun.a
SRCS = comun.c
OBJS = $(SRCS:.c=.o)
HDRS = $(SRCS:.c=.h)

.PHONY: all debug clean re


all: $(TRGT)
	cp $(TRGT) ../libraries/
	cp inc/$(HDRS) ../include/

debug: all

debug: DEBUG=-DDEBUG

$(TRGT): $(OBJS)
	$(AR) $(AFLAGS) $(TRGT) $(OBJS)
	@rm -f $(OBJS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(DEBUG) -c $^

clean:
	@rm -f $(TRGT) $(OBJS)
	@echo "clean"


re: clean all