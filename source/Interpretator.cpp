#include "Interpretator.hpp"

translator::Interpretator::Interpretator(std::string program) : parser(program)
{
}

void translator::Interpretator::intetretate()
{
    parser.analyze();
    executer.execute(parser.prog, parser.get_tid());
}
