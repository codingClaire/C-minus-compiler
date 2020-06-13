#ifndef LEXER_H
#define LEXER_H
#include "globals.h"

extern vector<lexicalUnit> lexicalUnitList;
void LexicalAnalysis(std::ifstream &cminusProgram,bool printFlag);
void printLexicalUnit(lexicalUnit s);
#endif 