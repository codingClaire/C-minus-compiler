#include "parser.h"
using namespace std;

int main(){
    ifstream cminusProgram;
    cminusProgram.open("test.cm",ios::in);
    parser A;
    A.SyntaxAnalysis(cminusProgram,true);
    cminusProgram.close();
    return 0;
}
