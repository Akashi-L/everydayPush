#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

typedef int Status; //状态
typedef  int KeyType; //关键字类型
#define  FALSE 0
#define  TRUE 1
#define  OVERFLOW -2
#define  ERROR -1
#define  OK 1

#define EQ(a,b) ((a)==(b))
#define LT(a,b) ((a)<(b))
#define LQ(a,b) ((a)<=(b))

//二叉树
typedef struct BiTreeNode {
	KeyType data;//数据(在这里数据当作key来使用)
	BiTreeNode* lchild, *rchild;//左右孩子指针
}BiTreeNode, *BiTree;
Status InsertBST(BiTree *, KeyType );

//创建二叉排序树
void creatTree(BiTree* B ) {
	printf("输入结点数:\n");
	int nodeN;
	scanf("%d",&nodeN);
	for (int i = 0; i < nodeN; i++) {
		int dataN;
		printf("输入第%d个数据:", i + 1);
		scanf("%d", &dataN);
		InsertBST(B, dataN);
	}
}

int length ;
//二叉排序树查找
int SearchBST(BiTree T, KeyType key, BiTree f, BiTree *p) {
	//在根指针T所指二叉排序树中递归得查找其关键字等于key的数据元素，若查找成功
	//则指针p指向该数据元素的结点，并返回true，否则指针p指向查找路径上访问的
	//最后一个结点并返回false，指针f指向T的双亲，其初始调用值为null
	//查找不成功
	if (!T) {
		*p = f;
		//printf("查找失败\n");
		
		return FALSE;
	}
	//查找成功
	else if (EQ(key, T->data)) {
		*p = T;
		length++;
		//*pathLength = length;
		//printf("查找成功\n");
		return TRUE;
	}
	//在左子树中继续查找
	else if (LT(key, T->data)) { length++;  SearchBST(T->lchild, key, T, p); }
	//在右子树中继续查找
	else { length++;  SearchBST(T->rchild, key, T, p); }
}


//遍历
//打印结点内容
Status PrintElement(KeyType e) {
	printf("%d ", e);
	return OK;
}

//先序
Status PreOrderTraverse(BiTree T, Status Visit(KeyType)) {
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
Status InOrderTraverse(BiTree T, Status Visit(KeyType)) {
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
Status PostOrderTraverse(BiTree T, Status(*Visit)(KeyType)) {
	if (T) {
		//顺序为先左子树再右子树再根结点
		if (PostOrderTraverse(T->lchild, PrintElement))
			if (PostOrderTraverse(T->rchild, PrintElement))
				if (Visit(T->data))  return OK;
		return ERROR;
	}
	else return OK;
}

//插入
Status InsertBST(BiTree *T, KeyType e) {
	//!!!!!
	BiTree test = NULL;
	BiTree* p = &test;//???
	//当二叉排序树T中不存在关键字等于e.key的数据元素时，插入e并返回True，否则返回False
	if (!(SearchBST((*T), e, NULL, p))) {
		BiTree s=NULL;
		s = (BiTree)malloc(sizeof(BiTreeNode));
		s->data = e;
		s->lchild = s->rchild=NULL;
		if (!(*p)) *T = s;//被插结点*s为新的根节点    //改
		else if LT(e, (*p)->data) (*p)->lchild = s;//被插结点*s为左孩子
		else (*p)->rchild = s;//被插结点*s为右孩子
		return TRUE;
	}
	else return FALSE; //树中已有关键字相同的结点，不再插入
}

//删除 //p => (*p)
Status Delete(BiTree *p) {
	//从二叉排序树中删除结点p，并重接它的左或右子树
	BiTree q;
	//右子树空则只需重接它的左子树
	if (!(*p)->rchild) {
		q = *p;
		(*p) = (*p)->lchild;
		free(q);
	}
	//只需重接它的右子树
	else if (!(*p)->lchild) {
		q = *p;
		*p = (*p)->rchild;
		free(q);
	}
	//左右子树均不为空
	else {
		//!!!!!!!!!!!!!!!!!!!!!!!!!!
		BiTree *s = (BiTree*)malloc(sizeof(BiTree));
		q = *p;
		*s = (*p)->lchild;
		//转左，然后向右到尽头
		while ((*s)->rchild) {
			q = *s;
			*s = (*s)->rchild;
		}
		//s指向被删结点的“前驱”
		(*p)->data = (*s)->data;
		//重接*q的右子树
		if (q != *p) q->lchild = (*s)->lchild;
		//重接*q的左子树
		else q->lchild = (*s)->lchild;
		delete s;
		//free(*s);
	}
	return TRUE;
}


//删除
Status DeleteBST(BiTree *T, KeyType key) {
	//若二叉排序树T中存在关键字等于key的数据元素时，则删除该数据元素结点,并返回TRUE；否则返回FALSE

	//不存在关键字等于key的数据元素
	if (!*T) return FALSE;
	else {
		if (EQ(key, (*T)->data)) return Delete(T); //找到关键字等于key的数据元素
		else if (LT(key, (*T)->data)) return DeleteBST(&(*T)->lchild, key);
		else return DeleteBST(&(*T)->rchild, key);
	}
}

//格式化打印
//每个结点之前需要打印的空格,其中counter传入层数，结点在哪一层打印几个空格
Status PrintTreeElem(KeyType e, int counter) {
	for (int i = 0; i < counter; i++) {
		printf("  ");
	}
	printf("%d\n", e);
	return OK;
}
//按凹入表的形式打印出树
Status PrintTree(BiTree T, Status(*Visit)(KeyType, int), int counter) {
	//在printTree函数中counter++意味着，每调用一层则counter加一，即为层数加一
	counter++;
	if (T) {
		if (PrintTree(T->rchild, Visit, counter))
			if (Visit(T->data, counter))
				if (PrintTree(T->lchild, Visit, counter))    return OK;
		return ERROR;
	}
	else return OK;
}


int main() {
	BiTree b = NULL;
	BiTree*ptr = &b;

	//创建树
	creatTree(ptr);

	//打印树
	printf("按凹入表的形式打印出树:\n");
	PrintTree(b,PrintTreeElem, 0);

	//遍历
	printf("前序遍历:");
	PreOrderTraverse(b, PrintElement);
	printf("\n中序遍历:");
	InOrderTraverse(b, PrintElement);
	printf("\n后序遍历:");
	PostOrderTraverse(b, PrintElement);
	BiTree p; 
	length = 0;
	printf("\n请输入想要查找的数字:");

	//存储想要查找的数字
	int num;
	scanf("%d", &num);

	//是否找到
	int searchFlag ;
	searchFlag=SearchBST(b, num, NULL, &p);
	if (searchFlag == FALSE) {
		printf("查找失败\n");
	}
	else if (searchFlag = TRUE) {
		printf("查找成功\n");
	}

	//查找长度
	printf("查找长度为:%d\n", length);

	//删除操作
	printf("请输入想要删除的数字:");
	int deleteN;
	scanf("%d", &deleteN);
	DeleteBST(ptr, deleteN);
	printf("删除之后的树:\n");
	PrintTree(b, PrintTreeElem, 0);

	//插入操作
	printf("请输入想要插入的数字:");
	int insertN;
	scanf("%d", &insertN);
	InsertBST(ptr, insertN);
	printf("插入之后的树:\n");
	PrintTree(b, PrintTreeElem, 0);
	getchar();
	getchar();
	return 0;
}