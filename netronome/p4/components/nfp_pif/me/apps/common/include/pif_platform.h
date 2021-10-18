/*
 * Copyright 2017 Netronome, Inc.
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
 * @file          common/include/pif_platform.h
 * @brief         Supported platforms.
 */

#ifndef __PIF_PLATFORM_H__
#define __PIF_PLATFORM_H__

/*
 * PLATFORM_PORT_SELECTION
 *
 * Define PLATFORM_PORT_SELECTION as one of the following to type:
 * CDP_PLATFORM - worker island 32 to 38
 * SF_PLATFORM (starfighter) - worker islands 32 .. 38
 * FWA_PLATFORM (huh?
 * H_PLATFORM (hydrogen, lithium, carbon, beryllium) - worker islands 32 .. 36
 */
#define CDP_PLATFORM    1
#define SF_PLATFORM     2
#define FWA_PLATFORM    3
#define H_PLATFORM      4
#ifndef PLATFORM_PORT_SELECTION
    #warning "Platform was not defined. Defaulting to SF platform"
    #define PLATFORM_PORT_SELECTION SF_PLATFORM
#endif

#ifndef PIF_VERSION
#define PIF_VERSION "6.1.0.1-preview"
#endif

#endif
