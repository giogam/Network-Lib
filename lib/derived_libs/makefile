SUBDIRS:= lib_routines lib_games

libs:$(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@;

clean:
	$(MAKE) -C lib_routines clean
	$(MAKE) -C lib_games clean

install:
		mv ./lib_routines/libroutines.so ../../_build/.lib
		mv ./lib_games/libgames.so ../../_build/.lib

.PHONY: libs  $(SUBDIRS) clean
