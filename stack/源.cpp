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

//��֤���������Ƿ�Ϸ�
Status clarity(char brackets[]) {
	SqStack S;
	InitStack(&S);
	char b;
	//�����������ַ���Ϊ'\0'ʱ��֤����������δ����
	for (int i = 0; brackets[i] != 0; i++) {
		//������������������ѹջ
		if (brackets[i] == '(' || brackets[i] == '[') {
			Push(&S, brackets[i]);
		}
		//������������ţ�����Ҫ��������if����е������1.Pop�Ƿ���Ե��������ţ��������ʧ��֤����������ĿС��������
		//2.�����������ŵ�ǰһ���Ƿ�����֮��ƥ���������
		else if (brackets[i] == ')' || brackets[i] == ']') {
			if (Pop(&S, &b) == ERROR || brackets[i] == ')'&&b != '(' || brackets[i] == ']'&&b != '[') {
				printf("�˴�����ƥ�䲻�Ϸ�");
				return ERROR;
			}
		}
		else {
			printf("���������������������");
			return ERROR;
		}
	 }
	//ջ��֤������ȫ���������쳣��֤����������ƥ��
	if (S.base == S.top) {
		printf("ƥ��");
		return OK;
	}
	else {
		printf("�˴�����ƥ�䲻�Ϸ�");
		return ERROR;
	}
}

int main() {
	//������������
	printf("������һ����������:");
	char brackets[20] = "";
	scanf_s("%s", brackets,20);
	//�ж������Ƿ�Ϸ�
	clarity(brackets);

	getchar();
	getchar();
	return 0;
}