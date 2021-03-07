#ifndef BITREE_H_INCLUDED
#define BITREE_H_INCLUDED

#define OK 1
#define OVERFLOW -2
#define ERROR -1

typedef int Status;
typedef char TElemType;
typedef struct BiTNode {
	TElemType data;
	struct BiTNode* lchild;
	struct BiTNode* rchild;
}BiTNode, * BiTree;

//初始化
Status InitBiTree(BiTree* T) {
	*T = (BiTree)malloc(sizeof(BiTNode));
	(*T)->lchild = NULL;
	(*T)->rchild = NULL;
	return OK;
}

//以先序输入的方式创建树
Status CreatBiTree(BiTree* T) {
	char c;
	scanf_s("%c", &c);
	getchar();
	//如果输入空，则没有结点
	if (c == ' ') {
		*T = NULL;
	}
	else {
		//结点非空
		if (!*T) {
			*T = (BiTree)malloc(sizeof(BiTNode));
			if (!*T) return ERROR;
			(*T)->lchild = NULL;
			(*T)->rchild = NULL;
		}
		(*T)->data = c;
		//创建左右子树
		CreatBiTree(&(*T)->lchild);
		CreatBiTree(&(*T)->rchild);
	}
	return OK;
}

//打印结点内容
Status PrintElement(TElemType e) {
	printf("%c ", e);
	return OK;
}

//先序
Status PreOrderTraverse(BiTree T, Status Visit(TElemType)) {
	if (T) {
		if (Visit(T->data))
			//顺序为先根结点再左子树再右子树
			if (PreOrderTraverse(T->lchild, PrintElement))
				if (PreOrderTraverse(T->rchild, PrintElement))   return OK;
		return ERROR;
	}
	else return OK;
}

//中序
Status InOrderTraverse(BiTree T, Status Visit(TElemType)) {
	if (T) {
		//顺序为先左子树再根结点再右子树
		if (InOrderTraverse(T->lchild, PrintElement))
			if (Visit(T->data))
				if (InOrderTraverse(T->rchild, PrintElement))    return OK;
		return ERROR;
	}
	else return OK;
}

//后序
Status PostOrderTraverse(BiTree T, Status(*Visit)(TElemType)) {
	if (T) {
		//顺序为先左子树再右子树再根结点
		if (PostOrderTraverse(T->lchild, PrintElement))
			if (PostOrderTraverse(T->rchild, PrintElement))
				if (Visit(T->data))  return OK;
		return ERROR;
	}
	else return OK;
}

#endif // BITREE_H_INCLUDED
