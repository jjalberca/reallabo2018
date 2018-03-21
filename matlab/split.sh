#!/bin/bash

split -l 1201 capture new

a=1
b=1
for i in new*; do
	name=$(printf "%dV %d" "$a" "$b")
	mv -i -- "$i" "$name"
	let b=b+1
	if [ $b -gt 10 ] 
	then
		let b=1
		let a=a+1
	fi
done

find . -name "*V*" -exec sed -i '1d' {} \;
