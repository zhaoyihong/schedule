#!/usr/bin/env python
# coding=utf-8

f1 = open('app_diff', 'r')
f2 = open('workindex64', 'r')

infos = list()
for line in f1:
    tmp = line.split()
    infos.append(tmp)

workloads = list()
for line in f2:
    line = line.replace(',',' ')
    tmp = line.split()
    workloads.append(tmp)
size = len(tmp)

#print workloads

import shelve
f3 = shelve.open('swaps.db')
k = 1
for works in workloads:
    if len(works) == 0:
        break
    workinfo = list()
    nums = list()
    for i in works:
        myline = list()
        for j in works:
            print i,j
            myline.append(float(infos[int(i)][int(j)]))
        workinfo.append(myline)
        nums.extend(myline)

    #print works
    #print workinfo
    nums.sort()
    #print nums
    mid = nums[len(nums)/2]
    #print mid

    swaps = list()
    for x in range(0, size):
        for y in range(x + 1, size):
            if workinfo[x][y] > mid:
                swaps.append((x, y))
    print swaps
    f3['workload%d' % k] = swaps 
    k += 1    
    print 

f1.close()
f2.close()
f3.close()
