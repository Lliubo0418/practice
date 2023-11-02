name = " aleX"
# a. 移除 name 变量对应的值两边的空格，并输入移除有的内容
print(name.strip())
# b.判断 name 变量对应的值是否以 "al" 开头，并输出结果
print(name.startswith('al'))
# c.判断 name 变量对应的值是否以 "X" 结尾，并输出结果
print(name.endswith('X'))
# d.将 name 变量对应的值中的 “ l” 替换为 “ p”，并输出结果
print(name.replace('l','p'))
# e.将 name 变量对应的值根据 “ l” 分割，并输出结果。
print(name.split('l'))
# f.请问，上一题 e分割之后得到值是什么类型？
print(name.split("e"))
# g.将 name 变量对应的值变大写，并输出结果
print(name.upper())
# h.将 name 变量对应的值变小写，并输出结果
print(name.lower())
# i.请输出 name 变量对应的值的第 2 个字符？
print(name[2])
# j. 请输出 name 变量对应的值的前 3 个字符？
print(name[0:3])
# k. 请输出 name 变量对应的值的后 2 个字符？
print(name[-2:])
# l.请输出 name 变量对应的值中 “ e” 所在索引位置？
print(name.index('e'))


print('************')
print('******列表******')

li = ['alex','eric','rain']
# a.计算列表长度并输出
print(len(li))
# b.列表中追加元素 “seven”，并输出添加后的列表
li.append('seven')
print(li)
# c.请在列表的第1个位置插入元素 “Tony”，并输出添加后的列表
li.insert(0,"Tony")
print(li)
# d.请修改列表第2个位置的元素为 “Kelly”，并输出修改后的列表
li[1] = "Kelly"
print(li)
# e.请删除列表中的元素 “eric”，并输出修改后的列表
li.remove("eric") 
print(li)


print('************')
print('******元组******')
tu=('alex','eric','rain')

# a.计算元组长度并输出
print(len(tu))
# b.获取元组的第2个元素，并输出
print(tu[1])
# c.获取元组的第 1-2个元素，并输出
print(tu[0:2])
# d.请使用 for 输出元组的元素
for i in tu:
    print(i)
# e.请使用 for、len、range 输出元组的索引
for i in range(len(tu)):
    print(i,tu[i])


print('************')
print('******字典******')

dic ={'k1':"v1","k2":"v2","k3":[11,22,33]}
# a.请循环输出所有的 key
for i in dic:
    print(i)
# b.请循环输出所有的value
for i in dic:
    print(dic[i])
# c.请循环输出所有的 key 和 value
for i in dic:
    print(i,dic[i])
# d.请在字典中添加一个键值对，"k4":"v4"，输出添加后的字典
dic["k4"] = "v4"
print(dic)
# e.请在修改字典中 “k1” 对应的值为 “alex”，输出修改后的字典
dic['k1'] = "alex"
print(dic)
# f.请在 k3 对应的值中追加一个元素44，输出修改后的字典
dic["k3"].append(44)
print(dic)
# g.请在 k3 对应的值的第1个位置插入个元素18，输出修改后的字典
dic["k3"].insert(0,18)
print(dic)