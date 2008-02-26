#!/bin/sh

PROG=./avrusb

while true; do
	time=`date +"%H %M %S"`
	for i in `seq 0 7`; do
		c=`echo $time | sed -e "s/^.\{$i\}\(.\).*$/\1/"`
		$PROG set_c_7seg $((7 - $i)) "$c"
	done
	sleep 1
done
