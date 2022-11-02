SUBDIRS = apero showip simpleclient simpleserver

.PHONY: all debug clean re $(SUBDIRS)

all: $(SUBDIRS)

debug: all

debug: DEBUG=-DDEBUG

$(SUBDIRS):
	$(MAKE) -C $@ debug


clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

re: clean all

