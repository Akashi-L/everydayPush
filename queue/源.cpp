#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996);

#define STACK_INIT_SIZE 100
#define STACKINCKEMENT 10
#define OK 1
#define ERROR 0
#define OVERFLOW -2

typedef int Status;
typedef struct{
	int carnumber;
	int time;
}SElemType;

typedef struct {
	SElemType* base;
	SElemType* top;
	int stacksize;
}SqStack;

//ջ
Status InitStack(SqStack* S) {
	//������ջ
	S->base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	//�ж�ջ�Ƿ񴴽��ɹ�
	if (!S->base) exit(OVERFLOW);
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return OK;
}

Status Push(SqStack*S, int carnum_,int time_) {
	if (S->top - S->base >= S->stacksize) {
		//ջ������չ�ռ�
		S->base = (SElemType*)realloc(S->base, (S->stacksize + STACKINCKEMENT) * sizeof(SElemType));
		if (!S->base)exit(OVERFLOW);
		S->top = S->base + S->stacksize;
		S->stacksize += STACKINCKEMENT;
	}
	S->top->carnumber = carnum_;
	S->top->time = time_;
	S->top++;
	return OK;
}

Status Pop(SqStack*S, SElemType *e) {
	//���ջ�ǿգ���top element��ֵ��eȻ��ɾ��the top element
	if (S->top == S->base)return ERROR;
	*e = *--S->top;
	return OK;
}

//����
#define QUEUE_INIT_SIZE 10//���д洢�ռ�ĳ�ʼ������
#define QUEUE_INCREMENT 2//���д洢�ռ�ķ�������
typedef struct {
	int carnumber_;
	int time_;
}QElemType;
struct SqQueue {
	QElemType*base;//��ʼ���Ķ�̬����洢�ռ�
	int rear;//βָ�룬�����в��գ�ָ�����βԪ�ص���һ��λ��
	int front;
	int queuesize;//��ǰ����Ĵ洢����
};
//������д洢�ռ�
Status InitQueue(SqQueue *Q) {
	Q->base = (QElemType*)malloc(QUEUE_INIT_SIZE * sizeof(QElemType));
	if (!Q->base) exit(OVERFLOW);
	Q->front = Q->rear = 0;
	return OK;
}

//����Ԫ��eΪQ���µĶ�βԪ��
void EnQueue(SqQueue* Q, int carnum,int time_) {
	if ((*Q).rear == (*Q).queuesize)//��ǰ�洢�ռ�����
	{
		(*Q).base = (QElemType*)realloc((*Q).base, ((*Q).queuesize + QUEUE_INCREMENT) * sizeof(QElemType));
		//����ʧ��
		if (!(*Q).base) exit(OVERFLOW);
		(*Q).queuesize += QUEUE_INCREMENT;//���Ӵ洢����
	}	
	(*Q).base[(*Q).rear].carnumber_ = carnum;
	(*Q).base[(*Q).rear].time_ = time_;
	(*Q).rear++; //�����Ԫ�أ���βָ��+1
}

Status DeQueue(SqQueue* Q, QElemType *e) {
	//�����в��գ���ɾ��Q�Ķ�ͷԪ�أ���e������ֵ��������OK�����򷵻�ERROR
	int i;
	if (Q->rear)//���в���
	{
		*e = *(*Q).base;
		for (i = 1; i < Q->rear; i++) {
			Q->base[i - 1] = Q->base[i];//һ��ǰ�ƶ���Ԫ��
		}
		Q->rear--;//βָ��ǰ��
		return OK;
	}
	else return ERROR;
}
//�õ������е�һ��Ԫ��
Status getFirst(SqQueue Q, QElemType *q) {
	if (Q.rear)//���в���
	{
		*q= *Q.base;
		return OK;
	}
	else {
		return ERROR;
	}
}

//ͣ��
//stateΪ״̬�����ﻹ����ȥ
void parkCar(int n,SqStack *S,SqQueue *Q,char state,int carnum,int time_) {
	SElemType e;
	SqStack S_temp;
	QElemType q;
	InitStack(&S_temp);
	//�������״̬ΪA�����ջ�Ƿ���������δ�������������ͣ��
	if (S->top - S->base<n&&state=='A') {
		Push(S, carnum, time_);
		printf("\n��ͣ�����е�λ��Ϊ:%d", S->top-S->base);
		return;
	}
	//�������״̬ΪA�����ջ�Ƿ��������������򽫳�ͣ�����
	else if (S->top - S->base >= n && state == 'A') {
		EnQueue(Q, carnum, time_);
		printf("\n�ڶ����е�λ��Ϊ:%d", Q->rear);
		return;
	}
	//�������״̬ΪD�����ջ���Ƿ��Ѿ����ڸó��������ڽ�������
	else if (state == 'D') {
		while (Pop(S, &e)) {
			if (e.carnumber == carnum) {
				printf("\n��ͣ������ͣ����ʱ��Ϊ:%d", time_ - e.time);
				printf("\nӦ���ɵ�ͣ����:%d\n",(time_ - e.time)*2);
				break;
			}
			//�ڼ����Ҫ�����ĳ��Ƿ���ͣ������ʱ�������г�Pop��������Push����һ��ջ����ʱͣ��Ϊ��Ҫ��ȥ��������·����ͣ�����˳���������
			Push(&S_temp, e.carnumber, e.time);
		}
		//��S_tempջ����ʱͣ��Ϊ��Ҫ��ȥ��������·����ͣ�����˳���������һ��������������Push��ͣ����ջ
		while (Pop(&S_temp, &e)) {
			Push(S, e.carnumber, e.time);
		}
		//�ж϶������Ƿ��г�
		if (!getFirst(*Q, &q)) return;
		//��������еĵ�һ������Push��ͣ����ջ�����ڶ�����ɾ�������г�ջ����
		if (Push(S,q.carnumber_,time_)) {
			DeQueue(Q, &q);
		}
		return;
	}
	else if (state == 'E') { exit(0); }
}
int main() {
	SqStack S;
	InitStack(&S);
	SqQueue Q;
	InitQueue(&Q);
	//�趨ͣ������λ����
	int n_;
	printf("��ͣ������λ����:");
	scanf("%d", &n_);
	getchar();
	//�����������������ȥ����Ϣ���������պ��뼰�������ȥ��ʱ��
	char state_;
	int carnum_;
	int time_;
	while(1){
		printf("\n����������:");
		scanf("%c %d %d", &state_, &carnum_, &time_);
		getchar();
		parkCar(n_, &S, &Q, state_, carnum_, time_);
	}
	return 0;
}