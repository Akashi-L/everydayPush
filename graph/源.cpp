#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"queue.h"
#include"adjList.h"

int main() {
	//无向图
	ALGraph G1;
	//DFS指定起点编号为n1,BFS指定起点编号为m1
	int n1;
	int m1;
	printf("创建无向图(边的输入格式为:顶点i 顶点j)\n");
	//创建无向图
	CreateALGraph(&G1);
	//DFS
	printf("输入指定的起点:");
	scanf("%d", &n1);
	printf("\n图的深度优先遍历为:");
	DFSTraverse(G1, n1);
	//DFS生成树边集
	printf("\nDFS生成树的边集:");
	DFSTraverseTree(G1, n1);
	//BFS
	printf("\n输入指定的起点:");
	scanf("%d", &m1);
	printf("\n图的广度优先遍历为:");
	BFSTraverse(G1, m1);
	//BFS生成树边集
	printf("\nBFS生成树的边集:");
	BFSTraverseTree(G1, m1);
	//使用栈非递归实现DFS
	printf("\n栈实现深度优先遍历:");
	DFSTraverseStack(G1);
	//有向图
	ALGraph G2;
	//创建有向图
	printf("\n创建有向图，注意边上两顶点的输入顺序,(vi,vj)表示的边为从顶点i指向顶点j");
	CreateALGraph_D(&G2);
	//DFS指定起点编号为n2,BFS指定起点编号为m2
	int n2;
	int m2;
	//DFS
	printf("\n输入指定的起点:");
	scanf("%d", &n2);
	printf("\n图的深度优先遍历为:");
	DFSTraverse(G2, n2);
	printf("\n输入指定的起点:");
	scanf("%d", &m2);
	//BFS
	printf("\n图的广度优先遍历为:");
	BFSTraverse(G2, m2);
	system("pause");
	return 0;
}