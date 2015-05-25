#!/bin/sh

if [ $# -ne  4 ]; then
	echo "sh cost_single.sh path size xuhao loop"
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
path="data/matrix${size}-9-${k}"
arg="${path} ${size} ${loop}"
if echo "$bin" | grep 'semi' >/dev/null  ;then
    bins=`sed -n "${k}p" /home/yihong/schedule/src/semi-online/workindex`
    arg="${path} ${size} ${loop} $bins"
fi
loop2=100

for i in `seq 1 $loop2`
do
	r1=`$bin $arg | tail -n 1`
	t1=`echo "$t1+$r1" | bc`
		
	[ $( echo "$r1 < $min1" | bc ) -eq 1 ] &&   min1=$r1
	[ $( echo "$r1 > $max1" | bc ) -eq 1 ] &&   max1=$r1
done

t1=`echo "scale=5;$t1/$loop2" | bc`

workload=`tail -n 1 data/matrix${size}-9-${k}`
echo $workload

#base=`python base.py $workload`
#echo $base

#t0=`echo "scale=6;$t1/$base" | bc`

min1=`awk -va=$t1 -vb=$min1 'BEGIN{print a-b}'`
max1=`awk -va=$t1 -vb=$max1 'BEGIN{print b-a}'`



echo -n -e "min\t"
echo $min1 #最小
echo -n -e "max\t"
echo $max1 #最大
echo -n -e "avg\t"
echo $t1  # 实际值
#echo -n -e "relative\t"
#echo $t0  # 相对base的增长
