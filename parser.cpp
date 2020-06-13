#include "parser.h"
int current = 0;
static int indentno = -2;
static bool printTreeFlag = true;
static int count = 0;
static int terminalcount = 0;
#define INDENT indentno += 2
#define UNINDENT indentno -= 2
map<StmtKind, string> NonTerminalDict = {
    {SELECTION_STMT, "selection_stmt"},
    {PROGRAM, "program"},
    {DECLARATION_LIST, "declaration_list"},
    {ITERATION_STMT, "iteration_stmt"},
    {RETURN_STMT, "return_stmt"},
    {COMPOUND_STMT, "compound_stmt"},
    {EXPRESSION_STMT, "expression_stmt"},
    {VAR_DECLARATION, "var_declaration"},
    {TYPE_SPECIFIER, "type_specifier"},
    {DIFF_DECLARATION, "diff_declaration"},
    {VAR, "var"},
    {DECLARATION, "declaration"},
    {PARAMS, "params"},
    {PARAM_LIST, "param_list"},
    {PARAM, "param"},
    {LOCAL_DECLARATION, "local_declaration"},
    {STATEMENT_LIST, "statement_list"},
    {EXPRESSION, "expression"},
    {LEFT_EXPRESSION, "expression"},
    {LEFT_PART, "leftpart"},
    {SIMPLE_EXPRESSION1, "simple_expression1"},
    {ADDITIVE_EXPRESSION1, "additive_expression1"},
    {TERM1, "term1"},
    {TERM2, "term2"},
    {SIMPLE_EXPRESSION2, "simple_expression2"},
    {ADDITIVE_EXPRESSION2, "additive_Expression2"},
    {SIMPLE_EXPRESSION3, "simple_expression3"},
    {ADDITIVE_EXPRESSION3, "additive_expression3"},
    {TERM3, "term3"},
    {FACTOR3, "factor3"},
    {ADDITIVE_EXPRESSION, "additive_expression"},
    {FACTOR, "factor"},
    {TERM, "term"},
    {FACTOR_PART, "factor_part"},
    {ARGS, "args"},
    {ARG_LIST, "arg_list"},
    {RELOP, "relop"},
    {ADDOP, "addop"},
    {MULOP, "mulop"},
    {SIMPLE_EXPRESSION, "simple_expression"},
    {STATEMENT, "statement"}};

map<TokenType, string> TerminalDict = {
    {ENDFILE, "EOF"},
    {ERROR, "ERROR"},
    {INT, "INT"},
    {VOID, "VOID"},
    {IF, "IF"},
    {ELSE, "ELSE"},
    {RETURN, "RETURN"},
    {WHILE, "WHILE"},
    {VOID, "VOID"},
    {NUM, "NUM"},
    {ID, "ID"},
    {SMALL, "SMALL"},
    {LARGE, "LARGE"},
    {NOLARGE, "NOLARGE"},
    {NOSMALL, "NOSMALL"},
    {ASSIGN, "ASSIGN"},
    {EQUAL, "EQUAL"},
    {NOTEQUAL, "NOTEQUAL"},
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {TIMES, "TIMES"},
    {OVER, "OVER"},
    {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},
    {LBRACKET, "LBRACKET"},
    {RBRACKET, "RBRACKET"},
    {LSBRACKET, "LSBRACKET"},
    {RSBRACKET, "RSBRACKET"},
    {SEMI, "SEMI"},
    {COMMA, "COMMA"},
    {LCOMMENT, "LCOMMENT"},
    {RCOMMENT, "RCOMMENT"},
};

static void syntaxError(string message, int i)
{
    printTreeFlag = false;
    cout << ">>>" << endl;
    cout << ">>>Syntax error at line " << lexicalUnitList[i].lineNo << ": " << message << endl;
}

TreeNode *parser::match(TokenType expected, int i)
{
    if (lexicalUnitList[i].type == expected)
    {
        current++;
        return newTokenNode(expected, i);
    }
    else
    {
        printTreeFlag = false;
        return NULL;
    }
}

bool parser::isMatched(TokenType expected, int i)
{
    return lexicalUnitList[i].type == expected;
}

TreeNode *parser::newTokenNode(TokenType e, int i)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL)
        cout << "Out of memory error at line:" << lexicalUnitList[i].lineNo;
    else
    {
        for (int j = 0; j < MAXCHILDREN; j++)
            t->child[j] = NULL;
        t->sibling = NULL;
        t->kind.token = e;
        t->nodekind = TERMINAL;
        t->lineNo = lexicalUnitList[i].lineNo;
        t->label = count;
        count++;
        if (e == NUM)
        {
            t->attr.val = stoi(lexicalUnitList[i].originstring);
        }
        else
        {
            t->attr.name = lexicalUnitList[i].originstring.c_str();
        }
    }
    return t;
}

TreeNode *parser::newStmtNode(StmtKind kind, int i)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL)
        cout << "Out of memory error at line:" << lexicalUnitList[i].lineNo;
    else
    {
        for (int j = 0; j < MAXCHILDREN; j++)
            t->child[j] = NULL;
        t->sibling = NULL;
        t->nodekind = NONTERMINAL;
        t->kind.stmt = kind;
        t->label = count;
        count++;
        t->lineNo = lexicalUnitList[i].lineNo;
    }
    return t;
}

bool parser::isMatchedRelop(int i)
{
    return lexicalUnitList[i].type == NOSMALL || lexicalUnitList[i].type == NOLARGE || lexicalUnitList[i].type == SMALL || lexicalUnitList[i].type == LARGE || lexicalUnitList[i].type == EQUAL || lexicalUnitList[i].type == NOTEQUAL;
}

bool parser::isMatchedAddop(int i)
{
    return lexicalUnitList[i].type == PLUS || lexicalUnitList[i].type == MINUS;
}

bool parser::isMatchedMulop(int i)
{
    return lexicalUnitList[i].type == TIMES || lexicalUnitList[i].type == OVER;
}

//产生式开始

TreeNode *parser::program()
{

    TreeNode *t = newStmtNode(PROGRAM, current);
    t->child[0] = declaration_list();
    return t;
}

TreeNode *parser::declaration_list()
{
    //declaration_list -> declaration{declaration}
    TreeNode *t = newStmtNode(DECLARATION_LIST, current);
    t->child[0] = declaration();
    TreeNode *p = t->child[0];
    while (lexicalUnitList[current].type == INT || lexicalUnitList[current].type == VOID)
    {
        TreeNode *q = declaration();
        if (q != NULL)
        {
            p->sibling = q;
            p = q;
        }
        else
            break;
    }
    return t;
}

TreeNode *parser::declaration()
{
    // declaration -> type_specifier "ID" diff_declaration
    TreeNode *t = newStmtNode(DECLARATION, current);
    TreeNode *p = t->child[0] = type_specifier();
    if (p != NULL)
    {
        if (isMatched(ID, current))
        {
            TreeNode *q = t->child[1] = match(ID, current);

            t->child[2] = diff_declaration();
            if (t->child[2] == NULL)
            {
                syntaxError("Missing Declaration Part!", current);
                return NULL;
            }
        }
        else
        {
            syntaxError("Unexpected Name in Declaration!", current);
            return NULL;
        }
    }
    else
    {
        syntaxError("Unexpected Type in Declaration!", current);
        return NULL;
    }
    return t;
}

TreeNode *parser::diff_declaration()
{

    TreeNode *t = newStmtNode(DIFF_DECLARATION, current);
    if (isMatched(LSBRACKET, current))
    { //[ 数组
        TreeNode *p = t->child[0] = match(LSBRACKET, current);
        if (isMatched(NUM, current))
        {
            TreeNode *q = match(NUM, current); //NUM
            p->sibling = q;
            if (isMatched(RSBRACKET, current))
            {
                TreeNode *s = match(RSBRACKET, current); //]
                q->sibling = s;
                p = s;
            }
            else
            {
                syntaxError("missing ] in Array Declaration!", current);
                return NULL;
            }
        }
        else
        {
            syntaxError("missing Number of Array Size in Array Declaration!", current);
            return NULL;
        }
        //多维数组的情况
        while (isMatched(LSBRACKET, current))
        {
            TreeNode *m = match(LSBRACKET, current); //[
            p->sibling = m;
            if (isMatched(NUM, current))
            {
                TreeNode *q = match(NUM, current); //NUM
                m->sibling = q;
                if (isMatched(RSBRACKET, current))
                {
                    TreeNode *s = match(RSBRACKET, current); //]
                    q->sibling = s;
                    p = s;
                }
                else
                {
                    syntaxError("missing ] in Array Declaration!", current);
                    return NULL;
                }
            }
            else
            {
                syntaxError("missing Number of Array Size in Array Declaration!", current);
                return NULL;
            }
        }
        if (isMatched(SEMI, current))
            p->sibling = match(SEMI, current);
        else
        {
            syntaxError("missing ; in Declaration!", current);
            return NULL;
        }
    }
    else if (isMatched(SEMI, current))
    { //非数组
        t->child[0] = match(SEMI, current);
    }
    else if (isMatched(LPAREN, current))
    { //( 函数
        t->child[0] = match(LPAREN, current);
        if (isMatched(VOID, current) || isMatched(INT, current))
        {
            t->child[1] = params();
            if (t->child[1] != NULL && isMatched(RPAREN, current))
            {
                t->child[2] = match(RPAREN, current);
                t->child[3] = compound_stmt();
                if (t->child[3] == NULL)
                {
                    syntaxError("missing Function Body in Function Declaration!", current);
                    return NULL;
                }
            }
            else
            {
                syntaxError("missing ) in Function Declaration !", current);
                return NULL;
            }
        }
        else
        {
            syntaxError("unexpected Params in Function Declaration ", current);
            return NULL;
        }
    }
    else
    { //其他
        syntaxError("unexpected token -> ", current);
        return NULL;
    }
    return t;
}

TreeNode *parser::var_declaration()
{
    //var_declaration -> type_specifier "ID" [ "[NUM]"]
    TreeNode *t = newStmtNode(VAR_DECLARATION, current);
    t->child[0] = type_specifier();
    if (t->child[0] == NULL)
    {
        syntaxError("Unexpected Type in Declaration!", current);
        return NULL;
    }
    TreeNode *p = t->child[1] = match(ID, current);
    if (t->child[1] == NULL)
    {
        syntaxError("Unexpected Name in Declaration!", current);
        return NULL;
    }
    while (isMatched(LSBRACKET, current))
    {
        TreeNode *q = match(LSBRACKET, current); //[
        TreeNode *r = match(NUM, current);       //NUM
        if (r == NULL)
        {
            syntaxError("missing Number of Array Size in Array Declaration!", current);
            return NULL;
        }
        TreeNode *s = match(RSBRACKET, current); //]
        if (s == NULL)
        {
            syntaxError("missing ] in Array Declaration!", current);
            return NULL;
        }
        p->sibling = q;
        q->sibling = r;
        r->sibling = s;
        p = s;
    }
    p->sibling = match(SEMI, current);
    if (p->sibling == NULL)
    {
        syntaxError("missing ; in Var Declaration!", current);
        return NULL;
    }
    return t;
}

TreeNode *parser::type_specifier()
{
    //type_specifier -> "INT" | "VOID"
    TreeNode *t = newStmtNode(TYPE_SPECIFIER, current);
    if (isMatched(INT, current) || isMatched(VOID, current))
    {
        t->child[0] = match(lexicalUnitList[current].type, current);
    }
    else
    {
        syntaxError("Unexpected Token in type_specifier!", current);
        return NULL;
    }
    return t;
}

TreeNode *parser::params()
{
    //params -> param_list  |  "VOID"
    TreeNode *t = newStmtNode(PARAMS, current);
    if (isMatched(VOID, current))
    {
        t->child[0] = match(VOID, current);
    }
    else if (isMatched(INT, current))
    {
        t->child[0] = param_list();
    }
    else
    {
        syntaxError("Unexpected Params Type!", current);
        return NULL;
    }
    return t;
}

TreeNode *parser::param_list()
{
    //param_list -> param {, param}
    TreeNode *t = newStmtNode(PARAM_LIST, current);
    if (isMatched(INT, current))
    {
        t->child[0] = param();
        TreeNode *p = t->child[0];
        while (isMatched(COMMA, current))
        {
            TreeNode *q = match(COMMA, current); //,
            if (isMatched(INT, current))
            {
                TreeNode *s = param(); //param
                if (s != NULL)
                {
                    p->sibling = q;
                    q->sibling = s;
                    p = s;
                }
            }
        }
    }
    return t;
}

TreeNode *parser::param()
{
    //param -> INT ID ["[" "]"]
    TreeNode *t = newStmtNode(PARAM, current);
    if (isMatched(INT, current))
    {
        t->child[0] = match(INT, current); //INT
        if (isMatched(ID, current))
        {
            t->child[1] = match(ID, current); //ID
            if (isMatched(LSBRACKET, current))
            {
                t->child[2] = match(LSBRACKET, current); //[
                if (isMatched(LSBRACKET, current))
                    t->child[3] = match(RSBRACKET, current); //]
                else
                {
                    syntaxError("missing ] in Array Parameter!", current);
                    return NULL;
                }
            }
        }
        else
        {
            syntaxError("missing Parameter Name!", current);
            return NULL;
        }
    }
    else
    {
        syntaxError("missing Parameter Type!", current);
        return NULL;
    }
    return t;
}

TreeNode *parser::compound_stmt()
{
    //compound_stmt -> "{" local_declarations statement_list "}"
    TreeNode *t = newStmtNode(COMPOUND_STMT, current);
    t->child[0] = match(LBRACKET, current);
    if (t->child[0] == NULL)
    {
        syntaxError("Missing { in compound_stmt!", current);
        return NULL;
    }
    t->child[1] = local_declarations();
    if (t->child[1] == NULL)
    {
        syntaxError("Unmatched local_declarations in compound_stmt!", current);
        return NULL;
    }
    t->child[2] = statement_list();
    if (t->child[2] == NULL)
    {
        syntaxError("unmatched statement_list in compound_stmt!", current);
        return NULL;
    }
    t->child[3] = match(RBRACKET, current);
    if (t->child[3] == NULL)
    {
        syntaxError("Missing } in compound_stmt!", current);
        return NULL;
    }
    return t;
}

TreeNode *parser::local_declarations()
{
    //local_declarations ->{var_declaration}
    TreeNode *t = newStmtNode(LOCAL_DECLARATION, current);
    if (isMatched(VOID, current) || isMatched(INT, current))
    {
        TreeNode *p = t->child[0] = var_declaration();
        if (t->child[0] == NULL)
        {
            syntaxError("Unmatched var_declaration in local_declarations!", current);
            return NULL;
        }
        while (isMatched(VOID, current) || isMatched(INT, current))
        {
            TreeNode *q = var_declaration();
            if (q == NULL)
            {
                syntaxError("Unmatched var_declaration in local_declarations!", current);
                return NULL;
            }
            p->sibling = q;
            p = q;
        }
    }
    return t;
}

TreeNode *parser::statement_list()
{
    //statement_list ->{statement}
    TreeNode *t = newStmtNode(STATEMENT_LIST, current);
    if (isMatched(ID, current) || isMatched(LPAREN, current) || isMatched(IF, current) ||
        isMatched(LBRACKET, current) || isMatched(WHILE, current) || isMatched(RETURN, current))
    {
        TreeNode *p = t->child[0] = statement();
        while (isMatched(ID, current) || isMatched(LPAREN, current) || isMatched(IF, current) ||
               isMatched(LBRACKET, current) || isMatched(WHILE, current) || isMatched(RETURN, current))
        {
            TreeNode *q = statement();
            p->sibling = q;
            p = q;
        }
    }
    return t;
}

TreeNode *parser::statement()
{
    //statement -> expression_stmt | compound_stmt |
    //              selection_stmt |iteration_stmt | return_stmt
    TreeNode *t = newStmtNode(STATEMENT, current);
    switch (lexicalUnitList[current].type)
    {
    case ID:
    case NUM:
    case LPAREN:
        t->child[0] = expression_stmt();
        if (t->child[0] == NULL)
        {
            syntaxError("Unmatched express_stmt in statement!", current);
            return NULL;
        }
        break;
    case IF:
        t->child[0] = selection_stmt();
        if (t->child[0] == NULL)
        {
            syntaxError("Unmatched selection_stmt in statement!", current);
            return NULL;
        }
        break;
    case WHILE:
        t->child[0] = iteration_stmt();
        if (t->child[0] == NULL)
        {
            syntaxError("Unmatched iteration_stmt in statement!", current);
            return NULL;
        }
        break;
    case RETURN:
        t->child[0] = return_stmt();
        if (t->child[0] == NULL)
        {
            syntaxError("Unmatched return_stmt in statement!", current);
            return NULL;
        }
        break;
    case LBRACKET:
        t->child[0] = compound_stmt();
        if (t->child[0] == NULL)
        {
            syntaxError("Unmatched compound_stmt in statement!", current);
            return NULL;
        }
        break;
    default:
        syntaxError("Unexpected Statement Token!", current);
        break;
    }
    return t;
}

TreeNode *parser::expression_stmt()
{
    //expression_stmt -> {expression ;};
    TreeNode *t = newStmtNode(EXPRESSION_STMT, current);
    if (isMatched(ID, current) || isMatched(LPAREN, current) || isMatched(NUM, current))
    {
        t->child[0] = expression();
        t->child[1] = match(SEMI, current);
    }
    TreeNode *p = t->child[1];
    while (isMatched(ID, current) || isMatched(LPAREN, current) || isMatched(NUM, current))
    {
        TreeNode *q = expression();
        TreeNode *s = match(SEMI, current);
        p->sibling = q;
        q->sibling = s;
        p = s;
    }
    return t;
}

TreeNode *parser::selection_stmt()
{
    //selection_stmt ->"IF" ( expression ) statement ["ELSE" statement]
    TreeNode *t = newStmtNode(SELECTION_STMT, current);
    t->child[0] = match(IF, current);
    if (t->child[0] == NULL)
    {
        syntaxError("Missing IF Symbol in Selection Statement!", current);
        return NULL;
    }
    t->child[1] = match(LPAREN, current);
    if (t->child[1] == NULL)
    {
        syntaxError("Missing ( in Selection Statement!", current);
        return NULL;
    }
    t->child[2] = expression();
    if (t->child[2] == NULL)
    {
        syntaxError("Unmatched expression in Selection Statement!", current);
        return NULL;
    }
    t->child[3] = match(RPAREN, current);
    if (t->child[3] == NULL)
    {
        syntaxError("Missing ) in Selection Statement!", current);
        return NULL;
    }
    TreeNode *p = t->child[4] = statement();
    if (t->child[4] == NULL)
    {
        syntaxError("Unmatched statement in Selection Statement!", current);
        return NULL;
    }
    if (isMatched(ELSE, current))
    {
        TreeNode *s = match(ELSE, current);
        p->sibling = s;
        TreeNode *k = statement();
        if (k == NULL)
        {
            syntaxError("Unmatched statement in Selection Statement!", current);
            return NULL;
        }
        s->sibling = k;
    }
    return t;
}

TreeNode *parser::iteration_stmt()
{
    //iteration_stmt -> "WHILE" ( expression ) statement
    TreeNode *t = newStmtNode(ITERATION_STMT, current);
    t->child[0] = match(WHILE, current);
    t->child[1] = match(LPAREN, current);
    if (t->child[1] == NULL)
    {
        syntaxError("missing ( in iteration_stmt!", current);
        return NULL;
    }
    t->child[2] = expression();
    if (t->child[2] == NULL)
    {
        syntaxError("Unmatched expression in iteration_stmt!", current);
        return NULL;
    }
    t->child[3] = match(RPAREN, current);
    if (t->child[3] == NULL)
    {
        syntaxError("missing ) in iteration_stmt!", current);
        return NULL;
    }
    t->child[4] = statement();
    if (t->child[4] == NULL)
    {
        syntaxError("Unmatched statement in iteration_stmt!", current);
        return NULL;
    }
    return t;
}

TreeNode *parser::return_stmt()
{
    //return_stmt -> RETURN  [expression]
    TreeNode *t = newStmtNode(RETURN_STMT, current);
    t->child[0] = match(RETURN, current);
    t->child[1] = expression();
    if (t->child[1] == NULL)
    {
        t->child[1] = match(SEMI, current);
        if (t->child[1] == NULL)
        {
            syntaxError("Missing ; in Return!", current);
            return NULL;
        }
    }
    else
    {
        t->child[2] = match(SEMI, current);
        if (t->child[2] == NULL)
        {
            syntaxError("Missing ; in Return!", current);
            return NULL;
        }
    }
    return t;
}

TreeNode *parser::expression()
{
    //expression -> "ID" left_expression | simple_expression3
    TreeNode *t = newStmtNode(EXPRESSION, current);
    if (isMatched(ID, current))
    {
        t->child[0] = match(ID, current);
        TreeNode *p = t->child[1] = left_expression();
        if (p == NULL)
        {
            syntaxError("Unmatched left_expression in expression!", current);
            return NULL;
        }
    }
    else if (isMatched(LPAREN, current) || isMatched(NUM, current))
    {
        t->child[0] = simple_expression3();
        if (t->child[0] == NULL)
        {
            syntaxError("Unmatched simple_expression3 in expression!", current);
            return NULL;
        }
    }
    //expression是可空的，在return;的情况下
    return t;
}

TreeNode *parser::left_expression()
{
    //left_expression->[ "[" expression "]" ] left_part | simple_expression2
    TreeNode *t = newStmtNode(LEFT_EXPRESSION, current);
    if (isMatched(ASSIGN, current) || isMatchedMulop(current) || isMatchedRelop(current) || isMatchedAddop(current))
    {
        t->child[0] = left_part();
        if (t->child[0] == NULL)
        {
            syntaxError("Missing left_part in left_expression!", current);
            return NULL;
        }
    }
    else if (isMatched(LSBRACKET, current))
    {
        t->child[0] = match(LSBRACKET, current);
        t->child[1] = expression();
        if (t->child[1] == NULL)
        {
            syntaxError("Unmatched in left_expression!", current);
            return NULL;
        }
        TreeNode *p = t->child[2] = match(RSBRACKET, current);
        while (isMatched(LSBRACKET, current))
        {
            TreeNode *q = match(LSBRACKET, current);
            TreeNode *r = expression();
            if (r == NULL)
            {
                syntaxError("Unmatched in left_expression!", current);
                return NULL;
            }
            TreeNode *s = match(RSBRACKET, current);
            if (s == NULL)
            {
                syntaxError("Missing ] in Array in left_expression!", current);
                return NULL;
            }
            p->sibling = q;
            q->sibling = r;
            r->sibling = s;
            p = s;
        }
        p->sibling = left_part();
        if (p->sibling == NULL)
        {
            syntaxError("Missing left_part in left_expression!", current);
            return NULL;
        }
    }
    else if (isMatched(LPAREN, current))
    {
        t->child[0] = simple_expression2();
        if (t->child[0] == NULL)
        {
            syntaxError("Unmatched in left_expression!", current);
            return NULL;
        }
    }
    return t;
}

TreeNode *parser::left_part()
{ //left_part-> "=" expression | simple_expression1
    TreeNode *t = newStmtNode(LEFT_PART, current);

    if (isMatched(ASSIGN, current))
    {
        t->child[0] = match(ASSIGN, current);
        t->child[1] = expression();
        if (t->child[1] == NULL)
        {
            syntaxError("Missing Expression in Assign Situation in left_part!", current);
            return NULL;
        }
    }
    else if (isMatchedAddop(current) || isMatchedMulop(current) || isMatchedRelop(current))
    {
        t->child[0] = simple_expression1();
        if (t->child[0] == NULL)
        {
            syntaxError("Unmatched Simple_expression1 in left_part!", current);
            return NULL;
        }
    }
    return t;
}

TreeNode *parser::simple_expression1()
{
    //simple_expression1 -> additive_expression1 [relop additive_expression]
    TreeNode *t = newStmtNode(SIMPLE_EXPRESSION1, current);
    TreeNode *p = t->child[0] = additive_expression1();
    if (p == NULL)
    {
        syntaxError("Unmatched additive_expression1 in simple_expression1", current);
        return NULL;
    }
    if (isMatchedRelop(current))
    {
        t->child[1] = relop();
        if (t->child[1] == NULL)
        {
            syntaxError("Unmatched relop in simple_expression1!", current);
            return NULL;
        }
        t->child[2] = additive_expression();
        if (t->child[2] == NULL)
        {
            syntaxError("Unmatched additive_expression in simple_expression1!", current);
            return NULL;
        }
    }
    return t;
}

TreeNode *parser::additive_expression1()
{
    //additive_expression1 -> term1 { addop term }
    TreeNode *t = newStmtNode(ADDITIVE_EXPRESSION1, current);
    TreeNode *p = t->child[0] = term1();
    if (p == NULL)
    {
        syntaxError("Unmatched term1 in additive_expression1!", current);
        return NULL;
    }
    while (isMatchedAddop(current))
    {
        TreeNode *q = addop();
        TreeNode *r = term();
        if (r == NULL)
        {
            syntaxError("Unmatched term in additive_expression1!", current);
            return NULL;
        }
        p->sibling = q;
        q->sibling = r;
        p = r;
    }
    return t;
}

TreeNode *parser::term1()
{
    //term1->{mulop factor}
    TreeNode *t = newStmtNode(TERM1, current);
    if (isMatchedMulop(current))
    {
        TreeNode *p = t->child[0] = mulop();
        TreeNode *q = t->child[1] = factor();
        if (q == NULL)
        {
            syntaxError("Unmatched factor in term1!", current);
            return NULL;
        }
        while (isMatchedMulop(current))
        {
            TreeNode *r = mulop();
            TreeNode *s = factor();
            if (s == NULL)
            {
                syntaxError("Unmatched term in term1!", current);
                return NULL;
            }
            q->sibling = r;
            r->sibling = s;
            q = s;
        }
    }
    return t;
}

TreeNode *parser::simple_expression2()
{
    //simple_expression2 -> additive_expression2 [relop additive_expression]
    TreeNode *t = newStmtNode(SIMPLE_EXPRESSION2, current);
    TreeNode *p = t->child[0] = additive_expression2();
    if (p == NULL)
    {
        syntaxError("Unmatched additive_expression2 in simple_expression2", current);
        return NULL;
    }
    if (isMatchedRelop(current))
    {
        t->child[1] = relop();
        if (t->child[1] == NULL)
        {
            syntaxError("Unmatched relop in simple_expression2!", current);
            return NULL;
        }
        t->child[2] = additive_expression();
        if (t->child[2] == NULL)
        {
            syntaxError("Unmatched additive_expression in simple_expression2!", current);
            return NULL;
        }
    }
    return t;
}

TreeNode *parser::additive_expression2()
{
    //additive_expression2 -> term2 { addop term }
    TreeNode *t = newStmtNode(ADDITIVE_EXPRESSION2, current);
    TreeNode *p = t->child[0] = term2();
    if (p == NULL)
    {
        syntaxError("Unmatched term2 in additive_expression2!", current);
        return NULL;
    }
    while (isMatchedAddop(current))
    {
        TreeNode *q = addop();
        TreeNode *r = term();
        if (r == NULL)
        {
            syntaxError("Unmatched term in additive_expression2!", current);
            return NULL;
        }
        p->sibling = q;
        q->sibling = r;
        p = r;
    }
    return t;
}

TreeNode *parser::term2()
{
    //term2->"(" args ")"{mulop factor}
    TreeNode *t = newStmtNode(TERM2, current);
    t->child[0] = match(LPAREN, current);
    if (t->child[0] == NULL)
    {
        syntaxError("missing ( in function call in term2!", current);
        return NULL;
    }
    t->child[1] = args();
    if (t->child[1] == NULL)
    {
        syntaxError("Unmatched arg in function call in term2!", current);
        return NULL;
    }
    TreeNode *p = t->child[2] = match(RPAREN, current);
    if (t->child[2] == NULL)
    {
        syntaxError("missing ) in function call in term2!", current);
        return NULL;
    }
    while (isMatchedMulop(current))
    {
        TreeNode *q = mulop();
        TreeNode *r = factor();
        if (r == NULL)
        {
            syntaxError("Unmatched factor in term2!", current);
            return NULL;
        }
        p->sibling = q;
        q->sibling = r;
        p = r;
    }
    return t;
}

TreeNode *parser::simple_expression3()
{
    //simple_expression3-> additive_expression3 [relop additive_expression]
    TreeNode *t = newStmtNode(SIMPLE_EXPRESSION3, current);
    TreeNode *p = t->child[0] = additive_expression3();
    if (p == NULL)
    {
        syntaxError("Unmatched additive_expression2 in simple_expression2", current);
        return NULL;
    }
    if (isMatchedRelop(current))
    {
        t->child[1] = relop();
        if (t->child[1] == NULL)
        {
            syntaxError("Unmatched relop in simple_expression1!", current);
            return NULL;
        }
        t->child[2] = additive_expression();
        if (t->child[2] == NULL)
        {
            syntaxError("Unmatched additive_expression in simple_expression1!", current);
            return NULL;
        }
    }
    return t;
}

TreeNode *parser::additive_expression3()
{
    //additive_expression3 -> term3 { addop term }
    TreeNode *t = newStmtNode(ADDITIVE_EXPRESSION3, current);
    TreeNode *p = t->child[0] = term3();
    if (p == NULL)
    {
        syntaxError("Unmatched term3 in additive_expression2!", current);
        return NULL;
    }
    while (isMatchedAddop(current))
    {
        TreeNode *q = addop();
        TreeNode *r = term();
        if (r == NULL)
        {
            syntaxError("Unmatched term in additive_expression2!", current);
            return NULL;
        }
        p->sibling = q;
        q->sibling = r;
        p = r;
    }
    return t;
}

TreeNode *parser::term3()
{
    //term3->factor3 {mulop factor}
    TreeNode *t = newStmtNode(TERM3, current);
    TreeNode *m = t->child[0] = factor3();
    if (m == NULL)
    {
        syntaxError("Unmatched factor3 in term3!", current);
        return NULL;
    }
    if (isMatchedMulop(current))
    {
        TreeNode *p = t->child[1] = mulop();
        TreeNode *q = t->child[2] = factor();
        if (q == NULL)
        {
            syntaxError("Unmatched factor in term3!", current);
            return NULL;
        }
        while (isMatchedMulop(current))
        {
            TreeNode *r = mulop();
            TreeNode *s = factor();
            if (s == NULL)
            {
                syntaxError("Unmatched factor in term3!", current);
                return NULL;
            }
            q->sibling = r;
            r->sibling = s;
            q = s;
        }
    }
    return t;
}

TreeNode *parser::factor3()
{
    //factor3->"("expression")"| "NUM"
    TreeNode *t = newStmtNode(FACTOR3, current);
    if (isMatched(LPAREN, current))
    {
        t->child[0] = match(LPAREN, current);
        t->child[1] = expression();
        if (t->child[1] == NULL)
        {
            syntaxError("Unmatched expression in factor3!", current);
            return NULL;
        }
        t->child[2] = match(RPAREN, current);
        if (t->child[2] == NULL)
        {
            syntaxError("expected ) in factor3!", current);
            return NULL;
        }
    }
    else if (isMatched(NUM, current))
    {
        t->child[0] = match(NUM, current);
    }
    else
    {
        syntaxError("Unexpected Token in factor3!", current);
        return NULL;
    }
    return t;
}

TreeNode *parser::simple_expression()
{
    //simple_expression -> additive_expression [relop additive_expression]
    TreeNode *t = newStmtNode(SIMPLE_EXPRESSION, current);
    TreeNode *p = t->child[0] = additive_expression();
    if (p == NULL)
    {
        syntaxError("Unmatched additive_expression in simple_expression", current);
        return NULL;
    }
    if (isMatchedRelop(current))
    {
        t->child[1] = relop();
        if (t->child[1] == NULL)
        {
            syntaxError("Unmatched relop in simple_expression!", current);
            return NULL;
        }
        t->child[2] = additive_expression();
        if (t->child[2] == NULL)
        {
            syntaxError("Unmatched additive_expression in simple_expression!", current);
            return NULL;
        }
    }
    return t;
}

TreeNode *parser::additive_expression()
{
    //additive-expression -> term { addop term }
    TreeNode *t = newStmtNode(ADDITIVE_EXPRESSION, current);
    TreeNode *p = t->child[0] = term();
    if (p == NULL)
    {
        syntaxError("Unmatched term in additive_expression!", current);
        return NULL;
    }
    while (isMatchedAddop(current))
    {
        TreeNode *q = addop();
        TreeNode *r = term();
        if (r == NULL)
        {
            syntaxError("Unmatched term in additive_expression!", current);
            return NULL;
        }
        p->sibling = q;
        q->sibling = r;
        p = r;
    }
    return t;
}

TreeNode *parser::term()
{
    //term->factor{mulop factor}
    TreeNode *t = newStmtNode(TERM, current);
    TreeNode *m = t->child[0] = factor();
    if (m == NULL)
    {
        syntaxError("Unmatched factor in term!", current);
        return NULL;
    }
    if (isMatchedMulop(current))
    {
        TreeNode *p = t->child[1] = mulop();
        TreeNode *q = t->child[2] = factor();
        if (q == NULL)
        {
            syntaxError("Unmatched factor in term!", current);
            return NULL;
        }
        while (isMatchedMulop(current))
        {
            TreeNode *r = mulop();
            TreeNode *s = factor();
            if (s == NULL)
            {
                syntaxError("Unmatched factor in term!", current);
                return NULL;
            }
            q->sibling = r;
            r->sibling = s;
            q = s;
        }
    }
    return t;
}

TreeNode *parser::factor()
{
    //factor->"ID" factor_part|factor3
    TreeNode *t = newStmtNode(FACTOR, current);
    if (isMatched(ID, current))
    {
        t->child[0] = match(ID, current);
        t->child[1] = factor_part();
        if (t->child[1] == NULL)
        {
            syntaxError("Unmatched factor_part in factor!", current);
            return NULL;
        }
    }
    else if (isMatched(LPAREN, current) || isMatched(NUM, current))
    {
        t->child[0] = factor3();
        if (t->child[0] == NULL)
        {
            syntaxError("Unmatched factor3 in factor!", current);
            return NULL;
        }
    }
    else
    {
        syntaxError("Unexpected Token in factor!", current);
        return NULL;
    }
    return t;
}

TreeNode *parser::factor_part()
{
    //factor_part->[ "[" expression "]" ]|"(" args ")"
    TreeNode *t = newStmtNode(FACTOR_PART, current);
    if (isMatched(LSBRACKET, current))
    {
        t->child[0] = match(LSBRACKET, current);
        t->child[1] = expression();
        if (t->child[1] == NULL)
        {
            syntaxError("Unmatched expression in factor_part!", current);
            return NULL;
        }
        TreeNode *p = t->child[2] = match(RSBRACKET, current);
        if (t->child[2] == NULL)
        {
            syntaxError("Missing ] in factor_part!", current);
            return NULL;
        }
        while (isMatched(LSBRACKET, current))
        {
            TreeNode *q = match(LSBRACKET, current);
            TreeNode *r = expression();
            if (r == NULL)
            {
                syntaxError("Unmatched in left_expression!", current);
                return NULL;
            }
            TreeNode *s = match(RSBRACKET, current);
            if (s == NULL)
            {
                syntaxError("Missing ] in Array in left_expression!", current);
                return NULL;
            }
            p->sibling = q;
            q->sibling = r;
            r->sibling = s;
            p = s;
        }
        p->sibling = left_part();
        if (p->sibling == NULL)
        {
            syntaxError("Missing left_part in left_expression!", current);
            return NULL;
        }
    }
    else if (isMatched(LPAREN, current))
    {
        t->child[0] = match(LPAREN, current);
        t->child[1] = args();
        if (t->child[1] == NULL)
        {
            syntaxError("Unmatched args in factor_part!", current);
            return NULL;
        }
        t->child[2] = match(RPAREN, current);
        if (t->child[2] == NULL)
        {
            syntaxError("Missing ) in factor_part!", current);
            return NULL;
        }
    }
    return t;
}

treeNode *parser::relop()
{
    TreeNode *t = newStmtNode(RELOP, current);
    if (isMatchedRelop(current))
    {
        t->child[0] = match(lexicalUnitList[current].type, current);
    }
    else
    {
        syntaxError("Unexpected token in relop!", current);
        return NULL;
    }
    return t;
}

treeNode *parser::addop()
{
    TreeNode *t = newStmtNode(ADDOP, current);
    if (isMatchedAddop(current))
    {
        t->child[0] = match(lexicalUnitList[current].type, current);
    }
    else
    {
        syntaxError("Unexpected token in addop!", current);
        return NULL;
    }
    return t;
}

treeNode *parser::mulop()
{
    TreeNode *t = newStmtNode(MULOP, current);
    if (isMatchedMulop(current))
    {
        t->child[0] = match(lexicalUnitList[current].type, current);
    }
    else
    {
        syntaxError("Unexpected token in mulop!", current);
        return NULL;
    }
    return t;
}

TreeNode *parser::args()
{
    //args->[arg-list]
    TreeNode *t = newStmtNode(ARGS, current);
    if (isMatched(ID, current) || isMatched(LPAREN, current) || isMatched(NUM, current))
    {
        t->child[0] = arg_list();
    }
    return t;
}

TreeNode *parser::arg_list()
{
    //arg-list-> expression{, expression}
    TreeNode *t = newStmtNode(ARG_LIST, current);
    TreeNode *p = t->child[0] = expression();
    while (isMatched(COMMA, current))
    {
        TreeNode *q = match(COMMA, current);
        TreeNode *s = expression();
        if (s == NULL)
        {
            syntaxError("Unmatched expression in arg_list!", current);
            return NULL;
        }
        p->sibling = q;
        q->sibling = s;
        p = s;
    }
    return t;
}

void parser::SyntaxAnalysis(ifstream &cminusProgram, bool printFlag)
{
    TreeNode *t;
    LexicalAnalysis(cminusProgram, true);
    t = program();
    cout<<"=======Syntax Analysis========="<<endl;
    if (printFlag && printTreeFlag)
    {
        printTree(t);
        fstream output;
        getGraph(t, output);
    }
    
}

static void printSpaces(void)
{
    for (int i = 0; i < indentno; i++)
        cout << " ";
}

void parser::printTree(TreeNode *tree)
{
    INDENT;
    while (tree != NULL)
    {
        printSpaces();
        cout << getNode(tree) << endl;
        for (int i = 0; i < MAXCHILDREN; i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
    UNINDENT;
}

string parser::getNode(TreeNode *tree)
{
    if (tree->nodekind == NONTERMINAL)
        return NonTerminalDict[tree->kind.stmt];
    else //(tree->nodekind == TERMINAL)
        return TerminalDict[tree->kind.token];
}

void parser::getLink(TreeNode *tree, fstream &fs)
{
    fs << to_string(tree->label) << "[label=" << getNode(tree) << "];\n";
    if (tree->nodekind == TERMINAL)
    {
        if (tree->kind.token == NUM)
            fs << "Node" << to_string(terminalcount) << "[label=\"" << tree->attr.val << "\"];\n";
        else
            fs << "Node" << to_string(terminalcount) << "[label=\"" << tree->attr.name << "\"];\n";
        fs << to_string(tree->label) << "->"
           << "Node" << to_string(terminalcount) << ";\n";
        terminalcount++;
    }
    for (int i = 0; i < MAXCHILDREN; i++)
    {
        if (tree->child[i] != NULL)
        {
            TreeNode *p = tree->child[i];
            fs << to_string(p->label) << "[label=" << getNode(p) << "];\n";
            fs << to_string(tree->label) << "->" << to_string(p->label) << ";\n";
            TreeNode *q;
            while (p->sibling != NULL)
            {
                q = p->sibling;
                fs << to_string(tree->label) << "->" << to_string(q->label) << ";\n";
                getLink(q, fs);
                p = q;
            }
            getLink(tree->child[i], fs);
        }
    }
}

void parser::getGraph(TreeNode *root, fstream &fs)
{
    fs.open("output.dot", fstream::out);
    fs << "digraph G{\n";
    getLink(root, fs);
    fs << "\n}";
    fs.close();
    system("dot -Tpng .\\output.dot -o out.png");
    system(".\\output.dot");
    system(".\\out.png");
}
