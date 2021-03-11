#include<stdio.h>
#include<stdlib.h>
#include<cmath>

#define OVERFLOW -2

//定义结点
typedef struct Lnode {
	char *cityname;
	int x;
	int y;
	struct Lnode* next;
}Lnode,*LinkList;

//链表初始化
void InitList(LinkList *L) {
	*L = (LinkList)malloc(sizeof(Lnode));
	if (!L) exit(OVERFLOW);
	(*L)->next = (LinkList)malloc(sizeof(Lnode));
	(*L)->next->next = NULL;
}

//通过坐标查找城市
void getCity(LinkList L, int x_, int y_,char *city) {
	LinkList p;
	p = L->next;
	while (p) {
		//查找坐标看是否匹配
		if (p->x == x_&&p->y==y_) {
			city = p->cityname;
			printf("用坐标查找法，查找到（200,300）坐标处的城市是:");
			printf("%s\n", city);
			return;
		}
		p = p->next;
	}
	printf("用坐标查找法未查找到坐标为（200,300）的城市\n");
	return;
	
}
//通过城市名称查找坐标
void getxy(LinkList L,char* cityname) {
	LinkList p;
	p = L->next;
	while (p) {
		//查找城市名称看是否匹配
		if (*(p->cityname)==*cityname){
			printf("%s的坐标为：（%d,%d）\n",cityname ,p->x,p->y);
			return ;
		}
		p = p->next;
	}
	printf("没有找到");
	return;
}

//插入城市
void ListInsert(LinkList L,char* cityname, int x_, int y_) {
	LinkList p,s;
	p = L;
	while (p->next->next) {
		p = p->next;
	}
	s = (Lnode*)malloc(sizeof(Lnode));
	if (!s) exit(OVERFLOW);
	//将用户想加入的城市的信息加入到表中
	s->cityname = cityname;
	s->x = x_;
	s->y = y_;
	s->next = p->next;
	p->next = s;
	return;
}

//删除城市
void listDelete(LinkList L,char*cityname,int* x_,int* y_) {
	LinkList s,p;
	p = L->next;
	//p->next不为空指针且还未查找到需要删除的城市则继续往下查找
	while (p->next&&*(p->next->cityname)!=*cityname) {
		p = p->next;
	}
	if (!(p->next)) {
		printf("没有要删的城市\n");
	}
	s = p->next;
	p->next = s->next;
	*x_ = s->x;
	*y_ = s->y;
	free(s);
	printf("删除的城市为:%s,其坐标为:(%d,%d)\n", cityname,*x_,*y_);
}

//更新城市
void listUpdate(LinkList L) {
	char* cityname_=(char*)malloc(sizeof(char)*20);
	printf("给出你想更改的城市名称:\n");
	scanf_s("%s",cityname_,20);
	LinkList p;
	//更新城市的名称、坐标
	for (p = L->next; p->next!= NULL; p = p->next) {
		if (*(p->cityname) == (*cityname_)) {
			printf("请输入新的城市名称:\n");
			scanf_s("%s", p->cityname,20);
			printf("请输入新的城市坐标x:\n");
			scanf_s("%d", &(p->x));
			printf("请输入新的城市坐标y:\n");
			scanf_s("%d", &(p->y));
			return;
		}
	}
	printf("%s", "没有该城市\n");
	return;
}

//距离
//city为所给城市，distance为所给距离
void cityDistance(LinkList L,char*city,int distance){
	LinkList p;
	//获得city的坐标
	int x_,y_;
	double d;//两点之间的距离
	for (p = L->next; p != NULL; p = p->next) {
		if (*(p->cityname) == *city) {
			x_ = p->x;
			y_ = p->y;
			break;
		}
	}
	if (p == NULL) {
		printf("该城市不存在\n");
		exit(OVERFLOW);
	}
	printf("与%s相聚%d内的城市有:\n", city, distance);
	//遍历所有城市
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

	//添加城市
	InitList(&L);
	ListInsert(L,city1,100, 100);
	ListInsert(L, city2, 200, 300);
	ListInsert(L, city3, 200, 400);

	//按坐标查找
	char* city4 = NULL;
	getCity(L, 200, 300,city4);

	//按城市名查找
	getxy(L, city2);

	//删除城市
	int x_=0,y_=0;
	listDelete(L, city3, &x_,&y_);

	//更新
	listUpdate(L);

	//距离
	cityDistance(L, city2, 500);

	getchar();
	getchar();
	return 0;
}
