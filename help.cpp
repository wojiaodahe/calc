#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include "calc.h"
#include "help.h"

const char* help()
{
	return "help function";
}

double Help(struct expr* e)
{
	struct function* fNode;

	printf("use it like this:\n");
	printf("!x*(x<<x)+x&x|0xff+~(a|b)\n");
	printf("getmask(x1,x1) get the mask from 'x1' to 'x2'\n");
	printf("sin(x)+con(y)*z^g  z^y means z to the yth power\n");

	const char* helpInformation = "Hello\r\nThis is a calculator. \r\nYou can input a whole expression and get result by pressing ENTER. Just link this: !x*(x<<x)+x&x|0xff+~(a|b)\r\n You can get History-Input by pressing KEY-UP or KEY-DOWN\r\nIt support functions: 'sin' or 'cos' or 'getmask'...\r\nFunction 'ls' can list all functions supported now\r\nYou can get the help information of one function by 'help xxx' command(xxx is the function's name).\r\nAnd also, help is a function, so you can get the help's help information by 'help help'\r\n $X is the Xth result: $0 $1 $10086.... you can use it lisk this: $2 + 4 << 8.\r\n$@ is the last result\r\nMore features are under development\r\nEnjoy\r\n\r\n";                                                                                

	fNode = findFunctionByName(e->cc);
	if (fNode)
		strcpy(e->resultStr, fNode->help());
	else
		strcpy(e->resultStr, helpInformation);
	return 0;
}

static struct function HelpFunctionNode =
{
	0, "help", Help, help, FUNCTION_TYPE_1, NULL, NULL
};

void initHelpNode(void)
{
	registerFunctionNode(&HelpFunctionNode);
}