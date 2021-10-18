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

#include <assert.h>
#include <nfp_chipres.h>
#include <nfp.h>

#include <system_init_c.h>
#include <app_master_common.h>

#define CTX_INIT    APP_MASTER_CTX_NFDCFG

int
main(void)
{
    if (ctx() == CTX_INIT)
        system_init_done();

    if (ctx() == APP_MASTER_CTX_NFDCFG)
        app_master_nfdcfg_poll();

    if (ctx() == APP_MASTER_CTX_NFDSTATS)
        app_master_nfdstats_poll();

    if (ctx() == APP_MASTER_CTX_MACPORT)
        app_master_macport_poll();

    if (ctx() == APP_MASTER_CTX_MACTIME)
        app_master_mactime_poll();

    return 0;
}
