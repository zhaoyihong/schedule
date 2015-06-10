#-*- coding: utf-8 -*-
'''
   分析最大交换对会出现在哪些应用在哪些内核之间 

'''


#file = open('degrees.txt','r')
file = open('degrees.txt','r')
lno = 1
infos = list()
for line in file:
    if lno != 1:
        curlist = line.split()
        del curlist[0]
        del curlist[0]
        #print curlist
        infos.append(curlist)
    lno += 1

#print infos

i = 0
size = len(curlist)

result = list()

i=0
while i<15: 
    j=0
    while j<8:
        k = i+1
        while k < 15:
            x = 0
            while x < 8:
                swap_incress = float(infos[i][j]) + float(infos[k][x]) - float(infos[i][x]) - float(infos[k][j])
                result.append(((i,j,k,x),swap_incress))
                #if i == 9 and k == 10:
                #    print result[-1]
                #elif i == 10 and k == 9:
                #    print result[-1]

                #print '(%d %d)(%d %d) ' % (i,j,k,x) ,
                x+=1
            k+=1
        j+=1
        print 
    i+=1

result.sort(key=lambda x:(x[1]),reverse=True)

i = 0
while i < 50:
    print result[i]
    i+=1

top200 = result[0:50]
keys = [ (x[0][0],x[0][2]) for x in top200 ]
#print keys
keyset= set(keys)

key_cnt = [ (x,keys.count(x)) for x in keyset ]

print key_cnt

