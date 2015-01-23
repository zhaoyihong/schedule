#!/bin/sh

# 调用 do_matrix-ex.sh 

for i in `seq 1 8`
do
	sh do_matrix-ex.sh 32 10 4  > matrix4-1-$i
done
