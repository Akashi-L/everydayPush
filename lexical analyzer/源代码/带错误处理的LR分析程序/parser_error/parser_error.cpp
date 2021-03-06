#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <fstream>
#include<deque>
#include <iomanip>
#include <string>
#include"lex.h"

#define N 50
#define STATENUM 16
#define TERNIMALNUM 8
#define NONTERNIMALNUM 3
#define PRODUCTIONNUM 9
#define ERROR -1

using namespace std;

//�ս����
string TERMINAL[TERNIMALNUM] = { "+", "-", "*", "/", "(", ")", "num", "$" };
//���ս����
string NON_TERMINAL[NONTERNIMALNUM] = { "E", "T", "F" };

struct Action {
    //�������ƽ�����Լ����û�ж���
    string action;
    //S5��5Ϊtarget��R5��5Ϊtarget���Դ�����
    int target{ 0 };
};

//����
struct Symbol {
    string type;
    string value;
};

//����ʽ
struct Production {
    //����ʽ���
    string left;
    //����ʽ�ұ�
    string right;
};

// ״̬ջ 
deque<int>stateStack; 
// ����ջ 
deque<Symbol> symbolStack; 
// ������Ŵ�
deque<Symbol> input; 

//���д����ʾ�ķ�����
Action anly_action_error[STATENUM][TERNIMALNUM]{
    //"+", "-", "*", "/", "(", ")", "num", "$" 
    {{"E", 1}, {"E", 1}, {"E", 1}, {"E", 1}, {"S", 4}, {"E", 3}, {"S", 5}, {"E", 1} },
    { {"S", 6}, {"S", 7}, {"E", 1}, {"E", 1}, {"E", 2}, {"E", 3}, {"E", 2}, {"ACC", 0} },
    { {"R", 3}, {"R", 3}, {"S", 8}, {"S", 9}, {"R", 3}, {"R", 3}, {"R", 3}, {"R", 3} } ,
    { {"R", 6}, {"R", 6}, {"R", 6}, {"R", 6}, {"R", 6}, {"R", 6}, {"R", 6}, {"R", 6} },
    { {"E", 1}, {"E", 1}, {"E", 1}, {"E", 1}, {"S", 4}, {"E", 1}, {"S", 5}, {"E", 1} } ,
    { {"R", 8}, {"R", 8}, {"R", 8}, {"R", 8}, {"R", 8}, {"R", 8}, {"R", 8}, {"R", 8} } ,
    { {"E", 1}, {"E", 1}, {"E", 1}, {"E", 1}, {"S", 4}, {"E", 1}, {"S", 5}, {"E", 1} },
    { {"E", 1}, {"E", 1}, {"E", 1}, {"E", 1}, {"S", 4}, {"E", 1}, {"S", 5}, {"E", 1} } ,
    { {"E", 1}, {"E", 1}, {"E", 1}, {"E", 1}, {"S", 4}, {"E", 1}, {"S", 5}, {"E", 1} } ,
    { {"E", 1}, {"E", 1}, {"E", 1}, {"E", 1}, {"S", 4}, {"E", 1}, {"S", 5}, {"E", 1} } ,
    { {"S", 6}, {"S", 7}, {"E", 4}, {"E", 4}, {"E", 2}, {"S", 15}, {"E", 2}, {"E", 4} } ,
    { {"R", 1}, {"R", 1}, {"S", 8}, {"S", 9}, {"R", 1}, {"R", 1}, {"R", 1}, {"R", 1} } ,
    { {"R", 2}, {"R", 2}, {"S", 8}, {"S", 9}, {"R", 2}, {"R", 2}, {"R", 2}, {"R", 2} } ,
    { {"R", 4}, {"R", 4}, {"R", 4}, {"R", 4}, {"R", 4}, {"R", 4}, {"R", 4}, {"R", 4} } ,
    { {"R", 5}, {"R", 5}, {"R", 5}, {"R", 5}, {"R", 5}, {"R", 5}, {"R", 5}, {"R", 5} } ,
    { {"R", 7}, {"R", 7}, {"R", 7}, {"R", 7}, {"R", 7}, {"R", 7}, {"R", 7}, {"R", 7} }
};

//goto��
int anly_goto[STATENUM][NONTERNIMALNUM]{
    //{ "E", "T", "F" };
        { 1, 2, 3 },
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 },
        {10, 2, 3 },
        { 0, 0, 0 },
        { 0, 11, 3 },
        { 0, 12, 3 },
        { 0, 0, 13 },
        { 0, 0, 14 },
        { 0, 0, 0},
        { 0, 0, 0},
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
};

//����ʽ��
Production production[PRODUCTIONNUM]{
    {"E'","E"},
    {"E","E+T"},
    {"E","E-T"},
    {"E","T"},
    {"T","T*F"},
    {"T","T/F"},
    {"T","F"},
    {"F","(E)"},
    {"F","n"}
};

//��״̬0��"#"��"$"ѹջ
void init() {
    //��״̬0ѹ�����ջ����"#"ѹ�����ջ
    Symbol bottom;
    bottom.type = "BOTTOM";
    bottom.value = '#';
    symbolStack.push_back(bottom);
    stateStack.push_back(0);

    //��$ѹ��������Ŵ�
    Symbol end;
    end.type = "END";
    end.value = '$';
    input.push_back(end);
}

//�õ��±�
//arrayΪ��Ҫ������ƥ����ս�����߷��ս����sizeΪ��Ĵ�С��symbolΪ��ǰ������ַ�
//��ƥ������ҵ���ǰ������ַ���λ�÷����±꣬���δ�ҵ���֤����ǰ�����ַ����Ǹ��ķ�����ʶ��ķ��ţ�����error
int getIndex(string* array, int  size, string symbol) {
    for (int i = 0; i < size; i++) {
        if (array[i] == symbol) {
            return i;
        }
    }
    return ERROR;
}

//�õ���һ������
Action getNextAction(int state, Symbol input) {
    Action next;
    Action notFindERROR;
    notFindERROR.action = "NOTFIND";
    int index;

    if (input.type == "SEPARATOR" || input.type == "OPERATOR" || input.type == "END") {
       //ͨ��getIndex�����õ���ǰ�����ַ���ƥ����ж�Ӧ���±�index
        index = getIndex(TERMINAL, TERNIMALNUM, input.value);
        //���index==error����ǰ������Ų��Ǹ��ķ�����ʶ��ķ��ţ�����notFindERROR
        if (index == ERROR) {
            next = notFindERROR;
        }
        else {
            //��ϵ�ǰջ��״̬state���ڷ��������ҵ�����Ӧ�Ķ���Ϊanly_action[state][index]
            next = anly_action_error[state][index];
        }

    }
    else if (input.type == "NUMBER") {
        for (int i = 0; i < TERNIMALNUM; i++) {
            if (TERMINAL[i] == "num") {
                next = anly_action_error[state][i];
            }
        }
    }
    return next;
}

//�õ���Լ���״̬
int getGoto(int state, Symbol stack) {
    int next;
    int index;
    //ͨ����ǰ״̬state�ͷ���ջ��ջ�������ڷ��ս�����е��±�index
    //��goto�����ҵ���Ҫѹ���״̬Ϊanly_goto[state][index]
    index = getIndex(NON_TERMINAL, NONTERNIMALNUM, stack.value);
    next = anly_goto[state][index];
    return next;
}

//�õ�����ָ���Ҫʹ�õ�Symbol
Symbol getError(Action action, int state) {
    Symbol error;
    //1.ȱ���������
    //�ָ�����һ�������num����ջ
    if (action.target == 1) {
        error.type = "NUMBER";
        error.value = "num";
    }
    //2.ȱ���������
    //�ָ�����һ�������������š� + �� ����ջ
    else if (action.target == 2) {
        error.type = "OPERATOR";
        error.value = "+";
    }
    //3.���Ų�ƥ�䣬�ж���������
    else if (action.target == 3) {
        error.type = "UNNECESSARY";
        error.value = " ";
    }
    //4.ȱ��������
    //�ָ�����һ������������Ų���ջ
    else if (action.target == 4) {
        error.type = "SEPARATOR";
        error.value = ")";
    }
    return error;
}

//��ӡ
void printStack_error(Action action) {
    //״̬ջ
    string stateString;
    for (int i = 0; i < stateStack.size(); i++) {
        stateString = stateString + std::to_string(stateStack[i]) + " ";
    }
    cout << left << setw(20) << stateString;
    //����ջ
    string symbolString;
    for (int i = 0; i < symbolStack.size(); i++) {
        symbolString += symbolStack[i].value;
    }
    cout << left << setw(20) << symbolString;

    //������ʽ
    string expressionString;
    for (int i = 0; i < input.size(); i++) {
        expressionString += input[i].value;
    }
    cout << right << setw(20) << expressionString;

    //����
    cout << "\t";
    if (action.action == "S") {
        cout << "S" << action.target << " shift" << action.target << endl;
    }
    else if (action.action == "R") {
        cout << action.action << action.target << " " << "reduce by " << production[action.target].left << " -> " << production[action.target].right << endl;
    }
    else if (action.action == "ACC") {
        cout << "ACC" << endl;
    }
    else if (action.action == "Nl") {
        cout << "ERROR���ö����޶��壡" << endl;
    }
    else if (action.action == "NOTFIND") {
        cout << "ERROR���ò������޷�ʶ��!" << endl;
    }
    else if (action.action == "E") {
        if (action.target == 1) {
            cout << "ȱ���������" << endl;
        }
        else if (action.target == 2) {
            cout << "ȱ�ٲ�����" << endl;
        }
        else if (action.target == 3) {
            cout << "���Ų�ƥ��" << endl;
        }
        else if (action.target == 4) {
            cout << "ȱ��������" << endl;
        }
    }
    cout << endl;
}

//���д���ָ��ķ�������
void LR_error() {
    //��ʼ��
    init();
    Action action;
    int counter = 0;
    cout << "����\t" << "״̬ջ" << setw(20) << "����ջ" << right << setw(20) << "������" << setw(20) << "\t����" << endl;
    do {

        action = getNextAction(stateStack.back(), input.front());
        //�ƽ�
        cout << ++counter << "\t";
        printStack_error(action);
        if (action.action == "S") {
            stateStack.push_back(action.target);
            symbolStack.push_back(input.front());
            input.pop_front();
        }
        else if (action.action == "R") {
            //��Ҫ�����ķ�����
            int popNum = production[action.target].right.size();
            for (int i = 0; i < popNum; i++) {
                stateStack.pop_back();
                symbolStack.pop_back();
            }
            //��Ҫѹջ�ķ���
            Symbol pushSymbol;
            pushSymbol.value = production[action.target].left;
            pushSymbol.type = "NONTERNIMAL";
            symbolStack.push_back(pushSymbol);
            //��Ҫѹ���״̬
            int state;
            state = getGoto(stateStack.back(), pushSymbol);
            stateStack.push_back(state);
        }
        else if (action.action == "ACC") {
            return;
        }
        else if (action.action == "Nl") {
            cout << "����" << endl;
            return;
        }
        else if (action.action == "NOTFIND") {
            cout << "����" << endl;
            return;
        }
        else if (action.action == "E") {
            Symbol addError = getError(action, stateStack.back());
            //����ô���ָ��������ΪUNNECESSARY���ж��������ţ�����ֻ��Ҫ��������Ŵ��еĸ�������ɾ��
            if (addError.type == "UNNECESSARY") {
                input.pop_front();
                continue;
            }
            else {
                input.push_front(addError);
            }
            
        }
    } while (1);
    return;
}

//������ʽ
void input_expression() {
    ofstream outfile("code.cpp", ios::out);
    string temp;
    //�����ʽ���뵽code.cpp�ļ�������#��ʾ�������
    while (getline(cin, temp))
    {
        if (temp == "#") break;
        outfile << temp << endl;
    }
    outfile.close();
}

//�򿪴ʷ�����֮��������ļ�
void openFile() {
    fstream file("output.txt");
    if (!file.is_open()) {
        cout << "�ļ���ʧ��";
        return;
    }
    while (file.peek() != EOF) {
        Symbol symbol;
        file >> symbol.value >> symbol.type;
        if (symbol.type != "")
            input.push_back(symbol);
    }
}

//�ʷ�����
void lex_anly(FILE* input, FILE* output, FILE* error, FILE* note) {
    if (input == NULL) {
        printf("Cannot find the file!\nStrike any key to exit!\n");
        system("pause");
        exit(1);
    }
    else {
        //ִ�з�������
        LexAnalyse(input, output, error, note);
        //�ر��ļ�
        fclose(input);
        fclose(output);
        fclose(error);
        fclose(note);
        cout << "�ʷ�������ɣ������������﷨����:" << endl;
    }
    openFile();
}

int main() {
    cout << "��������Ҫ�����ľ��ӣ�" << endl;
    cout << "���������ʽ�������Ȼس�������#�ٻس�" << endl;
    //������ʽ
    input_expression();
    //�ʷ�����
    FILE* input, * output, * error, * note;
    input = fopen("code.cpp", "r");
    output = fopen("output.txt", "w");
    error = fopen("error.txt", "w");
    note = fopen("note.txt", "w");
    lex_anly(input, output, error, note);
    //�﷨����
    LR_error();
    return 0;
}