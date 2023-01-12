import requests
import os
import urllib3
import time
try:
    # 启动时删除上一次的日志文件，防止堆积
    logs_status = os.path.exists("./Logs/Core.log")
    if logs_status is True:
        # w+ 可以新建文件的读写模式
        os.remove("./Logs/Core.log")


    # 创建日志文件并写入
    def writelog(message):
        # 第一次启动创建日志目录
        logs_dir_status = os.path.exists("./Logs")
        if logs_dir_status is False:
            os.mkdir("./Logs")
        # 软件运行过程中检测是第一次执行还是第N+1次执行，保证日志文件不被覆盖
        logs_status = os.path.exists("./Logs/Core.log")
        if logs_status is False:
            # w+ 可以新建文件的读写模式
            logs = open("./Logs/Core.log", "w", encoding="ansi")
        else:
            # a+ 可以追加内容的读写模式
            logs = open("./Logs/Core.log", "a+", encoding="ansi")
        logs.write("[" + time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) + "]" + message + "\n")
        logs.close()


    writelog("Launching Python Core...")
    writelog("Starting to check the config...")

    bat_status = os.path.exists("./Logs/点我打开 Core 日志.bat")
    if bat_status is False:
        writelog("It is the first launching , create the control bat file successfully!")
        f = open("./Logs/点我打开 Core 日志.bat", "w+", encoding="ansi")
        f.write("taskkill /f /im Core.exe\nnotepad Core.log")
        f.close()

    # 连接API准备工作
    api_url = "https://api.locyanfrp.cn"
    try:
        writelog("Starting to init the requests...")
        # 关闭keep-alive
        s = requests.session()
        s.keep_alive = False
        # 设置重连次数为5
        requests.DEFAULT_RETRIES = 5
        # 关闭urllib3 不验证SSL产生的报错
        urllib3.disable_warnings()
    except:
        writelog("Failed, Stopping the Python Core...")
        exit()


    def getnamebycode(code):  # 通过token获取用户名
        url = api_url + "/Account/GetUserNameByFrpToken?token=" + code
        rs = requests.get(url, verify=False).json()
        if rs["status"] != 0:
            return 0
        else:
            return rs["username"]


    # 主程序
    f = open("SQLTransmission.sys", "r", encoding="ansi")
    a = f.read()
    text = a.split("\n")
    action = text[0]

    if action == "1":  # 获取服务器列表
        # 获取文件路径
        filepath = os.getcwd() + "\\" + "ServerList.dll"
        # 创建文件
        file = open(filepath, "w", encoding="ansi")
        url = api_url + "/Proxies/GetServerList"

        rs = requests.get(url, verify=False).json()
        # 将信息写入
        file.write(str(len(rs)) + "\n")  # 加入服务器数量
        for i in rs:
            file.write(str(i["id"]) + " " + i["name"] + " " + i["hostname"] + "\n")
        writelog("Getting Server List Successfully")
        file.close()

    elif action == "2":  # 获取用户隧道
        writelog("Starting to get user's proxies")
        user = getnamebycode(text[1])
        # 获取文件路径
        filepath = os.getcwd() + "\\" + "MyServerList.dll"
        # 创建文件
        file = open(filepath, "w+", encoding="ansi")
        # 数据库获取信息

        url = api_url + "/Proxies/GetProxiesListByUsername?username=" + user
        rs = requests.get(url, verify=False).json()
        file.write(str(len(rs["proxies"])) + "\n")
        if rs["status"] == 0:
            # rs["proxies"]为一个数组
            for i in rs["proxies"]:
                ue = i["use_encryption"]
                uc = i["use_compression"]
                domain = i["domain"]
                # 转换格式
                if ue == "true":
                    ue = "1"
                else:
                    ue = "0"
                if uc == "true":
                    uc = "1"
                else:
                    uc = "0"
                if domain == "" or domain is None:
                    file.write(file.read() + str(i["id"]) + " " + i["proxy_name"] + " " + str(i["local_ip"]) + " " + str(
                        i["remote_port"]) + " " + i["proxy_type"] + " " + str(
                        i["node"]) + " " + ue + " " + uc + " " + "0" + " " + i["local_ip"] + "\n")
                else:
                    domain = domain.replace('["', '')
                    domain = domain.replace('"]', '')
                    file.write(file.read() + str(i["id"]) + " " + i["proxy_name"] + " " + str(i["local_ip"]) + " " + str(
                        i["remote_port"]) + " " + i["proxy_type"] + " " + str(
                        i["node"]) + " " + ue + " " + uc + " " + "0" + " " + domain + " " + i["local_ip"] + "\n")
            writelog("Getting proxies successfully")
        else:
            writelog("Getting user's proxies failed, they dont have proxies now!")
            exit()
        file.close()
    elif action == "3":
        # 获取文件路径
        filepath = os.getcwd() + "\\" + "Update.dll"
        # 创建文件
        file = open(filepath, "w+", encoding="ansi")
        file.write("V1.0")
        writelog("Successfully Get the current version")
        file.close()
    else:
        writelog("failed execute this document")
    f.close()
except Exception as e:
    logs = open("./Logs/Core.log", "a+", encoding="ansi")
    logs.write("[" + time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) + "]" + str(e) + "\n")
    logs.close()
