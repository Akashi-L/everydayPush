#pragma once
/** ���ж��弰��ز���(��ȱ������õ���ѭ������) **/
#define MaxVex 30

typedef struct LoopQueue {
	int data[MaxVex];
	int front, rear;
}LoopQueue, *Queue; //���нṹ

//��ʼ������
void initQueue(Queue *Q);

//�п�
int QueueEmpty(Queue Q);

//����
int QueueFull(Queue Q);

//��β����Ԫ��
void EnQueue(Queue *Q, int e);

//��ͷɾ��Ԫ��
void DeQueue(Queue *Q, int *e);
