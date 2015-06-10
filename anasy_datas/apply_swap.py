#-*- coding: utf-8 -*-
'''
  使用统计的方法 分析最大交换对会出现在哪些应用在哪些内核之间 

'''

def local_search(infos):
    global schedule
    
    print schedule
    core =0
    cost = 0.0
    for app in schedule:
        cost += float(infos[app][core])
        core += 1
    print "old cost : ",cost
    
    newcost = 0.0

    #产生随机交换对
    swaps = list()
    apps = range(0,8)
    random.shuffle(apps)
    for i in xrange(0,4):
        swaps.append((apps[i],apps[i+4]))
    print swaps
    
    nschedule = schedule[:]
    for app1,app2 in swaps:
        core1 = schedule.index(app1)
        core2 = schedule.index(app2)
        nschedule[core1] = app2
        nschedule[core2] = app1
        newcost += float(infos[app2][core1]) + float(infos[app1][core2])

   
    if cost < newcost:
        nschedule = schedule[:]
        newcost = 0.0
        for app1,app2 in swaps:
            core1 = schedule.index(app1)
            core2 = schedule.index(app2)
            oscost = float(infos[app1][core1]) + float(infos[app2][core2])
            nscost = float(infos[app1][core2]) + float(infos[app2][core1])
            
            if nscost < oscost:   
                nschedule[core1] = app2
                nschedule[core2] = app1
                newcost += nscost
            else:
                newcost += oscost

    schedule = nschedule
    print schedule
    print "new cost : ",newcost
    print 
    return newcost


if __name__ == '__main__':

    import sys,random
    if len(sys.argv) != 3:
        print "Usage: %s matrix_path interval" % sys.argv[0]
        sys.exit(-1)
    

    path = sys.argv[1]
    interval = int(sys.argv[2])
    
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
    
    schedule = range(0,8)
    total = 0.0
    loop = 1000
    for x in xrange(0,loop):
        random.shuffle(schedule)
        for  i in xrange(0,interval):
            infos = allloops[i]
            total += local_search(infos)
        print
        print

    print total/loop


