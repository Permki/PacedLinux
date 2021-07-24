#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xf654425, "module_layout" },
	{ 0x658dec53, "param_ops_int" },
	{ 0xb91451a5, "tcp_reno_undo_cwnd" },
	{ 0xf0f736e4, "tcp_unregister_congestion_control" },
	{ 0x807c4a8d, "tcp_register_congestion_control" },
	{ 0x46793187, "tcp_cong_avoid_ai" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0xe1df4fb0, "tcp_slow_start" },
	{ 0xc4f0da12, "ktime_get_with_offset" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "AB7E5A1B31C30535769B442");
