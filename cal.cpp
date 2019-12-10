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

const static char* lib[] = { "sin", "cos", "tan", "lg", "ln", "log", "getmask" }; //支持的函数列表
static double calA(struct expr* e); //计算完整表达式的值，在此声明一下，以用于前向调用，到最后再实现它

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

/*
 * 获取 第from位到第to位的掩码
 * 例
 * from = 0  to = 7  则 mask = 0xff
 * from = 12 to = 15 则 mask = 0xf000
 */

unsigned long getmask(unsigned long from, unsigned long to)
{
	if (to > from)
		return (~(~(0) << (to - from + 1))) << from;
	else
		return (~(~(0) << (from - to + 1))) << to;
}

static double calE(struct expr* e) //计算一个数字、函数、或者小括号的值
{
	int i;
	double a, b;
	char fun[16];

	if (*e->cc == '(') //在此处理括号
	{
		e->cc++; //跳过左括号
		return calA(e); //括号之内是完整的表达式结构
	}
	else if (*e->cc >= 'a' && *e->cc <= 'z') //出现一个字母，表示这应该是一个函数，做函数处理
	{
		//截取函数名
		for (i = 0; *e->cc >= 'a' && *e->cc <= 'z'; e->cc++)
		{
			fun[i] = *e->cc;
			i++;
		}
		fun[i] = '\0';
		//判断函数名,在此不做错误检查
		i = 0;
		while (strcmp(lib[i], fun) != 0)
			i++;
		//执行函数计算
		e->cc++;//跳过函数名之后的左括号
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
		case 5: //这里处理二元函数log(a,b),','分开的两个参数是完整的两个表达式结构
			a = calA(e);
			b = calA(e);
			return log(b) / log(a); //对数的换底公式
		case 6:
			a = calA(e);
			b = calA(e);
			return getmask((unsigned int)a, (unsigned int)b);
			break;
		}
	}
	else //普通实数
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

static double calC(struct expr* e) //计算一个乘方项的值
{
	double a;

	a = calD(e);//第一个D项的值
	if (*e->cc == '^')
	{
		e->cc++;
		return pow(a, calC(e));//因为乘方是右结合的，所以，要先把右面的值算完才成
	}
	else
		return a;
}

static double calB(struct expr* e) //计算一个乘除项的值
{
	double a, b;
	char c;

	a = calC(e);//第一个C项的值
	while (1)
	{
		if (*e->cc == '*' || *e->cc == '/' || *e->cc == '%' || *e->cc == '|' || *e->cc == '&')
		{
			c = *e->cc;
			e->cc++;
			b = calC(e);//下一个C项的值
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

static double calA(struct expr* e)//计算完整表达式的值
{
	char c;
	double a, b;

	a = calB(e);//第一个B项的值
	while (*e->cc == '+' || *e->cc == '-')
	{
		c = *e->cc;
		e->cc++;
		b = calB(e);//下一个B项的值
		if (c == '+')
			a = a + b;
		else
			a = a - b;
	}

	if (*e->cc == ')' || *e->cc == ',') //由于括号包括起来的范围也是一个完整的表达式，所以要做退括号处理,','是因为对数函数log用它分开两个参数
		e->cc++;

	return a;
}

void print_bit(unsigned long num)
{
	int i;
	unsigned long bit = 0x01;

	for (i = 63; i >= 0; i--) /* 从第一个为1的位开始打印 */
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
