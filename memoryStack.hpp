//
//  memoryStack.hpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/27.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#ifndef memoryStack_hpp
#define memoryStack_hpp

#include <stdio.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

using namespace std;

class MemoryStack{
public:
    map<string, stack<int>> memory;
    
    //Pop a shate of variable
    void popVariable(string name);
    //Push a new state of variable
    void pushVariable(string name, int value);
    //Reset the value of a variable in the very state
    void setVariable(string name, int value);
    int getVariable(string name);
    //Handle the effect and DO NOT forget to use cleanEffect in Calculator
    void handleSideEffect(map<string, int> sideEffect);
private:
};

#endif /* memoryStack_hpp */
