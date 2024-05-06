#include"Poliz.hpp"

void translator::Poliz::push_lex(translator::Lex lex)
{
    container.push_back(lex);
}

void translator::Poliz::push_lex(translator::Lex lex, int place)
{
    if (place >= container.size())
        throw "Poliz_out_of_range";
    container[place] = lex;
}

void translator::Poliz::blank()
{
    push_lex(Lex());
}

int translator::Poliz::get_carrier()
{
    return container.size();
}

translator::Lex &translator::Poliz::operator[](int index)
{
    if (index >= container.size())
        throw "Poliz_out_of_range";
    return container[index];
}

void translator::Poliz::print()
{
    for (auto a : container)
        std::cout << a << ' ';
    std::cout << std::endl;
}
