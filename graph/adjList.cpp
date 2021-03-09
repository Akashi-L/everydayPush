#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include "adjList.h"
#include"queue.h"
#include"sqStack.h"

#define MaxVerNum 30 //最大顶点数为30
#define  InfoType  int
#define VertexType int

//判断所有的结点是否都被访问的标志数组
int visited[MaxVerNum];

//创建图(AL无向图)
void CreateALGraph(ALGraph *G) {
	int i, j, k;
	if (G == NULL) {
		G = (ALGraph*)malloc(sizeof(ALGraph));
	}
	printf("输入图的结点数以及边数:");
	scanf("%d%d", &G->vernum, &G->arcnum);
	rewind(stdin);
	//输入顶点信息
	printf("输入各个顶点的数据:\n");
	for (int i = 1; i <= G->vernum; ++i) {
		printf("顶点%d:", i);
		scanf("%d", &(G->vertices[i].vertex));
		G->vertices[i].firstedge = NULL;
		rewind(stdin);
	}

	for (int k = 0; k < G->arcnum; ++k) {
		printf("输入(vi,vj)上顶点序号:");
		scanf("%d%d", &i, &j);
		//插入结点
		ArcNode*ptr = (ArcNode*)malloc(sizeof(ArcNode));
		//设置顶点i的邻接表
		ptr->adjvex = j;
		ptr->next = G->vertices[i].firstedge;
		G->vertices[i].firstedge = ptr;

		//设置j的邻接表
		ptr = (ArcNode*)malloc(sizeof(ArcNode));
		ptr->adjvex = i;
		ptr->next = G->vertices[j].firstedge;
		G->vertices[j].firstedge = ptr;
	}
}


//返回G中第V个顶点的第一个邻接点序号（1,2,3...)。如果v无邻接点，返回0
int FirstAdjVex(ALGraph G, int v) {
	if (!G.vertices[v].firstedge) return 0;  //???是边表头指针吗
	else return(G.vertices[v].firstedge->adjvex);
}

//返回G中第V个顶点的相对于顶点W的下一个邻接点的序号
//如果V无相对于顶点W的下一个邻接点，返回0
//W为V邻接表中的某个邻接点，该顶点目前刚被访问
int NextAdjVex(ALGraph G, int v, int w) {
	//用来遍历邻接表中的邻接点
	ArcNode* p;
	p = G.vertices[v].firstedge;
	while (p&&p->adjvex != w) p = p->next;
	if (p->adjvex == w && p->next) return(p->next->adjvex);
	else return 0;
}

//访问函数，这里执行打印顶点编号的操作
void VISIT(int v) {
	printf("%d", v);//???
}

//访问G中第V个顶点
void DFS(ALGraph G, int v) {
	visited[v] = 1;
	//访问操作
	VISIT(v);
	for (int w = FirstAdjVex(G, v); w > 0; w = NextAdjVex(G, v, w)) {
		if (!visited[w]) DFS(G, w);
	}
}

//DFS
//从结点n开始遍历
void DFSTraverse(ALGraph G, int n) {
	//初始化visit数组
	for (int k = 1; k <= G.vernum; ++k) visited[k] = 0;
	//DFS
	for (int k = n; k <= n + G.vernum; ++k) {
		int v;
		//为了方便起见，vertices[0]中没有存相应的数据，所以需要进行次操作，避免使用到xertices[0]
		//v为此时需要判断的顶点的编号
		v = k - 1;
		v = v % G.vernum;
		v = v + 1;
		if (!visited[v]) { DFS(G, v); }
	}
}

//BFS
//结点n为起点
void BFSTraverse(ALGraph G, int n) {
	for (int v = 1; v <= G.vernum; ++v) visited[v] = 0;
	Queue Q;
	initQueue(&Q);
	int u;
	for (int k = n; k <= n + G.vernum; ++k) {
		int v;
		//为了方便起见，vertices[0]中没有存相应的数据，所以需要进行次操作，避免使用到xertices[0]
		//v为此时需要判断的顶点的编号
		v = k - 1;
		v = v % G.vernum;
		v = v + 1;
		if (!visited[v]) {
			visited[v] = 1;
			VISIT(v);
			//如果顶点v未被访问过则入队
			EnQueue(&Q, v);
			while (!QueueEmpty(Q)) {
				//队列不为空则出队，并将其未被访问的邻接点入队
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


//打印生成树的边集
void DFSTree(ALGraph G, int v) {
	visited[v] = 1;
	//访问操作
	for (int w = FirstAdjVex(G, v); w > 0; w = NextAdjVex(G, v, w)) {
		if (!visited[w])
		{
			//w为v的邻接点，生成树的边为上一个访问的顶点与本顶点之间的边
			printf("%d-%d ", v, w);
			DFSTree(G, w);
		}
	}
}

//DFS
//从结点n开始遍历
void DFSTraverseTree(ALGraph G, int n) {
	//初始化visit数组
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
//结点n为起点
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

//使用栈非递归实现深度优先遍历
void DFSTraverseStack(ALGraph G) {
	//初始化visit数组
	for (int k = 1; k <= G.vernum; ++k) visited[k] = 0;
	//将编号为1的顶点压栈，即从顶点1开始遍历
	int e = 1;
	int m;
	//栈
	SqStack *s = (SqStack*)malloc(sizeof(SqStack));
	InitStack(s);
	Push(s, e);
	//当栈不为空时
	while (!sqStackEmpty(s)) {
		//出栈
		Pop(s, &m);
		//若出栈的顶点已经被访问过了则不打印
		if (!visited[m]) {
			//出栈的顶点未被访问过则将visited标志置1，并且打印该顶点
			visited[m] = 1;
			printf("%d ", m);
			//将出栈顶点的邻接表中所有未被访问的顶点压栈
			for (int w = FirstAdjVex(G, m); w > 0; w = NextAdjVex(G, m, w)) {
				//未被访问
				if (visited[w] == 0) {
					Push(s, w);
				}
			}
		}
		else continue;
	}
}

//创建图(AL有向图)
void CreateALGraph_D(ALGraph *G) {
	int i, j, k;
	if (G == NULL) {
		G = (ALGraph*)malloc(sizeof(ALGraph));
	}
	printf("输入图的结点数以及边数:");
	scanf("%d%d", &G->vernum, &G->arcnum);
	rewind(stdin);

	printf("输入各个顶点的数据:\n");
	for (int i = 1; i <= G->vernum; ++i) {
		printf("顶点%d:", i);
		scanf("%d", &(G->vertices[i].vertex));
		G->vertices[i].firstedge = NULL;
		rewind(stdin);
	}

	for (int k = 0; k < G->arcnum; ++k) {
		printf("输入(vi,vj)上顶点序号:");
		scanf("%d%d", &i, &j);
		//插入结点
		ArcNode*ptr = (ArcNode*)malloc(sizeof(ArcNode));
		//设置顶点i的邻接表
		ptr->adjvex = j;
		ptr->next = G->vertices[i].firstedge;
		G->vertices[i].firstedge = ptr;
	}
}