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

void octonary() {
	SqStack S;
	SElemType e;
	//获得一个十进制数
	int num;
	printf("请输入一个十进制数:");
	scanf_s("%d", &num);
	InitStack(&S);
	//num每次都执行num/8操作，当num不为0时证明十进制转换成八进制未结束
	while (num != 0) {
		Push(&S, num % 8);
		num = num / 8;
	}
	//当top和base不指向同一个位置时证明栈中还有元素未弹出
	while (S.top!= S.base) {
		Pop(&S, &e);
		printf("%d", e);
	}
}

int main() {
	//进行十进制转换成八进制的运算
	octonary();
	getchar();
	getchar();
	return 0;
}