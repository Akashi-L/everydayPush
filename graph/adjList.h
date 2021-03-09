#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"queue.h"
#include"sqStack.h"

#define MaxVerNum 30 //最大顶点数为30
typedef int InfoType;
typedef int VertexType;

//邻接表类型
typedef struct ArcNode {
	int adjvex; //邻接点域(该邻接点在顶点数组中的下标)
	InfoType *Info; //权重
	struct ArcNode* next = NULL; //指向下一个邻接点的指针域
}ArcNode;//边表结点

//表头结点类型
typedef struct Vnode {
	VertexType vertex; //顶点域(顶点信息)
	ArcNode* firstedge; //边表头指针
}Vnode, AdjList[MaxVerNum];

//图的类型
typedef struct {
	AdjList vertices;//邻接表
	int vernum, arcnum;//顶点数和边数
}ALGraph, *GraphAdjList;

//创建图(AL无向图)
void CreateALGraph(ALGraph *G);

//返回G中第V个顶点的第一个邻接点序号（1,2,3...)。如果v无邻接点，返回0
int FirstAdjVex(ALGraph G, int v);

//返回G中第V个顶点的相对于顶点W的下一个邻接点的序号
//如果V无相对于顶点W的下一个邻接点，返回0
//W为V邻接表中的某个邻接点，该顶点目前刚被访问
int NextAdjVex(ALGraph G, int v, int w);

//访问函数，这里执行打印顶点编号的操作
void VISIT(int v);

//访问G中第V个顶点
void DFS(ALGraph G, int v);

//DFS
//从结点n开始遍历
void DFSTraverse(ALGraph G, int n);

//BFS
//结点n为起点
void BFSTraverse(ALGraph G, int n);

//打印生成树的边集
void DFSTree(ALGraph G, int v);

//DFS
//从结点n开始遍历
void DFSTraverseTree(ALGraph G, int n);

//BFS
//结点n为起点
void BFSTraverseTree(ALGraph G, int n);

//使用栈非递归实现深度优先遍历
void DFSTraverseStack(ALGraph G);

//创建图(AL有向图)
void CreateALGraph_D(ALGraph *G);
