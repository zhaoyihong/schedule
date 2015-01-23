#!/bin/sh

if [ $# -lt 2 ] ;then
	echo "$0 xuhao size"
	exit
fi

x=$1 #序号
size=$2 #大小

if [ $size -eq 4 ] ;then
	lines=82
elif [ $size -eq 16 ];then
	lines=274
elif [ $size -eq 8 ];then
	lines=146
fi

path=matrix$size-$x 
{
echo "loop 0"
 ../bin/getAverage $(($size*$size)) 16 $path  | tail -n 1 | awk  -vline=$size -F" " '{ for(i=1;i<=NF;++i){ if(i%(line)==0){ printf ($i"\n") }else {printf($i" ") } }}' 
#tail -n 82  $path
tail -n $lines  $path
}> m$size-$x

#../bin/normalize $size m$size-$x > m$size-${x}n

