#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include "adjList.h"
#include"queue.h"
#include"sqStack.h"

#define MaxVerNum 30 //��󶥵���Ϊ30
#define  InfoType  int
#define VertexType int

//�ж����еĽ���Ƿ񶼱����ʵı�־����
int visited[MaxVerNum];

//����ͼ(AL����ͼ)
void CreateALGraph(ALGraph *G) {
	int i, j, k;
	if (G == NULL) {
		G = (ALGraph*)malloc(sizeof(ALGraph));
	}
	printf("����ͼ�Ľ�����Լ�����:");
	scanf("%d%d", &G->vernum, &G->arcnum);
	rewind(stdin);
	//���붥����Ϣ
	printf("����������������:\n");
	for (int i = 1; i <= G->vernum; ++i) {
		printf("����%d:", i);
		scanf("%d", &(G->vertices[i].vertex));
		G->vertices[i].firstedge = NULL;
		rewind(stdin);
	}

	for (int k = 0; k < G->arcnum; ++k) {
		printf("����(vi,vj)�϶������:");
		scanf("%d%d", &i, &j);
		//������
		ArcNode*ptr = (ArcNode*)malloc(sizeof(ArcNode));
		//���ö���i���ڽӱ�
		ptr->adjvex = j;
		ptr->next = G->vertices[i].firstedge;
		G->vertices[i].firstedge = ptr;

		//����j���ڽӱ�
		ptr = (ArcNode*)malloc(sizeof(ArcNode));
		ptr->adjvex = i;
		ptr->next = G->vertices[j].firstedge;
		G->vertices[j].firstedge = ptr;
	}
}


//����G�е�V������ĵ�һ���ڽӵ���ţ�1,2,3...)�����v���ڽӵ㣬����0
int FirstAdjVex(ALGraph G, int v) {
	if (!G.vertices[v].firstedge) return 0;  //???�Ǳ߱�ͷָ����
	else return(G.vertices[v].firstedge->adjvex);
}

//����G�е�V�����������ڶ���W����һ���ڽӵ�����
//���V������ڶ���W����һ���ڽӵ㣬����0
//WΪV�ڽӱ��е�ĳ���ڽӵ㣬�ö���Ŀǰ�ձ�����
int NextAdjVex(ALGraph G, int v, int w) {
	//���������ڽӱ��е��ڽӵ�
	ArcNode* p;
	p = G.vertices[v].firstedge;
	while (p&&p->adjvex != w) p = p->next;
	if (p->adjvex == w && p->next) return(p->next->adjvex);
	else return 0;
}

//���ʺ���������ִ�д�ӡ�����ŵĲ���
void VISIT(int v) {
	printf("%d", v);//???
}

//����G�е�V������
void DFS(ALGraph G, int v) {
	visited[v] = 1;
	//���ʲ���
	VISIT(v);
	for (int w = FirstAdjVex(G, v); w > 0; w = NextAdjVex(G, v, w)) {
		if (!visited[w]) DFS(G, w);
	}
}

//DFS
//�ӽ��n��ʼ����
void DFSTraverse(ALGraph G, int n) {
	//��ʼ��visit����
	for (int k = 1; k <= G.vernum; ++k) visited[k] = 0;
	//DFS
	for (int k = n; k <= n + G.vernum; ++k) {
		int v;
		//Ϊ�˷��������vertices[0]��û�д���Ӧ�����ݣ�������Ҫ���дβ���������ʹ�õ�xertices[0]
		//vΪ��ʱ��Ҫ�жϵĶ���ı��
		v = k - 1;
		v = v % G.vernum;
		v = v + 1;
		if (!visited[v]) { DFS(G, v); }
	}
}

//BFS
//���nΪ���
void BFSTraverse(ALGraph G, int n) {
	for (int v = 1; v <= G.vernum; ++v) visited[v] = 0;
	Queue Q;
	initQueue(&Q);
	int u;
	for (int k = n; k <= n + G.vernum; ++k) {
		int v;
		//Ϊ�˷��������vertices[0]��û�д���Ӧ�����ݣ�������Ҫ���дβ���������ʹ�õ�xertices[0]
		//vΪ��ʱ��Ҫ�жϵĶ���ı��
		v = k - 1;
		v = v % G.vernum;
		v = v + 1;
		if (!visited[v]) {
			visited[v] = 1;
			VISIT(v);
			//�������vδ�����ʹ������
			EnQueue(&Q, v);
			while (!QueueEmpty(Q)) {
				//���в�Ϊ������ӣ�������δ�����ʵ��ڽӵ����
				DeQueue(&Q, &u);
				for (int w = FirstAdjVex(G, u); w > 0; w = NextAdjVex(G, u, w)) {
					if (!visited[w]) {
						visited[w] = 1;
						VISIT(w);
						EnQueue(&Q, w);
					}
				}
			}//end while
		}//end if
	}
}


//��ӡ�������ı߼�
void DFSTree(ALGraph G, int v) {
	visited[v] = 1;
	//���ʲ���
	for (int w = FirstAdjVex(G, v); w > 0; w = NextAdjVex(G, v, w)) {
		if (!visited[w])
		{
			//wΪv���ڽӵ㣬�������ı�Ϊ��һ�����ʵĶ����뱾����֮��ı�
			printf("%d-%d ", v, w);
			DFSTree(G, w);
		}
	}
}

//DFS
//�ӽ��n��ʼ����
void DFSTraverseTree(ALGraph G, int n) {
	//��ʼ��visit����
	for (int k = 1; k <= G.vernum; ++k) visited[k] = 0;
	//DFS
	for (int k = n; k <= n + G.vernum; ++k) {
		int v;
		v = k - 1;
		v = v % G.vernum;
		v = v + 1;
		if (!visited[v]) { DFSTree(G, v); }
	}
}

//BFS
//���nΪ���
void BFSTraverseTree(ALGraph G, int n) {
	for (int v = 1; v <= G.vernum; ++v) visited[v] = 0;
	Queue Q;
	initQueue(&Q);
	int u;
	for (int k = n; k <= n + G.vernum; ++k) {
		int v;
		v = k - 1;
		v = v % G.vernum;
		v = v + 1;
		if (!visited[v]) {
			visited[v] = 1;
			EnQueue(&Q, v);
			while (!QueueEmpty(Q)) {
				DeQueue(&Q, &u);
				for (int w = FirstAdjVex(G, u); w > 0; w = NextAdjVex(G, u, w)) {
					if (!visited[w]) {
						visited[w] = 1;
						printf("%d-%d ", u, w);
						EnQueue(&Q, w);
					}
				}
			}//end while
		}//end if
	}
}

//ʹ��ջ�ǵݹ�ʵ��������ȱ���
void DFSTraverseStack(ALGraph G) {
	//��ʼ��visit����
	for (int k = 1; k <= G.vernum; ++k) visited[k] = 0;
	//�����Ϊ1�Ķ���ѹջ�����Ӷ���1��ʼ����
	int e = 1;
	int m;
	//ջ
	SqStack *s = (SqStack*)malloc(sizeof(SqStack));
	InitStack(s);
	Push(s, e);
	//��ջ��Ϊ��ʱ
	while (!sqStackEmpty(s)) {
		//��ջ
		Pop(s, &m);
		//����ջ�Ķ����Ѿ������ʹ����򲻴�ӡ
		if (!visited[m]) {
			//��ջ�Ķ���δ�����ʹ���visited��־��1�����Ҵ�ӡ�ö���
			visited[m] = 1;
			printf("%d ", m);
			//����ջ������ڽӱ�������δ�����ʵĶ���ѹջ
			for (int w = FirstAdjVex(G, m); w > 0; w = NextAdjVex(G, m, w)) {
				//δ������
				if (visited[w] == 0) {
					Push(s, w);
				}
			}
		}
		else continue;
	}
}

//����ͼ(AL����ͼ)
void CreateALGraph_D(ALGraph *G) {
	int i, j, k;
	if (G == NULL) {
		G = (ALGraph*)malloc(sizeof(ALGraph));
	}
	printf("����ͼ�Ľ�����Լ�����:");
	scanf("%d%d", &G->vernum, &G->arcnum);
	rewind(stdin);

	printf("����������������:\n");
	for (int i = 1; i <= G->vernum; ++i) {
		printf("����%d:", i);
		scanf("%d", &(G->vertices[i].vertex));
		G->vertices[i].firstedge = NULL;
		rewind(stdin);
	}

	for (int k = 0; k < G->arcnum; ++k) {
		printf("����(vi,vj)�϶������:");
		scanf("%d%d", &i, &j);
		//������
		ArcNode*ptr = (ArcNode*)malloc(sizeof(ArcNode));
		//���ö���i���ڽӱ�
		ptr->adjvex = j;
		ptr->next = G->vertices[i].firstedge;
		G->vertices[i].firstedge = ptr;
	}
}