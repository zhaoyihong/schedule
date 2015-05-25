#!/bin/sh

if [ $# -ne  4 ]; then
	echo "sh test.sh path size xuhao loop"
	exit
fi

bin=$1
size=$2
k=$3
loop=$4

#total
t1=0

min1=1000000000
max1=0
path="data/matrix${size}-9-${k}"
arg="${path} ${size} 50"

for i in `seq 1 $loop`
do
	r1=`$bin $arg | tail -n 4 | head -n 1`
	t1=`echo "$t1+$r1" | bc`
		
	[ $( echo "$r1 < $min1" | bc ) -eq 1 ] &&   min1=$r1
	[ $( echo "$r1 > $max1" | bc ) -eq 1 ] &&   max1=$r1
done

t1=`echo "scale=5;$t1/$loop" | bc`
min1=`awk -va=$t1 -vb=$min1 'BEGIN{print a-b}'`
max1=`awk -va=$t1 -vb=$max1 'BEGIN{print b-a}'`

echo -n -e "min\t"
echo $min1 #负偏差
echo -n -e "max\t"
echo $max1 #正偏差
echo -n -e "real\t"
echo $t1  # 实际值
