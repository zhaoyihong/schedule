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
t6=0

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

min6=10000
max6=0

bin1=../bin/localsearch
bin2=../bin/hungarian2
bin3=../bin/random
bin4=../bin/prob_init
bin5=
bin6=../bin/prob

arg="data/matrix${size}-4-${k} ${size} 31"

loop=100
for i in `seq 1 $loop`
do
	#r1=` ../bin/localsearch data/matrix4-$k 4 16   | tail -n 1`
	#r2=`../bin/ga4 data/m4-$k 16 | tail -n 1 | awk '{print $NF}'`

#    text1=`$bin1 $arg`
#    text2=`$bin2 $arg`
#    text3=`$bin3 $arg`
#    text4=`$bin4 $arg`
#    text5=`$bin5 $arg`
#    text6=`$bin6 $arg`


 #   r1=`echo $text1 | tail -n 1` #开销
 #   p1=`echo $text1 | tail -n 2 | head -n 1` #时间

  #  r2=`echo $text2 | tail -n 1`
  #  p2=`echo $text2 | tail -n 2 | head -n 1`

   # r3=`echo $text3 | tail -n 1`
   # p3=`echo $text3 | tail -n 2 | head -n 1`
    
  #  r4=`echo $text4 | tail -n 1`
   # p4=`echo $text4 | tail -n 2 | head -n 1`

  #  r6=`echo $text6 | tail -n 1`
  #  p6=`echo $text6 | tail -n 2 | head -n 1`


	r1=` ../bin/localsearch data/matrix${size}-4-${k} ${size} 31  | tail -n 2 | head -n 1`
	r2=`../bin/hungarian2 data/matrix${size}-4-${k} ${size} 31 | tail -n 2 | head -n 1`
	r3=`../bin/random data/matrix${size}-4-${k} ${size} 31  | tail -n 2 | head -n1 `
	r4=`../bin/prob_init data/matrix${size}-4-${k} ${size} 31  | tail -n 2 | head -n1`
	r5=`../bin/robin data/matrix${size}-4-${k} ${size} 31  | tail -n 2 | head -n1`
	r6=`../bin/prob data/matrix${size}-4-${k} ${size} 31  | tail -n 2 | head -n1`


    r2=0
    r3=0
    r4=0

	#echo "$r1 $r2"
	t1=`echo "$t1+$r1" | bc`
	t2=`echo "$t2+$r2" | bc `	
	t3=`echo "$t3+$r3" | bc `	
	t4=`echo "$t4+$r4" | bc `	
	t5=`echo "$t5+$r5" | bc `	
	t6=`echo "$t6+$r6" | bc `	
	
	
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

    [ $( echo "$r6 < $min6" | bc ) -eq 1 ] &&   min6=$r6
	[ $( echo "$r6 > $max6" | bc ) -eq 1 ] &&   max6=$r6


done

t0=`sh hungry.sh ${size} data/matrix${size}-4-${k} | tail -n 1`

#echo "$t1 $t2"
t1=`echo "scale=5;$t1/$loop" | bc`
t2=`echo "scale=5;$t2/$loop" | bc`
t3=`echo "scale=5;$t3/$loop" | bc`
t4=`echo "scale=5;$t4/$loop" | bc`
t5=`echo "scale=5;$t5/$loop" | bc`
t6=`echo "scale=5;$t6/$loop" | bc`

workload=`tail -n 1 data/matrix${size}-4-${k}`
echo $workload

base=`python base.py $workload`
echo $base

t0=`echo "scale=6;$t0/$base" | bc`
t1=`echo "scale=6;$t1/$base" | bc`
t2=`echo "scale=6;$t2/$base" | bc`
t3=`echo "scale=6;$t3/$base" | bc`
t4=`echo "scale=6;$t4/$base" | bc`
t5=`echo "scale=6;$t5/$base" | bc`
t6=`echo "scale=6;$t6/$base" | bc`

echo "hungarian:$t0"
echo "local:$t1,min:$min1,max:$max1"
echo "hun2:$t2,min:$min2,max:$max2"
echo "random:$t3,min:$min3,max:$max3"
echo "prob_init:$t4,min:$min4,max:$max4"
echo "robin:$t5,min:$min5,max:$max5"
echo "prob:$t6,min:$min6,max:$max6"
echo ""

