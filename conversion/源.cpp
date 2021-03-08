#include<stdio.h>
#include<stdlib.h>

#define STACK_INIT_SIZE 100
#define STACKINCKEMENT 10
#define OK 1
#define ERROR 0
#define OVERFLOW -2
typedef int SElemType;
typedef int Status;

typedef struct {
	SElemType* base;
	SElemType* top;
	int stacksize;
}SqStack;

Status InitStack(SqStack* S) {
	//������ջ
	S->base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	//�ж�ջ�Ƿ񴴽��ɹ�
	if (!S->base) exit(OVERFLOW);
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return OK;
}

Status Push(SqStack*S, SElemType e) {
	if (S->top - S->base >= S->stacksize) {
		//ջ������չ�ռ�
		S->base = (SElemType*)realloc(S->base, (S->stacksize + STACKINCKEMENT) * sizeof(SElemType));
		if (!S->base)exit(OVERFLOW);
		S->top = S->base + S->stacksize;
		S->stacksize += STACKINCKEMENT;
	}
	*S->top++ = e;
	return OK;
}

Status Pop(SqStack*S, SElemType *e) {
	//���ջ�ǿգ���top element��ֵ��eȻ��ɾ��the top element
	if (S->top == S->base)return ERROR;
	*e = *--S->top;
	return OK;
}

void octonary() {
	SqStack S;
	SElemType e;
	//���һ��ʮ������
	int num;
	printf("������һ��ʮ������:");
	scanf_s("%d", &num);
	InitStack(&S);
	//numÿ�ζ�ִ��num/8��������num��Ϊ0ʱ֤��ʮ����ת���ɰ˽���δ����
	while (num != 0) {
		Push(&S, num % 8);
		num = num / 8;
	}
	//��top��base��ָ��ͬһ��λ��ʱ֤��ջ�л���Ԫ��δ����
	while (S.top!= S.base) {
		Pop(&S, &e);
		printf("%d", e);
	}
}

int main() {
	//����ʮ����ת���ɰ˽��Ƶ�����
	octonary();
	getchar();
	getchar();
	return 0;
}