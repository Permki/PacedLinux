/*
 * Copyright 2018 Netronome, Inc.
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
 */

#ifndef __APP_MASTER_COMMON_H__
#define __APP_MASTER_COMMON_H__

#define APP_MASTER_CTX_NFDCFG   0
#define APP_MASTER_CTX_NFDSTATS 1
#define APP_MASTER_CTX_MACPORT  2
#define APP_MASTER_CTX_MACTIME  3
#define APP_MASTER_CTX_APP0     4
#define APP_MASTER_CTX_APP1     5
#define APP_MASTER_CTX_APP2     6
#define APP_MASTER_CTX_APP3     6

#define APP_MASTER_POLL_NFDCFG   100
#define APP_MASTER_POLL_NFDSTATS 2000
#define APP_MASTER_POLL_MACTIME  349525 /* should be roughly 5ms */
#define APP_MASTER_POLL_MACPORT  2000

extern __gpr __shared uint32_t app_master_nfd_config_changed;

void app_master_nfdcfg_poll(void);

void app_master_nfdstats_poll(void);

void app_master_macport_poll(void);

void app_master_mactime_poll(void);
#endif /* __APP_MASTER_COMMON_H__ */
