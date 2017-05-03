#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xae4b0ceb, "module_layout" },
	{ 0xe61a6d2f, "gpio_unexport" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x12c554f0, "cdev_del" },
	{ 0x82f776b7, "gpio_export" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xea01506a, "cdev_add" },
	{ 0x6f395f2, "cdev_alloc" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xf9a482f9, "msleep" },
	{ 0x432fd7f6, "__gpio_set_value" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x695b2cfd, "down_interruptible" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x27e1a049, "printk" },
	{ 0xc9f5df35, "up" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "3AD458D9AA41A0FA5798F79");
