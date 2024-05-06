#include "Table_id.hpp"

translator::Identificator translator::Table_id::operator[](int index) const
{
    if (index < container.size()) 
        return container[index];
    else
        throw "out_of_range";
}

translator::Identificator &translator::Table_id::operator[](int index)
{
    if (index < container.size()) 
        return container[index];
    else
        throw "out_of_range";
}

int translator::Table_id::push(std::string lex_name) 
{
    for (int i = 0; i < container.size(); ++i)
        if (container[i].get_name() == lex_name)
            return i;
    Identificator buf;
    buf.set_name(lex_name);
    container.push_back(buf);
    return container.size() - 1;
}
