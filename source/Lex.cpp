#include "Lex.hpp"

translator::Lex::Lex(translator::type_of_lex type, int n) : lex_t(type), lex_id(n)
{
}

translator::type_of_lex translator::Lex::get_type() const
{
    return lex_t;
}

int translator::Lex::get_id() const
{
    return lex_id;
}

std::ostream& translator::operator<<(std::ostream &s, translator::Lex lex)
{
    s << '(' << lex.lex_t << ',' << lex.lex_id << ");";
    return s;
}
