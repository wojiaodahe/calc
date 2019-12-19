#include "calc.h"
#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

static struct expr* hisHead = NULL;
static struct expr* current = NULL;

int __removeHistory(struct expr** head, struct expr* node)
{
	struct expr** curr;
	struct expr* entry;

	for (curr = head; *curr; )
	{
		entry = *curr;
		if (entry == node)
		{
			*curr = entry->next;
			//free(entry);
			return 0;
		}
		else
			curr = &entry->next;
	}
	return 1;
}

/* 获取历史纪录 */
struct expr* getHistoryNodeBySeq(struct expr** head, unsigned int seq)
{
	struct expr** curr;
	struct expr* entry;
	unsigned int s = 0;

	for (curr = head; *curr; )
	{
		entry = *curr;
		if (s == seq)
			return entry;
		curr = &entry->next;
		
		s++;
	}
	return NULL;
}

/* 只获取参与数学运算的纪录 */
struct expr* getMathHistoryNodeBySeq(struct expr** head, unsigned int seq)
{
	struct expr** curr;
	struct expr* entry;
	unsigned int s = 0;

	for (curr = head; *curr; )
	{
		entry = *curr;
		curr = &entry->next;

		if (entry->flag & EXPRESSION_TYPE_MATH)
		{
			if (s == seq)
				return entry;
			s++;
		}
	}
	return NULL;
}

int __addHistoryNode(struct expr** head, struct expr* node)
{
	struct expr** curr = NULL;
	struct expr* entry = NULL;

	for (curr = head; *curr; )
	{
		entry = *curr;
		curr = &entry->next;
	}

	*curr = node;
	node->prev = entry;
	hisHead->prev = node;
	current = NULL;

	return 0;
}

int addHistoryNode(struct expr* node)
{
	return __addHistoryNode(&hisHead, node);
}

void removeHistoryNode(struct expr* node)
{
	__removeHistory(&hisHead, node);
}

char* getHistoryPrev(void)
{
	struct expr* tmp;
	
	if (!hisHead || current == hisHead)
		return NULL;
	
	if (!current)
	{
		current = hisHead->prev;
		return current->str;
	}
	
	current = current->prev;
	return current->str;
}

char* getHistoryNext(void)
{
	if (!current)
		return NULL;
	
	current = current->next;
	
	if (!current)
		return NULL;

	return current->str;
}

/* $0 == 第0个运算结果
 * $1 == 第一个运算结果
 * 只包括数学运算表达式的结果,不包括help alias这样不参数运算结果
 * 例如:
 * 1+2
 * help getmask
 * 2+3
 * 则$0 == 3 $1 == 5
 */
double getDollarX(unsigned int seq)
{
	struct expr* entry;

	entry = getMathHistoryNodeBySeq(&hisHead, seq);

	if (!entry)
	{
		//set error message
		return 0;
	}

	return entry->result;
}

double getDollarAt(void)
{
	struct expr* entry;
	
	if (!hisHead)
		return NULL;

	entry = hisHead->prev;
	do
	{
		if (entry->flag & EXPRESSION_TYPE_MATH)
			return entry->result;
	} while (entry != hisHead);

	return 0;
}