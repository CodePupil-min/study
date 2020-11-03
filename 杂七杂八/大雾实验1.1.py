# -*- coding: utf-8 -*-
# 2020/11/3
import execjs
import base64
import re


def main():
    print('''
写在前面：
    首先，先规范一下输入。
1、要输入表格的时候请将表格一行一行地全部输入进去，
    每个数字中间以空格隔开，不要加其他符号，输完后按回车。
2、输入数据的话就直接输入回车就行，不要输入其他符号。
    然后，本程序的算出来的结果只是参考，作者水平有限，如果有
计算错误还请自己手算，或者对比看一下的源码，估计是不小心删掉
了一些必要的东西。''')
    print('''
###下面开始运行：
========================================================================''')
    while True:
        S().Execute()
        print("按任意键继续。。。")
        input()


class S:
    bs64str = "" # 原始字符串
    old_str = "" # 解码后的字符串
    js_str = "" # 程序可以运行的字符串
    table_list_len = 0 # 表格个数
    fill_list_len = 0 # 空的个数
    table_name_list = [] # 哪个表
    fill_name_list = [] # 哪个空
    result = 0 # 计算结果
    enob = 0 # 有效位数

    def Execute(self):
        print("粘贴那一段很长的字符串↓")
        self.bs64str = input()
        self.AnalysisJs()
        print("\n初始源码====================================================")
        print(self.old_str)
        print("============================================================")
        if self.table_list_len == 0 and self.fill_list_len == 0:
            print("并不是计算代码")
            return
        print("\n修正后源码===================================================")
        print(self.js_str)
        print("============================================================")
        print("需要输入的表格个数:"+str(self.table_list_len))
        print("需要输入的数据个数:"+str(self.fill_list_len))
        self.ExecuteJs()
        print("\n计算的结果为："+str(self.result)+"\n")
        print("应该是保留"+str(self.enob)+"位有效数字或小数(也不一定)")
        print("PS：如果计算出来的结果太离谱就自己算吧。。。")

    def ExecuteJs(self):
        data_table_list = []
        data_list = []
        for i in range(0, self.table_list_len):
            print("输入本页第" + self.table_name_list[i] + "个表格的数据(空格隔开):")
            temp_list = list(map(float, input().strip().split()))
            data_table_list.append(temp_list)
        for i in range(0, self.fill_list_len):
            print("输入本页第" + self.fill_name_list[i] + "个空的数据:")
            data_list.append(input())
        js = execjs.compile(self.js_str)
        result = js.call('Count', data_table_list, data_list)
        self.result = result

    # 解析js语句
    def AnalysisJs(self):
        js_str = base64.b64decode(self.bs64str).decode("utf-8")
        self.old_str = js_str
        pattern0 =".*Float\..*;" # 用来匹配要删掉的语句
        pattern1 = "_q\[_map.table.*\].answers" # 匹配数组
        pattern2 = "_q\[_map.datafill.*\].answers" # 匹配空
        pattern3 = "[1-9]\d*" # 匹配数字
        delete_list = re.findall(pattern0, js_str, flags=0)
        table_list = re.findall(pattern1, js_str, flags=0)
        fill_list = re.findall(pattern2, js_str, flags=0)
        enob = 0
        table_name_list = []
        fill_name_list = []
        for i in range(0,len(delete_list)):
            js_str = js_str.replace(delete_list[i], "")
            e = re.findall(pattern3, delete_list[i], flags=0)
            if len(e) != 0:
                enob = e[0]
        for i in range(0, len(table_list)):
            var = "table["+str(i)+"]"
            js_str = js_str.replace(table_list[i], var)
            name = re.findall(pattern3, table_list[i], flags=0)
            if len(name) != 0:
                table_name_list.append(name[0])
            table_list[i] = var
        for i in range(0, len(fill_list)):
            var = "fill["+str(i)+"]"
            js_str = js_str.replace(fill_list[i], var)
            name = re.findall(pattern3, fill_list[i], flags=0)
            if len(name) != 0:
                fill_name_list.append(name[0])
            fill_list[i] = var

        head = "function Count(table,fill){\n"
        var0 = "res"
        temp = re.findall(r'var (.*)=', js_str)
        if len(temp) != 0:
            var0 = temp[-1]
        end = "return "+var0+";\n}"
        self.js_str = head + js_str + end
        self.table_list_len = len(table_list)
        self.fill_list_len = len(fill_list)
        self.table_name_list = table_name_list
        self.fill_name_list = fill_name_list
        self.enob = enob


if __name__ == '__main__':
    main()
