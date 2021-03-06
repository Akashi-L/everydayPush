#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include<map>

//关键字、分隔符、操作符表大小预定义

#define MAX_STR 65535

#define KEYWORDS_SIZE 32

#define OPERATORS_SIZE 18

#define SEPARATORS_SIZE 15

//各类单词当前分析状态

#define START 1                 //开始分析

#define ANL_IDORKEYWORD 2       //分析是标识符还是关键字

#define ANL_NUMBER 3            //分析常数

#define ANL_NOTE 4              //分析注释，包括单行注释和多行注释

#define ANL_LITERAL 5           //分析引号见间的字符串

#define DONE  6                 //分析完成

//单词类型
#define TYPE_KEYWORD 1          //关键字

#define TYPE_IDENTIFIER 2       //标识符

#define TYPE_NUMBER 3           //数字

#define TYPE_NOTE 4             //注释

#define TYPE_LITERAL 5          //引号间的字符串

#define TYPE_OPERATOR 6         //运算符

#define TYPE_SEPARATOR 7        //分隔符

#define TYPE_ERROR 8            //错误

#define TYPE_ERRORID 9          //标识符错误

#define TYPE_ERRORNUM 10        //常数错误

#define TYPE_UNKNOWN 11         //未知

#define TYPE_ENDFILE 12         //文件结束

//关键字、分隔符、操作符表
const char* Operators[OPERATORS_SIZE] = { "+","-","*","/","%","=","==","!=","<","<=",">",">=","+=","*=" ,"/=","-=","++","--"};

const char* Separators[SEPARATORS_SIZE] = { ",",";",".","\'","\"","(",")","[","]","{","}","//","/*","*/","#" };

const char* Keywords[KEYWORDS_SIZE] = { "int" , "long" , "short" , "float" , "double" , "char" , "unsigned" , "signed" , "const" , "void" , "volatile" , "enum" , "struct" , "union" , "if" , "else" , "goto" , "switch" , "case" , "do" , "while" , "for" , "continue" , "break" , "return" , "default" , "typedef" , "auto" , "register" , "extern" , "static" , "sizeof" };

//符号表
std::map<std::string, int>ID;
std::map<std::string, int>::iterator itr;


char c=' ';//当前读入字符
int line_no = 1;//行号
int charCounter = 0;//字符个数
int idCounter = 0;//标识符个数
int numCounter = 0;//常数个数
int keywordCounter = 0;//关键字个数
int opCounter = 0;//操作符个数
int sepCounter = 0;//分隔符个数
int idAddress = 0;//标识符在标识符表中的位置

//判断是否为操作符
int IsOperator(char c){
    for (int i = 0; i < OPERATORS_SIZE; i++)
        if (Operators[i][0] == c)
            return 1;
    return 0;
}

//判断是否为分隔符
int IsSeparator(char c){
    for (int i = 0; i < SEPARATORS_SIZE; i++)
        if (Separators[i][0] == c)
            return 1;
    return 0;
}

//判断是否是关键字
int IsKeyword(char* str){
    for (int i = 0; i < KEYWORDS_SIZE; i++)
        if (strcmp(Keywords[i], str) == 0)
            return 1;
    return 0;
}

//检查字母型单词是否在符号表中，如果没有就加入
int InsertID(char* str) {  
    //查找是否存在当前字符串
    itr = ID.find(str);
    if (itr != ID.end())
        return ID[str];

    else {
        ID[str] = ID.size();
        return ID[str];
    }
}

//向不同的文件中打印分析结果，outf文件中输出单词分析结果及统计，note文件中输出注释，error文件中输出检测出的错误
void printtoFile(FILE* outf, FILE* note,FILE* error, int type, char* str){
    //因为keyword和id在一起分析的，这里将这两者区分开
    if (IsKeyword(str) == 1) type = TYPE_KEYWORD;

    switch (type) {

    case TYPE_KEYWORD:    fprintf(outf,"\t%d\t %s\t %s\t %s %s %s\n", line_no,str,"关键字","<",str,",->"); break;

    case TYPE_IDENTIFIER: fprintf(outf,"\t%d\t %s\t %s\t %s %d %s\n ", line_no,str, "标识符", "<id,",idAddress,">"); break;

    case TYPE_NUMBER:     fprintf(outf,"\t%d\t %s\t %s\t %s %s %s\n", line_no,str, "常数", "<num,",str,">"); break;

    case TYPE_NOTE: {

        fprintf(note, "%s\t %d\t %s\n","NOTE:",line_no,str);

         } break;

    case TYPE_LITERAL:   fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "字符串", "<", str, ",->"); break;

    case TYPE_OPERATOR: {
        if(strcmp(str,"=") == 0){
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "操作符", "<", "assign-op", ",->");
        }
        else if (strcmp(str, "<") == 0) {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "操作符", "<", str, ",LT>");
        }
        else if (strcmp(str, "<=") == 0) {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "操作符", "<", str, ",LE>");
        }
        else if (strcmp(str, "==") == 0) {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "操作符", "<", str, ",EQ>");
        }
        else if (strcmp(str, ">") == 0) {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "操作符", "<", str, ",GT>");
        }
        else if (strcmp(str, ">=") == 0) {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "操作符", "<", str, ",GE>");
        }
        else {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "操作符", "<", str, ",->");
        }
        
    } break;

    case TYPE_SEPARATOR:  fprintf(outf,"\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "分隔符", "<", str, ",->"); break;

    case TYPE_ERROR:      fprintf(error, "%s\t %d\t %s\n", str, line_no, "错误"); break;

    case TYPE_ERRORID:     fprintf(error, "%s\t %d\t %s\n", str, line_no, "标识符命名错误"); break;

    case TYPE_ERRORNUM:     fprintf(error, "%s\t %d\t %s\n", str, line_no, "小数格式错误"); break;

    default:break;

    }
}

//词法分析器
void LexAnalyse(FILE* inf, FILE* outf,FILE* error, FILE* note){
   
    //每次读入的字符串
    char str[MAX_STR];
    //指示读入的字符应该在str数组中存的下标
    int i;
    //state为当前分析单词的状态，type为分析出的单词可能的类型
    int state, type;
    //行注释标志
    char flag_one_line_note;
    //用于限定小数中只能有一个小数点否则可能会出现错误
    char flag_had_got_dot;

    //读到文件结束符结束分析
    while (!feof(inf)){
        //每次开始下一个单词的读取时重组i
        i = 0;
        //设置为开始状态
        state = START;
        //重置标志
        flag_one_line_note = 0;

        flag_had_got_dot = 0;

        //读入字符直到单词分析结束
        while (state != DONE){
            //读入一个字符
            c = fgetc(inf);
            //根据state状态进行处理
            switch (state){

            //开始分析单词状态
            case START:

                //读入的是空格等
                if(c == ' ' || c == '\t');
                else if (c == '\n'){
                    line_no++;
                }
                //读入的是字符和下划线
                else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')||c == '_'){
                    state = ANL_IDORKEYWORD;
                    type = TYPE_IDENTIFIER;
                    str[i] = c; i++;

                }
                //读入的是数字
                else if (c >= '0' && c <= '9'){
                    state = ANL_NUMBER;
                    type = TYPE_NUMBER;
                    str[i] = c; i++;
                }
                //读入的是/，合法的有两种情况，要么是注释要么是操作符
                else if (c == '/'){
                    str[i] = c; i++;
                    //超前读入，以便判断/具体属于什么
                    c = fgetc(inf);
           
                    if (c == '/') {
                        flag_one_line_note = 1;
                        state = ANL_NOTE;
                        type = TYPE_NOTE;
                        str[i] = c; i++;
                       
                    }

                    else if (c == '*') {
                        state = ANL_NOTE;
                        type = TYPE_NOTE;
                        str[i] = c; i++;
                       
                    }
                    else if (c == '=') {
                        str[1] = '='; i = 2;/* str[2]='\0'    */
                            state = DONE;
                            type = TYPE_OPERATOR;
                           
                    }
                    else{
                        state = DONE;
                        type = TYPE_OPERATOR;
                        fseek(inf, -1, SEEK_CUR);/* 文件流指针前移1个字节  */
                        i = 1;/* str[1]='\0'; */
                    }

                }
                //对于有两个字符的并且需要超前读入的操作符的判断，包括<=,>=,++等，如果超前读入后与我们期望判断的一致则结束，否则回退

                else if (c == '<' || c == '>'){
                    type = TYPE_OPERATOR;
                    str[0] = c;
                    //超前读入
                    c = fgetc(inf);
                    
                    if (c == '='){
                        str[1] = '='; i = 2;/* str[2]='\0'    */
                        state = DONE;
                    }

                    else{
                        fseek(inf, -1, SEEK_CUR);/* 文件流指针前移1个字节  */
                        i = 1;/* str[1]='\0'; */
                        state = DONE;
                    }
                }

                //分析操作符++,+=
                else if (c == '+' ) {
                    type = TYPE_OPERATOR;
                    str[0] = c;
                    c = fgetc(inf);
                    if (c == '=') {

                        str[1] = '='; i = 2;/* str[2]='\0'    */
                        state = DONE;

                    }
                    else if (c == '+') {
                        str[1] = '+'; i = 2;/* str[2]='\0'    */
                        state = DONE;
                    }

                    else {
                        fseek(inf, -1, SEEK_CUR);/* 文件流指针前移1个字节  */
                        i = 1;/* str[1]='\0'; */
                        state = DONE;
                    }
                }
                //分析操作符--，-=
                else if (c == '-') {
                type = TYPE_OPERATOR;
                str[0] = c;
                c = fgetc(inf);
                if (c == '=') {

                    str[1] = '='; i = 2;/* str[2]='\0'    */
                    state = DONE;

                }
                else if (c == '-') {
                    str[1] = '-'; i = 2;/* str[2]='\0'    */
                    state = DONE;
                }

                else {
                    fseek(inf, -1, SEEK_CUR);/* 文件流指针前移1个字节  */
                    i = 1;/* str[1]='\0'; */
                    state = DONE;
                    }
                }
                //分析操作符*=
                else if (c == '*') {
                type = TYPE_OPERATOR;
                str[0] = c;
                c = fgetc(inf);
                if (c == '=') {

                    str[1] = '='; i = 2;/* str[2]='\0'    */
                    state = DONE;

                }

                else {
                    fseek(inf, -1, SEEK_CUR);/* 文件流指针前移1个字节  */
                    i = 1;/* str[1]='\0'; */
                    state = DONE;
                    }
                }
                //分析操作符！=
                else if (c == '!'){
                    str[0] = c;
                    c = fgetc(inf);
                    if (c == '=') {
                        type = TYPE_OPERATOR;
                        state = DONE;
                        str[1] = '='; i = 2;/* str[2]='\0'    */
                    }
                    else {
                        type = TYPE_ERROR;
                        fseek(inf, -1, SEEK_CUR);/* 文件流指针前移1个字节  */
                        i = 1;/* str[1]='\0'; */
                    }

                }
                //分析不需要超前读入的操作符
                else if (IsOperator(c)) {
                    state = DONE;
                    type = TYPE_OPERATOR;
                    str[0] = c; i = 1;/* str[1]='\0'; */
                }

                //分析引号间的字符串类型
                else if (c == '\"' || c == '\'') {
                    state = ANL_LITERAL;
                    type = TYPE_LITERAL;
                    str[0] = c; i = 1;
                }
                //分析分隔符
                else if (IsSeparator(c)) {
                    state = DONE;
                    type = TYPE_SEPARATOR;
                    str[0] = c; i = 1;/* str[1]='\0'; */
                }
                //如果读入的是eof则分析结束
                else if (c == EOF){

                    state = DONE;
                    type = TYPE_ENDFILE;
                }

                else{
                    state = DONE;
                    type = TYPE_UNKNOWN;
                }

                break;/* case STA */

            //进入分析标识符和关键字类型
            case ANL_IDORKEYWORD:
                //合法读入
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'){
                    str[i] = c; i++;
                }
                //读入的是空格等代表当前单词读入结束了
                else if (c == ' ' || c == '\t' || c == '\n' || IsOperator(c) || IsSeparator(c)) {
                    state = DONE;
                    fseek(inf, -1, SEEK_CUR);/* 文件流指针前移1个字节  */
                }
                //error
                else{
                    str[i] = c; i++;
                    type = TYPE_ERRORID;      
                }
                break;/* case STA */
            
            //分析常数状态
            case ANL_NUMBER:
                //合法读入
                if (c >= '0' && c <= '9') {
                    str[i] = c; i++;
                }
                //小数
                else if (c == '.') {
                    str[i] = c; i++;
                    c = fgetc(inf);
                    //一个小数点
                    if (flag_had_got_dot == 0) {   
                        if (c >= '0' && c <= '9') {
                            str[i] = c; i++;
                            flag_had_got_dot = 1;
                        }
                        //多个小数点非法
                        else {
                            fseek(inf, -1, SEEK_CUR);/* 文件流指针前移1个字节  */
                            
                            type = TYPE_ERRORNUM;         /*  如 1..h  标记为ERROR类型 */  
                        }
                    }
                    else {
                        type = TYPE_ERROR;
                        str[i] = c; i++;
                    }
                }
                //指数
                else if (c=='E') {
                    str[i] = c; i++;
                    c = fgetc(inf);
                    if (c >= '0' && c <= '9') {
                        str[i] = c; i++;
                    }
                    else if (c == '+' || c == '-') {
                        str[i] = c; i++;
                        c = fgetc(inf);
                        if (c >= '0' && c <= '9') {
                            str[i] = c; i++;
                        }
                        else {
                            type = TYPE_ERROR;
                            fseek(inf, -1, SEEK_CUR);
                        }
                    }
                    else {
                        type = TYPE_ERROR;
                        fseek(inf, -1, SEEK_CUR);
                    }
                }
                //比较典型的error
                else if ((c >= 'a' && c < 'e')||( c>'e' && c<='z') || (c >= 'A' && c<'E')||( c>'E'&& c <= 'Z') || c == '_') {
                    type = TYPE_ERROR;
                    if(type==TYPE_ERROR){
                        type = TYPE_ERRORID;
                    }
                    str[i] = c; i++;
                }
                //当前单词读入结束
                else if (c == ' ' || c == '\t' || c == '\n' || IsOperator(c) || IsSeparator(c)) {
                    state = DONE;
                    fseek(inf, -1, SEEK_CUR);/* 文件流指针前移1个字节  */
             
                }
                //error
                else {
                    str[i] = c; i++;
                    type = TYPE_ERROR;
                 
                }

                break;/* case STA */
            
            //分析注释状态
            case ANL_NOTE:
                //单行注释情况
                if (flag_one_line_note == 1) {
                    if (c == '\n') {
                        state = DONE;
                        fseek(inf, -1, SEEK_CUR);/* 文件流指针前移1个字节 */
                    }
                    //没读到换行就继续读
                    else{
                        str[i] = c; i++;
                    }

                }
                //多行注释情况/**/
                else {
                    //error
                    if (feof(inf)){
                        state = DONE;
                        type = TYPE_ERROR;
                    }
                    //读到换行
                    else if (c == '\n'){
                        line_no++;
                        str[i] = c; i++;
                    }
                    //判断多行注释是否结束
                    else if (c == '*') {
                        str[i] = c; i++;
                        c = fgetc(inf);
                        if (c == '/'){
                            state = DONE;
                            str[i] = c; i++;
                        }
                        //多行注释的结束部分，即*/出现问题             
                        else{
                            if (feof(inf)) {
                                state = DONE;
                                type = TYPE_ERROR;
                            }
                            fseek(inf, -1, SEEK_CUR);/* 文件流指针前移1个字节  */
                        }
                    }

                    else {
                        //多行注释继续读
                        str[i] = c; i++;
                    }
                }

                break;/* case STA */
            //分析引号间的字符串类型
            case ANL_LITERAL:
                //error
                if (feof(inf)) {
                    state = DONE;
                    type = TYPE_ERROR;
                }

                else if (c == '\n'){
                    line_no++;
                    str[i] = c; i++;
                }
                //结束
                else if (c == '\"' || c == '\'') {
                    state = DONE;
                    str[i] = c; i++;
                }

                else if (c == '\\'){
                    str[i] = c; i++;
                    c = fgetc(inf);
                    str[i] = c; i++;
                }

                else {
                    str[i] = c; i++;
                }

                break;/* case STA */

            case DONE:  break;/* case STA */

            default:        break;/* case STA */

            }

        }/* state=STA_DONE */

        str[i] = '\0';
        
        //计数
        if (type != TYPE_NOTE) {
            charCounter = charCounter + i;
        }
        if (IsKeyword(str) == 1) type = TYPE_KEYWORD;
        if (type == TYPE_KEYWORD) {
            keywordCounter++;
        }
        else if (type == TYPE_NUMBER) {
            numCounter++;
        }
        else if (type == TYPE_IDENTIFIER) {
            idCounter++;
            idAddress= InsertID(str); 
        }
        else if (type == TYPE_OPERATOR) {
            opCounter++;
        }
        else if (type == TYPE_SEPARATOR) {
            sepCounter++;
        }
        printtoFile(outf, note,error,type, str);/* DONE一次输出一次 */
    }/* feof(inf) */

}



/* 主函数 */

int main(){

    FILE* input, * output, *error, *note;

    //打开待分析的文件
    if ((input = fopen("code2.cpp", "r")) == NULL){
        printf("Cannot find the file!\nStrike any key to exit!\n");
        system("pause");
        exit(1);
    }
    //
    else{
        //文件输出格式设置
        output = fopen("output2.txt", "w");
        error = fopen("error2.txt", "w");
        note = fopen("note2.txt", "w");
        fprintf(output, "\t%s\t %s\t %s\t %s\n","行号", "单词", "类型", "记号形式");
        fprintf(note, "\t%s\t %s\n", "行号", "详情");
        fprintf(error, "%s\t %s\t %s\n","单词" ,"行号", "error");
        //执行分析程序
        LexAnalyse(input, output,error,note);
        fprintf(output, "%s\n", "----------------------总结----------------------");
        fprintf(output, "\t\t%s %d\n","总行数：",line_no);
        fprintf(output, "\t\t%s %d\n", "字符总数：", charCounter);
        fprintf(output, "\t\t%s %d\n", "关键字个数：", keywordCounter);
        fprintf(output, "\t\t%s %d\n", "标识符个数：", idCounter);
        fprintf(output, "\t\t%s %d\n", "分隔符个数：", sepCounter);
        fprintf(output, "\t\t%s %d\n", "操作符个数：", opCounter);
        fprintf(output, "\t\t%s %d\n", "常数个数：", numCounter);
        //关闭文件
        fclose(input);
        fclose(output);
        fclose(error);
        fclose(note);
        printf("分析完成\noutput.txt中记录了单词分析结果和统计\nerror.txt中记录了出错信息\nnote.txt中记录了读出的注释\n更多信息请查看相关文件\n");
        system("pause");
    }
}