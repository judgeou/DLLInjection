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
	// �Ƿ�ɹ�ȡ�ý��̾��
	bool isGood();
	// �����븴�Ƶ�Զ�̽��̣��Զ������ڴ�
	void * writeBuffer(UCHAR * buff, int size);
	// �����븴�Ƶ�Զ�̽���
	bool writeBuffer(UCHAR * buff, int size, void * remoteAddress);
	// ִ��Զ�̴���
	void attachThreadRun(void * remoteAddress);
	// �����ش��븴�Ƶ�Զ�̲�ִ��
	void * runShellCode(UCHAR * code, int size);
	~ProcessHacker();
private:
	bool getProcess(const wchar_t * processName);
	DWORD getFirstThread();
	int processid;
	HANDLE pHandle;
};

