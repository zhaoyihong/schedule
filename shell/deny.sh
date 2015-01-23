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
MAX_SCORE=100
DECREASE_LIMIT=0.25

for((b=1;b<=16;b++))
{
	score[$b]=$MAX_SCORE
	isdecreased[$b]="0"
}
 

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


random_cnt=0
random_record[""]=""
#不重复的随机数 且与历史序列也不重复 注意 只能运行24次
#
randomSchedule() 
{
	random_cnt=$(($random_cnt + 1))
	flag=1
	while [ $flag -eq 1 ]
	do 
		record=""
		cpu=1
		list=([1]=0 [2]=0 [3]=0 [4]=0)
		flag=0
		while [ $cpu -ne 5  ] 
		do
			random=$(($RANDOM%4 + 1)) 
			while [ ${list[$random]} -ne 0 ] 
			do
				random=$(($RANDOM%4 + 1))
			done
			scorenum=$((($cpu-1)*4 + $random))
			if [ `echo "$(($RANDOM%$MAX_SCORE)) > ${score[$scorenum]} " | bc` -eq 1  ]  ; then
				#忽略这个分配 重新分配
				continue
			fi
			list[$random]=1
			schedule[$cpu]=$random
			record=$record""$random
			cpu=$(($cpu+1))
		done

		#判断这个分配是否重复
		for a in ${random_record[@]}
		do
			if [ "$record" == "$a" ];then
				flag=1
				break
			fi
		done 	
	done

	random_record[$random_cnt]=$record
	for b in ${schedule[@]}
	do
		echo -n $b" "
	done		
	echo ""
}


printarray()
{
	for x in $@
	do
		echo -n $x
	done
	echo ""
}



#探索一个阶段 稳定9个阶段 总共24次
localsearch()
{
	list=`randomSchedule`
       	old[1]=`echo $list | awk '{print $1}'`
	old[2]=`echo $list | awk '{print $2}'`		
	old[3]=`echo $list | awk '{print $3}'`		
	old[4]=`echo $list | awk '{print $4}'`		

	printarray ${old[@]}
	result=0
	echo "-------------"	

	for((i=1;i<=24;i++))
	{
		#explore
		#loop=$((($i-1)*10+1))
		loop=$i
		list=`randomSchedule`
		new[1]=`echo $list | awk '{print $1}'`
        	new[2]=`echo $list | awk '{print $2}'`
        	new[3]=`echo $list | awk '{print $3}'`
        	new[4]=`echo $list | awk '{print $4}'`		
		
		new_t=0
		old_t=0
		printarray ${old[@]}
		printarray ${new[@]}
		
		for((j=1;j<=4;j++)) #bin
                {
		     echo "loop=$loop,j=$j"
                     ed_old=`getEd2 $loop $j ${old[$j]}`
                     ed_new=`getEd2 $loop $j ${new[$j]}`
		     new_t=`echo "scale=6;$ed_new + $new_t" | bc `
		     old_t=`echo "scale=6;$ed_old + $old_t" | bc `
		     if [ ${old[$j]} -eq ${new[$j]} ] ; then
			 echo "ed[$j][${old[$j]}] , ed[$j][${new[$j]}]  same core "
			 continue
		     fi
		    {
		     #set -x 
		     newnum=$((  (${new[$j]} + 4*($j-1))  ))
		     oldnum=$((  (${old[$j]} + 4*($j-1))  ))
		     if [[ `echo "$ed_old<$ed_new" | bc ` -eq "1"  &&  -z  `echo ${isdecreased[$newnum]} | grep $oldnum ` ]];then
	#		score[ $newnum ]=$(( ${score[$newnum]}  - 30 ))
			[ `echo "$ed_new-0.0001<0" | bc` -eq 1 ] && ed_new=0.0001 
		     	decrease_rate=`echo "scale=6;$ed_old  / $ed_new" | bc`
		     	[ `echo " $decrease_rate < $DECREASE_LIMIT" | bc` -eq 1 ] && decrease_rate=$DECREASE_LIMIT
			#score[ $newnum ]=`echo "scale=6;${score[$oldnum]}  *  $ed_old  / $ed_new" | bc `
			score[ $newnum ]=`echo "scale=6;${score[$oldnum]}  *  $decrease_rate" | bc `
			echo "ed[$j][${old[$j]}] <  ed[$j][${new[$j]}] " 
			isdecreased[$newnum]="${isdecreased[$newnum]}_$oldnum"	
  		     elif [[ `echo "$ed_old>$ed_new" | bc ` -eq "1"  &&   -z ` echo ${isdecreased[$oldnum]} | grep $newnum ` ]];then
	#		score[ $oldnum ]=$(( ${score[$oldnum]}  - 30 ))
			[ `echo "$ed_old-0.0001<0" | bc` -eq 1 ] && ed_old=0.0001 
		     	decrease_rate=`echo "scale=6;$ed_new  / $ed_old" | bc`
		     	[ `echo " $decrease_rate < $DECREASE_LIMIT" | bc` -eq 1 ] && decrease_rate=$DECREASE_LIMIT
			#score[ $oldnum ]=` echo "scale=6;${score[$newnum]}  * $ed_new  / $ed_old " | bc `
			score[ $oldnum ]=` echo "scale=6;${score[$newnum]}  * $decrease_rate " | bc `
			echo "ed[$j][${old[$j]}]  >  ed[$j][${new[$j]}] " 
			isdecreased[$oldnum]="${isdecreased[$oldnum]}_$newnum"	
		     elif [[ `echo "$ed_old == $ed_new" | bc ` -eq "1"  ]];then
			echo "ed[$j][${old[$j]}] == ed[$j][${new[$j]}]  "
		     else
			 echo "not equal but decreased"
			 echo -e  "ed[$j][${old[$j]}]\t$ed_old\t$oldnum\t ${isdecreased[$oldnum]}\ned[$j][${new[$j]}]\t$ed_new\t$newnum\t${isdecreased[$newnum]} " 

		     fi
		     set +x 
		     }
                }
		
		echo "old cost: $old_t"	
		echo "new cost: $new_t"	
	
		if [ `echo "$old_t>$new_t" | bc ` -eq "1"  ];then
		   echo "swap"
		    old[1]=${new[1]}
                    old[2]=${new[2]}
                    old[3]=${new[3]}
                    old[4]=${new[4]}
		fi
						
		for((b=1;b<=16;b++))
		{
			echo -n ${score[$b]}" "
		}
		echo ""
		
		for x in ${isdecreased[@]}
		do
			echo  -n $x" "
		done 
		echo ""
	
	
		#steady 	
		#for((k=1;k<=10;k++))
		#{	
		#	loop=$((($i-1)*10+$k))
		#	for((j=1;j<=4;j++))
                #	{
                #       	ed2=`getEd2 $loop $j ${old[$j]}`
                #      	   	result=`echo "scale=6;$result+$ed2" | bc`
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

#for((x=1;x<=24;x++))
#{
#	randomSchedule
#}

