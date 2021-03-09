#include<stdio.h>
#include<stdlib.h>
#include"queue.h"

#define MaxVex 30
//���ж��弰��ز���(ѭ������) 

//��ʼ��
void initQueue(Queue *Q) {
	*Q = (Queue)malloc(sizeof(LoopQueue));
	(*Q)->front = (*Q)->rear = 0;
}

//�п�
int QueueEmpty(Queue Q) {
	if (Q->front == Q->rear) {
		return 1;
	}
	else {
		return 0;
	}
}

//����
int QueueFull(Queue Q) {
	if ((Q->rear + 1) % MaxVex == Q->front) {
		return 1;
	}
	else {
		return 0;
	}
}

//��β����Ԫ��
void EnQueue(Queue *Q, int e) {
	if (!QueueFull(*Q)) {
		(*Q)->data[(*Q)->rear] = e;
		(*Q)->rear = ((*Q)->rear + 1) % MaxVex;
	}
}

//��ͷɾ��Ԫ��
void DeQueue(Queue *Q, int *e) {
	if (!QueueEmpty(*Q)) {
		*e = (*Q)->data[(*Q)->front];
		(*Q)->front = ((*Q)->front + 1) % MaxVex;
	}
}
