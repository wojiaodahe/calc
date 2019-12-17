#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include "calc.h"
#include "getmask.h"


/*
 * 获取 第from位到第to位的掩码
 * 例
 * from = 0  to = 7  则 mask = 0xff
 * from = 12 to = 15 则 mask = 0xf000
 */

const char* getmaskHelp(void)
{
	static const char* __getmaskHelp__ = "usage: gm(to,from) or gmsk(to,from) or getmask(to,from)\r\nget the mask from bit 'from' to bit 'to\r\nfor example:\r\nif from == 0 to == 7 then we will get the mask from 0 to 7 bits: 0xff";

	return __getmaskHelp__;
}

double _getmask(unsigned long from, unsigned long to)
{
	if (to > from)
		return (~(~(0) << (to - from + 1))) << from;
	else
		return (~(~(0) << (from - to + 1))) << to;
}

double getmask(struct expr* e)
{
	double a, b;

	a = calA(e);
	b = calA(e);
	return _getmask((unsigned int)a, (unsigned int)b);
}

static struct function getmaskFunctionNode =
{
	0, "getmask", getmask, getmaskHelp, FUNCTION_TYPE_0, NULL, NULL
};

void initGetMaskNode(void)
{
	registerFunctionNode(&getmaskFunctionNode);
}