#include "Identificator.hpp"

translator::Identificator::Identificator() : declare(false), assign(false), value(NULL)
{    
}

std::string translator::Identificator::get_name() const
{
    return name;
}

void translator::Identificator::set_name(std::string name)
{
    this->name = name;
}

 
bool translator::Identificator::get_declare() const
{
    return declare;
}

void translator::Identificator::set_declare()
{
    declare = true;
}
 
bool translator::Identificator::get_assign() const
{
    return assign;
}
 
void translator::Identificator::set_assign()
{
    assign = true;
}

translator::type_of_lex translator::Identificator::get_type() const
{
    return type;
}

void translator::Identificator::set_type(translator::type_of_lex type)
{
    this->type = type;
}

void* translator::Identificator::get_value() const
{
    return value;
}

void translator::Identificator::set_value(const void* value)
{
    if (type == LEX_BOOL)
        this->value = new bool(*(bool*)value);
    else if (type == LEX_INT)
        this->value = new int(*(int*)value);
    else if (type == LEX_RECORD)
        // ...
        this->value = NULL;
    else 
        throw "bad_type";
}