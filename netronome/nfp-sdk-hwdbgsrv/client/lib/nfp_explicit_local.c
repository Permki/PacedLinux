#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nfp.h>
#include <nfp_cpp.h>
#include <nfp_cpp_internal.h>
#include <nfp_explicit.h>

struct nfp_cpp_explicit {
    struct nfp_cpp *cpp; /* master cpp handle */
    struct nfp_cpp_explicit_data_internal cmd_data;
    uint8_t push_data[128];
    int push_len;
#define OPT_TGT (1 << 0)
#define OPT_DAT (1 << 1)
#define OPT_SIG (1 << 2)
#define OPT_PST (1 << 3)
    int opt_set;
};

#define REQUIRED_OPTS (OPT_TGT | OPT_PST)

struct nfp_cpp_explicit *nfp_cpp_explicit_acquire(struct nfp_cpp *cpp)
{
    struct nfp_cpp_explicit *ret;

    if (!cpp) {
        errno = EINVAL;
        return NULL;
    }

    ret = malloc(sizeof(struct nfp_cpp_explicit));
    if (!ret) {
        errno = ENOMEM;
        return NULL;
    }

    ret->cpp = cpp;
    ret->push_len = -1;
    ret->opt_set = 0;

    memset(&ret->cmd_data, '\0', sizeof(ret->cmd_data));

    /* these are fields that may not be used by the caller */
    ret->cmd_data.pull_len = -1;
    ret->cmd_data.posted = -1;
    ret->cmd_data.data_master_island = 0;

    /* these value have 0 as default, which the sim will populate */
    ret->cmd_data.data_ref = 0;
    ret->cmd_data.data_master = 0;
    ret->cmd_data.signal_master = 0;
    ret->cmd_data.signal_ref = 0;

    return ret;
}

int nfp_cpp_explicit_set_target(struct nfp_cpp_explicit *expl, uint32_t cpp_id, uint8_t len, uint8_t mask)
{
    if (!expl) {
        errno = EINVAL;
        return -1;
    }

    expl->cmd_data.cpp_id = cpp_id;
    expl->cmd_data.length = len;
    expl->cmd_data.bytemask = mask;
    expl->opt_set |= OPT_TGT;
    return 0;
}

int nfp_cpp_explicit_set_data(struct nfp_cpp_explicit *expl, uint8_t data_master, uint16_t data_ref)
{
    if (!expl) {
        errno = EINVAL;
        return -1;
    }

    expl->cmd_data.data_master = data_master;
    expl->cmd_data.data_ref = data_ref;
    expl->opt_set |= OPT_DAT;
    return 0;
}

int nfp_cpp_explicit_set_signal(struct nfp_cpp_explicit *expl, uint8_t signal_master, uint8_t signal_ref)
{
    if (!expl) {
        errno = EINVAL;
        return -1;
    }

    expl->cmd_data.signal_master = signal_master;
    expl->cmd_data.signal_ref = signal_ref;
    expl->opt_set |= OPT_SIG;

    return 0;
}

int nfp_cpp_explicit_set_posted(struct nfp_cpp_explicit *expl,
                                int posted,
                                uint8_t siga,
                                enum nfp_cpp_explicit_signal_mode siga_mode,
                                uint8_t sigb,
                                enum nfp_cpp_explicit_signal_mode sigb_mode)
{
    if (!expl) {
        errno = EINVAL;
        return -1;
    }

    expl->cmd_data.posted = posted;
    expl->cmd_data.siga = siga;
    expl->cmd_data.siga_mode = siga_mode;
    expl->cmd_data.sigb = sigb;
    expl->cmd_data.sigb_mode = sigb_mode;
    expl->opt_set |= OPT_PST;
    return 0;
}

int nfp_cpp_explicit_put(struct nfp_cpp_explicit *expl, const void *buff, size_t len)
{
    /* we check for these flags */
    int opt = (OPT_TGT);

    if (!buff || !expl || len > 128) {
        errno = EINVAL;
        return -1;
    }

    if ((expl->opt_set & opt) != opt) {
        errno = EINVAL;
        return -1;
    }

    memcpy(expl->cmd_data.pull_data, buff, len);
    expl->cmd_data.pull_len = len;

    return 0;
}

int nfp_cpp_explicit_do(struct nfp_cpp_explicit *expl, uint64_t address)
{
    int opt = REQUIRED_OPTS;
    int push_len;
    int ret;

    if (!expl) {
        errno = EINVAL;
        return -1;
    }

    if ((expl->opt_set & opt) != opt) {
        errno = EINVAL;
        return -1;
    }

    expl->cmd_data.address = address;

    ret = nfp_cpp_explicit_do_internal(expl->cpp,
                                       &expl->cmd_data,
                                       expl->push_data,
                                       sizeof(expl->push_data),
                                       &push_len);
    if (ret >= 0)
        expl->push_len = push_len;

    return ret;
}

int nfp_cpp_explicit_get(struct nfp_cpp_explicit *expl, void *buff, size_t len)
{
    int opt = REQUIRED_OPTS;

    if (!buff || !expl || len > 128) {
        errno = EINVAL;
        return -1;
    }

    if ((expl->opt_set & opt) != opt) {
        errno = EINVAL;
        return -1;
    }

    /* nothing has happened yet */
    if (expl->push_len < 0) {
        errno = EBUSY;
        return -1;
    }

    memcpy(buff, expl->push_data, len);

    return 0;
}

void nfp_cpp_explicit_release(struct nfp_cpp_explicit *expl)
{
    if (!expl)
        return;

    free(expl);
}

struct nfp_cpp *nfp_cpp_explicit_cpp(struct nfp_cpp_explicit *expl)
{
    if (!expl)
        return NULL;

    return expl->cpp;
}
