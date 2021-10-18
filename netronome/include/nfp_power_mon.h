/**
 * Copyright (C) 2013-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * nfp and board power monitoring
 *
 */
#ifndef __NFP_POWER_MON__
#define __NFP_POWER_MON__

int nfp_power_monitoring_platform(struct nfp_device *nfp);

int nfp_have_active_qsfp(struct nfp_device *nfp);

int nfp_system_power_service_active(struct nfp_device *nfp);

int nfp_system_power_service_data(struct nfp_device *nfp,
				  uint32_t *rawval,
				  float *val,
				  float *limit);

int nfp_system_power(struct nfp_device *nfp,
		     uint32_t *rawval,
		     float *val,
		     float *limit);

int nfp_system_power_foreach_rail(struct nfp_device *nfp,
				  int (*callback)(struct nfp_device *,
						  void *,
						  const char *,
						  const char *),
				  void *context);

int nfp_system_power_rail(struct nfp_device *nfp,
			  const char *rail,
			  uint32_t *rval,
			  float *val);

struct pid_log_entry {
	float process;
	float kp;
	float ki;
	float kd;
	float sp;
	float fr;
	float i;
	uint32_t ctrl;
} __attribute__((__packed__));

struct power_log_entry {
	struct pid_log_entry power;
	struct pid_log_entry temperature;
	uint32_t ms_ticks;
	uint16_t raw_power;
	uint8_t ctrl;
	uint8_t reserved[7];
	uint16_t checksum;
} __attribute__((__packed__));

int nfp_system_power_log(struct nfp_device *nfp, struct power_log_entry *e);

int nfp_system_power_consume_log(struct nfp_device *nfp,
				 struct power_log_entry *e);

#define NFP_SENSOR_CHIPTEMP 0
#define NFP_SENSOR_ASMBYPOWER 1
#define NFP_SENSOR_ASMBY12V 2
#define NFP_SENSOR_ASMBY3V3 3

int nfp_sensor(struct nfp_device *nfp, int sensor_id, uint32_t *val);

#endif /* __NFP_POWER_MON__ */
