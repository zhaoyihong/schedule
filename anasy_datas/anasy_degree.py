#-*- coding: utf-8 -*-
'''
    使用程序计算应用程序之间的差异性
    
    degrees.txt是应用程序在不同降级内核上运行的实际开销
    degrees2.txt 是应用程序在不同降级内核上运行的开销增长百分比

    我的核心思想是 如果两个程序对硬错误的敏感性不同，它们在两个上面两个文件中一定会出现差异。
    调度程序时应该尽量使用敏感性不同的应用来互换。
    

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

while i<15:
    j=0
    while j<15:
        degree = 0.0
        k = 0
        while k < size:
            degree += abs(float(infos[i][k])+std[j] -float(infos[j][k])-std[i])
            k+=1
        print degree,' ',
        j+=1 
    i+=1
    print 

    

