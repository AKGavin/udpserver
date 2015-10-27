#!/bin/bash
. env.sh
if [ $# == 0 ];then
	cd src;
	make;
	make -f makeobj;
	make install;
elif [ $# == 1 ];then
	if [ $1 == 'clean' ];then
		rm -f obj/* include/* lib/* ./MonitorServer
		cd src;
		make clean;
	else
		echo "$0 or $0 clean"
	fi
fi
