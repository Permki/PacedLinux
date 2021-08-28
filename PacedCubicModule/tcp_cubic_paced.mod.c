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
	{ 0x4cd18aac, "tcp_reno_undo_cwnd" },
	{ 0x92454004, "tcp_unregister_congestion_control" },
	{ 0x6a79872, "tcp_register_congestion_control" },
	{ 0x7b2b2fb1, "tcp_cong_avoid_ai" },
	{ 0xf1969a8e, "__usecs_to_jiffies" },
	{ 0xb7d440a5, "tcp_slow_start" },
	{ 0x837b7b09, "__dynamic_pr_debug" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "29025C1007D27368F909F41");
