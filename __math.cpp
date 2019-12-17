#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include "calc.h"

double string2dec(char** p)
{
	double num1 = 0;
	double num2 = 0;
	double n = 1;

	while ('0' <= **p && **p <= '9')  //循环得到小数点前面的数
	{
		num1 = num1 * 10 + (**p - '0');
		(*p)++;
	}

	if (**p == '.') //循环得到 小数点后面的数
	{
		(*p)++;
		while ('0' <= **p && **p <= '9')
		{
			n /= 10;
			num2 += (**p - '0') * n;
			(*p)++;
		}
	}

	return num1 + num2; //返回操作数

}

const char HEX[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
unsigned long string2hex(char** str)
{
	char* p = *str;
	unsigned long val = 0;

	if (*p == '0' && (*(p + 1) == 'X' || *(p + 1) == 'x'))
		p += 2;

	while (*p)
	{
		if (*p >= '0' && *p <= '9')
		{
			val = val << 4;
			val |= HEX[*p - 48];
		}
		else if (*p >= 'a' && *p <= 'f')
		{
			val = val << 4;
			val |= HEX[*p - 97 + 10];
		}
		else if (*p >= 'A' && *p <= 'F')
		{
			val = val << 4;
			val |= HEX[*p - 65 + 10];
		}
		else
		{
			break;
		}
		p++;
	}

	*str = p;
	return val;
}

double string2num(struct expr* e)
{
	if (*e->cc == '0' && (*(e->cc + 1) == 'x' || *(e->cc + 1) == 'X'))
	{
		return string2hex(&e->cc);
	}
	else
		return string2dec(&e->cc);
}

double funSin(struct expr* e)
{
	return sin(calA(e));
}

double funCos(struct expr* e)
{
	return cos(calA(e));
}
double funTan(struct expr* e)
{
	return tan(calA(e));
}

double funLg(struct expr* e)
{
	return log10(calA(e));
}

double funLn(struct expr* e)
{
	return log(calA(e));
}

double funLog(struct expr* e)
{
	double a, b;

	a = calA(e);
	b = calA(e);

	return log(b) / log(a);
}
