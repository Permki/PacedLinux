
extern const char *fw_default;

int nfp_nsp_fw_load(struct nfp_device *nfp, const char *firmware, void **buffer, size_t *buffer_size);
int nfp_nsp_fw_exec(struct nfp_device *nfp, void *buffer, size_t size);
