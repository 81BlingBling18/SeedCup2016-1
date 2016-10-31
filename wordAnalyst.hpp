//
//  wordAnalyst.hpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/28.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#ifndef wordAnalyst_hpp
#define wordAnalyst_hpp

#include "token.hpp"
#include <stdio.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>
#include <iomanip>
#include <cstring>

using namespace std;

class WordAnalyst{
public:
    vector<Token> getTokens();
private:
    int leftSmall = 0;//左小括号
    int rightSmall = 0;//右小括号
    int leftMiddle = 0;//左中括号
    int rightMiddle = 0;//右中括号
    int leftBig = 0;//左大括号
    int rightBig = 0;//右大括号
    int lineBra[6][1000] = { 0 };//括号和行数的对应关系，第一维代表左右6种括号
    int static_iden_number = 0;//模拟标志符的地址，自增
    vector<Token> tokens;
    vector<Token> identifiers;
    void scanner();
    void createNewNode(string content, string describe, int type, int addr, int line);
    int createNewIden(string content, string descirbe, int type, int addr, int line);
    void printNodeLink();
    void printIdentLink();
    void preProcess(string word, int line);
    void close();
    int seekKey(string word);
    string key[32] = { "auto","break","case","char","const","continue","default","do","double",
        "else","enum","extern","float","for","goto","if","int","long","register",
        "return","short","signed","sizeof","static","struct","switch","typedef",
        "union","unsigned","void","volatile","while"
    };
};

#endif /* wordAnalyst_hpp */
