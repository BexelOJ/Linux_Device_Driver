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
	{ 0x2782e85c, "gpio_to_desc" },
	{ 0xdf2ef134, "gpiod_set_raw_value" },
	{ 0xebe5da4a, "gpio_free" },
	{ 0xe8213e80, "_printk" },
	{ 0x76a97ac1, "gpio_request" },
	{ 0xb14b1ce6, "gpiod_direction_output_raw" },
	{ 0x67628f51, "msleep" },
	{ 0x5cd571fc, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x2782e85c,
	0xdf2ef134,
	0xebe5da4a,
	0xe8213e80,
	0x76a97ac1,
	0xb14b1ce6,
	0x67628f51,
	0x5cd571fc,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"gpio_to_desc\0"
	"gpiod_set_raw_value\0"
	"gpio_free\0"
	"_printk\0"
	"gpio_request\0"
	"gpiod_direction_output_raw\0"
	"msleep\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "519A3DF0E49CBDDFAAE9BC1");
