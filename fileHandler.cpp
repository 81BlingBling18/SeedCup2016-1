//
//  fileHandler.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/27.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#include "fileHandler.hpp"

using namespace std;

string FileHandler::getFromFile(){
    string codes;
    in.open("/Users/tangyifeng/input.txt", ios::in);
    printf("#%d\n", in.is_open());
    while(getline(in, codes)){
    }
    in.close();
    return codes;
}

void FileHandler::saveToFile(string result){
    out.open("/Users/tangyifeng/input.txt", ios::out);
    out << result;
    out.close();
}