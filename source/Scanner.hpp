#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "Lex.hpp"
#include "Table_id.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string.h>

namespace translator
{

    class Scanner
    {
        enum State {H, IDENT, NUMB, COM, ALE, DELIM, NEQ};
        State state;
        static std::vector<std::string> TW;
        static std::vector<std::string> TD;
        static std::vector<type_of_lex> words;
        static std::vector<type_of_lex> dlms;
        std::fstream file;
        char c;
        std::string buf;

        void clear();
        void add();
        int look(const std::string buf, std::vector<std::string> list);
        void gc();
    
    public:
        Lex get_lex(std::string& record_name); // дописать записи
        Scanner(const std::string& program);
    };
}


#endif // ! _SCANNER_H_
