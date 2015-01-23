#!/bin/bash

dir=`dirname $0`
source ../config/config.sh


if [ $# != 1 ];
then
	echo "参数错误!"
	echo "$0 matrixpath"
	exit
fi

matrix=$1
if [ ! -e "$matrix" ]; then
	echo "矩阵不存在!"
	exit
fi

schedule=([1]=0 [2]=0 [3]=0 [4]=0)


getEd2()
{
	[ $# != 3 ] && return
	loop=$1
	bin=$2
	cpu=$3

	row=$((($loop-1)*5+$bin+1))
	column=$cpu
	awk -vr=$row -vc=$column '{if(NR==r){print $c}}' $matrix	
}

randomSchedule()
{
	list=([1]=0 [2]=0 [3]=0 [4]=0)
	cpu=1

	while [ $cpu -ne 5  ] 
	do
		random=$(($RANDOM%4 + 1)) 
		while [ ${list[$random]} -ne 0 ] 
		do
			random=$(($RANDOM%4 + 1))
		done
		list[$random]=1
		schedule[$cpu]=$random
		cpu=$(($cpu+1))
	done
	
	for i in ${schedule[@]}
	do
		echo $i
	done		
}


printarray()
{
	for x in $@
	do
		echo -n $x
	done
	echo ""
}



#探索一个阶段 稳定9个阶段 总共16次
localsearch()
{
	list=`randomSchedule`
       	old[1]=`echo $list | awk '{print $1}'`
	old[2]=`echo $list | awk '{print $2}'`		
	old[3]=`echo $list | awk '{print $3}'`		
	old[4]=`echo $list | awk '{print $4}'`		

	printarray ${old[@]}
	echo "-------------"	
	result=0

	for((i=1;i<=24;i++))
	{
		#explore
		#loop=$((($i-1)*10+1))
		loop=$i
		if [ $(($i%2)) -eq 0 ];then #1-2 3-4
			new[1]=${old[2]}
			new[2]=${old[1]}
			new[3]=${old[4]}
			new[4]=${old[3]}
		
			echo -n "old:"	
			printarray ${old[@]}
			echo -n "swap1:"	
			printarray ${new[@]}

			oldt=0
			newt=0
			for((j=1;j<=4;j++))
			{
				ed_old[$j]=`getEd2 $loop $j ${old[$j]}`
				ed_new[$j]=`getEd2 $loop $j ${new[$j]}`
			}
			swap1_old=`echo "scale=6;${ed_old[1]} + ${ed_old[2]}" | bc `
			swap1_new=`echo "scale=6;${ed_new[1]} + ${ed_new[2]}" | bc `
			swap2_old=`echo "scale=6;${ed_old[3]} + ${ed_old[4]}" | bc `
			swap2_new=`echo "scale=6;${ed_new[3]} + ${ed_old[4]}" | bc `
			oldt=`echo "scale=6;$swap1_old + $swap2_old" | bc `
			newt=`echo "scale=6;$swap1_new + $swap2_new" | bc `
			if [ `echo "$newt<$oldt" | bc `  -eq "1" ];then
				old[1]=${new[1]}	
				old[2]=${new[2]}	
				old[3]=${new[3]}	
				old[4]=${new[4]}	
			else 
				if [ `echo "$swap1_new<$swap1_old" | bc`  -eq "1" ] ;then
					old[1]=${new[1]}	
					old[2]=${new[2]}				
				fi
				
				if [ `echo "$swap2_new<$swap2_old" | bc`  -eq "1" ] ;then
					old[3]=${new[3]}	
					old[4]=${new[4]}				
				fi	
			fi

			echo -n "swap2:"	
			printarray ${old[@]}
			
		else # 1-3 2-4
			new[1]=${old[3]}
                        new[2]=${old[4]}
                        new[3]=${old[1]}
                        new[4]=${old[2]}
			
			echo -n "old:"	
			printarray ${old[@]}
			echo -n "swap1:"	
			printarray ${new[@]}

                        oldt=0
                        newt=0
                        for((j=1;j<=4;j++))
                        {
                                ed_old[$j]=`getEd2 $loop $j ${old[$j]}`
                                ed_new[$j]=`getEd2 $loop $j ${new[$j]}`
                        }
                        swap1_old=`echo "scale=6;${ed_old[1]} + ${ed_old[3]}" | bc `
                        swap1_new=`echo "scale=6;${ed_new[1]} + ${ed_new[3]}" | bc `
                        swap2_old=`echo "scale=6;${ed_old[2]} + ${ed_old[4]}" | bc `
                        swap2_new=`echo "scale=6;${ed_new[2]} + ${ed_old[4]}" | bc `
                        oldt=`echo "scale=6;$swap1_old + $swap2_old" | bc `
                        newt=`echo "scale=6;$swap1_new + $swap2_new" | bc `
                        if [ `echo "$newt < $oldt" | bc` -eq "1" ];then
                                old[1]=${new[1]}
                                old[2]=${new[2]}
                                old[3]=${new[3]}
                                old[4]=${new[4]}
                        else
                                if [ `echo "$swap1_new < $swap1_old"  | bc` -eq "1" ] ;then
                                        old[1]=${new[1]}
                                        old[3]=${new[3]}
                                fi

                                if [ `echo "$swap2_new < $swap2_old" |bc`  -eq "1" ] ;then
                                        old[2]=${new[2]}
                                        old[4]=${new[4]}
                                fi
                        fi
			echo -n "swap2:"	
			printarray ${old[@]}
		fi
		
		#目前新的方案在explore阶段的开销,验证用
		ed_explore_new=0
		for((j=1;j<=4;j++))
		{
			ed2=`getEd2 $loop $j ${old[$j]}`
			ed_explore_new=`echo " scale=6;$ed_explore_new+$ed2" | bc`
		}
		echo "old cost: $oldt"	
		echo "new cost: $ed_explore_new"	
	
		#steady 	
		#for((k=1;k<=10;k++))
		#{	
		#	loop=$((($i-1)*10+$k))
		#	for((j=1;j<=4;j++))
                #	{
                 #       	ed2=`getEd2 $loop $j ${old[$j]}`
                 #       	result=`echo "scale=6;$result+$ed2" | bc`
                #	}
		#}

		for((j=1;j<=4;j++))
                {
                        ed2=`getEd2 $loop $j ${old[$j]}`
                        result=`echo "scale=6;$result+$ed2" | bc`
                }

		
		echo "-------------------------------"

	}

	echo "result:$result"



}


localsearch


