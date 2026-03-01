#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x20\x00\x00\x00\x51\x39\x19\x70"
	"input_allocate_device\0\0\0"
	"\x10\x00\x00\x00\xd8\x7e\x99\x92"
	"_printk\0"
	"\x20\x00\x00\x00\xf9\xa3\xc3\xe9"
	"input_register_device\0\0\0"
	"\x1c\x00\x00\x00\x4a\x97\x3b\x64"
	"input_free_device\0\0\0"
	"\x20\x00\x00\x00\x35\xed\xca\xf3"
	"input_register_handler\0\0"
	"\x20\x00\x00\x00\x9d\x41\xb2\x69"
	"input_unregister_device\0"
	"\x24\x00\x00\x00\x27\x2f\xbe\x8d"
	"input_unregister_handler\0\0\0\0"
	"\x18\x00\x00\x00\x59\xa6\x03\x33"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B6305FFA42226C0855C10A5");
