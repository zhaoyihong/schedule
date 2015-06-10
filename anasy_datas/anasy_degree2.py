#-*- coding: utf-8 -*-
'''
    使用程序计算应用程序之间的差异性
    
    degrees.txt是应用程序在不同降级内核上运行的实际开销
    degrees2.txt 是应用程序在不同降级内核上运行的开销增长百分比

    我的核心思想是 如果两个程序对硬错误的敏感性不同，它们在两个上面两个文件中一定会出现差异。
    调度程序时应该尽量使用敏感性不同的应用来互换。

    这个程序是与其他应用各个降级的差的绝对值的和

'''

file = open('degrees.txt','r')
lno = 1
infos = list()
std = list()
for line in file:
    if lno != 1:
        curlist = line.split()
        curlist.pop(0)
        cpu0 = float(curlist.pop(0))
        std.append(cpu0)
        infos.append(curlist)
    lno += 1

#print infos

i = 0
j = 0
k = 0
size = len(curlist)
    
diff = 0.0
for i in range (0,15): #i是主动发起的比较的程序
    for x in range(0,15): #x是被动比较的处理器
        diff = 0.0
        for j in range(0,8): 
            for y in range(0,8):
                diff += abs(float(infos[i][j]) + float(infos[x][y]) - float(infos[i][y]) - float(infos[x][j])) 
        print diff,
    print 
