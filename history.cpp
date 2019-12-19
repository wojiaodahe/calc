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

/* ��ȡ��ʷ��¼ */
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

/* ֻ��ȡ������ѧ����ļ�¼ */
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
	current = node;

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
	
	if (!current)
		return NULL;
	
	if (!current->prev)
		return current->str;

	tmp = current;
	current = current->prev;
	
	return tmp->str;
}

char* getHistoryNext(void)
{
	if (!current || !current->next)
		return NULL;
	
	current = current->next;

	return current->str;
}

/* $0 == ��0��������
 * $1 == ��һ��������
 * ֻ������ѧ������ʽ�Ľ��,������help alias����������������
 * ����:
 * 1+2
 * help getmask
 * 2+3
 * ��$0 == 3 $1 == 5
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