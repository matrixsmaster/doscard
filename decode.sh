#!/bin/bash
while read line
do
	name=`addr2line -f -e $2 $line | head -n1`
	point=`addr2line -s -e $2 $line`
	echo "$name [$point]"
done < $1
