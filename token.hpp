//
//  token.hpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/28.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#ifndef token_hpp
#define token_hpp

#include <stdio.h>

#include <stdio.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

using namespace std;

#define AUTO 1
#define BREAK 2
#define CASE 3
#define CHAR 4
#define CONST 5
#define CONTINUE 6
#define DEFAULT 7
#define DO 8
#define DOUBLE 9
#define ELSE 10
#define ENUM 11
#define EXTERN 12
#define FLOAT 13
#define FOR 14
#define GOTO 15
#define IF 16
#define INT 17
#define LONG 18
#define REGISTER 19
#define RETURN 20
#define SHORT 21
#define SIGNED 22
#define SIZEOF 23
#define STATIC 24
#define STRUCT 25
#define SWITCH 26
#define TYPEDEF 27
#define UNION 28
#define UNSIGNED 29
#define VOID 30
#define VOLATILE 31
#define WHILE 32
#define KEY_DESC "keyword"

//标志符
#define IDENTIFER 40
#define IDENTIFER_DESC "identf"

//常量
#define INT_VAL 51 //整形常量
#define CHAR_VAL 52 //字符常量
#define FLOAT_VAL 53 //浮点数常量
#define STRING_VAL 54 //双精度浮点数常量
#define MACRO_VAL 55 //宏常量
#define CONSTANT_DESC "constant"

/* 运算符 */
#define NOT 61   // !
#define BYTE_AND 62 //&
#define COMPLEMENT 63 // ~
#define BYTE_XOR  64 // ^
#define MUL 65 // *
#define DIV 66// /
#define MOD 67 // %
#define ADD 68 // +
#define SUB 69 // -
#define LES_THAN 70 // <
#define GRT_THAN 71 // >
#define ASG 72 // =
#define ARROW 73 // ->
#define SELF_ADD 74 // ++
#define SELF_SUB 75 // --
#define LEFT_MOVE 76 // <<
#define RIGHT_MOVE 77 // >>
#define LES_EQUAL 78 // <=
#define GRT_EQUAL 79 // >=
#define EQUAL 80 // ==
#define NOT_EQUAL 81 // !=
#define AND 82 // &&
#define OR 83 // ||
#define COMPLETE_ADD 84 // +=
#define COMPLETE_SUB 85 // -=
#define COMPLETE_MUL 86 // *=
#define COMPLETE_DIV 87 // /=
#define COMPLETE_BYTE_XOR 88 // ^=
#define COMPLETE_BYTE_AND 89 // &=
#define COMPLETE_COMPLEMENT 90 // ~=
#define COMPLETE_MOD 91 //%=
#define BYTE_OR 92 // |

#define OPE_DESC "operator"

//限界符
#define LEFT_BRA 100 // (
#define RIGHT_BRA 101 // )
#define LEFT_INDEX 102 // [
#define RIGHT_INDEX 103 // ]
#define L_BOUNDER 104 //  {
#define R_BOUNDER 105 // }
#define POINTER 106 // .
#define JING 107 // #
#define UNDER_LINE 108 // _
#define COMMA 109 // ,
#define SEMI 110 // ;
#define SIN_QUE 111 // '
#define DOU_QUE 112 // "

#define CLE_OPE_DESC "boundary"

#define NOTE1 120 // "/**/"注释
#define NOTE2 121 // "//"注释
#define NOTE_DESC "note"


#define HEADER 130 //头文件
#define HEADER_DESC "header"

#define _NULL "null"


/*  type: variable
 *  name: a
 *  value: 1
 *  line: 1 
 */

class Token{
public:
    string describe;
    string content;
    int type;//种别码, 上面的宏定义
    int addr;//入口地址
    int line;
    Token (string content, string describe, int type, int addr, int line)
    {
        this->content = content;
        this->describe = describe;
        this->type = type;
        this->addr = addr;
        this->line = line;
    }
};

#endif /* token_hpp */
