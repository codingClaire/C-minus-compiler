#ifndef _GLOBALS_H_
#define _GLOBALS_H_
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
using namespace std;
#define MAXRESERVED 6
#define MAXCHILDREN 5

typedef enum
{
    ENDFILE,ERROR,IF,ELSE,RETURN,WHILE,
    VOID,INT,ID,NUM,
    SMALL,LARGE,NOLARGE,NOSMALL, //< > <= >=
    ASSIGN,EQUAL,NOTEQUAL, //= == !=
    PLUS,MINUS,TIMES,OVER, //+ - * /
    LPAREN,RPAREN,LBRACKET,RBRACKET,//() []
    LSBRACKET,RSBRACKET, //[]
    SEMI,COMMA, // ; ,
    LCOMMENT, // /*
    RCOMMENT,// */
    BLANK,
} TokenType;

typedef enum
{
    START,INNUM,INID,
    INSLASH, INASSIGN,
    INEQUAL,INSMALL,INLARGE,
    INCOMMENT1,INCOMMENT2,
    INNOEQUAL,DONE,
} StateType; //状态

struct lexicalUnit
{
    string originstring; //原字符串
    TokenType type;      //类型
    int lineNo;          //行号
    lexicalUnit(string o, TokenType t, int l)
    {
        originstring = o;
        type = t;
        lineNo = l;
    };
};

extern int lineno;
typedef enum
{
    NONTERMINAL,
    TERMINAL
} NodeKind;

typedef enum
{
    PROGRAM,DECLARATION_LIST,
    DECLARATION,DIFF_DECLARATION,
    VAR,SELECTION_STMT,
    ITERATION_STMT,RETURN_STMT,
    COMPOUND_STMT,PARAMS,
    PARAM_LIST,PARAM,
    LOCAL_DECLARATION,STATEMENT_LIST,
    STATEMENT,EXPRESSION_STMT,
    VAR_DECLARATION,FUN_DECLARATION,
    TYPE_SPECIFIER,EXPRESSION,
    LEFT_EXPRESSION,LEFT_PART,
    SIMPLE_EXPRESSION1,ADDITIVE_EXPRESSION1,
    TERM1,SIMPLE_EXPRESSION2,
    ADDITIVE_EXPRESSION2,
    TERM2,SIMPLE_EXPRESSION3,
    SIMPLE_EXPRESSION,ADDITIVE_EXPRESSION3,
    TERM3,FACTOR3,ADDITIVE_EXPRESSION,
    TERM,FACTOR,FACTOR_PART,ARGS,
    ARG_LIST,RELOP,ADDOP,MULOP,
} StmtKind;


typedef struct treeNode
{
    struct treeNode *child[MAXCHILDREN];
    struct treeNode *sibling;
    NodeKind nodekind;
    int lineNo;
    union {
        StmtKind stmt;
        TokenType token;
    } kind;
    union {
        int val;
        const char *name;
    } attr;
    int label;
} TreeNode;

#endif