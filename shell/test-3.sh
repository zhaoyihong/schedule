#!/bin/sh

if [ $# -ne  2 ]; then
	echo "sh test.sh size xuhao"
	exit
fi

size=$1
k=$2

t1=0
t2=0

min1=10000
max1=0

min2=10000
max2=0


loop=100
for i in `seq 1 $loop`
do
	#r1=` ../bin/localsearch data/m4-$k 4 16   | tail -n 1`
	#r2=`../bin/ga4 data/m4-$k 16 | tail -n 1 | awk '{print $NF}'`
	r1=` ../bin/localsearch data/m${size}-${k} ${size} 16   | tail -n 1`
	r2=`../bin/ga${size} data/m${size}-${k} 16 | tail -n 1 | awk '{print $NF}'`

	#echo "$r1 $r2"
	t1=`echo "$t1+$r1" | bc`
	t2=`echo "$t2+$r2" | bc `	
	
	[ $( echo "$r1 < $min1" | bc ) -eq 1 ] &&   min1=$r1
	[ $( echo "$r2 < $min2" | bc ) -eq 1 ] &&   min2=$r2
	[ $( echo "$r1 > $max1" | bc ) -eq 1 ] &&   max1=$r1
	[ $( echo "$r2 > $max2" | bc ) -eq 1 ] &&   max2=$r2

done

t0=`sh hungryol.sh ${size} data/m${size}-${k} | tail -n 1`

#echo "$t1 $t2"
t1=`echo "scale=3;$t1/$loop" | bc`
t2=`echo "scale=3;$t2/$loop" | bc`

tail -n1 data/m${size}-${k}

echo "hungry:$t0"
echo "local: $t1,min:$min1,max:$max1"
echo "ga $t2,min:$min2,max:$max2"
echo ""
