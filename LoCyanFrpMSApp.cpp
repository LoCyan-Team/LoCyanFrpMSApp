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
#include <direct.h> // 文件夹创建模块
#include "requests.h" // 爬取网页模块
#include "utils.h" // requests.h 的前置
#define VERSION (string)"Version 1.3"
#pragma comment(lib, "Urlmon.lib")

using namespace std; // cin / cout 定义模块
using namespace requests; // requests 定义模块
ofstream fout;
ifstream fin;
fstream _file;

inline void main2(); // 隧道管理选择模块
inline void main3(); // 映射选项选择模块
inline void main4(); // 关于界面功能选择模块
inline void login(); // 登录模块
inline void Unlogin(); // 登出模块
inline void PrintGUI1(); // 显示 GUI
inline void PrintGUI2(); // 显示隧道管理模块 GUI
inline void PrintGUI3(); // 显示映射选项模块 GUI
inline void PrintGUI4(); // 显示关于模块 GUI
inline void PrintNodeList(); // 打印节点列表
inline void PrintTunnelList(bool must_online, bool must_offline); // 打印隧道列表
inline void GetNode(); // 获取节点列表
inline void GetTunnel(); // 获取隧道列表
inline void Delete(); // 删除隧道模块
inline void Create(); // 创建隧道模块
inline void Start(); // 启动映射模块
inline void End(); // 关闭映射模块
inline void color(short attr); // 输出彩色文字
inline void Cout(int x, char a); // 连续输出 x 次 a
inline void print(string s, int a, int b); // 输出提示语
inline bool file(string filename); // 判断文件是否存在
inline bool ifTunnel(int id); // 判断隧道是否存在
inline bool ifNode(int id); // 判断节点是否存在 True
inline bool ifOnline(int id); // 获取隧道在线状态
inline int FromIDFindNodeLen(int id); // 根据节点 ID 找在数组中的下标
inline int numlen(int num); // 获取一个数字的长度（位数）
inline int FromIDFindNodeID(int id); // 根据隧道 ID 找节点 ID
inline int FromIDFindTTunnelLen(int id); // 根据隧道 ID 找数组下标
inline string DEC(string url, char c); // 删除转义符
inline string Json(string json, string project); // 获取 Json 某个项的值
inline string FromIDFindNodeName(int id); // 根据节点 ID 找名字
inline string FromIDFindNodeUrl(int id); // 根据节点 ID 找地址
inline string FromIDFindTunnelName(int id); // 根据隧道 ID 找名字
inline string FromIDFindProtocol(int id); // 根据隧道 ID 找隧道协议
inline string LTS(string s); // 大写转小写
inline string UTF8ToANSI(string s);  // 编码转换
inline string ANSItoUTF8(string strAnsi); // 将 ANSI 编码转换为 UTF-8
inline LPCWSTR stringToLPCWSTR(string orig); // LPCWSTR -> String
string UserInternalID/* FrpToken */, name/* 用户名 */, password/* 密码 */, Temp/* 更新缓存 */;
bool ifLogin = false/* 是否已登录 */, ifAuxiliary = false /* 有没有下载辅助模块 */;
int StartingTunnelList[100010], Node_quantity/* 节点数量 */, MyTunnelLen/* 用户的隧道数量 */, TestLen = 0/* 特殊情况下 PrintTunnelList 输出的隧道数 */, OnlineLen = 0/* 在线隧道数 */, Online[100010] = {}/* 在线隧道 */, UnusefulNodeLen = 0;

struct Nodeinfo // 节点信息
{
	bool use;
	int ID = 0;
	string Name, IP, RealIP;
}Node[100010];
struct Tunnelinfo
{
	bool online = false;
	int ID = 0, Node = 0, a = 0, b = 0;
	string Name, Protocol, IP, url, internal_port, external_port;
}Tunnel[1000010];

int main() // 主函数模块
{
	print("正在初始化程序", 51, 52);
	SetConsoleTitle(L"LoCyanFrp 智能控制面板"); // 设置标题
	HWND hWnd = GetConsoleWindow();
	RECT rc;
	GetWindowRect(hWnd, &rc);
	SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
	SetWindowPos(hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, NULL); // 设置窗口长宽高
	auto hStdin = ::GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	::GetConsoleMode(hStdin, &mode); // 禁止左键选择
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	::SetConsoleMode(hStdin, mode);
	system("mode con cols=119 lines=30"); // 设置窗口长宽高
	system("cls");
	print("正在执行初始化", 51, 52);

	Temp = Get("https://api.locyanfrp.cn/LoCyanFrpMSApp/MSApp_update?version=" + VERSION).GetText();
	if (Json(Temp, "needupdate") == "1")
	{
		string Return = Json(Temp, "important");
		if (Return == "1")
		{
			print("正在更新中", 53, 54);
			string download_url = Json(Temp, "download_url");
			download_url = DEC(download_url, '\\');
			URLDownloadToFile(nullptr, stringToLPCWSTR(download_url), L"UpdateTEMP", 0, nullptr);
			fout.open("Update.bat"); // 创建并打开文件
			fout << "@echo off\nif \"%1\" == \"h\" goto begin\nmshta vbscript:createobject(\"wscript.shell\").run(\"\"\"%~nx0\"\" h\",0)(window.close)&&exit\n:begin\nREM\ntaskkill /f /im LoCyanFrpMSApp.exe\ndel /f /s /q LoCyanFrpMSApp.exe\nrename UpdateTEMP LoCyanFrpMSApp.exe\nstart LoCyanFrpMSApp.exe";
			fout.close();
			system("start Update.bat");
			return 0;
		}
		else {
			system("cls");
			Cout(119, '-');
			cout << "|";
			Cout(117, ' ');
			cout << "|\n|";
			Cout(48, ' ');
			color(14);
			cout << "有新版本！是否更新？";
			color(9);
			Cout(49, ' ');
			cout << "|\n|";
			Cout(43, ' ');
			color(2);
			cout << "是（按 Y）";
			color(9);
			cout << "     |     ";
			color(4);
			cout << "否（按 N）";
			color(9);
			Cout(43, ' ');
			cout << "|\n|";
			Cout(117, ' ');
			cout << "|\n";
			Cout(119, '-');
			while (true)
			{
				if (_kbhit()) // 判断是否有按按键
				{
					char a = _getch(); // 判断按了啥键     
					switch (a)
					{
					case 'y': {
						print("正在更新中", 53, 54);
						string download_url = Json(Temp, "download_url");
						download_url = DEC(download_url, '\\');
						URLDownloadToFile(nullptr, stringToLPCWSTR(download_url), L"UpdateTEMP", 0, nullptr);
						fout.open("Update.bat"); // 创建并打开文件
						fout << "@echo off\nif \"%1\" == \"h\" goto begin\nmshta vbscript:createobject(\"wscript.shell\").run(\"\"\"%~nx0\"\" h\",0)(window.close)&&exit\n:begin\nREM\ntaskkill /f /im LoCyanFrpMSApp.exe\ndel /f /s /q LoCyanFrpMSApp.exe\nrename UpdateTEMP LoCyanFrpMSApp.exe\nstart LoCyanFrpMSApp.exe";
						fout.close();
						system("start Update.bat");
						return 0;
					}
					case 'n':print("正在执行初始化", 51, 52); goto Go;
					case 'Y': {
						print("正在更新中", 53, 54);
						string download_url = Json(Temp, "download_url");
						download_url = DEC(download_url, '\\');
						URLDownloadToFile(nullptr, stringToLPCWSTR(download_url), L"UpdateTEMP", 0, nullptr);
						fout.open("Update.bat"); // 创建并打开文件
						fout << "@echo off\nif \"%1\" == \"h\" goto begin\nmshta vbscript:createobject(\"wscript.shell\").run(\"\"\"%~nx0\"\" h\",0)(window.close)&&exit\n:begin\nREM\ntaskkill /f /im LoCyanFrpMSApp.exe\ndel /f /s /q LoCyanFrpMSApp.exe\nrename UpdateTEMP LoCyanFrpMSApp.exe\nstart LoCyanFrpMSApp.exe";
						fout.close();
						system("start Update.bat");
						return 0;
					}
					case 'N':print("正在执行初始化", 51, 52); goto Go;
					}
				}
				Sleep(20); // 防止卡 / 占用过多资源
			}
		}
	}
Go:;
	if (file("AppLogin.dll")) // 判断 FrpAuxiliaryApp.exe 是否存在，存在就读取
	{
		fin.open("AppLogin.dll"); // 打开文件
		fin >> ifLogin; // 获取是否登录
		if (ifLogin)
		{
			fin >> UserInternalID >> name >> password; // 如已登录，获取 Frp 的 Token
		}
		fin.close();
		GetTunnel();
		if (file("OnlineTunnel.list"))
		{
			fin.open("OnlineTunnel.list"); // 打开文件
			fin >> OnlineLen; // 获取是否登录
			for (int i = 0; i < OnlineLen; i++)
			{
				fin >> Online[i];
				if (ifOnline(Online[i]))
				{
					Tunnel[FromIDFindTTunnelLen(Online[i])].online = true;
				}
				else {
					OnlineLen--;
				}
			}
			fin.close();
		}
	}
	GetNode();

	system("cls");
	PrintGUI1(); // 打印 GUI
	while (true)
	{
		if (_kbhit()) // 判断是否有按按键
		{
			char a = _getch(); // 判断按了啥键     
			switch (a)
			{
			case '1':login(); PrintGUI1(); break;
			case '2':Unlogin(); PrintGUI1(); break;
			case '3':main2(); PrintGUI1(); break;
			case '4':main4(); PrintGUI1(); break;
			case '5':return 0; break;
			}
		}
		Sleep(20); // 防止卡 / 占用过多资源
	}
	return 0;
}
inline void main2() // 隧道管理选择模块
{
	system("cls");
	if (!ifLogin)
	{
		Cout(119, '-');
		cout << "|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(50, ' ');
		color(4);
		cout << "您当前尚未登录！";
		color(9);
		Cout(51, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n";
		Cout(119, '-');
		Sleep(2000);
		system("cls");
		return;
	}
	PrintGUI2();
	while (true)
	{
		if (_kbhit())
		{
			char a = _getch();
			switch (a)
			{
			case 'D': Delete(); PrintGUI2(); break;
			case 'C': Create(); PrintGUI2(); break;
			case 'O': main3(); PrintGUI2(); break;
			case 'R': system("cls"); return;
			case 'd': Delete(); PrintGUI2(); break;
			case 'c': Create(); PrintGUI2(); break;
			case 'o': main3(); PrintGUI2(); break;
			case 'r': system("cls"); return;
			}
		}
		Sleep(20);
	}
	return;
}
inline void main3() // 映射选项选择模块
{
	system("cls");
	if (MyTunnelLen == 0)
	{
		print("您没有任何一个隧道！", 48, 49);
		Sleep(2000);
		system("cls");
		return;
	}
	if (!file("FrpAuxiliaryApp.exe") && !ifAuxiliary) // 判断 FrpAuxiliaryApp.exe 是否存在，不存在就下载
	{
		Cout(119, '-');
		cout << "|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(49, ' ');
		color(4);
		cout << "正在下载辅助模块...";
		color(9);
		Cout(49, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n";
		Cout(119, '-');
		string download_url = Json(Get("https://api.locyanfrp.cn/LoCyanFrpMSApp/AuxiliaryApp_update?version=114514").GetText(), "download_url"); // 获取下载地址
		download_url = DEC(download_url, '\\'); // 删除转义符
		URLDownloadToFile(nullptr, stringToLPCWSTR(download_url), L"DownLoadTEMP", 0, nullptr); // 下载
		system("rename DownLoadTEMP FrpAuxiliaryApp.exe"); // 成功
		system("cls");
	}
	ifAuxiliary = true;

	PrintGUI3();
	while (true)
	{
		if (_kbhit())
		{
			char a = _getch();
			switch (a)
			{
			case 'S': Start(); PrintGUI3(); break;
			case 'F': End(); PrintGUI3(); break;
			case 'R': system("cls"); return;
			case 's': Start(); PrintGUI3(); break;
			case 'f': End(); PrintGUI3(); break;
			case 'r': system("cls"); return;
			}
		}
		Sleep(20);
	}
	return;
}
inline void main4() // 关于界面功能选择模块
{
	PrintGUI4();
	while (true)
	{
		if (_kbhit()) // 判断是否有按按键
		{
			char a = _getch(); // 判断按了啥键     
			switch (a)
			{
			case 'R':system("cls"); return;
			case 'r':system("cls"); return;
			}
		}
		Sleep(20); // 防止卡 / 占用过多资源
	}
	return;
}

inline void login() // 登录模块
{
	if (ifLogin) // 如果已登录
	{
		system("cls");
		Cout(119, '-');
		cout << "|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(53, ' ');
		color(4);
		cout << "您已登录！";
		color(9);
		Cout(54, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n";
		Cout(119, '-');
		Sleep(2000); // 等待一会-让用户看清提示语
		system("cls");
		return;
	}
	while (true) // 死循环保持在登录界面
	{
		system("cls");
		color(9);
		Cout(48, '-');
		color(5);
		cout << "LoCyan Frp 智能控制面板";
		color(9);
		Cout(48, '-');
		cout << "|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(42, ' ');
		color(4);
		cout << "每项内容填好后请按 “回车” 键";
		color(9);
		Cout(45, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n";
		Cout(119, '-');
		HANDLE hout;
		COORD coord = { 47 , 3 };
		hout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hout, coord); // 在第 47 行，第 3 列插入文字
		cout << "用户名：";
		cin >> name; // 用户名输入-以后要居中-待改进
		name = ANSItoUTF8(name);
		coord = { 47 , 5 };
		hout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hout, coord); // 在第 47 行，第 5 列插入文字
		cout << "密码：";
		char ch;
		password = "";
		while ((ch = _getch()) != '\r') // 读取输入字符，直到回车键被按下
		{
			if (ch == '\b') // 处理退格键
			{
				if (!password.empty())
				{
					password.pop_back(); // 删除末尾的字符
					_putch('\b');     // 退格
					_putch(' ');      // 输出空格覆盖原字符
					_putch('\b');     // 退格回到原位置
				}
			}else{
				password.push_back(ch); // 将输入字符保存到字符串中
				_putch('*');         // 在终端上显示的字符用 "*" 填充
			}
		}
		password = ANSItoUTF8(password);
		print("正在校验密码", 52, 53);
		string Return = Json(Get("https://api.locyanfrp.cn/User/DoLogin?username=" + name + "&password=" + password).GetText(), "status"); // 查看密码是否正确
		if (Return == "-1")
		{
			print("用户名或密码错误！", 49, 50);
			Sleep(3000);
			continue;
		}
		else {
			string login_token = Json(Get("https://api.locyanfrp.cn/User/DoLogin?username=" + name + "&password=" + password).GetText(), "token"); // 获取用于登录的 Token
			UserInternalID = Json(Get("https://api.locyanfrp.cn/Account/info?username=" + name + "&token=" + login_token).GetText(), "token"); // 获取用于 Frp 的 Token
			fout.open("AppLogin.dll");
			fout << 1 << " " << UserInternalID << " " << name << " " << password;
			fout.close();
			ifLogin = true;
			print("正在获取隧道...", 51, 51);
			GetTunnel();
			print("登录成功！", 53, 54);
			Sleep(2000);
			system("cls");
			return;
		}
	}
	return;
}
inline void Unlogin() // 退出登录模块
{
	if (!ifLogin) // 如果还没有登录
	{
		system("cls");
		Cout(119, '-');
		cout << "|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(50, ' ');
		color(4);
		cout << "您当前尚未登录！";
		color(9);
		Cout(51, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n";
		Cout(119, '-');
		Sleep(2000);
		system("cls");
		return;
	}
	print("正在退出登录", 52, 53);
	Sleep(2000);
	system("del /f /s /q AppLogin.dll>nul"); // 把登陆文件删了
	system("del /f /s /q OnlineTunnel.list>nul"); // 把本地启动隧道列表删了
	ifLogin = false; // 不要忘记再把变量清了
	system("cls");
	return;
}
inline void PrintGUI1() // 显示 GUI
{
	color(9);
	Cout(48, '-');
	color(5);
	cout << "LoCyan Frp 智能控制面板";
	color(9);
	Cout(48, '-');
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(52, ' ');
	color(6);
	cout << "登录（按 1）";
	color(9);
	Cout(53, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(52, ' ');
	color(6);
	cout << "登出（按 2）";
	color(9);
	Cout(53, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(50, ' ');
	color(6);
	cout << "隧道管理（按 3）";
	color(9);
	Cout(51, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(52, ' ');
	color(6);
	cout << "关于（按 4）";
	color(9);
	Cout(53, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(50, ' ');
	color(6);
	cout << "退出程序（按 5）";
	color(9);
	Cout(51, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n";
	color(9);
	Cout(119, '-');
	if (!ifLogin)
	{
		cout << "|";
		Cout(117, ' ');
		cout << "|\n|";
		Cout(50, ' ');
		color(4);
		cout << "您当前尚未登录！";
		color(9);
		Cout(51, ' ');
		cout << "|\n|";
		Cout(117, ' ');
		cout << "|\n";
		Cout(119, '-');
	}
	return;
}
inline void PrintGUI2() // 显示隧道管理模块 GUI
{
	color(9);
	Cout(48, '-');
	color(5);
	cout << "LoCyan Frp 智能控制面板";
	color(9);
	Cout(48, '-');
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(50, ' ');
	color(6);
	cout << "删除隧道（按 D）";
	color(9);
	Cout(51, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(50, ' ');
	color(6);
	cout << "创建隧道（按 C）";
	color(9);
	Cout(51, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(50, ' ');
	color(6);
	cout << "映射选项（按 O）";
	color(9);
	Cout(51, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(50, ' ');
	color(6);
	cout << "返回上级（按 R）";
	color(9);
	Cout(51, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n";
	color(9);
	Cout(119, '-');
	return;
}
inline void PrintGUI3() // 显示映射选项模块 GUI
{
	color(9);
	Cout(48, '-');
	color(5);
	cout << "LoCyan Frp 智能控制面板";
	color(9);
	Cout(48, '-');
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(50, ' ');
	color(6);
	cout << "启动映射（按 S）";
	color(9);
	Cout(51, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(50, ' ');
	color(6);
	cout << "关闭映射（按 F）";
	color(9);
	Cout(51, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(50, ' ');
	color(6);
	cout << "返回上级（按 R）";
	color(9);
	Cout(51, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n";
	color(9);
	Cout(119, '-');
	return;
}
inline void PrintGUI4() // 显示关于模块 GUI
{
	system("cls");
	color(9);
	Cout(48, '-');
	color(5);
	cout << "LoCyan Frp 智能控制面板";
	color(9);
	Cout(48, '-');
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(43, ' ');
	color(14);
	cout << "LoCyanFrp Manager and Start App";
	color(9);
	Cout(43, ' ');
	cout << "|\n|";
	Cout(38, ' ');
	color(10);
	cout << "一个小而精致的 LoCyanFrp 隧道启动、管理器";
	color(9);
	Cout(38, ' ');
	cout << "|\n|";
	Cout(49, ' ');
	cout << "Powered by";
	color(6);
	cout << " C++ ";
	color(9);
	cout << "/";
	color(6);
	cout << " C";
	color(9);
	Cout(50, ' ');
	cout << "|\n|";
	Cout(37, ' ');
	color(8);
	cout << "开发人员：";
	color(11);
	cout << "Zhiyuan";
	color(9);
	cout << " | ";
	color(8);
	cout << "技术支持：";
	color(11);
	cout << "Daiyangcheng";
	color(9);
	Cout(38, ' ');
	cout << "|\n|";
	Cout(31, ' ');
	cout << "开源地址：";
	color(11);
	cout << "https://github.com/LoCyan-Team/LoCyanFrpMSApp";
	color(9);
	Cout(31, ' ');
	cout << "|\n";
	Cout(119, '-');
	cout << "|";
	string ver = "软件版本：" + VERSION;
	int len = ver.length();
	if ((40 - len) % 2 == 0)
	{
		int k = (40 - len) / 2;
		Cout(k - 1, ' ');
		cout << "软件版本：";
		color(14);
		cout << VERSION;
		color(9);
		Cout(40 - k - len, ' ');
	}
	else {
		int k = (40 - len - 1) / 2;
		Cout(k, ' ');
		cout << "软件版本：";
		color(14);
		cout << VERSION;
		color(9);
		Cout(40 - k - len - 1, ' ');
	}
	cout << "|";
	string c;
	bool temp = false;
	len = c.length();
	if (Json(Temp, "needupdate") == "1")
	{
		temp = true;
		c = "有新版本！";
	}
	else {
		c = "此程序为最新版本！";
	}
	if ((60 - len) % 2 == 0)
	{
		int k = (60 - len) / 2;
		Cout(k - 1, ' ');
		if (temp)
		{
			color(4);
		}
		else {
			color(2);
		}
		cout << c;
		color(9);
		Cout(60 - k - len, ' ');
	}
	else {
		int k = (60 - len - 1) / 2;
		Cout(k, ' ');
		if (temp)
		{
			color(4);
		}
		else {
			color(2);
		}
		cout << c;
		color(9);
		Cout(60 - k - len - 1, ' ');
	}
	cout << "|";
	Cout(119, '-');
	cout << "\n|";
	Cout(54, ' ');
	color(4);
	cout << "按 R 退出";
	color(9);
	Cout(54, ' ');
	cout << "|\n";
	Cout(119, '-');
	return;
}
inline void PrintNodeList() // 打印节点列表
{
	color(9);
	Cout(48, '-');
	color(5);
	cout << "LoCyan Frp 智能控制面板";
	color(9);
	Cout(48, '-');
	cout << endl;
	string a = "节点 ID", b = "节点名称", c = "节点 IP";
	int len = a.length();
	cout << "|";
	if ((15 - len) % 2 == 0)
	{
		int k = (15 - len) / 2;
		Cout(k - 1, ' ');
		cout << a;
		Cout(15 - k - len, ' ');
	}
	else {
		int k = (15 - len - 1) / 2;
		Cout(k, ' ');
		cout << a;
		Cout(15 - k - len - 1, ' ');
	}
	cout << "|";
	len = b.length();
	if ((53 - len) % 2 == 0)
	{
		int k = (53 - len) / 2;
		Cout(k - 1, ' ');
		cout << b;
		Cout(53 - k - len, ' ');
	}
	else {
		int k = (53 - len - 1) / 2;
		Cout(k, ' ');
		cout << b;
		Cout(53 - k - len - 1, ' ');
	}
	cout << "|";
	len = c.length();
	if ((50 - len) % 2 == 0)
	{
		int k = (50 - len) / 2;
		Cout(k - 1, ' ');
		cout << c;
		Cout(50 - k - len, ' ');
	}
	else {
		int k = (50 - len - 1) / 2;
		Cout(k, ' ');
		cout << c;
		Cout(50 - k - len - 1, ' ');
	}
	cout << "|\n";
	for (int i = 0; i < Node_quantity; i++)
	{
		if (!Node[i].use)
		{
			continue;
		}
		int len = numlen(Node[i].ID);
		cout << "|";
		color(12);
		if ((15 - len) % 2 == 0)
		{
			int k = (15 - len) / 2;
			Cout(k - 1, ' ');
			cout << Node[i].ID;
			Cout(15 - k - len, ' ');
		}
		else {
			int k = (15 - len - 1) / 2;
			Cout(k, ' ');
			cout << Node[i].ID;
			Cout(15 - k - len - 1, ' ');
		}
		color(9);
		cout << "|";
		color(13);
		len = Node[i].Name.length();
		if ((15 - len) % 2 == 0)
		{
			int k = (53 - len) / 2;
			Cout(k - 1, ' ');
			cout << Node[i].Name;
			Cout(53 - k - len, ' ');
		}
		else {
			int k = (53 - len - 1) / 2;
			Cout(k, ' ');
			cout << Node[i].Name;
			Cout(53 - k - len - 1, ' ');
		}
		color(9);
		cout << "|";
		color(15);
		len = Node[i].IP.length();
		if ((15 - len) % 2 == 0)
		{
			int k = (50 - len) / 2;
			Cout(k - 1, ' ');
			cout << Node[i].IP;
			Cout(50 - k - len, ' ');
		}
		else {
			int k = (50 - len - 1) / 2;
			Cout(k, ' ');
			cout << Node[i].IP;
			Cout(50 - k - len - 1, ' ');
		}
		color(9);
		cout << "|\n";
	}
	Cout(119, '-');
	cout << endl;
	return;
}
inline void PrintTunnelList(bool must_online, bool must_offline) // 打印隧道列表
{
	TestLen = 0;
	color(9);
	Cout(48, '-');
	color(5);
	cout << "LoCyan Frp 智能控制面板";
	color(9);
	Cout(48, '-');
	cout << endl;
	string a = "隧道 ID", b = "隧道名称", c = "隧道信息", d = "是否加密", e = "是否压缩", f = "使用节点";
	int len = a.length();
	cout << "|";
	if ((7 - len) % 2 == 0)
	{
		int k = (7 - len) / 2;
		Cout(k - 1, ' ');
		cout << a;
		Cout(7 - k - len, ' ');
	}
	else {
		int k = (7 - len - 1) / 2;
		Cout(k, ' ');
		cout << a;
		Cout(7 - k - len - 1, ' ');
	}
	cout << "|";
	len = b.length();
	if ((27 - len) % 2 == 0)
	{
		int k = (27 - len) / 2;
		Cout(k - 1, ' ');
		cout << b;
		Cout(27 - k - len, ' ');
	}
	else {
		int k = (27 - len - 1) / 2;
		Cout(k, ' ');
		cout << b;
		Cout(27 - k - len - 1, ' ');
	}
	cout << "|";
	len = f.length();
	if ((38 - len) % 2 == 0)
	{
		int k = (38 - len) / 2;
		Cout(k - 1, ' ');
		cout << f;
		Cout(38 - k - len, ' ');
	}
	else {
		int k = (38 - len - 1) / 2;
		Cout(k, ' ');
		cout << f;
		Cout(38 - k - len - 1, ' ');
	}
	cout << "|";
	len = c.length();
	if ((27 - len) % 2 == 0)
	{
		int k = (27 - len) / 2;
		Cout(k - 1, ' ');
		cout << c;
		Cout(27 - k - len, ' ');
	}
	else {
		int k = (27 - len - 1) / 2;
		Cout(k, ' ');
		cout << c;
		Cout(27 - k - len - 1, ' ');
	}
	cout << "|";
	len = d.length();
	if ((9 - len) % 2 == 0)
	{
		int k = (9 - len) / 2;
		Cout(k - 1, ' ');
		cout << d;
		Cout(9 - k - len, ' ');
	}
	else {
		int k = (9 - len - 1) / 2;
		Cout(k, ' ');
		cout << d;
		Cout(9 - k - len - 1, ' ');
	}
	cout << "|";
	len = e.length();
	if ((9 - len) % 2 == 0)
	{
		int k = (9 - len) / 2;
		Cout(k - 1, ' ');
		cout << e;
		Cout(9 - k - len, ' ');
	}
	else {
		int k = (9 - len - 1) / 2;
		Cout(k, ' ');
		cout << e;
		Cout(9 - k - len - 1, ' ');
	}
	cout << "|\n";
	for (int i = 0; i < MyTunnelLen; i++)
	{
		if (must_online && !Tunnel[i].online)
		{
			continue;
		}
		if (must_offline && Tunnel[i].online)
		{
			continue;
		}
		TestLen++;
		int an = Tunnel[i].ID;
		b = Tunnel[i].Name, f = FromIDFindNodeName(Tunnel[i].Node);
		len = numlen(an);
		cout << "|";
		color(6);
		if ((7 - len) % 2 == 0)
		{
			int k = (7 - len) / 2;
			Cout(k, ' ');
			cout << an;
			Cout(7 - k - len, ' ');
		}
		else {
			int k = (7 - len - 1) / 2;
			Cout(k, ' ');
			cout << an;
			Cout(7 - k - len, ' ');
		}
		color(9);
		cout << "|";
		color(15);
		len = b.length();
		if ((27 - len) % 2 == 0)
		{
			int k = (27 - len) / 2;
			Cout(k - 1, ' ');
			cout << b;
			Cout(27 - k - len, ' ');
		}
		else {
			int k = (27 - len - 1) / 2;
			Cout(k, ' ');
			cout << b;
			Cout(27 - k - len - 1, ' ');
		}
		color(9);
		cout << "|";
		color(13);
		len = f.length();
		if ((38 - len) % 2 == 0)
		{
			int k = (38 - len) / 2;
			Cout(k - 1, ' ');
			cout << f;
			Cout(38 - k - len, ' ');
		}
		else {
			int k = (38 - len - 1) / 2;
			Cout(k, ' ');
			cout << f;
			Cout(38 - k - len - 1, ' ');
		}
		color(9);
		cout << "|";
		color(8);
		string Temp = Tunnel[i].internal_port + " -> " + Tunnel[i].external_port + " [" + Tunnel[i].Protocol + "]", Temp2 = Tunnel[i].internal_port + " -> " + Tunnel[i].external_port + " [";
		len = Temp.length();
		if ((27 - len) % 2 == 0)
		{
			int k = (27 - len) / 2;
			Cout(k - 1, ' ');
			cout << Temp2;
			if (Tunnel[i].Protocol == "tcp")
			{
				color(2);
				cout << "Tcp";
			}
			else if (Tunnel[i].Protocol == "udp")
			{
				color(6);
				cout << "Udp";
			}
			else if (Tunnel[i].Protocol == "http" || Tunnel[i].Protocol == "https")
			{
				color(5);
				if (Tunnel[i].Protocol == "http")
				{
					cout << "Http";
				}
				else {
					cout << "Https";
				}
			}
			else {
				color(9);
				if (Tunnel[i].Protocol == "xtcp")
				{
					cout << "Xtcp";
				}
				else {
					cout << "Stcp";
				}
			}
			color(8);
			cout << "]";
			Cout(27 - k - len, ' ');
		}
		else {
			int k = (28 - len - 1) / 2;
			Cout(k - 1, ' ');
			cout << Temp2;
			if (Tunnel[i].Protocol == "tcp")
			{
				color(2);
				cout << "Tcp";
			}
			else if (Tunnel[i].Protocol == "udp")
			{
				color(6);
				cout << "Udp";
			}
			else if (Tunnel[i].Protocol == "http" || Tunnel[i].Protocol == "https")
			{
				color(5);
				if (Tunnel[i].Protocol == "http")
				{
					cout << "Http";
				}
				else {
					cout << "Https";
				}
			}
			else {
				color(9);
				if (Tunnel[i].Protocol == "xtcp")
				{
					cout << "Xtcp";
				}
				else {
					cout << "Stcp";
				}
			}
			color(8);
			cout << "]";
			Cout(28 - k - len - 1, ' ');
		}
		color(9);
		cout << "|";
		len = 2;
		if ((9 - len) % 2 == 0)
		{
			int k = (9 - len) / 2;
			Cout(k - 1, ' ');
			if (Tunnel[i].a)
			{
				color(10);
				cout << "是";
			}
			else {
				color(8);
				cout << "否";
			}
			Cout(9 - k - len, ' ');
		}
		else {
			int k = (9 - len - 1) / 2;
			Cout(k, ' ');
			if (Tunnel[i].a)
			{
				color(10);
				cout << "是";
			}
			else {
				color(8);
				cout << "否";
			}
			Cout(9 - k - len - 1, ' ');
		}
		color(9);
		cout << "|";
		len = 2;
		if ((9 - len) % 2 == 0)
		{
			int k = (9 - len) / 2;
			Cout(k - 1, ' ');
			if (Tunnel[i].b)
			{
				color(10);
				cout << "是";
			}
			else {
				color(8);
				cout << "否";
			}
			Cout(9 - k - len, ' ');
		}
		else {
			int k = (9 - len - 1) / 2;
			Cout(k, ' ');
			if (Tunnel[i].b)
			{
				color(10);
				cout << "是";
			}
			else {
				color(8);
				cout << "否";
			}
			Cout(9 - k - len - 1, ' ');
		}
		color(9);
		cout << "|\n";
	}
	if (must_online)
	{
		Cout(119, '-');
		cout << "|";
		Cout(42, ' ');
		color(4);
		cout << "列表仅包含已在本地启动的隧道！";
		color(9);
		Cout(45, ' ');
		cout << "|\n";
	}
	else if (must_offline)
	{
		Cout(119, '-');
		cout << "|";
		Cout(41, ' ');
		color(4);
		cout << "列表仅包含尚未在本地启动的隧道！";
		color(9);
		Cout(44, ' ');
		cout << "|\n";
	}
	Cout(119, '-');
	cout << endl;
	return;
}
inline void GetNode() // 获取节点列表
{
	if (_access("Temp", 0) == -1) // 判断日志文件夹是否存在
	{
		_mkdir("Temp");
	}
	fout.open(".\\Temp\\ServerList.sys");
	fout << Get("https://api.locyanfrp.cn/LoCyanFrpMSApp/GetServerList").GetText();
	fout.close();
	fin.open(".\\Temp\\ServerList.sys");
	fin >> Node_quantity; // 输入隧道数量
	//cout << Node_quantity << "\n";
	for (int i = 0; i < Node_quantity; i++)
	{
		fin >> Node[i].ID; // 输入隧道 ID
		//cout << Node[i].ID << " ";
		fin >> Node[i].Name >> Node[i].IP >> Node[i].RealIP; // 输入隧道名、地址、IP
		fin >> Node[i].use; // 输入隧道是否可用
		Node[i].Name = UTF8ToANSI(Node[i].Name), Node[i].IP = UTF8ToANSI(Node[i].IP), Node[i].RealIP = UTF8ToANSI(Node[i].RealIP);
		//cout << Node[i].Name << " " << Node[i].IP << "\n";
	}
	for (int i = 0; i < Node_quantity; i++)
	{
		if (!Node[i].use)
		{
			UnusefulNodeLen++;
		}
	}
	//cout << Node_quantity << " " << UnusefulNodeLen;
	//system("pause");
	fin.close();
	//system("pause");
	//system("del /f /s /q .\\Temp\\ServerList.sys>nul");
	return;
}
inline void GetTunnel() // 获取隧道列表
{
	if (_access("Temp", 0) == -1) // 判断日志文件夹是否存在
	{
		_mkdir("Temp");
	}
	fout.open(".\\Temp\\MyServerList.dll");
	fout << Get("https://api.locyanfrp.cn/LoCyanFrpMSApp/GetTunnelList?username=" + name).GetText();
	fout.close();
	fin.open(".\\Temp\\MyServerList.dll");
	fin >> MyTunnelLen; // 用户的隧道数量
	for (int i = 0; i < MyTunnelLen; i++)
	{
		int temp;
		fin >> Tunnel[i].ID;
		fin >> Tunnel[i].Name;
		fin >> Tunnel[i].internal_port >> Tunnel[i].external_port;
		fin >> Tunnel[i].Protocol;
		fin >> Tunnel[i].Node >> Tunnel[i].a >> Tunnel[i].b >> temp;
		if (temp)
		{
			fin >> Tunnel[i].url;
			Tunnel[i].url = UTF8ToANSI(Tunnel[i].url);
		}
		fin >> Tunnel[i].IP;
		Tunnel[i].Name = UTF8ToANSI(Tunnel[i].Name), Tunnel[i].Protocol = UTF8ToANSI(Tunnel[i].Protocol);
		//cout << Tunnel[i].ID << " " << Tunnel[i].Name << " " << Tunnel[i].internal_port << " " << Tunnel[i].external_port << " " << Tunnel[i].Protocol << " " << Tunnel[i].Node << " " << Tunnel[i].a << " " << Tunnel[i].b << " " << temp << " " ;
		//if (temp)
		//{
		//	cout << Tunnel[i].url << " " ;
		//}
		//cout << Tunnel[i].IP "\n";
	}
	fin.close();
	//system("pause");
	//system("del /f /s /q .\\Temp\\MyServerList.dll>nul");
	return;
}
inline void Delete() // 删除隧道模块
{
	system("cls");
	if (MyTunnelLen + 10 > 30)
	{
		system(("mode con cols=119 lines=" + to_string(MyTunnelLen + 10)).c_str()); // 设置窗口长宽高
	}
	if (MyTunnelLen == 0)
	{
		print("您没有任何一个隧道！", 48, 49);
		Sleep(2000);
		system("cls");
		return;
	}
	PrintTunnelList(false, false);
	int TunnelID;
	string token, Return;
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(42, ' ');
	color(4);
	cout << "隧道 ID 填好后请按 “回车” 键";
	color(9);
	Cout(45, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n";
	Cout(119, '-');
	HANDLE hout;
	COORD coord = { 47 , MyTunnelLen + 5 };
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, coord); // 在第 35 行，第 MyTunnelLen + 6 列插入文字
	cout << "要删除的隧道 ID：";
	cin >> TunnelID;
	system("cls");
	if (!ifTunnel(TunnelID))
	{
		print("您不拥有该隧道！", 50, 51);
	}
	print("正在删除隧道", 52, 53);
	token = Json(Get("https://api.locyanfrp.cn/User/DoLogin?username=" + name + "&password=" + password).GetText(), "token"); // 获取用于登录的 Token
	Return = Json(Get("https://api.locyanfrp.cn/Proxies/Remove?proxyid=" + to_string(TunnelID) + "&username=" + name + "&token=" + token).GetText(), "status"); // 尝试删除指定隧道，并获取返回值
	system("mode con cols=119 lines=30"); // 设置窗口长宽高 
	print("正在删除隧道", 52, 53);
	if (Return == "true")
	{
		GetTunnel();
		print("删除成功！", 53, 54);
	}
	else if (Return == "-1")
	{
		print("您不拥有该隧道！", 50, 51);
	}
	else
	{
		print("未知错误！", 53, 54);
	}
	Sleep(2000);
	system("cls");
	return;
}
inline void Create() // 创建隧道模块
{
	string Tunnel_name, InlineIP, Protocol, InlinePort, OnlinePort, HTTPSdoname, token, Compress = "0", Encrypt = "0", TryGet, Return;
	int ServerID;
	system("cls");
	if (Node_quantity - UnusefulNodeLen + 17 > 30)
	{
		system(("mode con cols=119 lines=" + to_string(Node_quantity - UnusefulNodeLen + 17)).c_str()); // 设置窗口长宽高
	}
	PrintNodeList();
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(42, ' ');
	color(4);
	cout << "每项内容填好后请按 “回车” 键";
	color(9);
	Cout(45, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n";
	Cout(119, '-');
	HANDLE hout1;
	COORD coord1 = { 50 , Node_quantity - UnusefulNodeLen + 4 };
	hout1 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout1, coord1);
	cout << "隧道名：";
	cin >> Tunnel_name;
	HANDLE hout2;
	COORD coord2 = { 50 , Node_quantity - UnusefulNodeLen + 5 };
	hout2 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout2, coord2);
	cout << "内网 IP：";
	cin >> InlineIP;
	HANDLE hout3;
	COORD coord3 = { 50 , Node_quantity - UnusefulNodeLen + 6 };
	hout3 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout3, coord3);
	cout << "隧道类型：";
	cin >> Protocol;
	Protocol = LTS(Protocol);
	HANDLE hout4;
	COORD coord4 = { 50 , Node_quantity - UnusefulNodeLen + 7 };
	hout4 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout4, coord4);
	cout << "内网端口：";
	cin >> InlinePort;
	HANDLE hout5;
	COORD coord5 = { 50 , Node_quantity - UnusefulNodeLen + 8 };
	hout5 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout5, coord5);
	cout << "外网端口：";
	cin >> OnlinePort;
	string TF;
	HANDLE hout6;
	COORD coord6 = { 50 , Node_quantity - UnusefulNodeLen + 9 };
	hout6 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout6, coord6);
	cout << "是否压缩：";
	cin >> TF;
	if (TF == "是")
	{
		Compress = "1";
	}
	HANDLE hout7;
	COORD coord7 = { 50 , Node_quantity - UnusefulNodeLen + 10 };
	hout7 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout7, coord7);
	cout << "是否加密：";
	cin >> TF;
	if (TF == "是")
	{
		Encrypt = "1";
	}
	HANDLE hout8;
	COORD coord8 = { 50 , Node_quantity - UnusefulNodeLen + 11 };
	hout8 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout8, coord8);
	cout << "节点 ID：";
	cin >> ServerID;
	string TestProtocol = Protocol;
	if (Protocol == "tcp")
	{
		Protocol = "1";
	}
	else if (Protocol == "udp")
	{
		Protocol = "2";
	}
	else if (Protocol == "http")
	{
		Protocol = "3";
	}
	else if (Protocol == "https")
	{
		Protocol = "4";
	}
	else if (Protocol == "xtcp")
	{
		Protocol = "5";
	}
	else if (Protocol == "stcp")
	{
		Protocol = "6";
	}
	TryGet = "https://api.locyanfrp.cn/Proxies/add?username=" + name + "&name=" + Tunnel_name + "&key=" + UserInternalID + "&ip=" + InlineIP + "&type=" + Protocol + "&lp=" + InlinePort + "&rp=" + OnlinePort + "&ue=" + Encrypt + "&uz=" + Compress + "&id=" + to_string(ServerID);
	if (Protocol == "3" || Protocol == "4")
	{
		HANDLE hout9;
		COORD coord9 = { 50 , Node_quantity - UnusefulNodeLen + 12 };
		hout9 = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hout9, coord9);
		cout << "绑定域名：";
		cin >> HTTPSdoname;
		TryGet = TryGet + "&url=" + HTTPSdoname;
	}
	system("cls");
	print("正在创建隧道", 52, 53);
	system("mode con cols=119 lines=30"); // 设置窗口长宽高
	if (ifNode(ServerID))
	{
		print("节点 ID 不存在！", 50, 51);
	}
	print("正在创建隧道", 52, 53);
	TryGet = TryGet + "&token=" + Json(Get("https://api.locyanfrp.cn/User/DoLogin?username=" + name + "&password=" + password).GetText(), "token");
	Return = Json(Get(TryGet).GetText(), "status");
	system("cls");
	if (Return == "true")
	{
		print("正在创建隧道", 52, 53);
		GetTunnel();
		print("创建成功！", 53, 54);
	}
	else if (Return == "-1")
	{
		print("隧道类型错误！", 51, 52);
	}
	else if (Return == "-2")
	{
		print("信息不完整！", 52, 53);
	}
	else if (Return == "-3")
	{
		print("远程端口被占用！", 50, 51);
	}
	else if (Return == "-4")
	{
		print("远程端口属于保留范围！", 47, 48);
	}
	else if (Return == "-6")
	{
		print("隧道名被占用！", 51, 52);
	}
	else
	{
		print("未知错误！", 53, 54);
	}
	Sleep(2000);
	system("cls");
	return;
}
inline void Start() // 启动隧道模块
{
	system("cls");
	int id;
	PrintTunnelList(false, true);
	system("cls");
	if (TestLen == 0)
	{
		print("您没有任何一个离线隧道！", 46, 47);
		Sleep(2000);
		system("cls");
		return;
	}
	if (TestLen + 13 > 30)
	{
		system(("mode con cols=119 lines=" + to_string(TestLen + 13)).c_str()); // 设置窗口长宽高
	}
	PrintTunnelList(false, true);
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(42, ' ');
	color(4);
	cout << "隧道 ID 填好后请按 “回车” 键";
	color(9);
	Cout(45, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n";
	Cout(119, '-');
	HANDLE hout;
	COORD coord = { 47 , TestLen + 7 };
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, coord); // 在第 35 行，第 MyTunnelLen + 6 列插入文字
	cout << "要启动的隧道 ID：";
	cin >> id;
	if (!ifTunnel(id))
	{
		print("您不拥有该隧道！", 50, 51);
		Sleep(2000);
		system("cls");
		return;
	}
	print("正在启动中...", 52, 52);
	if (ifOnline(id)) // 在线
	{
		print("该隧道已在异地启动！", 48, 49);
		Sleep(2000);
		system("cls");
		return;
	}
	fout.open("Frp-Name.log");
	fout << id << " " << UserInternalID;
	fout.close();
	system("start FrpAuxiliaryApp.exe");
	int time = 0;
	while (true)
	{
		if (ifOnline(id))
		{
			Tunnel[FromIDFindTTunnelLen(id)].online = true, Online[OnlineLen++] = id;
			fout.open("OnlineTunnel.list"); // 打开文件
			fout << OnlineLen << '\n'; // 获取是否登录
			for (int i = 0; i < OnlineLen; i++)
			{
				fout << Online[i] << " ";
			}
			fout.close();
			print("启动成功！访问地址已复制到剪贴板，按任意键继续", 35, 36);
			string FrpUrl = Node[FromIDFindNodeLen(Tunnel[FromIDFindTTunnelLen(id)].Node)].IP + ":" + Tunnel[FromIDFindTTunnelLen(id)].external_port;
			OpenClipboard(NULL), EmptyClipboard();
			HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, (FrpUrl.length() + 1) * sizeof(char));
			char* pClipboardData = static_cast<char*>(GlobalLock(hClipboardData));
			strcpy_s(pClipboardData, FrpUrl.length() + 1, FrpUrl.c_str());
			GlobalUnlock(hClipboardData), SetClipboardData(CF_TEXT, hClipboardData);  // 复制内容到剪贴板
			CloseClipboard();
			_getch();
			system("cls");
			return;
		}
		Sleep(1000);
		time += 3;
		if (time > 60)
		{
			system("cls");
			print("启动失败！请检查网络或查看日志！", 42, 43);
			system(("taskkill /f /im " + to_string(id) + ".exe").c_str());
			system("cls");
			print("启动失败！请检查网络或查看日志！", 42, 43);
			system(("notepad ./Logs/FrpAuxiliaryApp-" + to_string(id) + ".log").c_str());
			system(("notepad ./Logs/" + to_string(id) + ".log").c_str());
			system("cls");
			return;
		}
	}
	system("cls");
	return;
}
inline void End() // 关闭映射模块
{
	system("cls");
	int id;
	PrintTunnelList(true, false);
	system("cls");
	if (TestLen == 0)
	{
		print("您没有启动任何一个隧道！", 46, 47);
		Sleep(2000);
		system("cls");
		return;
	}
	if (TestLen + 13 > 30)
	{
		system(("mode con cols=119 lines=" + to_string(TestLen + 13)).c_str()); // 设置窗口长宽高
	}
	PrintTunnelList(true, false);
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(42, ' ');
	color(4);
	cout << "隧道 ID 填好后请按 “回车” 键";
	color(9);
	Cout(45, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n";
	Cout(119, '-');
	HANDLE hout;
	COORD coord = { 47 , TestLen + 7 };
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, coord); // 在第 35 行，第 TestLen + 6 列插入文字
	cout << "要关闭的隧道 ID：";
	cin >> id;
	if (!ifTunnel(id))
	{
		print("您不拥有该隧道！", 50, 51);
		Sleep(2000);
		system("cls");
		return;
	}
	print("正在关闭中...", 52, 52);
	if (!ifOnline(id)) // 离线
	{
		print("该隧道已离线！", 51, 52);
		Tunnel[FromIDFindTTunnelLen(id)].online = false;
		int j = 0;
		for (int i = 0; i < OnlineLen; i++) {
			if (Online[i] != id)
			{
				Online[j++] = Online[i];
			}
		}
		OnlineLen--;
		fout.open("OnlineTunnel.list"); // 打开文件
		fout << OnlineLen << '\n'; // 获取是否登录
		for (int i = 0; i < OnlineLen; i++)
		{
			fout << Online[i] << " ";
		}
		fin.close();
		Sleep(2000);
		system("cls");
		return;
	}
	system(("taskkill /f /im " + to_string(id) + ".exe>nul").c_str());
	print("关闭成功！", 53, 54);
	Tunnel[FromIDFindTTunnelLen(id)].online = false;
	int j = 0;
	for (int i = 0; i < OnlineLen; i++) {
		if (Online[i] != id)
		{
			Online[j++] = Online[i];
		}
	}
	OnlineLen--;
	fout.open("OnlineTunnel.list"); // 打开文件
	fout << OnlineLen << '\n'; // 获取是否登录
	for (int i = 0; i < OnlineLen; i++)
	{
		fout << Online[i] << " ";
	}
	fin.close();
	Sleep(2000);
	system("cls");
	return;
}
inline void color(short attr) // 输出彩色文本，颜色值查询：Cmd -> color ?c
{
	if (attr >= 0 && attr <= 15)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	return;
}
inline void Cout(int x, char a) // 连续输出 x 次 a
{
	for (int i = 0; i < x; i++)
	{
		cout << a;
	}
	return;
}
inline void print(string s, int a, int b) // 输出提示语-自行调整格式-中文占两个字符！
{
	system("cls");
	color(9);
	Cout(119, '-');
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(a, ' ');
	color(4);
	cout << s;
	color(9);
	Cout(b, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n";
	Cout(119, '-');
}
inline void no() // 暂时不用临时代码存放位
{
	return;
}

inline bool file(string filename) // 判断 filename 存不存在
{
	_file.open(filename, ios::in);
	if (!_file)
	{
		_file.close();
		return false;
	}
	_file.close();
	return true;
}
inline bool ifTunnel(int id) // 判断隧道是否存在 True / False
{
	for (int i = 0; i < MyTunnelLen; i++)
	{
		if (Tunnel[i].ID == id)
		{
			return true;
		}
	}
	return false;
}
inline bool ifNode(int id) // 判断节点是否存在 True / False
{
	for (int i = 0; i < Node_quantity; i++)
	{
		if (Node[i].ID == id)
		{
			return true;
		}
	}
	return false;
}
inline bool ifOnline(int id) // 获取隧道在线状态
{
	string Name = FromIDFindTunnelName(id), Protocol = FromIDFindProtocol(id), NodeID = to_string(FromIDFindNodeID(id)), Return = Json(Get("https://api.locyanfrp.cn/Proxies/GetOnlineStatus?type=" + Protocol + "&proxyname=" + Name + "&user=" + UserInternalID + "&node=" + NodeID).GetText(), "status"), Return2; // 获取 API 要用的信息
	if (Return == "true") // 启动过
	{
		Return = Json(Get("https://api.locyanfrp.cn/Proxies/GetOnlineStatus?type=" + Protocol + "&proxyname=" + Name + "&user=" + UserInternalID + "&node=" + NodeID).GetText(), "online"); // 获取是否在线
		if (Return == "online")
		{
			return true;
		}
	}
	return false;
}
inline int FromIDFindNodeID(int id) // 根据隧道 ID 找节点 ID
{
	for (int i = 0; i < MyTunnelLen; i++)
	{
		if (Tunnel[i].ID == id)
		{
			return Tunnel[i].Node;
		}
	}
}
inline int numlen(int num) // 获取一个数字的长度（位数）
{
	int len = 0;
	for (; num > 0; len++)
	{
		num /= 10;
	}
	return len;
}
inline int FromIDFindTTunnelLen(int id) // 根据隧道 ID 找节点在数组中的下标
{
	for (int i = 0; i < MyTunnelLen; i++)
	{
		if (Tunnel[i].ID == id)
		{
			return i;
		}
	}
}
inline int FromIDFindNodeLen(int id) // 根据节点 ID 找在数组中的下标
{
	for (int i = 0; i < Node_quantity; i++)
	{
		if (Node[i].ID == id)
		{
			return i;
		}
	}
}
inline string DEC(string url, char c) // 删除超链接中转义符
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
inline string Json(string json, string project) // 获取 json 的 project 项的值
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
inline string FromIDFindNodeName(int id) // 根据节点 ID 找名字
{
	for (int i = 0; i < Node_quantity; i++)
	{
		if (Node[i].ID == id)
		{
			return Node[i].Name;
		}
	}
	return "节点获取失败！";
}
inline string FromIDFindNodeUrl(int id) // 根据节点 ID 找地址
{
	for (int i = 0; i < Node_quantity; i++)
	{
		if (Node[i].ID == id)
		{
			return Node[i].IP;
		}
	}
}
inline string FromIDFindTunnelName(int id) // 根据隧道 ID 找名字
{
	for (int i = 0; i < MyTunnelLen; i++)
	{
		if (Tunnel[i].ID == id)
		{
			return Tunnel[i].Name;
		}
	}
}
inline string FromIDFindProtocol(int id) // 根据隧道 ID 找隧道协议
{
	for (int i = 0; i < MyTunnelLen; i++)
	{
		if (Tunnel[i].ID == id)
		{
			return Tunnel[i].Protocol;
		}
	}
}
inline string LTS(string s) // 大写 -> 小写
{
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
		{
			s[i] += 32;
		}
	}
	return s;
}
inline string UTF8ToANSI(string s)  // 将 UTF-8 编码转换为 ANSI
{
	BSTR    bstrWide;
	char* pszAnsi;
	int     nLength;
	const char* pszCode = s.c_str();
	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, NULL, NULL);
	bstrWide = SysAllocStringLen(NULL, nLength);
	MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide, nLength);
	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	pszAnsi = new char[nLength];
	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString(bstrWide);
	string r(pszAnsi);
	delete[] pszAnsi;
	return r;
}
inline string ANSItoUTF8(string strAnsi) // 将 ANSI 编码转换为 UTF-8
{
	UINT nLen = MultiByteToWideChar(936, NULL, strAnsi.c_str(), -1, NULL, NULL);
	WCHAR* wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(936, NULL, strAnsi.c_str(), -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR* szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	strAnsi = szBuffer;
	delete[]wszBuffer;
	delete[]szBuffer;
	return strAnsi;
}
inline LPCWSTR stringToLPCWSTR(string orig) // 将 LPCWSTR 形式的字符串转化为 String 形式的字符串
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

/* 注释：
* 1.system("cls")：清屏
* 2.freopen：别用
* 3.输出 / 输入文件：ANSI
* 4.更改输出颜色，输出后记得改回来
*/
