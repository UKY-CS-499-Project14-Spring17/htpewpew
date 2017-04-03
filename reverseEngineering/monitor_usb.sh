#!/bin/bash

tmp=(`lsusb | grep QinHeng | cut -d: -f1 | awk '{print $2,$4}'`)

bus_id=`echo ${tmp[0]}|sed 's#^0*##'`
dev_id=`echo ${tmp[1]}|sed 's#^0*##'`

stty -F /dev/ttyUSB0 115200

modprobe usbmon
tshark -i usbmon${bus_id} -Y "usb.addr contains \"${bus_id}.${dev_id}\"" -T fields -e frame.number -e frame.time -e usb.addr -e usb.data_len -e usb.capdata
