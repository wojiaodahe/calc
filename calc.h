#pragma once

#define PI  3.14159
#define N   1000

struct expr
{
	char str[N + 1];
	char* cc;
	double result;
	char* resultStr;
};

/* TYPE_0:�ɲ�������ĺ�������sin getmask cos�� TYPE_1:����������ĺ�������help clear set alias�� */
#define FUNCTION_TYPE_0	(1 << 0)
#define FUNCTION_TYPE_1	(1 << 1)
struct function
{
	int id;
	const char* name;
	double (*fun)(struct expr* e);
	const char* (*help)(void);
	unsigned int flag; /* ��ʱ����ָ������ type_0 or type_1  */

	struct function* next;
	struct function* prev;
};

enum funId
{
	FUNCTION_ID_SIN = 0,
	FUNCTION_ID_COS,
	FUNCTION_ID_TAN,
	FUNCTION_ID_LG,
	FUNCTION_ID_LN,
	FUNCTION_ID_LOG,

	FUNCTION_ID_GETMASK,
	FUNCTION_ID_GMASK,
	FUNCTION_ID_GMSK,
	FUNCTION_ID_GM,


};
extern int registerFunctionNode(struct function* node);
extern void unregisterFunctionNode(struct function* node);
extern struct function* findFunctionByName(const char* name);

double calA(struct expr* e); //�����������ʽ��ֵ���ڴ�����һ�£�������ǰ����ã��������ʵ����