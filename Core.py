import json
import requests
import sys
import os
import mysql.connector
import ctypes
import urllib3

# 隐藏python窗口
whnd = ctypes.windll.kernel32.GetConsoleWindow()
if whnd != 0:
    ctypes.windll.user32.ShowWindow(whnd, 0)
    ctypes.windll.kernel32.CloseHandle(whnd)

# 连接数据库
try:
    http = urllib3.PoolManager()
    http.request('GET', 'https://api.locyanfrp.cn')

    db = mysql.connector.connect(
            host="",
            port=,
            user="",
            passwd="",
            database=""
    )
    db_connect = True
except:
    db_connect = False


def getnamebycode(code):  # 通过token获取用户名
    cursor = db.cursor()
    cursor.execute("SELECT * FROM tokens WHERE `token` = " + "'" + code + "';")
    result = cursor.fetchall()
    for i in result:
        print(i[1])
        return i[1]


f = open("SQLTransmission.sys", "r", encoding="ansi")
a = f.read()
text = a.split("\n")
action = text[0]

if action == "1":  # 获取服务器列表
    # 获取文件路径
    filepath = os.getcwd() + "\\" + "ServerList.sys"
    # 创建文件
    file = open(filepath, "w", encoding="ansi")
    # 数据库获取信息
    cursor = db.cursor()
    cursor.execute("SELECT * FROM nodes")
    result = cursor.fetchall()
    # 将信息写入
    file.write(str(len(result)) + "\n")  # 加入服务器数量
    for i in result:
        file.write(str(i[0]) + " " + i[1] + " " + i[3] + "\n")
    file.close()

elif action == "2":  # 获取用户隧道

    user = getnamebycode(text[1])

    # 获取文件路径
    filepath = os.getcwd() + "\\" + "MyServerList.dll"
    # 创建文件
    file = open(filepath, "w", encoding="ansi")
    # 数据库获取信息
    cursor = db.cursor()
    cursor.execute("SELECT * FROM proxies WHERE `username` = " + "'" + user + "';")
    result = cursor.fetchall()

    file.write(str(len(result)) + "\n")  # 加入隧道数量

    if len(result) == 0:  # 若没有隧道则直接结束运行
        sys.exit()
    print(result)
    for i in result:
        ue = i[6]
        uz = i[7]
        domain = i[8]

        if ue == "true":
            ue = "1"
        else:
            ue = "0"

        if uz == "true":
            uz = "1"
        else:
            uz = "0"

        if domain == "" or domain is None:
            file.write(str(i[0]) + " " + i[2] + " " + str(i[5]) + " " + str(i[11]) + " " + i[3] + " " + str(
                i[16]) + " " + ue + " " + uz + " " + "0" + " " + i[4] + "\n")
        else:
            domain = domain.replace('["', '')
            domain = domain.replace('"]', '')
            file.write(str(i[0]) + " " + i[2] + " " + str(i[5]) + " " + str(i[11]) + " " + i[3] + " " + str(
                i[16]) + " " + ue + " " + uz + " " + "1" + " " + domain + " " + i[4] + "\n")

    file.close()
else:
    print("failed execute this document")
f.close()
db.close()
