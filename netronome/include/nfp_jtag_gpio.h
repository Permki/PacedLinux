
#ifndef __JTAG_CONTROLLER_INTERFACE_H__
#define __JTAG_CONTROLLER_INTERFACE_H__
/******************************************************************************/
struct jtag_ctrl_intf {
	/*
	 *   tms: bits to clock out
	 *   tdo: bit0 will contain din(dout of chain) after tms clocking
	 *   tdi: value to present on dout(din of chain) while clocking tms
	 *   cnt: bit-count(== clock-cycles) to clock
	 */
	int (*clock_tms)(struct jtag_ctrl_intf *ji,
			 uint32_t *tms,
			 uint8_t *tdo,
			 uint32_t tdi,
			 uint32_t cnt);

	/*
	 *   tms: value to present on tms while clocking dout/din
	 *   tdi: bits to clock out to chain
	 *   tdo: bits clocked in from chain
	 *   cnt: bit-count(== clock-cycles) to clock
	 */
	int (*clock_din_dout)(struct jtag_ctrl_intf *ji,
			      uint32_t tms,
			      uint8_t *tdi,
			      uint8_t *tdo,
			      uint32_t cnt);
};

/******************************************************************************/
#endif /* __JTAG_CONTROLLER_INTERFACE_H__ */

struct nfp_gpio_jtag_pinmap {
	int32_t tck;
	int32_t tms;
	int32_t tdo;
	int32_t tdi;
	int32_t trst;
	int32_t muxsel;
};

struct jtag_ctrl_intf *nfp_gpio_jtag_init(struct nfp_device *nfp,
					  struct nfp_gpio_jtag_pinmap *map,
					  int32_t freq_hz);

void nfp_gpio_jtag_deinit(struct jtag_ctrl_intf *jci);
