#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <string>

#define UCHAR unsigned char

class ProcessHacker
{
public:
	ProcessHacker(const wchar_t * processName);
	// 是否成功取得进程句柄
	bool isGood();
	// 将代码复制到远程进程，自动申请内存
	void * writeBuffer(UCHAR * buff, int size);
	// 将代码复制到远程进程
	bool writeBuffer(UCHAR * buff, int size, void * remoteAddress);
	// 执行远程代码
	void attachThreadRun(void * remoteAddress);
	// 将本地代码复制到远程并执行
	void * runShellCode(UCHAR * code, int size);
	~ProcessHacker();
private:
	bool getProcess(const wchar_t * processName);
	DWORD getFirstThread();
	int processid;
	HANDLE pHandle;
};

