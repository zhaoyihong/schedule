#!/bin/bash

dir=`dirname $0`
source ../config/config.sh


if [ $# != 3 ];
then
	echo "参数错误!"
	echo "$0 matrixpath size maxloop"
	exit
fi

matrix=$1
size=$2
maxloop=$3

if [ ! -e "$matrix" ]; then
	echo "矩阵不存在!"
	exit
fi

#schedule=([1]=0 [2]=0 [3]=0 [4]=0)
for i in `seq 1 $size`
do
	schedule[$i]=0
done



getEd2()
{
	[ $# != 3 ] && return
	loop=$1
	bin=$2
	cpu=$3

	row=$((($loop-1)*$(($size+1))+$bin+1))
	column=$cpu
	awk -vr=$row -vc=$column '{if(NR==r){print $c}}' $matrix	
}


hungry()
{
	result=0
	for((i=1;i<=$maxloop;i++))
	{
		inputStr=""	
		for((k=1;k<=$size;k++))
		{
			for((j=1;j<=$size;j++))
			{
				ed2=`getEd2 $i $k $j`
				ed2[$((($k-1)*$size+$j))]=$ed2
				inputStr=$inputStr" "$ed2
			}
		}
		
		echo "$inputStr" > 1 	
		list=`echo $inputStr |  $SCHEDULE_BINPATH/h$size`	
		echo $list
		
		loopresult=0
		cnt=1
		for cost in $list
		do
			if [ $cost -eq 1 ]; then
				loopresult=`echo " scale=6;${ed2[$cnt]} + $loopresult " | bc `
			fi
			cnt=$(($cnt+1))
		done
		
		echo $loopresult
		result=`echo "scale=6;$loopresult + $result" | bc`	

	}
	
	echo $result	
}



hungry




