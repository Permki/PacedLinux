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
	{ 0x50b8a407, "tcp_reno_undo_cwnd" },
	{ 0xfa2de51b, "tcp_unregister_congestion_control" },
	{ 0x47665993, "tcp_register_congestion_control" },
	{ 0xcd730a6a, "tcp_cong_avoid_ai" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0xdd47795b, "tcp_slow_start" },
	{ 0xc4f0da12, "ktime_get_with_offset" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "9747EC28D9D40C430F9F246");
