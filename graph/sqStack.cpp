#include<stdio.h>
#include<stdlib.h>
#include"sqStack.h"

#define STACK_INIT_SIZE 100
#define STACKINCKEMENT 10
#define OK 1
#define ERROR -1
#define OVERFLOW -2
typedef int SElemType;
typedef int Status;

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

int sqStackEmpty(SqStack* S) {
	//ջ�շ���1
	if (S->top == S->base) return 1;
	//�ǿշ���0
	else return 0;
}