#!/bin/sh

source ../config/config.sh

function usage
{
	echo "Usage:"
	echo  "参数1:矩阵size  参数2:矩阵文件path"
	echo  "$0 4 matrix160 "
}

if [ $# != 2 ];then
	usage
	exit 1
fi

$SCHEDULE_BINPATH/normalize $1 $2

