SUBDIRS:=00-maxServer 01-charCount 02-calc 03-admiCtrl 04-DS 05-FTP 06-TTT 07-chat 08-TTT-FT cardGame

libs:$(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@;

clean:
	$(MAKE) -C 00-maxServer clean
	$(MAKE) -C 01-charCount clean
	$(MAKE) -C 02-calc 			clean
	$(MAKE) -C 03-admiCtrl 	clean
	$(MAKE) -C 04-DS 				clean
	$(MAKE) -C 05-FTP 			clean
	$(MAKE) -C 06-TTT 			clean
	$(MAKE) -C 07-chat			clean
	$(MAKE) -C 08-TTT-FT 	  clean
	$(MAKE) -C cardGame 	  clean


install:
	$(MAKE) -C 00-maxServer install
	$(MAKE) -C 01-charCount install
	$(MAKE) -C 02-calc 			install
	$(MAKE) -C 03-admiCtrl 	install
	$(MAKE) -C 04-DS			 	install
	$(MAKE) -C 05-FTP			 	install
	$(MAKE) -C 06-TTT			 	install
	$(MAKE) -C 07-chat			install
	$(MAKE) -C 08-TTT-FT		install
	$(MAKE) -C cardGame 	  install


.PHONY: libs clean install $(SUBDIRS)
