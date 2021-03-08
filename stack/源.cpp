#include<stdio.h>
#include<stdlib.h>

#define STACK_INIT_SIZE 100
#define STACKINCKEMENT 10
#define OK 1
#define ERROR -1
#define OVERFLOW -2
typedef char SElemType;
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

//验证换号序列是否合法
Status clarity(char brackets[]) {
	SqStack S;
	InitStack(&S);
	char b;
	//当所读出的字符不为'\0'时，证明括号序列未读完
	for (int i = 0; brackets[i] != 0; i++) {
		//如果读到左括号则进行压栈
		if (brackets[i] == '(' || brackets[i] == '[') {
			Push(&S, brackets[i]);
		}
		//如果读到右括号，则需要考虑如下if语句中的情况：1.Pop是否可以弹出左括号，如果弹出失败证明左括号数目小于右括号
		//2.所读入右括号的前一个是否是与之相匹配的左括号
		else if (brackets[i] == ')' || brackets[i] == ']') {
			if (Pop(&S, &b) == ERROR || brackets[i] == ')'&&b != '(' || brackets[i] == ']'&&b != '[') {
				printf("此串括号匹配不合法");
				return ERROR;
			}
		}
		else {
			printf("输入错误，请输入括号序列");
			return ERROR;
		}
	 }
	//栈空证明括号全被弹出无异常，证明括号序列匹配
	if (S.base == S.top) {
		printf("匹配");
		return OK;
	}
	else {
		printf("此串括号匹配不合法");
		return ERROR;
	}
}

int main() {
	//输入括号序列
	printf("请输入一个括号排列:");
	char brackets[20] = "";
	scanf_s("%s", brackets,20);
	//判断序列是否合法
	clarity(brackets);

	getchar();
	getchar();
	return 0;
}