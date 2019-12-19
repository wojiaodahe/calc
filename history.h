
#pragma once

#include "calc.h"

struct history
{
	struct expr e;
	struct history* next;
	struct history* prev;
};

extern double getDollarX(unsigned int seq);
extern int addHistoryNode(struct expr* node);
extern void removeHistoryNode(struct expr* node);
extern char* getHistoryPrev(void);
extern char* getHistoryNext(void);



