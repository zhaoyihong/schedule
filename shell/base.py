#!/usr/bin/python
# coding=utf-8

db = {}

file = open("./base","r")
for line in file:
    list_pair = line.split()
    name = list_pair[0]
    name = name + ".mips"
    value = list_pair[1]
    db[name] = value


import sys
i = 1
argc = len(sys.argv)
result = 0

while i<argc:
    name = sys.argv[i]
    if name in db:
        result += float(db[name])
    i = i+1

print result 
