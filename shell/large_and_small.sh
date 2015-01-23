#!/bin/bash

dir=`dirname $0`
source ../config/config.sh

#app=$1
small=$SCHEDULE_PATH/config/tsmall.conf
big=$SCHEDULE_PATH/config/tbig.conf




do_run_once()
{
	app=$1
	echo $app
	report=$SCHEDULE_PATH/report_$app
	rm $report
	cd $SESC_BUILD_PATH
	spoint=100
	for((i=0;i<10;++i)) ;do

		echo "123456" | sudo -S $SESC_SCRIPT_PATH/run.pl -sesc=sesc.mem -c=$small -bhome=$SPEC2000_PATH -bindir=$SPEC2000_BIN_PATH  -data=ref  -spoint=$spoint -spointsize=$SPOINT_SIZE -ext=.mips  $app 1>&2 
		sudo  $SESC_SCRIPT_PATH/report.pl -last >> $report 


		echo "" >> $report
		yes | $SESC_SCRIPT_PATH/run.pl --clean
		echo "-----------------------------------------" >> $report 
		echo "" >> $report 
		
		sudo $SESC_SCRIPT_PATH/run.pl -sesc=sesc.mem -c=$big -bhome=$SPEC2000_PATH -bindir=$SPEC2000_BIN_PATH  -data=ref  -spoint=$spoint -spointsize=$SPOINT_SIZE -ext=.mips  $app 1>&2 
		
		sudo $SESC_SCRIPT_PATH/report.pl -last >> $report 
		yes | $SESC_SCRIPT_PATH/run.pl --clean


		echo "-----------------------------------------" >> $report 
		echo "-----------------------------------------" >> $report 
		echo "" >> $report 
		echo "" >> $report 
		echo "" >> $report 
		echo "" >> $report 
		echo "" >> $report 


		spoint=$(($spoint +1))
	done
}



bins=`ls $SPEC2000_BIN_PATH | sed 's/.mips//g'`
for bin in $bins
do
	do_run_once $bin
done


