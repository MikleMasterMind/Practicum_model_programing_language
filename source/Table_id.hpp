#ifndef _TABLE_ID_H_
#define _TABLE_ID_H_

#include <vector>
#include <string>
#include "Identificator.hpp"

namespace translator 
{

    class Table_id
    {
        std::vector<Identificator> container;
    public:
        Identificator operator[](int index) const;
        Identificator& operator[](int index);
        int push(std::string lex_name);
    };
}

#endif // ! _TABLE_ID_H_