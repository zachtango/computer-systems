#!/bin/bash

for x in {a..z}
do
	echo -n "$x: "
	
	grep -c "^$x" passwd.txt

done


