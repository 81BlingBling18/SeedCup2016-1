//
//  iteratorManager.hpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/29.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#ifndef iteratorManager_hpp
#define iteratorManager_hpp

#include "token.hpp"

#include <stdio.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

using namespace std;

class IteratorManager{
public:
    IteratorManager(vector<Token> *tokens);
    vector<Token>::iterator getIt();
    void move(int step);
    void moveTo(vector<Token>::iterator aim);
    void jump(int step);
    void jumpTo(vector<Token>::iterator aim);
    vector<Token>::iterator getEnd();
private:
    vector<Token>::iterator it;
    vector<Token> tokens;
    vector<Token> *origin;
    vector<Token>::iterator tokensBegin;
    vector<Token>::iterator originBegin;
    int lastLine;
    void printLine();
    vector<Token>::iterator transferIn(vector<Token>::iterator aim);
    vector<Token>::iterator transferOut(vector<Token>::iterator aim);
};

#endif /* iteratorManager_hpp */
