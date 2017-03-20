SUBDIRS:=lib_network lib_utils lib_thread

libs:$(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@;

clean:
	$(MAKE) -C lib_network  clean
	$(MAKE) -C lib_utils    clean
	$(MAKE) -C lib_thread   clean

install:
	mv ./lib_network/libnetwork.so ../../_build/.lib
	mv ./lib_utils/libutils.so ../../_build/.lib
	mv ./lib_thread/libthread.so ../../_build/.lib

.PHONY: libs  $(SUBDIRS) clean
