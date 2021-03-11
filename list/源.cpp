#include<stdio.h>
#include<stdlib.h>
#include<cmath>

#define OVERFLOW -2

//������
typedef struct Lnode {
	char *cityname;
	int x;
	int y;
	struct Lnode* next;
}Lnode,*LinkList;

//�����ʼ��
void InitList(LinkList *L) {
	*L = (LinkList)malloc(sizeof(Lnode));
	if (!L) exit(OVERFLOW);
	(*L)->next = (LinkList)malloc(sizeof(Lnode));
	(*L)->next->next = NULL;
}

//ͨ��������ҳ���
void getCity(LinkList L, int x_, int y_,char *city) {
	LinkList p;
	p = L->next;
	while (p) {
		//�������꿴�Ƿ�ƥ��
		if (p->x == x_&&p->y==y_) {
			city = p->cityname;
			printf("��������ҷ������ҵ���200,300�����괦�ĳ�����:");
			printf("%s\n", city);
			return;
		}
		p = p->next;
	}
	printf("��������ҷ�δ���ҵ�����Ϊ��200,300���ĳ���\n");
	return;
	
}
//ͨ���������Ʋ�������
void getxy(LinkList L,char* cityname) {
	LinkList p;
	p = L->next;
	while (p) {
		//���ҳ������ƿ��Ƿ�ƥ��
		if (*(p->cityname)==*cityname){
			printf("%s������Ϊ����%d,%d��\n",cityname ,p->x,p->y);
			return ;
		}
		p = p->next;
	}
	printf("û���ҵ�");
	return;
}

//�������
void ListInsert(LinkList L,char* cityname, int x_, int y_) {
	LinkList p,s;
	p = L;
	while (p->next->next) {
		p = p->next;
	}
	s = (Lnode*)malloc(sizeof(Lnode));
	if (!s) exit(OVERFLOW);
	//���û������ĳ��е���Ϣ���뵽����
	s->cityname = cityname;
	s->x = x_;
	s->y = y_;
	s->next = p->next;
	p->next = s;
	return;
}

//ɾ������
void listDelete(LinkList L,char*cityname,int* x_,int* y_) {
	LinkList s,p;
	p = L->next;
	//p->next��Ϊ��ָ���һ�δ���ҵ���Ҫɾ���ĳ�����������²���
	while (p->next&&*(p->next->cityname)!=*cityname) {
		p = p->next;
	}
	if (!(p->next)) {
		printf("û��Ҫɾ�ĳ���\n");
	}
	s = p->next;
	p->next = s->next;
	*x_ = s->x;
	*y_ = s->y;
	free(s);
	printf("ɾ���ĳ���Ϊ:%s,������Ϊ:(%d,%d)\n", cityname,*x_,*y_);
}

//���³���
void listUpdate(LinkList L) {
	char* cityname_=(char*)malloc(sizeof(char)*20);
	printf("����������ĵĳ�������:\n");
	scanf_s("%s",cityname_,20);
	LinkList p;
	//���³��е����ơ�����
	for (p = L->next; p->next!= NULL; p = p->next) {
		if (*(p->cityname) == (*cityname_)) {
			printf("�������µĳ�������:\n");
			scanf_s("%s", p->cityname,20);
			printf("�������µĳ�������x:\n");
			scanf_s("%d", &(p->x));
			printf("�������µĳ�������y:\n");
			scanf_s("%d", &(p->y));
			return;
		}
	}
	printf("%s", "û�иó���\n");
	return;
}

//����
//cityΪ�������У�distanceΪ��������
void cityDistance(LinkList L,char*city,int distance){
	LinkList p;
	//���city������
	int x_,y_;
	double d;//����֮��ľ���
	for (p = L->next; p != NULL; p = p->next) {
		if (*(p->cityname) == *city) {
			x_ = p->x;
			y_ = p->y;
			break;
		}
	}
	if (p == NULL) {
		printf("�ó��в�����\n");
		exit(OVERFLOW);
	}
	printf("��%s���%d�ڵĳ�����:\n", city, distance);
	//�������г���
	for (p = L->next; p != NULL; p = p->next) {
		d = sqrtf((p->x - x_)*(p->x - x_) + (p->y - y_)*(p->y - y_));
		if (d <= distance) {
			printf("%s\t", p->cityname);
		}
	}
}

int main() {
	LinkList L;
	char city1[20] = "BeiJing";
	char city2[20] = "ShangHai";
	char city3[20] = "HuBei";

	//��ӳ���
	InitList(&L);
	ListInsert(L,city1,100, 100);
	ListInsert(L, city2, 200, 300);
	ListInsert(L, city3, 200, 400);

	//���������
	char* city4 = NULL;
	getCity(L, 200, 300,city4);

	//������������
	getxy(L, city2);

	//ɾ������
	int x_=0,y_=0;
	listDelete(L, city3, &x_,&y_);

	//����
	listUpdate(L);

	//����
	cityDistance(L, city2, 500);

	getchar();
	getchar();
	return 0;
}
