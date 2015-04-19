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

min1=10000
max1=0
path="data/matrix${size}-5-${k}"
arg="${path} ${size} 31"

for i in `seq 1 $loop`
do
	r1=`$bin $arg | tail -n 1`
	t1=`echo "$t1+$r1" | bc`
		
	[ $( echo "$r1 < $min1" | bc ) -eq 1 ] &&   min1=$r1
	[ $( echo "$r1 > $max1" | bc ) -eq 1 ] &&   max1=$r1
done

t1=`echo "scale=5;$t1/$loop" | bc`

workload=`tail -n 1 data/matrix${size}-4-${k}`
echo $workload

base=`python base.py $workload`
#echo $base

t0=`echo "scale=6;$t1/$base" | bc`

echo -n -e "min\t"
echo $min1 #最小
echo -n -e "max\t"
echo $max1 #最大
echo -n -e "real\t"
echo $t1  # 实际值
echo -n -e "relative\t"
echo $t0  # 相对base的增长
