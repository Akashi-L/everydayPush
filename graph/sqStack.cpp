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
	//创建空栈
	S->base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	//判断栈是否创建成功
	if (!S->base) exit(OVERFLOW);
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return OK;
}

Status Push(SqStack*S, SElemType e) {
	if (S->top - S->base >= S->stacksize) {
		//栈满，扩展空间
		S->base = (SElemType*)realloc(S->base, (S->stacksize + STACKINCKEMENT) * sizeof(SElemType));
		if (!S->base)exit(OVERFLOW);
		S->top = S->base + S->stacksize;
		S->stacksize += STACKINCKEMENT;
	}
	*S->top++ = e;
	return OK;
}

Status Pop(SqStack*S, SElemType *e) {
	//如果栈非空，则将top element赋值给e然后删除the top element
	if (S->top == S->base)return ERROR;
	*e = *--S->top;
	return OK;
}

int sqStackEmpty(SqStack* S) {
	//栈空返回1
	if (S->top == S->base) return 1;
	//非空返回0
	else return 0;
}