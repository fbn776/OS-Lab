#!/bin/bash

isPalin() {
	[ "$1" = $(echo "$1" | rev) ]
}

start=$1
end=$2
for (( i=start ; i<=end ; i++)) ; do
	if isPalin "$i" ; then
		echo "$i"
	fi
done
