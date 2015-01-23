#!/bin/bash

dir=`dirname $0`
source ../config/config.sh


if [ $# != 2 ];
then
	echo "参数错误!"
	echo "$0 total matrixpath"
	exit
fi

total=$1
for size in `seq 1 $total`
do
	tmp=$(($size * $size))
	if [ $tmp -eq $total ] ;then
		break;
	fi
done 


matrix=$2
if [ ! -e "$matrix" ]; then
	echo "矩阵不存在!"
	exit
fi



getEd2()
{
	[ $# != 3 ] && return
	loop=$1
	bin=$2
	cpu=$3

	row=$((($loop-1)*($total+1)+$bin+1))
	column=$cpu
	awk -vr=$row -vc=$column '{if(NR==r){print $c}}' $matrix	
}


hungry()
{
	result=0
	for((i=1;i<=16;i++)) #loop
	{
		list=()	
		for((k=1;k<=$total;k++))
		{
			for((j=1;j<=$total;j++))
			{
				ed2[$((($k-1)*$total+$j))]=`getEd2 $i $k $j`
			}
		}
			
		list=`echo "${ed2[@]}" | $SCHEDULE_BINPATH/hungry$total`
		#list=`echo "${ed2[1]} ${ed2[2]} ${ed2[3]} ${ed2[4]} ${ed2[5]} ${ed2[6]} ${ed2[7]} ${ed2[8]} ${ed2[9]} ${ed2[10]} ${ed2[11]} ${ed2[12]} ${ed2[13]} ${ed2[14]} ${ed2[16]} ${ed2[16]} " | $SCHEDULE_BINPATH/hungry`
		
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




