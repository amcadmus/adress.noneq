#!/bin/bash

targets=`ls | grep "^pert"`
# rm -f h.count.name

make -C tools/h.bond makedir
make -C tools/h.bond -j 4
make -C tools/average
make -C tools/average -j 4

# for i in $targets;
# do
#     echo "# processing $i"
#     cd $i
#     if test ! -f mytop; then
# 	echo "old version, no mytop"
# 	../tools/h.bond/equi.ch4.1 -o h.count.out &> /dev/null
#     else
# 	../tools/h.bond/equi.ch4.1 -t mytop -o h.count.out &> /dev/null
#     fi
#     cd ..
#     echo "$i/h.count.out" >> h.count.name
# done

./tools/average/average.file


