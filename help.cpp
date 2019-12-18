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
	int id; 
	struct function* fNode;

	printf("use it like this:\n");
	printf("!x*(x<<x)+x&x|0xff+~(a|b)\n");
	printf("getmask(x1,x1) get the mask from 'x1' to 'x2'\n");
	printf("sin(x)+con(y)*z^g  z^y means z to the yth power\n");

	fNode = findFunctionByName(e->cc);
	if (fNode)
		strcpy(e->resultStr, fNode->help());

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