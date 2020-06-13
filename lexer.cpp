#include "lexer.h"

vector<lexicalUnit> lexicalUnitList;
struct
{
    string str;
    TokenType tok;
} reservedWords[MAXRESERVED] //保留字
    = {{"if", IF}, {"else", ELSE}, {"int", INT}, {"return", RETURN}, {"void", VOID}, {"while", WHILE}};

static TokenType searchReservedWord(string &originalString)
{
    for (int i = 0; i < MAXRESERVED; i++)
    {
        if (originalString.compare(reservedWords[i].str) == 0)
            return reservedWords[i].tok;
    }
    return ID;
}

string originalString;
int lineNo = 1;
TokenType getToken(ifstream &cminusProgram)
{
    int tokenStringIndex = 0;
    TokenType currentToken;
    StateType state = START;
    originalString = "";
    bool saveFlag;
    while (state != DONE)
    {
        char c = cminusProgram.peek();
        //cout<<c<<endl;
        saveFlag = true;
        switch (state)
        {
        case START:
            if (isdigit(c))
                state = INNUM;
            else if (isalpha(c))
                state = INID;
            else if (c == '=')
                state = INASSIGN;
            else if (c == '<')
                state = INSMALL;
            else if (c == '>')
                state = INLARGE;
            else if (c == '/')
                state = INSLASH;
            else if (c == '!')
                state = INNOEQUAL;
            else
            {
                state = DONE;
                switch (c)
                {
                case EOF:
                    saveFlag = false;
                    currentToken = ENDFILE;
                    break;
                case ' ':
                case '\t':
                    currentToken = BLANK;
                    state = DONE;
                    break;
                case '\n':
                    lineNo++;
                    currentToken = BLANK;
                    state = DONE;
                    break;
                case '+':
                    currentToken = PLUS;
                    break;
                case '-':
                    currentToken = MINUS;
                    break;
                case '*':
                    currentToken = TIMES;
                    break;
                case '(':
                    currentToken = LPAREN;
                    break;
                case ')':
                    currentToken = RPAREN;
                    break;
                case '{':
                    currentToken = LBRACKET;
                    break;
                case '}':
                    currentToken = RBRACKET;
                    break;
                case '[':
                    currentToken = LSBRACKET;
                    break;
                case ']':
                    currentToken = RSBRACKET;
                    break;
                case ',':
                    currentToken = COMMA;
                    break;
                case ';':
                    currentToken = SEMI;
                    break;
                default:
                    currentToken = ERROR;
                    break;
                }
            }
            break;
        case INNUM:
            if (!isdigit(c))
            {
                saveFlag = false;
                state = DONE;
                currentToken = NUM;
            }
            break;
        case INID:
            if (!isalpha(c))
            {
                saveFlag = false;
                state = DONE;
                currentToken = ID;
            }
            else
            {
                state = INID;
            }
            break;
        case INSLASH:
            if (c == '*')
            {
                state = INCOMMENT1;
                currentToken = LCOMMENT;
            }
            else
            {
                saveFlag = false;
                state = DONE;
                currentToken = OVER;
            }
            break;
        case INASSIGN:
            if (c == '=')
            {
                state = DONE;
                currentToken = EQUAL;
            }
            else
            {
                saveFlag = false;
                state = DONE;
                currentToken = ASSIGN;
            }
            break;
        case INNOEQUAL:
            if (c == '=')
            {
                state = DONE;
                currentToken = NOTEQUAL;
            }
            else
            {
                state = DONE;
                currentToken = ERROR;
            }
            break;
        case INSMALL:
            if (c == '=')
            {
                state = DONE;
                currentToken = NOLARGE;
            }
            else
            {
                saveFlag = false;
                state = DONE;
                currentToken = SMALL;
            }
            break;
        case INLARGE:
            if (c == '=')
            {
                state = DONE;
                currentToken = NOSMALL;
            }
            else
            {
                saveFlag = false;
                state = DONE;
                currentToken = LARGE;
            }
            break;
        case INCOMMENT1:
            if (c == '*')
            {
                state = INCOMMENT2;
            }
            break;
        case INCOMMENT2:
            if (c == '/')
            {
                state = DONE;
                currentToken = RCOMMENT;
            }
            else if (c == '*')
            {
                state = INCOMMENT2;
            }
            else
            {
                state = INCOMMENT1;
            }
            break;
        case DONE:
            break;
        default:
            cout << "Lexer Bug: State= " << state << endl;
            state = DONE;
            currentToken = ERROR;
            break;
        }
        if (saveFlag)
        {
            char newchar = cminusProgram.get();
            originalString += c;
        }
        if (state == DONE)
        {
            if (currentToken == ID)
                currentToken = searchReservedWord(originalString);
            if (currentToken != RCOMMENT&&currentToken!=BLANK)
            {
                lexicalUnit tmp(originalString, currentToken, lineNo);
                //cout<<tmp.originstring<<" "<<tmp.type<<endl;
                lexicalUnitList.push_back(tmp);
            }
        }
    }
    return currentToken;
}

void printLexicalUnit(lexicalUnit l)
{
    switch (l.type)
    {
    case IF:
    case ELSE:
    case RETURN:
    case WHILE:
    case VOID:
    case INT:
        cout << l.lineNo << ": Reserved Word:" << l.originstring << endl;
        break;

    case ID:
        cout << l.lineNo << ":ID,Name=" << l.originstring << endl;
        break;
    case NUM:
        cout << l.lineNo << ":NUM,Val=" << l.originstring << endl;
        break;

    case SMALL:
    case LARGE:
    case NOLARGE:
    case NOSMALL:
    case ASSIGN:
    case EQUAL:
    case NOTEQUAL:
    case PLUS:
    case MINUS:
    case TIMES:
    case OVER:
    case LPAREN:
    case RPAREN:
    case LBRACKET:
    case RBRACKET:
    case LSBRACKET:
    case RSBRACKET:
    case SEMI:
    case COMMA:
        cout << l.lineNo << ":" << l.originstring << endl;
        break;

    case LCOMMENT:
        break;
    case RCOMMENT:
        break;
    case BLANK:
        break;

    case ENDFILE:
        cout << "EOF" << endl;
        break;
    case ERROR:
        cout << "Error:" << l.originstring << endl;
        break;
    default:
        cout << "Unkown token:" << l.originstring << endl;
        break;
    }
}

void LexicalAnalysis(ifstream &cminusProgram, bool printFlag)
{
    TokenType t = getToken(cminusProgram);
    while (t != ENDFILE)
    {
        t = getToken(cminusProgram);
    }
    if (printFlag)
    {
        cout<<"=======Lexical Analysis========="<<endl;
        for (int i = 0; i < lexicalUnitList.size(); i++)
        {
            printLexicalUnit(lexicalUnitList[i]);
        }
    }
}