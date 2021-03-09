#include<stdio.h>
#include<stdlib.h>
#include"queue.h"

#define MaxVex 30
//队列定义及相关操作(循环队列) 

//初始化
void initQueue(Queue *Q) {
	*Q = (Queue)malloc(sizeof(LoopQueue));
	(*Q)->front = (*Q)->rear = 0;
}

//判空
int QueueEmpty(Queue Q) {
	if (Q->front == Q->rear) {
		return 1;
	}
	else {
		return 0;
	}
}

//判满
int QueueFull(Queue Q) {
	if ((Q->rear + 1) % MaxVex == Q->front) {
		return 1;
	}
	else {
		return 0;
	}
}

//队尾插入元素
void EnQueue(Queue *Q, int e) {
	if (!QueueFull(*Q)) {
		(*Q)->data[(*Q)->rear] = e;
		(*Q)->rear = ((*Q)->rear + 1) % MaxVex;
	}
}

//队头删除元素
void DeQueue(Queue *Q, int *e) {
	if (!QueueEmpty(*Q)) {
		*e = (*Q)->data[(*Q)->front];
		(*Q)->front = ((*Q)->front + 1) % MaxVex;
	}
}
