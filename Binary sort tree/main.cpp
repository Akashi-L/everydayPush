#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

typedef int Status; //״̬
typedef  int KeyType; //�ؼ�������
#define  FALSE 0
#define  TRUE 1
#define  OVERFLOW -2
#define  ERROR -1
#define  OK 1

#define EQ(a,b) ((a)==(b))
#define LT(a,b) ((a)<(b))
#define LQ(a,b) ((a)<=(b))

//������
typedef struct BiTreeNode {
	KeyType data;//����(���������ݵ���key��ʹ��)
	BiTreeNode* lchild, *rchild;//���Һ���ָ��
}BiTreeNode, *BiTree;
Status InsertBST(BiTree *, KeyType );

//��������������
void creatTree(BiTree* B ) {
	printf("��������:\n");
	int nodeN;
	scanf("%d",&nodeN);
	for (int i = 0; i < nodeN; i++) {
		int dataN;
		printf("�����%d������:", i + 1);
		scanf("%d", &dataN);
		InsertBST(B, dataN);
	}
}

int length ;
//��������������
int SearchBST(BiTree T, KeyType key, BiTree f, BiTree *p) {
	//�ڸ�ָ��T��ָ�����������еݹ�ò�����ؼ��ֵ���key������Ԫ�أ������ҳɹ�
	//��ָ��pָ�������Ԫ�صĽ�㣬������true������ָ��pָ�����·���Ϸ��ʵ�
	//���һ����㲢����false��ָ��fָ��T��˫�ף����ʼ����ֵΪnull
	//���Ҳ��ɹ�
	if (!T) {
		*p = f;
		//printf("����ʧ��\n");
		
		return FALSE;
	}
	//���ҳɹ�
	else if (EQ(key, T->data)) {
		*p = T;
		length++;
		//*pathLength = length;
		//printf("���ҳɹ�\n");
		return TRUE;
	}
	//���������м�������
	else if (LT(key, T->data)) { length++;  SearchBST(T->lchild, key, T, p); }
	//���������м�������
	else { length++;  SearchBST(T->rchild, key, T, p); }
}


//����
//��ӡ�������
Status PrintElement(KeyType e) {
	printf("%d ", e);
	return OK;
}

//����
Status PreOrderTraverse(BiTree T, Status Visit(KeyType)) {
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
Status InOrderTraverse(BiTree T, Status Visit(KeyType)) {
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
Status PostOrderTraverse(BiTree T, Status(*Visit)(KeyType)) {
	if (T) {
		//˳��Ϊ�����������������ٸ����
		if (PostOrderTraverse(T->lchild, PrintElement))
			if (PostOrderTraverse(T->rchild, PrintElement))
				if (Visit(T->data))  return OK;
		return ERROR;
	}
	else return OK;
}

//����
Status InsertBST(BiTree *T, KeyType e) {
	//!!!!!
	BiTree test = NULL;
	BiTree* p = &test;//???
	//������������T�в����ڹؼ��ֵ���e.key������Ԫ��ʱ������e������True�����򷵻�False
	if (!(SearchBST((*T), e, NULL, p))) {
		BiTree s=NULL;
		s = (BiTree)malloc(sizeof(BiTreeNode));
		s->data = e;
		s->lchild = s->rchild=NULL;
		if (!(*p)) *T = s;//������*sΪ�µĸ��ڵ�    //��
		else if LT(e, (*p)->data) (*p)->lchild = s;//������*sΪ����
		else (*p)->rchild = s;//������*sΪ�Һ���
		return TRUE;
	}
	else return FALSE; //�������йؼ�����ͬ�Ľ�㣬���ٲ���
}

//ɾ�� //p => (*p)
Status Delete(BiTree *p) {
	//�Ӷ�����������ɾ�����p�����ؽ��������������
	BiTree q;
	//����������ֻ���ؽ�����������
	if (!(*p)->rchild) {
		q = *p;
		(*p) = (*p)->lchild;
		free(q);
	}
	//ֻ���ؽ�����������
	else if (!(*p)->lchild) {
		q = *p;
		*p = (*p)->rchild;
		free(q);
	}
	//������������Ϊ��
	else {
		//!!!!!!!!!!!!!!!!!!!!!!!!!!
		BiTree *s = (BiTree*)malloc(sizeof(BiTree));
		q = *p;
		*s = (*p)->lchild;
		//ת��Ȼ�����ҵ���ͷ
		while ((*s)->rchild) {
			q = *s;
			*s = (*s)->rchild;
		}
		//sָ��ɾ���ġ�ǰ����
		(*p)->data = (*s)->data;
		//�ؽ�*q��������
		if (q != *p) q->lchild = (*s)->lchild;
		//�ؽ�*q��������
		else q->lchild = (*s)->lchild;
		delete s;
		//free(*s);
	}
	return TRUE;
}


//ɾ��
Status DeleteBST(BiTree *T, KeyType key) {
	//������������T�д��ڹؼ��ֵ���key������Ԫ��ʱ����ɾ��������Ԫ�ؽ��,������TRUE�����򷵻�FALSE

	//�����ڹؼ��ֵ���key������Ԫ��
	if (!*T) return FALSE;
	else {
		if (EQ(key, (*T)->data)) return Delete(T); //�ҵ��ؼ��ֵ���key������Ԫ��
		else if (LT(key, (*T)->data)) return DeleteBST(&(*T)->lchild, key);
		else return DeleteBST(&(*T)->rchild, key);
	}
}

//��ʽ����ӡ
//ÿ�����֮ǰ��Ҫ��ӡ�Ŀո�,����counter����������������һ���ӡ�����ո�
Status PrintTreeElem(KeyType e, int counter) {
	for (int i = 0; i < counter; i++) {
		printf("  ");
	}
	printf("%d\n", e);
	return OK;
}
//����������ʽ��ӡ����
Status PrintTree(BiTree T, Status(*Visit)(KeyType, int), int counter) {
	//��printTree������counter++��ζ�ţ�ÿ����һ����counter��һ����Ϊ������һ
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

	//������
	creatTree(ptr);

	//��ӡ��
	printf("����������ʽ��ӡ����:\n");
	PrintTree(b,PrintTreeElem, 0);

	//����
	printf("ǰ�����:");
	PreOrderTraverse(b, PrintElement);
	printf("\n�������:");
	InOrderTraverse(b, PrintElement);
	printf("\n�������:");
	PostOrderTraverse(b, PrintElement);
	BiTree p; 
	length = 0;
	printf("\n��������Ҫ���ҵ�����:");

	//�洢��Ҫ���ҵ�����
	int num;
	scanf("%d", &num);

	//�Ƿ��ҵ�
	int searchFlag ;
	searchFlag=SearchBST(b, num, NULL, &p);
	if (searchFlag == FALSE) {
		printf("����ʧ��\n");
	}
	else if (searchFlag = TRUE) {
		printf("���ҳɹ�\n");
	}

	//���ҳ���
	printf("���ҳ���Ϊ:%d\n", length);

	//ɾ������
	printf("��������Ҫɾ��������:");
	int deleteN;
	scanf("%d", &deleteN);
	DeleteBST(ptr, deleteN);
	printf("ɾ��֮�����:\n");
	PrintTree(b, PrintTreeElem, 0);

	//�������
	printf("��������Ҫ���������:");
	int insertN;
	scanf("%d", &insertN);
	InsertBST(ptr, insertN);
	printf("����֮�����:\n");
	PrintTree(b, PrintTreeElem, 0);
	getchar();
	getchar();
	return 0;
}