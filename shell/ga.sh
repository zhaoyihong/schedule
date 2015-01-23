#!/bin/sh
source ../config/config.sh

input=$1
currentdir=`pwd`


function cpgadata()
{
	cp $input gadata.txt 
}

function rmgadata()
{
	rm gadata.txt 
}


function usage()
{
	echo "Usage:$0 inputfile"
}

if [ $# -lt 1 ];then
	usage
	exit
fi

cpgadata
$SCHEDULE_BINPATH/ga | tail -n 1 | awk '{print $5}'
rmgadata






