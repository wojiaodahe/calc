#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

#define PI  3.14159
#define N   1000

struct expr
{
	char str[N + 1];
	char* cc;
};

const static char* lib[] = { "sin", "cos", "tan", "lg", "ln", "log", "getmask" }; //֧�ֵĺ����б�
static double calA(struct expr* e); //�����������ʽ��ֵ���ڴ�����һ�£�������ǰ����ã��������ʵ����

double string2dec(char** p)
{
	double num1 = 0;
	double num2 = 0;
	double n = 1;

	while ('0' <= **p && **p <= '9')  //ѭ���õ�С����ǰ�����
	{
		num1 = num1 * 10 + (**p - '0');
		(*p)++;
	}

	if (**p == '.') //ѭ���õ� С����������
	{
		(*p)++;
		while ('0' <= **p && **p <= '9')
		{
			n /= 10;
			num2 += (**p - '0') * n;
			(*p)++;
		}
	}

	return num1 + num2; //���ز�����

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

/*
 * ��ȡ ��fromλ����toλ������
 * ��
 * from = 0  to = 7  �� mask = 0xff
 * from = 12 to = 15 �� mask = 0xf000
 */

unsigned long getmask(unsigned long from, unsigned long to)
{
	if (to > from)
		return (~(~(0) << (to - from + 1))) << from;
	else
		return (~(~(0) << (from - to + 1))) << to;
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
	{
		//��ȡ������
		for (i = 0; *e->cc >= 'a' && *e->cc <= 'z'; e->cc++)
		{
			fun[i] = *e->cc;
			i++;
		}
		fun[i] = '\0';
		//�жϺ�����,�ڴ˲���������
		i = 0;
		while (strcmp(lib[i], fun) != 0)
			i++;
		//ִ�к�������
		e->cc++;//����������֮���������
		switch (i)
		{
		case 0:
			return sin(calA(e));
		case 1:
			return cos(calA(e));
		case 2:
			return tan(calA(e));
		case 3:
			return log10(calA(e));
		case 4:
			return log(calA(e));
		case 5: //���ﴦ���Ԫ����log(a,b),','�ֿ��������������������������ʽ�ṹ
			a = calA(e);
			b = calA(e);
			return log(b) / log(a); //�����Ļ��׹�ʽ
		case 6:
			a = calA(e);
			b = calA(e);
			return getmask((unsigned int)a, (unsigned int)b);
			break;
		}
	}
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

static double calA(struct expr* e)//�����������ʽ��ֵ
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

void print_bit(unsigned long num)
{
	int i;
	unsigned long bit = 0x01;

	for (i = 63; i >= 0; i--) /* �ӵ�һ��Ϊ1��λ��ʼ��ӡ */
	{
		if (num & (bit << i))
			break;
	}

	for (; i >= 0; i--)
	{
		printf("%d", (num & (bit << i)) ? 1 : 0);
		if (!(i % 4))
			printf(" ");
	}
}

void help(void)
{
	printf("use it like this:\n");
	printf("!x*(x<<x)+x&x|0xff+~(a|b)\n");
	printf("getmask(x1,x1) get the mask from 'x1' to 'x2'\n");
	printf("sin(x)+con(y)*z^g  z^y means z to the yth power\n");
}

int calc(char *str)
{
	double result;
	struct expr e;

	strcpy(e.str, str);
	e.cc = e.str;

	result = calA(&e);

	printf("flt: %lf\n", result);
	printf("dec: %ld\n", (long)result);
	printf("hex: %lx\n", (unsigned long)result);
	printf("bin: ");
	print_bit((unsigned long)result);
	printf("\n");

	return result;
}
