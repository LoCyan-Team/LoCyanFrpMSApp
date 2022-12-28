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
using namespace std; // cin / cout 定义模块
ofstream fout;
ifstream fin;

void PrintGUI(); // 显示 GUI
void color(short attr); // 输出彩色文字
void Cout(int x, char a); // 连续输出 x 次 a

string UserInternalID; // 用户密钥
bool ifLogin = false; // 是否登录-t/f-默认f

int main() // 主函数模块
{
	SetConsoleTitle(L"LoCyan Frp 智能控制面板"); // 设置标题
	HWND hWnd = GetConsoleWindow();
	RECT rc;
	GetWindowRect(hWnd, &rc);
	SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
	SetWindowPos(hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, NULL); // 设置窗口长宽高
	system("mode con cols=119 lines=35"); // 设置窗口长宽高

	PrintGUI(); // 打印 GUI
	while (true)
	{
		if (_kbhit()) // 判断是否有按按键
		{
			char a = _getch(); // 判断按了啥键     
			switch (a)
			{
				case '1': 
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

/* 注释：
* 1.system("cls")：清屏
* 2.freopen：别用
* 3.输出 / 输入文件：ANSI
* 4.更改输出颜色，输出后记得改回来
*/