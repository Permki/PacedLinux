/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_p4info.h
 * @brief         P4Info definitions based upon p4info.proto file from the
 *                P4Lang PI project on Github (https://github.com/p4lang/PI)
 *
 */

#ifndef __NFP_PIF_P4INFO_DEFS_H__
#define __NFP_PIF_P4INFO_DEFS_H__

#include <stdint.h>
#include <stdbool.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"

__BEGIN_DECLS

typedef enum {
    P4INFO_EXTERN = 1,
    P4INFO_TABLE = 2,
    P4INFO_ACTION = 3,
    P4INFO_ACTION_PROFILE = 4,
    P4INFO_COUNTER = 5,
    P4INFO_DIRECT_COUNTER = 6,
    P4INFO_METER = 7,
    P4INFO_DIRECT_METER = 8,
    P4INFO_CONTROLLER_PACKET_METADATA = 9
} p4info_type_t;

typedef struct {
  // ids share the same number-space; e.g. table ids cannot overlap with counter
  // ids. Even though this is irrelevant to this proto definition, the ids are
  // allocated in such a way that it is possible based on an id to deduce the
  // resource type (e.g. table, action, counter, ...). This means that code
  // using these ids can detect if the wrong resource type is used
  // somewhere. This also means that ids of different types can be mixed
  // (e.g. direct resource list for a table) without ambiguity. Note that id 0
  // is reserved and means "invalid id".
  uint32_t id;
  // fully qualified name of the P4 object, e.g. c1.c2.ipv4_lpm
  char *name;
  // an alias for the P4 object, probably shorter than its name. The only
  // constraint is for it to be unique with respect to other P4 objects of the
  // same type. By default, the compiler uses the shortest suffix of the name
  // that uniquely indentifies the object. For example if the P4 program
  // contains two tables with names s.c1.t and s.c2.t, the default aliases will
  // respectively be c1.t and c2.t. The P4 programmer may also override the
  // default alias for any P4 object (TBD). When resolving a P4 object id, an
  // application should be able to indiscriminately use the name or the alias.
  char *alias;
  char *annotations;
} p4info_preamble_t;

typedef struct {
    uint32_t extern_type_id;
    char *extern_type_name;
    char *extern_data;          // TODO: might change as to fit our externs
    uint32_t datalen;
} p4info_extern_t;

typedef enum {   
    UNSPECIFIED = 0,
    VALID = 1,
    EXACT = 2,
    LPM = 3,
    TERNARY = 4,
    RANGE = 5,
} p4info_matchtype_t;

typedef struct {
    uint32_t id;
    char *name;
    char *annotation;
    int32_t bitwidth;
    p4info_matchtype_t match_type;
} p4info_matchfield_t;

typedef struct {
  uint32_t id;
  char *annotations;
} p4info_actionref_t;

typedef struct {
    p4info_matchfield_t *match_fields;
    uint32_t num_match_fields;
    // even when the table is indirect (see implementation_id) below, this field
    // includes all possible actions for the table; by using ActionRef instead 
    // of a repeated field of action ids, each action reference in a P4 table is
    //  able to have its own annotations
    p4info_actionref_t *action_refs;
    uint32_t num_action_refs;
    // 0 (default value) means that the table does not have a const default 
    // action
    uint32_t const_default_action_id;
    // a table may have a const default action, whose action parameter values 
    // can be changed at runtime. However, in most cases the parameters of the 
    // default action are also bound at compile-time and cannot be changed by 
    // the runtime, which is what this boolean flag indicates.
    bool const_default_action_has_mutable_params;
    // P4 id of the "implementation" for this table (e.g. action profile id); 0
    // (default value) means that the table is a regular (direct) match table. 
    // As of today, only action profiles are supported but other table
    // implementations may be added in the future
    uint32_t implementation_id;
    // ids of the direct resources (if any) attached to this table; for now this
    // includes only direct counters and direct meters, but other resources may be
    // added in the future
    uint32_t *direct_resource_ids;
    uint32_t num_resource_ids;
    int64_t size;  // max number of entries in table
    bool with_entry_timeout;  // entry ageing is enabled for table
} p4info_table_t;

typedef struct {
    uint32_t id;
    char *name;
    char *annotations;
    int32_t bitwidth;
} p4info_param_t;

typedef struct {
    p4info_param_t *params;
    uint32_t num_params;
} p4info_action_t;

typedef struct {
    // the ids of the tables sharing this action profile
    uint32_t *table_ids;
    uint32_t num_tbl_ids;
    // true iff the action profile used dynamic selection
    bool with_selector;
    int64_t size;  // max number of member entries in action profile
} p4info_actionprofile_t;

typedef enum {
    CNTR_UNSPECIFIED = 0,
    CNTR_BYTES = 1,
    CNTR_PACKETS = 2,
    CNTR_BOTH = 3  // not part of the P4 spec yet but will be in the future
} p4info_cntrunit_t;

typedef struct {
    p4info_cntrunit_t unit;
    // number of entries in the counter array
    int64_t size;
} p4info_counter_t;

typedef struct {
    p4info_cntrunit_t unit;
    // the id of the table to which the counter is attached
    uint32_t direct_table_id;
} p4info_directcntr_t;

  // Corresponds to 'type' attribute for meter in P4 spec.
typedef enum {
    METER_UNSPECIFIED = 0,
    METER_BYTES = 1,
    METER_PACKETS = 2
} p4info_meterunit_t;

// Not part of the P4 spec yet but will be in the future.
typedef enum {
    COLOR_UNAWARE = 0,  // default value
    COLOR_AWARE = 1,
} p4info_metertype_t;

typedef struct {
    p4info_meterunit_t unit;
    p4info_metertype_t type;
} p4info_meterspec_t;

typedef struct {
    p4info_meterspec_t spec;
    // number of entries in the meter array
    int64_t size;
} p4info_meter_t;

typedef struct {
    p4info_meterspec_t spec;
    // the id of the table to which the meter is attached
    uint32_t direct_table_id;
} p4info_directmeter_t;

// Any metadata associated with controller Packet-IO (Packet-In or Packet-Out)
// is modeled as P4 headers carrying special annotations
// @controller_metadata("packet_out") and @controller_metadata("packet_in")
// respectively. There can be at most one header each with these annotations.
// This message captures the info contained within these special headers,
// and used in p4runtime.proto to supply the metadata.
typedef struct {
    // preamble.name and preamble.id will specify header type ("packet_out" or
    // "packet_in" for now).

    // Ordered based on header layout.
    // This is a constraint on the generator of this P4Info.
     uint32_t id;
    // This is the name of the header field (not fully-qualified), similar
    // to e.g. Action.Param.name.
    char *name;
    char *annotations;
    int32_t bitwidth;
} p4info_cntrlrpacketmetadata_t;

typedef union {
    p4info_extern_t p4_extern;    // choose the name as to not clash with keyword
    p4info_table_t table;
    p4info_action_t action;
    p4info_actionprofile_t action_profile;
    p4info_counter_t counter;
    p4info_directcntr_t direct_cntr;
    p4info_meter_t meter;
    p4info_directmeter_t direct_meter;
    p4info_cntrlrpacketmetadata_t cntrlr_packet_metadata;
} p4info_type_u;

typedef struct {
    p4info_preamble_t preamble;
    p4info_type_t type;
    p4info_type_u data;
} p4info_t;


__END_DECLS

#endif /* !__NFP_PIF_P4INFO_DEFS_H__ */