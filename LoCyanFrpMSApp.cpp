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
#include <windows.h> // cmd
#include <mysql.h> // Mysql
#include <cstdio> // scanf / printf
#include <conio.h> // 识别按键模块
#include "requests.h" // 爬取网页模块
#include "utils.h" // requests.h 的前置

using namespace std; // cin / cout 定义模块
using namespace requests; // requests 定义模块
ofstream fout;
ifstream fin;

void login(); // 登录模块
void PrintGUI(); // 显示 GUI
void color(short attr); // 输出彩色文字
void Cout(int x, char a); // 连续输出 x 次 a
void print(string s, int a, int b); // 输出提示语
string Json(string json, string project); // 获取 Json 某个项的值

string UserInternalID; // 用户密钥
bool ifLogin = false; // 是否登录-t/f-默认f

int main() // 主函数模块
{
	SetConsoleTitle(L"LoCyan Frp 智能控制面板"); // 设置标题（
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
		fin >> UserInternalID; // 如已登录，获取 Frp 的 Token
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
				case '2':
				case '3':
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
		string name, password; // 定义：name-用户名 | password-密码 
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
			fout << 1 << " " << UserInternalID;
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
void no() // 暂时不用临时代码存放位
{
	//----------MYSQL----------
	MYSQL mysql;
	mysql_init(&mysql);
	const char* host = ""; // MySQL IP
	const char* user = ""; // MySQL 用户名
	const char* pass = ""; // MySQL 密码
	const char* db = ""; // MySQL 数据库名
	if (!mysql_real_connect(&mysql, host, user, pass, db, 3307, 0, 0)) // 链接并判断返回值
	{
		cout << "数据库连接失败！\n" << mysql_error(&mysql) << endl;
	}
	else {
		cout << "成功" << endl;
	}
	string code = "", sql = "SELECT * FROM tokens WHERE `token` = '" + code + "';"; // SQL 语句
	mysql_query(&mysql, sql.c_str()); // 执行 SQL 命令
	MYSQL_RES* result = mysql_store_result(&mysql);
	MYSQL_ROW row = mysql_fetch_row(result); // 获取返回值
	cout << row[1];
	mysql_free_result(result); // 释放空间
	return;
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


/* 注释：
* 1.system("cls")：清屏
* 2.freopen：别用
* 3.输出 / 输入文件：ANSI
* 4.更改输出颜色，输出后记得改回来
*/
