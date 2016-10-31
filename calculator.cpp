//
//  calculator.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/27.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//
//
// A demo for calculating all expression.
// Suppose that I have gotten the token stream and the code of all tokens.

#include "calculator.hpp"

using namespace::std;

Calculator::Calculator(MemoryStack * memoryStack){
    initialPriority();
    this->memoryStack = memoryStack;
}

int Calculator::doCalculator(vector<Token> tokens){
    stack<string> symbols;
    stack<Num> result;
    prehandlerTokens(tokens);
    for(int i = 0; i < tokens.size(); i++){
        if(find(allSymbols.begin(), allSymbols.end(), tokens[i].content) != allSymbols.end()){
            if(symbols.empty() || priority.at(tokens.at(i).content) > priority.at(symbols.top())){
                symbols.push(tokens.at(i).content);
            }else{
                while(!symbols.empty() && priority.at(tokens.at(i).content) <= priority.at(symbols.top())){
                    result.push(calculate(result, symbols.top()));
                    symbols.pop();
                }
                symbols.push(tokens.at(i).content);
            }
            
        }
        else{
            if(tokens[i].describe.compare("identf") == 0){
                result.push(Num(tokens[i].content, memoryStack->getVariable(tokens[i].content)));
            }else if(tokens[i].content.compare("__FLAG__") == 0){
                result.push(Num());
            }else{
                result.push(Num(atoi(tokens[i].content.c_str())));
            }
        }
    }
    while(!symbols.empty()){
        result.push(calculate(result, symbols.top()));
        symbols.pop();
    }
    return result.top().num;
}

void Calculator::initialPriority(){
    allSymbols.insert(allSymbols.end(), "++");
    allSymbols.insert(allSymbols.end(), "--");
    allSymbols.insert(allSymbols.end(), "*");
    allSymbols.insert(allSymbols.end(), "/");
    allSymbols.insert(allSymbols.end(), "+");
    allSymbols.insert(allSymbols.end(), "-");
    allSymbols.insert(allSymbols.end(), ">=");
    allSymbols.insert(allSymbols.end(), "<=");
    allSymbols.insert(allSymbols.end(), ">");
    allSymbols.insert(allSymbols.end(), "<");
    allSymbols.insert(allSymbols.end(), "!=");
    allSymbols.insert(allSymbols.end(), "==");
    priority.insert(pair<string, int>("++", 5));
    priority.insert(pair<string, int>("--", 5));
    priority.insert(pair<string, int>("*", 4));
    priority.insert(pair<string, int>("/", 4));
    priority.insert(pair<string, int>("+", 3));
    priority.insert(pair<string, int>("-", 3));
    priority.insert(pair<string, int>(">=", 2));
    priority.insert(pair<string, int>("<=", 2));
    priority.insert(pair<string, int>(">", 2));
    priority.insert(pair<string, int>("<", 2));
    priority.insert(pair<string, int>("==", 1));
    priority.insert(pair<string, int>("!=", 1));
}

Num Calculator::calculate(stack<Num> & result, string symbol){
    Num a = result.top();
    result.pop();
    Num b = result.top();
    result.pop();
    Num variable = b;
    if(symbol.compare("++") == 0){
        bool isA;
        if(b.name.compare("__FLAG__") == 0){
            isA = true;
            variable = a;
        }
        if(side_effect.find(variable.name) != side_effect.end()){
            int temp = side_effect.at(variable.name);
            side_effect.erase(variable.name);
            side_effect.insert(pair<string, int>(variable.name, temp + 1));
        }else{
            side_effect.insert(pair<string, int>(variable.name, 1));
        }
        return (isA) ? variable.num + 1 : variable.num;
    }else if(symbol.compare("--") == 0){
        bool isA;
        if(b.name.compare("__FLAG__") == 0){
            isA = true;
            variable = a;
        }
        if(side_effect.find(variable.name) != side_effect.end()){
            int temp = side_effect.at(variable.name);
            side_effect.erase(variable.name);
            side_effect.insert(pair<string, int>(variable.name, temp - 1));
        }else{
            side_effect.insert(pair<string, int>(variable.name, -1));
        }
        return (isA) ? variable.num - 1 : variable.num;
    }else if(symbol.compare("*") == 0){
        return Num(b.num * a.num);
    }else if(symbol.compare("/") == 0){
        return Num(b.num / a.num);
    }else if(symbol.compare("+") == 0){
        return Num(b.num + a.num);
    }else if(symbol.compare("-") == 0){
        return Num(b.num - a.num);
    }else if(symbol.compare(">=") == 0){
        return Num((int)(b.num >= a.num));
    }else if(symbol.compare("<=") == 0){
        return Num((int)(b.num <= a.num));
    }else if(symbol.compare("<") == 0){
        return Num((int)(b.num < a.num));
    }else if(symbol.compare(">") == 0){
        return Num((int)(b.num > a.num));
    }else if(symbol.compare("!=") == 0){
        return Num((int)(b.num != a.num));
    }else if(symbol.compare("==") == 0){
        return Num((int)(b.num == a.num));
    }
    return 0;
}

void Calculator::prehandlerTokens(vector<Token> & tokens){
    for(int i = 0; i < tokens.size(); i++){
        if(tokens.at(i).content.compare("++") == 0 || tokens.at(i).content.compare("--") == 0){
            if(i == 0 || find(allSymbols.begin(), allSymbols.end(), tokens[i - 1].content) != allSymbols.end()){
                tokens.insert(tokens.begin() + i, Token("__FLAG__", "Constant", -1, -1, -1));
            }else{
                tokens.insert(tokens.begin() + i + 1, Token("__FLAG__", "Constant", -1, -1, -1));
            }
            i++;
        }
    }
}

void Calculator::cleanEffect(){
    side_effect.clear();
}

Num::Num(string name, int num){
    this->name = name;
    this->num = num;
}

Num::Num(int num){
    this->name = "__NULL__";
    this->num = num;
}

Num::Num(){
    this->name = "__FLAG__";
    this->num = -1;
}

