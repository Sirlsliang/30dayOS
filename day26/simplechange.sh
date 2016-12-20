#!/bin/bash

if [ $# -le 2 ]; then
	rm -rf $1 
	mv $2 $1
fi
	


