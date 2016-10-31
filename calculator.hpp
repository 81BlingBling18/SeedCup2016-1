//
//  calculator.hpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/27.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#ifndef calculator_hpp
#define calculator_hpp

#include "token.hpp"
#include "memoryStack.hpp"

#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

using namespace std;

class Num{
public:
    string name;
    int num;
    Num(string name, int num);
    Num(int num);
    Num();
};

class Calculator{
public:
    // The side effect in case of ++ or --
    // <int, int> just for test
    Calculator(MemoryStack * memory);
    map<string, int> side_effect;
    int doCalculator(vector<Token> tokens);
    //Clean Effect after every step of handle effect!!!
    void cleanEffect();
private:
    // The set of all possible symbols
    vector<string> allSymbols;
    // The priority of all symbols
    map<string, int> priority;
    void initialPriority();
    // Return the value of expression (num or 1/0)
    Num calculate(stack<Num> & result, string symbol);
    // Add a flag for ++ and --
    void prehandlerTokens(vector<Token> & tokens);
    MemoryStack * memoryStack;
};

#endif /* calculator_hpp */
