#!/bin/sh

PROG=./avrusb

if [ -z "$1" ]; then
	echo "usage: $0 <text>" >&2
	exit 1
fi

# pad if neccessary
text="$1       "

for i in `seq 0 7`; do
	c=`echo "$text" | sed -e "s/^.\{$i\}\(.\).*$/\1/"`
	$PROG set_c_7seg $((7 - $i)) "$c"
done
