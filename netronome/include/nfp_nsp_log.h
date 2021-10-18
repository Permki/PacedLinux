#ifndef __NFP_NSP_LOG_H__
#define __NFP_NSP_LOG_H__

struct nsp_log_entry {
	uint64_t tc_us;
	uint64_t dur_us;
	uint16_t type;
	uint16_t rsvd1;
	uint32_t data1;
	uint32_t data2;
	uint32_t rsvd2;
} __attribute__ ((__packed__));

struct nfp_nsp_log {
	struct nsp_log_entry *log;
	uint32_t size;
	uint32_t dim;
	uint32_t cnt;
};

int nsp_log_init(struct nfp_device *nfp, struct nfp_nsp_log *nl);
int nsp_log(struct nfp_device *nfp, uint64_t tc_us, uint64_t dur_us,
	    uint16_t type, uint32_t data1, uint32_t data2);

void nsp_log_info(struct nfp_nsp_log *nl);
void nsp_log_show(struct nfp_nsp_log *nl, int start, int cnt);
const char *nsp_log_entry_name(struct nsp_log_entry *e);
void nsp_log_clear(struct nfp_nsp_log *nl);

#endif // __NFP_NSP_LOG_H__
