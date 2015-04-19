#!/usr/bin/env python
# coding=utf-8


一.已经字符串 s = "i,am,lilei",请用两种办法取出之间的“am”字符。

s="i,am,lilei"
print s[2:4]
print s.split(",")[1]


二.在python中，如何修改字符串？

s="hello,world"
s1 = s.replace("e","o");
print s1


三.bool("2012" == 2012) 的结果是什么。


print bool("2012" == 2012) #False



四.已知一个文件 test.txt，内容如下：
____________
2012来了。
2012不是世界末日。
2012欢乐多。
_____________

1.请输出其内容。
2.请计算该文本的原始长度。
3.请去除该文本的换行。
4.请替换其中的字符"2012"为"2013"。
5.请取出最中间的长度为5的子串。
6.请取出最后2个字符。
7.请从字符串的最初开始，截断该字符串，使其长度为11.
8.请将{4}中的字符串保存为test1.py文本.



file = open("test.txt","r")
content = file.read().decode("utf-8")  #不写读取个数,则读取到文件末尾
#1.请输出其内容。
print content
#2.请计算该文本的原始长度。
print len(content)
#3.请去除该文本的换行。
print content.replace("\n","").encode("utf-8")
#4.请替换其中的字符"2012"为"2013"。
content4=content.replace("2012","2013").encode("utf-8")
print content4
#5.请取出最中间的长度为5的子串。
print content[len(content)/2:len(content)/2+5].encode("utf-8")
#6.请取出最后2个字符。
print content[-2:].encode("utf-8")
#7.请从字符串的最初开始，截断该字符串，使其长度为11.
print content[:11].encode("utf-8")
#8.请将{4}中的字符串保存为test1.py文本.
file = open("test1.py","w")
file.write(content4) #如果是unicode类型会报错
file.close()



五.请用代码的形式描述python的引用机制。



import sys

cinfo = '1234'
print id(cinfo)
print sys.getrefcount('1234') 

binfo = '1234'
print id(binfo)
print sys.getrefcount('1234')

#cinfo 和 binfo的id是一样的,说明指向同一对象  并且 refcount增加



六.已知如下代码

________
a = "中文编程"
b = a
c = a
a = "python编程"
b = u'%s' % a
d = "中文编程"
e = a
c = b
b2 = a.replace("中","中")
________

1.请给出str对象"中文编程"的引用计数
2.请给出str对象"python编程"的引用计数


# 要记住 refcount 是从3开始的
# replace,字符串模板都是新生成一个字符串
# a=1 ,b=a 再 a=2 ,此时b仍然指向1

a = "中文编程"
print sys.getrefcount("中文编程") #4
b = a 
print sys.getrefcount("中文编程") #5
c = a 
print sys.getrefcount("中文编程") # 6
a = "python编程"
print sys.getrefcount("中文编程") #5
b = u"%s"%a.decode("utf-8")
print sys.getrefcount("中文编程") #4
d = "中文编程"
print sys.getrefcount("中文编程") #5
e = a #不增加
print sys.getrefcount("中文编程") #5
c = b #
print sys.getrefcount("中文编程") #4 
b2 = a.replace("中","中")
print sys.getrefcount("中文编程") #4




七.已知如下变量
________
a = "字符串拼接1"
b = "字符串拼接2"
________

1.请用四种以上的方式将a与b拼接成字符串c。并指出每一种方法的优劣。
2.请将a与b拼接成字符串c，并用逗号分隔。
3.请计算出新拼接出来的字符串长度，并取出其中的第七个字符。


a = "字符串拼接1"
b = "字符串拼接2"
print a+b
print "%s%s" % (a,b)
print "{},{}".format(a,b)
print "".join([a,b])




八.请阅读string模块，并且，根据string模块的内置方法输出如下几题的答案。

1.包含0-9的数字。
2.所有小写字母。
3.所有标点符号。
4.所有大写字母和小写字母。
5.请使用你认为最好的办法将{1-4}点中的字符串拼接成一个字符串。


import string 
print string.digits
print string.ascii_lowercase
print string.punctuation
print string.letters

letters=[]
letters.append(string.digits)
letters.append(string.ascii_lowercase)
letters.append(string.punctuation)
letters.append(string.letters)
print "".join(letters)





九.已知字符串
________

a = "i,am,a,boy,in,china"
________

1.假设boy和china是随时可能变换的，例boy可能改成girl或者gay，而china可能会改成别的国家，你会如何将上面的字符串，变为可配置的。
2.请使用2种办法取出其间的字符"boy"和"china"。
3.请找出第一个"i"出现的位置。
4.请找出"china"中的"i"字符在字符串a中的位置。
5.请计算该字符串一共有几个逗号。


#1
s = "i,am,a,%s,in,%s" % ("boy","china")
print s
#2 
print s.split(",")[3],s.split(",")[5]
print s[7:10],s[14:]
#3
print s.find("i")
#4
print s.find("i",s.find("china"))
print s.rfind("i")
#5 
print s.count(',')



十.请将模块string的帮助文档保存为一个文件。

import sys
import string
file = open("string_help.txt","w")
sys.stdout = file
print help("string")
file.close()



