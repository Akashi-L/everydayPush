#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"queue.h"
#include"sqStack.h"

#define MaxVerNum 30 //��󶥵���Ϊ30
typedef int InfoType;
typedef int VertexType;

//�ڽӱ�����
typedef struct ArcNode {
	int adjvex; //�ڽӵ���(���ڽӵ��ڶ��������е��±�)
	InfoType *Info; //Ȩ��
	struct ArcNode* next = NULL; //ָ����һ���ڽӵ��ָ����
}ArcNode;//�߱���

//��ͷ�������
typedef struct Vnode {
	VertexType vertex; //������(������Ϣ)
	ArcNode* firstedge; //�߱�ͷָ��
}Vnode, AdjList[MaxVerNum];

//ͼ������
typedef struct {
	AdjList vertices;//�ڽӱ�
	int vernum, arcnum;//�������ͱ���
}ALGraph, *GraphAdjList;

//����ͼ(AL����ͼ)
void CreateALGraph(ALGraph *G);

//����G�е�V������ĵ�һ���ڽӵ���ţ�1,2,3...)�����v���ڽӵ㣬����0
int FirstAdjVex(ALGraph G, int v);

//����G�е�V�����������ڶ���W����һ���ڽӵ�����
//���V������ڶ���W����һ���ڽӵ㣬����0
//WΪV�ڽӱ��е�ĳ���ڽӵ㣬�ö���Ŀǰ�ձ�����
int NextAdjVex(ALGraph G, int v, int w);

//���ʺ���������ִ�д�ӡ�����ŵĲ���
void VISIT(int v);

//����G�е�V������
void DFS(ALGraph G, int v);

//DFS
//�ӽ��n��ʼ����
void DFSTraverse(ALGraph G, int n);

//BFS
//���nΪ���
void BFSTraverse(ALGraph G, int n);

//��ӡ�������ı߼�
void DFSTree(ALGraph G, int v);

//DFS
//�ӽ��n��ʼ����
void DFSTraverseTree(ALGraph G, int n);

//BFS
//���nΪ���
void BFSTraverseTree(ALGraph G, int n);

//ʹ��ջ�ǵݹ�ʵ��������ȱ���
void DFSTraverseStack(ALGraph G);

//����ͼ(AL����ͼ)
void CreateALGraph_D(ALGraph *G);
