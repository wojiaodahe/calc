#include <windows.h>
#include <CommCtrl.h>

// C RunTime Header Files
#include <math.h>
#include <objbase.h>


TCHAR* CharToTchar(TCHAR *dest, const char *src, int strLen, int tcharLen)
{
#ifdef _UNICODE 
	MultiByteToWideChar(CP_UTF8, 0, src, strLen, dest, tcharLen);
	return dest;
#else 
	return src;
#endif
}

char* TcharToChar(char *dest, TCHAR* src, int strLen, int tcharLen)
{
#ifdef _UNICODE 
	WideCharToMultiByte(CP_UTF8, 0, src, tcharLen, dest, strLen, 0, 0);
	return dest;
#else 
	return src;
#endif
}