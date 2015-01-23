#!/bin/bash
#生成一个三维矩阵用的
#行表示cpu,列表示bin
#结果输出在标准输出中

dir=`dirname $0`
source ../config/config.sh



loops=$1

if [ $# != 1 ];
then
	echo "参数错误!"
	echo "$0 loops"
	exit
fi

i=0

bins=(gzip gcc parser crafty)
cpus=(1 2 3 4)

while true
do
	if [ "$i" -eq "$loops" ];then
		break
	fi
	
	echo "loop $i"

	for bin in ${bins[@]}
	do
		line=""
		for cpu in ${cpus[@]}
		do
			#echo "bin:$bin,cpu:$cpu" 
			ed2=`sh do_runonce.sh $cpu $bin $i`
			line=$line"\t"$ed2
		done
		echo -e $line	
	done

	i=$(($i+1))
done

echo ""
echo "bins:"${bin[@]}
