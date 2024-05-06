#ifndef _LEX_H_
#define _LEX_H_

#include <iostream>

namespace translator 
{

    enum type_of_lex
    {
        LEX_NULL,
        LEX_PROGRAM,
        LEX_BEGIN,
        LEX_END,
        LEX_VAR,
        LEX_ASSIGN,
        LEX_INT,
        LEX_BOOL,
        LEX_RECORD,
        LEX_WRITE,
        LEX_READ,
        LEX_FIN,
        LEX_COLON, // ":"
        LEX_LPAREN,
        LEX_RPAREN,
        LEX_EQ,
        LEX_NEQ,
        LEX_LEQ,
        LEX_LSS,
        LEX_GEQ,
        LEX_GTR,
        LEX_PLUS,
        LEX_MINUS,
        LEX_TIMES,
        LEX_SLASH,
        LEX_SEMICOLON, // ";"
        LEX_COMMA, // ","
        LEX_NUM,
        LEX_ID,
        LEX_WHILE,
        LEX_DO,
        LEX_IF,
        LEX_THEN,
        LEX_ELSE,
        LEX_AND, 
        LEX_OR,
        LEX_NOT,
        LEX_TRUE,
        LEX_FALSE,
        POLIZ_LABEL,
        POLIZ_ADDRESS,
        POLIZ_GO,
        POLIZ_FGO
    };

    class Lex
    {
        type_of_lex lex_t;
        int lex_id;

    public:
        Lex(type_of_lex type = LEX_NULL, int n = 0);
        type_of_lex get_type() const;
        int get_id() const;
        friend std::ostream& operator<<(std::ostream& s, Lex lex);
    };


}

#endif // ! _LEX_H_