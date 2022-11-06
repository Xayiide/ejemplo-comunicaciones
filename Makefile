SUBDIRS = apero comun showip simpleclient simpleserver

.PHONY: all libs debug clean re $(SUBDIRS)

all: $(SUBDIRS)

libs: apero comun

debug: all

debug: DEBUG=-DDEBUG

$(SUBDIRS):
	$(MAKE) -C $@ debug


clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done

re: clean all

