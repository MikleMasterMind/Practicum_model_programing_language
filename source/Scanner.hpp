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
    const int BUF_SIZE = 800;

    class Scanner
    {
        enum State {H, IDENT, NUMB, COM, ALE, DELIM, NEQ};
        State state;
        static char* TW[];
        static char* TD[];
        static type_of_lex words[];
        static type_of_lex dlms[];
        std::fstream file;
        char c;
        char buf[BUF_SIZE];
        int buf_top;

        void clear();
        void add();
        int look(const char* buf, char** list);
        void gc();
    
    public:
        Lex get_lex(Table_id& TID); // дописать записи
        Scanner(const std::string& program);
    };
}


#endif // ! _SCANNER_H_
