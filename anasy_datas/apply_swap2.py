#-*- coding: utf-8 -*-
'''
  使用统计的方法 分析最大交换对会出现在哪些应用在哪些内核之间 

'''

def local_search(infos):
    global schedule #指派方案
    print schedule
    core =0
    cost = 0.0
    for app in schedule:
        cost += float(infos[app][core])
        core += 1
    print "old cost : ",cost
    
    newcost = 0.0
    global tongji #离线信息,记录特殊交换对
    global disabled_swaps #交换对是否可用
    global last_swaps #上一轮的使用的特殊交换对

    #暂时禁用上一轮的特殊交换对
    last_swaps_status = disabled_swaps[:]
    for i in last_swaps:
        disabled_swaps[i] = 1

    tongji2 = [ tongji[i] for i in range(0,len(tongji)) if disabled_swaps[i] == 0] #所有可用的交换对
   
    

    #恢复上一轮的特殊交换对在disabled_swaps中的状态
    for i in last_swaps:
        disabled_swaps[i] = last_swaps_status[i]

    last_swaps = list()

    swaps = list()    #本轮的所有交换对
    usedapp = list()  #已被调度的app
    
    if len(tongji2) != 0:
         for i in xrange(0,10):
            index = random.randint(0,len(tongji2)-1)
            sw = tongji2[index]             
            if sw[0] not in usedapp and sw[1] not in usedapp: 
                usedapp.append(sw[0])
                usedapp.append(sw[1])  
                swaps.append(sw)
                disabled_swaps[tongji.index(sw)] = 1
                last_swaps.append(tongji.index(sw))
    print swaps
    print disabled_swaps

    if len(swaps) != 4:
        unusedapp = [ i  for i in xrange(0,size)  if i not in usedapp]
        lenu = len(unusedapp)
        random.shuffle(unusedapp)
        for i in xrange(0,lenu/2):
            swaps.append((unusedapp[i],unusedapp[lenu-i-1]))
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
                for i in range(0,len(disabled_swaps)):
                    if disabled_swaps[i] == 1:
                        if app1 in tongji[i] or app2 in tongji[i]:
                            disabled_swaps[i] = 0
            else:
                newcost += oscost
    else:
        disabled_swaps = [0] * len(tongji)

    schedule =  nschedule
    print schedule
    print "new cost : ",newcost
    print 
    return newcost


if __name__ == '__main__':

    import sys,random
    if len(sys.argv) != 5:
        print "Usage: %s matrix_path index size interval" % sys.argv[0]
        sys.exit(-1)
    

    path = sys.argv[1]
    size = int(sys.argv[3])
    interval = int(sys.argv[4])

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
        loop = all[ 1+(size+1)*i : (size+1)*(i+1) ]
        allloops.append(loop);
    
    import shelve 
    db  = shelve.open('swaps.db')
    tongji = db['workload%s' % sys.argv[2] ] 
    

    print tongji

    schedule = range(0,size)
    random.shuffle(schedule)

    loop =1000
    total = 0.0
    for x in xrange(0,loop):
        random.shuffle(schedule) #初始化
        disabled_swaps = [0] * len(tongji) #交换对是否可用
        last_swaps = list() #上一轮使用的交换对序号，不可以连续两轮都使用同一个交换对
        for i in xrange(0,interval):
            infos = allloops[i]
            total += local_search(infos)
        print
        print 

    print total/loop

    db.close()
