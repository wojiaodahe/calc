#include <Windows.h>
#include <locale.h>

int OpenAndReadFile(LPCWSTR filePath, char **buf)
{
	int ret;
	char* readBuf;
	DWORD dwSize;
	DWORD dwHigh = 0;


	HANDLE handle = CreateFile(filePath, 
							   GENERIC_READ, 
							   0, 
							   NULL, 
							   OPEN_EXISTING,
							   FILE_ATTRIBUTE_NORMAL,
							   NULL);

	if (handle == NULL) 
		return -1;

	dwSize = GetFileSize(handle, &dwHigh);
	readBuf = (char *)malloc(dwSize);

	ret = ReadFile(handle, readBuf, dwSize, 0, NULL);
	CloseHandle(handle);

	if (ret == 0) 
	{
		free(readBuf);
		return -1;
	}

	*buf = readBuf;

	return dwSize;
}