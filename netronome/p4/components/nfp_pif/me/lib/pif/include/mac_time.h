#ifndef __MAC_TIME_H__
#define __MAC_TIME_H__

struct mac_time_state {
    uint32_t mac_time_s; /* last synced mac time seconds */
    uint32_t mac_time_ns; /* last synced mac time nanoseconds */
    uint32_t me_time; /* me cycle time at last sync */
    uint16_t conv_mult; /* multiplier to convert me time to ns */
    uint16_t conv_rshift; /* right shift to convert me time to ns */
};

struct mac_time_data {
    uint32_t sec;
    uint32_t nsec;
};

/* call in a house keeping thread periodically
 * should be called frequently enough that 32-bit
 * timer wrapping isn't a consideration
 * this should also be done often enough that the ns to me cycle
 * conversion inaccuracy doesn't drift too much
 */
void mac_time_update();

/* retrieve the current mac time */
void mac_time_fetch(__xread struct mac_time_state *mac_time_xfer);

/* calculate a 64-bit time based on current me time and last synced mac time */
struct mac_time_data mac_time_calc(struct mac_time_state mac_time_buf);

/* calculate a 64-bit time based on current me time and last synced mac time */
struct mac_time_data mac_time_calc_pkt(struct mac_time_state mac_time_buf,
                                       uint32_t pkt_time_ns);

/* shift and multiple to convert me cycles to nanoseconds,
 * populated by host
 */
extern volatile __export __addr40 __ctm uint32_t mac_time_mult;
extern volatile __export __addr40 __ctm uint32_t mac_time_rshift;

#endif /* __MAC_TIME_H__ */
