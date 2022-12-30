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
#include "requests.h" // 爬取网页模块
#include "utils.h" // requests.h 的前置
#pragma comment(lib, "Urlmon.lib")

using namespace std; // cin / cout 定义模块
using namespace requests; // requests 定义模块
ofstream fout;
ifstream fin;
fstream _file;

void login(); // 登录模块
void PrintGUI(); // 显示 GUI
void PrintGUI2(bool ifGetNode); // 显示隧道管理选择模块
void PrintNodeList(); // 打印节点列表
void PrintTunnelList(); // 打印隧道列表
void GetNode(); // 获取节点列表
void GetTunnel(); // 获取隧道列表
void Delete(); // 删除隧道模块
void Create(); // 创建隧道模块
void color(short attr); // 输出彩色文字
void Cout(int x, char a); // 连续输出 x 次 a
void print(string s, int a, int b); // 输出提示语
void Unlogin(); // 登出模块
bool file(string filename); // 判断文件是否存在
int numlen(int num); // 获取一个数字的长度（位数）
string Json(string json, string project); // 获取 Json 某个项的值
string FromIDFindNodeName(int id); // 根据节点 ID 找名字
string LTS(string s); // 大写转小写
LPCWSTR stringToLPCWSTR(string orig); // LPCWSTR -> String

string UserInternalID, name, password; // 定义：name-用户名 | password-密码 | UserInternalID-FrpToken
bool ifLogin = false/* 是否已登录 */,ifCore = false /* 有没有下载辅助模块 */ ;
int Node_quantity/* 节点数量 */, MyTunnelLen/* 用户的隧道数量 */;

struct Nodeinfo // 节点信息
{
	int ID;
	string Name, IP;
}Node[100010];
struct Tunnelinfo
{
	int ID, Node, a, b;
	string Name, Protocol, IP, url, internal_port, external_port;
}Tunnel[100010];

int main() // 主函数模块
{
	SetConsoleTitle(L"LoCyanFrpMSApp"); // 设置标题
	HWND hWnd = GetConsoleWindow();
	RECT rc;
	GetWindowRect(hWnd, &rc);
	SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
	SetWindowPos(hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, NULL); // 设置窗口长宽高
	system("mode con cols=119 lines=35"); // 设置窗口长宽高

	fin.open("AppLogin.dll"); // 打开文件
	fin >> ifLogin ; // 获取是否登录
	if (ifLogin)
	{
		fin >> UserInternalID >> name >> password; // 如已登录，获取 Frp 的 Token
	}
	fin.close();

	PrintGUI(); // 打印 GUI
	while (true)
	{
		if (_kbhit()) // 判断是否有按按键
		{
			char a = _getch(); // 判断按了啥键     
			switch (a)
			{
				case '1':login(); break;
				case '2':Unlogin(); break;
				case '3':PrintGUI2(false); break;
				case '4':
				case '5':return 0; break;
			}
		}
		Sleep(20); // 防止卡 / 占用过多资源
	}

	system("pause"); // 防止退出-Debug

	return 0;
}

void login() // 重磅上场+++ 登录模块
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
		PrintGUI(); // 输出 GUI
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
		coord = { 47 , 5 };
		hout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hout, coord); // 在第 47 行，第 5 列插入文字
		cout << "密码：";
		cin >> password; // 输入密码-以后要改成输入时隐藏-待改进
		print("正在校验密码", 52, 53);
		string Return = Json(Get("https://api.locyanfrp.cn/User/DoLogin?username=" + name + "&password=" + password).GetText() , "status"); // 查看密码是否正确
		if (Return == "-1")
		{
			print("用户名或密码错误！", 49, 50);
			Sleep(3000);
			continue;
		}else{
			string login_token = Json(Get("https://api.locyanfrp.cn/User/DoLogin?username=" + name + "&password=" + password).GetText(), "token"); // 获取用于登录的 Token
			UserInternalID = Json(Get("https://api.locyanfrp.cn/Account/info?username=" + name + "&token=" + login_token).GetText(), "token"); // 获取用于 Frp 的 Token
			fout.open("AppLogin.dll");
			fout << 1 << " " << UserInternalID << " " << name << " " << password ;
			fout.close();
			ifLogin = true;
			print("登录成功！", 53, 54);
			Sleep(2000);
			system("cls");
			PrintGUI();
			return;
		}
	}
	return;
}
void PrintGUI() // 显示 GUI
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
void PrintGUI2(bool ifGetNode) // 隧道管理选择模块
{
	if (!ifLogin)
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
		PrintGUI();
		return;
	}
	system("cls");
	if (!file("Core.exe") && !ifCore)
	{
		system("cls");
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
		URLDownloadToFile(nullptr, L"https://download.locyan.cn/d/Core.exe", L"DownLoadTEMP", 0, nullptr);
		system("rename DownLoadTEMP Core.exe");
		system("cls");
	}
	ifCore = true;
	if (!ifGetNode)
	{
		GetNode();
	}
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
	cout << "启动映射（按 O）";
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
	while (true)
	{
		if (_kbhit())
		{
			char a = _getch();
			switch (a)
			{
				case 'D': Delete(); break;
				case 'C': Create(); break;
				case 'O': break;
				case 'R': system("cls"); PrintGUI(); return; break;
				case 'd': Delete(); break;
				case 'c': Create(); break;
				case 'o': break;
				case 'r': system("cls"); PrintGUI(); return; break;
			}
		}
		Sleep(20);
	}
	return;
}
void PrintNodeList() // 打印节点列表
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
void PrintTunnelList() // 打印隧道列表
{
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
				len++;
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
					len++;
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
				len++;
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
	Cout(119, '-');
	cout << endl;
	return;
}
void GetNode() // 获取节点列表
{
	print("正在获取节点列表", 50, 51);
	fout.open("SQLTransmission.sys");
	fout << 1; // 对暗号。。。
	fout.close();
	system("start Core.exe");
	while (true) // 智能等待
	{
		if (file("ServerList.sys"))
		{
			break;
		}
		Sleep(20);
	}
	Sleep(5000);
	fin.open("ServerList.sys");
	fin >> Node_quantity; // 输入隧道数量
	for (int i = 0; i < Node_quantity; i++)
	{
		fin >> Node[i].ID; // 输入隧道 ID
		fin >> Node[i].Name >> Node[i].IP; // 输入隧道名、IP
	}
	fin.close();
	system("cls");
	Cout(119, '-');
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(52, ' ');
	color(4);
	cout << "正在删除缓存";
	color(9);
	Cout(53, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n";
	Cout(119, '-');
	COORD coord = { 45 , 3 };
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, coord);
	system("del /f /s /q SQLTransmission.sys"); // 删除与 Python 软件的发送文件
	coord = { 45 , 4 };
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, coord);
	system("del /f /s /q ServerList.sys"); // 删除与 Python 软件的接收文件
	system("cls");
	return;
}
void GetTunnel() // 获取隧道列表
{
	print("正在获取隧道列表", 50, 51);
	fout.open("SQLTransmission.sys");
	fout << 2 << endl << UserInternalID; // 对暗号。。。
	fout.close();
	system("start Core.exe");
	while (true) // 智能等待
	{
		if (file("MyServerList.dll"))
		{
			break;
		}
		Sleep(20);
	}
	Sleep(5000);
	fin.open("MyServerList.dll");
	fin >> MyTunnelLen; // 用户的隧道数量
	for (int i = 0; i < MyTunnelLen; i++)
	{
		int temp;
		fin >> Tunnel[i].ID;
		fin >> Tunnel[i].Name >> Tunnel[i].internal_port >> Tunnel[i].external_port >> Tunnel[i].Protocol;
		fin >> Tunnel[i].Node >> Tunnel[i].a >> Tunnel[i].b >> temp;
		if (temp)
		{
			fin >> Tunnel[i].url;
		}
		fin >> Tunnel[i].IP;

	}
	fin.close();
	system("cls");
	Cout(119, '-');
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(52, ' ');
	color(4);
	cout << "正在删除缓存";
	color(9);
	Cout(53, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n";
	Cout(119, '-');
	COORD coord = { 45 , 3 };
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, coord);
	system("del /f /s /q SQLTransmission.sys");
	coord = { 45 , 4 };
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, coord);
	system("del /f /s /q MyServerList.sys");
	system("cls");
	return;
}
void Delete() // 删除隧道模块
{
	system("cls");
	GetTunnel();
	PrintTunnelList();
	string TunnelID, token, Return;
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
	print("正在删除隧道", 52, 53);
	token = Json(Get("https://api.locyanfrp.cn/User/DoLogin?username=" + name + "&password=" + password).GetText(),"token"); // 获取用于登录的 Token
	Return = Json(Get("https://api.locyanfrp.cn/Proxies/Remove?proxyid=" + TunnelID + "&username=" + name + "&token=" + token).GetText(),"status"); // 尝试删除指定隧道，并获取返回值
	if (Return == "0")
	{
		print("删除成功！", 53, 54);
	}
	else if (Return == "-1")
	{
		print("您不拥有此隧道！", 50, 51);
	}
	else if (Return == "-2")
	{
		print("未知错误！", 53, 54);
	}
	Sleep(2000);
	system("cls");
	PrintGUI2(true);
	return;
}
void Create() // 创建隧道模块
{
	string Tunnel_name, InlineIP, Protocol, InlinePort, OnlinePort, HTTPSdoname, token, Compress = "0", Encrypt = "0", ServerID, TryGet, Return;
	system("cls");
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
	COORD coord1 = { 50 , Node_quantity + 4 };
	hout1 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout1, coord1);
	cout << "隧道名：";
	cin >> Tunnel_name;
	HANDLE hout2;
	COORD coord2 = { 50 , Node_quantity + 5 };
	hout2 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout2, coord2);
	cout << "内网 IP：";
	cin >> InlineIP;
	HANDLE hout3;
	COORD coord3 = { 50 , Node_quantity + 6 };
	hout3 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout3, coord3);
	cout << "隧道类型：";
	cin >> Protocol;
	Protocol = LTS(Protocol);
	HANDLE hout4;
	COORD coord4 = { 50 , Node_quantity + 7 };
	hout4 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout4, coord4);
	cout << "内网端口：";
	cin >> InlinePort;
	HANDLE hout5;
	COORD coord5 = { 50 , Node_quantity + 8 };
	hout5 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout5, coord5);
	cout << "外网端口：";
	cin >> OnlinePort;
	string TF;
	HANDLE hout6;
	COORD coord6 = { 50 , Node_quantity + 9 };
	hout6 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout6, coord6);
	cout << "是否压缩：";
	cin >> TF;
	if (TF == "是")
	{
		Compress = "1";
	}
	HANDLE hout7;
	COORD coord7 = { 50 , Node_quantity + 10 };
	hout7 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout7, coord7);
	cout << "是否加密：";
	cin >> TF;
	if (TF == "是")
	{
		Encrypt = "1";
	}
	HANDLE hout8;
	COORD coord8 = { 50 , Node_quantity + 11 };
	hout8 = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout8, coord8);
	cout << "节点 ID：";
	cin >> ServerID;
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
	system("cls");
	print("正在创建隧道", 52, 53);
	TryGet = "https://api.locyanfrp.cn/Proxies/add?username=" + name + "&name=" + Tunnel_name + "&key=" + UserInternalID + "&ip=" + InlineIP + "&type=" + Protocol + "&lp=" + InlinePort + "&rp=" + OnlinePort + "&ue=" + Encrypt + "&uz=" + Compress + "&id=" + ServerID;
	if (Protocol == "https" || Protocol == "http")
	{
		HANDLE hout9;
		COORD coord9 = { 50 , Node_quantity + 12 };
		hout9 = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hout9, coord9);
		cout << "绑定域名：";
		cin >> HTTPSdoname;
		TryGet = TryGet + "&url=" + HTTPSdoname;
	}
	TryGet = TryGet + "&token=" + Json(Get("https://api.locyanfrp.cn/User/DoLogin?username=" + name + "&password=" + password).GetText(), "token");
	Return = Json(Get(TryGet).GetText(), "status");
	system("cls");
	if (Return == "0")
	{
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
		print("远程端口属于保留范围！",47, 48);
	}
	else if (Return == "-5")
	{
		print("未知错误！", 53, 54);
	}
	else if (Return == "-6")
	{
		print("隧道名被占用！", 51, 52);
	}
	Sleep(2000);
	system("cls");
	PrintGUI2(true);
	return;
}
void color(short attr) // 输出彩色文本，颜色值查询：Cmd -> color ?c
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
void Cout(int x, char a) // 连续输出 x 次 a
{
	for (int i = 0; i < x; i++)
	{
		cout << a;
	}
	return;
}
void print(string s, int a, int b) // 输出提示语-大多情况下会格式错误。。。
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
void Unlogin() // 退出登录模块
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
		PrintGUI();
		return;
	}
	system("cls");
	Cout(119, '-');
	cout << "|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(52, ' ');
	color(4);
	cout << "正在退出登录";
	color(9);
	Cout(53, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n|";
	Cout(117, ' ');
	cout << "|\n";
	Cout(119, '-');
	Sleep(2000);
	HANDLE hout;
	COORD coord = { 45 , 3 };
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, coord);
	system("del /f /s /q AppLogin.dll"); // 把登陆文件删了
	ifLogin = false; // 不要忘记再把变量清了
	system("cls");
	PrintGUI();
	return;
}
void no() // 暂时不用临时代码存放位
{
	return;
}

bool file(string filename) // 判断 filename 存不存在
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

int numlen(int num) // 获取一个数字的长度（位数）
{
	int len = 0;
	for (; num > 0; len++)
	{
		num /= 10;
	}
	return len;
}

string Json(string json , string project) // 获取 json 的 project 项的值
{
	int json_start = json.find(project) + project.size() + 2 , json_finish = 0;
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
string FromIDFindNodeName(int id) // 根据节点 ID 找名字
{
	for (int i = 0; i <= Node_quantity; i++)
	{
		if (Node[i].ID == id)
		{
			return Node[i].Name;
		}
	}
	return "节点获取失败！";
}
string LTS(string s) // 大写 -> 小写
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

LPCWSTR stringToLPCWSTR(string orig) // 将 LPCWSTR 形式的字符串转化为 String 形式的字符串
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