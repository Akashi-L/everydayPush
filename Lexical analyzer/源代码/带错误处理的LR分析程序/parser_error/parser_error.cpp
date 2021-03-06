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

//终结符表
string TERMINAL[TERNIMALNUM] = { "+", "-", "*", "/", "(", ")", "num", "$" };
//非终结符表
string NON_TERMINAL[NONTERNIMALNUM] = { "E", "T", "F" };

struct Action {
    //动作，移进、规约或者没有动作
    string action;
    //S5中5为target，R5中5为target，以此类推
    int target{ 0 };
};

//符号
struct Symbol {
    string type;
    string value;
};

//产生式
struct Production {
    //产生式左边
    string left;
    //产生式右边
    string right;
};

// 状态栈 
deque<int>stateStack; 
// 符号栈 
deque<Symbol> symbolStack; 
// 输入符号串
deque<Symbol> input; 

//带有错误表示的分析表
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

//goto表
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

//产生式表
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

//将状态0、"#"和"$"压栈
void init() {
    //将状态0压入符号栈，将"#"压入符号栈
    Symbol bottom;
    bottom.type = "BOTTOM";
    bottom.value = '#';
    symbolStack.push_back(bottom);
    stateStack.push_back(0);

    //将$压入输入符号串
    Symbol end;
    end.type = "END";
    end.value = '$';
    input.push_back(end);
}

//得到下标
//array为需要被用来匹配的终结符或者非终结符表，size为表的大小，symbol为当前读入的字符
//在匹配表中找到当前读入的字符的位置返回下标，如果未找到，证明当前读入字符不是该文法可以识别的符号，返回error
int getIndex(string* array, int  size, string symbol) {
    for (int i = 0; i < size; i++) {
        if (array[i] == symbol) {
            return i;
        }
    }
    return ERROR;
}

//得到下一个动作
Action getNextAction(int state, Symbol input) {
    Action next;
    Action notFindERROR;
    notFindERROR.action = "NOTFIND";
    int index;

    if (input.type == "SEPARATOR" || input.type == "OPERATOR" || input.type == "END") {
       //通过getIndex函数得到当前输入字符在匹配表中对应的下标index
        index = getIndex(TERMINAL, TERNIMALNUM, input.value);
        //如果index==error，则当前输入符号不是该文法可以识别的符号，返回notFindERROR
        if (index == ERROR) {
            next = notFindERROR;
        }
        else {
            //结合当前栈顶状态state，在分析表中找到所对应的动作为anly_action[state][index]
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

//得到规约后的状态
int getGoto(int state, Symbol stack) {
    int next;
    int index;
    //通过当前状态state和符号栈中栈顶符号在非终结符表中的下标index
    //在goto表中找到需要压入的状态为anly_goto[state][index]
    index = getIndex(NON_TERMINAL, NONTERNIMALNUM, stack.value);
    next = anly_goto[state][index];
    return next;
}

//得到错误恢复需要使用的Symbol
Symbol getError(Action action, int state) {
    Symbol error;
    //1.缺少运算对象
    //恢复：把一个假想的num补入栈
    if (action.target == 1) {
        error.type = "NUMBER";
        error.value = "num";
    }
    //2.缺少运算符号
    //恢复：把一个假想的运算符号“ + ” 补入栈
    else if (action.target == 2) {
        error.type = "OPERATOR";
        error.value = "+";
    }
    //3.括号不匹配，有多余右括号
    else if (action.target == 3) {
        error.type = "UNNECESSARY";
        error.value = " ";
    }
    //4.缺少右括号
    //恢复：把一个假想的右括号补入栈
    else if (action.target == 4) {
        error.type = "SEPARATOR";
        error.value = ")";
    }
    return error;
}

//打印
void printStack_error(Action action) {
    //状态栈
    string stateString;
    for (int i = 0; i < stateStack.size(); i++) {
        stateString = stateString + std::to_string(stateStack[i]) + " ";
    }
    cout << left << setw(20) << stateString;
    //符号栈
    string symbolString;
    for (int i = 0; i < symbolStack.size(); i++) {
        symbolString += symbolStack[i].value;
    }
    cout << left << setw(20) << symbolString;

    //输入表达式
    string expressionString;
    for (int i = 0; i < input.size(); i++) {
        expressionString += input[i].value;
    }
    cout << right << setw(20) << expressionString;

    //动作
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
        cout << "ERROR：该动作无定义！" << endl;
    }
    else if (action.action == "NOTFIND") {
        cout << "ERROR：该操作符无法识别!" << endl;
    }
    else if (action.action == "E") {
        if (action.target == 1) {
            cout << "缺少运算对象" << endl;
        }
        else if (action.target == 2) {
            cout << "缺少操作符" << endl;
        }
        else if (action.target == 3) {
            cout << "括号不匹配" << endl;
        }
        else if (action.target == 4) {
            cout << "缺少右括号" << endl;
        }
    }
    cout << endl;
}

//带有错误恢复的分析程序
void LR_error() {
    //初始化
    init();
    Action action;
    int counter = 0;
    cout << "步骤\t" << "状态栈" << setw(20) << "符号栈" << right << setw(20) << "输入流" << setw(20) << "\t动作" << endl;
    do {

        action = getNextAction(stateStack.back(), input.front());
        //移进
        cout << ++counter << "\t";
        printStack_error(action);
        if (action.action == "S") {
            stateStack.push_back(action.target);
            symbolStack.push_back(input.front());
            input.pop_front();
        }
        else if (action.action == "R") {
            //需要弹出的符号数
            int popNum = production[action.target].right.size();
            for (int i = 0; i < popNum; i++) {
                stateStack.pop_back();
                symbolStack.pop_back();
            }
            //需要压栈的符号
            Symbol pushSymbol;
            pushSymbol.value = production[action.target].left;
            pushSymbol.type = "NONTERNIMAL";
            symbolStack.push_back(pushSymbol);
            //需要压入的状态
            int state;
            state = getGoto(stateStack.back(), pushSymbol);
            stateStack.push_back(state);
        }
        else if (action.action == "ACC") {
            return;
        }
        else if (action.action == "Nl") {
            cout << "错误" << endl;
            return;
        }
        else if (action.action == "NOTFIND") {
            cout << "错误" << endl;
            return;
        }
        else if (action.action == "E") {
            Symbol addError = getError(action, stateStack.back());
            //如果该错误恢复块的类型为UNNECESSARY（有多余右括号），则只需要将输入符号串中的该右括号删除
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

//输入表达式
void input_expression() {
    ofstream outfile("code.cpp", ios::out);
    string temp;
    //将表达式输入到code.cpp文件，读入#表示输入结束
    while (getline(cin, temp))
    {
        if (temp == "#") break;
        outfile << temp << endl;
    }
    outfile.close();
}

//打开词法分析之后输出的文件
void openFile() {
    fstream file("output.txt");
    if (!file.is_open()) {
        cout << "文件打开失败";
        return;
    }
    while (file.peek() != EOF) {
        Symbol symbol;
        file >> symbol.value >> symbol.type;
        if (symbol.type != "")
            input.push_back(symbol);
    }
}

//词法分析
void lex_anly(FILE* input, FILE* output, FILE* error, FILE* note) {
    if (input == NULL) {
        printf("Cannot find the file!\nStrike any key to exit!\n");
        system("pause");
        exit(1);
    }
    else {
        //执行分析程序
        LexAnalyse(input, output, error, note);
        //关闭文件
        fclose(input);
        fclose(output);
        fclose(error);
        fclose(note);
        cout << "词法分析完成，接下来进行语法分析:" << endl;
    }
    openFile();
}

int main() {
    cout << "请输入需要分析的句子：" << endl;
    cout << "如果输入表达式结束请先回车再输入#再回车" << endl;
    //输入表达式
    input_expression();
    //词法分析
    FILE* input, * output, * error, * note;
    input = fopen("code.cpp", "r");
    output = fopen("output.txt", "w");
    error = fopen("error.txt", "w");
    note = fopen("note.txt", "w");
    lex_anly(input, output, error, note);
    //语法分析
    LR_error();
    return 0;
}