#!/bin/bash

usage()
{
    echo "Starts and stops port expander firmware on all port expander NICs on the system."
    echo "Usage: portexapp.sh [start|stop|restart]"
    echo "Start: start port expander firmware. Use if no firmware is currently loaded."
    echo "Stop: stop the port expander firmware on all port expander NICs."
    echo "Restart: stop and restart the port expander firmware."
}

if [ -z $1 ]
then
    usage
    exit
fi

#find correct NFP
NFP_CNT=$(ls /dev/ | grep -c nfp-cpp)

if [ $NFP_CNT -eq 0 ]
then
   echo "Error: No NFP devices detected on the system"
   exit
fi

NFPS=()
fwfiles=()

for ((v=0;v<NFP_CNT;v++))
do
    PARTNO=$(nfp-hwinfo -n $v | grep assembly.partno)
    #look for mockup
    echo $PARTNO | grep -q "AMDA0997"
    if [ $? -eq 0 ]
    then
	NFPS+=($v)
	if [ $1 != 'stop' ]
	then
	    fwfiles+=('AMDA0997_0001/NFP_4001_0N/B0/portex.nffw')
	else
	    fwfiles+=('')
	fi
    fi
    #look for othe port expander models
done

portex_cnt=${#NFPS[@]}

if [ $portex_cnt -eq 0 ]
then
    echo "Error: no port-expander NFPs detected on the system"
    exit
fi

NETRONOME_DIR=${NETRONOME_DIR:='/opt/netronome'}

# Use various common routines from BSP
. $(dirname $0)/../lib/nfp-shutils

APPLICATION=Portex

initTextFormatting

for ((n=0;n<portex_cnt;n++))
do
    echo "Starting port expander on NFP ${NFPS[$n]}"
    appctl -n ${NFPS[$n]} $1 ${fwfiles[$n]}
done
