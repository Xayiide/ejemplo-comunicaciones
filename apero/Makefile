CC     = gcc
CFLAGS = -Wall -Wextra -O0 -g3

AR     = ar
AFLAGS = rvs

TRGT = libapero.a
SRCS = apero.c
OBJS = $(SRCS:.c=.o)
HDRS = $(SRCS:.c=.h)

.PHONY: all clean re


all: $(TRGT)
	cp $(TRGT) ../showip/lib/
	cp inc/$(HDRS) ../showip/inc/

$(TRGT): $(OBJS)
	$(AR) $(AFLAGS) $(TRGT) $(OBJS)
	@rm -f $(OBJS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $^

clean:
	@rm -f $(TRGT) $(OBJS)
	@echo "clean"


re: clean all