/*
 * Copyright 2015-present Netronome, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file          app_master.c
 * @brief         Code for various application house keeping
 */

#include <nfp.h>

#include <nfp/me.h>
#include <nfp/mem_bulk.h>
#include <nfp/remote_me.h>
#include <nfp6000/nfp_me.h>
#include <hashmap.h>

#ifndef PIF_GLOBAL_FLOWCACHE_DISABLED
#include <flow_cache_c.h>
#endif /* PIF_GLOBAL_FLOWCACHE_DISABLED */

#include <pif_common.h>
#include <system_init_c.h>
#include <app_master_common.h>

#define CTX_INIT    APP_MASTER_CTX_NFDCFG
#define CTX_PVS     APP_MASTER_CTX_APP0
#define CTX_CFG     APP_MASTER_CTX_APP1

#define MAX_APP_MES 128
#define APP_ME_BURST 8

volatile __export __addr40 __ctm uint32_t pif_app_mes[MAX_APP_MES];
volatile __export __addr40 __ctm uint32_t pif_config_change = 0;
__gpr __shared uint32_t pif_current_config = 0;

/* poll PIF application parser value sets */
static void pvs_poll()
{
    while (1) {
        pif_value_set_scan_configs();
        sleep(100);
    }
    /* not reached */
}

/* poll PIF application configuration changes */
static void config_change_poll(void)
{
    __xread uint32_t app_mes[APP_ME_BURST];
    int i, j;
    int done;

    while (1) {
        if (pif_config_change || app_master_nfd_config_changed) {
            pif_current_config += 1;

            done = 0;
            for (i = 0; i < MAX_APP_MES / APP_ME_BURST; i++) {
                mem_read32(&app_mes,
                           (__mem __addr40 void *)&pif_app_mes[i * APP_ME_BURST],
                           sizeof(app_mes));
                for (j = 0; j < APP_ME_BURST; j++) {
                    if (app_mes[j] == 0) {
                        done = 1;
                        break;
                    }
                    remote_csr_write(app_mes[j] >> 4,
                                     app_mes[j] & 0xf,
                                     local_csr_mailbox_3,
                                     pif_current_config);
                }
                if (done)
                    break;
            }

            app_master_nfd_config_changed = 0;
            pif_config_change = 0;
        }
        sleep(100);
    }
    /* not reached */
}

int
main(void)
{
    if (ctx() == CTX_INIT) {
#ifndef PIF_GLOBAL_FLOWCACHE_DISABLED
        fc_global_init();
#endif /* PIF_GLOBAL_FLOWCACHE_DISABLED */
        camp_hash_init();
        pif_master_init();
        system_init_done();
    }

    if (ctx() == APP_MASTER_CTX_NFDCFG)
        app_master_nfdcfg_poll();

    if (ctx() == APP_MASTER_CTX_NFDSTATS)
        app_master_nfdstats_poll();

    if (ctx() == APP_MASTER_CTX_MACPORT)
        app_master_macport_poll();

    if (ctx() == APP_MASTER_CTX_MACTIME)
        app_master_mactime_poll();

    if (ctx() == CTX_PVS)
        pvs_poll();

    if (ctx() == CTX_CFG)
        config_change_poll();

    return 0;
}
