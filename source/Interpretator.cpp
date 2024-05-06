#include "Interpretator.hpp"

translator::Interpretator::Interpretator(const char* program) : parser(program)
{
}

void translator::Interpretator::intetretate()
{
    parser.analyze();
    executer.execute(parser.prog, parser.get_tid());
}
