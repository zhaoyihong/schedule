#!/usr/bin/env python
# coding=utf-8

import sys

if len(sys.argv) != 2:
    print "Usage:%s loop" % sys.argv[0]
    sys.exit(-1)

loop = int(sys.argv[1])

file = open('hungarian_data','r')
result = 0.0

i = 0

for line in file:
    result += float(line.strip())
    i += 1
    if i == loop:
        break

print result

