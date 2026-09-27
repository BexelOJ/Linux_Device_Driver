obj-m += ldd_20261001_moduleDependencyProvider.o
obj-m += ldd_20261001_moduleDependencyConsumer.o

KBUILD_EXTRA_SYMBOLS := \
/home/exin/Workspace/LDD/LDD_2026/LDD_10_2026/ldd_20261001_symbolExport/Module.symvers

all:
	make -C /lib/modules/$(shell uname -r)/build \
	M=$(PWD) \
	KBUILD_EXTRA_SYMBOLS=$(KBUILD_EXTRA_SYMBOLS) \
	modules

clean:
	make -C /lib/modules/$(shell uname -r)/build \
	M=$(PWD) clean