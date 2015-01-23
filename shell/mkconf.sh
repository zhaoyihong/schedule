#!/bin/bash
source ../config/config.sh



function usage()
{
	echo "Usage : $filename numbers(4x) destpath"
}



function checkenv()
{
	if [ -e "$destpath" ];then
		if [ ! -d "$destpath" ] ;then
			echo "Error:$destpath exists,and it's not a directionary! exit!"
			exit 1
		fi
	fi

	if [ ! -d "$destpath" ] ;then
		mkdir $destpath
	fi

}

function createConfs()
{
	cd $destpath
	local min=1000 #500MHZ
	local max=2000 #2000MHZ
	local cnt=1;
	for  i in `seq 1 4` 
	do
		filename=$ORGIN_CONFIG_PATH/cpu${i}.conf
		if [ ! -e  "$filename" ];then
			echo "$filename not exists!"
			continue
		fi

		for j in `seq 1 $loops`
		do
			local random=$(($RANDOM%($max-$min) + $min))"000000"
			local destfile=cpu${cnt}.conf 
			awk -va=$random '{if($0~/^Frequency/){printf("Frequency = %s",a ) } else {print $0} }' $filename > $destfile
			$SCHEDULE_BINPATH/wattchify $destfile tmp
			$SCHEDULE_BINPATH/cactify tmp tcpu${cnt}.conf
			
			cnt=$(($cnt + 1))
		done
	done

	cd -
}


filename=$0
if [ $# -ne 2 ] ; then
	 usage 
	 exit 1
fi

nums=$1
destpath=$2
loops=$(($nums/4))

checkenv
createConfs


#awk -Fv=123 '{if($0~/^Frequency/){printf("Frequency = %d",v ) } else {print $0} }' cpu1.conf
