//
//  grammarAnalyst.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/29.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#include "grammarAnalyst.hpp"

GrammarAnalyst::GrammarAnalyst(vector<Token> * tokens):calculator(&memoryStack){
    calculator = Calculator(&memoryStack);
}

//
// Analyse a part of codes
//

int GrammarAnalyst::analyse(vector<Token> * tokens){
    int breakFlag;
    IteratorManager manager(tokens);
    newVariable.push(vector<string>());
    for(manager.jumpTo(tokens->begin()); manager.getIt() != tokens->end(); ){
        breakFlag = handleCurrentIt(&manager);
        if(breakFlag == BREAK_END)
            return breakFlag;
    }
    cleanNewVariable();
    return NORMAL_END;
}

//
// Analyse a block of codes
// Return -1 when getting break
//

int GrammarAnalyst::handleCurrentIt(IteratorManager * manager){
    if(manager->getIt()->describe.compare("keyword") == 0){
        if(manager->getIt()->content.compare("while") == 0){
            return handleWhile(manager);
        }
        else if(manager->getIt()->content.compare("if") == 0){
            return handleIf(manager);
        }
        else if(manager->getIt()->content.compare("for") == 0){
            return handleFor(manager);
        }
        else if(manager->getIt()->content.compare("int") == 0){
            return handleInt(manager);
        }
        else if(manager->getIt()->content.compare("do") == 0){
            //handleDo(&manager);
        }
        else if(manager->getIt()->content.compare("break") == 0){
            for(int i = 0; i < newVariable.top().size(); i++){
                memoryStack.popVariable(newVariable.top()[i]);
            }
            newVariable.pop();
            manager->move(1);
            return BREAK_END;
        }
    }
    else if(manager->getIt()->describe.compare("identf") == 0){
        if(manager->getIt()->content.compare("printf") == 0){
            return handlePrintf(manager);
        }
        else if((manager->getIt() + 2)->describe.compare("boundary") == 0){
            return handleSelfChange(manager);
        }
        else{
            return handleAssignment(manager);
        }
    }else if(manager->getIt()->describe.compare("operator") == 0){
        return handleSelfChange(manager);
    }
    else if(manager->getIt()->describe.compare("note") == 0){
        manager->jump(1);
    }
    return NORMAL_END;
}

//
// Handle self change
//

int GrammarAnalyst::handleSelfChange(IteratorManager * manager){
    handleExpression(manager);
    manager->jump(1);
    return NORMAL_END;
}

//
// Handle assignment
//

int GrammarAnalyst::handleAssignment(IteratorManager * manager){
    bool isLast = true;
    vector<Token>::iterator temp = manager->getIt();
    do{
        manager->jump(1);
        if(manager->getIt()->content.compare("=") == 0)
            isLast = false;
    }while(manager->getIt()->content.compare(";") != 0 && manager->getIt()->content.compare(",") != 0 );
    manager->jumpTo(temp);
    if(isLast){
        int value = handleExpression(manager);
        manager->jump(1);
        return value;
    }
    else{
        string name = manager->getIt()->content;
        manager->move(2);
        int value = handleAssignment(manager);
        memoryStack.setVariable(name, value);
        return value;
    }
}

//
// Handle int
//

int GrammarAnalyst::handleInt(IteratorManager * manager){
    vector<string> * variableInState = &newVariable.top();
    manager->jump(1);
    while(manager->getIt()->describe.compare("identf") == 0){
        variableInState->insert(variableInState->end(), manager->getIt()->content);
        memoryStack.pushVariable(manager->getIt()->content, 0);
        if((manager->getIt() + 1)->content == ";"){
            manager->jump(2);
            break;
        }
        else if((manager->getIt() + 1)->content == "="){
            handleAssignment(manager);
            if((manager->getIt() - 1)->content == ";")
                break;
        }
        else if((manager->getIt() + 1)->content == ","){
            manager->jump(2);
        }
    }
    return NORMAL_END;
}

//
// Handle if
//

int GrammarAnalyst::handleIf(IteratorManager * manager){
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfIfBorder = false;
    bool expression;
    vector<Token>::iterator startOfIf = manager->getIt();
    vector<Token>::iterator endOfIt;
    
    // To get the end iterator of if block
    endOfIt = getTheEndOfBlock(manager);
    manager->jumpTo(startOfIf);
    
    manager->jump(2);
    expression = (handleExpression(manager) == 0) ? false : true;
    manager->jump(1);
    if(manager->getIt()->content == "{"){
        flagOfIfBorder = true;
        manager->jump(1);
    }
    if(!flagOfIfBorder){
        if(expression){
            if(handleCurrentIt(manager) == BREAK_END)
                return BREAK_END;
        }
        else{
            while((manager->getIt() + 1)->line == manager->getIt()->line){
                manager->jump(1);
            }
            manager->jump(1);
            if(manager->getIt()->content.compare("else") == 0){
                manager->jump(1);
                if(manager->getIt()->content.compare("{") != 0){
                    if(handleCurrentIt(manager) == BREAK_END)
                        return BREAK_END;
                }
                else{
                    manager->jump(1);
                    while(!(manager->getIt()->content == "}" && stack == 0)){
                        if(manager->getIt()->content == "{")
                            stack++;
                        else if(manager->getIt()->content == "}")
                            stack--;
                        subTokens.insert(subTokens.end(), *manager->getIt());
                        manager->jump(1);
                    }
                    manager->jump(1);
                    if(analyse(&subTokens) == BREAK_END)
                        return BREAK_END;
                }
            }
        }
    }else{
        if(!expression){
            while(!(manager->getIt()->content == "}" && stack == 0)){
                if(manager->getIt()->content == "{")
                    stack++;
                else if(manager->getIt()->content == "}")
                    stack--;
                subTokens.insert(subTokens.end(), *manager->getIt());
                manager->jump(1);
            }
            stack = 0;
            manager->jump(1);
            
            if((manager->getIt() != manager->getEnd()) && manager->getIt()->content.compare("else") == 0){
                manager->jump(1);
                if(manager->getIt()->content.compare("{") != 0){
                    if(handleCurrentIt(manager) == BREAK_END)
                        return BREAK_END;
                }
                else{
                    manager->jump(1);
                    while(!(manager->getIt()->content == "}" && stack == 0)){
                        if(manager->getIt()->content == "{")
                            stack++;
                        else if(manager->getIt()->content == "}")
                            stack--;
                        subTokens.insert(subTokens.end(), *manager->getIt());
                        manager->jump(1);
                    }
                    stack = 0;
                    manager->jump(1);
                    if(analyse(&subTokens) == BREAK_END)
                        return BREAK_END;
                }
            }
        }
        else{
            while(!(manager->getIt()->content == "}" && stack == 0)){
                if(manager->getIt()->content == "{")
                    stack++;
                else if(manager->getIt()->content == "}")
                    stack--;
                subTokens.insert(subTokens.end(), *manager->getIt());
                manager->jump(1);
            }
            stack = 0;
            manager->jump(1);
            if(analyse(&subTokens) == BREAK_END)
                return BREAK_END;
        }
    }
    manager->jumpTo(endOfIt);
    return NORMAL_END;
}


// At next token
/*
void GrammarAnalyst::handleDo(){
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfBorder = false;
    bool flagOfRead = false;
    vector<Token>::iterator oldIt;
    it++;
    do{
        if()
    }
}
 */

//
// Handle for
//

int GrammarAnalyst::handleFor(IteratorManager * manager){
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfBorder = false;
    bool flagOfRead = false;
    
    vector<Token>::iterator startOfFor = manager->getIt();
    vector<Token>::iterator endOfFor;
    endOfFor = getTheEndOfBlock(manager);
    manager->jumpTo(startOfFor);
    
    vector<Token>::iterator startOfCondition;
    vector<Token>::iterator startOfThirdExp;
    vector<Token>::iterator startOfContent;
    while(manager->getIt()->content.compare(";") != 0)
        manager->jump(1);
    manager->jump(1);
    startOfCondition = manager->getIt();
    
    while(manager->getIt()->content.compare(";") != 0)
        manager->jump(1);
    manager->jump(1);
    startOfThirdExp = manager->getIt();
    while(manager->getIt()->content.compare(")") != 0)
        manager->jump(1);
    startOfContent = manager->getIt() + 1;
    manager->jumpTo(startOfFor);
    
    newVariable.push(vector<string>());
    manager->move(2);
    for(handleExpressionInFor(manager); manager->jumpTo(startOfCondition), handleExpression(manager);
        manager->jumpTo(startOfThirdExp),handleExpressionInFor(manager)){
        manager->jumpTo(startOfContent);
        if(manager->getIt()->content == "{"){
            flagOfBorder = true;
            manager->jump(1);
        }
        if(!flagOfBorder){
            if(handleCurrentIt(manager) == BREAK_END)
                break;
        }
        else {
            if(!flagOfRead){
                while(!(manager->getIt()->content == "}" && stack == 0)){
                    if(manager->getIt()->content == "{")
                        stack++;
                    else if(manager->getIt()->content == "}")
                        stack--;
                    subTokens.insert(subTokens.end(), *manager->getIt());
                    manager->jump(1);
                }
                flagOfRead = true;
            }
            if(analyse(&subTokens) == BREAK_END)
                break;
            cout << (startOfFor + 2)->line << " ";
        }
        manager->jumpTo(startOfFor + 2);
        
    }
    manager->jumpTo(endOfFor);
    cleanNewVariable();
    return NORMAL_END;
}

int GrammarAnalyst::handleExpressionInFor(IteratorManager * manager){
    while(true){
        handleCurrentIt(manager);
        if((manager->getIt() - 1)->content.compare(";") == 0 || (manager->getIt() - 1)->content.compare(")") == 0)
            break;
    }
    return NORMAL_END;
}

//
// Handle while
//

int GrammarAnalyst::handleWhile(IteratorManager * manager){
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfBorder = false;
    bool flagOfRead = false;
    
    vector<Token>::iterator startOfWhile = manager->getIt();
    vector<Token>::iterator endOfWhile;
    endOfWhile = getTheEndOfWhileOrFor(manager);
    manager->jumpTo(startOfWhile);
    
    manager->move(2);
    while(handleExpression(manager) == 1){
        manager->jump(1);
        if(manager->getIt()->content == "{"){
            flagOfBorder = true;
            manager->jump(1);
        }
        if(!flagOfBorder){
            if(handleCurrentIt(manager) == BREAK_END)
                break;
        }
        else {
            if(!flagOfRead){
                while(!(manager->getIt()->content == "}" && stack == 0)){
                    if(manager->getIt()->content == "{")
                        stack++;
                    else if(manager->getIt()->content == "}")
                        stack--;
                    subTokens.insert(subTokens.end(), *manager->getIt());
                    manager->jump(1);
                }
                flagOfRead = true;
            }
            if(analyse(&subTokens) == BREAK_END)
                break;
            cout << (startOfWhile + 2)->line << " ";
        }
        manager->jumpTo(startOfWhile + 2);
        
    }
    manager->jumpTo(endOfWhile);
    return NORMAL_END;
}

vector<Token>::iterator GrammarAnalyst::getTheEndOfWhileOrFor(IteratorManager * manager){
    int stack = 0;
    while(manager->getIt()->content.compare(")") != 0)
        manager->jump(1);
    manager->jump(1);
    if(manager->getIt()->content.compare("{") == 0){
        while(!(manager->getIt()->content == "}" && stack == 1)){
            if(manager->getIt()->content == "{")
                stack++;
            else if(manager->getIt()->content == "}")
                stack--;
            manager->jump(1);
        }
        manager->jump(1);
        
    }
    else{
        manager->jumpTo(getTheEndOfBlock(manager));
    }
    return manager->getIt();
}

vector<Token>::iterator GrammarAnalyst::getTheEndOfIf(IteratorManager * manager){
    int stack = 0;
    while(manager->getIt()->content.compare(")") != 0)
        manager->jump(1);
    manager->jump(1);
    if(manager->getIt()->content.compare("{") == 0){
        while(!(manager->getIt()->content == "}" && stack == 1)){
            if(manager->getIt()->content == "{")
                stack++;
            else if(manager->getIt()->content == "}")
                stack--;
            manager->jump(1);
        }
        stack = 0;
        manager->jump(1);
    }
    else{
        manager->jumpTo(getTheEndOfBlock(manager));
    }
    while((manager->getIt() != manager->getEnd()) && (manager->getIt()->content.compare("else") == 0)){
        manager->jump(1);
        if(manager->getIt()->content.compare("if") == 0){
            while(manager->getIt()->content.compare(")") != 0)
                manager->jump(1);
            manager->jump(1);
        }
        if(manager->getIt()->content.compare("{") == 0){
            while(!(manager->getIt()->content == "}" && stack == 1)){
                if(manager->getIt()->content == "{")
                    stack++;
                else if(manager->getIt()->content == "}")
                    stack--;
                manager->jump(1);
            }
            stack = 0;
            manager->jump(1);
        }
        else{
            manager->jumpTo(getTheEndOfBlock(manager));
        }
    }
    return manager->getIt();
}


vector<Token>::iterator GrammarAnalyst::getTheEndOfBlock(IteratorManager * manager){
    if(manager->getIt()->content.compare("if") == 0){
        return getTheEndOfIf(manager);
    }
    else if(manager->getIt()->content.compare("for") == 0 || manager->getIt()->content.compare("while") == 0){
        return getTheEndOfWhileOrFor(manager);
    }
    else{
        while(manager->getIt()->content.compare(";") != 0)
            manager->jump(1);
        return manager->getIt() + 1;
    }
}


//
// Handle printf
//

int GrammarAnalyst::handlePrintf(IteratorManager * manager){
    manager->move(5);
    while(manager->getIt()->content == ","){
        manager->move(1);
        handleExpression(manager);
    }
    manager->jump(2);
    return NORMAL_END;
}

//
// Handle expression
//

int GrammarAnalyst::handleExpression(IteratorManager * manager){
    vector<Token> expression;
    int value;
    while(manager->getIt()->describe != "boundary"){
        expression.insert(expression.end(), *manager->getIt());
        manager->move(1);
    }
    value =  calculator.doCalculator(expression);
    memoryStack.handleSideEffect(calculator.side_effect);
    calculator.cleanEffect();
    return value;
}

void GrammarAnalyst::cleanNewVariable(){
    for(int i = 0 ; i < newVariable.top().size(); i++){
        memoryStack.popVariable(newVariable.top()[i]);
    }
    newVariable.pop();
}
