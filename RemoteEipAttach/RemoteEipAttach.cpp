// RemoteEipAttach.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ProcessHacker.h"

using namespace std;

char * GetShellCode(const wchar_t * fileName, size_t &outSize);

int main()
{
	wchar_t dllName[100];
	wchar_t processName[100];

	wcout << "请输入DLL路径：";
	wcin >> dllName;
	wcout << "请输入进程名称：";
	wcin >> processName;
	size_t size = 0;
	auto code = GetShellCode(dllName, size);
	if (size) {
		ProcessHacker ph(processName);
		if (ph.isGood()) {
			ph.runShellCode((UCHAR *)code, size);
			delete[] code;
			wcout << "已成功注入进程, 回车退出";
		}
		else {
			wcout << "打开进程失败";
		}
	}
	else {
		wcout << "DLL 文件损坏";
	}

	getchar();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
