#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <nfp-common/nfp_platform.h>
#include <nfp-common/ns_log.h>
#include "nfp_client_common.h"

#ifndef NFP_RPC_DEFAULT_URL
#define NFP_RPC_DEFAULT_URL "tcp://localhost:20606"
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

static const char *get_envvar(const char *env_str,
                              char *env_buf, int env_buf_len)
{
    const char *var;
#ifdef _WIN32
    int ret;
    ret = GetEnvironmentVariable(env_str, env_buf, env_buf_len);
    if (ret && ret < env_buf_len)
        var = env_buf;
    else
        var = NULL;
#else
    var = getenv(env_str);
#endif
    return var;
}

char *nfp_get_server_url(int id)
{
    char env_str[1024];
    char env_buf[1024];
    const char *url;
    char *str;

    /* first try the common RPC env var */
    if (id < 0)
        snprintf(env_str, 1024, "NFP_RPC_URL_DEFAULT");
    else
        snprintf(env_str, 1024, "NFP_RPC_URL_DEV%d", id);

    url = get_envvar(env_str, env_buf, sizeof(env_buf));

    if (!url) {
        /* then try the old SIM one env var */
        if (id < 0)
            snprintf(env_str, sizeof(env_str), "SDK_SIM_URL_DEFAULT");
        else
            snprintf(env_str, sizeof(env_str), "SDK_SIM_URL_DEV%d", id);

        url = get_envvar(env_str, env_buf, sizeof(env_buf));

        /* if no match use the default */
        if (!url)
            url = NFP_RPC_DEFAULT_URL;
    }

    str = (char *) malloc(strlen(url) + 1);
    if (!str)
        return NULL;

    sprintf(str, "%s", url);

    return str;
}

static int warned_once = 0;
static int ignore = -1;

int nfp_client_check_version(const char *api_name,
                             int client_version,
                             int client_compat,
                             int server_version)
{
    /* FIXME */
    char *env_str = "NFP_RPC_VERSION_CHECK";
    char env_buf[1024];
    const char *var;
    int err = 0;

    if (ignore < 0) {
        var = get_envvar(env_str, env_buf, sizeof(env_buf));

        /* if the env var is there ignore mismatches */
        if (var)
            ignore = atoi(var);
        else /* default is ignore */
            ignore = 1;
    }

    if (client_version != server_version)
        err = 1;

    if (!err)
        return 0;

    if (ignore) {
        if (!warned_once)
            NS_LOG(NS_LOG_LVL_WARN,
                   "\n\tRPC client version mismatch for API %s detected: "
                   "\n\tserver version %d, client compat %d, client version %d. "
                   "\n\tIgnoring mismatch, no further warnings will be generated",
                   api_name, server_version, client_compat, client_version);
        warned_once = 1;
        return 0;
    }

    if (!warned_once)
        NS_LOG(NS_LOG_LVL_ERROR,
               "\n\tRPC client version mismatch for API %s detected: "
               "\n\tserver version %d, client compat %d, client version %d. "
               "\n\tNo further error messages will be generated",
               api_name, server_version, client_compat, client_version);

    warned_once = 1;

    return -1;
}

int handle_common_init(struct handle_common *hc)
{
    if (!hc) {
        NS_LOG(NS_LOG_LVL_WARN, "null common handle passed\n");
        return -1;
    }

    memset(hc,0,sizeof(*hc));

    if (pthread_mutex_init(&hc->callbacks_mutex, NULL) != 0) {
        NS_LOG(NS_LOG_LVL_WARN, "common handle data init failed\n");
        return -1;
    }

    return 0;
}

void handle_common_free(struct handle_common *hc)
{
    if (!hc)
        return;

    pthread_mutex_destroy(&hc->callbacks_mutex);
    if (hc->callbacks)
        free(hc->callbacks);
}
