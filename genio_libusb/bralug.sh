#!/bin/sh

PROG=./avrusb

while true; do
	$PROG set_c_7seg 7 ' '
	$PROG set_c_7seg 6 'B'
	$PROG set_c_7seg 5 'r'
	$PROG set_c_7seg 4 'a'
	$PROG set_c_7seg 3 'L'
	$PROG set_c_7seg 2 'U'
	$PROG set_c_7seg 1 'G'
	$PROG set_c_7seg 0 ' '
	sleep 1
	$PROG set_c_7seg 7 ' '
	$PROG set_c_7seg 6 ' '
	$PROG set_c_7seg 5 ' '
	$PROG set_c_7seg 4 'o'
	$PROG set_c_7seg 3 'n'
	$PROG set_c_7seg 2 ' '
	$PROG set_c_7seg 1 ' '
	$PROG set_c_7seg 0 ' '
	sleep 1
	$PROG set_c_7seg 7 'C'
	$PROG set_c_7seg 6 'L'
	$PROG set_c_7seg 5 'T'
	$PROG set_c_7seg 4 ' '
	$PROG set_c_7seg 3 '2'
	$PROG set_c_7seg 2 '0'
	$PROG set_c_7seg 1 '0'
	$PROG set_c_7seg 0 '8'
	sleep 1
done
