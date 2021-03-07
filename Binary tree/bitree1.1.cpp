#include <stdio.h>
#include <stdlib.h>
#include"bitree.h"

int main()
{
	//声明树T
	BiTree T;
	//初始化
	InitBiTree(&T);
	//以先序输入的方式创建树
	CreatBiTree(&T);
	printf("前序：\n");
	//先序遍历
	PreOrderTraverse(T, PrintElement);
	printf("\n");
	//中序遍历
	printf("中序：\n");
	InOrderTraverse(T, PrintElement);
	printf("\n");
	//后序遍历
	printf("后序：\n");
	PostOrderTraverse(T, PrintElement);
	getchar();
	system("pause");
	getchar();
	return 0;
}
