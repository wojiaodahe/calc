#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

#include "calc.h"
#include "getmask.h"
#include "__math.h"

static int calStatus = 0; /* ������ͨ��return����,�м����״̬����calStatus�� */
static struct function* funHead;

struct function* traverseList(struct function** head, int (*Callback)(struct function *node, void *argv), void* argv)
{
	struct function** curr;
	struct function* entry;

	for (curr = head; *curr; )
	{
		entry = *curr;
		if (Callback(entry, argv))
			return entry;

		curr = &entry->next;
	}

	return NULL;
}

int functionMatchName(struct function* node1, void *argv)
{
	return strcmp(node1->name, (const char *)argv) == 0;
}

int __removeFunction(struct function** head, struct function* node)
{
	struct function** curr;
	struct function* entry;

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

struct function *findFunctionByName(const char* name)
{
	struct function* entry = NULL;

	entry = traverseList(&funHead, functionMatchName, (void *)name);
	
	return entry;
}

int __addFunctionNode(struct function** head, struct function* node)
{
	struct function** curr;
	struct function* entry;

	entry = findFunctionByName(node->name);
	if (entry)
		return -1;

	for (curr = head; *curr; )
	{
		entry = *curr;
		curr = &entry->next;
	}

	*curr = node;

	return 0;
}

int registerFunctionNode(struct function* node)
{
	return __addFunctionNode(&funHead, node);
}

void unregisterFunctionNode(struct function* node)
{
	__removeFunction(&funHead, node);
}

double handleFunction(struct expr* e)
{
	int i, id;
	char fun[64];
	struct function* fNode;

	for (i = 0; *e->cc >= 'a' && *e->cc <= 'z'; e->cc++)
	{
		fun[i] = *e->cc;
		i++;

	if (i >= sizeof(fun))
	{
		//set error status and return
		return 0;
	}
	}
	fun[i] = '\0';

	if (*e->cc == '(')
		e->cc++; //����������֮���������

	if (*e->cc == ' ')
		e->cc++; //����������֮��Ŀո�

	fNode = findFunctionByName(fun);
	if (fNode && fNode->fun)
		return fNode->fun(e);
}

static double calE(struct expr* e) //����һ�����֡�����������С���ŵ�ֵ
{
	int i;
	double a, b;
	char fun[16];

	if (*e->cc == '(') //�ڴ˴�������
	{
		e->cc++; //����������
		return calA(e); //����֮���������ı��ʽ�ṹ
	}
	else if (*e->cc >= 'a' && *e->cc <= 'z') //����һ����ĸ����ʾ��Ӧ����һ������������������
		return handleFunction(e);
	else //��ͨʵ��
	{
		a = string2num(e);
		return a;
	}
}

static double calD(struct expr* e)
{
	int ret;
	if (*e->cc == '!')
	{
		e->cc++;
		ret = (unsigned long)calD(e);
		return !ret;
	}
	else if (*e->cc == '~')
	{
		e->cc++;
		ret = (unsigned long)calD(e);
		return ~ret;
	}
	else
		return calE(e);
}

static double calC(struct expr* e) //����һ���˷����ֵ
{
	double a;

	a = calD(e);//��һ��D���ֵ
	if (*e->cc == '^')
	{
		e->cc++;
		return pow(a, calC(e));//��Ϊ�˷����ҽ�ϵģ����ԣ�Ҫ�Ȱ������ֵ����ų�
	}
	else
		return a;
}

static double calB(struct expr* e) //����һ���˳����ֵ
{
	double a, b;
	char c;

	a = calC(e);//��һ��C���ֵ
	while (1)
	{
		if (*e->cc == '*' || *e->cc == '/' || *e->cc == '%' || *e->cc == '|' || *e->cc == '&')
		{
			c = *e->cc;
			e->cc++;
			b = calC(e);//��һ��C���ֵ
			if (c == '*')
				a = a * b;
			else if (c == '/')
				a = a / b;
			else if (c == '%')
				a = (unsigned long)a % (unsigned long)b;
			else if (c == '|')
				a = (unsigned long)a | (unsigned long)b;
			else if (c == '&')
				a = (unsigned long)a & (unsigned long)b;
		}
		else if (strncmp(e->cc, "<<", 2) == 0)
		{
			e->cc += 2;
			b = calC(e);
			a = (unsigned long)a << (unsigned long)b;
		}

		else if (strncmp(e->cc, ">>", 2) == 0)
		{
			e->cc += 2;
			b = calC(e);
			a = (unsigned long)a >> (unsigned long)b;
		}
		else
			break;
	}

	return a;
}

double calA(struct expr* e)//�����������ʽ��ֵ
{
	char c;
	double a, b;

	a = calB(e);//��һ��B���ֵ
	while (*e->cc == '+' || *e->cc == '-')
	{
		c = *e->cc;
		e->cc++;
		b = calB(e);//��һ��B���ֵ
		if (c == '+')
			a = a + b;
		else
			a = a - b;
	}

	if (*e->cc == ')' || *e->cc == ',') //�������Ű��������ķ�ΧҲ��һ�������ı��ʽ������Ҫ�������Ŵ���,','����Ϊ��������log�����ֿ���������
		e->cc++;

	return a;
}

char *print_bit(unsigned long num)
{
	static char str[512];
	char* tmp;
	int i;
	unsigned long bit = 0x01;

	memset(str, 0, sizeof (str));
	tmp = str;

	for (i = 31; i >= 0; i--) /* �ӵ�һ��Ϊ1��λ��ʼ��ӡ */
	{
		if (num & (bit << i))
			break;
	}
//		sprintf(str, "%d", num);
//		return str; 

	for (; i >= 0; i--)
	{
		//printf("%d", (num & (bit << i)) ? 1 : 0);
		*tmp++ = (num & (bit << i)) ? '1' : '0';
		if (!(i % 4))
			*tmp++ = ' ';
	}

	return str;
}


void stripBlank(char* str)
{
	char* tmp0;
	char* tmp1;

	tmp0 = str;
	tmp1 = str;

	while (*tmp1)
	{
		while (*tmp1 && *tmp1 != ' ')
			*tmp0++ = *tmp1++;

		while (*tmp1 == ' ')
			tmp1++;

		/* �ո����˶�����ĸ��������������� help getmask ���������������ַ���֮��Ҫ��һ���ո�*/
		if (*(tmp0 - 1) >= 'a' && *(tmp0 - 1) <= 'z' && *tmp1 >= 'a' && *tmp1 <= 'z')
			tmp0++;
	}

	*tmp0 = 0;
}

int calc(char *str, char *result_str)
{
	double result;
	struct expr e;

	stripBlank(str);

	strcpy(e.str, str);
	e.cc = e.str;
	e.resultStr = result_str;
	*e.resultStr = 0;

	result = calA(&e);
	if (!strlen(e.resultStr))
		sprintf(result_str, "flt: %lf\r\ndec: %ld\r\nhex: %lx\r\nbin: %s\r\n\r\n", result, (unsigned long)result, (unsigned long)result, print_bit((long)result));
	
	return result;
}
