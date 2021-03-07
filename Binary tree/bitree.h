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

//��ʼ��
Status InitBiTree(BiTree* T) {
	*T = (BiTree)malloc(sizeof(BiTNode));
	(*T)->lchild = NULL;
	(*T)->rchild = NULL;
	return OK;
}

//����������ķ�ʽ������
Status CreatBiTree(BiTree* T) {
	char c;
	scanf_s("%c", &c);
	getchar();
	//�������գ���û�н��
	if (c == ' ') {
		*T = NULL;
	}
	else {
		//���ǿ�
		if (!*T) {
			*T = (BiTree)malloc(sizeof(BiTNode));
			if (!*T) return ERROR;
			(*T)->lchild = NULL;
			(*T)->rchild = NULL;
		}
		(*T)->data = c;
		//������������
		CreatBiTree(&(*T)->lchild);
		CreatBiTree(&(*T)->rchild);
	}
	return OK;
}

//��ӡ�������
Status PrintElement(TElemType e) {
	printf("%c ", e);
	return OK;
}

//����
Status PreOrderTraverse(BiTree T, Status Visit(TElemType)) {
	if (T) {
		if (Visit(T->data))
			//˳��Ϊ�ȸ��������������������
			if (PreOrderTraverse(T->lchild, PrintElement))
				if (PreOrderTraverse(T->rchild, PrintElement))   return OK;
		return ERROR;
	}
	else return OK;
}

//����
Status InOrderTraverse(BiTree T, Status Visit(TElemType)) {
	if (T) {
		//˳��Ϊ���������ٸ������������
		if (InOrderTraverse(T->lchild, PrintElement))
			if (Visit(T->data))
				if (InOrderTraverse(T->rchild, PrintElement))    return OK;
		return ERROR;
	}
	else return OK;
}

//����
Status PostOrderTraverse(BiTree T, Status(*Visit)(TElemType)) {
	if (T) {
		//˳��Ϊ�����������������ٸ����
		if (PostOrderTraverse(T->lchild, PrintElement))
			if (PostOrderTraverse(T->rchild, PrintElement))
				if (Visit(T->data))  return OK;
		return ERROR;
	}
	else return OK;
}

#endif // BITREE_H_INCLUDED
