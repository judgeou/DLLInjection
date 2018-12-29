#include "ProcessHacker.h"

using namespace std;

ProcessHacker::ProcessHacker(const wchar_t * processName)
{
	getProcess(processName);
}

bool ProcessHacker::getProcess(const wchar_t * processName) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, (DWORD)NULL);

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			wstring pFileName(entry.szExeFile);
			wstring searchPName(processName);
			if (pFileName == searchPName) {
				processid = entry.th32ProcessID;
				pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processid);
				break;
			}
		}
	}
	CloseHandle(snapshot);
	return true;
}

bool ProcessHacker::isGood() {
	return (processid != 0) && (pHandle != 0);
}

bool ProcessHacker::writeBuffer(UCHAR * buff, int size, void * remoteAddress) {
	return WriteProcessMemory(pHandle, remoteAddress, buff, size, NULL);
}

void * ProcessHacker::writeBuffer(UCHAR * buff, int size) {
	auto remoteAddr = VirtualAllocEx(pHandle, NULL, size, MEM_COMMIT, PAGE_READWRITE);
	writeBuffer(buff, size, remoteAddr);
	DWORD old;
	VirtualProtectEx(pHandle, remoteAddr, size, PAGE_EXECUTE_READ, &old);
	return remoteAddr;
}

DWORD ProcessHacker::getFirstThread() {
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	auto pid = processid;
	if (h != INVALID_HANDLE_VALUE) {
		THREADENTRY32 te;
		te.dwSize = sizeof(te);
		if (Thread32First(h, &te)) {
			do {
				if (te.th32OwnerProcessID == pid) {
					return te.th32ThreadID;
				}
				te.dwSize = sizeof(te);
			} while (Thread32Next(h, &te));
		}
		CloseHandle(h);
	}
	return 0;
}

void ProcessHacker::attachThreadRun(void * remoteAddress) {
	UCHAR wraperCode[] = { 0x60, 0x9C, 0xB8, 00, 00, 00, 00, 0xFF, 0xD0, 0x9D, 0x61, 0x68, 00, 00, 00, 00, 0xC3 };
	int wraperSize = sizeof(wraperCode);
	*(DWORD *)&wraperCode[3] = (DWORD)remoteAddress;

	// 停住线程，获取Eip
	auto tid = getFirstThread();
	HANDLE tHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, tid);
	CONTEXT ctx;
	ctx.ContextFlags = CONTEXT_ALL;
	SuspendThread(tHandle);
	GetThreadContext(tHandle, &ctx);

	// 将EIP写入跳转
	*(DWORD*)&wraperCode[12] = ctx.Eip;

	// 将wraperCode写入远程
	auto wraperAddr = writeBuffer(wraperCode, wraperSize);

	// 修改线程EIP
	ctx.Eip = (DWORD)wraperAddr;
	SetThreadContext(tHandle, &ctx);
	ResumeThread(tHandle);
	
	CloseHandle(tHandle);
}

void * ProcessHacker::runShellCode(UCHAR * code, int size) {
	auto codeAddr = writeBuffer(code, size);
	attachThreadRun(codeAddr);
	return codeAddr;
}

ProcessHacker::~ProcessHacker()
{
	CloseHandle(pHandle);
}
