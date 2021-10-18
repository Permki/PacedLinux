#include <errno.h>
#include <stdio.h>

#include <nfp.h>
#include <nfp_nffw.h>

/* these are largely thing used by the loader itself, as so are hidden
 * behind the load/unload APIs; however a non-remoted loaded will fail
 * without these.
 * we just have stubs for completeness
 */



int nfp_nffw_info_fw_modular_set(struct nfp_device *nfp, int is_modular)
{
    errno = ENOSYS;
    return -1;
}

uint8_t nfp_nffw_info_fwid_alloc(struct nfp_device *nfp)
{
    errno = ENOSYS;
    return 0;
}

int nfp_nffw_info_fwid_free(struct nfp_device *nfp, uint8_t fwid)
{
    errno = ENOSYS;
    return -1;
}

int nfp_nffw_info_fw_mip_set(struct nfp_device *nfp, uint8_t fwid,
                             uint32_t cpp_id, uint64_t off)
{
    errno = ENOSYS;
    return -1;
}

int nfp_nffw_info_fw_debugger_attached_set(struct nfp_device *nfp,
                                           int is_attached)
{
    errno = ENOSYS;
    return -1;
}

int nfp_nffw_info_fw_loaded_set(struct nfp_device *nfp, int is_loaded)
{
    errno = ENOSYS;
    return -1;
}
