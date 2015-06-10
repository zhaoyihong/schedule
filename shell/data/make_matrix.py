#!/usr/bin/env python
# coding=utf-8

import sys,shelve

if len(sys.argv) != 3:
    print "Usage:%s workload beishu" % sys.argv[0]
    sys.exit(-1)


db = shelve.open('bindata8-9.db')
beishu = int(sys.argv[2])

i = 1
with open(sys.argv[1]) as f :
    for line in f:
        outfile = open('matrix%d-9-%d'% (beishu*4,i),'w')
        writebuff = list()
        works = line.split(',')
        works = works * beishu
        for loop in range(0,100):
            writebuff.append('loop %d\n' % loop)
            for work in works:
                work = work.strip()
                for j in range(beishu-1):
                     writebuff.append(db[work][loop].rstrip())
                writebuff.append(db[work][loop])
        writebuff.append('\n')
        workload_str = (line.rstrip() + ',') * (beishu-1) + line
        writebuff.append('work load:%s' % workload_str)
        outfile.writelines(writebuff)
        outfile.close()
        i = i+1
db.close()
