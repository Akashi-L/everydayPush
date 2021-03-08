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

//栈
Status InitStack(SqStack* S) {
	//创建空栈
	S->base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	//判断栈是否创建成功
	if (!S->base) exit(OVERFLOW);
	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;
	return OK;
}

Status Push(SqStack*S, int carnum_,int time_) {
	if (S->top - S->base >= S->stacksize) {
		//栈满，扩展空间
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
	//如果栈非空，则将top element赋值给e然后删除the top element
	if (S->top == S->base)return ERROR;
	*e = *--S->top;
	return OK;
}

//队列
#define QUEUE_INIT_SIZE 10//队列存储空间的初始分配量
#define QUEUE_INCREMENT 2//队列存储空间的分配增量
typedef struct {
	int carnumber_;
	int time_;
}QElemType;
struct SqQueue {
	QElemType*base;//初始化的动态分配存储空间
	int rear;//尾指针，若队列不空，指向队列尾元素的下一个位置
	int front;
	int queuesize;//当前分配的存储容量
};
//分配队列存储空间
Status InitQueue(SqQueue *Q) {
	Q->base = (QElemType*)malloc(QUEUE_INIT_SIZE * sizeof(QElemType));
	if (!Q->base) exit(OVERFLOW);
	Q->front = Q->rear = 0;
	return OK;
}

//插入元素e为Q的新的队尾元素
void EnQueue(SqQueue* Q, int carnum,int time_) {
	if ((*Q).rear == (*Q).queuesize)//当前存储空间已满
	{
		(*Q).base = (QElemType*)realloc((*Q).base, ((*Q).queuesize + QUEUE_INCREMENT) * sizeof(QElemType));
		//分配失败
		if (!(*Q).base) exit(OVERFLOW);
		(*Q).queuesize += QUEUE_INCREMENT;//增加存储容量
	}	
	(*Q).base[(*Q).rear].carnumber_ = carnum;
	(*Q).base[(*Q).rear].time_ = time_;
	(*Q).rear++; //入队新元素，队尾指针+1
}

Status DeQueue(SqQueue* Q, QElemType *e) {
	//若队列不空，则删除Q的队头元素，用e返回其值，并返回OK；否则返回ERROR
	int i;
	if (Q->rear)//队列不空
	{
		*e = *(*Q).base;
		for (i = 1; i < Q->rear; i++) {
			Q->base[i - 1] = Q->base[i];//一次前移队列元素
		}
		Q->rear--;//尾指针前移
		return OK;
	}
	else return ERROR;
}
//得到队列中第一个元素
Status getFirst(SqQueue Q, QElemType *q) {
	if (Q.rear)//队列不空
	{
		*q= *Q.base;
		return OK;
	}
	else {
		return ERROR;
	}
}

//停车
//state为状态，到达还是离去
void parkCar(int n,SqStack *S,SqQueue *Q,char state,int carnum,int time_) {
	SElemType e;
	SqStack S_temp;
	QElemType q;
	InitStack(&S_temp);
	//如果读入状态为A，检查栈是否已满，若未满则可以像其中停车
	if (S->top - S->base<n&&state=='A') {
		Push(S, carnum, time_);
		printf("\n在停车场中的位置为:%d", S->top-S->base);
		return;
	}
	//如果读入状态为A，检查栈是否已满，若已满则将车停入队列
	else if (S->top - S->base >= n && state == 'A') {
		EnQueue(Q, carnum, time_);
		printf("\n在队列中的位置为:%d", Q->rear);
		return;
	}
	//如果读入状态为D，检查栈中是否已经存在该车，若存在将车开出
	else if (state == 'D') {
		while (Pop(S, &e)) {
			if (e.carnumber == carnum) {
				printf("\n在停车场中停留的时间为:%d", time_ - e.time);
				printf("\n应缴纳的停车费:%d\n",(time_ - e.time)*2);
				break;
			}
			//在检查需要开车的车是否在停车场中时，将所有车Pop出来并且Push到另一个栈中临时停放为给要离去的汽车让路而从停车场退出来的汽车
			Push(&S_temp, e.carnumber, e.time);
		}
		//将S_temp栈中临时停放为给要离去的汽车让路而从停车场退出来的汽车一个个弹出，并且Push回停车场栈
		while (Pop(&S_temp, &e)) {
			Push(S, e.carnumber, e.time);
		}
		//判断队列中是否有车
		if (!getFirst(*Q, &q)) return;
		//如果对列中的第一个车能Push入停车场栈则将其在队列中删除，进行出栈操作
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
	//设定停车场车位数量
	int n_;
	printf("请停车场车位数量:");
	scanf("%d", &n_);
	getchar();
	//输入汽车“到达”或“离去”信息、汽车牌照号码及到达或离去的时刻
	char state_;
	int carnum_;
	int time_;
	while(1){
		printf("\n请输入数据:");
		scanf("%c %d %d", &state_, &carnum_, &time_);
		getchar();
		parkCar(n_, &S, &Q, state_, carnum_, time_);
	}
	return 0;
}