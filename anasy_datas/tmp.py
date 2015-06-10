#!/usr/bin/env python
# coding=utf-8

file = open('matrix8-1-1')
row = 1
gcc=list()
bzip2=list()
for line in file:
    if row%3 == 2:
        gcc.append(line.strip().split())
    if row%3 == 0:
        bzip2.append(line.strip().split())
    row +=1

print gcc
print bzip2

s1 = [ float(gcc[i][0]) + float(bzip2[i][1]) for i in range(0,len(gcc))]
s2 = [ float(gcc[i][1]) + float(bzip2[i][0]) for i in range(0,len(gcc))]

for x in s1:
    print x

print 

print 


for y in s2:
    print y

print 

for i in range(0,len(bzip2)):
    print bzip2[i][0]

print 

for i in range(0,len(bzip2)):
    print bzip2[i][1]
