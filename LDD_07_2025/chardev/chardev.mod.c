#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe8213e80, "_printk" },
	{ 0xf106214b, "__register_chrdev" },
	{ 0x52b15b3b, "__unregister_chrdev" },
	{ 0xd272d446, "trace_hardirqs_on" },
	{ 0x9c4ed43a, "alt_cb_patch_nops" },
	{ 0xd272d446, "trace_hardirqs_off" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0x5cd571fc, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xe8213e80,
	0xf106214b,
	0x52b15b3b,
	0xd272d446,
	0x9c4ed43a,
	0xd272d446,
	0x90a48d82,
	0x5cd571fc,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"_printk\0"
	"__register_chrdev\0"
	"__unregister_chrdev\0"
	"trace_hardirqs_on\0"
	"alt_cb_patch_nops\0"
	"trace_hardirqs_off\0"
	"__ubsan_handle_out_of_bounds\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "0B522DC037F64FBED4B46C2");
