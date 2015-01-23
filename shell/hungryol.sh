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

	row=$((($loop)*($total+1)+$bin+1))
	column=$cpu
	awk -vr=$row -vc=$column '{if(NR==r){print $c}}' $matrix	
}


url="http://www.hungarianalgorithm.com/solve.php?steps=0&c="
hungry()
{

	result=0
	for((i=1;i<=16;i++)) #loop
	{
		data=""
		for((k=1;k<=$total;k++))
		{
			for((j=1;j<=$total;j++))
			{
				ed2=`getEd2 $i $k $j`
				data=${data}${ed2}"-"
			}
			data=${data}"-"
		}

	  	#tmp=`curl -s "${url}${data}"  | grep "optimal value is" | awk '{print $5}' | sed 's/.<\/p>.*$//g'`
	  	tmp=`curl -s "${url}${data}" | grep "The optimal value equals " | sed 's/.*equals //'  | sed 's/.<.*$//'`
	#	echo "$data"
	#	echo "$tmp"	
		result=`echo "scale=6;$result+$tmp" | bc`
	}		
	echo $result

}


hungry




