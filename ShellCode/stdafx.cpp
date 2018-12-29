#include "stdafx.h"
#include "converter.h"
#include <fstream>

using namespace std;

char * GetFileBuffer(const wchar_t * fileName, size_t &outSize) {
	ifstream file(fileName, ios::binary | ios::ate);
	if (file.good()) {
		file.seekg(0, file.end);
		size_t size = file.tellg();
		file.seekg(0, file.beg);
		auto buffer = new char[size];
		file.read(buffer, size);
		file.close();
		outSize = size;
		return buffer;
	}
	else {
		return 0;
	}
}

bool GetShellCode(char * dllBuffer, size_t size, char * &outBuffer, DWORD &outSize) {
	if (dllBuffer[0] == 'M' && dllBuffer[1] == 'Z') {
		return ConvertToShellcode(dllBuffer, size, 0, 0, 0, SRDI_CLEARHEADER, outBuffer, outSize);
	}
	else {
		return false;
	}
}

char * GetShellCode(const wchar_t * fileName, size_t &outSize) {
	size_t fileSize = 0;
	auto fileBuffer = GetFileBuffer(fileName, fileSize);
	if (fileSize == 0) {
		return 0;
	}
	
	char * codeBuffer;
	DWORD codeSize;
	auto r = GetShellCode(fileBuffer, fileSize, codeBuffer, codeSize);

	outSize = codeSize;
	delete[] fileBuffer;
	return codeBuffer;
}