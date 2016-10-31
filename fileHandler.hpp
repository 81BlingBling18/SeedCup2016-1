//
//  fileHandler.hpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/27.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#ifndef fileHandler_hpp
#define fileHandler_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

using namespace std;

class FileHandler{
public:
    void saveToFile(string result);
    string getFromFile();
private:
    ifstream in;
    ofstream out;
};

#endif /* fileHandler_hpp */
