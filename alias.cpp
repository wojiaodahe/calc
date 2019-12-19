#include "alias.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include "calc.h"

const char* aliasHelp(void)
{
	static const char* __getmaskHelp__ = "usage: alias source_name dest_nmae. for example: alias getmask gm";

	return __getmaskHelp__;
}

double alias(struct expr* e)
{
	struct function* source;
	struct function* dest;
	char name[MAX_FUNCTION_NAME_SIZE] = { 0 };

	if (!getFunctionName(e, name))
	{
		sprintf(e->resultStr, "%s: Source Name Can't To Be NULL\r\n\r\n", __func__);
		return -1;
	}
	source = findFunctionByName(name);
	if (!source)
	{
		sprintf(e->resultStr, "No function named %s\r\n\r\n", e->cc);
		return -1;
	}

	dest = allocFunctionNode();
	if (!dest)
	{
		sprintf(e->resultStr, "%s Out Of Memmory\r\n\r\n", __func__);
		return -1;
	}
	
	memset(name, 0, sizeof (name));
	if (!getFunctionName(e, name))
	{
		sprintf(e->resultStr, "%s: Dest Name Can't To Be NULL\r\n\r\n", __func__);
		return -1;
	}

	dest->fun = source->fun;
	dest->flag = source->flag;
	dest->help = source->help;
	dest->next = NULL;
	dest->prev = NULL;

	strcpy(dest->name, name);
	if (registerFunctionNode(dest) < 0)
	{
		freeFunctionNode(dest);
		sprintf(e->resultStr, "%s: register Failed\r\n\r\n", __func__);
		return -1;
	}

	sprintf(e->resultStr, "%s: %s to %s Success\r\n\r\n", __func__, source->name, dest->name);
	return 0;
}

static struct function AliasFunctionNode =
{
	0, "alias", alias, aliasHelp, FUNCTION_TYPE_1, NULL, NULL
};

void initAliasNode(void)
{
	registerFunctionNode(&AliasFunctionNode);
}