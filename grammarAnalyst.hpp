//
//  grammarAnalyst.hpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/29.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#ifndef grammarAnalyst_hpp
#define grammarAnalyst_hpp

#include "token.hpp"
#include "memoryStack.hpp"
#include "calculator.hpp"
#include "iteratorManager.hpp"

#include <stdio.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

#define NORMAL_END 1
#define BREAK_END -1

using namespace std;

//string allTypes[8] = {"ReservedWord", "Variable", "DataType", "Symbols",
//"Border", "End", "NextLine", "Annotation"};


class GrammarAnalyst{
public:
    int analyse(vector<Token> * tokens);
    GrammarAnalyst(vector<Token> * tokens);
private:
    int handleAssignment(IteratorManager * manager);
    int handleInt(IteratorManager * manager);
    int handleIf(IteratorManager * manager);
    int handleFor(IteratorManager * manager);
    //void handleDo(IteratorManager * manager);
    int handleWhile(IteratorManager * manager);
    int handlePrintf(IteratorManager * manager);
    int handleExpression(IteratorManager * manager);
    
    int handleCurrentIt(IteratorManager * manager);
    int handleSelfChange(IteratorManager * manager);
    vector<Token>::iterator getTheEndOfIf(IteratorManager * manager);
    vector<Token>::iterator getTheEndOfWhileOrFor(IteratorManager * manager);
    //vector<Token>::iterator getTheEndOfDo(IteratorManager * manager);
    vector<Token>::iterator getTheEndOfBlock(IteratorManager * manager);
    int handleExpressionInFor(IteratorManager * manager);
    void cleanNewVariable();
    vector<Token>::iterator it;
    Calculator calculator;
    MemoryStack memoryStack;
    stack<vector<string>> newVariable;
};
#endif /* grammarAnalyst_hpp */
