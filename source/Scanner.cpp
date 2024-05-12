#include "Scanner.hpp"

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

std::vector<std::string> translator::Scanner::TW = 
{
    "",
    "and",
    "begin",
    "end",
    "bool",
    "do",
    "else",
    "if",
    "false",
    "int",
    "not",
    "or",
    "program",
    "read",
    "then",
    "true",
    "var",
    "while",
    "write",
    "record"
};

std::vector<std::string> translator::Scanner::TD =
{
    "",
    "@",
    ";",
    ",",
    ":",
    ":=",
    "(",
    ")",
    "=",
    "<",
    ">",
    "+",
    "-",
    "*",
    "/",
    "<=",
    "!=",
    ">="
};

std::vector<translator::type_of_lex> translator::Scanner::words =
{
    LEX_NULL,
    LEX_AND,
    LEX_BEGIN,
    LEX_END,
    LEX_BOOL,
    LEX_DO,
    LEX_ELSE,
    LEX_IF,
    LEX_FALSE,
    LEX_INT,
    LEX_NOT,
    LEX_OR,
    LEX_PROGRAM,
    LEX_READ,
    LEX_THEN,
    LEX_TRUE,
    LEX_VAR,
    LEX_WHILE,
    LEX_WRITE,
    LEX_RECORD,
    LEX_NULL
};

std::vector<translator::type_of_lex> translator::Scanner::dlms =
{
    LEX_NULL,
    LEX_FIN,
    LEX_SEMICOLON,
    LEX_COMMA,
    LEX_COLON,
    LEX_ASSIGN,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_EQ,
    LEX_LSS,
    LEX_GTR,
    LEX_PLUS,
    LEX_MINUS,
    LEX_TIMES,
    LEX_SLASH,
    LEX_LEQ,
    LEX_NEQ,
    LEX_GEQ,
    LEX_NULL
};

void translator::Scanner::clear() 
{
    buf.clear();
}

void translator::Scanner::add()
{
    buf.push_back(c);
}

int translator::Scanner::look(std::string buf, std::vector<std::string> list)
{
    for (int i = 0; i < list.size(); ++i)
        if (buf == list[i])
            return i;
    return 0;
}

void translator::Scanner::gc()
{
    file.read(&c, 1);
    if (file.eof())
        c = '@';
}

translator::Lex translator::Scanner::get_lex(translator::Table_id &TID, std::string &record_name)
{
    int d, j;
    state = H;
    while (true) {
        switch(state) {
            case H:
                if (c == ' ' || c == '\n' || c== '\r' || c == '\t')
                    gc();
                else if (isalpha(c)) {
                    clear();
                    add();
                    gc();
                    state = IDENT;
                }
                else if (isdigit(c)) {
                    d = c - '0';
                    gc();
                    state = NUMB;
                }
                else if (c == '{') {
                    gc();
                    state = COM;
                }
                else if ( c== ':' || c== '<' || c== '>') {
                    clear();
                    add();
                    gc();
                    state = ALE;
                }
                else if (c == '@') {
                    return Lex(LEX_FIN);
                }
                else if (c == '!') {
                    clear();
                    add();
                    gc();
                    state = NEQ;
                }
                else {
                    state = DELIM;
                }
                break;
            case IDENT:
                if (isalpha(c) || isdigit(c) || c == '.') {
                    add();
                    gc();
                }
                else {
                    #ifdef DEBUG
                    std::cout << '|' << buf << '|' << std::endl;
                    #endif
                    record_name = buf;
                    if (j = look(buf, TW)) {
                        record_flag = words[j] == LEX_RECORD;
                        return Lex(words[j], j);
                    }
                    else {
                        if (!record_flag) // don't save record name as identificator
                            j = TID.push(buf);
                        return Lex(LEX_ID, j);
                    }
                }
                break;
            case NUMB:
                if (isdigit(c)) {
                    d = d * 10 + (c - '0');
                    gc();
                }
                else {
                    #ifdef DEBUG
                    std::cout << '|' << d << '|' << std::endl;
                    #endif
                    return Lex(LEX_NUM, d);
                }
                break;
            case COM:
                if (c == '}') {
                    gc();
                    state = H;
                }
                else if (c == '@' || c == '{') {
                    throw c;
                }
                else {
                    gc();
                }
                break;
            case ALE:
                if (c == '=') {
                    add();
                    gc();
                    j = look(buf, TD);
                    #ifdef DEBUG
                    std::cout << '|' << buf << '|' << std::endl;
                    #endif
                    return Lex(dlms[j], j);
                }
                else {
                    j = look(buf, TD);
                    #ifdef DEBUG
                    std::cout << '|' << buf << j << '|' << std::endl;
                    #endif
                    return Lex(dlms[j], j);
                }
                break;
            case NEQ:
                if ( c == '=' ) {
                    add();
                    gc();
                    j = look(buf, TD);
                    #ifdef DEBUG
                    std::cout << '|' << "!=" << ' ' << j << '|' << std::endl;
                    #endif
                    return Lex(LEX_NEQ, j);
                }
                else {
                    throw '!';
                }
                break;
            case DELIM:
                clear();
                add();
                if (j = look(buf, TD)) {
                    gc();
                    #ifdef DEBUG
                    std::cout << '|' << buf << '|' << std::endl;
                    #endif
                    return Lex(dlms[j], j);
                }
                else {
                    throw c;
                }
                break;
        } // end switch
    }
}   

translator::Scanner::Scanner(const std::string &program) : record_flag(false)
{
    file.open(program);
    state = H;
    clear();
    gc();
}
