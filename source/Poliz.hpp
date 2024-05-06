#ifndef _POLIZ_H_
#define _POLIZ_H_

#include"Lex.hpp"
#include<vector>
#include<iostream>

namespace translator
{
    class Poliz
    {
        std::vector<Lex> container;
    public:
        void push_lex(Lex lex);
        void push_lex(Lex lex, int place);
        void blank();
        int get_carrier();
        Lex& operator[](int index);
        void print();
    };
}

#endif // ! _POLIZ_H_