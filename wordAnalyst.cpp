//
//  wordAnalyst.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/28.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#include "wordAnalyst.hpp"

//string allTypes[6] = {"ReservedWord", "Variable", "DataType", "Symbols", "Border", "End"};
//                          0              1            2           3         4       5
//string allReservedWord[8] = {"while", "if", "break", "printf", "int", "do", "else", "for"};
//string allSymbols[12] = {"+", "-", "*", "/", "++", "--", ">", "<", ">=", "<=", "!=", "=="};
//string allBorder[10] = {"{", "}", "\"", "\'", "(", ")", ",", "/*", "*/" ,"//"};
//string allEnd[2] = {";", "\n"};
//Token(string self, int line, int type);

using namespace std;

vector<Token> WordAnalyst::getTokens(){
    scanner();
    printNodeLink();
    return tokens;
}

void WordAnalyst::createNewNode(string content, string describe, int type, int addr, int line){
    Token token(content, describe, type, addr, line);
    tokens.push_back(token);
}

//返回值是新的标志符的入口地址
int WordAnalyst::createNewIden(string content, string descirbe, int type, int addr, int line){
    int flag = 0;
    int addr_temp = -2;
    for (int i = 0; i < identifiers.size(); i++)
    {
        if (content.compare(identifiers[i].content) == 0)
        {
            flag = 1;
            addr_temp = identifiers[i].addr;
        }
    }
    if (flag == 0)
    {
        addr_temp = ++static_iden_number;//用自增来模拟入口地址
    }
    Token token(content, descirbe, type, addr, line);
    identifiers.push_back(token);
    return addr_temp;
}

void WordAnalyst::printNodeLink(){
    cout << "************************************分析表******************************" << endl << endl;
    cout << setw(30) << "内容" << setw(10) << "描述" << "\t" << "种别码" << "\t" << "地址" << "\t" << "行号" << endl;
    for(int i = 0; i < tokens.size(); i++)
    {
        Token t = tokens[i];
        if (t.type == IDENTIFER)
        {
            cout << setw(30) << t.content << setw(10) << t.describe << "\t" << t.type << "\t" << t.addr << "\t" << t.line << endl;
        }
        else
        {
            cout << setw(30) << t.content << setw(10) << t.describe << "\t" << t.type << "\t" << "\t" << t.line << endl;
        }
    }
    cout << endl << endl;
}

//标志符表
void WordAnalyst::printIdentLink(){
    cout << "************************************标志符表******************************" << endl << endl;
    cout << setw(30) << "内容" << setw(10) << "描述" << "\t" << "种别码" << "\t" << "地址" << "\t" << "行号" << endl;
    for (int i = 0; i < identifiers.size(); i++)
    {
        Token t = tokens[i];
        cout << setw(30) << t.content << setw(10) << t.describe << "\t" << t.type << "\t" << t.addr << "\t" << t.line << endl;
    }
    cout << endl << endl;
}

//预处理，处理头文件和宏定义
void WordAnalyst::preProcess(string word, int line){
    string include_temp = "include";
    string define_temp = "define";
    
    int flag = 0;
    long p_include = word.find(include_temp, 0);
    if (p_include != string::npos)
    {
        flag = 1;
        int i;
        for (i = 8;;)
        {
            if (word[i] == ' ' || word[i] == '\t')
            {
                i++;
            }
            else
            {
                break;
            }
        }
        createNewNode(word.substr(i, word.length()), HEADER_DESC, HEADER, -1, line);
    }
    else
    {
        long p_define = word.find(define_temp, 0);
        if (p_define != string::npos)
        {
            flag = 1;
            int i;
            for (i = 7;;)
            {
                if (word[i] == ' ' || word[i] == '\t')
                {
                    i++;
                }
                else
                {
                    break;
                }
            }
            createNewNode(word.substr(i, word.length()), CONSTANT_DESC, MACRO_VAL, -1, line);
        }
    }
}

void WordAnalyst::close(){
    /*	delete tokens;
     delete identifiers;*/
}

int WordAnalyst::seekKey(string word){
    for (int i = 0; i<32; i++)
    {
        if (word.compare(key[i]) == 0)
        {
            return i + 1;
        }
    }
    return IDENTIFER;

}

void WordAnalyst::scanner(){
    char filename[50];
    char ch;
    char array[30];//单词长度上限是30
    char * word;
    int i;
    int line = 1;//行数
    
    
    FILE * infile;
    printf("请输入要进行语法分析的C语言程序：\n");
    scanf("%s", filename);
    infile = fopen(filename, "r");
    while (!infile)
    {
        printf("打开文件失败！\n");
        return;
    }
    ch = fgetc(infile);
    while (ch != EOF)
    {
        
        i = 0;
        //以字母或者下划线开头,处理关键字或者标识符
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_')
        {
            while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '_')
            {
                array[i++] = ch;
                ch = fgetc(infile);
            }
            word = new char[i + 1];
            memcpy(word, array, i);
            word[i] = '\0';
            int seekTemp = seekKey(word);
            if (seekTemp != IDENTIFER)
            {
                createNewNode(word, KEY_DESC, seekTemp, -1, line);
            }
            else
            {
                int addr_tmp = createNewIden(word, IDENTIFER_DESC, seekTemp, -1, line);
                createNewNode(word, IDENTIFER_DESC, seekTemp, addr_tmp, line);
            }
            fseek(infile, -1L, SEEK_CUR);//向后回退一位
        }
        //以数字开头，处理数字
        else if (ch >= '0' && ch <= '9')
        {
            int flag = 0;
            int flag2 = 0;
            //处理整数
            while (ch >= '0' && ch <= '9')
            {
                array[i++] = ch;
                ch = fgetc(infile);
            }
            //处理float
            if (ch == '.')
            {
                flag2 = 1;
                array[i++] = ch;
                ch = fgetc(infile);
                if (ch >= '0' && ch <= '9')
                {
                    while (ch >= '0' && ch <= '9')
                    {
                        array[i++] = ch;
                        ch = fgetc(infile);
                    }
                }
                else
                {
                    flag = 1;
                }
                
                //处理Double
                if (ch == 'E' || ch == 'e')
                {
                    array[i++] = ch;
                    ch = fgetc(infile);
                    if (ch == '+' || ch == '-')
                    {
                        array[i++] = ch;
                        ch = fgetc(infile);
                    }
                    if (ch >= '0' && ch <= '9')
                    {
                        array[i++] = ch;
                        ch = fgetc(infile);
                    }
                    else
                    {
                        flag = 2;
                    }
                }
                
            }
            word = new char[i + 1];
            memcpy(word, array, i);
            word[i] = '\0';
            if (flag == 1)
            {
                //createNewError(word, FLOAT_ERROR, FLOAT_ERROR_NUM, line);
            }
            else if (flag != 2)
            {
                if (flag2 == 0)
                {
                    createNewNode(word, CONSTANT_DESC, INT_VAL, -1, line);
                }
                else
                {
                    createNewNode(word, CONSTANT_DESC, FLOAT_VAL, -1, line);
                }
            }
            fseek(infile, -1L, SEEK_CUR);//向后回退一位
        }
        //以"/"开头
        else if (ch == '/')
        {
            ch = fgetc(infile);
            //处理运算符"/="
            if (ch == '=')
            {
                createNewNode("/=", OPE_DESC, COMPLETE_DIV, -1, line);
            }
            //处理"/*  */"型注释
            else if (ch == '*')
            {
                ch = fgetc(infile);
                while (1)
                {
                    while (ch != '*')
                    {
                        if (ch == '\n')
                        {
                            line++;
                        }
                        ch = fgetc(infile);
                        if (ch == EOF)
                        {
                            return;
                        }
                    }
                    ch = fgetc(infile);
                    if (ch == '/')
                    {
                        break;
                    }
                    if (ch == EOF)
                    {
                        return;
                    }
                }
                createNewNode(_NULL, NOTE_DESC, NOTE1, -1, line);
            }
            //处理"//"型注释
            else if (ch == '/')
            {
                while (ch != '\n')
                {
                    ch = fgetc(infile);
                    if (ch == EOF)
                    {
                        createNewNode(_NULL, NOTE_DESC, NOTE2, -1, line);
                        return;
                    }
                }
                line++;
                createNewNode(_NULL, NOTE_DESC, NOTE2, -1, line);
                if (ch == EOF)
                {
                    return;
                }
            }
            //处理除号
            else
            {
                createNewNode("/", OPE_DESC, DIV, -1, line);
            }
        }
        //处理常量字符串
        else if (ch == '"')
        {
            createNewNode("\"", CLE_OPE_DESC, DOU_QUE, -1, line);
            ch = fgetc(infile);
            i = 0;
            while (ch != '"')
            {
                array[i++] = ch;
                if (ch == '\n')
                {
                    line++;
                }
                ch = fgetc(infile);
                if (ch == EOF)
                {
                    return;
                }
            }
            word = new char[i + 1];
            memcpy(word, array, i);
            word[i] = '\0';
            createNewNode(word, CONSTANT_DESC, STRING_VAL, -1, line);
            createNewNode("\"", CLE_OPE_DESC, DOU_QUE, -1, line);
        }
        //处理常量字符
        else if (ch == '\'')
        {
            createNewNode("\'", CLE_OPE_DESC, SIN_QUE, -1, line);
            ch = fgetc(infile);
            i = 0;
            while (ch != '\'')
            {
                array[i++] = ch;
                if (ch == '\n')
                {
                    line++;
                }
                ch = fgetc(infile);
                if (ch == EOF)
                {
                    return;
                }
            }
            word = new char[i + 1];
            memcpy(word, array, i);
            word[i] = '\0';
            createNewNode(word, CONSTANT_DESC, CHAR_VAL, -1, line);
            createNewNode("\'", CLE_OPE_DESC, SIN_QUE, -1, line);
        }
        else if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
        {
            if (ch == '\n')
            {
                line++;
            }
        }
        else
        {
            if (ch == EOF)
            {
                return;
            }
            //处理头文件和宏常量（预处理）
            else if (ch == '#')
            {
                while (ch != '\n' && ch != EOF)
                {
                    array[i++] = ch;
                    ch = fgetc(infile);
                }
                word = new char[i + 1];
                memcpy(word, array, i);
                word[i] = '\0';
                preProcess(word, line);
                
                fseek(infile, -1L, SEEK_CUR);//向后回退一位
            }
            //处理-开头的运算符
            else if (ch == '-')
            {
                array[i++] = ch;
                ch = fgetc(infile);
                if (ch >= '0' && ch <= '9')
                {
                    int flag = 0;
                    int flag2 = 0;
                    //处理负数整数
                    while (ch >= '0' && ch <= '9')
                    {
                        array[i++] = ch;
                        ch = fgetc(infile);
                    }
                    //处理负数float
                    if (ch == '.')
                    {
                        flag2 = 1;
                        array[i++] = ch;
                        ch = fgetc(infile);
                        if (ch >= '0' && ch <= '9')
                        {
                            while (ch >= '0' && ch <= '9')
                            {
                                array[i++] = ch;
                                ch = fgetc(infile);
                            }
                        }
                        else
                        {
                            flag = 1;
                        }
                        //处理负数Double
                        if (ch == 'E' || ch == 'e')
                        {
                            array[i++] = ch;
                            ch = fgetc(infile);
                            if (ch == '+' || ch == '-')
                            {
                                array[i++] = ch;
                                ch = fgetc(infile);
                            }
                            if (ch >= '0' && ch <= '9')
                            {
                                array[i++] = ch;
                                ch = fgetc(infile);
                            }
                            else
                            {
                                flag = 2;
                            }
                        }
                    }
                    word = new char[i + 1];
                    memcpy(word, array, i);
                    word[i] = '\0';
                    
                    if (flag != 1 && flag != 2)
                    {
                        if (flag2 == 0)
                        {
                            createNewNode(word, CONSTANT_DESC, INT_VAL, -1, line);
                        }
                        else
                        {
                            createNewNode(word, CONSTANT_DESC, FLOAT_VAL, -1, line);
                        }
                    }
                    fseek(infile, -1L, SEEK_CUR);//向后回退一位
                }
                else if (ch == '>')
                {
                    createNewNode("->", OPE_DESC, ARROW, -1, line);
                }
                else if (ch == '-')
                {
                    createNewNode("--", OPE_DESC, SELF_SUB, -1, line);
                }
                else if (ch == '=')
                {
                    createNewNode("--", OPE_DESC, SELF_SUB, -1, line);
                }
                else
                {
                    createNewNode("-", OPE_DESC, SUB, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            //处理+开头的运算符
            else if (ch == '+')
            {
                ch = fgetc(infile);
                if (ch == '+')
                {
                    createNewNode("++", OPE_DESC, SELF_ADD, -1, line);
                }
                else if (ch == '=')
                {
                    createNewNode("+=", OPE_DESC, COMPLETE_ADD, -1, line);
                }
                else
                {
                    createNewNode("+", OPE_DESC, ADD, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            //处理*开头的运算符  
            else if (ch == '*')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("*=", OPE_DESC, COMPLETE_MUL, -1, line);
                }
                else
                {
                    createNewNode("*", OPE_DESC, MUL, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            //处理按^开头的运算符  
            else if (ch == '^')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("^=", OPE_DESC, COMPLETE_BYTE_XOR, -1, line);
                }
                else
                {
                    createNewNode("^", OPE_DESC, BYTE_XOR, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            //处理%开头的运算符  
            else if (ch == '%')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("%=", OPE_DESC, COMPLETE_MOD, -1, line);
                }
                else
                {
                    createNewNode("%", OPE_DESC, MOD, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            //处理&开头的运算符  
            else if (ch == '&')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("&=", OPE_DESC, COMPLETE_BYTE_AND, -1, line);
                }
                else if (ch == '&')
                {
                    createNewNode("&&", OPE_DESC, AND, -1, line);
                }
                else
                {
                    createNewNode("&", OPE_DESC, BYTE_AND, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            //处理~开头的运算符  
            else if (ch == '~')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("~=", OPE_DESC, COMPLETE_COMPLEMENT, -1, line);
                }
                else
                {
                    createNewNode("~", OPE_DESC, COMPLEMENT, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            //处理!开头的运算符  
            else if (ch == '!')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("!=", OPE_DESC, NOT_EQUAL, -1, line);
                }
                else
                {
                    createNewNode("!", OPE_DESC, NOT, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            //处理<开头的运算符  
            else if (ch == '<')
            {
                ch = fgetc(infile);
                if (ch == '<')
                {
                    createNewNode("<<", OPE_DESC, LEFT_MOVE, -1, line);
                }
                else if (ch == '=')
                {
                    createNewNode("<=", OPE_DESC, LES_EQUAL, -1, line);
                }
                else
                {
                    createNewNode("<", OPE_DESC, LES_THAN, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            //处理>开头的运算符  
            else if (ch == '>')
            {
                ch = fgetc(infile);
                if (ch == '>')
                {
                    createNewNode(">>", OPE_DESC, RIGHT_MOVE, -1, line);
                }
                else if (ch == '=')
                {
                    createNewNode(">=", OPE_DESC, GRT_EQUAL, -1, line);
                }
                else
                {
                    createNewNode(">", OPE_DESC, GRT_THAN, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            //处理|开头的运算符  
            else if (ch == '|')
            {
                ch = fgetc(infile);
                if (ch == '|')
                {
                    createNewNode("||", OPE_DESC, OR, -1, line);
                }
                else
                {
                    createNewNode("|", OPE_DESC, BYTE_OR, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            else if (ch == '=')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("==", OPE_DESC, EQUAL, -1, line);
                }
                else
                {
                    createNewNode("=", OPE_DESC, ASG, -1, line);
                    fseek(infile, -1L, SEEK_CUR);
                }
            }
            else if (ch == '(')
            {
                leftSmall++;
                lineBra[0][leftSmall] = line;
                createNewNode("(", CLE_OPE_DESC, LEFT_BRA, -1, line);
            }
            else if (ch == ')')
            {
                rightSmall++;
                lineBra[1][rightSmall] = line;
                createNewNode(")", CLE_OPE_DESC, RIGHT_BRA, -1, line);
            }
            else if (ch == '[')
            {
                leftMiddle++;
                lineBra[2][leftMiddle] = line;
                createNewNode("[", CLE_OPE_DESC, LEFT_INDEX, -1, line);
            }
            else if (ch == ']')
            {
                rightMiddle++;
                lineBra[3][rightMiddle] = line;
                createNewNode("]", CLE_OPE_DESC, RIGHT_INDEX, -1, line);
            }
            else if (ch == '{')
            {
                leftBig++;
                lineBra[4][leftBig] = line;
                createNewNode("{", CLE_OPE_DESC, L_BOUNDER, -1, line);
            }
            else if (ch == '}')
            {
                rightBig++;
                lineBra[5][rightBig] = line;
                createNewNode("}", CLE_OPE_DESC, R_BOUNDER, -1, line);
            }
            else if (ch == '.')
            {
                createNewNode(".", CLE_OPE_DESC, POINTER, -1, line);
            }
            else if (ch == ',')
            {
                createNewNode(",", CLE_OPE_DESC, COMMA, -1, line);
            }
            else if (ch == ';')
            {
                createNewNode(";", CLE_OPE_DESC, SEMI, -1, line);
            }
            else
            {
                char temp[2];
                temp[0] = ch;
                temp[1] = '\0';
            }
        }
        ch = fgetc(infile);
    }
}