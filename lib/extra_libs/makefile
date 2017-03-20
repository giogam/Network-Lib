SUBDIRS:= lib_session

libs:$(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@;

clean:
	$(MAKE) -C lib_session  clean

install:
		mv ./lib_session/libsession.so   ../../_build/.lib

.PHONY: libs  $(SUBDIRS) clean
