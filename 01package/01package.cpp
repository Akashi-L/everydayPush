#include <iostream>
#define N 50
using namespace std;
int n, best_value, max_weight;//物品数量，目前最优价值，背包容量 
int current_value, current_weight;//当前价值，当前重量
int rest_value;//剩余物品的价值
int current[N];//当前解
int best[N];//最终存储状态
int resultWeight;//最终重量
int count;//目前拿了几个物品
//物品重量和价值
int weights[N];
int vals[N];

//如果价值加上剩余物品的价值可能比当前最大价值大，则返回1
int bound(int t) {
	rest_value -= vals[t];
	if (rest_value + current_value >= best_value) {
		return 1;
	}
	else return 0;
}

void backTrack(int t) {
	//到达叶结点
	if (t > n - 1) {
		//如果当前装入方式的价值更大则进行更新
		if (current_value > best_value) {
			for (int i = 0; i < n; i++) {
				best[i] = current[i];
			}
			best_value = current_value;
			resultWeight = current_weight;
		}
		return;
	}
	//如果当前重量加上需要装入物品的重量小于背包容量，则将该物品装入
	if (current_weight + weights[t] <= max_weight) {
		current[t] = 1;
		current_value += vals[t];
		current_weight += weights[t];
		//装下一个物品
		backTrack(t + 1);
		//回溯
		current_value -= vals[t];
		current_weight -= weights[t];
	}
	//剪枝
	if (bound(t) == 1) {
		current[t] = 0;
		backTrack(t + 1);
	}
	rest_value += vals[t];
}

int  package() {
	//初始化
	current_weight = 0;
	current_value = 0;
	for (int i = 0; i < n; i++) {
		rest_value += vals[i];
	}
	backTrack(0);
	return best_value;
}


int main()
{
	cout << "注意：请按程序要求输入数据，可参考实验报告样例" << endl;
	cout << "请输入物品的个数：";
	cin >> n;
	cout << "请输入背包的容量：";
	cin >> max_weight;
	cout << "请依次输入物品的重量：";
	for (int i = 0; i < n; i++)
	{
		cin >> weights[i];
	}
	cout << "请依次输入物品的价值：";
	for (int i = 0; i < n; i++)
	{
		cin >> vals[i];
	}
	//装包
	package();
	cout << "最大价值：" << best_value << endl;
	cout << "重量：" << resultWeight << endl;
	cout << "装入物品：" << endl;
	for (int i = 0; i < n; i++) {
		if (best[i] != 0) {
			cout << i + 1 << " ";
		}
	}
	return 0;
}
