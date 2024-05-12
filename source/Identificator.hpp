#ifndef _IDENTIFICATOR_H_
#define _IDENTIFICATOR_H_

#include <string>
#include "Lex.hpp"

namespace translator 
{
    class Identificator 
    {
        std::string name;
        bool declare;
        bool assign;
        type_of_lex type;
        int value;
    public:
        Identificator();
        std::string get_name() const;
        void set_name(std::string name);
        bool get_declare() const;
        void set_declare();
        bool get_assign() const;
        void set_assign();
        type_of_lex get_type() const;
        void set_type(type_of_lex type);
        int get_value() const;
        void set_value(int value);
    };
}

#endif // ! _IDENTIFICATOR_H_