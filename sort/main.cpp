#include<iostream>
#include<time.h>
#include<stdlib.h>
#define MAXSIZE 100
#define SIZE 100 //��̬��������

typedef int KeyType; //����ؼ�������Ϊ��������
typedef struct {
	KeyType key; //�ؼ�����
	//����������
}RedType;
typedef struct {
	RedType rc; //��¼��
	int next;  //ָ����
}SLNode; //��������
typedef struct {
	SLNode r[SIZE]; //0�ŵ�ԪΪ��ͷ���
	int length;
}SLinkListType; //��̬��������
typedef struct {
	RedType r[MAXSIZE + 1]; //r[0]���û�������Ϊ�ڱ�
	int length; //˳�����
}SqList;   //˳�������

typedef SqList HeapType; //�Ѳ���˳���洢��ʾ

void init(SqList&L) {
	L.length = 100;
	srand(time(0));
	
	for (int i = 1; i < 101; i++) {
		L.r[i].key = rand()%100;
	}
}
//ֱ�Ӳ�������
void InsertSort(SqList&L) {
	int j=0;
	int cmp=0, move = 0;
	//��˳���L��ֱ�Ӳ�������
	for (int i = 2; i <= L.length; ++i) {
		cmp++;
		if (L.r[i].key < L.r[i - 1].key) {  //��<�����轫L.r[i]���������ӱ�
			L.r[0] = L.r[i];        //�����ڱ�
			L.r[i] = L.r[i - 1];
			move = move + 2;
			for (j = i - 2; L.r[0].key < L.r[j].key; j--)
				cmp++;
				move++;
				L.r[j + 1] = L.r[j]; //��¼����
		}
		L.r[j + 1] = L.r[0]; //���뵽��ȷλ��
		move++;
	}
	printf("ֱ�Ӳ�������ıȽϴ���Ϊ:%d,�ƶ�����Ϊ:%d \n", cmp, move);
}

//�۰��������
void BInserSort(SqList&L) {
	//��˳���L���۰��������
	int low, high;
	int cmp = 0;
	int move = 0;
	int i, j;
	for (i = 2; i <= L.length; ++i) {
		move++;
		L.r[0] = L.r[i];
		low = 1;
		high = i - 1;
		while (low <= high) { //low<=highʱ��δ�ҵ�
			int m = (low + high) / 2;
			if (L.r[0].key < L.r[m].key) {
				cmp++;
				high = m - 1; 	//�������ڱ�������[low, mid - 1]��Ѱ��
			}
			else low = m + 1; //��С�ڵ����ڱ�������[mid + 1, high]��Ѱ��
		}//while
		for (j = i - 1; j >= high + 1; --j) {
			L.r[j + 1] = L.r[j]; move++;
		}//��i - 1��high + 1ȫ������һ��
		L.r[high + 1] = L.r[0];
		move++;
	}
	printf("�۰�����ıȽϴ���Ϊ:%d,�ƶ�����Ϊ:%d \n", cmp, move);
}

//ϣ������
int dlta[] = { 11,7,5,3,1 };
void ShellInsert(SqList&L, int dk,int &cmp,int &move) {
	//��˳���L��һ��ϣ����������ǰ���¼λ�õ�������dk
	//r[0]Ϊ�ݴ浥Ԫ����j<=0ʱ������λ���Ѿ�=�ҵ�
	int i, j;
	for(i = dk + 1; i <= L.length; ++i){
		cmp++;
		if (L.r[i].key < L.r[i - dk].key) {  //�轫L.r[i]�������������ӱ�
			L.r[0] = L.r[i];    //�ݴ���L.r[0]
			cmp++;
			move++;
			for (j = i - dk; j > 0 && L.r[0].key < L.r[j].key; j -= dk) { 
				L.r[j + dk] = L.r[j];//��¼����
				move++;
				move++;
			}
			L.r[j + dk] = L.r[0]; //����
			move++;
			cmp++;
		}
    }
}

void ShellSort(SqList&L, int dlta[], int t) {
	//����������dlta[0..t-1]��˳���L��ϣ������
	int cmp = 0;
	int move = 0;
	for (int k = 0; k < t; ++k) {
		ShellInsert(L, dlta[k],cmp,move); //һ������Ϊdlta[k]�Ĳ�������
	}
	printf("ϣ������ıȽϴ���Ϊ:%d,�ƶ�����Ϊ:%d \n", cmp, move);
}

//��������
void BubbleSort(SqList&L) {
	//��L�м�¼���ؼ��ִ�С��������
	int i, j;
	int cmp = 0;
	int move = 0;
	RedType t;
	int change = 1;//�Ƿ��н����ı�־
	for ( i = L.length-1; i > 1 && change; --i) { //�������һ��ÿ�˱Ƚϵ�Ԫ��
		change = 0;//һ�˽���ǰ�Ѱѽ�����־��0
		for ( j = 1; j < i; ++j) { //�ӵ�һ��Ԫ�ؿ�ʼ�Ƚ�
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
	printf("��������ıȽϴ���Ϊ:%d,�ƶ�����Ϊ:%d \n", cmp, move);
}

//��������
int Partition(SqList&L, int low, int high,int& cmp,int& move) {
	//����˳���L���ӱ�L.r[low..high]�ļ�¼��ʹ�����¼��λ��������������λ�ã���ʱ
	//����֮ǰ(��)�ļ�¼������(С)����
	int pivotkey;//����
	L.r[0] = L.r[low];
	pivotkey = L.r[low].key; //���ӱ�ĵ�һ����¼��Ϊ�����¼
	move++;
	while (low < high) {	//�ӱ�����˽�������м�ɨ��
		while (low < high&&L.r[high].key >= pivotkey) {
			--high; cmp++;
		}
		cmp++; move++;
		L.r[low] = L.r[high]; //���������¼С�ļ�¼�������Ͷ�
		while (low < high&&L.r[low].key <= pivotkey) {
			++low; cmp++;
		}
		L.r[high] = L.r[low];//���������¼��ļ�¼�������߶�
		cmp++; move++;
	}
	L.r[low] = L.r[0];
	move++;
	return low; //������������λ��
}
void QSort(SqList&L, int low, int high,int& cmp,int& move) {
	//��˳���L�е�������L.r[low..high]������
	int pivotloc;
	if (low < high) {  //���ȴ���1
		pivotloc = Partition(L, low, high,cmp,move); //��L.r[low..high]һ��Ϊ��
		QSort(L, low, pivotloc - 1,cmp,move); //�Ե��ӱ�ݹ�����pivotloc�������λ��
		QSort(L, pivotloc + 1, high,cmp,move);//�Ը��ӱ�ݹ�����
	}
}
void QuickSort(SqList&L) {
	int move = 0;
	int cmp = 0;
	//��˳���L������
	QSort(L, 1, L.length,cmp,move);
	printf("���ŵıȽϴ���Ϊ:%d,�ƶ�����Ϊ:%d \n", cmp, move);
}

//��ѡ������
//ѡ���ؼ�����С�ļ�¼
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
	//��˳���L����ѡ������
	int i,j = 0;
	int move = 0;
	int cmp = 0;
	for (i = 1; i < L.length; i++) { //ѡ���iС�ļ�¼����������λ
	 j = SelectMinKey(L, i,cmp,move);//��L.r[i..L.length]��ѡ��key��С�ļ�¼
		if (i != j) {//���i����¼����
			cmp++;
			RedType temp = L.r[i];
			L.r[i] = L.r[j];
			L.r[j] = temp;
			move = move + 3;
		}
	}
	printf("��ѡ������ıȽϴ���Ϊ:%d,�ƶ�����Ϊ:%d \n", cmp, move);
}
//������
void HeapAdjust(HeapType&H, int s, int m,int& cmp,int& move) {
	//��֪H.r[s..m]�м�¼�Ĺؼ��ֳ�H.r[s].key֮�������ѵĶ���
	RedType rc = H.r[s];
	move++;
	int j = 0;
	for (j = 2 * s; j <= m; j *= 2) { //��key�ϴ�ĺ��ӽ������ɸѡ
		cmp++;
		if (j < m&&H.r[j].key < H.r[j + 1].key) ++j; //��Ϊkey�ϴ�ļ�¼���±�
		cmp++;
		if (!(rc.key < H.r[j].key)) break;
		move++;
		H.r[s] = H.r[j];
		s = j;
	}
	move++;
	H.r[s] = rc; //����
}
void HeapSort(HeapType&H) {
	//��˳���H���ж�����
	int i = 0;
	int cmp = 0;
	int move = 0;
	for (i = H.length / 2; i > 0; --i) { //��H.r[1..H.length]���ɴ󶥶�
		HeapAdjust(H, i, H.length,cmp,move);
	}
	for (int i = H.length; i > 1; --i) {
		RedType temp = H.r[1];
		H.r[1] = H.r[i];    //���Ѷ���¼�͵�ǰδ������������H.r[1..i]�����һ����¼�໥����
		H.r[i] = temp;
		move = move + 3;
		HeapAdjust(H, 1, i - 1,cmp,move);//��H.r[1..i-1]���µ���Ϊ�󶥶�
	}
	printf("������ıȽϴ���Ϊ:%d,�ƶ�����Ϊ:%d \n", cmp, move);
}
//�鲢����
void Merge(RedType SR[], RedType* TR, int i, int m, int n,int &cmp,int &move) {
	//�������SR[i..m]��SR[m+1..n]�鲢Ϊ�����TR[i..n]
	int j,k;
	//��SR�м�¼��С����ز���TR
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
	//��ʣ��Ԫ�ظ���
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
	//��SR[s..t]�鲢����ΪTR1[s..t]
	if (s == t) {		
		TR1[s] = SR[s];
	}
	else {
		int m;
		m = (s + t) / 2;//��SR[s..t]ƽ��ΪSR[s..m]��SR[m+1..t]
		//����TR2�������������ֵ
		RedType* TR2 = (RedType*)malloc(sizeof(RedType) * (t + 1));
		MSort(SR, TR2, s, m,cmp,move); //�ݹ�ؽ�SR[s..m]�鲢Ϊ�����TR2[s..m]
		MSort(SR, TR2, m + 1, t,cmp,move);//�ݹ�ؽ�SR[m+1..t]�鲢Ϊ�����TR2[m+1..t]
		Merge(TR2, TR1, s, m, t,cmp,move);//��TR2[s..m]��TR2[m+1..t]�鲢��TR1[s..t]
	}
}
void MergeSort(SqList&L) {
	//��˳���L���鲢����
	int cmp=0, move=0;
	MSort(L.r, L.r, 1, L.length,cmp,move);
	printf("�鲢����Ƚϴ���:%d,�ƶ�����:%d \n", cmp, move);
}
int main() {
	for (int i = 0; i < 5; i++) {
		printf("\n\n-------------------��%d������-------------------\n", i + 1);
		SqList S;
		init(S);
		printf("\n��ʼ��:\n");
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}
		printf("\n");
		printf("\n��������:\n");
		InsertSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}
		init(S);
		printf("\n�۰��������:\n");
		BInserSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}
		init(S);
		printf("\nϣ������\n");
		ShellSort(S, dlta, 5);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}

		init(S);
		printf("\nð������\n");
		BubbleSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}

		init(S);
		printf("\n��������\n");
		QuickSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}

		init(S);
		printf("\nѡ������\n");
		SelectSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}

		init(S);
		printf("\n������\n");
		HeapSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}

		init(S);
		printf("\n�鲢����\n");
		MergeSort(S);
		for (int i = 1; i < 101; i++) {
			printf("%d ", S.r[i].key);
		}
	}
	getchar();
	return 0;
}