#!/bin/bash

fib() {
	n=$1
	a=0
	b=1

	for ((i = 1 ; i <= n; i++)) do
		echo "$a"
		next=$((a + b))
		a=$b
		b=$next
	done
}


read -p "Enter n: " num
fib $num
