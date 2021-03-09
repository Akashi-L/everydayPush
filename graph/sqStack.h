#pragma once
#include<stdio.h>
#include<stdlib.h>

#define STACK_INIT_SIZE 100
#define STACKINCKEMENT 10
#define OK 1
#define ERROR -1
#define OVERFLOW -2
typedef int SElemType;
typedef int Status;

typedef struct {
	SElemType* base;
	SElemType* top;
	int stacksize;
}SqStack;

//��ʼ��
Status InitStack(SqStack* S);

//ѹջ
Status Push(SqStack*S, SElemType e);

//��ջ
Status Pop(SqStack*S, SElemType *e);

//�п�
int sqStackEmpty(SqStack* S);