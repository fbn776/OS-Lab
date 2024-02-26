#!/bin/bash

read -p "Enter filename: " fname

if ! [ -e $fname ] ; then
	echo "File not found"
else
	read -p "Enter key: " key
        if cat "$fname" | grep -q "$key" ; then
		echo "Word found"
	else
		echo "Not found"
	fi
fi
