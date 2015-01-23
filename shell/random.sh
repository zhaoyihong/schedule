#!/bin/bash

dir=`dirname $0`
source ../config/config.sh


if [ $# != 1 ];
then
	echo "参数错误!"
	echo "$0 matrixpath"
	exit
fi

matrix=$1
if [ ! -e "$matrix" ]; then
	echo "矩阵不存在!"
	exit
fi

schedule=([1]=0 [2]=0 [3]=0 [4]=0)


getEd2()
{
	[ $# != 3 ] && return
	loop=$1
	bin=$2
	cpu=$3

	row=$((($loop-1)*5+$bin+1))
	column=$cpu
	awk -vr=$row -vc=$column '{if(NR==r){print $c}}' $matrix	
}

randomSchedule()
{
	list=([1]=0 [2]=0 [3]=0 [4]=0)
	cpu=1

	while [ $cpu -ne 5  ] 
	do
		random=$(($RANDOM%4 + 1)) 
		while [ ${list[$random]} -ne 0 ] 
		do
			random=$(($RANDOM%4 + 1))
		done
		list[$random]=1
		schedule[$cpu]=$random
		cpu=$(($cpu+1))
	done
	
	for i in ${schedule[@]}
	do
		echo $i
	done		
}


random()
{
	list=`randomSchedule`
	echo $list
        array[1]=`echo $list | awk '{print $1}'`
	array[2]=`echo $list | awk '{print $2}'`		
	array[3]=`echo $list | awk '{print $3}'`		
	array[4]=`echo $list | awk '{print $4}'`		
	
	result=0
	for((i=1;i<=24;i++))
	{
		for((j=1;j<=4;j++))
		{
			ed2=`getEd2 $i $j ${array[$j]}`
			result=`echo "scale=6;$result+$ed2" | bc`
		}

	}
	
	echo $result	
}






#randomSchedule
random




