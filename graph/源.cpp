#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"queue.h"
#include"adjList.h"

int main() {
	//����ͼ
	ALGraph G1;
	//DFSָ�������Ϊn1,BFSָ�������Ϊm1
	int n1;
	int m1;
	printf("��������ͼ(�ߵ������ʽΪ:����i ����j)\n");
	//��������ͼ
	CreateALGraph(&G1);
	//DFS
	printf("����ָ�������:");
	scanf("%d", &n1);
	printf("\nͼ��������ȱ���Ϊ:");
	DFSTraverse(G1, n1);
	//DFS�������߼�
	printf("\nDFS�������ı߼�:");
	DFSTraverseTree(G1, n1);
	//BFS
	printf("\n����ָ�������:");
	scanf("%d", &m1);
	printf("\nͼ�Ĺ�����ȱ���Ϊ:");
	BFSTraverse(G1, m1);
	//BFS�������߼�
	printf("\nBFS�������ı߼�:");
	BFSTraverseTree(G1, m1);
	//ʹ��ջ�ǵݹ�ʵ��DFS
	printf("\nջʵ��������ȱ���:");
	DFSTraverseStack(G1);
	//����ͼ
	ALGraph G2;
	//��������ͼ
	printf("\n��������ͼ��ע����������������˳��,(vi,vj)��ʾ�ı�Ϊ�Ӷ���iָ�򶥵�j");
	CreateALGraph_D(&G2);
	//DFSָ�������Ϊn2,BFSָ�������Ϊm2
	int n2;
	int m2;
	//DFS
	printf("\n����ָ�������:");
	scanf("%d", &n2);
	printf("\nͼ��������ȱ���Ϊ:");
	DFSTraverse(G2, n2);
	printf("\n����ָ�������:");
	scanf("%d", &m2);
	//BFS
	printf("\nͼ�Ĺ�����ȱ���Ϊ:");
	BFSTraverse(G2, m2);
	system("pause");
	return 0;
}