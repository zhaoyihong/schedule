#!/bin/bash

dir=`dirname $0`
source ../config/config.sh

cpuid=$1
app=$2
spoint=$3


if [ $# != 3 ];
then
	echo "参数错误!"
	echo "sh $0 cpuid(1-128) app(gzip,gcc,parser,crafty...) spoint(x10M)"
	exit
fi


#conf=$SCHEDULE_PATH/config/cpu${cpuid}_t.conf
conf=$SCHEDULE_PATH/config/128confs/tcpu${cpuid}.conf
if [ ! -e "$conf" ];then 
	echo "错误:$conf 不存在"
	exit
fi

cd $SESC_BUILD_PATH

$SESC_SCRIPT_PATH/run.pl -sesc=sesc.mem -c=$conf -bhome=$SPEC2000_PATH -bindir=$SPEC2000_BIN_PATH  -data=ref  -spoint=$spoint -spointsize=$SPOINT_SIZE -ext=.mips  $app 1>&2 
#$SESC_SCRIPT_PATH/run.pl -sesc=sesc.mem -c=$SCHEDULE_PATH/config/cpu${cpuid}_t.conf -bhome=$SPEC2000_PATH -bindir=$SPEC2000_BIN_PATH  -data=ref  -spoint=$spoint -spointsize=$SPOINT_SIZE -ext=.mips  $app 1>&2 


result=`$SESC_SCRIPT_PATH/report.pl -last`
#$SESC_SCRIPT_PATH/report.pl -last
$SESC_SCRIPT_PATH/report.pl -last | awk '{if(FNR==4){time=$7};if(FNR==21){energy=$7}} END{print energy*time*time} '

rm -rf words lib *.dat *.blk ref.* test.*


