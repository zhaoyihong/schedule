#!/bin/bash

if [ $# != 1 ]
then
	echo "参数错误.Usage:sh result.sh <path>"
	exit
fi

path=$1
filename=$(basename $path)


sed $path -e 's/,min//g' -e's/,max//g' -e's/y:/ /'  -e's/l:/l /' -e 's/work load/workload/g' -e's/:/ /g' -e's/ /\t/g' -e's/.mips//g' -e 's/hungr/hungarian/g' -e '/^$/d' > ${filename}_1

awk -F'\t' -vOFS='\t' '{if(NF==1){print $0}else if($1=="hungarian"){BASE=$2;} else if($1=="workload"){print $0} else{ print $1,($2-BASE)/BASE}}' ${filename}_1 | sed 's/-1//' > ${filename}_2 


