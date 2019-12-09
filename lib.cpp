#include <windows.h>
#include <CommCtrl.h>

// C RunTime Header Files
#include <math.h>
#include <objbase.h>


TCHAR* CharToTchar(TCHAR *dest, const char *src, int len)
{
#ifdef _UNICODE 
	MultiByteToWideChar(CP_UTF8, 0, src, len, dest, 256);
	return dest;
#else 
	return src;
#endif
}

char* TcharToChar(char *dest, TCHAR* src, int len)
{
#ifdef _UNICODE 
	WideCharToMultiByte(CP_UTF8, 0, src, len, dest, 256, 0, 0);
	return dest;
#else 
	return src;
#endif
}