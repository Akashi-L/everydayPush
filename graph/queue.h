#pragma once
/** 队列定义及相关操作(广度遍历会用到此循环队列) **/
#define MaxVex 30

typedef struct LoopQueue {
	int data[MaxVex];
	int front, rear;
}LoopQueue, *Queue; //队列结构

//初始化队列
void initQueue(Queue *Q);

//判空
int QueueEmpty(Queue Q);

//判满
int QueueFull(Queue Q);

//队尾插入元素
void EnQueue(Queue *Q, int e);

//队头删除元素
void DeQueue(Queue *Q, int *e);
