#ifndef _EXECUTER_H_
#define _EXECUTER_H_

#include "Lex.hpp"
#include "Poliz.hpp"
#include "Stack.hpp"
#include "Scanner.hpp"
#include <iostream>
#include <string>

namespace translator
{
    class Executer
    {
        Lex cur;
    public:
        void execute(Poliz& prog, Table_id& TID);
    };
}

#endif // ! _EXECUTER_H_