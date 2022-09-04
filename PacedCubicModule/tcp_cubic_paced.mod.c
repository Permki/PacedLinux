#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
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
__used
__attribute__((section("__versions"))) = {
	{ 0x1810ae6, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xa225b92a, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0xead8c0dd, __VMLINUX_SYMBOL_STR(tcp_reno_undo_cwnd) },
	{ 0x6d155615, __VMLINUX_SYMBOL_STR(tcp_unregister_congestion_control) },
	{ 0xfde4f886, __VMLINUX_SYMBOL_STR(tcp_register_congestion_control) },
	{ 0x9a37ee, __VMLINUX_SYMBOL_STR(tcp_cong_avoid_ai) },
	{ 0x7f02188f, __VMLINUX_SYMBOL_STR(__msecs_to_jiffies) },
	{ 0xc4f28797, __VMLINUX_SYMBOL_STR(tcp_slow_start) },
	{ 0xac9657d8, __VMLINUX_SYMBOL_STR(ktime_get_with_offset) },
	{ 0x15ba50a6, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "EAC252814D8174793D01B4B");
