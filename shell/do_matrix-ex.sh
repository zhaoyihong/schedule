#!/bin/bash
#生成一个三维矩阵用的
#行表示cpu,列表示bin
#结果输出在标准输出中

dir=`dirname $0`
source ../config/config.sh


loops=$1
begcpu=$2
size=$3

if [ $# != 3 ];
then
	echo "参数错误!"
	echo "$0 loops begcpu size"
	exit
fi


binall=(`ls $SPEC2000_BIN_PATH`)
#cpus=`seq 1 $size`
cpus=`seq $begcpu $(($begcpu+$size))`

#seq $begcpu $(($begcpu+$size))
#exit

bins=()
for((i=0;i<$size;++i))
do
	bins[$i]=${binall[$(($RANDOM%15))]}
done		

function testbins()
{
	for bin in ${binall[@]}
	do
		bin=`echo $bin | sed 's/.mips//'`
		sh do_runonce.sh 1 $bin 1
	done
	exit
}
#testbins



i=0
while true
do
	if [ "$i" -eq "$loops" ];then
		break
	fi
	
	echo "loop $i"

	for bin in ${bins[@]}
	do
		bin=`echo $bin | sed 's/.mips//'`
		line=""
		for cpu in ${cpus[@]}
		do
			#echo "sh do_runonce.sh $cpu $bin $i+50"
			ed2=`sh do_runonce.sh $cpu $bin $(($i+50))`
			line=$line"\t"$ed2
		done
		echo -e $line	
	done

	i=$(($i+1))
done



echo ""
echo "work load": ${bins[@]}
