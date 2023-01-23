#include <bitset>
#include <cctype>
#include <cerrno>
#include <clocale>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cstring> // string
#include <ctime>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <limits>
#include <io.h>
#include <list> 
#include <map>
#include <stdlib.h>
#include <iomanip>
#include <io.h>
#include <ios>
#include <iosfwd>
#include <iostream> // cin / cout
#include <istream>
#include <ostream> // file in / out
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <utility>
#include <vector>
#include <cwchar>
#include <cwctype>
#include <Urlmon.h>
#include <windows.h> // cmd
#include <cstdio> // scanf / printf
#include <conio.h> // 识别按键模块
#include <direct.h> // 文件判断模块
#include "requests.h" // 爬取网页模块
#include "utils.h"
#pragma comment(lib, "Urlmon.lib")

using namespace std; // cin / cout 定义模块
using namespace requests; // requests 定义模块
fstream _file;
ofstream fout;
ifstream fin;

string Frpc_version, App_version = "V2.0", id , token, Frp_log;

string Json(string json, string project) // 获取 json 的 project 项的值
{
    int json_start = json.find(project) + project.size() + 2, json_finish = 0;
    string json_return;
    if (json[json_start] == '"')
    {
        json_start++;
    }
    for (int i = json_start; i < json.size(); i++)
    {
        json_finish++;
        if (json[i + 1] == '"' || json[i + 1] == ',' || json[i + 1] == '}')
        {
            break;
        }
    }
    json_return = json.substr(json_start, json_finish);
    return json_return;
}
string DEC(string url, char c) // 删除转义符
{
    for (string::iterator i = url.begin(); i != url.end(); i++)
    {
        if (*i == c)
        {
            url.erase(i--);
        }
    }
    return url;
}
bool file(string filename) // 判断 filename 存不存在
{
    _file.open(filename, ios::in); // 尝试打开文件
    if (!_file) // 如果报错
    {
        _file.close();
        return false;
    }
    _file.close();
    return true;
}
LPCWSTR stringToLPCWSTR(string orig) // 将 LPCWSTR 形式的字符串转化为 String 形式的字符串
{
    size_t origsize = orig.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
    mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
    return wcstring;
}

int main()
{
    ShowWindow(GetForegroundWindow(), SW_HIDE); // 隐藏窗口，但是会闪一下-待改进

    if (_access("Logs", 0) == -1) // 判断日志文件夹是否存在
    {
        _mkdir("Logs");
    }
    cout << "Software initialization succeeded!\n";
    cout << "Reading Frpc Name......\n";
    FILE* FilePointer; // 文件安全指针
    fin.open("Frp-Name.log");
    fin >> id >> token;
    cout << "Frp name:" << id << "\n";
    fin.close();
    freopen_s(&FilePointer, (".\\Logs\\点我打开隧道 ID 为 " + id + " 的 Frpc 日志.bat").c_str(), "w", stdout); // 创建并打开文件
    cout << ("taskkill /f /im " + id + ".exe\nnotepad " + id +".log"); // 输入一些 Bat 代码
    freopen_s(&FilePointer, (".\\Logs\\点我打开隧道 ID 为 " + id + " 的 FrpAuxiliaryApp 日志.bat").c_str(), "w", stdout); // 创建并打开文件
    cout << "taskkill /f /im FrpAuxiliaryApp.exe\ntaskkill /f /im " + id + ".exe\nnotepad FrpAuxiliaryApp-" + id + ".log"; // 输入一些 Bat 代码
    Frp_log = ".\\Logs\\FrpAuxiliaryApp-" + id + ".log";
    freopen_s(&FilePointer, Frp_log.c_str(), "w", stdout); // 创建并打开文件



    cout << "Checking updates...\n";
    cout << "Version:" << App_version << '\n';
    string need_update = Json(Get("https://api.locyanfrp.cn/LoCyanFrpMSApp/AuxiliaryApp_update?version=" + App_version).GetText(), "needupdate");
    if (need_update == "1")
    {
        cout << "Oh!It need update!\n";
        string download_url = Json(Get("https://api.locyanfrp.cn/LoCyanFrpMSApp/AuxiliaryApp_update?version=114514").GetText(), "download_url");
        download_url = DEC(download_url, '\\');
        cout << "Getting......\n";
        URLDownloadToFile(nullptr, stringToLPCWSTR(download_url), L"UpdateTEMP", 0, nullptr);
        cout << "Updateing......";
        freopen_s(&FilePointer, "Update.bat", "w", stdout); // 创建并打开文件
        cout << "@echo off\nif \"%1\" == \"h\" goto begin\nmshta vbscript:createobject(\"wscript.shell\").run(\"\"\"%~nx0\"\" h\",0)(window.close)&&exit\n:begin\nREM\ntaskkill /f /im FrpAuxiliaryApp.exe\ndel /f /s /q FrpAuxiliaryApp.exe\nrename UpdateTEMP FrpAuxiliaryApp.exe\nstart FrpAuxiliaryApp.exe";
        fclose(stdout); // 关闭文件输出
        system("start Update.bat");
        return 0;
    }
    if (!file("Frpc.exe"))
    {
        string download_url = Json(Get("https://api.locyanfrp.cn/LoCyanFrpMSApp/Frpc_update?version=114514").GetText(), "download_url"), version = Json(Get("https://api.locyanfrp.cn/LoCyanFrpMSApp/Frpc_update?version=114514").GetText(), "version");
        download_url = DEC(download_url,'\\');
        URLDownloadToFile(nullptr, stringToLPCWSTR(download_url), L"DownLoadTEMP", 0, nullptr);
        system("rename DownLoadTEMP Frpc.exe");
        freopen_s(&FilePointer, "Frpc.version", "w", stdout); // 创建并打开文件
        cout << version << "\n";
        freopen_s(&FilePointer, Frp_log.c_str(), "w", stdout); // 打开文件
        cout << "Software initialization succeeded!\n";
        cout << "Oops!There is no Frpc in the directory, downloading...\n";
        cout << "Download succeeded!\n";
    }
    else {
        cout << "Checking for Frpc updates...\n";
        fin.open("Frpc.version");
        fin >> Frpc_version;
        fin.close();
        cout << "Frpc version:" << Frpc_version << "\n" ;
        cout << "Geting Frpc lastet updates...\n";
        need_update = Json(Get("https://api.locyanfrp.cn/LoCyanFrpMSApp/Frpc_update?version=" + Frpc_version).GetText(), "needupdate");
        if (need_update == "1")
        {
            cout << "Oh!It need update!";
            string download_url = Json(Get("https://api.locyanfrp.cn/LoCyanFrpMSApp/Frpc_update?version=114514").GetText(), "download_url"), version = Json(Get("https://api.locyanfrp.cn/LoCyanFrpMSApp/Frpc_update?version=114514").GetText(), "version"); // 获取最新版本版本号及下载地址
            system("del /f /s /q Frpc.exe");
            download_url = DEC(download_url, '\\');
            URLDownloadToFile(nullptr, stringToLPCWSTR(download_url), L"DownLoadTEMP", 0, nullptr);
            system("rename DownLoadTEMP Frpc.exe");
            freopen_s(&FilePointer, "Frpc.version", "w", stdout); // 创建并打开文件
            cout << version << "\n" ;
            freopen_s(&FilePointer, Frp_log.c_str(), "w", stdout); // 打开文件
            cout << "Software initialization succeeded!\n";
            cout << "Checking for Frpc updates...\n";
            cout << "Geting Frpc lastet updates...\n";
            cout << "Oh!It need update!";
            cout << "Update succeeded!\n";
        }
        else {
            cout << "Oh,It's lastet version!\n";
        }
    }



    cout << "Starting Frpc......\n";
    if (_access("Temp", 0) == -1) // 判断日志文件夹是否存在
    {
        cout << "Don't have .\\Temp\\, mkdiring...\n";
        _mkdir("Temp");
    }
    if (!file(".\\Temp\\" + id + ".exe"))
    {
        cout << "Copying...\nCmd return:";
        system("copy Frpc.exe .\\Temp\\FRPC_TEMP");
        cout << "Renameing...\n";
        system(("rename .\\Temp\\FRPC_TEMP " + id + ".exe").c_str());
    }
    freopen_s(&FilePointer, (".\\Logs\\" + id + ".log").c_str(), "w", stdout); // 打开文件
    system((".\\Temp\\" + id + " -u" + token + " -p" + id).c_str()); // 启动隧道
    //system("del /f /s /q Frp-Settings.ini");
    freopen_s(&FilePointer, Frp_log.c_str(), "w", stdout); // 打开文件
    cout << "Deleting Frp-Settings.ini...\n";
    cout << "Frpc is begin running, exiting...\n";
    cout << "Goodbye!";
    fclose(stdout); // 关闭文件输出
    return 0;
}