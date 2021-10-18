#ifndef __NFP_COMMON_H__
#define __NFP_COMMON_H__

#include <stdint.h>
#include <pthread.h>

struct handle_common {
    pthread_mutex_t callbacks_mutex; /* Protect the callbacks structure */
    void (**callbacks)(void*);
};

char *nfp_get_server_url(int device);

int nfp_client_check_version(const char *api_name,
                             int client_version,
                             int client_compat,
                             int server_version);

int handle_common_init(struct handle_common *hc);
void handle_common_free(struct handle_common *hc);

#endif /* __NFP_COMMON_H__ */
