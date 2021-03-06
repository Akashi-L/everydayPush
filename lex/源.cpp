#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include<map>

//�ؼ��֡��ָ��������������СԤ����

#define MAX_STR 65535

#define KEYWORDS_SIZE 32

#define OPERATORS_SIZE 18

#define SEPARATORS_SIZE 15

//���൥�ʵ�ǰ����״̬

#define START 1                 //��ʼ����

#define ANL_IDORKEYWORD 2       //�����Ǳ�ʶ�����ǹؼ���

#define ANL_NUMBER 3            //��������

#define ANL_NOTE 4              //����ע�ͣ���������ע�ͺͶ���ע��

#define ANL_LITERAL 5           //�������ż�����ַ���

#define DONE  6                 //�������

//��������
#define TYPE_KEYWORD 1          //�ؼ���

#define TYPE_IDENTIFIER 2       //��ʶ��

#define TYPE_NUMBER 3           //����

#define TYPE_NOTE 4             //ע��

#define TYPE_LITERAL 5          //���ż���ַ���

#define TYPE_OPERATOR 6         //�����

#define TYPE_SEPARATOR 7        //�ָ���

#define TYPE_ERROR 8            //����

#define TYPE_ERRORID 9          //��ʶ������

#define TYPE_ERRORNUM 10        //��������

#define TYPE_UNKNOWN 11         //δ֪

#define TYPE_ENDFILE 12         //�ļ�����

//�ؼ��֡��ָ�������������
const char* Operators[OPERATORS_SIZE] = { "+","-","*","/","%","=","==","!=","<","<=",">",">=","+=","*=" ,"/=","-=","++","--"};

const char* Separators[SEPARATORS_SIZE] = { ",",";",".","\'","\"","(",")","[","]","{","}","//","/*","*/","#" };

const char* Keywords[KEYWORDS_SIZE] = { "int" , "long" , "short" , "float" , "double" , "char" , "unsigned" , "signed" , "const" , "void" , "volatile" , "enum" , "struct" , "union" , "if" , "else" , "goto" , "switch" , "case" , "do" , "while" , "for" , "continue" , "break" , "return" , "default" , "typedef" , "auto" , "register" , "extern" , "static" , "sizeof" };

//���ű�
std::map<std::string, int>ID;
std::map<std::string, int>::iterator itr;


char c=' ';//��ǰ�����ַ�
int line_no = 1;//�к�
int charCounter = 0;//�ַ�����
int idCounter = 0;//��ʶ������
int numCounter = 0;//��������
int keywordCounter = 0;//�ؼ��ָ���
int opCounter = 0;//����������
int sepCounter = 0;//�ָ�������
int idAddress = 0;//��ʶ���ڱ�ʶ�����е�λ��

//�ж��Ƿ�Ϊ������
int IsOperator(char c){
    for (int i = 0; i < OPERATORS_SIZE; i++)
        if (Operators[i][0] == c)
            return 1;
    return 0;
}

//�ж��Ƿ�Ϊ�ָ���
int IsSeparator(char c){
    for (int i = 0; i < SEPARATORS_SIZE; i++)
        if (Separators[i][0] == c)
            return 1;
    return 0;
}

//�ж��Ƿ��ǹؼ���
int IsKeyword(char* str){
    for (int i = 0; i < KEYWORDS_SIZE; i++)
        if (strcmp(Keywords[i], str) == 0)
            return 1;
    return 0;
}

//�����ĸ�͵����Ƿ��ڷ��ű��У����û�оͼ���
int InsertID(char* str) {  
    //�����Ƿ���ڵ�ǰ�ַ���
    itr = ID.find(str);
    if (itr != ID.end())
        return ID[str];

    else {
        ID[str] = ID.size();
        return ID[str];
    }
}

//��ͬ���ļ��д�ӡ���������outf�ļ���������ʷ��������ͳ�ƣ�note�ļ������ע�ͣ�error�ļ�����������Ĵ���
void printtoFile(FILE* outf, FILE* note,FILE* error, int type, char* str){
    //��Ϊkeyword��id��һ������ģ����ｫ���������ֿ�
    if (IsKeyword(str) == 1) type = TYPE_KEYWORD;

    switch (type) {

    case TYPE_KEYWORD:    fprintf(outf,"\t%d\t %s\t %s\t %s %s %s\n", line_no,str,"�ؼ���","<",str,",->"); break;

    case TYPE_IDENTIFIER: fprintf(outf,"\t%d\t %s\t %s\t %s %d %s\n ", line_no,str, "��ʶ��", "<id,",idAddress,">"); break;

    case TYPE_NUMBER:     fprintf(outf,"\t%d\t %s\t %s\t %s %s %s\n", line_no,str, "����", "<num,",str,">"); break;

    case TYPE_NOTE: {

        fprintf(note, "%s\t %d\t %s\n","NOTE:",line_no,str);

         } break;

    case TYPE_LITERAL:   fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "�ַ���", "<", str, ",->"); break;

    case TYPE_OPERATOR: {
        if(strcmp(str,"=") == 0){
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "������", "<", "assign-op", ",->");
        }
        else if (strcmp(str, "<") == 0) {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "������", "<", str, ",LT>");
        }
        else if (strcmp(str, "<=") == 0) {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "������", "<", str, ",LE>");
        }
        else if (strcmp(str, "==") == 0) {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "������", "<", str, ",EQ>");
        }
        else if (strcmp(str, ">") == 0) {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "������", "<", str, ",GT>");
        }
        else if (strcmp(str, ">=") == 0) {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "������", "<", str, ",GE>");
        }
        else {
            fprintf(outf, "\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "������", "<", str, ",->");
        }
        
    } break;

    case TYPE_SEPARATOR:  fprintf(outf,"\t%d\t %s\t %s\t %s %s %s\n ", line_no, str, "�ָ���", "<", str, ",->"); break;

    case TYPE_ERROR:      fprintf(error, "%s\t %d\t %s\n", str, line_no, "����"); break;

    case TYPE_ERRORID:     fprintf(error, "%s\t %d\t %s\n", str, line_no, "��ʶ����������"); break;

    case TYPE_ERRORNUM:     fprintf(error, "%s\t %d\t %s\n", str, line_no, "С����ʽ����"); break;

    default:break;

    }
}

//�ʷ�������
void LexAnalyse(FILE* inf, FILE* outf,FILE* error, FILE* note){
   
    //ÿ�ζ�����ַ���
    char str[MAX_STR];
    //ָʾ������ַ�Ӧ����str�����д���±�
    int i;
    //stateΪ��ǰ�������ʵ�״̬��typeΪ�������ĵ��ʿ��ܵ�����
    int state, type;
    //��ע�ͱ�־
    char flag_one_line_note;
    //�����޶�С����ֻ����һ��С���������ܻ���ִ���
    char flag_had_got_dot;

    //�����ļ���������������
    while (!feof(inf)){
        //ÿ�ο�ʼ��һ�����ʵĶ�ȡʱ����i
        i = 0;
        //����Ϊ��ʼ״̬
        state = START;
        //���ñ�־
        flag_one_line_note = 0;

        flag_had_got_dot = 0;

        //�����ַ�ֱ�����ʷ�������
        while (state != DONE){
            //����һ���ַ�
            c = fgetc(inf);
            //����state״̬���д���
            switch (state){

            //��ʼ��������״̬
            case START:

                //������ǿո��
                if(c == ' ' || c == '\t');
                else if (c == '\n'){
                    line_no++;
                }
                //��������ַ����»���
                else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')||c == '_'){
                    state = ANL_IDORKEYWORD;
                    type = TYPE_IDENTIFIER;
                    str[i] = c; i++;

                }
                //�����������
                else if (c >= '0' && c <= '9'){
                    state = ANL_NUMBER;
                    type = TYPE_NUMBER;
                    str[i] = c; i++;
                }
                //�������/���Ϸ��������������Ҫô��ע��Ҫô�ǲ�����
                else if (c == '/'){
                    str[i] = c; i++;
                    //��ǰ���룬�Ա��ж�/��������ʲô
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
                        fseek(inf, -1, SEEK_CUR);/* �ļ���ָ��ǰ��1���ֽ�  */
                        i = 1;/* str[1]='\0'; */
                    }

                }
                //�����������ַ��Ĳ�����Ҫ��ǰ����Ĳ��������жϣ�����<=,>=,++�ȣ������ǰ����������������жϵ�һ����������������

                else if (c == '<' || c == '>'){
                    type = TYPE_OPERATOR;
                    str[0] = c;
                    //��ǰ����
                    c = fgetc(inf);
                    
                    if (c == '='){
                        str[1] = '='; i = 2;/* str[2]='\0'    */
                        state = DONE;
                    }

                    else{
                        fseek(inf, -1, SEEK_CUR);/* �ļ���ָ��ǰ��1���ֽ�  */
                        i = 1;/* str[1]='\0'; */
                        state = DONE;
                    }
                }

                //����������++,+=
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
                        fseek(inf, -1, SEEK_CUR);/* �ļ���ָ��ǰ��1���ֽ�  */
                        i = 1;/* str[1]='\0'; */
                        state = DONE;
                    }
                }
                //����������--��-=
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
                    fseek(inf, -1, SEEK_CUR);/* �ļ���ָ��ǰ��1���ֽ�  */
                    i = 1;/* str[1]='\0'; */
                    state = DONE;
                    }
                }
                //����������*=
                else if (c == '*') {
                type = TYPE_OPERATOR;
                str[0] = c;
                c = fgetc(inf);
                if (c == '=') {

                    str[1] = '='; i = 2;/* str[2]='\0'    */
                    state = DONE;

                }

                else {
                    fseek(inf, -1, SEEK_CUR);/* �ļ���ָ��ǰ��1���ֽ�  */
                    i = 1;/* str[1]='\0'; */
                    state = DONE;
                    }
                }
                //������������=
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
                        fseek(inf, -1, SEEK_CUR);/* �ļ���ָ��ǰ��1���ֽ�  */
                        i = 1;/* str[1]='\0'; */
                    }

                }
                //��������Ҫ��ǰ����Ĳ�����
                else if (IsOperator(c)) {
                    state = DONE;
                    type = TYPE_OPERATOR;
                    str[0] = c; i = 1;/* str[1]='\0'; */
                }

                //�������ż���ַ�������
                else if (c == '\"' || c == '\'') {
                    state = ANL_LITERAL;
                    type = TYPE_LITERAL;
                    str[0] = c; i = 1;
                }
                //�����ָ���
                else if (IsSeparator(c)) {
                    state = DONE;
                    type = TYPE_SEPARATOR;
                    str[0] = c; i = 1;/* str[1]='\0'; */
                }
                //����������eof���������
                else if (c == EOF){

                    state = DONE;
                    type = TYPE_ENDFILE;
                }

                else{
                    state = DONE;
                    type = TYPE_UNKNOWN;
                }

                break;/* case STA */

            //���������ʶ���͹ؼ�������
            case ANL_IDORKEYWORD:
                //�Ϸ�����
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'){
                    str[i] = c; i++;
                }
                //������ǿո�ȴ���ǰ���ʶ��������
                else if (c == ' ' || c == '\t' || c == '\n' || IsOperator(c) || IsSeparator(c)) {
                    state = DONE;
                    fseek(inf, -1, SEEK_CUR);/* �ļ���ָ��ǰ��1���ֽ�  */
                }
                //error
                else{
                    str[i] = c; i++;
                    type = TYPE_ERRORID;      
                }
                break;/* case STA */
            
            //��������״̬
            case ANL_NUMBER:
                //�Ϸ�����
                if (c >= '0' && c <= '9') {
                    str[i] = c; i++;
                }
                //С��
                else if (c == '.') {
                    str[i] = c; i++;
                    c = fgetc(inf);
                    //һ��С����
                    if (flag_had_got_dot == 0) {   
                        if (c >= '0' && c <= '9') {
                            str[i] = c; i++;
                            flag_had_got_dot = 1;
                        }
                        //���С����Ƿ�
                        else {
                            fseek(inf, -1, SEEK_CUR);/* �ļ���ָ��ǰ��1���ֽ�  */
                            
                            type = TYPE_ERRORNUM;         /*  �� 1..h  ���ΪERROR���� */  
                        }
                    }
                    else {
                        type = TYPE_ERROR;
                        str[i] = c; i++;
                    }
                }
                //ָ��
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
                //�Ƚϵ��͵�error
                else if ((c >= 'a' && c < 'e')||( c>'e' && c<='z') || (c >= 'A' && c<'E')||( c>'E'&& c <= 'Z') || c == '_') {
                    type = TYPE_ERROR;
                    if(type==TYPE_ERROR){
                        type = TYPE_ERRORID;
                    }
                    str[i] = c; i++;
                }
                //��ǰ���ʶ������
                else if (c == ' ' || c == '\t' || c == '\n' || IsOperator(c) || IsSeparator(c)) {
                    state = DONE;
                    fseek(inf, -1, SEEK_CUR);/* �ļ���ָ��ǰ��1���ֽ�  */
             
                }
                //error
                else {
                    str[i] = c; i++;
                    type = TYPE_ERROR;
                 
                }

                break;/* case STA */
            
            //����ע��״̬
            case ANL_NOTE:
                //����ע�����
                if (flag_one_line_note == 1) {
                    if (c == '\n') {
                        state = DONE;
                        fseek(inf, -1, SEEK_CUR);/* �ļ���ָ��ǰ��1���ֽ� */
                    }
                    //û�������оͼ�����
                    else{
                        str[i] = c; i++;
                    }

                }
                //����ע�����/**/
                else {
                    //error
                    if (feof(inf)){
                        state = DONE;
                        type = TYPE_ERROR;
                    }
                    //��������
                    else if (c == '\n'){
                        line_no++;
                        str[i] = c; i++;
                    }
                    //�ж϶���ע���Ƿ����
                    else if (c == '*') {
                        str[i] = c; i++;
                        c = fgetc(inf);
                        if (c == '/'){
                            state = DONE;
                            str[i] = c; i++;
                        }
                        //����ע�͵Ľ������֣���*/��������             
                        else{
                            if (feof(inf)) {
                                state = DONE;
                                type = TYPE_ERROR;
                            }
                            fseek(inf, -1, SEEK_CUR);/* �ļ���ָ��ǰ��1���ֽ�  */
                        }
                    }

                    else {
                        //����ע�ͼ�����
                        str[i] = c; i++;
                    }
                }

                break;/* case STA */
            //�������ż���ַ�������
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
                //����
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
        
        //����
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
        printtoFile(outf, note,error,type, str);/* DONEһ�����һ�� */
    }/* feof(inf) */

}



/* ������ */

int main(){

    FILE* input, * output, *error, *note;

    //�򿪴��������ļ�
    if ((input = fopen("code2.cpp", "r")) == NULL){
        printf("Cannot find the file!\nStrike any key to exit!\n");
        system("pause");
        exit(1);
    }
    //
    else{
        //�ļ������ʽ����
        output = fopen("output2.txt", "w");
        error = fopen("error2.txt", "w");
        note = fopen("note2.txt", "w");
        fprintf(output, "\t%s\t %s\t %s\t %s\n","�к�", "����", "����", "�Ǻ���ʽ");
        fprintf(note, "\t%s\t %s\n", "�к�", "����");
        fprintf(error, "%s\t %s\t %s\n","����" ,"�к�", "error");
        //ִ�з�������
        LexAnalyse(input, output,error,note);
        fprintf(output, "%s\n", "----------------------�ܽ�----------------------");
        fprintf(output, "\t\t%s %d\n","��������",line_no);
        fprintf(output, "\t\t%s %d\n", "�ַ�������", charCounter);
        fprintf(output, "\t\t%s %d\n", "�ؼ��ָ�����", keywordCounter);
        fprintf(output, "\t\t%s %d\n", "��ʶ��������", idCounter);
        fprintf(output, "\t\t%s %d\n", "�ָ���������", sepCounter);
        fprintf(output, "\t\t%s %d\n", "������������", opCounter);
        fprintf(output, "\t\t%s %d\n", "����������", numCounter);
        //�ر��ļ�
        fclose(input);
        fclose(output);
        fclose(error);
        fclose(note);
        printf("�������\noutput.txt�м�¼�˵��ʷ��������ͳ��\nerror.txt�м�¼�˳�����Ϣ\nnote.txt�м�¼�˶�����ע��\n������Ϣ��鿴����ļ�\n");
        system("pause");
    }
}