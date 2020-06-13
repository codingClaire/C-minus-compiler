#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

class parser{
    public:
        void SyntaxAnalysis(ifstream &cminusProgram,bool printFlag);
    private:
        TreeNode* match(TokenType expected,int i);
        bool isMatched(TokenType expected,int i);
        bool isMatchedRelop(int i);
        bool isMatchedAddop(int i);
        bool isMatchedMulop(int i);
        TreeNode* newStmtNode(StmtKind kind,int i);
        TreeNode* newTokenNode(TokenType e,int i);

        TreeNode* program();
        TreeNode* declaration_list();
        TreeNode* declaration();
        TreeNode* diff_declaration();
        TreeNode* var_declaration();
        TreeNode* type_specifier();
        TreeNode* params();
        TreeNode* param_list();
        TreeNode* param();
        TreeNode* compound_stmt();
        TreeNode* local_declarations();
        TreeNode* statement_list();
        TreeNode* statement();
        TreeNode* expression_stmt();
        TreeNode* selection_stmt();
        TreeNode* iteration_stmt();
        TreeNode* return_stmt();
        TreeNode* expression();
        TreeNode* left_expression();
        TreeNode* left_part();
        TreeNode* simple_expression();
        TreeNode* simple_expression1();
        TreeNode* simple_expression2();
        TreeNode* simple_expression3();
        TreeNode* additive_expression();
        TreeNode* additive_expression1();
        TreeNode* additive_expression2();
        TreeNode* additive_expression3();
        TreeNode* term();
        TreeNode* term1();
        TreeNode* term2();
        TreeNode* term3();
        TreeNode* factor();
        TreeNode* factor_part();
        TreeNode* factor3();
        TreeNode* relop();
        TreeNode* addop();
        TreeNode* mulop();
        TreeNode* call();
        TreeNode* args();
        TreeNode* arg_list();
        void printTree(TreeNode *tree);
        string getNode(TreeNode* tree);
        void getLink(TreeNode* root,fstream &fs);
        void getGraph(TreeNode* root,fstream &fs);
};
#endif