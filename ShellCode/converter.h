#pragma once
#include <Windows.h>
#define DEREF_64(name) *(DWORD64 *)(name)
#define DEREF_32(name) *(DWORD *)(name)
#define DEREF_16(name) *(WORD *)(name)
#define DEREF_8(name) *(BYTE *)(name)
#define ROTR32(value, shift)                                                   \
  (((DWORD)value >> (BYTE)shift) | ((DWORD)value << (32 - (BYTE)shift)))

#define SRDI_CLEARHEADER 0x1

DWORD GetFileContents(LPCSTR filename, LPSTR *data, DWORD &size);
DWORD HashFunctionName(LPSTR name);
BOOL ConvertToShellcode(LPVOID inBytes, DWORD length, DWORD userFunction,
	LPVOID userData, DWORD userLength, DWORD flags,
	LPSTR &outBytes, DWORD &outLength);