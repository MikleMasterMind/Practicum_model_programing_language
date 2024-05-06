#ifndef _INTERPRETATOR_H_
#define _INTERPRETATOR_H_

#include "Parser.hpp"
#include "Executer.hpp"
#include <string>

namespace translator
{
    class Interpretator
    {
        Parser parser;
        Executer executer;
    public:
        Interpretator(const char* program);
        void intetretate();
    };
}

#endif // ! _INTERPRETATOR_H_