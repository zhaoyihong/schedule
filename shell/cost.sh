#!/bin/sh

if [ $# -ne  2 ]; then
	echo "sh test.sh size xuhao"
	exit
fi

size=$1
k=$2

t1=0
t2=0
t3=0
t4=0
t5=0

min1=10000
max1=0

min2=10000
max2=0

min3=10000
max3=0

min4=10000
max4=0

min5=10000
max5=0

loop=100
for i in `seq 1 $loop`
do
	#r1=` ../bin/localsearch data/m4-$k 4 16   | tail -n 1`
	#r2=`../bin/ga4 data/m4-$k 16 | tail -n 1 | awk '{print $NF}'`
	r1=` ../bin/localsearch data/m${size}-${k} ${size} 16   | tail -n 1`
	r2=`../bin/ga${size} data/m${size}-${k} 16 | tail -n 1 | awk '{print $NF}'`
	r3=`../bin/random data/m${size}-${k} ${size} 16  | tail -n 1`
	r4=`../bin/deny_random data/m${size}-${k} ${size} 16  | tail -n 1`
	r5=`../bin/deny_life data/m${size}-${k} ${size} 16  | tail -n 1`

	#echo "$r1 $r2"
	t1=`echo "$t1+$r1" | bc`
	t2=`echo "$t2+$r2" | bc `	
	t3=`echo "$t3+$r3" | bc `	
	t4=`echo "$t4+$r4" | bc `	
	t5=`echo "$t5+$r5" | bc `	
	
	
	[ $( echo "$r1 < $min1" | bc ) -eq 1 ] &&   min1=$r1
	[ $( echo "$r1 > $max1" | bc ) -eq 1 ] &&   max1=$r1
	

	[ $( echo "$r2 < $min2" | bc ) -eq 1 ] &&   min2=$r2
	[ $( echo "$r2 > $max2" | bc ) -eq 1 ] &&   max2=$r2

	[ $( echo "$r3 < $min3" | bc ) -eq 1 ] &&   min3=$r3
	[ $( echo "$r3 > $max3" | bc ) -eq 1 ] &&   max3=$r3
	
	[ $( echo "$r4 < $min4" | bc ) -eq 1 ] &&   min4=$r4
	[ $( echo "$r4 > $max4" | bc ) -eq 1 ] &&   max4=$r4
	
	[ $( echo "$r5 < $min5" | bc ) -eq 1 ] &&   min5=$r5
	[ $( echo "$r5 > $max5" | bc ) -eq 1 ] &&   max5=$r5
done

t0=`sh hungry.sh ${size} data/m${size}-${k} | tail -n 1`

#echo "$t1 $t2"
t1=`echo "scale=5;$t1/$loop" | bc`
t2=`echo "scale=5;$t2/$loop" | bc`
t3=`echo "scale=5;$t3/$loop" | bc`
t4=`echo "scale=5;$t4/$loop" | bc`
t5=`echo "scale=5;$t5/$loop" | bc`

tail -n1 data/m${size}-${k}

echo "hungry:$t0"
echo "local: $t1,min:$min1,max:$max1"
echo "ga $t2,min:$min2,max:$max2"
echo "random $t3,min:$min3,max:$max3"
echo "deny_random $t4,min:$min4,max:$max4"
echo "deny_life $t5,min:$min5,max:$max5"
echo ""
