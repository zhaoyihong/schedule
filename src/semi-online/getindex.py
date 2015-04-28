#!/usr/bin/env python
# coding=utf-8

import sys

l1 = ['ammp', 'bzip2', 'crafty', 'equake', 'gap', 'gcc', 'gzip', 'mcf', 'mesa', 'parser', 'swim', 'twolf', 'vortex', 'vpr', 'wupwise']


l2 = sys.argv[1].split(',')

print l2

print [ l1.index(name.strip()) for name in l2 ]


