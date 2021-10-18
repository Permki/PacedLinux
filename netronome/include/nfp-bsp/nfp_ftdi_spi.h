/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_ftdi_spi.h
 *
 */

#include <unistd.h>
#include <dirent.h>


//#define ARM_PROM_CS 0
//#define WIZNET_CS   1
//#define CFG_PROM_CS 4

struct nfp_ftdi_spi *nfp_ftdi_spi_open ( char *sno );//, unsigned int iface );
int nfp_ftdi_spi_close ( struct nfp_ftdi_spi *handle );
int nfp_ftdi_spi_divisor ( struct nfp_ftdi_spi *handle, unsigned int divisor );

int nfp_ftdi_spi_command ( struct nfp_ftdi_spi *handle, unsigned int cs, unsigned char *wcmd, unsigned int wcmdlen, unsigned char *rcmd, unsigned int *rcmdlen );

int prom_identification ( struct nfp_ftdi_spi *handle, unsigned int cs, unsigned int *id );
int read_prom ( struct nfp_ftdi_spi *handle, unsigned int cs, unsigned int id,  unsigned char *data, unsigned int datalen );
int write_prom ( struct nfp_ftdi_spi *handle, unsigned int cs, unsigned int id, unsigned char *write_data, unsigned int datalen );
int erase_prom ( struct nfp_ftdi_spi *handle, unsigned int cs, unsigned int id, unsigned int datalen );
int smoke_prom ( struct nfp_ftdi_spi *handle, unsigned int cs, unsigned int id, unsigned int sectors, unsigned int *errors );
