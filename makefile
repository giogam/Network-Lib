
LIBDIR:=lib
CORE_LIBDIR:=$(LIBDIR)/core_libs
DER_LIBDIR:=$(LIBDIR)/derived_libs
EXTRA_LIBDIR:=$(LIBDIR)/extra_libs

BUILD_DIR:=_build
SRC_DIR:=src

all: pre-build libs main-build

pre-build:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/.lib

libs:
	$(MAKE) -C $(CORE_LIBDIR)
	$(MAKE) -C $(CORE_LIBDIR)   install
	$(MAKE) -C $(DER_LIBDIR)
	$(MAKE) -C $(DER_LIBDIR)    install
	$(MAKE) -C $(EXTRA_LIBDIR)
	$(MAKE) -C $(EXTRA_LIBDIR)  install

main-build:
	$(MAKE) -C $(SRC_DIR)
	$(MAKE) -C $(SRC_DIR) install

clean:
	$(MAKE) -C $(CORE_LIBDIR) clean
	$(MAKE) -C $(DER_LIBDIR) clean
	$(MAKE) -C $(EXTRA_LIBDIR) clean
	$(MAKE) -C $(SRC_DIR) clean

.PHONY: all pre-build libs clean
