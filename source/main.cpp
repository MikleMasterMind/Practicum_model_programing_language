#include "Interpretator.hpp"
#include "Lex.hpp"
#include <iostream>

int main(int argc, void** argv) {
    try {
        translator::Interpretator magic((char*)argv[1]);
        magic.intetretate();
        return 0;
    }
    catch(char c) {
        std::cout << "unexpected symbol" << c << std::endl;
        return 1;
    }
    catch(translator::Lex l) {
        std::cout << "unexpected lexeme" << l << std::endl;
        return 1;
    }
    catch(const char* s) {
        std::cout << s << std::endl;
        return 1;
    }
}