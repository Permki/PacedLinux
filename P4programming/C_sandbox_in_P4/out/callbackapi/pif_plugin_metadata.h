/* Copyright (C) 2015-2016,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PIF_PLUGIN_metadata_H__
#define __PIF_PLUGIN_metadata_H__
/*
 * Access function prototypes
 */

/* get standard_metadata.clone_spec */
uint32_t pif_plugin_meta_get__standard_metadata__clone_spec(EXTRACTED_HEADERS_T *extracted_headers);

/* set standard_metadata.clone_spec */
void pif_plugin_meta_set__standard_metadata__clone_spec(EXTRACTED_HEADERS_T *extracted_headers, uint32_t val);

/* get standard_metadata.egress_spec */
uint32_t pif_plugin_meta_get__standard_metadata__egress_spec(EXTRACTED_HEADERS_T *extracted_headers);

/* set standard_metadata.egress_spec */
void pif_plugin_meta_set__standard_metadata__egress_spec(EXTRACTED_HEADERS_T *extracted_headers, uint32_t val);

/* get standard_metadata.egress_port */
uint32_t pif_plugin_meta_get__standard_metadata__egress_port(EXTRACTED_HEADERS_T *extracted_headers);

/* set standard_metadata.egress_port */
void pif_plugin_meta_set__standard_metadata__egress_port(EXTRACTED_HEADERS_T *extracted_headers, uint32_t val);

/* get standard_metadata.packet_length */
uint32_t pif_plugin_meta_get__standard_metadata__packet_length(EXTRACTED_HEADERS_T *extracted_headers);

/* set standard_metadata.packet_length */
void pif_plugin_meta_set__standard_metadata__packet_length(EXTRACTED_HEADERS_T *extracted_headers, uint32_t val);

/* get standard_metadata.egress_instance */
uint32_t pif_plugin_meta_get__standard_metadata__egress_instance(EXTRACTED_HEADERS_T *extracted_headers);

/* set standard_metadata.egress_instance */
void pif_plugin_meta_set__standard_metadata__egress_instance(EXTRACTED_HEADERS_T *extracted_headers, uint32_t val);

/* get standard_metadata.parser_error_location */
uint32_t pif_plugin_meta_get__standard_metadata__parser_error_location(EXTRACTED_HEADERS_T *extracted_headers);

/* set standard_metadata.parser_error_location */
void pif_plugin_meta_set__standard_metadata__parser_error_location(EXTRACTED_HEADERS_T *extracted_headers, uint32_t val);

/* get standard_metadata.ingress_port */
uint32_t pif_plugin_meta_get__standard_metadata__ingress_port(EXTRACTED_HEADERS_T *extracted_headers);

/* set standard_metadata.ingress_port */
void pif_plugin_meta_set__standard_metadata__ingress_port(EXTRACTED_HEADERS_T *extracted_headers, uint32_t val);

/* get standard_metadata.instance_type */
uint32_t pif_plugin_meta_get__standard_metadata__instance_type(EXTRACTED_HEADERS_T *extracted_headers);

/* set standard_metadata.instance_type */
void pif_plugin_meta_set__standard_metadata__instance_type(EXTRACTED_HEADERS_T *extracted_headers, uint32_t val);

/* get standard_metadata.parser_status */
uint32_t pif_plugin_meta_get__standard_metadata__parser_status(EXTRACTED_HEADERS_T *extracted_headers);

/* set standard_metadata.parser_status */
void pif_plugin_meta_set__standard_metadata__parser_status(EXTRACTED_HEADERS_T *extracted_headers, uint32_t val);






/*
 * Access function implementations
 */

#include "pif_parrep.h"
#include "pif_headers.h"

__forceinline uint32_t pif_plugin_meta_get__standard_metadata__clone_spec(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    return PIF_HEADER_GET_standard_metadata___clone_spec(md);
}

__forceinline void pif_plugin_meta_set__standard_metadata__clone_spec(EXTRACTED_HEADERS_T *extracted_headers,uint32_t val)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    PIF_HEADER_SET_standard_metadata___clone_spec(md, val);
}

__forceinline uint32_t pif_plugin_meta_get__standard_metadata__egress_spec(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    return PIF_HEADER_GET_standard_metadata___egress_spec(md);
}

__forceinline void pif_plugin_meta_set__standard_metadata__egress_spec(EXTRACTED_HEADERS_T *extracted_headers,uint32_t val)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    PIF_HEADER_SET_standard_metadata___egress_spec(md, val);
}

__forceinline uint32_t pif_plugin_meta_get__standard_metadata__egress_port(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    return PIF_HEADER_GET_standard_metadata___egress_port(md);
}

__forceinline void pif_plugin_meta_set__standard_metadata__egress_port(EXTRACTED_HEADERS_T *extracted_headers,uint32_t val)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    PIF_HEADER_SET_standard_metadata___egress_port(md, val);
}

__forceinline uint32_t pif_plugin_meta_get__standard_metadata__packet_length(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    return PIF_HEADER_GET_standard_metadata___packet_length(md);
}

__forceinline void pif_plugin_meta_set__standard_metadata__packet_length(EXTRACTED_HEADERS_T *extracted_headers,uint32_t val)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    PIF_HEADER_SET_standard_metadata___packet_length(md, val);
}

__forceinline uint32_t pif_plugin_meta_get__standard_metadata__egress_instance(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    return PIF_HEADER_GET_standard_metadata___egress_instance(md);
}

__forceinline void pif_plugin_meta_set__standard_metadata__egress_instance(EXTRACTED_HEADERS_T *extracted_headers,uint32_t val)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    PIF_HEADER_SET_standard_metadata___egress_instance(md, val);
}

__forceinline uint32_t pif_plugin_meta_get__standard_metadata__parser_error_location(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    return PIF_HEADER_GET_standard_metadata___parser_error_location(md);
}

__forceinline void pif_plugin_meta_set__standard_metadata__parser_error_location(EXTRACTED_HEADERS_T *extracted_headers,uint32_t val)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    PIF_HEADER_SET_standard_metadata___parser_error_location(md, val);
}

__forceinline uint32_t pif_plugin_meta_get__standard_metadata__ingress_port(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    return PIF_HEADER_GET_standard_metadata___ingress_port(md);
}

__forceinline void pif_plugin_meta_set__standard_metadata__ingress_port(EXTRACTED_HEADERS_T *extracted_headers,uint32_t val)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    PIF_HEADER_SET_standard_metadata___ingress_port(md, val);
}

__forceinline uint32_t pif_plugin_meta_get__standard_metadata__instance_type(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    return PIF_HEADER_GET_standard_metadata___instance_type(md);
}

__forceinline void pif_plugin_meta_set__standard_metadata__instance_type(EXTRACTED_HEADERS_T *extracted_headers,uint32_t val)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    PIF_HEADER_SET_standard_metadata___instance_type(md, val);
}

__forceinline uint32_t pif_plugin_meta_get__standard_metadata__parser_status(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    return PIF_HEADER_GET_standard_metadata___parser_status(md);
}

__forceinline void pif_plugin_meta_set__standard_metadata__parser_status(EXTRACTED_HEADERS_T *extracted_headers,uint32_t val)
{
    __lmem struct pif_header_standard_metadata *md = (__lmem struct pif_header_standard_metadata *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_standard_metadata_OFF_LW);
    PIF_HEADER_SET_standard_metadata___parser_status(md, val);
}

#endif /* __PIF_PLUGIN_metadata_H__ */
