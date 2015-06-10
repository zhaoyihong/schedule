#-*- coding: utf-8 -*-
'''
  使用统计的方法 分析最大交换对会出现在哪些应用在哪些内核之间 

'''
def analy_matrix(infos):
    result = list()
    i=0
    while i<8: 
        j=0
        while j<8:
            k = i+1
            while k < 8:
                x = 0
                while x < 8:
                    swap_incress = float(infos[i][j]) + float(infos[k][x]) - float(infos[i][x]) - float(infos[k][j])
                    result.append(((i,j,k,x),swap_incress))
                    #print '(%d %d)(%d %d) ' % (i,j,k,x) ,
                    x+=1
                k+=1
            j+=1
        i+=1

    result.sort(key=lambda x:(x[1]),reverse=True)

    i = 0
    while i < 10:
        print result[i]
        i+=1

    top200 = result[0:50]
    keys = [ (x[0][0],x[0][2]) for x in top200 ]
    #print keys
    keyset= set(keys)

    key_cnt = [ (x,keys.count(x)) for x in keyset ]

    print key_cnt

if __name__ == '__main__':
    import sys
    if len(sys.argv) != 2:
        print "Usage: %s matrix_path" % sys.argv[0]
        sys.exit(-1)
    
    path = sys.argv[1]
    
    #file = open('degrees.txt','r')
    file = open(path,'r')
    lno = 1
    all = list()
    for line in file:
        curlist = line.split()
            #print curlist
        all.append(curlist)
        lno += 1

    #print all 

    #infos = all[0:9]

    i = 0
    allloops = list()
    for i in xrange(0,100):
        loop = all[ 1+9*i : 9*(i+1) ]
        allloops.append(loop);
        #print loop

    for i in xrange(1,5):
        analy_matrix(allloops[i])
    #print allloops


