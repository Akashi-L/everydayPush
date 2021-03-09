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

//³õÊ¼»¯
Status InitStack(SqStack* S);

//Ñ¹Õ»
Status Push(SqStack*S, SElemType e);

//³öÕ»
Status Pop(SqStack*S, SElemType *e);

//ÅÐ¿Õ
int sqStackEmpty(SqStack* S);