#include<iostream>
#include<time.h>
#include<stdlib.h>
#define MAXSIZE 100
#define SIZE 100 //静态链表容量

typedef int KeyType; //定义关键字类型为整数类型
typedef struct {
	KeyType key; //关键字项
	//其他数据项
}RedType;
typedef struct {
	RedType rc; //记录项
	int next;  //指针项
}SLNode; //表结点类型
typedef struct {
	SLNode r[SIZE]; //0号单元为表头结点
	int length;
}SLinkListType; //静态链表类型
typedef struct {
	RedType r[MAXSIZE + 1]; //r[0]闲置或者是设为哨兵
	int length; //顺序表长度
}SqList;   //顺序表类型

typedef SqList HeapType; //堆采用顺序表存储表示

void init(SqList&L) {
	L.length = 100;
	srand(time(0));
	
	for (int i = 1; i < 101; i++) {
		L.r[i].key = rand()%100;
	}
}
//直接插入排序
void InsertSort(SqList&L) {
	int j=0;
	int cmp=0, move = 0;
	//对顺序表L作直接插入排序
	for (int i = 2; i <= L.length; ++i) {
		cmp++;
		if (L.r[i].key < L.r[i - 1].key) {  //“<”，需将L.r[i]插入有序子表
			L.r[0] = L.r[i];        //复制哨兵
			L.r[i] = L.r[i - 1];
			move = move + 2;
			for (j = i - 2; L.r[0].key < L.r[j].key; j--)
				cmp++;
				move++;
				L.r[j + 1] = L.r[j]; //记录后移
		}
		L.r[j + 1] = L.r[0]; //插入到正确位置
		move++;
	}
	printf("直接插入排序的比较次数为:%d,移动次数为:%d \n", cmp, move);
}

//折半插入排序
void BInserSort(SqList&L) {
	//对顺序表L作折半插入排序
	int low, high;
	int cmp = 0;
	int move = 0;
	int i, j;
	for (i = 2; i <= L.length; ++i) {
		move++;
		L.r[0] = L.r[i];
		low = 1;
		high = i - 1;
		while (low <= high) { //low<=high时，未找到
			int m = (low + high) / 2;
			if (L.r[0].key < L.r[m].key) {
				cmp++;
				high = m - 1; 	//若大于哨兵，则在[low, mid - 1]里寻找
			}
			else low = m + 1; //若小于等于哨兵，则在[mid + 1, high]里寻找
		}//while
		for (j = i - 1; j >= high + 1; --j) {
			L.r[j + 1] = L.r[j]; move++;
		}//从i - 1到high + 1全部后移一个
		L.r[high + 1] = L.r[0];
		move++;
	}
	printf("折半排序的比较次数为:%d,移动次数为:%d \n", cmp, move);
}

//希尔排序
int dlta[] = { 11,7,5,3,1 };
void ShellInsert(SqList&L, int dk,int &cmp,int &move) {
	//对顺序表L作一趟希尔插入排序。前后记录位置的增量是dk
	//r[0]为暂存单元。当j<=0时，插入位置已经=找到
	int i, j;
	for(i = dk + 1; i <= L.length; ++i){
		cmp++;
		if (L.r[i].key < L.r[i - dk].key) {  //需将L.r[i]插入有序增量子表
			L.r[0] = L.r[i];    //暂存在L.r[0]
			cmp++;
			move++;
			for (j = i - dk; j > 0 && L.r[0].key < L.r[j].key; j -= dk) { 
				L.r[j + dk] = L.r[j];//记录后移
				move++;
				move++;
			}
			L.r[j + dk] = L.r[0]; //插入
			move++;
			cmp++;
		}
    }
}

void ShellSort(SqList&L, int dlta[], int t) {
	//按增量序列dlta[0..t-1]对顺序表L作希尔排序
	int cmp = 0;
	int move = 0;
	for (int k = 0; k < t; ++k) {
		ShellInsert(L, dlta[k],cmp,move); //一趟增量为dlta[k]的插入排序
	}
	printf("希尔排序的比较次数为:%d,移动次数为:%d \n", cmp, move);
}

//起泡排序
void BubbleSort(SqList&L) {
	//将L中记录按关键字从小到大排序
	int i, j;
	int cmp = 0;
	int move = 0;
	RedType t;
	int change = 1;//是否有交换的标志
	for ( i = L.length-1; i > 1 && change; --i) { //控制最后一个每趟比较的元素
		change = 0;//一趟交换前把把交换标志清0
		for ( j = 1; j < i; ++j) { //从第一个元素开始比较
			cmp++;
			if (L.r[j].key > L.r[j + 1].key) {
				t = L.r[j];
				L.r[j] = L.r[j + 1];
				L.r[j + 1] = t;
				move = move + 3;
				change = 1;
			}
		}
	}
	printf("起泡排序的比较次数为:%d,移动次数为:%d \n", cmp, move);
}

//快速排序
int Partition(SqList&L, int low, int high,int& cmp,int& move) {
	//交换顺序表L中子表L.r[low..high]的记录，使枢轴记录到位，并返回其所在位置，此时
	//在它之前(后)的记录均不大(小)于它
	int pivotkey;//枢轴
	L.r[0] = L.r[low];
	pivotkey = L.r[low].key; //用子表的第一个记录作为枢轴记录
	move++;
	while (low < high) {	//从表的两端交替地向中间扫描
		while (low < high&&L.r[high].key >= pivotkey) {
			--high; cmp++;
		}
		cmp++; move++;
		L.r[low] = L.r[high]; //将比枢轴记录小的记录交换到低端
		while (low < high&&L.r[low].key <= pivotkey) {
			++low; cmp++;
		}
		L.r[high] = L.r[low];//将比枢轴记录大的记录交换到高端
		cmp++; move++;
	}
	L.r[low] = L.r[0];
	move++;
	return low; //返回枢轴所在位置
}
void QSort(SqList&L, int low, int high,int& cmp,int& move) {
	//对顺序表L中的子序列L.r[low..high]作快排
	int pivotloc;
	if (low < high) {  //长度大于1
		pivotloc = Partition(L, low, high,cmp,move); //将L.r[low..high]一分为二
		QSort(L, low, pivotloc - 1,cmp,move); //对低子表递归排序，pivotloc是枢轴的位置
		QSort(L, pivotloc + 1, high,cmp,move);//对高子表递归排序
	}
}
void QuickSort(SqList&L) {
	int move = 0;
	int cmp = 0;
	//对顺序表L做快排
	QSort(L, 1, L.length,cmp,move);
	printf("快排的比较次数为:%d,移动次数为:%d \n", cmp, move);
}

//简单选择排序
//选出关键字最小的记录
int SelectMinKey(SqList&L, int i,int& cmp,int& move) {
	int temp = i;
	int j;
	for (j = i; j <= L.length; j++) {
		if (L.r[j].key <= L.r[temp].key) {
			cmp++;
			temp = j;
		}
	}
	return temp;
}
void SelectSort(SqList&L) {
	//对顺序表L作简单选择排序
	int i,j = 0;
	int move = 0;
	int cmp = 0;
	for (i = 1; i < L.length; i++) { //选择第i小的记录，并交换到位
	 j = SelectMinKey(L, i,cmp,move);//在L.r[i..L.length]中选择key最小的记录
		if (i != j) {//与第i个记录交换
			cmp++;
			RedType temp = L.r[i];
			L.r[i] = L.r[j];
			L.r[j] = temp;
			move = move + 3;
		}
	}
	printf("简单选择排序的比较次数为:%d,移动次数为:%d \n", cmp, move);
}
//堆排序
void HeapAdjust(HeapType&H, int s, int m,int& cmp,int& move) {
	//已知H.r[s..m]中记录的关键字除H.r[s].key之外均满足堆的定义
	RedType rc = H.r[s];
	move++;
	int j = 0;
	for (j = 2 * s; j <= m; j *= 2) { //沿key较大的孩子结点向下筛选
		cmp++;
		if (j < m&&H.r[j].key < H.r[j + 1].key) ++j; //即为key较大的记录的下标
		cmp++;
		if (!(rc.key < H.r[j].key)) break;
		move++;
		H.r[s] = H.r[j];
		s = j;
	}
	move++;
	H.r[s] = rc; //插入
}
void HeapSort(HeapType&H) {
	//对顺序表H进行堆排序
	int i = 0;
	int cmp = 0;
	int move = 0;
	for (i = H.length / 2; i > 0; --i) { //把H.r[1..H.length]建成大顶堆
		HeapAdjust(H, i, H.length,cmp,move);
	}
	for (int i = H.length; i > 1; --i) {
		RedType temp = H.r[1];
		H.r[1] = H.r[i];    //将堆顶记录和当前未经排序子序列H.r[1..i]中最后一个记录相互交换
		H.r[i] = temp;
		move = move + 3;
		HeapAdjust(H, 1, i - 1,cmp,move);//将H.r[1..i-1]重新调整为大顶堆
	}
	printf("堆排序的比较次数为:%d,移动次数为:%d \n", cmp, move);
}
//归并排序
void Merge(RedType SR[], RedType* TR, int i, int m, int n,int &cmp,int &move) {
	//将有序的SR[i..m]和SR[m+1..n]归并为有序的TR[i..n]
	int j,k;
	//将SR中记录由小到大地并入TR
	for (j = m + 1, k = i; i <= m && j <= n; ++k) {
		cmp++;
		if (SR[i].key <= SR[j].key) {
			TR[k] = SR[i++]; 
			move++;
		}
		else {
			TR[k] = SR[j++];
			move++;
		}
	}
	//将剩余元素复制
	if (i <= m) {
		for (int w = i; w <= m; w++) {
			TR[k] = SR[w];
			k++;
			move++;
		}
	}
	if (j <= n) {
		for (int w = j; w <= n; w++) {
			TR[k] = SR[w];
			k++;
			move++;
		}
	}
}
void MSort(RedType SR[], RedType* TR1, int s, int t,int &cmp,int &move) {
	//将SR[s..t]归并排序为TR1[s..t]
	if (s == t) {		
		TR1[s] = SR[s];
	}
	else {
		int m;
		m = (s + t) / 2;//将SR[s..t]平分为SR[s..m]和SR[m+1..t]
		//开辟TR2新数组用来存放值
		RedType* TR2 = (RedType*)malloc(sizeof(RedType) * (t + 1));
		MSort(SR, TR2, s, m,cmp,move); //递归地将SR[s..m]归并为有序的TR2[s..m]
		MSort(SR, TR2, m + 1, t,cmp,move);//递归地将SR[m+1..t]归并为有序的TR2[m+1..t]
		Merge(TR2, TR1, s, m, t,cmp,move);//将TR2[s..m]和TR2[m+1..t]归并到TR1[s..t]
	}
}
void MergeSort(SqList&L) {
	//对顺序表L作归并排序
	int cmp=0, move=0;
	MSort(L.r, L.r, 1, L.length,cmp,move);
	printf("归并排序比较次数:%d,移动次数:%d \n", cmp, move);
}
int main() {
	for (int i = 0; i < 5; i++) {
		printf("\n\n-------------------第%d组数据-------------------\n", i + 1);
		SqList S;
		init(S);
		printf("\n初始化:\n");
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}
		printf("\n");
		printf("\n插入排序:\n");
		InsertSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}
		init(S);
		printf("\n折半插入排序:\n");
		BInserSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}
		init(S);
		printf("\n希尔排序\n");
		ShellSort(S, dlta, 5);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}

		init(S);
		printf("\n冒泡排序\n");
		BubbleSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}

		init(S);
		printf("\n快速排序\n");
		QuickSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}

		init(S);
		printf("\n选择排序\n");
		SelectSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}

		init(S);
		printf("\n堆排序\n");
		HeapSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}

		init(S);
		printf("\n归并排序\n");
		MergeSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}
	}
	getchar();
	return 0;
}