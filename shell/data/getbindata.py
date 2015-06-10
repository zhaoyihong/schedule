#!/usr/bin/env python
# coding=utf-8

import sys

if len(sys.argv) != 2:
    print "Usage:%s matrix_path" % sys.argv[0]
    sys.exit(-1)
path = sys.argv[1]

import shelve
db = shelve.open('bindata8-9.db')

file = open(path,'r')
lines = file.readlines()
works = lines[-1].strip().replace('work load:',' ').split()
print works

for i in range(0,len(works)):
    work_data = list()
    for j in range(0,100):
        work_data.append(lines[i+1+j*9])
        db[works[i]] = work_data


for x,y in db.items():
    print x
    print y
file.close()
db.close()



